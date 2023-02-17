
#ifndef APP
#define APP
#include "../Server/server.h"


EN_cardError_t getCardData(ST_cardData_t* cardData);
EN_terminalError_t getTerminalData(ST_terminalData_t* termData);
void appStart(void);
#endif // !APP.h

