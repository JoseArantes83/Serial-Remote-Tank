#include <serial-remote-tank.h>
#include <lcd_8bits_4x20_Mod.c>

int1 processo_ligado = 0, fimTempo = 0;
int1 valvulaEntrada = 0, valvulaSaida = 0, heater = 0, cooler = 0, mixer = 0, sensorAlto = 0, sensorBaixo = 0;
int1  temperaturaAtual = 0, temperaturaSP = 0, temperaturaBaixa = 0, temperaturaAlta = 0,
int8 tela = 1, volume = 0, contador = 0, temperaturaMinima = 0, temperaturaMaxima = 0;

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
void mostraDados(int8 num_tela);
void mudaTela();

#INT_TIMER0
void TIMER0_isr(void)
{
   contador++;
   if (contador == 100)
   {
      contador = 0;
      fimTempo = 1;
   }
}

void main()
{
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_8 | RTCC_8_BIT); // 2,0 ms overflow

   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);

   lcd_init();

   while (TRUE)
   {

      if (fimTempo)
      {
         fimTempo = 0;

         switch (tela)
         {
         case 1:
            lcd_gotoxy(1, 1);
            printf(lcd_write_dat, "Pwr:");
            lcd_gotoxy(1, 2);
            printf(lcd_write_dat, "ValEnt:  ValSai:");
            lcd_gotoxy(1, 3);
            printf(lcd_write_dat, "Heater:  Cooler:");
            lcd_gotoxy(1, 4);
            printf(lcd_write_dat, "Mixer:");
            mostraDados(1);
            break;
         case 2:
            lcd_gotoxy(1, 1);
            printf(lcd_write_dat, "NvlAlt:  NvlBai:");
            lcd_gotoxy(1, 2);
            printf(lcd_write_dat, "TempBai:  TempAlt:");
            lcd_gotoxy(1, 3);
            printf(lcd_write_dat, "Volume:  Cooler:");
            lcd_gotoxy(1, 4);
            printf(lcd_write_dat, "TempAtua:  TempSP:");
            mostraDados(2);
            break;
         case 3:
            lcd_gotoxy(1, 1);
            printf(lcd_write_dat, "Histerese: ");
            lcd_gotoxy(1, 2);
            printf(lcd_write_dat, "TempoMistura: ");
            lcd_gotoxy(1, 3);
            printf(lcd_write_dat, "TempMin:");
            lcd_gotoxy(1, 4);
            printf(lcd_write_dat, "TempMax:");
            mostraDados(3);
            break;
         case 4:
            mostraDados(4);
            break;
         }
      }

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

void mudaTela()
{
   tela++;
   if (tela > 4)
   {
      tela = 1;
   }
}

void mostraDados(int8 num_tela)
{
   switch (num_tela)
   {
   case 1:
      lcd_gotoxy(5, 1);
      printf(lcd_write_dat, "%i", processo_ligado);
      lcd_gotoxy(8, 2);
      printf(lcd_write_dat, "%i", valvulaEntrada);
      lcd_gotoxy(17, 2);
      printf(lcd_write_dat, "%i", valvulaSaida);
      lcd_gotoxy(8, 3);
      printf(lcd_write_dat, "%i", heater);
      lcd_gotoxy(17, 3);
      printf(lcd_write_dat, "%i", cooler);
      lcd_gotoxy(7, 4);
      printf(lcd_write_dat, "%i", mixer);
      break;
   case 2:
      lcd_gotoxy(8, 1);
      printf(lcd_write_dat, "%i", sensorAlto);
      lcd_gotoxy(18, 1);
      printf(lcd_write_dat, "%i", sensorBaixo);
      lcd_gotoxy(9, 2);
      printf(lcd_write_dat, "%i", temperaturaMinima);
      lcd_gotoxy(19, 2);
      printf(lcd_write_dat, "%i", temperaturaMaxima);
      lcd_gotoxy(8, 3);
      printf(lcd_write_dat, "%i", volume);
      lcd_gotoxy(16, 3);
      printf(lcd_write_dat, "%i", cooler);
      lcd_gotoxy(7, 4);
      printf(lcd_write_dat, "%i", temperaturaAtual);
      lcd_gotoxy(17, 4);
      printf(lcd_write_dat, "%i", temperaturaSP);
      break;
   case 3:
      lcd_gotoxy(8, 1);
      printf(lcd_write_dat, "%i", sensorAlto);
      lcd_gotoxy(18, 1);
      printf(lcd_write_dat, "%i", sensorBaixo);
      lcd_gotoxy(9, 2);
      printf(lcd_write_dat, "%i", temperaturaMinima);
      lcd_gotoxy(19, 2);
      printf(lcd_write_dat, "%i", temperaturaMaxima);
      lcd_gotoxy(8, 3);
      printf(lcd_write_dat, "%i", volume);
      lcd_gotoxy(16, 4);
      printf(lcd_write_dat, "%i", cooler);
      break;
   case 4:
      lcd_gotoxy(5, 1);
      printf(lcd_write_dat, "");
      lcd_gotoxy(8, 2);
      printf(lcd_write_dat, "");
      lcd_gotoxy(17, 2);
      printf(lcd_write_dat, "");
      lcd_gotoxy(8, 3);
      printf(lcd_write_dat, "");
      lcd_gotoxy(17, 3);
      printf(lcd_write_dat, "");
      lcd_gotoxy(7, 4);
      printf(lcd_write_dat, "");
      break;
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
