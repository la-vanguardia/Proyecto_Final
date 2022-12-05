#include "utilities.h"
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

unsigned char stateTemp = ESPERAR;
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
            eClasificar();
            aClasificar();
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
    unsigned char start = equals(trama, START  );
    unsigned char stop = equals(trama, STOP );
    unsigned char config = equals(trama, CONFIG );
    unsigned char resultado = ErrorDecodificado;
    if( start ){
        resultado = StartDecodificado;
    }
    else if( stop ){
        resultado = StopDecodificado;
    }
    else if( config ){
        enviarMensaje("CONFIGURANDO...");
        resultado = ConfigDecodificado;
    }
    trmaUC = resultado;
}

void eMedir(){
    x += h;
    y += k;
    trama_t mediciones;
    unsigned char json[70] = {'\0'};
    
    double ubicacion[2] = {x, y};
    double temp[1] = {0.0};
    
    
    mediciones.ubicacion = ubicacion;
    
    temp[0] = leerTemperatura( SENSOR_1 );
    mediciones.Temperatura = temp;
    enviarTemp( mediciones );
    sendContinue();
    stateTemp = ESPERAR;
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
            enviarMensaje("MANDANDO COSITAS!");
            aConfigurarMedicion();
            break;
        case ErrorDecodificado:
            enviarMensaje("ERROR: trama invalida");
            stateTemp = ESPERAR;
            break;
    }
}

void aComenzarMedicion(){
    //Lleva al origen las medidciones
    x = 0;
    y = 0;
    
    sendStart();
    stateTemp = ESPERAR;
}

void aFrenarMedicion(){
    sendStop();
    stateTemp = ESPERAR;
}

void aConfigurarMedicion(){
    h = pasos[0];
    k = pasos[1];
    //TODO: envia a la plataforma los pasos h (eje x) y k (eje y
    stateTemp = ESPERAR;
    sendConfig(h, k);
}
