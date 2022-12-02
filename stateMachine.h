#include "json.h"
#include "recepcion.h"
#include "I2C.h"
#include "temperatura.h"
#include "plataforma.h"


#define START "START"
#define STOP  "STOP"
#define CONFIG  "CONFIG"

enum states{
    ESPERAR = 0,
    MEDIR = 1,
    CLASIFICAR = 2,
    DECODIFICAR = 3
};

enum tramasClasificadas{
    StartDecodificado,
    StopDecodificado,
    ConfigDecodificado,
    ErrorDecodificado
};

unsigned char stateTemp = ESPERAR, prevStateTemp = ESPERAR;
double x, y, h, k;


unsigned char trmaUC;

void stateMachineSensor();

void eDecodificar();
void eClasificar();
void eMedir();

void aDecodificar();
void aClasificar();
void aComenzarMedicion();
void aFrenarMedicion(); 
void aConfigurarMedicion();


void stateMachineSensor(){  
    switch( stateTemp ){
        case( DECODIFICAR ):
            eDecodificar();
            aDecodificar();
            break;
        case( CLASIFICAR ):
            aClasificar();
            eClasificar();
            break;
        case( MEDIR ):
            eMedir();
            break;
        default: 
            break;
        
    }
    
}

void eDecodificar(){
    decodificarJSONString( datos_recepcion_uart1 );
}

void eClasificar(){
    unsigned char start = equals( trama, START  );
    unsigned char stop = equals( trama, STOP );
    unsigned char config = equals(trama, CONFIG );
    unsigned char resultado = ErrorDecodificado;
    enviarMensaje(trama);
    enviarMensaje("valores de comparacion start - stop - equal");
    UART1_Write(start + 0x30);
    UART1_Write(stop + 0x30);
    UART1_Write(config + 0x30);
    if( start ){
        enviarMensaje("ESTAMOS EN START");
        resultado = StartDecodificado;
    }
    else if( stop ){
        enviarMensaje("Estamos en STOP");
        resultado = StopDecodificado;
    }
    else if( config ){
        enviarMensaje("ESTAMOS CONFIGURANDO");
        resultado = ConfigDecodificado;
    }
    trmaUC = resultado;
}

void eMedir(){
    trama_t mediciones;
    unsigned char json[70] = {'\0'};
    
    double ubicacion[2] = {0, 0};
    double temp[4] = {0.0, 0.0, 0.0, 0.0};
    unsigned char trama[50] = "hola";
    
    
    mediciones.ubicacion = ubicacion;
    mediciones.reversa = 0;
    mediciones.trama = trama;
    
    temp[0] = leerTemperatura( SENSOR_1 );
    mediciones.Temperatura = temp;
    //enviarTemp( mediciones );
    if(prevStateTemp == MEDIR){
        stateTemp = ESPERAR;
    }
    else{
        stateTemp = prevStateTemp;
        prevStateTemp = MEDIR;
    }
}

void aDecodificar(){
    stateTemp = CLASIFICAR;
}

void aClasificar(){
    enviarMensaje("ESTA CLASIFICANDO");
    switch( trmaUC ){
        case StartDecodificado:
            aComenzarMedicion();
            break;
        case StopDecodificado:
            aFrenarMedicion();
            break;
        case ConfigDecodificado:
            aConfigurarMedicion();
            break;
        case ErrorDecodificado:
            enviarMensaje("ERROR: trama invalida");
            stateTemp = ESPERAR;
            break;
    }
}

void aComenzarMedicion(){
    //TODO: enviar se�al de start a la plataforma
    sendStart();
    stateTemp = MEDIR;
}

void aFrenarMedicion(){
    //TODO: debe enviar una se�al de stop
    enviarMensaje("SEND STOP TO PLATFORM");
    sendStop();
    stateTemp = ESPERAR;
}

void aConfigurarMedicion(){
    //TODO: envia a la plataforma los pasos h (eje x) y k (eje y)
    h = jsonRecibido.Pasos[0];
    k = jsonRecibido.Pasos[1];
    stateTemp = ESPERAR;
}
