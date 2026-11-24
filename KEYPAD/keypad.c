#include "../app.h"
static char map[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
static char buf[7], npass[7];
static int8 n=0, mode=0, row=0, stable=0;
static int1 held=0;
static int32 last_scan=0;
static void clearbuf(void){ n=0; buf[0]=0; }
static void loadpass(void){ int8 i,c; for(i=0;i<6;i++){ c=read_eeprom(i); if(c<'0'||c>'9'){ strcpy(system_password,DEFAULT_PASSWORD); for(i=0;i<6;i++) write_eeprom(i,system_password[i]); system_password[6]=0; return; } system_password[i]=c; } system_password[6]=0; }
static char scan_key(void){ int8 r,c; int8 rp[4]={PIN_D0,PIN_D1,PIN_D2,PIN_D3}; int8 cp[4]={PIN_D4,PIN_D5,PIN_D6,PIN_D7}; for(r=0;r<4;r++) output_high(rp[r]); for(r=0;r<4;r++){ output_low(rp[r]); delay_us(20); for(c=0;c<4;c++) if(!input(cp[c])){ output_high(rp[r]); return map[r][c]; } output_high(rp[r]); } return 0; }
void keypad_init(void){ set_tris_d(0xF0); loadpass(); clearbuf(); mode=0; held=0; stable=0; }
static AUTH_RESULT handle(char k){ int8 i; if(k=='*'){ clearbuf(); lcd_put_line(1,"Input cleared"); return AUTH_NONE; } if(k=='A' && n==0 && mode==0){ mode=1; lcd_put_line(1,"Old password"); lcd_put_line(2,"Enter then #"); return AUTH_NONE; }
 if(k>='0'&&k<='9'&&n<6){ buf[n++]=k; buf[n]=0; lcd_put_at(2,n-1,"*"); return AUTH_NONE; }
 if(k!='#') return AUTH_NONE; buf[n]=0;
 if(mode==0){ if(n!=6){ clearbuf(); return AUTH_NONE; } if(!strcmp(buf,system_password)){ clearbuf(); return AUTH_SUCCESS; } clearbuf(); return AUTH_FAILED; }
 if(mode==1){ if(!strcmp(buf,system_password)){ mode=2; clearbuf(); lcd_put_line(1,"New password"); lcd_put_line(2,"Enter 6 digits"); } else { mode=0; clearbuf(); lcd_put_line(1,"Old password wrong"); } return AUTH_NONE; }
 if(mode==2){ if(n==6){ strcpy(npass,buf); mode=3; lcd_put_line(1,"Confirm password"); } clearbuf(); return AUTH_NONE; }
 if(mode==3){ if(n==6 && !strcmp(buf,npass)){ for(i=0;i<6;i++){ system_password[i]=npass[i]; write_eeprom(i,system_password[i]); } system_password[6]=0; lcd_put_line(1,"Password saved"); } else lcd_put_line(1,"Not matched"); mode=0; clearbuf(); return AUTH_NONE; }
 return AUTH_NONE; }
AUTH_RESULT keypad_task(void){ char k; if(system_locked || millis-last_scan<10) return AUTH_NONE; last_scan=millis; k=scan_key(); if(!k){ held=0; stable=0; return AUTH_NONE; } if(held) return AUTH_NONE; if(++stable>=2){ held=1; stable=0; return handle(k); } return AUTH_NONE; }
