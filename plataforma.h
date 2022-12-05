
#include "mcc_generated_files/uart3.h"


//Aca se encuentran las funciones para comunicacion de la plataforma

#define STOP_COMMAND "STOP"
#define START_COMMAND "START"
#define CONFIG_COMMAND "CONFIG"
#define CONTINUE_COMMAND "CONTINUE"

#define MEASURE_COMMAND "MEASURE"



void sendMessageToPlatform(unsigned char* message);
void sendStart();
void sendConfig(double h, double k);
void sendStop();
void sendCommand(char* command);



void sendMessageToPlatform(unsigned char* message){
    unsigned char length = getLength( message );
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
    unsigned char configMessage[30] = {'\0'};
    sprintf(configMessage, '%s-%.2f-%.2f', CONFIG_COMMAND, h, k);
    enviarMensaje(configMessage);
    sendMessageToPlatform(configMessage);
    
}