#include <serial-remote-tank.h>
#include <lcd_8bits_4x20_Mod.c>

int1 processo_ligado = 0, fimTempo = 0;
int1 valvulaEntrada = 0, valvulaSaida = 0, heater = 0, cooler = 0, mixer = 0, sensorAlto = 0, sensorBaixo = 0, quente = 0;
int16 temperaturaSP = 0, temperaturaBaixa = 0, temperaturaAlta = 0;
int8 tela = 1, volume = 0, contador = 0, temperaturaMinima = 0, temperaturaMaxima = 0, processo = 0;
int1 fimMistura = 0;
int8 tempoMistura = 0;
int16 tempo_ms = 5000, contaMistura = 0, temperaturaAtual = 0;
float conversao = 1.0 / 256.0;

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
void setTemperaturaEsperada(int16 temp);
int1 verificaQuente();

#INT_TIMER0
void TIMER0_isr(void)
{
   contador++;
   if (contador == 1000)
   {
      contador = 0;
      fimTempo = 1;
   }
   
   contaMistura++;
   
   if(contaMistura >= tempo_ms){
   
      fimMistura = 1;
      
      contaMistura = 0;
   
   }
}

void main()
{
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_8 | RTCC_8_BIT); // 2,0 ms overflow

   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);

   lcd_init();
   
   temperaturaSP = 30;
      
   setTemperaturaEsperada(temperaturaSP);

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
      
      if(processo == 0){
      
         sensorAlto = getSensorAlto();
      
         if(sensorAlto == 0){
         
            if(valvulaEntrada == 0)
               abreValvulaEntrada();
         
         }
         
         else{
         
            fechaValvulaEntrada();
            
            processo++;
         
         }
      
      }
      
      if(processo == 1){
      
         quente = verificaQuente();
      
         if(quente == 0){
         
            ligaHeater();
         
         }
         
         else{
         
            desligaHeater();
            
            processo++;
         
         }
      
      }
      
      if(processo == 2){
      
         if(mixer == 0){
      
            fimMistura = 0;
            
            contaMistura = 0;
            
            ligaMixer();
         
         }
      
         if(fimMistura == 1){
         
            desligaMixer();
            
            fimMistura = 0;
            
            processo++;
            
         }
      
      }
      
      if(processo == 3){
      
         sensorAlto = getSensorAlto();
      
         if(sensorAlto == 1){
         
            abreValvulaSaida();
         
         }
         
         else{
         
            sensorBaixo = getSensorBaixo();
         
            if(sensorBaixo == 0){
            
               fechaValvulaSaida();
               
               processo = 0;
            
            }
         
         }
      
      }

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
      printf(lcd_write_dat, "%lu", temperaturaAtual);
      lcd_gotoxy(17, 4);
      printf(lcd_write_dat, "%lu", temperaturaSP);
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
   
   valvulaEntrada = 1;
}

void fechaValvulaEntrada()
{ // Validado
   putc(0x00);
   putc(0x00);
   
   valvulaEntrada = 0;
}

void abreValvulaSaida()
{ // Validado
   putc(0x01);
   putc(0x01);
   
   valvulaSaida = 1;
}

void fechaValvulaSaida()
{ // Validado
   putc(0x01);
   putc(0x00);
   
   valvulaSaida = 0;
}

void ligaHeater()
{
   putc(0x02);
   putc(0x01);
   
   heater = 1;
}

void desligaHeater()
{
   putc(0x02);
   putc(0x00);
   
   heater = 0;
}

void ligaCooler()
{
   putc(0x03);
   putc(0x01);
   
   cooler = 1;
}

void desligaCooler()
{
   putc(0x03);
   putc(0x00);
   
   cooler = 0;
}

void ligaMixer()
{
   putc(0x04);
   putc(0x01);
   
   mixer = 1;
}

void desligaMixer()
{
   putc(0x04);
   putc(0x00);
   
   mixer = 0;
}

int1 getSensorBaixo()
{
   putc(0x11);
   return getc();
}

int16 getTemperatura(){

   int8 valorh = 0, valorl = 0;

   putc(0x32); 
   valorh=getc(); 
   valorl=getc();
   
   return (valorh<<8)|valorl;
}

int1 getSensorAlto()
{
   putc(0x10);
   return getc();
}

void setTemperaturaEsperada(int16 temp){

   putc(0x21);
   putc((temp&0xFF00)>>8); 
   putc(temp&0x00FF);

}

int1 verificaQuente(){

   putc(0x13);
   return getc();

}
