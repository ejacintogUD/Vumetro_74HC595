/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

 // Version 2024-3 
 // Manejo de 8 leds usando un registro de corrimiento 74HC595

#include "AnalogIn.h"
#include "DigitalOut.h"
#include "PinNameAliases.h"
#include "PinNames.h"
#include "ThisThread.h"
#include "Thread.h"
#include "cmsis_os2.h"
#include "mbed.h"

#define DS   D4 // alias de los pines 
#define SHCP D2
#define STCP D3

//#define salidas para manejo del registro de corrimiento
DigitalOut SerialData (DS);     // Datos seriales a correr
DigitalOut RegisterClk(SHCP);   // Reloj del registro de corrimiento
DigitalOut LatchClk   (STCP);   // Reloj del Latch del registro
DigitalOut led(LED1);


//AnalogIn Ain (A0); // Entrada analoga para el Vúmetro


//float Vin;  // Float donde se carga la entrada analoga
char leds=0;  //Variable donde se cargan los datos    


//protipos 
void shift_data (void);
void load_data (void);
void clean(void);
void send_data(void); // funcion del hilo



// hilos y herramientas del sistema operativo
Thread T_send_data(osPriorityNormal, 4096, NULL, NULL);


int main()
{
    // Arranque del programa
    SerialData  =0;
    RegisterClk =0;
    LatchClk    =0;
    clean();
    wait_us(1e6);
    // Encender los hilos
    T_send_data.start(send_data);
    while (true) 
        {
           led = !led;
           ThisThread::sleep_for(1s);
        }
       
      
}

void shift_data (int8_t data)
{
     for(int i=0; i<8; i++)
        {
           
            //if ((data & 0x01) == 0x01) SerialData = 1;
            //else                       SerialData = 0;              
            SerialData = ((data & 0x01) == 0x01) ? 1:0;
            RegisterClk=0;
            wait_us(1);
            RegisterClk=1;
            wait_us(0); 
            data = data>>1;
        }
}

void load_data(int8_t data)
{
    LatchClk=0;
    shift_data(data);
    LatchClk=1;    
}

void clean(void)
{
    load_data(0x00);
}


void send_data (void)
{
    char data= 0x55;
    while(true)
    {  
        data = ~data;
        load_data(data);
       
        ThisThread::sleep_for(200ms);
        
    }
}




