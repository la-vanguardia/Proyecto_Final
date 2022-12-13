
#include "mcc_generated_files/uart3.h"


//Aca se encuentran las funciones para comunicacion de la plataforma

#define STOP_COMMAND "STOP"
#define START_COMMAND "START"
#define CONFIG_COMMAND "CONFIG"
#define CONTINUE_COMMAND "CONTINUE"

#define MEASURE_X_COMMAND "MEASURE_X"
#define MEASURE_Y_COMMAND "MEASURE_Y"
#define MEASURE_INIT_COMMAND "MEASURE_INIT"
#define MEASURE_END_COMMAND "MEASURE_END"

unsigned char updateY = 0;
unsigned char updateX = 0;
unsigned char measureInit = 0;
unsigned char measureEnd = 0;

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
    sprintf(configMessage, "CONFIG-%.2f-%.2f", h, k);
    sendMessageToPlatform(configMessage);
    
}