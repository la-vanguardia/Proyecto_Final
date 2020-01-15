unsigned char datos_recepcion_uart1[100] = {'\0'}, ubicacion_actual = 0;
unsigned char contador = 0, bandera_recepcion = 0;



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
    IFS0bits.U1RXIF = 0;
    
    datos_recepcion_uart1[ ubicacion_actual ] = U1RXREG;
    
    ubicacion_actual++;
    
    contador = 0;
    TMR1 = 0;
    T1CONbits.TON = 1;
    
        
        

}

