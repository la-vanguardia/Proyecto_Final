#include "json.h"
#include "recepcion.h"
#include "I2C.h"
#include "temperatura.h"


enum states{
    ESPERAR,
    MEDIR,
    ENVIAR,
    CLASIFICAR,
    DECODIFICAR
};

enum tramas{
    START = "Start",
    STOP = "Stop",
    CONFIG = "Config"
};

enum tramasClasificadas{
    StartDecodificado,
    StopDecodificado,
    ConfigDecodificado,
    ErrorDecodificado
};

unsigned char stateTemp = ESPERAR;
recibir_t jsonRecibido;
unsigned char trama;

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
    jsonRecibido.trama = trama;
}

void aClasificar(){
    unsigned char start = equals( START, jsonRecibido.trama  ), stop = equals( STOP, jsonRecibido.trama );
    unsigned char config = equals( CONFIG, jsonRecibido.trama );
    unsigned char resultado = ErrorDecodificado;
    if( start ){
        resultado = StartDecodificado;
    }
    if( stop ){
        resultado = StopDecodificado;
    }
    if( config ){
        resultado = ConfigDecodificado;
    }
    trama = resultado;
}

void aDecodificar(){
    stateTemp = CLASIFICAR;
}

void aClasificar(){
    switch( trama ){
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