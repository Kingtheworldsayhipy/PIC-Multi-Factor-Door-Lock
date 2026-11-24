#include "../app.h"
int1 system_locked=0, door_is_open=0;
int8 wrong_count=0;
char system_password[7] = DEFAULT_PASSWORD;
volatile int32 millis=0;
static int32 lock_started=0, door_started=0, last_tick=0;
#int_TIMER0
void timer0_isr(void){ set_timer0(100); millis++; }
void auth_init(void)
{
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_32); set_timer0(100);
   enable_interrupts(INT_TIMER0); enable_interrupts(GLOBAL);
   system_locked=0; door_is_open=0; wrong_count=0; millis=0;
}
void auth_process(AUTH_RESULT r)
{
   if(r==AUTH_NONE || system_locked) return;
   if(r==AUTH_SUCCESS){ wrong_count=0; door_is_open=1; door_started=millis; servo_open(); buzzer_success(); lcd_put_line(1,"ACCESS GRANTED"); lcd_put_line(2,"Door opening"); }
   else if(r==AUTH_FAILED){ wrong_count++; buzzer_error(); if(wrong_count>=MAX_WRONG_ATTEMPTS){ system_locked=1; lock_started=millis; lcd_put_line(1,"SYSTEM LOCKED"); lcd_put_line(2,"Wait 30 seconds"); } else { lcd_put_line(1,"ACCESS DENIED"); lcd_put_line(2,"Try again"); } }
}
void auth_display_task(void)
{
   int8 remain; char s[17];
   if(system_locked){ remain=LOCKOUT_SECONDS-(int8)((millis-lock_started)/1000); sprintf(s,"Wait: %02u sec",remain); lcd_put_line(2,s); }
   if(system_locked && (millis-lock_started)>=(LOCKOUT_SECONDS*1000L)){ system_locked=0; wrong_count=0; lcd_put_line(1,"System ready"); lcd_put_line(2,"Authenticate"); }
   if(door_is_open && (millis-door_started)>=(DOOR_OPEN_SECONDS*1000L)){ door_is_open=0; servo_close(); lcd_put_line(1,"PIC DOOR LOCK"); lcd_put_line(2,"Door closed"); }
}
