/*
 * Magnetorquers.c
 *
 *  Created on: Apr 22, 2019
 *      Author: alexa
 */

#include "CubeSat.h"

void initializeMag()
{
    writeMag(0x22, 0x00);
    writeMag(0x20, 0x50);
    writeMag(0x23, 0x08);
}


void magXcontrol(bool op, bool dir)
{
    if(op == 1){
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_PIN_7);                  // Enable magnetorquers (PD7)
        if(dir == 1)
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0x0);                     // Turn on X magnetorquer in positive direction (PE3)
        else{
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x0);                     // Turn on X magnetorquer in negative direction (PE2)
        }
    }else{
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3);                  // Turn off X magnetorquer in positive direction
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);                  // Turn off X magnetorquer in negative direction
    }
    return;
}

void magYcontrol(bool op, bool dir)
{
    if(op == 1){
        GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, BLUE_LED);    // Turn on the blue LED
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, 0x0);         // Turn off the blue LED
    }
    return;
}

void magZcontrol(bool op, bool dir)
{
    if(op == 1){
        GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, GREEN_LED);   // Turn on the green LED
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, 0x0);         // Turn off the green LED
    }
    return;
}


