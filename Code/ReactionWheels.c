/*
 * ReactionWheels.c
 *
 *  Created on: Apr 22, 2019
 *      Author: alexa
 */

#include "CubeSat.h"

void initializeRW()
{
    uint8_t checkVal = 0x0;
    writeRW(0x02, 0xb6);

    while(checkVal != 0xdf){
        writeRW(0x03, 0xdf);
        readRW(&checkVal, 0x03);
    }

    writeRW(0x20, 0x06);
    writeRW(0x21, 0xbf);
    writeRW(0x22, 0xaa);
    writeRW(0x23, 0x00);
    writeRW(0x24, 0xb8);
    writeRW(0x25, 0x40);
    writeRW(0x26, 0x3f);
    writeRW(0x27, 0x06);
    writeRW(0x28, 0x11);
    writeRW(0x29, 0x58);
    writeRW(0x2a, 0x04);
    writeRW(0x2b, 0x3e);
}

void spinRWX(int tar_RW_speed)
{

    //SysCtlDelay(500000);                                      // Delay 1/10 s
    return;
}

void spinRWY(int tar_RW_speed)
{

    //SysCtlDelay(500000);                                      // Delay 1/10 s
    return;
}

void spinRWZ(int tar_RW_speed)
{
    // 100% Duty cycle = 20.4 kRPM
    // 1% duty cycle = 204 RPM
    // PD1 is PWM output for RWX
    // PD2 is dir pin

    //GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
    float dutyCycle = (tar_RW_speed/204) / 100.0f;            // Divide desired RMP by RPM per duty cycle percent to get duty cycle
    int speed = dutyCycle * 2000;                             // Convert duty cycle to clockcycles
    if(speed < 5){speed = 5;}                                 // Set speed to 5 if its less than 5, to avoid PWM going up to 100%
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, speed);            // Set PD1 to 50% duty cycle

    //SysCtlDelay(500000);                                    // Delay 1/10 s
    return;
}


