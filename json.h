typedef struct trama_enviar{
    unsigned char *trama;
    unsigned char reversa;
    double *ubicacion;
    double *Temperatura;
}trama_t;

enum{
    UBICACION,
    TEMPERATURA,
    REVERSA,
    TRAMA,
    ERROR
};


unsigned char longitudString(unsigned char *vect);
void crearJSONString(trama_t trama, unsigned char *json);
trama_t decodificarJSONString(unsigned char *json);
void split( unsigned char *Parametros, unsigned char *Valores, unsigned char *String);
unsigned char clasificar( unsigned char *Parametro );
unsigned char equals(unsigned char *String1, unsigned char *String2);
void obtener_ubicacion(double *ubicacion, unsigned char *Valores);

double toFloat(unsigned char *value);


unsigned char longitudString(unsigned char *vect){
    unsigned char longitud = 0, letra; 
    letra =  vect[longitud];
    while( vect[ longitud ] != '\0'){
        longitud++;
    }
    return longitud;
}

void crearJSONString(trama_t trama, unsigned char *json){
    sprintf(json, "{ubicacion: [%.2f, %.2f], Temperatura: [%.3f, %.3f, %.3f, %.3f], reversa: %u, trama: \"%s\"}", trama.ubicacion[0], trama.ubicacion[1], trama.Temperatura[0], trama.Temperatura[1], trama.Temperatura[2], trama.Temperatura[3], trama.reversa, trama.trama  );
}

trama_t decodificarJSONString(unsigned char *json){
    unsigned char i=0, j=0;
    unsigned char Parametros[60] = {'\0'}, done = 0;
    unsigned char Parametro[15] = {'\0'};
    unsigned char Valores[30] = {'\0'};
    unsigned char clase;
    trama_t trama;
    
    split(Parametros, Valores, json);
    
    while( ~done ){
        if( Parametros[i] != '\0' ){
            Parametro[j] = Parametros[i];
            j++;
        }
        else{
            j=0;
            clase = clasificar( Parametro );
            
            switch( clase ){
                case UBICACION:
                    obtener_ubicacion( trama.ubicacion, Valores  );
                    break;
                case TEMPERATURA:
                    
                    break;
                case TRAMA:
                    
                    break;
                case REVERSA:
                    
                    break;
            }
            
        }
        
        i++;
    }
    
    
    return trama;
}


void split( unsigned char *Parametros, unsigned char *Valores, unsigned char *String ){
    unsigned char longitud = longitudString( String ), i, k=0, j=0, done=0;
    
    for( i=1; i<longitud-1; i++){
        
        switch( String[i] ){
            case ':':
                Parametros[k] = '\0';
                k++;
                done = 1;
                break;
            case ',':
                Valores[j] = '-';
                j++;
                done = 0;
                break;
            case ' ':
                break;
            case '[':
                break;
            case ']':
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
        case 'u':
            is = equals( Parametro, 'ubiacion' );
            if(is == 1){
                clase = UBICACION;
            }
            break;
        case 'T':
            is = equals( Parametro, 'Temperatura' );
            if(is == 1){
                clase = TEMPERATURA;
            }
            break;
        case 'r':
            is = equals( Parametro, 'reversa' );
            if(is == 1){
                clase = REVERSA;
            }
            break;
        case 't':
            is = equals( Parametro, 'trama' );
            if(is == 1){
                clase = TRAMA;
            }
            break;
    }
    
    return clase;
}

unsigned char equals(unsigned char *String1, unsigned char *String2){
    unsigned char longitud1 = longitudString( String1 ), longitud2 = longitudString( String2 );
    unsigned char result = 0, i;
    if( longitud1 == longitud2 ){
        result = 1;
        for( i=0; i < longitud1; i++ ){
            if( String1[i] != String2[i] ){
                result = 0;
                break;
            }
        }
    }
    
    return result;
    
}

void obtener_ubicacion(double *ubicacion, unsigned char *Valores){
    unsigned char ValoresActualizados[60] = {'\0'}, i=0, j=0, valores = 0;
    unsigned char longitudValores = longitudString( Valores );
    unsigned char valor1[6] = {'\0'};
    unsigned char valor2[6] = {'\0'};
    double ubicaciones[2] = {0.0, 0.0};
    while( i < longitudValores ){
        switch( valores ){
            case 0:
                if( Valores[i] == '-' ){
                    i++;
                }
                else{
                    valor1[j] = Valores[i];
                    j++;
                }
                break;
            case 1:
                if( Valores[i] == '-' ){
                    i++;
                }
                else{
                    valor2[j] = Valores[i];
                    j++;
                }
                break;
            case 2:
                ValoresActualizados[i] = Valores[i];
                break;
        }
        
        i++;
        
    }
    
    
    ubicaciones[0] = toFloat( valor1 );
    ubicaciones[1] = toFloat( valor2 );
    
    Valores = ValoresActualizados;
    
}

double toFloat(unsigned char *value){
    
    return 0.0;
}