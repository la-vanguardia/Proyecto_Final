
#include <libpic30.h>


#define REGISTRO_TEMPERATURA 0x07
#define REGISTRO_CODIGO_FAMILIA 0x2E

double leerTemperatura( unsigned char sensor );
void enviarTemp( trama_t mediciones );

double leerTemperatura( unsigned char sensor ){
    unsigned char datos[2] = {0};
    recibirDatos( datos , 2, REGISTRO_TEMPERATURA , sensor );
    double tempSensor = ( ((datos[1] & 0x7F ) << 8) + datos[0] )* 0.02 - 273.15;
    return tempSensor;
}

void enviarTemp( trama_t mediciones){
    unsigned char mensaje[100] = {'\0'};
    crearJSONString( mediciones, mensaje );
    enviarMensaje( mensaje );
}



