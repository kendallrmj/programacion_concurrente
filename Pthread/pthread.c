#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

typedef struct matrixes {
    int** matrixA;
    int** matrixB;
    int** matrixC;    
    int N;
} matrixes;

int rowC;



void* multiplyRowAColumnB(void* arg){
    matrixes m = *((matrixes *)arg);
	int i = rowC++;
	for (int j = 0; j < m.N; j++){
    	for (int k = 0; k < m.N; k++){
    		m.matrixC[i][j] += m.matrixA[i][k] * m.matrixB[k][j];
        }
    }
}

void printMatrix(int** matrix, int N){
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++){
            printf ("%d ", matrix[i][j]);
        }
        printf ("\n");
	}
    printf ("\n");
}

int main(){
    //Recibe de entrada estandar un numero N
    int N;
    printf("Ingrese el valor de N : ");
    scanf ("%i", &N);
    int count = 0;
    //srand(time(0));
    double times[100];
    matrixes results[100];
    while(count<100){
        rowC=0;
        clock_t t;
        //Inicia tiempo
        t = clock();
        //Crea matrices y asigna valores aleatorios a matrices A y B
        matrixes m;
        m.matrixA = malloc(sizeof(int *) * N);
        m.matrixB = malloc(sizeof(int *) * N);
        m.matrixC = malloc(sizeof(int *) * N);
        m.N = N;
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

        //Declara y crea N hilos para realizar la multiplicacion de matrices
        pthread_t threads[N];
        for (int i = 0; i < N; i++) {
            int* p;
            pthread_create(&threads[i], NULL, multiplyRowAColumnB, &m);
        }

        //Espera que todos los hilos hayan finalizado
        for (int i = 0; i < N; i++){
            pthread_join(threads[i], NULL);
        }
        //Finaliza tiempo y lo guarda
        t = clock() - t;
        double timeTaken = ((double)t)/CLOCKS_PER_SEC;
        times[count]=timeTaken;

        //Guarda los resultados en una matriz
        matrixes result;
        result.matrixA = malloc(sizeof(int *) * N);
        result.matrixB = malloc(sizeof(int *) * N);
        result.matrixC = malloc(sizeof(int *) * N);
        result.N = N;
        for (int i = 0; i < N; i++) {
            result.matrixA[i] = malloc(sizeof(int) * N);
            result.matrixB[i] = malloc(sizeof(int) * N);
            result.matrixC[i] = malloc(sizeof(int) * N);
            for (int j = 0; j < N; j++) {
                result.matrixA[i][j] = m.matrixA[i][j];
                result.matrixB[i][j] = m.matrixB[i][j];
                result.matrixC[i][j] = m.matrixC[i][j];
            }
        }
        results[count]=result;

        //Libera espacio asignado
        for (int i = 0; i < N; i++) {
            free(m.matrixA[i]);
            free(m.matrixB[i]);
            free(m.matrixC[i]);
        }
        free(m.matrixA);
        free(m.matrixB);
        free(m.matrixC);       
        count++;
    }
    
    //Crea archivo para guardar los tiempos
    double sum = 0;
    double average;
    FILE* stats = fopen("Stats", "w");
    for(int i=0; i<100; i++){
        sum = sum + times[i];
        fprintf(stats,"Repeticion %d \n",i);
        fprintf(stats,"Tiempo %f s \n", times[i]);                
        fprintf(stats,"\n");                
    }

    //Crea archivo para guardar la matriz resultante
    for (int c = 0; c < 100; c++) {
        char filename[] = "Mat_";
        char strNum[3];
        sprintf(strNum, "%d", c);
        strcat(filename, strNum);

        FILE* mat_ = fopen(filename, "w");
        matrixes mTemp = results[c];

        fprintf(mat_,"Matriz A\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++){
                fprintf(mat_,"%d ", mTemp.matrixA[i][j]);
            }
            fprintf(mat_,"\n");
        }
        fprintf(mat_,"Matriz B\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++){
                fprintf(mat_,"%d ", mTemp.matrixB[i][j]);
            }
            fprintf(mat_,"\n");
        }    
        fprintf(mat_,"Matriz resultado\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++){
                fprintf(mat_,"%d ", mTemp.matrixC[i][j]);
            }
            fprintf(mat_,"\n");
        }

        fclose(mat_);
    }    
   pthread_exit(NULL);
   return 0;
}
