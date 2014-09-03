#include "keccakH.h"

// ////////////////////////////////////////////////////////////////////////////
void KeccakF(void)
{
  uint8_t roundN;
  
  for (roundN = 0; roundN < ROUNDS; roundN++)
  {
    theta();
    rho();
    pi();
    chi();
    iota(roundN);
  }
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void theta(void)
{
  asm("ST -Y, R15");
  asm("ST -Y, R14");
  asm("ST -Y, R13");
  asm("ST -Y, R12");
  asm("ST -Y, R11");
  asm("ST -Y, R10");
  asm("ST -Y,  R4");
  
  // //////////////////////////////////////////////////////////////////////////
  // Обчислюємо C[x]
  
  // R21 для повернення на верхню смугу сусіднього стовпця
  // R4 = 0 для ADC
  asm("LDI R21, 0");
  asm("MOV  R4, R21");
  asm("LDI R21, 152");
  
  // Адреса останнього байту в Z
  asm("LDI R30,  LOW(state + 192)");
  asm("LDI R31, HIGH(state + 192)");
  
  // cycle: for(x = 5; x > 0; x--)
  // R22 = x
  asm("LDI R22, 5");
  asm("theta_acum_x_cycle:");
  
    // R0-3, R16-19 - смуга
    asm("LD   R0, Z    ");
    asm("LDD  R1, Z + 1");
    asm("LDD  R2, Z + 2");
    asm("LDD  R3, Z + 3");
    asm("LDD R16, Z + 4");
    asm("LDD R17, Z + 5");
    asm("LDD R18, Z + 6");
    asm("LDD R19, Z + 7");
  
  
    // cycle: for(y = 4; y > 0; y--)
    // R23 = y
    asm("LDI R23, 4");
    asm("theta_acum_y_cycle:");
  
      // Переходимо на смугу нижче
      asm("SBIW R31:R30, 40");
  
      // Завантажуємо байт зі смуги і додаємо до накопиченого
      asm("LD  R20,     Z");
      asm("EOR  R0,   R20");
      asm("LDD R20, Z + 1");
      asm("EOR  R1,   R20");
      asm("LDD R20, Z + 2");
      asm("EOR  R2,   R20");
      asm("LDD R20, Z + 3");
      asm("EOR  R3,   R20");
      asm("LDD R20, Z + 4");
      asm("EOR R16,   R20");
      asm("LDD R20, Z + 5");
      asm("EOR R17,   R20");
      asm("LDD R20, Z + 6");
      asm("EOR R18,   R20");
      asm("LDD R20, Z + 7");
      asm("EOR R19,   R20");
  
      // Лічильник y--
      asm("DEC R23");
    asm("BRNE theta_acum_y_cycle");
  
    // Зберігаємо C[i]
    asm("ST -Y, R19");
    asm("ST -Y, R18");
    asm("ST -Y, R17");
    asm("ST -Y, R16");
    asm("ST -Y,  R3");
    asm("ST -Y,  R2");
    asm("ST -Y,  R1");
    asm("ST -Y,  R0");
  
    // Вертаємось на верхню смугу сусіднього стовпця
    asm("ADD R30, R21");
    asm("ADC R31,  R4");
  
    // Лічильник х--
    asm("DEC R22");
  asm("BRNE theta_acum_x_cycle");
  // //////////////////////////////////////////////////////////////////////////
  
  // //////////////////////////////////////////////////////////////////////////
  // Завантажуємо рядок байтів C[z = 7]
  asm("LDD R10, Y +  7");
  asm("LDD R11, Y + 15");
  asm("LDD R12, Y + 23");
  asm("LDD R13, Y + 31");
  asm("LDD R14, Y + 39");
  
  // Завантажуємо рядок байтів C[z = 0] так,
  // щоб відповідало D[x] = C[x - 1]
  asm("LD  R16, Y     ");
  asm("LDD R17, Y +  8");
  asm("LDD R18, Y + 16");
  asm("LDD R19, Y + 24");
  asm("LDD R15, Y + 32");
  
  // Копіюємо рядок байтів C[z = 0]
  asm("MOVW R1:R0, R17:R16");
  asm("MOVW R3:R2, R19:R18");
  asm("MOV R4, R15");
  
  // ROT(C[z = 0], 1)
  asm("LSL R10");
  asm("ROL  R0");
  asm("LSL R11");
  asm("ROL  R1");
  asm("LSL R12");
  asm("ROL  R2");
  asm("LSL R13");
  asm("ROL  R3");
  asm("LSL R14");
  asm("ROL  R4");
  
  // C[x - 1] ^ ROT(C[x + 1], 1)
  asm("EOR R15, R1");
  asm("EOR R16, R2");
  asm("EOR R17, R3");
  asm("EOR R18, R4");
  asm("EOR R19, R0");
  
  // Для переміщення на наступний рядок - R20
  // Для ADC R21
  asm("LDI R20, 199");
  asm("LDI R21,   0");
  
  // cycle: for(z = 0; z < 8; z++)
  // R22 = Z
  asm("LDI R30,  LOW(state)");
  asm("LDI R31, HIGH(state)");
  asm("LDI R22, 8");
  asm("theta_state_xor_d_z_cycle:");
  
    // cycle: for(y = 0, y < 5; y++)
    // R23 = y
    asm("LDI R23, 5");
    asm("theta_state_xor_d_y_cycle:");
  
      // Завантажуємо state[z]
      asm("LD  R10, Z     ");
      asm("LDD R11, Z +  8");
      asm("LDD R12, Z + 16");
      asm("LDD R13, Z + 24");
      asm("LDD R14, Z + 32");
  
      // state[z] ^ C
      asm("EOR R10, R15");
      asm("EOR R11, R16");
      asm("EOR R12, R17");
      asm("EOR R13, R18");
      asm("EOR R14, R19");
  
      // Зберігаємо state[z]
      asm("ST  Z     , R10");
      asm("STD Z +  8, R11");
      asm("STD Z + 16, R12");
      asm("STD Z + 24, R13");
      asm("STD Z + 32, R14");
  
      // Лічильник y--
      asm("ADIW R31:R30, 40");
      asm("DEC R23");
    asm("BRNE theta_state_xor_d_y_cycle");
  
  
    // Завантажуємо рядок байтів C[z]
    asm("LD  R10, Y     ");
    asm("LDD R11, Y +  8");
    asm("LDD R12, Y + 16");
    asm("LDD R13, Y + 24");
    asm("LDD R14, Y + 32");
  
    // Завантажуємо рядок байтів C[z + 1] так,
    // щоб відповідало D[x] = C[x - 1]
    asm("ADIW R29:R28, 1");
    asm("LD  R16, Y     ");
    asm("LDD R17, Y +  8");
    asm("LDD R18, Y + 16");
    asm("LDD R19, Y + 24");
    asm("LDD R15, Y + 32");
  
    // Копіюємо рядок байтів C[z + 1]
    asm("MOVW R1:R0, R17:R16");
    asm("MOVW R3:R2, R19:R18");
    asm("MOV R4, R15");
  
    // ROT(C[z + 1], 1)
    asm("LSL R10");
    asm("ROL  R0");
    asm("LSL R11");
    asm("ROL  R1");
    asm("LSL R12");
    asm("ROL  R2");
    asm("LSL R13");
    asm("ROL  R3");
    asm("LSL R14");
    asm("ROL  R4");
  
    // C[x - 1] ^ ROT(C[x + 1], 1)
    asm("EOR R15, R1");
    asm("EOR R16, R2");
    asm("EOR R17, R3");
    asm("EOR R18, R4");
    asm("EOR R19, R0");
  
    // Переміщаємось на наступний рядок
    asm("SUB  R30, R20");
    asm("SBCI R31,   0");
  
    // Лічильник z--
    asm("DEC R22");
  asm("BRNE theta_state_xor_d_z_cycle");
  // //////////////////////////////////////////////////////////////////////////
  
  // Видаляємо C
  asm("ADIW R29:R28, 32");
  
  // Завантажуємо збережені регістри
  asm("LD  R4, Y+");
  asm("LD R10, Y+");
  asm("LD R11, Y+");
  asm("LD R12, Y+");
  asm("LD R13, Y+");
  asm("LD R14, Y+");
  asm("LD R15, Y+");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void rho(void)
{
  // Напрям зсуву (вліво чи вправо) встановлюємо в регістрі R22:
  // вліво = 0, вправо = 1.
  // На скільки одиниць зсуву задаємо в регістрі R23.
  
  // state + 1
  asm("LDI R30,  LOW(state + 8)");
  asm("LDI R31, HIGH(state + 8)");
  asm("LDI R22, 0");
  asm("LDI R23, 1");
  asm("CALL ROT_from_61_to_3");
  
  // state + 2
  asm("LDI R22, 1");
  asm("LDI R23, 2");
  asm("CALL ROT_from_61_to_3");
  
  // state + 3
  asm("LDI R22, 0");
  asm("LDI R23, 4");
  asm("CALL ROT_from_20_to_28");
  
  // state + 4
  asm("LDI R23, 3");
  asm("CALL ROT_from_20_to_28");
  
  // state + 5
  asm("LDI R22, 1");
  asm("LDI R23, 4");
  asm("CALL ROT_from_36_to_45");
  
  // state + 6
  asm("LDI R22, 0");
  asm("LDI R23, 4");
  asm("CALL ROT_from_36_to_45");
  
  // state + 7
  asm("LDI R22, 1");
  asm("LDI R23, 2");
  asm("CALL ROT_from_6_to_10");
  
  // state + 8
  asm("LDI R23, 1");
  asm("CALL ROT_from_55_to_56");
  
  // state + 9
  asm("LDI R23, 4");
  asm("CALL ROT_from_20_to_28");
  
  // state + 10
  asm("LDI R22, 0");
  asm("LDI R23, 3");
  asm("CALL ROT_from_61_to_3");
  
  // state + 11
  asm("LDI R23, 2");
  asm("CALL ROT_from_6_to_10");
  
  // state + 12
  asm("LDI R23, 3");
  asm("CALL ROT_from_36_to_45");
  
  // state + 13
  asm("LDI R23, 1");
  asm("CALL ROT_from_20_to_28");
  
  // state + 14
  asm("LDI R22, 1");
  asm("LDI R23, 1");
  asm("CALL ROT_from_36_to_45");
  
  // state + 15
  asm("LDI R22, 0");
  asm("LDI R23, 1");
  asm("CALL ROT_from_36_to_45");
  
  // state + 16
  asm("LDI R23, 5");
  asm("CALL ROT_from_36_to_45");
  
  // state + 17
  asm("LDI R22, 1");
  asm("LDI R23, 1");
  asm("CALL ROT_from_14_to_18");
  
  // state + 18
  asm("LDI R23, 3");
  asm("CALL ROT_from_20_to_28");
  
  // state + 19
  asm("CALL ROT_from_6_to_10");
  
  // state + 20
  asm("LDI R22, 0");
  asm("LDI R23, 2");
  asm("CALL ROT_from_14_to_18");
  
  // state + 21
  asm("LDI R23, 2");
  asm("CALL ROT_from_61_to_3");
  
  // state + 22
  asm("LDI R22, 1");
  asm("LDI R23, 3");
  asm("CALL ROT_from_61_to_3");
  
  // state + 23
  asm("CALL ROT_from_55_to_56");
  
  // state + 24
  asm("LDI R23, 2");
  asm("CALL ROT_from_14_to_18");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void pi(void)
{
  uint8_t i, k;
  uint8_t temp0_0, temp0_1, temp0_2, temp0_3;
  uint8_t temp0_4, temp0_5, temp0_6, temp0_7;
  uint8_t temp1_0, temp1_1, temp1_2, temp1_3;
  uint8_t temp1_4, temp1_5, temp1_6, temp1_7;
  extern __flash uint8_t piTranslation[];
  
  temp0_0 = *(state + 80);
  temp0_1 = *(state + 81);
  temp0_2 = *(state + 82);
  temp0_3 = *(state + 83);
  temp0_4 = *(state + 84);
  temp0_5 = *(state + 85);
  temp0_6 = *(state + 86);
  temp0_7 = *(state + 87);
  
  *(state + 80) = *(state +  8);
  *(state + 81) = *(state +  9);
  *(state + 82) = *(state + 10);
  *(state + 83) = *(state + 11);
  *(state + 84) = *(state + 12);
  *(state + 85) = *(state + 13);
  *(state + 86) = *(state + 14);
  *(state + 87) = *(state + 15);

  k = 10;
  
  for (i = 2; i < (LANES - 1); i += 2)
  {
    temp1_0 = *(state + (*(piTranslation + k) << STEP)    );
    temp1_1 = *(state + (*(piTranslation + k) << STEP) + 1);
    temp1_2 = *(state + (*(piTranslation + k) << STEP) + 2);
    temp1_3 = *(state + (*(piTranslation + k) << STEP) + 3);
    temp1_4 = *(state + (*(piTranslation + k) << STEP) + 4);
    temp1_5 = *(state + (*(piTranslation + k) << STEP) + 5);
    temp1_6 = *(state + (*(piTranslation + k) << STEP) + 6);
    temp1_7 = *(state + (*(piTranslation + k) << STEP) + 7);

    *(state + (*(piTranslation + k) << STEP)    ) = temp0_0;
    *(state + (*(piTranslation + k) << STEP) + 1) = temp0_1;
    *(state + (*(piTranslation + k) << STEP) + 2) = temp0_2;
    *(state + (*(piTranslation + k) << STEP) + 3) = temp0_3;
    *(state + (*(piTranslation + k) << STEP) + 4) = temp0_4;
    *(state + (*(piTranslation + k) << STEP) + 5) = temp0_5;
    *(state + (*(piTranslation + k) << STEP) + 6) = temp0_6;
    *(state + (*(piTranslation + k) << STEP) + 7) = temp0_7;

    k = *(piTranslation + k);

    temp0_0 = *(state + (*(piTranslation + k) << STEP)    );
    temp0_1 = *(state + (*(piTranslation + k) << STEP) + 1);
    temp0_2 = *(state + (*(piTranslation + k) << STEP) + 2);
    temp0_3 = *(state + (*(piTranslation + k) << STEP) + 3);
    temp0_4 = *(state + (*(piTranslation + k) << STEP) + 4);
    temp0_5 = *(state + (*(piTranslation + k) << STEP) + 5);
    temp0_6 = *(state + (*(piTranslation + k) << STEP) + 6);
    temp0_7 = *(state + (*(piTranslation + k) << STEP) + 7);

    *(state + (*(piTranslation + k) << STEP)    ) = temp1_0;
    *(state + (*(piTranslation + k) << STEP) + 1) = temp1_1;
    *(state + (*(piTranslation + k) << STEP) + 2) = temp1_2;
    *(state + (*(piTranslation + k) << STEP) + 3) = temp1_3;
    *(state + (*(piTranslation + k) << STEP) + 4) = temp1_4;
    *(state + (*(piTranslation + k) << STEP) + 5) = temp1_5;
    *(state + (*(piTranslation + k) << STEP) + 6) = temp1_6;
    *(state + (*(piTranslation + k) << STEP) + 7) = temp1_7;
    
    k = *(piTranslation + k);
  }
  *(state +  8) = temp0_0;
  *(state +  9) = temp0_1;
  *(state + 10) = temp0_2;
  *(state + 11) = temp0_3;
  *(state + 12) = temp0_4;
  *(state + 13) = temp0_5;
  *(state + 14) = temp0_6;
  *(state + 15) = temp0_7;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void chi(void)
{  
  // У стек
  asm("ST -Y, R4");
  asm("ST -Y, R5");
  
  // Встановлюємо лічильник
  // R22 має = 200, R23 має = 8
  asm("LDI R22, 0");
  asm("LDI R23, 0");
  
  // state + лічильник -> Z
  asm("LDI R30,  LOW(state)");
  asm("LDI R31, HIGH(state)");
  
  // cycle: for (i = 0; i != 200; i += 40)
  // cycle: for (j = 0; j != 7; j++)
  asm("chi_cycle:");
  
    // Завантажуємо з кожної смуги два байта
    // R16 <- x[0,y], R17 <- x[1,y],...
    asm("LD   R0, Z     ");
    asm("LDD  R1, Z +  1");
    asm("LDD  R2, Z +  8");
    asm("LDD  R3, Z +  9");
    asm("LDD  R4, Z + 16");
    asm("LDD  R5, Z + 17");
    asm("LDD R16, Z + 24");
    asm("LDD R17, Z + 25");
    asm("LDD R18, Z + 32");
    asm("LDD R19, Z + 33");
  
    // x[0,y] ^ ( ~x[1,y] & x[2,y] )
    asm("MOVW R21:R20, R3:R2");
    asm("COM R21");
    asm("COM R20");
    asm("AND   R21, R5");
    asm("AND   R20, R4");
    asm("EOR   R21, R1");
    asm("EOR   R20, R0");
    asm("ST      Z, R20");
    asm("STD Z + 1, R21");
  
    // x[1,y] ^ ( ~x[2,y] & x[3,y] )
    asm("MOVW R21:R20, R5:R4");
    asm("COM R21");
    asm("COM R20");
    asm("AND   R21, R17");
    asm("AND   R20, R16");
    asm("EOR   R21,  R3");
    asm("EOR   R20,  R2");
    asm("STD Z + 8, R20");
    asm("STD Z + 9, R21");
  
    // x[2,y] ^ ( ~x[3,y] & x[4,y] )
    asm("MOVW R21:R20, R17:R16");
    asm("COM R21");
    asm("COM R20");
    asm("AND   R21,  R19");
    asm("AND   R20,  R18");
    asm("EOR   R21,   R5");
    asm("EOR   R20,   R4");
    asm("STD Z + 16, R20");
    asm("STD Z + 17, R21");
  
    // x[3,y] ^ ( ~x[4,y] & x[0,y] )
    asm("MOVW R21:R20, R19:R18");
    asm("COM R21");
    asm("COM R20");
    asm("AND   R21,   R1");
    asm("AND   R20,   R0");
    asm("EOR   R21,  R17");
    asm("EOR   R20,  R16");
    asm("STD Z + 24, R20");
    asm("STD Z + 25, R21");
  
    // x[4,y] ^ ( ~x[0,y] & x[1,y] )
    asm("MOVW R21:R20, R1:R0");
    asm("COM R21");
    asm("COM R20");
    asm("AND   R21,   R3");
    asm("AND   R20,   R2");
    asm("EOR   R21,  R19");
    asm("EOR   R20,  R18");
    asm("STD Z + 32, R20");
    asm("STD Z + 33, R21");
  
    // Лічильник R23 + 1
    asm("SUBI R23, (-2)");
  
    // Z -> Z + 1
    asm("ADIW R30,   2 ");
  
    // Поки не опрацюємо "plane"
    asm("CPI  R23,   8 ");
  asm("BRNE chi_cycle");
  
  // Лічильник R23 = 0
  // Лічильник R22 = R22 + 40
  // Z -> Z + 32
  asm("LDI  R23,   0 ");
  asm("ADIW R30,  32 ");
  asm("SUBI R22,(-40)");
  
  // Поки не опрацюємо усе
  asm("CPI  R22, 200 ");
  asm("BRNE chi_cycle");
  
  // Зі стеку
  asm("LD R5, Y+");
  asm("LD R4, Y+");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void iota(uint8_t round)
{
  // RC -> Z
  asm("LDI R30,  LOW(RC)");
  asm("LDI R31, HIGH(RC)");
  
  // Z + round
  asm("LSL R16");
  asm("LSL R16");
  asm("LDI R17,   0");
  asm("ADD R30, R16");
  asm("ADC R31, R17");
  
  // RC -> R0-3
  asm("LPM R0, Z+");
  asm("LPM R1, Z+");
  asm("LPM R2, Z+");
  asm("LPM R3, Z+");
  
  // state -> Z
  asm("LDI R30,  LOW(state)");
  asm("LDI R31, HIGH(state)");
  
  // state[0:7] -> R20-23
  asm("LD  R20, Z    ");
  asm("LDD R21, Z + 1");
  asm("LDD R22, Z + 3");
  asm("LDD R23, Z + 7");
  
  // state ^ RC
  asm("EOR R20, R0");
  asm("EOR R21, R1");
  asm("EOR R22, R2");
  asm("EOR R23, R3");
  
  // R20-23 -> state[0:7]
  asm("ST  Z    , R20");
  asm("STD Z + 1, R21");
  asm("STD Z + 3, R22");
  asm("STD Z + 7, R23");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ROT_from_61_to_3(void)
{
  // Завантажуємо смугу
  asm("LD   R0, Z    ");
  asm("LDD  R1, Z + 1");
  asm("LDD  R2, Z + 2");
  asm("LDD  R3, Z + 3");
  asm("LDD R16, Z + 4");
  asm("LDD R17, Z + 5");
  asm("LDD R18, Z + 6");
  asm("LDD R19, Z + 7");
  
  // Перевірка напрямку зсуву
  asm("check_ROT_from_61_to_3:");
    asm("CPI R22, 0");
    asm("BRNE ROT_from_61_to_3_rigth");
  
    // Зсув на одиницю вліво
    asm("BST R19, 7");
    asm("LSL  R0");
    asm("ROL  R1");
    asm("ROL  R2");
    asm("ROL  R3");
    asm("ROL R16");
    asm("ROL R17");
    asm("ROL R18");
    asm("ROL R19");
    asm("BLD  R0, 0");
    asm("RJMP ROT_from_61_to_3_condition");
  
    // Зсув на одиницю вправо
    asm("ROT_from_61_to_3_rigth:");
    asm("BST  R0, 0");
    asm("LSR R19");
    asm("ROR R18");
    asm("ROR R17");
    asm("ROR R16");
    asm("ROR  R3");
    asm("ROR  R2");
    asm("ROR  R1");
    asm("ROR  R0");
    asm("BLD R19, 7");
  
    asm("ROT_from_61_to_3_condition:");
    asm("DEC R23");
  asm("BRNE check_ROT_from_61_to_3");
  
  // Зберігаємо смугу
  asm("ST Z+,  R0");
  asm("ST Z+,  R1");
  asm("ST Z+,  R2");
  asm("ST Z+,  R3");
  asm("ST Z+, R16");
  asm("ST Z+, R17");
  asm("ST Z+, R18");
  asm("ST Z+, R19");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ROT_from_6_to_10(void)
{
  // Завантажуємо смугу
  asm("LD   R0, Z    ");
  asm("LDD  R1, Z + 1");
  asm("LDD  R2, Z + 2");
  asm("LDD  R3, Z + 3");
  asm("LDD R16, Z + 4");
  asm("LDD R17, Z + 5");
  asm("LDD R18, Z + 6");
  asm("LDD R19, Z + 7");
  
  // Перевірка на 8
  asm("CPI R23, 0");
  asm("BREQ if_8");
  
  // Перевірка напрямку зсуву
  asm("check_ROT_from_6_to_10:");
    asm("CPI R22, 0");
    asm("BRNE ROT_from_6_to_10_rigth");
  
    // Зсув на одиницю вліво
    asm("BST R19, 7");
    asm("LSL  R0");
    asm("ROL  R1");
    asm("ROL  R2");
    asm("ROL  R3");
    asm("ROL R16");
    asm("ROL R17");
    asm("ROL R18");
    asm("ROL R19");
    asm("BLD  R0, 0");
    asm("RJMP ROT_from_6_to_10_condition");
  
    // Зсув на одиницю вправо
    asm("ROT_from_6_to_10_rigth:");
    asm("BST  R0, 0");
    asm("LSR R19");
    asm("ROR R18");
    asm("ROR R17");
    asm("ROR R16");
    asm("ROR  R3");
    asm("ROR  R2");
    asm("ROR  R1");
    asm("ROR  R0");
    asm("BLD R19, 7");
  
    asm("ROT_from_6_to_10_condition:");
    asm("DEC R23");
  asm("BRNE check_ROT_from_6_to_10");
  
  // Зберігаємо смугу (тут і зсуваємо на 8 біт)
  asm("if_8:");
  asm("ST Z+, R19");
  asm("ST Z+,  R0");
  asm("ST Z+,  R1");
  asm("ST Z+,  R2");
  asm("ST Z+,  R3");
  asm("ST Z+, R16");
  asm("ST Z+, R17");
  asm("ST Z+, R18");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ROT_from_14_to_18(void)
{
  // Завантажуємо смугу
  asm("LD   R0, Z    ");
  asm("LDD  R1, Z + 1");
  asm("LDD  R2, Z + 2");
  asm("LDD  R3, Z + 3");
  asm("LDD R16, Z + 4");
  asm("LDD R17, Z + 5");
  asm("LDD R18, Z + 6");
  asm("LDD R19, Z + 7");
  
  // Перевірка напрямку зсуву
  asm("check_ROT_from_14_to_18:");
    asm("CPI R22, 0");
    asm("BRNE ROT_from_14_to_18_rigth");
  
    // Зсув на одиницю вліво
    asm("BST R19, 7");
    asm("LSL  R0");
    asm("ROL  R1");
    asm("ROL  R2");
    asm("ROL  R3");
    asm("ROL R16");
    asm("ROL R17");
    asm("ROL R18");
    asm("ROL R19");
    asm("BLD  R0, 0");
    asm("RJMP ROT_from_14_to_18_condition");
  
    // Зсув на одиницю вправо
    asm("ROT_from_14_to_18_rigth:");
    asm("BST  R0, 0");
    asm("LSR R19");
    asm("ROR R18");
    asm("ROR R17");
    asm("ROR R16");
    asm("ROR  R3");
    asm("ROR  R2");
    asm("ROR  R1");
    asm("ROR  R0");
    asm("BLD R19, 7");
  
    asm("ROT_from_14_to_18_condition:");
    asm("DEC R23");
  asm("BRNE check_ROT_from_14_to_18");
  
  // Зберігаємо смугу (тут і зсуваємо на 16 біт)
  asm("ST Z+, R18");
  asm("ST Z+, R19");
  asm("ST Z+,  R0");
  asm("ST Z+,  R1");
  asm("ST Z+,  R2");
  asm("ST Z+,  R3");
  asm("ST Z+, R16");
  asm("ST Z+, R17");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ROT_from_20_to_28(void)
{
  // Завантажуємо смугу
  asm("LD   R0, Z    ");
  asm("LDD  R1, Z + 1");
  asm("LDD  R2, Z + 2");
  asm("LDD  R3, Z + 3");
  asm("LDD R16, Z + 4");
  asm("LDD R17, Z + 5");
  asm("LDD R18, Z + 6");
  asm("LDD R19, Z + 7");
  
  // Перевірка напрямку зсуву
  asm("check_ROT_from_20_to_28:");
    asm("CPI R22, 0");
    asm("BRNE ROT_from_20_to_28_rigth");
  
    // Зсув на одиницю вліво
    asm("BST R19, 7");
    asm("LSL  R0");
    asm("ROL  R1");
    asm("ROL  R2");
    asm("ROL  R3");
    asm("ROL R16");
    asm("ROL R17");
    asm("ROL R18");
    asm("ROL R19");
    asm("BLD  R0, 0");
    asm("RJMP ROT_from_20_to_28_condition");
  
    // Зсув на одиницю вправо
    asm("ROT_from_20_to_28_rigth:");
    asm("BST  R0, 0");
    asm("LSR R19");
    asm("ROR R18");
    asm("ROR R17");
    asm("ROR R16");
    asm("ROR  R3");
    asm("ROR  R2");
    asm("ROR  R1");
    asm("ROR  R0");
    asm("BLD R19, 7");
    
    asm("ROT_from_20_to_28_condition:");
    asm("DEC R23");
  asm("BRNE check_ROT_from_20_to_28");
  
  // Зберігаємо смугу (тут і зсуваємо на 24 біт)
  asm("ST Z+, R17");
  asm("ST Z+, R18");
  asm("ST Z+, R19");
  asm("ST Z+,  R0");
  asm("ST Z+,  R1");
  asm("ST Z+,  R2");
  asm("ST Z+,  R3");
  asm("ST Z+, R16");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ROT_from_36_to_45(void)
{
  // Завантажуємо смугу
  asm("LD   R0, Z    ");
  asm("LDD  R1, Z + 1");
  asm("LDD  R2, Z + 2");
  asm("LDD  R3, Z + 3");
  asm("LDD R16, Z + 4");
  asm("LDD R17, Z + 5");
  asm("LDD R18, Z + 6");
  asm("LDD R19, Z + 7");
  
  // Перевірка напрямку зсуву
  asm("check_ROT_from_36_to_45:");
    asm("CPI R22, 0");
    asm("BRNE ROT_from_36_to_45_rigth");
    
    // Зсув на одиницю вліво
    asm("BST R19, 7");
    asm("LSL  R0");
    asm("ROL  R1");
    asm("ROL  R2");
    asm("ROL  R3");
    asm("ROL R16");
    asm("ROL R17");
    asm("ROL R18");
    asm("ROL R19");
    asm("BLD  R0, 0");
    asm("RJMP ROT_from_36_to_45_condition");
    
    // Зсув на одиницю вправо
    asm("ROT_from_36_to_45_rigth:");
    asm("BST  R0, 0");
    asm("LSR R19");
    asm("ROR R18");
    asm("ROR R17");
    asm("ROR R16");
    asm("ROR  R3");
    asm("ROR  R2");
    asm("ROR  R1");
    asm("ROR  R0");
    asm("BLD R19, 7");
    
    asm("ROT_from_36_to_45_condition:");
    asm("DEC R23");
  asm("BRNE check_ROT_from_36_to_45");
  
  // Зберігаємо смугу (тут і зсуваємо на 40 біт)
  asm("ST Z+,  R3");
  asm("ST Z+, R16");
  asm("ST Z+, R17");
  asm("ST Z+, R18");
  asm("ST Z+, R19");
  asm("ST Z+,  R0");
  asm("ST Z+,  R1");
  asm("ST Z+,  R2");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ROT_from_55_to_56(void)
{
  // Завантажуємо смугу
  asm("LD   R0, Z    ");
  asm("LDD  R1, Z + 1");
  asm("LDD  R2, Z + 2");
  asm("LDD  R3, Z + 3");
  asm("LDD R16, Z + 4");
  asm("LDD R17, Z + 5");
  asm("LDD R18, Z + 6");
  asm("LDD R19, Z + 7");
  
  // Перевірка на 56
  asm("CPI R23, 0");
  asm("BREQ if_56");
  
  // Перевірка напрямку зсуву
  asm("check_ROT_from_55_to_56:");
    asm("CPI R22, 0");
    asm("BRNE ROT_from_55_to_56_rigth");
    
    // Зсув на одиницю вліво
    asm("BST R19, 7");
    asm("LSL  R0");
    asm("ROL  R1");
    asm("ROL  R2");
    asm("ROL  R3");
    asm("ROL R16");
    asm("ROL R17");
    asm("ROL R18");
    asm("ROL R19");
    asm("BLD  R0, 0");
    asm("RJMP ROT_from_55_to_56_condition");
    
    // Зсув на одиницю вправо
    asm("ROT_from_55_to_56_rigth:");
    asm("BST  R0, 0");
    asm("LSR R19");
    asm("ROR R18");
    asm("ROR R17");
    asm("ROR R16");
    asm("ROR  R3");
    asm("ROR  R2");
    asm("ROR  R1");
    asm("ROR  R0");
    asm("BLD R19, 7");
    
    asm("ROT_from_55_to_56_condition:");
    asm("DEC R23");
  asm("BRNE check_ROT_from_55_to_56");
  
  // Зберігаємо смугу (тут і зсуваємо на 56 біт)
  asm("if_56:");
  asm("ST Z+,  R1");
  asm("ST Z+,  R2");
  asm("ST Z+,  R3");
  asm("ST Z+, R16");
  asm("ST Z+, R17");
  asm("ST Z+, R18");
  asm("ST Z+, R19");
  asm("ST Z+,  R0");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
__flash uint8_t piTranslation[] = {
   0, 10, 20,  5, 15,
  16,  1, 11, 21,  6,
   7, 17,  2, 12, 22,
  23,  8, 18,  3, 13,
  14, 24,  9, 19,  4
};

__flash uint8_t RC[] = {
  0x01, 0x00, 0x00, 0x00,
  0x82, 0x80, 0x00, 0x00,
  0x8A, 0x80, 0x00, 0x80,
  0x00, 0x80, 0x80, 0x80,
  0x8B, 0x80, 0x00, 0x00,
  0x01, 0x00, 0x80, 0x00,
  0x81, 0x80, 0x80, 0x80,
  0x09, 0x80, 0x00, 0x80,
  0x8A, 0x00, 0x00, 0x00,
  0x88, 0x00, 0x00, 0x00,
  0x09, 0x80, 0x80, 0x00,
  0x0A, 0x00, 0x80, 0x00,
  0x8B, 0x80, 0x80, 0x00,
  0x8B, 0x00, 0x00, 0x80,
  0x89, 0x80, 0x00, 0x80,
  0x03, 0x80, 0x00, 0x80,
  0x02, 0x80, 0x00, 0x80,
  0x80, 0x00, 0x00, 0x80,
  0x0A, 0x80, 0x00, 0x00,
  0x0A, 0x00, 0x80, 0x80,
  0x81, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x00, 0x80,
  0x01, 0x00, 0x80, 0x00,
  0x08, 0x80, 0x80, 0x80,
};
// ////////////////////////////////////////////////////////////////////////////
