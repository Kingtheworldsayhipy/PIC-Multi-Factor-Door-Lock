#include "../app.h"
static char rxbuf[25]; static int8 idx=0; static int1 overflow=0;
void bluetooth_init(void){idx=0;overflow=0;rxbuf[0]=0;}
AUTH_RESULT bluetooth_task(void){ char c; AUTH_RESULT r=AUTH_NONE; if(system_locked||!kbhit(BT))return AUTH_NONE; c=fgetc(BT); if(c=='\r')return AUTH_NONE; if(c=='\n'){rxbuf[idx]=0; if(!overflow&& !strncmp(rxbuf,"AUTH:",5) && !strcmp(rxbuf+5,system_password))r=AUTH_SUCCESS; else if(idx>0)r=AUTH_FAILED; idx=0;overflow=0;rxbuf[0]=0;return r;} if(idx<24)rxbuf[idx++]=c; else overflow=1; return AUTH_NONE; }
