#define FCY 20000000
#define SENSOR_1 0x5A
#define TEMPOBJ 0x07


#include "mcc_generated_files/system.h"
#include "mcc_generated_files/uart1.h"


#include <p33EP512GM304.h>
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "stateMachine.h"





void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt (  )
{
    IFS0bits.T1IF = 0;
    contador++;
    if( contador == 60 ){
        
        //stateTemp = DECODIFICAR;
        datos_recepcion_uart1[ ubicacion_actual ] = '\0';
        ubicacion_actual = 0;
        sendMessageToPlatform(datos_recepcion_uart1);
        T1CONbits.TON = 0;
    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1RXInterrupt( void )
{
    IFS0bits.U1RXIF = 0;
    datos_recepcion_uart1[ ubicacion_actual ] = U1RXREG;
    
    ubicacion_actual++;
    
    contador = 0;
    TMR1 = 0;
    T1CONbits.TON = 1;   
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U3RXInterrupt( void )
{
    IFS5bits.U3RXIF = 0;
    char data = U3RXREG;
    
    
}



int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    configurarI2C();
    enviarMensaje("HELLO WORLD");
   
    //sendMessageToPlatform("HELLO");
    while (1)
    {
        stateMachineSensor();
    }
    return 1; 
}