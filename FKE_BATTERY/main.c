#include "FKE.h"

State state = waitStart;

uint16_t ADC_data;
double V_read, I_read;
char str[100], str_Q[20], str_I[20], str_V[20], str_Iread[20], str_Vread[20];

double _Phi[2][2] = {{1, DT},{0, 1}};
double _xp[2][1] = {{X10},{X20}};
double _Pp[2][2] = {{P10, 0},{0, P20}};
double _H[2][2] = {{0, 0},{0, 1}};
double _Q[2][2] = {{Q1, 0},{0, Q2}};
double _R[2][2] = {{STD_V*STD_V, 0},{0, STD_I*STD_I}};

double xm1, xm2, I_est, Q_est, V_est;
matrix *Phi, *Phit, *xp, *xm, *Pp, *Pm, *K, *H, *Ht, *Q, *R;
matrix *PhiPp, *PhiPpPhit;
matrix *PmHt, *HPmHt, *HPmHtR, *inv, *y, *Hx, *yHx, *KyHx, *eye2, *KH, *IKH;

int main(void){
	SystemInit();

	Phi = matrix2struct(_Phi,2,2);
	Phit = makeMatrix(2,2);
	transposeMatrix(&Phit,Phi);
	xp = matrix2struct(_xp,2,1);
	xm = makeMatrix(2,1);
	Pp = matrix2struct(_Pp,2,2);
	Pm = makeMatrix(2,2);
	K = makeMatrix(2,2);
	H = matrix2struct(_H,2,2);
	Ht = makeMatrix(2,2);
	Q = matrix2struct(_Q,2,2);
	R = matrix2struct(_R,2,2);

	PhiPp = makeMatrix(2,2);
	PhiPpPhit = makeMatrix(2,2);

	PmHt = makeMatrix(2,2);
	HPmHt = makeMatrix(2,2);
	HPmHtR = makeMatrix(2,2);
	inv = makeMatrix(2,2);
	y = makeMatrix(2,1);
	Hx = makeMatrix(2,1);
	yHx = makeMatrix(2,1);
	KyHx = makeMatrix(2,1);
	eye2 = makeMatrix(2,2);
	eyeMatrix(&eye2);
	KH = makeMatrix(2,2);
	IKH = makeMatrix(2,2);

	BoardLED_Init();
	ADC1_Init();
	Switch_Init();
	USART1_Init(115200);
	PushButton_EXTI_Init();
	USART_SendString(USART1, "Waiting START\r\n\0");
	GPIO_WriteBit(GPIOD, GPIO_Pin_12, 1);

    while(1){
    	switch(state){
    	    case readSensors:
    	    	//Mede tensao da bateria
    	    	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);
    	    	ADC_SoftwareStartConv(ADC1);
    	    	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    	    	ADC_data = ADC_GetConversionValue(ADC1);
    	    	V_read = (ADC_data*3*4.5)/4095; //mV

    	    	//Mede corrente da bateria
    	    	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);
    	    	ADC_SoftwareStartConv(ADC1);
    	    	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    	    	ADC_data = ADC_GetConversionValue(ADC1);
    	    	if(ADC_data < BYTE_V0){
    	    		I_read = 0;	// Verifica se a corrente esta negativa
    	    	}else{
    	    		I_read = ((ADC_data - BYTE_V0)*3*2.8)/(0.185*1.8*4095);
    	    	}

    	    	state = runFKE;
    	    	if(V_read <= 8.1){ // Verifica se a descarga chegou ao fim
    	    		TIM2_Interrupt_Disable();
    	    		GPIO_WriteBit(GPIOC, GPIO_Pin_1, 0);	//Switch Relay OFF
    	    		GPIO_WriteBit(GPIOD, GPIO_Pin_14, 1);	//LED14 ON
    	    		GPIO_WriteBit(GPIOD, GPIO_Pin_12, 0);	//LED12 OFF
    	    		USART_SendString(USART1, "END\r\n\0");
    	    		state = openCircuit;
    	    	}
    	    	break;
    	    case runFKE:
    	    	//Propagacao
        		multiplyMatrix(&xm, Phi, xp); // xm = Phi*xp
        		multiplyMatrix(&PhiPp, Phi, Pp); // Phi*Pp
        		multiplyMatrix(&PhiPpPhit, PhiPp, Phit); //Phi*Pp*Phi'
        		sumMatrix(&Pm, PhiPpPhit, Q);	// Pm = Phi*Pp*Phi' + Q

        		elementMatrix(&xm1, xm, 1, 1);
        		elementMatrix(&xm2, xm, 2, 1);
        		//Calculo da Matriz H
        		*(H->data) = -BATT_K*BATT_Q*(BATT_Q+xm2)/((BATT_Q-xm1)*(BATT_Q-xm1)) - BATT_A*BATT_B*pow(EULER, -BATT_B*xm1);
        		*(H->data+1) = -(BATT_R + BATT_K/(1 - xm1/BATT_Q));
        		//Calcula y = h(x)
        		*(Hx->data) = BATT_V0 - BATT_R*xm2 - BATT_K*BATT_Q*(xm1+xm2)/(BATT_Q-xm1) + BATT_A*pow(EULER, -BATT_B*xm1);
        		*(Hx->data+1) = xm2;
        		//Pega valores medidos dos sensores
        		*(y->data) = V_read;
        		*(y->data+1) = I_read;

        		//Estimacao
        		transposeMatrix(&Ht, H);
        		multiplyMatrix(&PmHt, Pm, Ht);	// Pm*H'
        		multiplyMatrix(&HPmHt, H, PmHt);// H*Pm*H'
        		sumMatrix(&HPmHtR, HPmHt, R);	// H*Pm*H' + R
        		inverse2x2Matrix(&inv, HPmHtR);	// (H*Pm*H' + R)^-1
        		multiplyMatrix(&K, PmHt, inv);	// K = Pm*H'*(H*Pm*H' + R)^-1
        		subMatrix(&yHx, y, Hx);			// y-Hx
        		multiplyMatrix(&KyHx, K, yHx);	// K*(y-Hx)
        		sumMatrix(&xp, xm, KyHx);		// xp = xm + K*(y-Hx)
        		multiplyMatrix(&KH, K, H);		// K*H
        		subMatrix(&IKH, eye2, KH);		// I-K*H
        		multiplyMatrix(&Pp, IKH, Pm);	// Pp = (I-K*H)*Pm

        		//Limita o valor da carga entre 0 a Qmax
        		if(*(xp->data) > BATT_Q){
        			*(xp->data) = BATT_Q;
        		}else if(*(xp->data) <= 0){
        			*(xp->data) = 0;
        		}

        		//Extrai valores estimados
        		elementMatrix(&Q_est, xp, 1, 1);
        		elementMatrix(&I_est, xp, 2, 1);
        		elementMatrix(&V_est, Hx, 1, 1);
        		state = sendData;
    	    	break;
    	    case sendData:
    	    	ftoa(I_read, str_Iread, 9);
    	    	ftoa(V_read, str_Vread, 9);
    	    	ftoa(Q_est, str_Q, 9);
    	    	ftoa(I_est, str_I, 9);
    	    	ftoa(V_est, str_V, 9);
    	    	sprintf(str, "Ir:%s Vr:%s Q:%s I:%s V:%s\n\0", str_Iread, str_Vread, str_Q, str_I, str_V);
    	    	USART_SendString(USART1, str);
    	    	state = idle;
    	    	break;
    	    case idle:	break;
    	    case openCircuit: break;
    	    case waitStart: break;
    	    default: break;
    	}
    }
}

//Interrupcao da TIM2 a cada 1s
void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)){
		state = readSensors;
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

//Interrupcao do push button
void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0)) {
    	GPIO_WriteBit(GPIOC, GPIO_Pin_1, 1);	//Switch Relay ON
    	TIM2_Interrupt_Init();
        PushButton_EXTI_Disable();
        state = readSensors;
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
