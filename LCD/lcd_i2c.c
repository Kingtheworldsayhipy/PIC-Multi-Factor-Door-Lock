#include "../app.h"
#use i2c(Master,Fast=100000,sda=PIN_C4,scl=PIN_C3,stream=LCD_I2C)
#define LCD_ADDR 0x27
#define RS 0x01
#define EN 0x04
#define BL 0x08
static void expander(int8 x){i2c_start(LCD_I2C);i2c_write(LCD_I2C,LCD_ADDR<<1);i2c_write(LCD_I2C,x|BL);i2c_stop(LCD_I2C);}
static void pulse(int8 x){expander(x|EN);delay_us(2);expander(x&(~EN));delay_us(50);}
static void nibble(int8 x,int1 rs){x=(x&0xF0)|(rs?RS:0);expander(x);pulse(x);}
static void sendbyte(int8 x,int1 rs){nibble(x,rs);nibble(x<<4,rs);}
static void command(int8 x){sendbyte(x,0);if(x==1||x==2)delay_ms(2);}
void lcd_init(void){delay_ms(50);nibble(0x30,0);delay_ms(5);nibble(0x30,0);delay_us(150);nibble(0x30,0);nibble(0x20,0);command(0x28);command(0x0C);command(0x06);command(0x01);}
void lcd_clear(void){command(1);}
void lcd_put_at(int8 row,int8 col,char *s){int8 a=(row==1?0x80:0xC0)+col;command(a);while(*s)sendbyte(*s++,1);}
void lcd_put_line(int8 row,char *s){int8 n=strlen(s);lcd_put_at(row,0,s);while(n++<16)sendbyte(' ',1);}
