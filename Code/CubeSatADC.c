/*
 * CubeSatADC.c
 *
 *  Created on: Apr 22, 2019
 *      Author: alexa
 */

#include "CubeSat.h"

void pollADC(uint32_t * ADC0Value)
{
    ADCProcessorTrigger(ADC0_BASE, 3);                              // Trigger the sample sequence.
    while(!ADCIntStatus(ADC0_BASE, 3, false)){}                     // Wait until the sample sequence has completed.
    ADCIntClear(ADC0_BASE, 3);                                      // Clear interrupt flag
    ADCSequenceDataGet(ADC0_BASE, 3, ADC0Value);                    // Read the value from the ADC
}


