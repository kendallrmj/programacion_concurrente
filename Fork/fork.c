#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int *glob_var;

typedef struct matrixes {
    int** matrixA;
    int** matrixB;
    int** matrixC;    
    int N;
} matrixes;

matrixes *glob_matrixes;
int rowC;



void printMatrix(int** matrix, int N){
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++){
            printf ("%d ", matrix[i][j]);
        }
        printf ("\n");
	}
    printf ("\n");
}

void multiplyRowAColumnB(){
    matrixes m2;
    m2.matrixA = malloc(sizeof(int *) * glob_matrixes->N);
    m2.matrixB = malloc(sizeof(int *) * glob_matrixes->N);
    m2.matrixC = malloc(sizeof(int *) * glob_matrixes->N);
    m2.N = glob_matrixes->N;
	int i = rowC++;
	for (int j = 0; j < glob_matrixes->N; j++){
    	for (int k = 0; k < glob_matrixes->N; k++){
            m2.matrixA[j][k] = glob_matrixes->matrixA[j][k];
            m2.matrixB[j][k] = glob_matrixes->matrixB[j][k];
            m2.matrixC[i][j] += glob_matrixes->matrixA[i][k] * glob_matrixes->matrixB[k][j];
        }
    }
    *glob_matrixes=m2;
    printMatrix(glob_matrixes->matrixA,glob_matrixes->N);
    printMatrix(glob_matrixes->matrixB,glob_matrixes->N);
    printMatrix(glob_matrixes->matrixC,glob_matrixes->N);
    //;
}


int main(void){
    //Crea matrices y asigna valores aleatorios a matrices A y B
    int N =4;
    matrixes m;
    m.matrixA = malloc(sizeof(int *) * N);
    m.matrixB = malloc(sizeof(int *) * N);
    m.matrixC = malloc(sizeof(int *) * N); 
    m.N = N;
    int pids[N];
    //m.rowC = 0;
    for (int i = 0; i < N; i++) {
        m.matrixA[i] = malloc(sizeof(int) * N);
        m.matrixB[i] = malloc(sizeof(int) * N);
        m.matrixC[i] = malloc(sizeof(int) * N);
        for (int j = 0; j < N; j++) {
            m.matrixA[i][j] = rand()%50;
            m.matrixB[i][j] = rand()%50;
            m.matrixC[i][j] = 0;
        }
    }

    glob_matrixes = mmap(NULL, sizeof *glob_matrixes, PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   
    *glob_matrixes = m;
    for (i = 0; i < N; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            printf("Error with creating process\n");
            return 2;
        }
        if (pids[i] == 0) {
            // proceso hijo
            multiplyRowAColumnB();
            //exit(EXIT_SUCCESS);
            return 0;
        }else{
            wait(NULL);
    //        printMatrix(glob_matrixes->matrixA,N);
    //        printMatrix(glob_matrixes->matrixB,N);
    //        printMatrix(glob_matrixes->matrixC,N);
            munmap(glob_matrixes, sizeof *glob_matrixes);
        }
    }
    return 0;
}