#include <xc.h>
#include <libpic30.h>

#define READ 1
#define WRITE 0

void configurarI2C();
void iniciarComunicacion(unsigned char codigo_familia, unsigned char read); //read=1 r, read=0 w
void transmitirDato(unsigned char dato);
void transmitirDatos(unsigned char *datos, unsigned char numero_datos, unsigned char dirreccion, unsigned char codigo_familia);
unsigned char recibirDato(unsigned char detener);
void recibirDatos(unsigned char *datos, unsigned char numero_datos, unsigned char dirreccion, unsigned char codigo_familia);
void resetearI2C();
void iniciarI2C();
void detenerI2C();
unsigned char Longitud=0;

void configurarI2C(){
    //I2C1BRG = 0x21; // Velocidad para 400KHz y 30MHz (Ecuacion 19-1)   
    
    I2C1BRG = 0x4D;
    I2C1CONbits.I2CEN = 1;    
    IFS1bits.SI2C1IF = 0;  //Bandera de interrupcion I2C esclavo
    I2C1CONbits.DISSLW = 1;   //Desabilitar Slew Rate
    I2C1CONbits.IPMIEN = 0;
    I2C1CONbits.SMEN = 1;
}

void iniciarComunicacion(unsigned char codigo_familia, unsigned char read){
    unsigned char codigo = ( codigo_familia<<1 ) + read;
    transmitirDato( codigo );
}

void transmitirDato(unsigned char dato){
    while(I2C1STATbits.TBF == 1);
    IFS1bits.MI2C1IF = 0; //bajo la bandera
    
    I2C1TRN = dato; //envio el dato
    while(I2C1STATbits.ACKSTAT == 1); //espero que baje el ackstat
    while(IFS1bits.MI2C1IF == 1); //
    while(I2C1STATbits.TRSTAT == 1); //espero al tr


}

void transmitirDatos(unsigned char *datos, unsigned char numero_datos, unsigned char dirreccion, unsigned char codigo_familia){
    iniciarComunicacion(codigo_familia, WRITE);
    transmitirDato(dirreccion);
    
    unsigned char i;
    for(i=0; i<numero_datos; i++){
        transmitirDato( datos[i] );
    }
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN == 1);
}

unsigned char recibirDato(unsigned char detener){
    unsigned char dato;
    
    IFS1bits.MI2C1IF = 0; //bajo bandera
    I2C1CONbits.RCEN = 1; //habilito recepcion
    
    
    while(IFS1bits.MI2C1IF == 1); //espero bandera
    
    IFS1bits.MI2C1IF = 0; //bajo bandera
    while(I2C1STATbits.RBF == 0);
    dato = I2C1RCV; //leo el dato
    
    if(detener == 1){
        detenerI2C();
    }
    
    return dato;
}

void recibirDatos(unsigned char *datos, unsigned char numero_datos, unsigned char dirreccion, unsigned char codigo_familia){
    iniciarI2C();
    iniciarComunicacion(codigo_familia, WRITE);
    transmitirDato(dirreccion);
    
    resetearI2C();
    iniciarComunicacion(codigo_familia, READ);
    unsigned char i=0;
    Longitud=numero_datos;
    for(i=0; i<numero_datos; i++){
        
        datos[i] = recibirDato(0);
        if(i != (numero_datos - 1) ){
            I2C1CONbits.ACKDT = 0; // ACK
            I2C1CONbits.ACKEN = 1; // habilitador ACK
            while(I2C1CONbits.ACKEN == 1);
        }
        
    }
    
    detenerI2C();
}

void iniciarI2C(){
    I2C1CONbits.SEN = 1;    
    while(I2C1CONbits.SEN == 1);
}


void detenerI2C(){
    I2C1CONbits.ACKDT = 1; // ACK
    I2C1CONbits.ACKEN = 1; // habilitador ACK
    while(I2C1CONbits.ACKEN == 1);
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN == 1);
}

void resetearI2C(){
    I2C1CONbits.RSEN = 1;
    while(I2C1CONbits.RSEN == 1);
}