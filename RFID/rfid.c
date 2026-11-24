#include "../app.h"
#define CMD 0x01
#define IRQ 0x04
#define ERR 0x06
#define FIFO 0x09
#define LEVEL 0x0A
#define FRAME 0x0D
#define TXCTRL 0x14
#define MODE 0x11
#define TMODE 0x2A
#define TPRES 0x2B
#define TRELOADH 0x2C
#define TRELOADL 0x2D
#define TXASK 0x15
#define IDLE 0x00
#define TRANSCEIVE 0x0C
static int1 latched=0; static int8 uid[4];
/* Replace with the actual 4-byte UIDs of the two authorized cards. */
static int8 allowed[2][4]={{0xA1,0xB2,0xC3,0xD4},{0x11,0x22,0x33,0x44}};
static void wrbyte(int8 v){int8 i;for(i=0;i<8;i++){output_bit(PIN_B1,(v&0x80)!=0);output_low(PIN_B0);output_high(PIN_B0);v<<=1;}}
static int8 rdbyte(void){int8 i,v=0;for(i=0;i<8;i++){output_low(PIN_B0);v<<=1;if(input(PIN_B2))v|=1;output_high(PIN_B0);}return v;}
static void wr(int8 r,int8 v){output_low(PIN_B3);wrbyte((r<<1)&0x7E);wrbyte(v);output_high(PIN_B3);}
static int8 rd(int8 r){int8 v;output_low(PIN_B3);wrbyte(((r<<1)&0x7E)|0x80);v=rdbyte();output_high(PIN_B3);return v;}
static void setb(int8 r,int8 m){wr(r,rd(r)|m);} static void clrb(int8 r,int8 m){wr(r,rd(r)&(~m));}
static int1 trans(int8 *tx,int8 tn,int8 *rx,int8 *rn){int8 i,q,e;wr(CMD,IDLE);wr(IRQ,0x7F);wr(LEVEL,0x80);for(i=0;i<tn;i++)wr(FIFO,tx[i]);wr(CMD,TRANSCEIVE);setb(FRAME,0x80);i=0;do{q=rd(IRQ);i++;}while(!(q&0x30)&&!(q&1)&&i<100);clrb(FRAME,0x80);if(i>=100||(q&1))return 0;e=rd(ERR);if(e&0x1B)return 0;*rn=rd(LEVEL);if(*rn>16)return 0;for(i=0;i<*rn;i++)rx[i]=rd(FIFO);return 1;}
static int1 readuid(int8 *u){int8 tx[2],rx[16],n,i;tx[0]=0x26;wr(FRAME,7);if(!trans(tx,1,rx,&n)||n!=2)return 0;tx[0]=0x93;tx[1]=0x20;wr(FRAME,0);if(!trans(tx,2,rx,&n)||n!=5)return 0;if((rx[0]^rx[1]^rx[2]^rx[3])!=rx[4])return 0;for(i=0;i<4;i++)u[i]=rx[i];return 1;}
void rfid_init(void){set_tris_b(0x04);output_high(PIN_B3);output_high(PIN_B0);output_high(PIN_B4);output_low(PIN_B1);output_low(PIN_B4);delay_ms(5);output_high(PIN_B4);delay_ms(5);wr(CMD,0x0F);delay_ms(50);wr(TMODE,0x8D);wr(TPRES,0x3E);wr(TRELOADL,30);wr(TRELOADH,0);wr(TXASK,0x40);wr(MODE,0x3D);setb(TXCTRL,3);latched=0;}
AUTH_RESULT rfid_task(void){int8 i,j;int1 ok;if(system_locked)return AUTH_NONE;if(!readuid(uid)){latched=0;return AUTH_NONE;}if(latched)return AUTH_NONE;latched=1;for(i=0;i<2;i++){ok=1;for(j=0;j<4;j++)if(uid[j]!=allowed[i][j])ok=0;if(ok)return AUTH_SUCCESS;}return AUTH_FAILED;}
