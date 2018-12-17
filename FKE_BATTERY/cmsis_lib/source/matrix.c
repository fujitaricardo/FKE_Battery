#include <matrix.h>

void    LUdecomposition(matrix *a, matrix **l, matrix **u);
matrix* solver(matrix *a, matrix *b);

/**
  * @brief  Cria uma matriz do tipo matrix*.
  * @param  Numero de linhas e colunas da matriz.
  * @retval Ponteiro da matriz criada.
 **/
matrix* makeMatrix(short row, short col) {
	matrix	*out;

	if (row == 0 || col == 0)
		return NULL;

	out = (matrix*) malloc(sizeof(matrix));
	
	if (out == NULL)
		return NULL;
    
	out->row = row;
	out->col = col;
	out->data = (double*) malloc(sizeof(double) * row * col);
    
	if (out->data == NULL)
		return NULL;
    
	memset(out->data, 0.0, row * col * sizeof(double));
	
	return out;
}

/**
  * @brief  Libera a struct matrix* alocada.
  * @param  Endereco da matriz.
  * @retval None.
 **/
void freeMatrix(matrix *m) {
	if (m != NULL) 
	{
		if (m->data != NULL) 
		{
			free(m->data);
			m->data = NULL;
		}

		free(m);
		m = NULL;
	}
}

/**
  * @brief  Formata e Imprime os valores da matriz.
  * @param  Endereco da matriz.
  * @retval None.
 **/
/*void printMatrix(matrix *m) {
	double	*ptr = m->data;      

	printf("%d %d\n", m->row, m->col);

	for (short i = 0; i < m->row; i++) 
	{
		for (short j = 0; j < m->col; j++)
			printf("%9.10f", (*ptr++));
		printf("\n");
	}
}*/

/**
  * @brief  Converte matrix* em uma forma visivel no debug.
  * @param  O endereco inicial da matriz P[][], numero de linhas e colunas e o endereco da matriz. 
  * @retval None.
 **/
void struct2matrix(double *p, short *rowp, short *colp, matrix *a){
	double	*ptr = a->data;
    
	*rowp = a->row;
	*colp = a->col;
	
	for (short i = 0; i < (a->row * a->col); i++)
		p[i] = *(ptr++);
}

/**
  * @brief  Converte matriz A[][] em matrix*.
  * @param  Endereco da matriz, numero de linhas e colunas.
  * @retval None.
 **/
matrix* matrix2struct(double *A, short rowp, short colp){
	matrix	*m = makeMatrix(rowp, colp);
	double	*ptr = m->data;

	m->row = rowp;
	m->col = colp;

	for (short i = 0; i < (rowp * colp); i++)
		*(ptr++) = A[i];
	
	return m;
}

/**
  * @brief  Busca o elemeto desejado da matriz.
  * @param  Endereco do elemeto, endereco da matrix, linha e coluna do elemento.
  * @retval None.
 **/
void elementMatrix(double *res, matrix *m, short r_el, short col_el){
	/*
	if ((r_el - 1) > m->row || (col_el - 1) > m->col)
		return (double) NULL;
	*/
	
	double	*ptrM = m->data;
	
	for (short i = 0; i < m->row; i++)
	{
		for (short j = 0; j < m->col; j++)
		{
			if (i == (r_el - 1) && j == (col_el - 1))
				*res = *ptrM;
			
			ptrM++;
		}
	}
}

/**
  * @brief  Ve se ha elementos negativos na matriz (matriz de covariancia n pode ter elemento negativo).
  * @param  Endereco de teste e endereco da matriz.
  * @retval None.
 **/
/*
void isAllPositiveElements(bool *test, matrix *m){
	double	*pM = m->data;

	*test = true;
	
	for (short i = 0; i < (m->row * m->col); i++)
	{
		if (*(pM++) < 0.0)
			*test = false;
	}
}
*/

/**
  * @brief  Busca o maximo valor do vetor linha.
  * @param  Endereco de 'max' e endereco da matriz (vetor linha) 'b'. 
  * @retval None.
 **/
