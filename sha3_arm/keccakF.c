#include "keccakH.h"
#include <string.h>

// ////////////////////////////////////////////////////////////////////////////
void KeccakF(void)
{
  __asm("PUSH {R4-R11}");
  
  // //////////////////////////////////////////////////////////////////////////
  // Bit interleaving, start
  
  // R0 = msgBlock
  __asm("LDR    R0, ??DataTable1_2");
  
  // R1 = state
  __asm("LDR    R1, ??DataTable1");
  
  // Лічильник для циклу
  __asm("MOV   R12, #0");
  
  // Пробігаємо байти блоку повідомлення
  __asm("Bit_Interleaving_circle:");
  
  // R7, R8 містять байти для перестановки
  __asm("LDMIA  R0!, {R5-R6}");
  
  // R3, R4 містять смугу state
  __asm("LDMIA  R1, {R3-R4}");
  
  __asm("MOV    R7, R5");
  __asm("MOV    R8, R6");
  
  // Low, parity:
  __asm("AND    R7, R7, #0x55555555");
  __asm("ORR    R7, R7, R7, LSR #1");
  __asm("AND    R7, R7, #0x33333333");
  __asm("ORR    R7, R7, R7, LSR #2");
  __asm("AND    R7, R7, #0x0F0F0F0F");
  __asm("ORR    R7, R7, R7, LSR #4");
  __asm("AND    R7, R7, #0x00FF00FF");
  __asm("BFI    R7, R7, #8, #8");
  __asm("EOR    R3, R3, R7, LSR #8");
  
  // High, parity:
  __asm("AND    R8, R8, #0x55555555");
  __asm("ORR    R8, R8, R8, LSR #1");
  __asm("AND    R8, R8, #0x33333333");
  __asm("ORR    R8, R8, R8, LSR #2");
  __asm("AND    R8, R8, #0x0F0F0F0F");
  __asm("ORR    R8, R8, R8, LSR #4");
  __asm("AND    R8, R8, #0x00FF00FF");
  __asm("ORR    R8, R8, R8, LSR #8");
  __asm("EOR    R3, R3, R8, LSL #16");
  
  // Low, odd:
  __asm("AND    R5, R5, #0xAAAAAAAA");
  __asm("ORR    R5, R5, R5, LSL #1");
  __asm("AND    R5, R5, #0xCCCCCCCC");
  __asm("ORR    R5, R5, R5, LSL #2");
  __asm("AND    R5, R5, #0xF0F0F0F0");
  __asm("ORR    R5, R5, R5, LSL #4");
  __asm("AND    R5, R5, #0xFF00FF00");
  __asm("ORR    R5, R5, R5, LSL #8");
  __asm("EOR    R4, R4, R5, LSR #16");
  
  // High, odd:
  __asm("AND    R6, R6, #0xAAAAAAAA");
  __asm("ORR    R6, R6, R6, LSL #1");
  __asm("AND    R6, R6, #0xCCCCCCCC");
  __asm("ORR    R6, R6, R6, LSL #2");
  __asm("AND    R6, R6, #0xF0F0F0F0");
  __asm("ORR    R6, R6, R6, LSL #4");
  __asm("AND    R6, R6, #0xFF00FF00");
  __asm("ORR    R6, R6, R6, LSL #8");
  __asm("LSR    R6, R6, #16");
  __asm("EOR    R4, R4, R6, LSL #16");
  
  __asm("STR    R3, [R1]");
  __asm("STR    R4, [R1, #4]");
  
  // Порівняти з RATE_IN_LANES = 16
  __asm("CMP   R12, #15");
  
  // і++
  __asm("ADD   R12, R12, #1");
  
  // Зсуваємо адресу state на +8
  __asm("ADD    R1, R1, #8");
  
  __asm("BNE.W Bit_Interleaving_circle");
  
  // Bit interleaving, end
  // //////////////////////////////////////////////////////////////////////////
  
  // //////////////////////////////////////////////////////////////////////////
  // Prepare C, start
  
  // R0 = state
  __asm("LDR    R0, ??DataTable1");
  
  // Завантажуємо перші рядки
  __asm("LDM    R0, {R1-R10}");
  
  //Завантажуємо по черзі і XOR
  __asm("LDRD  R11, R12, [R0, #40]");
  __asm("EOR    R1,  R1, R11");
  __asm("EOR    R2,  R2, R12");
  
  __asm("LDRD  R11, R12, [R0, #48]");
  __asm("EOR    R3,  R3, R11");
  __asm("EOR    R4,  R4, R12");
  
  __asm("LDRD  R11, R12, [R0, #56]");
  __asm("EOR    R5,  R5, R11");
  __asm("EOR    R6,  R6, R12");
  
  __asm("LDRD  R11, R12, [R0, #64]");
  __asm("EOR    R7,  R7, R11");
  __asm("EOR    R8,  R8, R12");
  
  __asm("LDRD  R11, R12, [R0, #72]");
  __asm("EOR    R9,  R9, R11");
  __asm("EOR   R10, R10, R12");
  
  __asm("LDRD  R11, R12, [R0, #80]");
  __asm("EOR    R1,  R1, R11");
  __asm("EOR    R2,  R2, R12");
  
  __asm("LDRD  R11, R12, [R0, #88]");
  __asm("EOR    R3,  R3, R11");
  __asm("EOR    R4,  R4, R12");
  
  __asm("LDRD  R11, R12, [R0, #96]");
  __asm("EOR    R5,  R5, R11");
  __asm("EOR    R6,  R6, R12");
  
  __asm("LDRD  R11, R12, [R0, #104]");
  __asm("EOR    R7,  R7, R11");
  __asm("EOR    R8,  R8, R12");
  
  __asm("LDRD  R11, R12, [R0, #112]");
  __asm("EOR    R9,  R9, R11");
  __asm("EOR   R10, R10, R12");
  
  __asm("LDRD  R11, R12, [R0, #120]");
  __asm("EOR    R1,  R1, R11");
  __asm("EOR    R2,  R2, R12");
  
  __asm("LDRD  R11, R12, [R0, #128]");
  __asm("EOR    R3,  R3, R11");
  __asm("EOR    R4,  R4, R12");
  
  __asm("LDRD  R11, R12, [R0, #136]");
  __asm("EOR    R5,  R5, R11");
  __asm("EOR    R6,  R6, R12");
  
  __asm("LDRD  R11, R12, [R0, #144]");
  __asm("EOR    R7,  R7, R11");
  __asm("EOR    R8,  R8, R12");
  
  __asm("LDRD  R11, R12, [R0, #152]");
  __asm("EOR    R9,  R9, R11");
  __asm("EOR   R10, R10, R12");
  
  __asm("LDRD  R11, R12, [R0, #160]");
  __asm("EOR    R1,  R1, R11");
  __asm("EOR    R2,  R2, R12");
  
  __asm("LDRD  R11, R12, [R0, #168]");
  __asm("EOR    R3,  R3, R11");
  __asm("EOR    R4,  R4, R12");
  
  __asm("LDRD  R11, R12, [R0, #176]");
  __asm("EOR    R5,  R5, R11");
  __asm("EOR    R6,  R6, R12");
  
  __asm("LDRD  R11, R12, [R0, #184]");
  __asm("EOR    R7,  R7, R11");
  __asm("EOR    R8,  R8, R12");
  
  __asm("LDRD  R11, R12, [R0, #192]");
  __asm("EOR    R9,  R9, R11");
  __asm("EOR   R10, R10, R12");
  
  // Формуємо D, перші половини
  // R0 = D
  __asm("LDR    R0, ??DataTable1_1");
  
  __asm("EOR   R11, R9, R4, ROR #31");
  __asm("STR   R11, [R0]");
  
  __asm("EOR   R11, R1, R6, ROR #31");
  __asm("STR   R11, [R0, #8]");
  
  __asm("EOR   R11, R3, R8, ROR #31");
  __asm("STR   R11, [R0, #16]");
  
  __asm("EOR   R11, R5, R10, ROR #31");
  __asm("STR   R11, [R0, #24]");
  
  __asm("EOR   R11, R7, R2, ROR #31");
  __asm("STR   R11, [R0, #32]");
  
  // Формуємо D, другі половини
  __asm("EOR   R11, R10, R3");
  __asm("STR   R11, [R0, #4]");
  
  __asm("EOR   R11, R2, R5");
  __asm("STR   R11, [R0, #12]");
  
  __asm("EOR   R11, R4, R7");
  __asm("STR   R11, [R0, #20]");
  
  __asm("EOR   R11, R6, R9");
  __asm("STR   R11, [R0, #28]");
  
  __asm("EOR   R11, R8, R1");
  __asm("STR   R11, [R0, #36]");
  
  // Prepare C, end
  // //////////////////////////////////////////////////////////////////////////
  
  // //////////////////////////////////////////////////////////////////////////
  
  // //////////////////////////////////////////////////////////////////////////
  // Тут починається цикл раундів
  
  __asm("MOVS R12, #0");
  __asm("KeccakF_circle:");
  __asm("PUSH {R12}");
  
  // //////////////////////////////////////////////////////////////////////////
  // theta (D ^ state) + rho + pi, start
  
  // R0 = D
  __asm("LDR    R0, ??DataTable1_1");
  
  // R1-R8 = D[0-3]
  __asm("LDM    R0, {R1-R8}");
  
  // R0 = state
  __asm("LDR    R0, ??DataTable1");
  
  // R9, R10 = temp1, R11, R12 = temp2
  
  // Load (state + 1), (state + 10)
  __asm("LDRD   R9, R10, [R0, #8]");
  __asm("LDRD  R11, R12, [R0, #80]");
  
  // (state + 1, 10) ^ (D + 1, 0)
  __asm("EOR    R9,  R3,  R9");
  __asm("EOR   R10,  R4, R10");
  __asm("ROR   R10, R10, #31");
  __asm("EOR   R11,  R1, R11");
  __asm("ROR   R11, R11, #31");
  __asm("EOR   R12,  R2, R12");
  __asm("ROR   R12, R12, #30");
  
  // Store (state + 1)
  __asm("STR    R9, [R0, #84]");
  __asm("STR   R10, [R0, #80]");
  
  // Load (state + 7)
  __asm("LDRD   R9, R10, [R0, #56]");
  
  // (state + 7) ^ (D + 2)
  __asm("EOR    R9,  R5,  R9");
  __asm("ROR    R9,  R9, #29");
  __asm("EOR   R10,  R6, R10");
  __asm("ROR   R10, R10, #29");
  
  // Store (state + 10)
  __asm("STR   R11, [R0, #60]");
  __asm("STR   R12, [R0, #56]");
  
  // Load (state + 11)
  __asm("LDRD  R11, R12, [R0, #88]");
  
  // (state + 11) ^ (D + 1)
  __asm("EOR   R11,  R3, R11");
  __asm("ROR   R11, R11, #27");
  __asm("EOR   R12,  R4, R12");
  __asm("ROR   R12, R12, #27");
  
  // Store (state + 7)
  __asm("STR    R9, [R0, #88]");
  __asm("STR   R10, [R0, #92]");
  
  // Load (state + 17)
  __asm("LDRD   R9, R10, [R0, #136]");
  
  // (state + 17) ^ (D + 2)
  __asm("EOR    R9,  R5,  R9");
  __asm("ROR    R9,  R9, #25");
  __asm("EOR   R10,  R6, R10");
  __asm("ROR   R10, R10, #24");
  
  // Store (state + 11)
  __asm("STR   R11, [R0, #136]");
  __asm("STR   R12, [R0, #140]");
  
  // Load (state + 18)
  __asm("LDRD  R11, R12, [R0, #144]");
  
  // (state + 18) ^ (D + 3)
  __asm("EOR   R11,  R7, R11");
  __asm("ROR   R11, R11, #22");
  __asm("EOR   R12,  R8, R12");
  __asm("ROR   R12, R12, #21");
  
  // Store (state + 17)
  __asm("STR    R9, [R0, #148]");
  __asm("STR   R10, [R0, #144]");
  
  // Load (state + 3)
  __asm("LDRD   R9, R10, [R0, #24]");
  
  // (state + 3) ^ (D + 3)
  __asm("EOR    R9,  R7,  R9");
  __asm("ROR    R9,  R9, #18");
  __asm("EOR   R10,  R8, R10");
  __asm("ROR   R10, R10, #18");
  
  // Store (state + 18)
  __asm("STR   R11, [R0, #28]");
  __asm("STR   R12, [R0, #24]");
  
  // Load (state + 5)
  __asm("LDRD  R11, R12, [R0, #40]");
  
  // (state + 5) ^ (D + 0)
  __asm("EOR   R11,  R1, R11");
  __asm("ROR   R11, R11, #14");
  __asm("EOR   R12,  R2, R12");
  __asm("ROR   R12, R12, #14");
  
  // Store (state + 3)
  __asm("STR    R9, [R0, #40]");
  __asm("STR   R10, [R0, #44]");
  
  // Load (state + 16)
  __asm("LDRD   R9, R10, [R0, #128]");
  
  // (state + 16) ^ (D + 1)
  __asm("EOR    R9,  R3,  R9");
  __asm("ROR    R9,  R9, #10");
  __asm("EOR   R10,  R4, R10");
  __asm("ROR   R10, R10, #9");
  
  // Store (state + 5)
  __asm("STR   R11, [R0, #128]");
  __asm("STR   R12, [R0, #132]");
  
  // Load (state + 8)
  __asm("LDRD  R11, R12, [R0, #64]");
  
  // (state + 8) ^ (D + 3)
  __asm("EOR   R11,  R7, R11");
  __asm("ROR   R11, R11, #5");
  __asm("EOR   R12,  R8, R12");
  __asm("ROR   R12, R12, #4");
  
  // Store (state + 16)
  __asm("STR    R9, [R0, #68]");
  __asm("STR   R10, [R0, #64]");
  
  // Load (state + 21)
  __asm("LDRD   R9, R10, [R0, #168]");
  
  // (state + 21) ^ (D + 1)
  __asm("EOR    R9,  R3,  R9");
  __asm("ROR    R9,  R9, #31");
  __asm("EOR   R10,  R4, R10");
  __asm("ROR   R10, R10, #31");
  
  // Store (state + 8)
  __asm("STR   R11, [R0, #172]");
  __asm("STR   R12, [R0, #168]");
  
  // R0 = D
  __asm("LDR    R0, ??DataTable1_1");
  
  // R3, R4 = D[4]
  __asm("LDRD   R3, R4, [R0, #32]");
  
  // R0 = state
  __asm("LDR    R0, ??DataTable1");
  
  // Load (state + 24)
  __asm("LDRD  R11, R12, [R0, #192]");
  
  // (state + 24) ^ (D + 4)
  __asm("EOR   R11,  R3, R11");
  __asm("ROR   R11, R11, #25");
  __asm("EOR   R12,  R4, R12");
  __asm("ROR   R12, R12, #25");
  
  // Store (state + 21)
  __asm("STR    R9, [R0, #192]");
  __asm("STR   R10, [R0, #196]");
  
  // Load (state + 4)
  __asm("LDRD   R9, R10, [R0, #32]");
  
  // (state + 4) ^ (D + 4)
  __asm("EOR    R9,  R3,  R9");
  __asm("ROR    R9,  R9, #19");
  __asm("EOR   R10,  R4, R10");
  __asm("ROR   R10, R10, #18");
  
  // Store (state + 24)
  __asm("STR   R11, [R0, #32]");
  __asm("STR   R12, [R0, #36]");
  
  // Load (state + 15)
  __asm("LDRD  R11, R12, [R0, #120]");
  
  // (state + 15) ^ (D + 0)
  __asm("EOR   R11,  R1, R11");
  __asm("ROR   R11, R11, #12");
  __asm("EOR   R12,  R2, R12");
  __asm("ROR   R12, R12, #11");
  
  // Store (state + 4)
  __asm("STR    R9, [R0, #124]");
  __asm("STR   R10, [R0, #120]");
  
  // Load (state + 23)
  __asm("LDRD   R9, R10, [R0, #184]");
  
  // (state + 23) ^ (D + 3)
  __asm("EOR    R9,  R7,  R9");
  __asm("ROR    R9,  R9, #4");
  __asm("EOR   R10,  R8, R10");
  __asm("ROR   R10, R10, #4");
  
  // Store (state + 15)
  __asm("STR   R11, [R0, #188]");
  __asm("STR   R12, [R0, #184]");
  
  // Load (state + 19)
  __asm("LDRD  R11, R12, [R0, #152]");
  
  // (state + 19) ^ (D + 4)
  __asm("EOR   R11,  R3, R11");
  __asm("ROR   R11, R11, #28");
  __asm("EOR   R12,  R4, R12");
  __asm("ROR   R12, R12, #28");
  
  // Store (state + 23)
  __asm("STR    R9, [R0, #152]");
  __asm("STR   R10, [R0, #156]");
  
  // Load (state + 13)
  __asm("LDRD   R9, R10, [R0, #104]");
  
  // (state + 13) ^ (D + 3)
  __asm("EOR    R9,  R7,  R9");
  __asm("ROR    R9,  R9, #20");
  __asm("EOR   R10,  R8, R10");
  __asm("ROR   R10, R10, #19");
  
  // Store (state + 19)
  __asm("STR   R11, [R0, #104]");
  __asm("STR   R12, [R0, #108]");
  
  // Load (state + 12)
  __asm("LDRD  R11, R12, [R0, #96]");
  
  // (state + 12) ^ (D + 2)
  __asm("EOR   R11,  R5, R11");
  __asm("ROR   R11, R11, #11");
  __asm("EOR   R12,  R6, R12");
  __asm("ROR   R12, R12, #10");
  
  // Store (state + 13)
  __asm("STR    R9, [R0, #100]");
  __asm("STR   R10, [R0, #96]");
  
  // Load (state + 2)
  __asm("LDRD   R9, R10, [R0, #16]");
  
  // (state + 2) ^ (D + 2)
  __asm("EOR    R9,  R5,  R9");
  __asm("ROR    R9,  R9, #1");
  __asm("EOR   R10,  R6, R10");
  __asm("ROR   R10, R10, #1");
  
  // Store (state + 12)
  __asm("STR   R11, [R0, #20]");
  __asm("STR   R12, [R0, #16]");
  
  // Load (state + 20)
  __asm("LDRD  R11, R12, [R0, #160]");
  
  // (state + 20) ^ (D + 0)
  __asm("EOR   R11,  R1, R11");
  __asm("ROR   R11, R11, #23");
  __asm("EOR   R12,  R2, R12");
  __asm("ROR   R12, R12, #23");
  
  // Store (state + 2)
  __asm("STR    R9, [R0, #160]");
  __asm("STR   R10, [R0, #164]");
  
  // Load (state + 14)
  __asm("LDRD   R9, R10, [R0, #112]");
  
  // (state + 14) ^ (D + 4)
  __asm("EOR    R9,  R3,  R9");
  __asm("ROR    R9,  R9, #13");
  __asm("EOR   R10,  R4, R10");
  __asm("ROR   R10, R10, #12");
  
  // Store (state + 20)
  __asm("STR   R11, [R0, #112]");
  __asm("STR   R12, [R0, #116]");
  
  // Load (state + 22)
  __asm("LDRD  R11, R12, [R0, #176]");
  
  // (state + 22) ^ (D + 2)
  __asm("EOR   R11,  R5, R11");
  __asm("ROR   R11, R11, #2");
  __asm("EOR   R12,  R6, R12");
  __asm("ROR   R12, R12, #1");
  
  // Store (state + 14)
  __asm("STR    R9, [R0, #180]");
  __asm("STR   R10, [R0, #176]");
  
  // Load (state + 9)
  __asm("LDRD   R9, R10, [R0, #72]");
  
  // (state + 9) ^ (D + 4)
  __asm("EOR    R9,  R3,  R9");
  __asm("ROR    R9,  R9, #22");
  __asm("EOR   R10,  R4, R10");
  __asm("ROR   R10, R10, #22");
  
  // Store (state + 22)
  __asm("STR   R11, [R0, #76]");
  __asm("STR   R12, [R0, #72]");
  
  // Load (state + 6)
  __asm("LDRD  R11, R12, [R0, #48]");
  
  // Store (state + 9)
  __asm("STR    R9, [R0, #48]");
  __asm("STR   R10, [R0, #52]");
  
  // Load (state + 0)
  __asm("LDRD   R9, R10, [R0]");
  
  // (state + 0) ^ (D + 0)
  __asm("EOR    R9,  R1,  R9");
  __asm("EOR   R10,  R2, R10");
  
  // Store (state + 0)
  __asm("STR    R9, [R0]");
  __asm("STR   R10, [R0, #4]");
  
  // R0 = D
  __asm("LDR    R0, ??DataTable1_1");
  
  // R1-R8 = D[1]
  __asm("LDRD   R3, R4, [R0, #8]");
  
  // (state + 6) ^ (D + 1)
  __asm("EOR   R11,  R3, R11");
  __asm("ROR   R11, R11, #10");
  __asm("EOR   R12,  R4, R12");
  __asm("ROR   R12, R12, #10");
  
  // R0 = state
  __asm("LDR    R0, ??DataTable1");
  
  // Store (state + 6)
  __asm("STR   R11, [R0, #8]");
  __asm("STR   R12, [R0, #12]");
  // theta (D ^ state) + rho + pi, end
  // //////////////////////////////////////////////////////////////////////////
  
  // //////////////////////////////////////////////////////////////////////////
  // theta (prepare C) + chi, start
  
  // round number -> R12
  __asm("POP {R12}");
  
  // R0 = RC
  __asm("ADR    R0, RC");
  
  // R1 = RC[i, перша половина]
  __asm("LDR    R1, [R0, R12, LSL #3]");
  
  // R0 = state
  __asm("LDR    R0, ??DataTable1");
  
  // R6-R10 = state[перша половина, перший рядок]
  __asm("LDR    R6, [R0]");
  __asm("LDR    R7, [R0, #8]");
  __asm("LDR    R8, [R0, #16]");
  __asm("LDR    R9, [R0, #24]");
  __asm("LDR   R10, [R0, #32]");
  
  // Обчислюємо новий state і формуємо C
  __asm("BIC   R11, R8, R7");
  __asm("EOR   R11, R11, R6");
  
  // ^ RC (виняток)
  __asm("EOR    R1, R1, R11");
  
  __asm("BIC    R2, R9, R8");
  __asm("EOR    R2, R2, R7");
  
  __asm("BIC    R3, R10, R9");
  __asm("EOR    R3, R3, R8");
  
  __asm("BIC    R4, R6, R10");
  __asm("EOR    R4, R4, R9");
  
  __asm("BIC    R5, R7, R6");
  __asm("EOR    R5, R5, R10");
  
  // Зберігаємо state[перша половина, перший рядок]
  __asm("STR    R1, [R0]");
  __asm("STR    R2, [R0, #8]");
  __asm("STR    R3, [R0, #16]");
  __asm("STR    R4, [R0, #24]");
  __asm("STR    R5, [R0, #32]");
  
  // R6-R10 = state[перша половина, другий рядок]
  __asm("LDR    R6, [R0, #40]");
  __asm("LDR    R7, [R0, #48]");
  __asm("LDR    R8, [R0, #56]");
  __asm("LDR    R9, [R0, #64]");
  __asm("LDR   R10, [R0, #72]");
  
  // Обчислюємо новий state, ^ до С, зберігаємо state
  __asm("BIC   R11, R8, R7");
  __asm("EOR   R11, R11, R6");
  __asm("EOR    R1, R1, R11");
  __asm("STR   R11, [R0, #40]");
  
  __asm("BIC   R11, R9, R8");
  __asm("EOR   R11, R11, R7");
  __asm("EOR    R2, R2, R11");
  __asm("STR   R11, [R0, #48]");
  
  __asm("BIC   R11, R10, R9");
  __asm("EOR   R11, R11, R8");
  __asm("EOR    R3, R3, R11");
  __asm("STR   R11, [R0, #56]");
  
  __asm("BIC   R11, R6, R10");
  __asm("EOR   R11, R11, R9");
  __asm("EOR    R4, R4, R11");
  __asm("STR   R11, [R0, #64]");
  
  __asm("BIC   R11, R7, R6");
  __asm("EOR   R11, R11, R10");
  __asm("EOR    R5, R5, R11");
  __asm("STR   R11, [R0, #72]");
  
  // R6-R10 = state[перша половина, третій рядок]
  __asm("LDR    R6, [R0, #80]");
  __asm("LDR    R7, [R0, #88]");
  __asm("LDR    R8, [R0, #96]");
  __asm("LDR    R9, [R0, #104]");
  __asm("LDR   R10, [R0, #112]");
  
  // Обчислюємо новий state, ^ до С, зберігаємо state
  __asm("BIC   R11, R8, R7");
  __asm("EOR   R11, R11, R6");
  __asm("EOR    R1, R1, R11");
  __asm("STR   R11, [R0, #80]");
  
  __asm("BIC   R11, R9, R8");
  __asm("EOR   R11, R11, R7");
  __asm("EOR    R2, R2, R11");
  __asm("STR   R11, [R0, #88]");
  
  __asm("BIC   R11, R10, R9");
  __asm("EOR   R11, R11, R8");
  __asm("EOR    R3, R3, R11");
  __asm("STR   R11, [R0, #96]");
  
  __asm("BIC   R11, R6, R10");
  __asm("EOR   R11, R11, R9");
  __asm("EOR    R4, R4, R11");
  __asm("STR   R11, [R0, #104]");
  
  __asm("BIC   R11, R7, R6");
  __asm("EOR   R11, R11, R10");
  __asm("EOR    R5, R5, R11");
  __asm("STR   R11, [R0, #112]");
  
  // R6-R10 = state[перша половина, четвертий рядок]
  __asm("LDR    R6, [R0, #120]");
  __asm("LDR    R7, [R0, #128]");
  __asm("LDR    R8, [R0, #136]");
  __asm("LDR    R9, [R0, #144]");
  __asm("LDR   R10, [R0, #152]");
  
  // Обчислюємо новий state, ^ до С, зберігаємо state
  __asm("BIC   R11, R8, R7");
  __asm("EOR   R11, R11, R6");
  __asm("EOR    R1, R1, R11");
  __asm("STR   R11, [R0, #120]");
  
  __asm("BIC   R11, R9, R8");
  __asm("EOR   R11, R11, R7");
  __asm("EOR    R2, R2, R11");
  __asm("STR   R11, [R0, #128]");
  
  __asm("BIC   R11, R10, R9");
  __asm("EOR   R11, R11, R8");
  __asm("EOR    R3, R3, R11");
  __asm("STR   R11, [R0, #136]");
  
  __asm("BIC   R11, R6, R10");
  __asm("EOR   R11, R11, R9");
  __asm("EOR    R4, R4, R11");
  __asm("STR   R11, [R0, #144]");
  
  __asm("BIC   R11, R7, R6");
  __asm("EOR   R11, R11, R10");
  __asm("EOR    R5, R5, R11");
  __asm("STR   R11, [R0, #152]");
  
  // R6-R10 = state[перша половина, п'ятий рядок]
  __asm("LDR    R6, [R0, #160]");
  __asm("LDR    R7, [R0, #168]");
  __asm("LDR    R8, [R0, #176]");
  __asm("LDR    R9, [R0, #184]");
  __asm("LDR   R10, [R0, #192]");
  
  // Обчислюємо новий state, ^ до С, зберігаємо state
  __asm("BIC   R11, R8, R7");
  __asm("EOR   R11, R11, R6");
  __asm("EOR    R1, R1, R11");
  __asm("STR   R11, [R0, #160]");
  
  __asm("BIC   R11, R9, R8");
  __asm("EOR   R11, R11, R7");
  __asm("EOR    R2, R2, R11");
  __asm("STR   R11, [R0, #168]");
  
  __asm("BIC   R11, R10, R9");
  __asm("EOR   R11, R11, R8");
  __asm("EOR    R3, R3, R11");
  __asm("STR   R11, [R0, #176]");
  
  __asm("BIC   R11, R6, R10");
  __asm("EOR   R11, R11, R9");
  __asm("EOR    R4, R4, R11");
  __asm("STR   R11, [R0, #184]");
  
  __asm("BIC   R11, R7, R6");
  __asm("EOR   R11, R11, R10");
  __asm("EOR    R5, R5, R11");
  __asm("STR   R11, [R0, #192]");
  
  // Закидуємо С у стек
  __asm("PUSH {R1-R5}");
  
  // R0 = RC
  __asm("ADR    R0, RC");
  
  // R1 = RC[i, друга половина]
  __asm("ADD    R0, R0, R12, LSL #3");
  __asm("LDR    R1, [R0, #4]");
  
  // R0 = state
  __asm("LDR    R0, ??DataTable1");
  
  // R6-R10 = state[друга половина, перший рядок]
  __asm("LDR    R6, [R0, #4]");
  __asm("LDR    R7, [R0, #12]");
  __asm("LDR    R8, [R0, #20]");
  __asm("LDR    R9, [R0, #28]");
  __asm("LDR   R10, [R0, #36]");
  
  // Обчислюємо новий state і формуємо C
  __asm("BIC    R11, R8, R7");
  __asm("EOR    R11, R11, R6");
  
  // ^ RC (виняток)
  __asm("EOR    R1, R1, R11");
  
  __asm("BIC    R2, R9, R8");
  __asm("EOR    R2, R2, R7");
  
  __asm("BIC    R3, R10, R9");
  __asm("EOR    R3, R3, R8");
  
  __asm("BIC    R4, R6, R10");
  __asm("EOR    R4, R4, R9");
  
  __asm("BIC    R5, R7, R6");
  __asm("EOR    R5, R5, R10");
  
  // Зберігаємо state[друга половина, перший рядок]
  __asm("STR    R1, [R0, #4]");
  __asm("STR    R2, [R0, #12]");
  __asm("STR    R3, [R0, #20]");
  __asm("STR    R4, [R0, #28]");
  __asm("STR    R5, [R0, #36]");
  
  // R6-R10 = state[друга половина, другий рядок]
  __asm("LDR    R6, [R0, #44]");
  __asm("LDR    R7, [R0, #52]");
  __asm("LDR    R8, [R0, #60]");
  __asm("LDR    R9, [R0, #68]");
  __asm("LDR   R10, [R0, #76]");
  
  // Обчислюємо новий state, ^ до С, зберігаємо state
  __asm("BIC   R11, R8, R7");
  __asm("EOR   R11, R11, R6");
  __asm("EOR    R1, R1, R11");
  __asm("STR   R11, [R0, #44]");
  
  __asm("BIC   R11, R9, R8");
  __asm("EOR   R11, R11, R7");
  __asm("EOR    R2, R2, R11");
  __asm("STR   R11, [R0, #52]");
  
  __asm("BIC   R11, R10, R9");
  __asm("EOR   R11, R11, R8");
  __asm("EOR    R3, R3, R11");
  __asm("STR   R11, [R0, #60]");
  
  __asm("BIC   R11, R6, R10");
  __asm("EOR   R11, R11, R9");
  __asm("EOR    R4, R4, R11");
  __asm("STR   R11, [R0, #68]");
  
  __asm("BIC   R11, R7, R6");
  __asm("EOR   R11, R11, R10");
  __asm("EOR    R5, R5, R11");
  __asm("STR   R11, [R0, #76]");
  
  // R6-R10 = state[друга половина, третій рядок]
  __asm("LDR    R6, [R0, #84]");
  __asm("LDR    R7, [R0, #92]");
  __asm("LDR    R8, [R0, #100]");
  __asm("LDR    R9, [R0, #108]");
  __asm("LDR   R10, [R0, #116]");
  
  // Обчислюємо новий state, ^ до С, зберігаємо state
  __asm("BIC   R11, R8, R7");
  __asm("EOR   R11, R11, R6");
  __asm("EOR    R1, R1, R11");
  __asm("STR   R11, [R0, #84]");
  
  __asm("BIC   R11, R9, R8");
  __asm("EOR   R11, R11, R7");
  __asm("EOR    R2, R2, R11");
  __asm("STR   R11, [R0, #92]");
  
  __asm("BIC   R11, R10, R9");
  __asm("EOR   R11, R11, R8");
  __asm("EOR    R3, R3, R11");
  __asm("STR   R11, [R0, #100]");
  
  __asm("BIC   R11, R6, R10");
  __asm("EOR   R11, R11, R9");
  __asm("EOR    R4, R4, R11");
  __asm("STR   R11, [R0, #108]");
  
  __asm("BIC   R11, R7, R6");
  __asm("EOR   R11, R11, R10");
  __asm("EOR    R5, R5, R11");
  __asm("STR   R11, [R0, #116]");
  
  // R6-R10 = state[друга половина, четвертий рядок]
  __asm("LDR    R6, [R0, #124]");
  __asm("LDR    R7, [R0, #132]");
  __asm("LDR    R8, [R0, #140]");
  __asm("LDR    R9, [R0, #148]");
  __asm("LDR   R10, [R0, #156]");
  
  // Обчислюємо новий state, ^ до С, зберігаємо state
  __asm("BIC   R11, R8, R7");
  __asm("EOR   R11, R11, R6");
  __asm("EOR    R1, R1, R11");
  __asm("STR   R11, [R0, #124]");
  
  __asm("BIC   R11, R9, R8");
  __asm("EOR   R11, R11, R7");
  __asm("EOR    R2, R2, R11");
  __asm("STR   R11, [R0, #132]");
  
  __asm("BIC   R11, R10, R9");
  __asm("EOR   R11, R11, R8");
  __asm("EOR    R3, R3, R11");
  __asm("STR   R11, [R0, #140]");
  
  __asm("BIC   R11, R6, R10");
  __asm("EOR   R11, R11, R9");
  __asm("EOR    R4, R4, R11");
  __asm("STR   R11, [R0, #148]");
  
  __asm("BIC   R11, R7, R6");
  __asm("EOR   R11, R11, R10");
  __asm("EOR    R5, R5, R11");
  __asm("STR   R11, [R0, #156]");
  
  // R6-R10 = state[друга половина, п'ятий рядок]
  __asm("LDR    R6, [R0, #164]");
  __asm("LDR    R7, [R0, #172]");
  __asm("LDR    R8, [R0, #180]");
  __asm("LDR    R9, [R0, #188]");
  __asm("LDR   R10, [R0, #196]");
  
  // Обчислюємо новий state, ^ до С, зберігаємо state
  __asm("BIC   R11, R8, R7");
  __asm("EOR   R11, R11, R6");
  __asm("EOR    R1, R1, R11");
  __asm("STR   R11, [R0, #164]");
  
  __asm("BIC   R11, R9, R8");
  __asm("EOR   R11, R11, R7");
  __asm("EOR    R2, R2, R11");
  __asm("STR   R11, [R0, #172]");
  
  __asm("BIC   R11, R10, R9");
  __asm("EOR   R11, R11, R8");
  __asm("EOR    R3, R3, R11");
  __asm("STR   R11, [R0, #180]");
  
  __asm("BIC   R11, R6, R10");
  __asm("EOR   R11, R11, R9");
  __asm("EOR    R4, R4, R11");
  __asm("STR   R11, [R0, #188]");
  
  __asm("BIC   R11, R7, R6");
  __asm("EOR   R11, R11, R10");
  __asm("EOR    R5, R5, R11");
  __asm("STR   R11, [R0, #196]");
  
  // Дістаємо зі стеку C
  __asm("POP {R6-R10}");
  
  // Маємо:
  // R1-R5  - другі половини C
  // R6-R10 - перші половини C
  
  // Формуємо D, перші половини
  // R0 = D
  __asm("LDR    R0, ??DataTable1_1");
  
  __asm("EOR   R11, R10, R2, ROR #31");
  __asm("STR   R11, [R0]");
  
  __asm("EOR   R11, R6, R3, ROR #31");
  __asm("STR   R11, [R0, #8]");
  
  __asm("EOR   R11, R7, R4, ROR #31");
  __asm("STR   R11, [R0, #16]");
  
  __asm("EOR   R11, R8, R5, ROR #31");
  __asm("STR   R11, [R0, #24]");
  
  __asm("EOR   R11, R9, R1, ROR #31");
  __asm("STR   R11, [R0, #32]");
  
  // Формуємо D, другі половини
  __asm("EOR   R11, R5, R7");
  __asm("STR   R11, [R0, #4]");
  
  __asm("EOR   R11, R1, R8");
  __asm("STR   R11, [R0, #12]");
  
  __asm("EOR   R11, R2, R9");
  __asm("STR   R11, [R0, #20]");
  
  __asm("EOR   R11, R3, R10");
  __asm("STR   R11, [R0, #28]");
  
  __asm("EOR   R11, R4, R6");
  __asm("STR   R11, [R0, #36]");
  
  // theta (prepare C) + chi, end
  // //////////////////////////////////////////////////////////////////////////
  
  // Порівняти round number з rounds = 24
  __asm("CMP R12, #23");
  
  // і++
  __asm("ADD R12, R12, #1");
  
  __asm("BNE KeccakF_circle");
  
  // Тут закінчується цикл раундів
  // //////////////////////////////////////////////////////////////////////////
  
  // return from KeccakF
  __asm("POP {R4-R11}");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const uint32_t RC[] = 
{
  0x00000001, 0x00000000, 0x00000000, 0x00000089,
  0x00000000, 0x8000008B, 0x00000000, 0x80008080,
  0x00000001, 0x0000008B, 0x00000001, 0x00008000,
  0x00000001, 0x80008088, 0x00000001, 0x80000082,
  0x00000000, 0x0000000B, 0x00000000, 0x0000000A,
  0x00000001, 0x00008082, 0x00000000, 0x00008003,
  0x00000001, 0x0000808B, 0x00000001, 0x8000000B,
  0x00000001, 0x8000008A, 0x00000001, 0x80000081,
  0x00000000, 0x80000081, 0x00000000, 0x80000008,
  0x00000000, 0x00000083, 0x00000000, 0x80008003,
  0x00000001, 0x80008088, 0x00000000, 0x80000088,
  0x00000001, 0x00008000, 0x00000000, 0x80008082
};
// ////////////////////////////////////////////////////////////////////////////
