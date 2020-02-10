

#define REGISTRO_TEMPERATURA 0x07

double leerTemperatura( unsigned char sensor );
unsigned char cambiarCodigoFamilia( unsigned char sensorViejo, unsigned char sensorNuevo );

double leerTemperatura( unsigned char sensor ){
    unsigned char datos[2] = {0};
    recibirDatos( datos , 2, REGISTRO_TEMPERATURA , sensor );
    double tempSensor = ( ((datos[1] & 0x7F ) << 8) + datos[0] )* 0.02 - 273.15;
    return tempSensor;
}

unsigned char cambiarCodigoFamilia( unsigned char sensorViejo, unsigned char sensorNuevo ){
    return 0;
}

