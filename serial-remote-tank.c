#include <serial-remote-tank.h>
#include <lcd_8bits.c>

#INT_TIMER0
void  TIMER0_isr(void) 
{

}

void main()
{
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_8|RTCC_8_BIT);      //2,0 ms overflow


   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);
   
   lcd_init();

   while(TRUE)
   {


      //TODO: User Code
   }

}
