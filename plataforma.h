
#include "mcc_generated_files/uart2.h"
//Aca se encuentran las funciones para comunicacion de la plataforma

#define STOP_COMMAND "STOP"
#define START_COMMAND "START"
#define CONFIG_COMMAND "CONFIG"
#define CONTINUE_COMMAND "CONTINUE"


int getMessageLength(char* message);
void sendMessageToPlatform(char* message);
void sendStart();
void sendConfig(double h, double k);
void sendStop();
void sendCommand(char* command);

int getMessageLength(char* message){
    unsigned char length = 0;
    while( message[ length ] != '\0' ){
        length++;
    }
    return length;
}

void sendMessageToPlatform(char* message){
    unsigned char length = longitud( message ), i;
    for( i=0; i < length; i++ ){
        UART2_Write( message[ i ] );
    }
    UART2_Write(0x0D);
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