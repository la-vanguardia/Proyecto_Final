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


unsigned char flushCounter = 0;
unsigned char uart3Data[50] = {'\0'}, uart3Counter = 0;


void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt (  )
{
    IFS0bits.T1IF = 0;
    flushCounter++;
    if(flushCounter >= 100){
        //Se elimina la basura del serial
        T1CONbits.TON = 0;
        ubicacion_actual = 0;
    }
    
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1RXInterrupt( void )
{
    IFS0bits.U1RXIF = 0;
    T1CONbits.TON = 0;
    unsigned char data = U1RXREG;
    if(data == 0x0D){
        stateTemp = DECODIFICAR;
        datos_recepcion_uart1[ ubicacion_actual ] = '\0';
        ubicacion_actual = 0;
    }
    else{
        datos_recepcion_uart1[ ubicacion_actual ] = data;
        ubicacion_actual++;
        T1CONbits.TON = 1;
        flushCounter = 0;
    }
    
    
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U3RXInterrupt( void )
{
    IFS5bits.U3RXIF = 0;
    unsigned char data = U3RXREG;
    if(data == 0x0D){
        uart3Data[uart3Counter] = '\0';
        updateX = equals(uart3Data, MEASURE_X_COMMAND);
        updateY = equals(uart3Data, MEASURE_Y_COMMAND);
        measureInit = equals(uart3Data, MEASURE_INIT_COMMAND);
        measureEnd = equals(uart3Data, MEASURE_END_COMMAND);

        if(updateY){
            moveDirection *= CHANGE_DIRETCION;
        }

        if (updateX || updateY || measureEnd || measureInit){
            stateTemp = MEDIR;
        }

        uart3Counter = 0;
    }
    else{
        uart3Data[ uart3Counter ] = data;
        uart3Counter++;
    }
    
}

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    configurarI2C();
    
    while (1)
    { 
        stateMachineSensor();
    }
    return 1; 
}