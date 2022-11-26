

//Aca se encuentran las funciones para comunicacion de la plataforma

#define STOP_COMMAND "STOP"
#define START_COMMAND "START"
#define CONFIG_COMMAND "CONFIG"

void sendStart();
void sendConfig(double h, double k);
void sendStop();
void sendCommand(char* command);

