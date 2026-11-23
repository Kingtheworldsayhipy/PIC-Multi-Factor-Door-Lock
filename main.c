#include "config.h"
#include "app.h"
#use rs232(UART1, baud=9600, bits=8, parity=N, stop=1, stream=BT)
#include "SYSTEM/auth.c"
#include "LCD/lcd_i2c.c"
#include "OUTPUT/servo.c"
#include "OUTPUT/buzzer.c"
#include "KEYPAD/keypad.c"
#include "RFID/rfid.c"
#include "MORSE/morse.c"
#include "BLUETOOTH/bluetooth.c"

void main(void)
{
   AUTH_RESULT r;
   setup_adc(ADC_OFF);
   setup_adc_ports(NO_ANALOGS);
   setup_comparator(NC_NC_NC_NC);
   set_tris_a(0x01); set_tris_b(0x04); set_tris_c(0x80); set_tris_d(0xF0);
   output_low(SERVO_PIN); output_low(BUZZER_PIN);
   lcd_init(); servo_init(); buzzer_init(); auth_init();
   keypad_init(); rfid_init(); morse_init(); bluetooth_init();
   lcd_put_line(1,"PIC DOOR LOCK"); lcd_put_line(2,"Ready");
   while(TRUE)
   {
      r=keypad_task(); auth_process(r);
      r=rfid_task(); auth_process(r);
      r=morse_task(); auth_process(r);
      r=bluetooth_task(); auth_process(r);
      servo_task(); buzzer_task(); auth_display_task();
      delay_ms(1);
   }
}
