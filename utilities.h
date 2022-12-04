
unsigned char getLength(unsigned char* string) {
    unsigned char length = 0;
    while(string[length] != '\0'){
        length++;
    }
    return length;
}

unsigned char equals(unsigned char* string, unsigned char* string2){
    unsigned char longitud1 = getLength(string), longitud2 = getLength(string2);
    unsigned char result = 0, i;
    if( longitud1 == longitud2 ){
        result = 1;
        for( i=0; i < longitud1; i++ ){
            if( string[i] != string2[i] ){
                result = 0;              
                break;
            }
        }
    }
    
    return result;   
}