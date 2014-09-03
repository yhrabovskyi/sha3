#include <stdint.h>

#define RATE_IN_BYTES 128
#define RATE_IN_LANES 16
#define CAPACITY_IN_LANES 9

extern uint32_t state[50];
extern uint32_t D[10];
extern uint8_t msgBlock[200]; // Вх./вих. байти перед/після KeccakF()

// ////////////////////////////////////////////////////////////////////////////
void KeccakF(void);
void fromWordsToBytes(void);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void stateInit(void);
void padding(uint8_t tempLen);
// ////////////////////////////////////////////////////////////////////////////
