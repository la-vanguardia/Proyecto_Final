#include "mcc_generated_files/system.h"
#include "mcc_generated_files/uart1.h"




#include <p33EP512GM304.h>
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "stateMachine.h"

#define SENSOR_1 0x5A
#define TEMPOBJ 0x07

void enviarTemp( trama_t mediciones );

void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt (  )
{
    IFS0bits.T1IF = 0;
    contador++;
    if( contador == 60 ){
        
        stateTemp = DECODIFICAR;
        datos_recepcion_uart1[ ubicacion_actual ] = '\0';
        ubicacion_actual = 0;
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



int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    configurarI2C();

    trama_t mediciones;
    unsigned char json[70] = {'\0'};
    
    double ubicacion[2] = {0, 0};
    double temp[4] = {0.0, 0.0, 0.0, 0.0};
    unsigned char trama[50] = "START";
    
    
    mediciones.ubicacion = ubicacion;
    mediciones.Temperatura = temp;
    mediciones.reversa = 1;
    mediciones.trama = trama;
    
    double tempSensor = leerTemperatura( SENSOR_1 );
      
    while (1)
    {
        stateMachineSensor();
    }
    return 1; 
}

void enviarTemp( trama_t mediciones){
    unsigned char mensaje[100] = {'\0'};
    

    crearJSONString( mediciones, mensaje );

    enviarMensaje( mensaje );
}


