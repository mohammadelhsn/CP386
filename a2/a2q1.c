/**
 * @file a2q1.c
 * @author Mohammad El-Hassan (elha7950@mylaurier.ca)
 * @brief
 * @version 1.0
 * @date 2025-10-09
 * @copyright Copyright (c) 2025
 */

#include <stdio.h>
#include <pthread.h>

int rows1 = 0;
int cols1 = 0;
int rows2 = 0;
int cols2 = 0;

int matrixA[10][10];
int matrixB[10][10];
int resultingMatrix[10][10];
int row_indices[10];
pthread_t threads[10];

/**
 *  why does this make sense?
 *
 *  Because in a matrix of
 * 1 2 3
 * 4 5 6
 *
 * and
 * 1 2
 * 3 4
 * 5 6
 *
 * We pass in a row, for ex 0
 * 1 2 3
 * we loop over those columns
 * and multiply column of matrix b
 * so 1 * 1 + 2*3 + 3*5 = 22
 * and 1*2 + 2*4 + 3*6 = 28
 *
 */

/**
 * @brief
 *
 * @param arg
 * @return int
 */
void *run_worker(void *arg)
{
    // pass in row
    // iterate over columns
    int row = *(int *)arg;
    for (int col = 0; col < cols2; col++)
    {
        resultingMatrix[row][col] = 0;
        for (int k = 0; k < cols1; k++)
        {
            resultingMatrix[row][col] += matrixA[row][k] * matrixB[k][col];
        }
    }
}

int main()
{

    printf("Enter number of rows and columns\n");
    scanf("%d %d", &rows1, &cols1);

    printf("Enter number of rows and columns\n");
    scanf("%d %d", &rows2, &cols2);

    // We got the numbers
    // Next we need to get the matrix values

    printf("Matrix A\n");
    for (int row = 0; row < rows1; row++)
    {
        for (int col = 0; col < cols1; col++)
        {
            scanf("%d", &matrixA[row][col]);
        }
    }

    printf("Matrix B\n");
    for (int row = 0; row < rows2; row++)
    {
        for (int col = 0; col < cols2; col++)
        {
            scanf("%d", &matrixB[row][col]);
        }
    }

    for (int i = 0; i < rows1; i++)
    {
        row_indices[i] = i;
        pthread_create(&threads[i], NULL, run_worker, &row_indices[i]);
    }

    for (int i = 0; i < rows1; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Resulting matrix:\n");
    for (int i = 0; i < rows1; i++)
    {
        for (int j = 0; j < cols2; j++)
            printf("%d ", resultingMatrix[i][j]);
        printf("\n");
    }

    return 0;
}