#include "CubeSat.h"

// The error routine that is called if the driver library encounters an error.
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line){}
#endif

int volatile curr_X_rotation = 0;
int volatile curr_Y_rotation = 0;
int volatile curr_Z_rotation = 0;
int volatile curr_RWX_speed = 0;
int volatile curr_RWY_speed = 0;
int volatile curr_RWZ_speed = 0;

int main(void)
{
    InitializeMicro();

    // Variables
    uint32_t ADC0Value[1];                                                  // Variable to store ADC input
    uint8_t X_rot_H, Y_rot_H, Z_rot_H;                                      // Variables to store gyroscope input (highs)
    uint8_t X_rot_H_1, X_rot_H_2;
    uint8_t Y_rot_H_1, Y_rot_H_2;
    uint8_t Z_rot_H_1, Z_rot_H_2;
    uint8_t X_mag, Y_mag, Z_mag;
    float xSpeed;
    float ySpeed;
    float zSpeed;
    int currSpeed;                                                          // Variable to hold current reaction wheel speed in Hz
    //uint8_t X_rot_L, Y_rot_L, Z_rot_L;                                    // Variables to store gyroscope input (lows)
    float curr_or[3] = {1, 2, 3};                                           // Current orientation vector
    float tar_or[3] = {3, 2, 1};                                            // Target orientation vector
    float unit_cur_or[3] = {0, 0, 0};                                       // Unit vector for current orientation vector
    float unit_tar_or[3] = {0, 0, 0};                                       // Unit vector for target orientation vector
    float rotation_matrix[3][3] = {{0, 0, 0},{0, 0, 0},{0, 0, 0}};          // Rotation matrix between current orientation and target orientation
    float roll, pitch, yaw;                                                 // Necessary roll pitch and yaw to turn current orientation to target orientation

    uint32_t temp;
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3);                  // Turn on X magnetorquer in positive direction (PE3)
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);                  // Turn on X magnetorquer in negative direction (PE2)

    //writeGyro(0x20, 0x0F);                                                  // Set power-down mode to off
    //initializeRW();
    //initializeMag();
    while(1)        // Loop forever
    {
        //GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, RED_LED);     // Turn on the red LED
        //GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3);                  // Turn on X magnetorquer in positive direction (PE3)

        SysCtlDelay(16000000u / 3u);
        getUnitVec(curr_or, unit_cur_or);                   // Get unit vector for current orientation
        getUnitVec(tar_or, unit_tar_or);                    // Get unit vector for target orientation
        getRotationMatrix(unit_cur_or, unit_tar_or, rotation_matrix);       // Get rotation matrix between two unit vectors
        getThetas(rotation_matrix, &roll, &pitch, &yaw);                    // Get rotation angles from rotation matrix

        //readGyro(&X_rot_H, &Y_rot_H, &Z_rot_H);             // Read all six registers off of the gyro into variables
        if(X_rot_H <= 127){
            xSpeed = X_rot_H / 127;
            xSpeed = (xSpeed * 245) * -1;
        }else{
            xSpeed = 255 - X_rot_H;
            xSpeed = xSpeed / 127;
            xSpeed = xSpeed * 245;
        }
        if(Y_rot_H <= 127){
            ySpeed = Y_rot_H / 127;
            ySpeed = (ySpeed * 245) * -1;
        }else{
            ySpeed = 255 - Y_rot_H;
            ySpeed = ySpeed / 127;
            ySpeed = ySpeed * 245;
        }
        if(Z_rot_H <= 127){
            zSpeed = Z_rot_H / 127;
            zSpeed = (zSpeed * 245) * -1;
        }else{
            zSpeed = 255 - Z_rot_H;
            zSpeed = zSpeed / 127;
            zSpeed = zSpeed * 245;
        }

        temp = I2CReceive(0x69, 0x20);
        //readMag(&X_mag, &Y_mag, &Z_mag);

        //magXcontrol(1, 1);              // Turn on Magnetorquer X in the positive direction
        //magXcontrol(0, 0);              // Turn off Magnetorquer X

        //pollADC(ADC0Value);                                       // Get value from ADC0 on PB5

        float dutyCycle = *ADC0Value / 4095.0f;                   // Divide desired ADC value by max adc Value to get percent
        int speed = 0;
        if(xSpeed > speed){speed = xSpeed;}
        if(ySpeed > speed){speed = ySpeed;}
        if(zSpeed > speed){speed = zSpeed;}
        if(speed > 30){
            speed = speed * 100;
            spinRWZ(speed);
            SysCtlDelay(10000000);
        }
        //int speed = dutyCycle * 20400;                            // Convert duty cycle to clockcycles
        //spinRWZ(speed);
        //currSpeed = readRWspeed();

        //if(abs(curr_RWX_speed) > rwSatThresh){                                                  // If tumbling in either direction on X axis
        //    desaturate(0, &curr_X_rotation, &curr_RWX_speed, unit_cur_or);                      // Detumble on X axis
        //}
        //if(abs(curr_RWY_speed) > rwSatThresh){                                                  // If tumbling in either direction on Y axis
        //    desaturate(1, &curr_Y_rotation, &curr_RWY_speed, unit_cur_or);                      // Detumble on Y axis
        //}
        //if(abs(curr_RWZ_speed) > rwSatThresh){                                                  // If tumbling in either direction on Z axis
        //    desaturate(2, &curr_Z_rotation, &curr_RWZ_speed, unit_cur_or);                      // Detumble on Z axis
        //}
    }
}

