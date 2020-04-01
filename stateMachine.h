#include "json.h"
#include "recepcion.h"
#include "I2C.h"
#include "temperatura.h"

#define START "Start"
#define STOP  "Stop"
#define CONFIG  "Config"

enum states{
    ESPERAR,
    MEDIR,
    ENVIAR,
    CLASIFICAR,
    DECODIFICAR
};



enum tramasClasificadas{
    StartDecodificado,
    StopDecodificado,
    ConfigDecodificado,
    ErrorDecodificado
};

unsigned char stateTemp = ESPERAR;
recibir_t jsonRecibido;
unsigned char trmaUC;

void stateMachineSensor();

void eDecodificar();
void eClasificar();


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
            
            break;
        case( ENVIAR ):
            
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
    //prende el timer de la medicion
}

void aFrenarMedicion(){
    //apaga el timer de la medicion
}

void aConfigurarMedicion(){
    //configura los pasos de la medicion
}