/*
void maxVector(double *max, matrix *b){
	// Creating aux and temp variables
	double temp = 0;
	
	// Copying the enter matrix in a temp one
	matrix *a = makeMatrix(b->row, b->col);
	double *ptrA = a->data;
	
	scaleMatrix(&a, b, 1.0);
	
	// BubbleSorting (descending)
	for (short c = 0; c < (a->col - 1); c++)
	{
		for (short d = 0; d < (a->col - c - 1); d++)
		{
			if (ptrA[d] < ptrA[d + 1])
			{
				temp = ptrA[d];
				ptrA[d] = ptrA[d + 1];
				ptrA[d + 1] = temp;
			}
		}
	}		
	
	// Attributing the max value
	*max = ptrA[0];	
	
	freeMatrix(a);
}
*/

/**
  * @brief  Busca o minimo valor do vetor linha.
  * @param  Endereco de 'min' e endereco da matriz (vetor linha) 'b'. 
  * @retval None.
 **/
/*
void minVector(double *min, matrix *b){
	// Creating aux and temp variables
	double temp = 0;
	
	// Copying the enter matrix in a temp one
	matrix *a = makeMatrix(b->row, b->col);
	double *ptrA = a->data;
	
	scaleMatrix(&a, b, 1.0);
	
	// BubbleSorting (ascending)
	for (short c = 0; c < (a->col - 1); c++)
	{
		for (short d = 0; d < (a->col - c - 1); d++)
		{
			if (ptrA[d] > ptrA[d + 1])
			{
				temp = ptrA[d];
				ptrA[d] = ptrA[d + 1];
				ptrA[d + 1] = temp;
			}
		}
	}
	
	// Attributing the min value
	*min = ptrA[0];		
	
	freeMatrix(a);
}
*/

/**
  * @brief  Gera uma matriz de 1s.
  * @param  Endereco do ponteiro da matriz 'one'.
  * @retval None.
 **/
void oneMatrix(matrix **one){
	double *ptrOne = (*one)->data;
	for(short i = 0; i < ((*one)->row * (*one)->col); i++)
		*(ptrOne++) = 1;
}

/**
  * @brief  Gera uma matriz (deve ser quadrada!).
  * @param  Endereco do ponteiro da matriz identidade ('eye').
  * @retval None.
 **/
void eyeMatrix(matrix **eye) {
	double *ptr = (*eye)->data;
	for (short i = 0; i < ((*eye)->row)*((*eye)->row); i++){
		*ptr = 0;
		ptr += 1;
	}
	ptr = (*eye)->data;
	for (short i = 0; i < (*eye)->row; i++) 
	{
		*ptr = 1.0;
		ptr += (*eye)->row + 1;
	}
}

/**
  * @brief  Realiza a transposicao da matrix (A -> AT).
  * @param  Endereco do ponteiro da matriz transposta e o endereco da matriz a ser transposta.
  * @retval None.
 **/
void transposeMatrix(matrix **mt, matrix *m) {
	double	*ptrM = m->data, *ptrMT;

	for (short i = 0; i < m->row; i++) 
	{
		ptrMT = &(*mt)->data[i];
		for (short j = 0; j < m->col; j++) 
		{
			*ptrMT = *(ptrM++);
			ptrMT += (*mt)->col;
		}
	}
}

/**
  * @brief  Calcula o determinante da matrix pelo metodo LU.
  * @param  Endereco do determinante e o da matriz.
  * @retval None.
 **/
/*void detMatrix(double *det, matrix *m){
	//if (m->row != m->col)
	//	return (double)NULL;

	*det = 1.0;
    matrix* l = NULL;
    matrix* u = NULL;

    LUdecomposition(m, &l, &u);

    for (short i = 0; i < m->col; i++)
        *det *= u->data[i * m->col + i];

    freeMatrix(l);
    freeMatrix(u);
}
*/

/**
  * @brief  Soma os elementos da diagonal principal.
  * @param  Endereco do traco e da matriz.
  * @retval None.
 **/
/*
void traceMatrix(double *trace, matrix *m) {
	
	//if (m->row != m->col)
	//	return (double)NULL;

	short	size;
	double	*ptr = m->data;
	*trace = 0.0;

	if (m->row < m->col) 
		size = m->row;
	else 
		size = m->col;

	for (short i = 0; i < size; i++) 
	{
		(*trace) += *ptr;
		ptr += m->col + 1;
	}
}*/

/**
  * @brief  Calcula o inverso de cada elemento da matrix ou vetor.
  * @param  Endereco do ponteiro da matriz 'inv' de elemento invertido e o endereco da matriz.
  * @retval None.
 **/