void LButtonDown(void)
{
    uint32_t ADC0Value[1];

    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4) {
        GPIOIntRegister(GPIO_PORTF_BASE, LButtonUp);                       // Register our handler function for port F
        GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);     // Configure PF4 for rising edge trigger
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);                         // Clear interrupt flag

        pollADC(ADC0Value);                // Get value from ADC0 on PB5
        if(ADC0Value[0] < 1200){                                                      // 0-1v
            //GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, RED_LED);     // Turn on the red LED
            curr_X_rotation = 15;
        }
        else if(ADC0Value[0] < 2400){                                                 // 1-2v
            //GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, BLUE_LED);    // Turn on the blue LED
            curr_Y_rotation = 15;
            }
        else{                                                                         // 2-3.3v
            //GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, GREEN_LED);   // Turn on the green LED
            curr_Z_rotation = 15;
        }

        if(abs(curr_X_rotation) > tumbThresh){                           // If tumbling in either direction on X axis
            detumble(0, &curr_X_rotation, &curr_RWX_speed);              // Detumble on X axis
        }
        if(abs(curr_Y_rotation) > tumbThresh){                           // If tumbling in either direction on Y axis
            detumble(1, &curr_Y_rotation, &curr_RWY_speed);              // Detumble on Y axis
        }
        if(abs(curr_Z_rotation) > tumbThresh){                           // If tumbling in either direction on Z axis
            detumble(2, &curr_Z_rotation, &curr_RWZ_speed);              // Detumble on Z axis
        }
        GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, 0x0);
    }
}

void LButtonUp(void)
{
    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4) {
        // PF4 was interrupt cause
        GPIOIntRegister(GPIO_PORTF_BASE, LButtonDown);                     // Register our handler function for port F
        GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);    // Configure PF4 for falling edge trigger
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);                         // Clear interrupt flag
    }
}

