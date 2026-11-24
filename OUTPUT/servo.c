#include "../app.h"
static int1 enabled=0; static int32 last_frame=0;
void servo_init(void){output_low(SERVO_PIN);enabled=0;}
void servo_open(void){enabled=1;}
void servo_close(void){enabled=0;output_low(SERVO_PIN);}
void servo_task(void){ if(!enabled)return; if(millis-last_frame>=20){last_frame=millis;output_high(SERVO_PIN);delay_us(1500);output_low(SERVO_PIN);} }
