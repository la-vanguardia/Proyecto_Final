
#include "mcc_generated_files/uart1.h"

unsigned char datos_recepcion_uart1[100] = {'\0'}, ubicacion_actual = 0;
unsigned char contador = 0, bandera_recepcion = 0;

void enviarMensaje( unsigned char *mensaje );

void enviarMensaje(unsigned char *mensaje ){
    unsigned char longitud_mensaje = getLength( mensaje ), i;
    for( i=0; i<longitud_mensaje; i++ ){
        UART1_Write( mensaje[ i ] );
    }
    UART1_Write(0x0D); //Carry return para indicar fin del mensaje
}

