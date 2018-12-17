#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _matrix 
{
	short	row;
	short	col;
	double	*data;
} matrix;

/*
* Por definicao os enderecos de retorno sao do tipo matrix** e os de entrada 
* matrix*. Entretanto nao seria necessario essa forma, haja visto que as 
* matrizes nao sao criadas dentro das funcoes. (Em algumas sao, o que causaria
* certas incompatibilidades).
*/

// Cria uma matriz.
matrix* makeMatrix(short row, short col);

// Libera o espaco alocado para a matriz.
void    freeMatrix(matrix *m);

// Imprime a matriz.
//void    printMatrix(matrix *m);

// Converte matrix* para matriz em C.
void	struct2matrix(double *p, short *rowp, short *colp, matrix *a);

// Converte matriz em C para matrix*.
matrix* matrix2struct(double *A, short rowp, short colp);

// Obtem um elemento desejado da matriz.
void	elementMatrix(double *res, matrix *m, short r_el, short col_el);

// Verifica se a matriz e interamente positiva.
//void	isAllPositiveElements(bool *test, matrix *m);

// Busca o valor maximo do vetor linha.
//void	maxVector(double *max, matrix *b);

// Busca o valor minimo do vetor linha.	
//void	minVector(double *min, matrix *b);

// Cria uma matriz de uns.
void	oneMatrix(matrix **one);

// Cria uma matriz identidade (deve-se passar uma matriz quadrada!).
void	eyeMatrix(matrix **eye);

/* Operations */
// Transpoe a matriz.
void	transposeMatrix(matrix **mt, matrix *m);

// Calcula o determinante da matriz.
//void	detMatrix(double *det, matrix *m);

// Calcula o traco da matriz.
//void	traceMatrix(double *trace, matrix *m);

// Faz o inverso de cada elemento da matriz.
void	inverseElement(matrix **inv, matrix *m);

// Soma um escalar a todos os elementos da matriz.
void	sumSubScalarMatrix(matrix **addSub, matrix *m, double scalar);

// Multiplica por um escalar todos os elementos da matriz.
void	scaleMatrix(matrix **scale, matrix *m, double scalar);

// Soma a matriz 'a' com a matriz 'b', ambas devem ter os mesmo tamanho.
void	sumMatrix(matrix **sum, matrix *a, matrix *b);

// Subtrai a matriz 'a' da matriz 'b', ambas devem ter os mesmo tamanho.
void	subMatrix(matrix **sub, matrix *a, matrix *b);

// Multiplica a matriz 'a' pela matriz 'b', deve seguir a regra de multiplicacao de matriz.
void	multiplyMatrix(matrix **mul, matrix *a, matrix *b);

// Inverte uma matriz (regra de inversao).
void	inverseMatrix(matrix **inv, matrix *a);

void	inverse2x2Matrix(matrix **inv, matrix *a);

// Calcula o produto interno de duas matrizes ('a' e 'b').
//void	dotMatrix(matrix **dot, matrix *a, matrix *b);

// Calcula o produto externo de matrizes 3 x 3, 'a' e 'b'.
//void	crossMatrix3x3(matrix **cross, matrix *a, matrix *b);

// Calcula o produto externo vetorial 3 x 1 dos vetores 'a' e 'b'.
//void	crossVector(matrix **cross, matrix *a, matrix *b);

// Calcula a norma de um vetor.
//void	norm(double *norm, matrix *vec);

// Calcula o vetor normalizado.
//void	normVector(matrix **normalized, matrix *vec);
