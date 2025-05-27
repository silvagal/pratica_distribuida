#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 10

#define NUM_THREADS 5

// Função para preencher a matriz com números aleatórios de 0 a 9
void preencherMatriz(int matriz[N][N]) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            matriz[i][j] = rand() % 10;
        }
    }
}

// Função para exibir a matriz na tela
void mostrarMatriz(int matriz[N][N]) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            printf("%4d ", matriz[i][j]);
        }
        printf("\n");
    }
}

// Multiplicação de matrizes sem paralelismo
void multiplicarSequencial(int A[N][N], int B[N][N], int C[N][N]) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            C[i][j] = 0;
            for(int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Multiplicação de matrizes com paralelismo OpenMP
void multiplicarParalela(int A[N][N], int B[N][N], int C[N][N]) {
    #pragma omp parallel for
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            C[i][j] = 0;
            for(int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
	int A[N][N], B[N][N], C_seq[N][N], C_par[N][N];

	srand(time(NULL));  // Inicializa o gerador de números aleatórios

	// Configura o número de threads
	omp_set_num_threads(NUM_THREADS);
	printf("\nDimensões das matrizes: %ix%i", N, N);
	printf("\nQtd threads: %i\n", NUM_THREADS);

	// Preenche as matrizes A e B
	preencherMatriz(A);
	preencherMatriz(B);

	// Exibe as matrizes originais
	//printf("Matriz A:\n");
	//mostrarMatriz(A);

	//printf("\nMatriz B:\n");
	//mostrarMatriz(B);

	// Multiplicação Paralela
	double inicio_par = omp_get_wtime();
	multiplicarParalela(A, B, C_par);
	double fim_par = omp_get_wtime();
	double tempo_par = fim_par - inicio_par;

	// Multiplicação Sequencial
	double inicio_seq = omp_get_wtime();
	multiplicarSequencial(A, B, C_seq);
	double fim_seq = omp_get_wtime();
	double tempo_seq = fim_seq - inicio_seq;

	// Exibe os tempos
	printf("\nTempo de execucao (Sequencial): %.6f segundos\n", tempo_seq);
	printf("Tempo de execucao (Paralelo):   %.6f segundos\n", tempo_par);

	// Exibe os resultados
	//printf("\nMatriz Resultado (Sequencial):\n");
	//mostrarMatriz(C_seq);

	//printf("\nMatriz Resultado (Paralela):\n");
	//mostrarMatriz(C_par);

	// Verifica se os resultados são iguais
	int iguais = 1;
	for(int i = 0; i < N; i++) {
    	    for(int j = 0; j < N; j++) {
        	        if(C_seq[i][j] != C_par[i][j]) {
                       iguais = 0;
            	  break;
        	        }
    	    }
	}

	if(iguais)
    	    printf("\nResultado: Matrizes iguais.\n");
	else
    	    printf("\nResultado: Matrizes diferentes.\n");
	
	return 0;
}