void inverseElement(matrix **inv, matrix *m){
	double	*ptrM = m->data, *ptrInv = (*inv)->data;
	
	for (short i = 0; i < (m->row * m->col); i++)
	{
		if (*ptrM != 0)
			*(ptrInv++) = 1 / *(ptrM++);	
	}
}

/**
  * @brief  Realiza a soma de um escalar a todos elementos da matriz.
  * @param  Endereco do ponteiro da matriz 'addSub', o endereco da matriz e escalar. 
  * @retval None.
 **/
void sumSubScalarMatrix(matrix **addSub, matrix *m, double scalar) {
	double	*ptrM = m->data, *ptrAddSub = (*addSub)->data;

	for (short i = 0; i < (m->col * m->row); i++) 
		*(ptrAddSub++) = *(ptrM++) + scalar;
}

/**
  * @brief  Realiza o produto de uma escalar com uma matriz (kM = k*Mij p/ todo ij).
  * @param  Endereco do ponteiro da matriz 'scale', o endereco da matriz e o escalar
  * @retval None.
 **/
void scaleMatrix(matrix **scale, matrix *m, double scalar) {
	double	*ptrM = m->data, *ptrScale = (*scale)->data;

	for (short i = 0; i < (m->col * m->row); i++) 
		*(ptrScale++) = *(ptrM++) * scalar;
}

/**
  * @brief  Realiza a soma de duas matrizes (A + B = sum(AB)).
  * @param  Endereco do ponteiro da matriz 'sum' e os enderecos das matrizes.
  * @retval None.
 **/
void sumMatrix(matrix **sum, matrix *a, matrix *b){
	/*
	if (a->row != b->row || a->col != b->col)
		return NULL;
	*/
	
	double	*ptrA = a->data, *ptrB = b->data, *ptrSum = (*sum)->data;

	for (short i = 0; i < (a->row * a->col); i++) 
		*(ptrSum++) = *(ptrA++) + *(ptrB++); 
}

/**
  * @brief  Realiza a subtracao de duas matrizes (A-B = sub(AB)).
  * @param  Endereco do ponteiro da matriz 'sub' e os enderecos das matrizes.
  * @retval None.
 **/
void subMatrix(matrix **sub, matrix *a, matrix *b){
	/*
	if (a->row != b->row || a->col != b->col)
		return NULL;
	*/
	
	double	 *ptrA = a->data, *ptrB = b->data, *ptrSub = (*sub)->data;

	for (short i = 0; i < (a->row * a->col); i++) 
		*(ptrSub++) = *(ptrA++) - *(ptrB++); 
}

/**
  * @brief  Realiza o produto de duas matrizes (A*B = AB).
  * @param  Endereco do ponteiro da matriz 'mul' e os enderecos das matrizes.
  * @retval None.
 **/
void multiplyMatrix(matrix **mul, matrix *a, matrix *b){
	/*
	if (a->col != b->row)
		return NULL;
	*/
	
	double	*ptrA, *ptrB, *ptrMul = (*mul)->data;
	
	for (short i = 0; i < a->row; i++) 
	{
		for (short j = 0; j < b->col; j++) 
		{
			ptrA = &a->data[i * a->col];
			ptrB = &b->data[j];
			*ptrMul = 0;
			for (short k = 0; k < a->col; k++) 
			{
				*ptrMul += *ptrA * *ptrB;
				ptrA++;
				ptrB += b->col;
			}
			ptrMul++;
		}
	}
}

/**
  * @brief  Realiza a decomposicao LU da matriz (A = L * U).
  * @param  Endereco da matriz 'a' ser decomposta e modificada, como 'a' nao e criada dentro da 
  * funcao nao e necessario passar o endereco do ponteiro como 'l' e 'u'.
  * @retval None (a will be modified).
 **/
void LUdecomposition(matrix* a, matrix** l, matrix** u){
	int i, j, k;
	double* ptrA;
	double* ptrL;
	double* ptrU;
	double sum;

	*l = makeMatrix(a->row, a->col);
	*u = makeMatrix(a->row, a->col);

	    // Step 1: Assign 1 to the diagonal of the lower matrix.
	ptrL = (*l)->data;
	for (i = 0; i < a->col; i++) 
	{
		*ptrL = 1.0;
		ptrL += a->col + 1;
	}

	    // Step 2
	for (j = 0; j < a->col; j++) 
	{
	        // Part A: Solve for the upper matrix.
		for (i = 0; i <= j; i++) 
		{
			sum = 0.0;
			for (k = 0; k < i; k++) 
				sum += (*l)->data[i * a->col + k] * (*u)->data[k * a->col + j];

			(*u)->data[i * a->col + j] = a->data[i * a->col + j] - sum;
		}
		        // Part B: Solve fpr the lower matrix
		for (i = j + 1; i < a->col; i++) 
		{
			sum = 0.0;
			for (k = 0; k < j; k++) 
				sum += (*l)->data[i * a->col + k] * (*u)->data[k * a->col + j];
		
			(*l)->data[i * a->col + j] = 1.0 / (*u)->data[j * a->col + j] * (a->data[i * a->col + j] - sum);
		}
	}

	return;
}

