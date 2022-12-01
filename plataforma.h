
#include "mcc_generated_files/uart3.h"


//Aca se encuentran las funciones para comunicacion de la plataforma

#define STOP_COMMAND "STOP"
#define START_COMMAND "START"
#define CONFIG_COMMAND "CONFIG"
#define CONTINUE_COMMAND "CONTINUE"

#define MEASURE_COMMAND "MEASURE"


unsigned char getMessageLength(unsigned char* message);
void sendMessageToPlatform(unsigned char* message);
void sendStart();
void sendConfig(double h, double k);
void sendStop();
void sendCommand(char* command);

unsigned char getMessageLength(unsigned char* message){
    unsigned char length = 0;
    while( message[ length ] != '\0' ){
        length++;
    }
    return length;
}

void sendMessageToPlatform(unsigned char* message){
    unsigned char length = getMessageLength( message );
    unsigned char i;
    for( i=0; i < length; i++ ){
        UART3_Write( message[ i ] );
    }
    UART3_Write(0x0D);
}

void sendStart(){
    sendMessageToPlatform(START_COMMAND);
}

void sendStop(){
    sendMessageToPlatform(STOP_COMMAND);
}

void sendContinue(){
    sendMessageToPlatform(CONTINUE_COMMAND);
}

void sendConfig(double h, double k){
    sendMessageToPlatform(CONFIG_COMMAND);
}