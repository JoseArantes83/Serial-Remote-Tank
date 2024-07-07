#include <serial-remote-tank.h>
#include <lcd_8bits.c>

void abreValvulaEntrada();
void fechaValvulaEntrada();
void abreValvulaSaida();
void fechaValvulaSaida();
void ligaHeater();
void desligaHeater();
void ligaFan();
void desligaFan();
void ligaStirrer();
void desligaStirrer();

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
      
   }

}

void abreValvulaEntrada(){  // Validado
   putc(0x00);
   putc(0x01);
}

void fechaValvulaEntrada(){  // Validado
   putc(0x00);
   putc(0x00);
}
 
void abreValvulaSaida(){  // Validado
   putc(0x01);
   putc(0x01);
}

void fechaValvulaSaida(){  // Validado
   putc(0x01);
   putc(0x00);
}

void ligaHeater(){
   putc(0x02);
   putc(0x01);
}

void desligaHeater(){
   putc(0x02);
   putc(0x00);
}

void ligaFan(){
   putc(0x03);
   putc(0x01);
}

void desligaFan(){
   putc(0x03);
   putc(0x00);
}

void ligaStirrer(){
   putc(0x04);
   putc(0x01);
}

void desligaStirrer(){
   putc(0x04);
   putc(0x00);
}
