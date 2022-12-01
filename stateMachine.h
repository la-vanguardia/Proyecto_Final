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
recibir_t jsonRecibido;
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
    jsonRecibido.Pasos = paso;
    jsonRecibido.trama = trmaUC;
}

void eClasificar(){
    unsigned char start = equals( START, jsonRecibido.trama  ), stop = equals( STOP, jsonRecibido.trama );
    unsigned char config = equals( CONFIG, jsonRecibido.trama );
    unsigned char resultado = ErrorDecodificado;
    if( start ){
        resultado = StartDecodificado;
    }
    else if( stop ){
        resultado = StopDecodificado;
    }
    else if( config ){
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
