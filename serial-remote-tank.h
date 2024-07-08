#include <16F877A.h>
#device ADC = 10

#FUSES PUT       // Power Up Timer
#FUSES BROWNOUT  // Reset when brownout detected
#FUSES NOLVP     // No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOCPD     // No EE protection
#FUSES NOWRT     // Program memory not write protected
#FUSES NOPROTECT // Code not protected from reading

#use delay(crystal = 4MHz)
#use FIXED_IO(B_outputs = PIN_B6)

#define B_LIGA PIN_B0
#define B_DESLIGA PIN_B1
#define B_TROCA PIN_B2
#define B_SEL PIN_B3
#define B_MAIS PIN_B4
#define B_MENOS PIN_B5
#define BUZZER PIN_B6

#use rs232(baud = 9600, parity = N, xmit = PIN_C6, rcv = PIN_C7, bits = 8, stream = PORT1)
