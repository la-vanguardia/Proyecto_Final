/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.155.0-a
        Device            :  dsPIC33EP512GM304
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.40
        MPLAB 	          :  MPLAB X v5.25
*/

/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#define FCY 15000000 

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/uart1.h"

#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "json.h"








int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    //unsigned char cosa[] = "{X: 12.5, Y: 63.3, Temperatura: [321.3, 365.3, 125.3], trama: \"mediciones\", reversa: true}\n"; 
    unsigned char cosa[150] = {'\0'};
    unsigned char texto[15] = "HOLA";
    double temp[4] = {1.2, 3.2, 5.3, 11.3};
    double ubicacion[2] = {12.5, 33.5}; 
    trama_t trama;

    trama.reversa = 1;
    trama.trama = texto;
    trama.Temperatura = temp;
    trama.ubicacion = ubicacion;
    
    
    crearJSONString(trama, cosa);
    
    //unsigned char cosa[] = "HOLA\n";
    UART1_WriteBuffer(cosa,  longitudString( cosa ) );
    //UART1_Write( longitudString(cosa) );

    while (1)
    {
    
    
    }
    return 1; 
}


//X: %.2f, Y: %.2f, Temperatura: [%.3f, %.3f, %.3f], 
/**
 End of File
*/