/**
  * @brief  Resolve um sistema liner.
  * @param  Matrizes do sistema linear (Ax + B).
  * @retval Matriz com os valores do sistema resolvido.
 **/
matrix* solver(matrix* a, matrix* b){
	int i, j, k;
	double sum;
	matrix *l = NULL;
	matrix *u = NULL;
	matrix *y;
	matrix *x;
	double* row;

	LUdecomposition(a, &l, &u);
	y = makeMatrix(a->row, 1);
	x = makeMatrix(b->row, b->col);

	for (k = 0; k < b->col; k++) 
	{
	    // Perform backward subsitituion with L
	    // L * y = B_k
		for (i = 0; i < a->row; i++) 
		{
			row = l->data + i * a->col;
			sum = 0;
			for (j = 0; j < i; j++) 
				sum += y->data[j] * (*row++);
		
			y->data[i] = (b->data[i * b->col + k] - sum) / *row;
		}
		        // Perform backward subsitituion again with U
		        // U * x = y
		for (i = a->row - 1; i >= 0; i--) 
		{
			row = u->data + i * a->col + (a->col - 1);
			sum = 0;
			for (j = a->col - 1; j > i; j--) 
				sum += x->data[j * b->col + k] * (*row--);
			
			x->data[i * b->col + k] = (y->data[i] - sum) / *row;
		}
	}

	freeMatrix(l);
	freeMatrix(u);
	freeMatrix(y);
	return x;
}

/**
  * @brief  Realiza a inversa de uma matriz n x n.
  * @param  Endereco do ponteiro da matriz 'inv' e o endereco da matriz.
  * @retval None.
 **/
void inverseMatrix(matrix **inv, matrix *a) {
	/*
	if (a->row != a->col)
		return NULL;
	*/
	
	matrix *eye = makeMatrix(a->row, a->col);

	eyeMatrix(&eye);
	
	*inv = solver(a, eye);

	freeMatrix(eye);
}

void inverse2x2Matrix(matrix **inv, matrix *a){
	double a11 = *(a->data);
	double a12 = *(a->data + 1);
	double a21 = *(a->data + 2);
	double a22 = *(a->data + 3);
	double det = a11*a22 - a12*a21;

	*((*inv)->data) = a22/det;
	*((*inv)->data + 1) = -a21/det;
	*((*inv)->data + 2) = -a12/det;
	*((*inv)->data + 3) = a11/det;
}

/**
  * @brief  Calcula o produto escalar de vetores e matrizes.
  * @param  Endereco do ponteiro da matriz 'dot' e o endereco das matrizes.
  * @retval None.
 **/
/*
void dotMatrix(matrix **dot, matrix *a, matrix *b) {
	//if (a->row != b->row || a->col != b->col)
	//	return NULL;
	
	double	*ptrOut, *ptrA, *ptrB;
	
	if (a->col == 1 && a->row == 1)
		*dot = makeMatrix(1, 1);
	else
		*dot = makeMatrix(1, a->col);
	
	ptrOut = (*dot)->data;		
	
	for (short i = 0; i < a->col; i++) 
	{
		ptrA = &a->data[i];
		ptrB = &b->data[i];
		for (short j = 0; j < a->row; j++) 
			*ptrOut = *ptrOut + *(ptrA + a->col * j) * *(ptrB + a->col * j);
		
		if (a->row == 1 || a->col == 1)
			;
		else
			ptrOut++;
	}
}*/

/**
  * @brief  Calcula o produto vetorial de matrizes 3 x 3.
  * @param  End. do ponteiro para a matriz 'cross' e das matrizes.
  * @retval None.
 **/
