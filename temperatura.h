
#include <libpic30.h>




#define REGISTRO_TEMPERATURA 0x07
#define REGISTRO_CODIGO_FAMILIA 0x2E

double leerTemperatura( unsigned char sensor );
unsigned char cambiarCodigoFamilia( unsigned char sensorViejo, unsigned char sensorNuevo );

double leerTemperatura( unsigned char sensor ){
    unsigned char datos[2] = {0};
    recibirDatos( datos , 2, REGISTRO_TEMPERATURA , sensor );
    double tempSensor = ( ((datos[1] & 0x7F ) << 8) + datos[0] )* 0.02 - 273.15;
    return tempSensor;
}

/*
 
   for (int a = 0; a != 256; a++) {
     i2c_start_wait(0 + I2C_WRITE);
     i2c_write(0x2E);
     i2c_write(NewAddr1);
     i2c_write(NewAddr2);

     if (i2c_write(a) == 0) {
       i2c_stop();
       delay(100);
       Serial.print("> [ChangeAddr] Found correct CRC: 0x");
       Serial.println(a, HEX);
       return a;
     }
   }
   i2c_stop();
   Serial.println("> [ChangeAddr] Correct CRC not found");
   return -1;

 */


unsigned char cambiarCodigoFamilia( unsigned char sensorViejo, unsigned char sensorNuevo ){ 
    unsigned char codigo_familia_nuevo[3] = {0};
    unsigned char dato[4] = {'\0'};
    unsigned char i;
    codigo_familia_nuevo[0] = sensorViejo;
    
    iniciarComunicacion( 0x00, WRITE );
    transmitirDato( 0x2E );
    transmitirDato( 0x00 );
    transmitirDato( 0x00 );
    transmitirDato( 0x6F );
    
    detenerI2C();
    
    __delay_ms(100);
    
    iniciarComunicacion( 0x00, WRITE );
    transmitirDato( 0x2E );
    transmitirDato( 0x5B );
    transmitirDato( 0x00 );
    transmitirDato( 0xF4 );
    
    detenerI2C();
    
    return 0;
}

