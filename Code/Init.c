/*
 * Init.c
 *
 *  Created on: Apr 22, 2019
 *      Author: alexa
 */

#include "CubeSat.h"

void InitializeMicro()
{
    // Setup the system clock to run at 50 Mhz from PLL with crystal reference
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Set GPIO Pin types
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);                            // Enable GPIO port B (I2C: PB2 PB3)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);                            // Enable GPIO port D (Mag GPIO: PD7  RW GPIO: PD1 PD2)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);                            // Enable GPIO port E (Mag GPIO: PE3 PE2)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);                            // Enable GPIO port F (Lights: PF1 PF2 PF3  Buttons: PF4)
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED);     // Configure all 3 LEDs to run on the same GPIO channel
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);                      // Configure SW1 to run as input
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);                     // Configure PD2 as GPIO output (RWZ direction)
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_7);                     // Configure PD7 as GPIO output (mag enable)
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3|GPIO_PIN_2);          // Configure PE3,PE2 as GPIO output (mag x positive, mag x negative)

    // ADC Setup
    GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_5);                                         // Configure PB5 as ADC input
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);                                          // Enable and wait for the ADC0 module to be ready
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);                        // SS3 for 1 sample
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH11);  // Configure SS3
    ADCSequenceEnable(ADC0_BASE, 3);                                                     // Enable since its properly configured
    ADCIntClear(ADC0_BASE, 3);                                                           // Clear the interrupt flag so its ready for sampling

    // Push Button and Interrupt Setup
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  // Enable weak pullup resistor for PF4
    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4);                                              // Disable interrupt for PF4 (in case it was enabled)
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);                                                // Clear pending interrupts for PF4
    GPIOIntRegister(GPIO_PORTF_BASE, LButtonDown);                                            // Register our handler function for port F
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);                           // Configure PF4 for falling edge trigger
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);                                               // Enable interrupt for PF4

    // I2C initialization
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);                             // Enable and wait for I2C module 0 to be ready
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);                              // Reset module
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);                                     // Configure the pin muxing for I2C0 functions on port B2 and B3.
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);                         // Configure PB2 as SCL for I2C
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);                            // Configure PB3 as SDA for I2C
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);                // Enable and initialize the I2C0 master module.  Use the system clock for the I2C0 module.  The last parameter sets the I2C data transfer rate.
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;                            // clear I2C FIFOs

    // PWM Initialization
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);                                                                // Enable PWM Module 1
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);                                                                        // Allows default clock tied to PWM to be divided
    GPIOPinConfigure(GPIO_PD1_M1PWM1);                                                                         // Configure PD1 as PWM
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DBG_RUN);    // Configure PWM for
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 2000);                                                               // Set the Period in clock ticks (50MHz clock speed -> 25kHz)
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, 0);                                                                 // Set PD1 to 0% duty cycle
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);                                                                        // Enables PWM Generator
    PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);                                                            // Turn on PD1 with designated duty cycle

}


