#ifndef APP_H
#define APP_H
#include "config.h"
typedef enum { AUTH_NONE=0, AUTH_SUCCESS, AUTH_FAILED } AUTH_RESULT;
extern int1 system_locked, door_is_open;
extern int8 wrong_count;
extern char system_password[7];
extern volatile int32 millis;
void auth_init(void); void auth_process(AUTH_RESULT); void auth_display_task(void);
void lcd_init(void); void lcd_clear(void); void lcd_put_line(int8,char*); void lcd_put_at(int8,int8,char*);
void servo_init(void); void servo_open(void); void servo_close(void); void servo_task(void);
void buzzer_init(void); void buzzer_success(void); void buzzer_error(void); void buzzer_task(void);
void keypad_init(void); AUTH_RESULT keypad_task(void);
void rfid_init(void); AUTH_RESULT rfid_task(void);
void morse_init(void); AUTH_RESULT morse_task(void);
void bluetooth_init(void); AUTH_RESULT bluetooth_task(void);
#endif
