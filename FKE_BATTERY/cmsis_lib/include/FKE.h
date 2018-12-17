#include "stm32f4xx_conf.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "matrix.h"

//Constante de Euler para calculo de termo exp
#define EULER 2.718281828459046
//Valor calibrado de offset do sensor ACS712
#define BYTE_V0 2100

//Parametros da bateria
#define BATT_A 0.9933
#define BATT_B 2.666429650697716
#define BATT_V0 11.0294
#define BATT_K 0.014848679627948
#define BATT_Q 9
#define BATT_R 0.003

//Parametros FKE
#define DT 0.0002777777777777778
#define STD_I 0.1
#define STD_V 1
#define X10 5
#define X20 1
#define P10 25
#define P20 10
#define Q1 0
#define Q2 0

//Estados da maquina
typedef enum State{
	waitStart = 1,
	readSensors,
	runFKE,
	sendData,
	idle,
	openCircuit
}State;

/*-------------------
 *	Funcoes
 -------------------*/
//Inicializa os LEDs da placa
void BoardLED_Init(void);
//Inicializa GPIO do rele
void Switch_Init(void);
//Inicializa TIM2 com interrupcao de 1Hz
void TIM2_Interrupt_Init(void);
//Desabilita interrupcao TIM2
void TIM2_Interrupt_Disable(void);
//Inicializa canais 8 e 9 do ADC1
void ADC1_Init(void);
//Inicializa PB com interrupcao EXTI rising
void PushButton_EXTI_Init(void);
//Desabilita interrupcao EXTI
void PushButton_EXTI_Disable(void);
//Inicializa UART1 com baudrate dado
void USART1_Init(uint32_t baudrate);
//Envia string pela UART1
void USART_SendString(USART_TypeDef* USARTx, volatile char *s);
//Converte float para string
void ftoa(float n, char *res, int afterpoint);

/*
PC1 - SWITCH
PB1PB6 - USART TX
PB7 - USART RX
PB0 - ADC V
PB1 - ADC I*/
