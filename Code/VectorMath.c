/*
 * VectorMath.c
 *
 *  Created on: Apr 22, 2019
 *      Author: alexa
 */

#include "CubeSat.h"

void getUnitVec(float * vec, float * unit_vec)
{
    float quotient = sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]));
    unit_vec[0] = vec[0] / quotient;
    unit_vec[1] = vec[1] / quotient;
    unit_vec[2] = vec[2] / quotient;
    return;
}


void getRotationMatrix(float * vec_a, float * vec_b, float rotation_matrix[][3])
{
    float v[3] = {0, 0, 0};
    float c;
    float v_x[3][3] = {{0, 0, 0},{0, 0, 0},{0, 0, 0}};
    float I[3][3] = {{1, 0, 0},{0, 1, 0},{0, 0, 1}};
    float v_x_2[3][3] = {{0, 0, 0},{0, 0, 0},{0, 0, 0}};
    float temp1[3][3] = {{0, 0, 0},{0, 0, 0},{0, 0, 0}};
    float temp2[3][3] = {{0, 0, 0},{0, 0, 0},{0, 0, 0}};

    // v is the cross product of a and b
    v[0] = (vec_a[1] * vec_b[2]) - (vec_a[2] * vec_b[1]);
    v[1] = (vec_a[2] * vec_b[0]) - (vec_a[0] * vec_b[2]);
    v[2] = (vec_a[0] * vec_b[1]) - (vec_a[1] * vec_b[0]);
    // c is the dot product of a and b
    c = (vec_a[0] * vec_b[0]) + (vec_a[1] * vec_b[1]) + (vec_a[2] * vec_b[2]);
    // v_x is the skew-symmetric cross-product of v
    v_x[0][0] = 0;
    v_x[0][1] = -1 * v[2];
    v_x[0][2] = v[1];
    v_x[1][0] = v[2];
    v_x[1][1] = 0;
    v_x[1][2] = -1 * v[0];
    v_x[2][0] = -1 * v[1];
    v_x[2][1] = v[0];
    v_x[2][2] = 0;

    matrixMultiply(v_x, v_x, v_x_2, 3);                         // Calculate v_x squared
    matrixAdd(I, v_x, temp1, 3);                                // Add the identity matrix to v_x and store in temp1
    matrixMultiplyScalar(v_x_2, 1/(1+c), temp2, 3);             // Multiply v_x_2 and (1-c)/s^2 and store in temp2
    matrixAdd(temp1, temp2, rotation_matrix, 3);                // Add the two terms in temp1 and temp2, this should be the final rotation matrix between vec_a and vec_b
    return;
}

void getThetas(float rotation_matrix[][3], float * roll, float * pitch, float * yaw)
{
    *roll = atan2(rotation_matrix[1][0], rotation_matrix[0][0]) * (180 / PI);
    *pitch = atan2(rotation_matrix[2][0] * -1, sqrt(pow(rotation_matrix[2][1], 2) + pow(rotation_matrix[2][2], 2))) * (180 / PI);
    *yaw = atan2(rotation_matrix[2][0], rotation_matrix[2][2]) * (180 / PI);
}

void matrixMultiply(float mat1[][3],  float mat2[][3],  float res[][3], int N)
{
    int i, j, k;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            res[i][j] = 0;
            for (k = 0; k < N; k++)
            {
                res[i][j] += mat1[i][k] *  mat2[k][j];
            }
        }
    }
}

void matrixAdd(float mat1[][3],  float mat2[][3],  float res[][3], int N)
{
    int i, j = 0;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            res[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

void matrixMultiplyScalar(float mat[][3],  float scalar,  float res[][3], int N)
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            res[i][j] = mat[i][j] * scalar;
        }
    }
}



