#include <math.h>
#include <stdlib.h>
#include "mcc_generated_files/uart1.h"

#define MAX_CIFRA_DECIMAL 2
#define TRAMA_JSON "trama"
#define PASOS_JSON "pasos"


unsigned char trama[10] = {'\0'};
double pasos[2] = {0.0, 0.0};


typedef struct trama_enviar{
    double *ubicacion;
    double *Temperatura;
    unsigned char end;
}trama_t;

typedef struct trama_recibir{
    unsigned char *trama;
    double *Pasos;
}recibir_t;

enum{
    TRAMA,
    PASOS,
    ERROR
};

recibir_t jsonRecibido;


void crearJSONString(trama_t trama, unsigned char *json);
void decodificarJSONString(unsigned char *json);
void split( unsigned char *Parametros, unsigned char *Valores, unsigned char *String);
unsigned char clasificar( unsigned char *Parametro );
unsigned char equals(unsigned char *String1, unsigned char *String2);
void obtener_pasos(double *Pasos, unsigned char *Valores);
double toFloat(unsigned char *value, unsigned char maxima_cifra_decimal);
void obtener_trama(unsigned char *Trama, unsigned char *Valores);
void pasarVectores(unsigned char *Vector_Nuevo, unsigned char *Vector_Viejo  );
double potencia(double numero_base, int potencia);

unsigned char longitudString(unsigned char *vect){
    unsigned char longitud = 0, letra; 
    letra =  vect[longitud];
    while( vect[ longitud ] != '\0'){
        longitud++;
    }
    return longitud;
}

void crearJSONString(trama_t trama, unsigned char *json){
    sprintf(json, "{\"ubicacion\": [%.2f, %.2f], \"temperatura\": [%.3f], \"end\": %d}", trama.ubicacion[0], trama.ubicacion[1], trama.Temperatura[0], trama.end );
}

void decodificarJSONString(unsigned char *json){
    unsigned char i=0, j=0, marcador = 0;
    unsigned char Parametros[60] = {'\0'}, done = 0;
    unsigned char Parametro[15] = {'\0'};
    unsigned char Valores[30] = {'\0'};
    unsigned char clase;
    
    unsigned char paso_string[100] = {'\0'};
    
    split(Parametros, Valores, json);
   
    while( done == 0 ){
        if( Parametros[i] != '-' ){
            if( Parametros[i] == '\0' ){
                done = 1;
            }
            else{
                Parametro[j] = Parametros[i];
                j++;
            }
        }
        else{
            Parametro[j] = '\0';
            j=0;
           
            clase = clasificar( Parametro );
            
            switch( clase ){
                case TRAMA:
                    obtener_trama( jsonRecibido.trama, Valores );
                    break;
                case PASOS:
                    obtener_pasos( jsonRecibido.Pasos, Valores );
                    break;
                case ERROR:
                    enviarMensaje("ERROR: clase de parametro no encontrada");
                    done = 1;
                    break;
            }
            
        }
        
       i++;
    }
}


void split( unsigned char *Parametros, unsigned char *Valores, unsigned char *String ){
    unsigned char longitud = longitudString( String ), i, k=0, j=0, done=0;
    unsigned char is_vector = 0;
    for( i=0; i<longitud; i++){
        switch( String[i] ){
            case ':':
                Parametros[k] = '-';
                k++;
                done = 1;
                break;
            case ',':
                Valores[j] = '-';
                j++;
                if( is_vector == 0) {
                    done = 0;
                }
                break;
            case '[':
                is_vector = 1;
                break;
            case ']':
                is_vector = 0;
                break;
            case ' ':
            case '\"':
            case '\'':
            case '{':
            case '}':
                break;
            default:
                if(done){
                    Valores[j] = String[i];
                    j++;
                }
                else{
                    Parametros[k] = String[i];
                    k++;
                }
        }
        
    }
}

unsigned char clasificar( unsigned char *Parametro ){
    
    unsigned char clase = ERROR;
    unsigned char is = 0;
    switch( Parametro[0] ){
        case 't':
            is = equals( Parametro, TRAMA_JSON );
            if(is == 1){
                clase = TRAMA;
            }
            break;
        case 'p':
            is = equals( Parametro, PASOS_JSON );
            if(is == 1){
                clase = PASOS;
            }
            break;
    }
    
    return clase;
}



void obtener_pasos(double *Pasos, unsigned char *Valores){
    unsigned char ValoresActualizados[60] = {'\0'}, i=0, j=0, valores = 0;
    unsigned char longitudValores = longitudString( Valores );
    unsigned char valor1[10] = {'\0'};
    unsigned char valor2[10] = {'\0'};
    
    while( i < longitudValores ){
        switch( valores ){
            case 0:
                if( Valores[i] == '-' ){
                    valores = 1;
                    j = 0;
                }
                else{
                    valor1[j] = Valores[i];
                    j++;
                }
                break;
            case 1:
                if( Valores[i] == '-' ){
                    valores = 2;
                    j = 0;
                }
                else{
                    valor2[j] = Valores[i];
                    j++;
                }
                break;
            case 2:
                ValoresActualizados[j] = Valores[i];
                j++;
                break;
        }
        
        i++;
        
    }
    
    pasos[0] = toFloat( valor1, MAX_CIFRA_DECIMAL );
    pasos[1] = toFloat( valor2, MAX_CIFRA_DECIMAL );
    
    pasarVectores(Valores, ValoresActualizados);
}


double toFloat(unsigned char *value, unsigned char maxima_cifra_decimal ){
    unsigned char longitudValue = longitudString( value );
    char i = 0;
    unsigned char n=1, accion = 1;
    double number = 0.0;
    while(value[i] != '\0'){
        if(value[i] == '.'){
            accion = 0;
        }
        else if(accion){
            number = number * 10.0 + (double)(value[i] - 0x30);
        }
        else if(n <= maxima_cifra_decimal){
            number += (double)( value[i] - 0x30 ) * ( powf( 10.0, -1 * n) );
            n++;
        }
        else{
            break;
        }
        i++;
    }
    
 
    return number;
}

void obtener_trama(unsigned char *Trama, unsigned char *Valores){
    unsigned char i=0, j=0,ValoresActualizados[60] = {'\0'}, valor;
    unsigned char longitudValores = longitudString( Valores ), accion = 0;
   
    
    for( i=0; i<longitudValores; i++ ){

        valor = Valores[i];
        

        switch( accion ){
            case 0:
                if( valor == '-' ){
                    trama[j] = '\0';
                    accion = 1;
                    j=0;
                }
                else{
                    trama[j] = valor;
                    j++;
                }
                break;
            case 1:
                ValoresActualizados[j] = valor;
                j++;
                break;
            
        }
        
        
        
    }
    pasarVectores( Valores, ValoresActualizados );
}

void pasarVectores(unsigned char *Vector_Nuevo, unsigned char *Vector_Viejo  ){
    unsigned char longitud = longitudString( Vector_Viejo ), i;
    for(i=0; i<longitud; i++){
        Vector_Nuevo[i] = Vector_Viejo[i];
    }
    Vector_Nuevo[longitud] = '\0';
}

double potencia(double numero_base, int potencia){
    unsigned int i;
    unsigned char invertir = 0;
    double resultado = numero_base;
    
    if( potencia < 0 ){
        potencia *= -1;
        invertir = 1;
    }
    
    for(i=0; i<potencia; i++){
            
        resultado *= numero_base; 
            
    }
    
    if( invertir ){
        resultado = 1 / resultado;
    }
    
    
    
}