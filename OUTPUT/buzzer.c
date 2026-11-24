#include "../app.h"
static int8 mode=0; static int32 start=0;
void buzzer_init(void){output_low(BUZZER_PIN);mode=0;}
void buzzer_success(void){mode=1;start=millis;}
void buzzer_error(void){mode=2;start=millis;}
void buzzer_task(void){int32 t=millis-start;if(!mode){output_low(BUZZER_PIN);return;} if(mode==1){output_high(BUZZER_PIN);if(t>=120){output_low(BUZZER_PIN);mode=0;}}else{if((t/100)%2)output_high(BUZZER_PIN);else output_low(BUZZER_PIN);if(t>=400){output_low(BUZZER_PIN);mode=0;}}}
