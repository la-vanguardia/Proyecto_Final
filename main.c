

#define FCY 15000000 

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/uart1.h"

#include <p33EP512GM304.h>
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "json.h"
#include "recepcion.h"
#include "I2C.h"


#define SENSOR_1 0x5A
#define TEMPOBJ 0x07
#define MPU 0x68
#define TESTMEM 0x75

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    configurarI2C();
    
    unsigned char cosa[150] = {'\0'};
    unsigned char texto[15] = "HOLA";
    double temp[4] = {1.2, 3.2, 5.3, 11.3};
    double ubicacion[2] = {12.5, 33.5}; 

    unsigned char dato_recibido[60] = {'\0'};
 
    sprintf( dato_recibido, "{\"trama\": \"START\", \"paso\": [ 1.101, 20.034 ]}" );

    recibir_t dato;
    
    decodificarJSONString( dato_recibido );
 
    dato.trama = trama;
    dato.Pasos = paso;
    
    unsigned char es_lo_que_espero = equals( dato.trama, "START" );

    unsigned char datos_i2c[3] = {0};
    
    TEST( "HOLA ISA! " );
    
    recibirDatos( datos_i2c, 1, TESTMEM, MPU );
    
    TEST("DATO: ");

    UART1_Write( datos_i2c[0] );

    TEST("  FUNCIONO!");    
  
    
    
    while (1)
    {
        if( bandera_recepcion == 1){
            
            TEST( datos_recepcion_uart1 );
            bandera_recepcion = 0;
        }
    
    }
    return 1; 
}


//X: %.2f, Y: %.2f, Temperatura: [%.3f, %.3f, %.3f], 
/**
 End of File
*/