void detumble(int tar_RW, volatile int * curr_rotation, volatile int * curr_RW_speed)
{
    while(abs(*curr_rotation) > tumbThresh){
        if(tar_RW == 0){
            if(*curr_rotation > 0){
                spinRWX(*curr_RW_speed + 1);   // Increase wheel speed to decrease rotation
                *curr_rotation = *curr_rotation - 1;                         // Simulate satellite rotation change
            }
            else{
                spinRWX(*curr_RW_speed - 1);   // Decrease wheel speed to decrease rotation
                *curr_rotation = *curr_rotation + 1;                         // Simulate satellite rotation change
            }
        }
        else if(tar_RW == 1){
            if(*curr_rotation > 0){
                spinRWY(*curr_RW_speed + 1);   // Increase wheel speed to decrease rotation
                *curr_rotation = *curr_rotation - 1;                         // Simulate satellite rotation change
            }
            else{
                spinRWY(*curr_RW_speed - 1);   // Decrease wheel speed to decrease rotation
                *curr_rotation = *curr_rotation + 1;                         // Simulate satellite rotation change
            }
        }
        else if(tar_RW == 2){
            if(*curr_rotation > 0){
                spinRWZ(*curr_RW_speed + 1);   // Increase wheel speed to decrease rotation
                *curr_rotation = *curr_rotation - 1;                         // Simulate satellite rotation change
            }
            else{
                spinRWZ(*curr_RW_speed - 1);   // Decrease wheel speed to decrease rotation
                *curr_rotation = *curr_rotation + 1;                         // Simulate satellite rotation change
            }
        }
    }
    return;
}

void desaturate(int tar_RW, volatile int * curr_rotation, volatile int * curr_RW_speed, float * unit_or){
    while(abs(*curr_RW_speed) > rwRestThresh){
        if(tar_RW == 0){                                                    // If X wheel
            magYcontrol(1, 1);                                                  // Turn on magnetorquer Y
            while(unit_or[0] != 0 || unit_or[1] != 1 || unit_or[2] != 0){       // Wait for unit orientation vector to be <0, 1, 0>
                //SysCtlDelay(50000000);                                        // Wait 1 second then set vector to <0, 1, 0> to simulate change
                unit_or[0] = 0;
                unit_or[1] = 1;
                unit_or[2] = 0;
            }
            if(*curr_RW_speed > 0){                                         // If X wheel is spinning in positive direction
                spinRWX(*curr_RW_speed - 1);                                    // Decrease speed by one
            }
            else{                                                           // If X wheel is spinning in negative direction
                spinRWX(*curr_RW_speed + 1);                                    // Increase speed by one
            }
        }
        else if(tar_RW == 1){                                                   // If Y wheel
            magZcontrol(1, 1);                                                     // Turn on magnetorquer Z
            while(unit_or[0] != 0 || unit_or[1] != 0 || unit_or[2] != 1){       // Wait for unit orientation vector to be <0, 0, 1>
                //SysCtlDelay(50000000);                                        // Wait 1 second then set vector to <0, 0, 1> to simulate change
                unit_or[0] = 0;
                unit_or[1] = 0;
                unit_or[2] = 1;
            }
            if(*curr_RW_speed > 0){                                             // If Y wheel is spinning in positive direction
                spinRWY(*curr_RW_speed - 1);      // Decrease speed by one
            }
            else{                                                               // If Y wheel is spinning in negative direction
                spinRWY(*curr_RW_speed + 1);      // Increase speed by one
            }
        }
        else if(tar_RW == 2){                                                   // If Z wheel
            magXcontrol(1, 1);                                                  // Turn on magnetorquer X
            while(unit_or[0] != 0 || unit_or[1] != 0 || unit_or[2] != 1){       // Wait for unit orientation vector to be <1, 0, 0>
                //SysCtlDelay(50000000);                                        // Wait 1 second then set vector to <1, 0, 0> to simulate change
                unit_or[0] = 1;
                unit_or[1] = 0;
                unit_or[2] = 0;
            }
            if(*curr_RW_speed > 0){                                             // If Z wheel is spinning in positive direction
                spinRWZ(*curr_RW_speed - 1);      // Decrease speed by one
            }
            else{                                                               // If Z wheel is spinning in negative direction
                spinRWZ(*curr_RW_speed + 1);      // Increase speed by one
            }
        }
    }
    // Turn off any magnetorquer
    magXcontrol(0, 0);
    magYcontrol(0, 0);
    magZcontrol(0, 0);
    return;
}
