/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "AnalogIn.h"
#include "ThisThread.h"
#include "mbed.h"

//#define leds 3

DigitalOut Ds(D2);    // Entrada serial del registro de corrimiento
DigitalOut SHCP(D3); // Clock flanco de subida
DigitalOut STCP(D4); // Latch flanco de bajada

AnalogIn Ain (A0);

int i;
float Vin;
int leds=0;



int main()
{

    Ds=0;
    SHCP =0;
    STCP=1;

     for(i=0; i<8; i++)
        {
            Ds=0;
            SHCP = 0;
            STCP = 1;
            wait_us(1);
            SHCP = 1;
            STCP = 0;
            wait_us(1);
            
        }

    while (true) {
        Vin = Ain*7;
        leds = int(Vin);

        for(i=0; i<8; i++)
        {
            if (i< leds-1) Ds = 1;
            else Ds=0;
            SHCP = 0;
            STCP = 1;
            wait_us(10);
            SHCP = 1;
            STCP = 0;
            wait_us(10);
           
        }
       
        ThisThread::sleep_for(100ms);



    }
}
