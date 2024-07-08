#include <serial-remote-tank.h>
#include <lcd_8bits_4x20_Mod.c>

int1 processo_ligado = 0;
int1 valvulaEntrada = 0, valvulaSaida = 0, heater = 0, cooler = 0, mixer = 0, sensorAlto = 0, sensorBaixo = 0;

void abreValvulaEntrada();
void fechaValvulaEntrada();
void abreValvulaSaida();
void fechaValvulaSaida();
void ligaHeater();
void desligaHeater();
void ligaCooler();
void desligaCooler();
void ligaMixer();
void desligaMixer();
int1 getSensorBaixo();
int16 getTemperatura();
int1 getSensorAlto();

#INT_TIMER0
void TIMER0_isr(void)
{
}

void main()
{
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_8 | RTCC_8_BIT); // 2,0 ms overflow

   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);

   lcd_init();
   
   lcd_gotoxy(1, 1);
   printf(lcd_write_dat, "Pwr:");
   

   while (TRUE)
   {
      lcd_gotoxy(5, 1);
      printf(lcd_write_dat, "%i", processo_ligado);
      // criar booleanos de estado
      // testar se t� vazio
      // se n tive esvazia
      // testar se chegou no topo, se tiver, desliga a valvula de cima
      // e liga o heater
      // se chegar na temperatura desliga o heater, sen�o
      // continua esquentando.
      // liga o mixer
      // se n�o tiver dado o tempo, testa se temperatura est� adequada
      // e continua a partir da�
      // sen�o liga a valvula baixo
      // desliga heater
      // desliga mixer
      // se sensor baixo for 0, desliga valvula baixo e come�a de novo.
   }
}

void abreValvulaEntrada()
{ // Validado
   putc(0x00);
   putc(0x01);
}

void fechaValvulaEntrada()
{ // Validado
   putc(0x00);
   putc(0x00);
}

void abreValvulaSaida()
{ // Validado
   putc(0x01);
   putc(0x01);
}

void fechaValvulaSaida()
{ // Validado
   putc(0x01);
   putc(0x00);
}

void ligaHeater()
{
   putc(0x02);
   putc(0x01);
}

void desligaHeater()
{
   putc(0x02);
   putc(0x00);
}

void ligaCooler()
{
   putc(0x03);
   putc(0x01);
}

void desligaCooler()
{
   putc(0x03);
   putc(0x00);
}

void ligaMixer()
{
   putc(0x04);
   putc(0x01);
}

void desligaMixer()
{
   putc(0x04);
   putc(0x00);
}

int1 getSensorBaixo()
{
   putc(0x11);
   return getc();
}

int16 getTemperatura()
{
   putc(0x31);
   return getc();
}

int1 getSensorAlto()
{
   putc(0x10);
   return getc();
}
