
#include "mcc_generated_files/uart1.h"

unsigned char datos_recepcion_uart1[100] = {'\0'}, ubicacion_actual = 0;
unsigned char contador = 0, bandera_recepcion = 0;

void enviarMensaje( unsigned char *mensaje );

void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt (  )
{
    IFS0bits.T1IF = 0;
    contador++;
    if( contador == 60 ){
        
        bandera_recepcion = 1;
        datos_recepcion_uart1[ ubicacion_actual ] = '\0';
        ubicacion_actual = 0;
        T1CONbits.TON = 0;
    }
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _U1RXInterrupt( void )
{
    _U1RXIF = 0;
    
    datos_recepcion_uart1[ ubicacion_actual ] = U1RXREG;
    
    ubicacion_actual++;
    
    contador = 0;
    TMR1 = 0;
    T1CONbits.TON = 1;   
}

unsigned char longitud( unsigned char *mensaje ){
    unsigned char longitud_mensaje = 0;
    while( mensaje[ longitud_mensaje ] != '\0' ){
        longitud_mensaje++;
    }
    return longitud_mensaje;
}

void enviarMensaje(unsigned char *mensaje ){
    unsigned char longitud_mensaje = longitud( mensaje ), i;
    for( i=0; i<longitud_mensaje; i++ ){
        UART1_Write( mensaje[ i ] );
    }
}

