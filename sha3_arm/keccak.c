#include <string.h>
#include "keccakH.h"

// ////////////////////////////////////////////////////////////////////////////
// Зі слів (смуг) у байти (змінення послідовності бітів)
void fromWordsToBytes(void)
{
  __asm("PUSH {R4}");
  
  // R0 = state
  __asm("LDR    R0, ??DataTable1_1");
  
  // R4 = msgBlock
  __asm("LDR    R4, ??DataTable1");
  
  // Лічильник для циклу
  __asm("MOV   R12, #0");
  
  // Пробігаємо байти блоку повідомлення
  __asm("Bit_Interleaving_circle:");
  
  // R1, R2, R3 містять байти для перестановки
  __asm("LDMIA  R0!, {R2-R3}");
  __asm("MOV    R1, R2");
  
  __asm("BFI    R2, R3, #16, #16");
  __asm("BFC    R3, #0, #16");
  __asm("ORR    R3, R3, R1, LSR #16");
  
  __asm("EOR    R1, R2, R2, LSR #8");
  __asm("AND    R1, R1, #0x0000FF00");
  __asm("EOR    R2, R2, R1");
  __asm("EOR    R2, R2, R1, LSL #8");
  
  __asm("EOR    R1, R2, R2, LSR #4");
  __asm("AND    R1, R1, #0x00F000F0");
  __asm("EOR    R2, R2, R1");
  __asm("EOR    R2, R2, R1, LSL #4");
  
  __asm("EOR    R1, R2, R2, LSR #2");
  __asm("AND    R1, R1, #0x0C0C0C0C");
  __asm("EOR    R2, R2, R1");
  __asm("EOR    R2, R2, R1, LSL #2");
  
  __asm("EOR    R1, R2, R2, LSR #1");
  __asm("AND    R1, R1, #0x22222222");
  __asm("EOR    R2, R2, R1");
  __asm("EOR    R2, R2, R1, LSL #1");
  
  __asm("EOR    R1, R3, R3, LSR #8");
  __asm("AND    R1, R1, #0x0000FF00");
  __asm("EOR    R3, R3, R1");
  __asm("EOR    R3, R3, R1, LSL #8");
  
  __asm("EOR    R1, R3, R3, LSR #4");
  __asm("AND    R1, R1, #0x00F000F0");
  __asm("EOR    R3, R3, R1");
  __asm("EOR    R3, R3, R1, LSL #4");
  
  __asm("EOR    R1, R3, R3, LSR #2");
  __asm("AND    R1, R1, #0x0C0C0C0C");
  __asm("EOR    R3, R3, R1");
  __asm("EOR    R3, R3, R1, LSL #2");
  
  __asm("EOR    R1, R3, R3, LSR #1");
  __asm("AND    R1, R1, #0x22222222");
  __asm("EOR    R3, R3, R1");
  __asm("EOR    R3, R3, R1, LSL #1");
  
  __asm("STMIA  R4!, {R2-R3}");
  
  // Порівняти з CAPACITY_IN_LANES = 9
  __asm("CMP   R12, #8");
  
  // і++
  __asm("ADD   R12, R12, #1");
  
  __asm("BNE.W Bit_Interleaving_circle");
  
  __asm("POP {R4}");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// Доповнення останнього блоку
void padding(uint8_t tempLen)
{
  // Якщо лишається рівно-більше два вільні байти
  if ( (tempLen <= (RATE_IN_BYTES-2)) && (tempLen != 0) )
  {
    msgBlock[tempLen++] = 0x01;		// Додаємо 0х01
    for ( ; tempLen<(RATE_IN_BYTES-1); tempLen++) // Додаємо нулі
    {
      msgBlock[tempLen] = 0x00;
    }
    msgBlock[tempLen] = 0x80;		// Додаємо останню 0х08
  }
  // В іншому випадку
  else
  {
    // Якщо залишається один вільний байт
    if (tempLen == (RATE_IN_BYTES-1))
    {
      msgBlock[tempLen] = 0x81;         // Додаємо 0х81
    }
    // В іншому випадку немає вільних байтів, наступний блок: 0х01,0х00,...,0х80
    else
    {
      tempLen = 0;
      msgBlock[tempLen++] = 0x01;	// Додаємо 0х01
      for ( ; tempLen<(RATE_IN_BYTES-1); tempLen++) // Додаємо нулі
      {
        msgBlock[tempLen] = 0x00;
      }
      msgBlock[tempLen] = 0x80;	// Додаємо останню 0х08
    }
  }
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void stateInit (void)
{
  memset(state, 0, 50);
}
// ////////////////////////////////////////////////////////////////////////////
