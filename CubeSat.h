/*
 * CubeSat.h
 *
 *  Created on: Apr 22, 2019
 *      Author: alexa
 */

#ifndef CUBESAT_H_
#define CUBESAT_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdarg.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_i2c.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/i2c.h"
#include "driverlib/pwm.h"

// I2C addresses
#define GYRO_SLAVE_ADDR 0x69
#define MAG_SLAVE_ADDR 0x1e
#define MOTOR_SLAVE_ADDR 0x52
#define X_HIGH 0x29
#define X_LOW 0x28
#define Y_HIGH 0x2b
#define Y_LOW 0x2a
#define Z_HIGH 0x2d
#define Z_LOW 0x2c

// Define pin to LED and switch mapping.
#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3
#define SW_1      GPIO_PIN_4

#define PI 3.14159          // pi
#define rwSatThresh 65      // Saturation speed threshold for reaction wheel in RPM
#define rwRestThresh 0      // Rest speed threshold for reaction wheel in RPM
#define tumbThresh 0        // Tumbling rate threshold in DPS

// CubeSat.c
void LButtonDown(void);                                                                                         // Left button down control
void LButtonUp(void);                                                                                           // Left button up control
void detumble(int tar_RW, volatile int * curr_rotation, volatile int * curr_RW_speed);                          // Detumble target axis
void desaturate(int tar_RW, volatile int * curr_rotation, volatile int * curr_RW_speed, float * unit_or);       // Desaturate target wheel

// Init.c
void InitializeMicro();                                                           // Initialize all of the components of the TM4C123G

// VectorMath.c
void getUnitVec(float * vec, float * unit_vec);                                                    // Get a vectors unit vector
void getRotationMatrix(float * vec_a, float * vec_b, float rotation_matrix[][3]);                  // Get a rotation matrix between two vectors
void getThetas(float rotation_matrix[][3], float * roll, float * pitch, float * yaw);              // Get rotation angles from rotation matrix
void matrixMultiply(float mat1[][3],  float mat2[][3],  float res[][3], int N);                    // Matrix multiplication
void matrixAdd(float mat1[][3],  float mat2[][3],  float res[][3], int N);                         // Matrix addition
void matrixMultiplyScalar(float mat[][3],  float scalar,  float res[][3], int N);                  // Multiply a matrix by a scalar

// CubeSatADC.c
void pollADC(uint32_t * ADC0Value);                                                                // Write the adc output to its variable

// CubeSatI2C.c
uint32_t I2CReceive(uint16_t slave_addr, uint8_t reg);                                             // I2C Receive function
void I2CSend(uint16_t slave_addr, uint8_t reg, uint8_t data);                                      // I2C Send function
void readGyro(uint8_t * X_rot_H, uint8_t * Y_rot_H, uint8_t * Z_rot_H);                            // Read all values off the gyro into the appropriate variables
void writeGyro(uint8_t subAdd, uint8_t value);                                                     // Write a byte to the gyro at the given subaddress
void readMag(uint8_t *X_mag, uint8_t *Y_mag, uint8_t *Z_mag);
void writeMag(uint8_t subAdd, uint8_t value);
void readRW(uint8_t * result, uint8_t subAdd);                                                     // Read value off the motor controller from the appropriate register47
void writeRW(uint8_t subAdd, uint8_t value);                                                       // Write a byte to the motor controllers at the given subaddress
int readRWspeed();

// ReactionWheels.c
void initializeRW();                                                              // Initialize motor controller by writing to appropriate registers
void spinRWX(int tar_RW_speed);                                                   // Simulate a change of RWX duty cycle
void spinRWY(int tar_RW_speed);                                                   // Simulate a change of RWY duty cycle
void spinRWZ(int tar_RW_speed);                                                   // Simulate a change of RWZ duty cycle

// Magnetorquers.c
void initializeMag();
void magXcontrol(bool op, bool dir);                                              // Simulate controlling X magnetorquer by turning on or off red light
void magYcontrol(bool op, bool dir);                                              // Simulate controlling Y magnetorquer by turning on or off red light
void magZcontrol(bool op, bool dir);                                              // Simulate controlling Z magnetorquer by turning on or off red light

#endif /* CUBESAT_H_ */
