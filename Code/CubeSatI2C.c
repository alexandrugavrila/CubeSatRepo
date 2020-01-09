/*
 * i2c.c
 *
 *  Created on: Apr 22, 2019
 *      Author: alexa
 */

#include "CubeSat.h"

uint32_t I2CReceive(uint16_t slave_addr, uint8_t reg)
{
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);               // Specify that we are writing (a register address) to the slave device
    I2CMasterDataPut(I2C0_BASE, reg);                                  // Put register ID on the bus
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);           // Send control byte and register address byte to slave device
    while(I2CMasterBusy(I2C0_BASE));                                   // Wait for the MCU to finish transaction
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);                // Specify that we are going to read from slave device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);        // Send control byte and read from the register we specified
    while(I2CMasterBusy(I2C0_BASE));                                   // Wait for the MCU to finish transaction
    return I2CMasterDataGet(I2C0_BASE);                                // Return data pulled from the specified register
}

void I2CSend(uint16_t slave_addr, uint8_t reg, uint8_t data)
{
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);               // Specify that we are writing (a register address) to the slave device
    I2CMasterDataPut(I2C0_BASE, reg);                                  // Put register ID on the bus
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);      // Send control byte and register address on the bus
    while(I2CMasterBusy(I2C0_BASE));                                   // Wait for the MCU to finish the transaction
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);               // Specify that we are going to write to the register we specified
    I2CMasterDataPut(I2C0_BASE, data);                                 // Put the data to be written on the bus
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);  // Send control byte and the data on the bus
    while(I2CMasterBusy(I2C0_BASE));                                   // Wait for the MCU to finish the transaction
}

void readGyro(uint8_t * X_rot_H, uint8_t * Y_rot_H, uint8_t * Z_rot_H)
{
    *X_rot_H =  I2CReceive(GYRO_SLAVE_ADDR, X_HIGH);        // Read the X_H register on the gyro into the appropriate variable
    //*X_rot_L =  I2CReceive(GYRO_SLAVE_ADDR, X_LOW);       // Read the X_L register on the gyro into the appropriate variable
    *Y_rot_H =  I2CReceive(GYRO_SLAVE_ADDR, Y_HIGH);        // Read the Y_H register on the gyro into the appropriate variable
    //*Y_rot_L =  I2CReceive(GYRO_SLAVE_ADDR, Y_LOW);       // Read the Y_L register on the gyro into the appropriate variable
    *Z_rot_H =  I2CReceive(GYRO_SLAVE_ADDR, Z_HIGH);        // Read the Z_H register on the gyro into the appropriate variable
    //*Z_rot_L =  I2CReceive(GYRO_SLAVE_ADDR, Z_LOW);       // Read the Z_L register on the gyro into the appropriate variable
}

void writeGyro(uint8_t subAdd, uint8_t value)
{
    I2CSend(GYRO_SLAVE_ADDR, subAdd, value);
}

void readMag(uint8_t * X_mag, uint8_t * Y_mag, uint8_t * Z_mag)
{
    *X_mag = I2CReceive(MAG_SLAVE_ADDR, 0x29);
    *Y_mag = I2CReceive(MAG_SLAVE_ADDR, 0x2b);
    *Z_mag = I2CReceive(MAG_SLAVE_ADDR, 0x2d);
}

void writeMag(uint8_t subAdd, uint8_t value)
{
    I2CSend(MAG_SLAVE_ADDR, subAdd, value);
}

void readRW(uint8_t * result, uint8_t subAdd)
{
    *result = I2CReceive(MOTOR_SLAVE_ADDR, subAdd);
}

void writeRW(uint8_t subAdd, uint8_t value)
{
    I2CSend(MOTOR_SLAVE_ADDR, subAdd, value);
}

int readRWspeed()
{
    uint16_t speed1;
    uint16_t speed2;

    readRW(&speed1, 0x11);
    readRW(&speed2, 0x12);

    uint16_t speed = (speed1<<8) | speed2;
    speed = speed / 10;
    return speed;
}