/*
void crossMatrix3x3(matrix **cross, matrix *a, matrix *b){
	
	//if (a->row != b->row || a->col != b->col || a->row != 3 || a->col != 3 || b->row != 3 || b->col != 3)
	//	return NULL;


	double *ptrA = a->data, *ptrB = b->data, *ptrCross = (*cross)->data;
	
	//c(1,  :) = a(2,  :).*b(3,  :) - a(3,  :).*b(2,  :);
	*(ptrCross + 0) = *(ptrA + a->col + 0) * *(ptrB + 2*a->col + 0) - *(ptrA + 2*a->col + 0) * *(ptrB + a->col + 0);
	*(ptrCross + 1) = *(ptrA + a->col + 1) * *(ptrB + 2*a->col + 1) - *(ptrA + 2*a->col + 1) * *(ptrB + a->col + 1);
	*(ptrCross + 2) = *(ptrA + a->col + 2) * *(ptrB + 2*a->col + 2) - *(ptrA + 2*a->col + 2) * *(ptrB + a->col + 2);
	
	//c(2,  :) = a(3,  :).*b(1,  :) - a(1,  :).*b(3,  :);	
	*(ptrCross + a->col + 0) = *(ptrA + 2*a->col + 0) * *(ptrB + 0) - *(ptrA + 0) * *(ptrB + 2*a->col + 0);
	*(ptrCross + a->col + 1) = *(ptrA + 2*a->col + 1) * *(ptrB + 1) - *(ptrA + 1) * *(ptrB + 2*a->col + 1);	
	*(ptrCross + a->col + 2) = *(ptrA + 2*a->col + 2) * *(ptrB + 2) - *(ptrA + 2) * *(ptrB + 2*a->col + 2);
	
	//c(3,  :) = a(1,  :).*b(2,  :) - a(2,  :).*b(1,  :);
	*(ptrCross + 2*a->col + 0) = *(ptrA + 0) * *(ptrB + a->col + 0) - *(ptrA + a->col + 0) * *(ptrB + 0);
	*(ptrCross + 2*a->col + 1) = *(ptrA + 1) * *(ptrB + a->col + 1) - *(ptrA + a->col + 1) * *(ptrB + 1);
	*(ptrCross + 2*a->col + 2) = *(ptrA + 2) * *(ptrB + a->col + 2) - *(ptrA + a->col + 2) * *(ptrB + 2);
}*/

/**
  * @brief  Calcula o produto vetorial de vetores coluna. So permitido 3 x 1.
  * @param  End. do ponteiro para o vetor 'cross' e dos vetores.
  * @retval None.
 **/
/*void crossVector(matrix **cross, matrix *a, matrix *b){

	//if (a->col != 1 || b->col != 1)
	//	return NULL;

	//if (a->row > 3 || b->row > 3)
	//	return NULL;
	

	double	*ptrA = a->data, *ptrB = b->data, *ptrCross = (*cross)->data;
	
	*(ptrCross + 0) = *(ptrA + 1) * *(ptrB + 2) - *(ptrA + 2) * *(ptrB + 1);
	*(ptrCross + 1) = *(ptrA + 2) * *(ptrB + 0) - *(ptrA + 0) * *(ptrB + 2);
	*(ptrCross + 2) = *(ptrA + 0) * *(ptrB + 1) - *(ptrA + 1) * *(ptrB + 0);
}*/

/**
  * @brief  Calcula a norma de um vetor coluna ou linha.
  * @param  Endereco da variavel 'norm' e do vetor.
  * @retval None.
 **/
/*
void norm(double *norm, matrix *vec){
	double	*ptrVec = vec->data, sum = 0.0;

	for (short i = 0; i < (vec->row * vec->col); i++)
		sum = sum + (*ptrVec * (*ptrVec++));
	
	
	//if (sum < 0)
	//	return (double) NULL;

	 *norm = sqrt(sum);	
}*/

/**
  * @brief  Calcula o vetor coluna ou linha normalizado.
  * @param  Endereco do ponteiro do vetor normalizado ('normalized') e endereco do vetor a ser normalizado.
  * @retval None.
 **/
/*
void normVector(matrix **normalized, matrix *vec){
	//if (0 == norm(vec))
	//	return NULL;
	
	//if (vec->row != 1 && vec->col != 1)
	//	return NULL;
	
	double value = 0;
	
	norm(&value, vec);
	
	scaleMatrix(&(*normalized), vec, 1.0 / value);
}
*/
