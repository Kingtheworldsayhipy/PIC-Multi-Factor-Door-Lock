#include "../app.h"
#define S_MIN 80
#define S_MAX 350
#define L_MIN 500
#define L_MAX 1100
static char code[4][6]; static int8 d=0,n=0; static int1 prev=0,active=0,digit_done=0; static int16 held_ms=0,gap_ms=0; static int32 last_sample=0;
static void reset_morse(void){ int8 i; for(i=0;i<4;i++) code[i][0]=0; d=0;n=0;active=0;digit_done=0;held_ms=0;gap_ms=0; }
static void add(char c){ if(d<4&&n<5){code[d][n++]=c;code[d][n]=0;} }
static int1 decode(int8 x,char *o){ if(!strcmp(code[x],".----"))*o='1'; else if(!strcmp(code[x],"..---"))*o='2'; else if(!strcmp(code[x],"...--"))*o='3'; else if(!strcmp(code[x],"....-"))*o='4'; else return 0; return 1; }
void morse_init(void){ set_tris_a(0x01); reset_morse(); prev=0; }
AUTH_RESULT morse_task(void){ int1 p; char out[5]; int8 i; int16 dur; if(system_locked||millis-last_sample<10)return AUTH_NONE; last_sample=millis; p=!input(MORSE_BUTTON);
 if(p&&!prev){active=1;held_ms=0;gap_ms=0;} if(p&&held_ms<1500)held_ms+=10;
 if(!p&&prev){dur=held_ms;if(dur>=S_MIN&&dur<=S_MAX)add('.');else if(dur>=L_MIN&&dur<=L_MAX)add('-');else reset_morse();gap_ms=0;digit_done=0;}
 if(active&&!p){gap_ms+=10;if(gap_ms>=900&&!digit_done&&n>0){if(d<3){d++;n=0;digit_done=1;}} if(gap_ms>=2200){if(d!=3||n==0){reset_morse();prev=p;return AUTH_FAILED;} for(i=0;i<4;i++)if(!decode(i,&out[i])){reset_morse();prev=p;return AUTH_FAILED;} out[4]=0; reset_morse(); prev=p; return !strcmp(out,"1234")?AUTH_SUCCESS:AUTH_FAILED;}}
 prev=p; return AUTH_NONE; }
