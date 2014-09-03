#define DIV          8 // На скільки ділимо 64-бітне слово
#define STEP         3 // На скільки ділимо смугу, для операції зсуву 
#define ROUNDS      24 // К-сть раундів
#define LANES       25
#define CAPACITY   576
#define RATE      1024
#define KeccakPermutationSizeInBytes 200

extern uint8_t state[LANES*DIV];
extern uint8_t msgBlock[KeccakPermutationSizeInBytes]; // Перед/після KeccakF()

// ////////////////////////////////////////////////////////////////////////////
void KeccakF(void);
void theta(void);
void rho(void);
void pi(void);
void chi(void);
void iota(uint8_t round);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ROT_from_61_to_3(void);
void ROT_from_6_to_10(void);
void ROT_from_14_to_18(void);
void ROT_from_20_to_28(void);
void ROT_from_36_to_45(void);
void ROT_from_55_to_56(void);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void padding(uint8_t tempLen);
void xor_block_to_state(void);
void cpy_hash_to_block(void);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void stateInit(void);
// ////////////////////////////////////////////////////////////////////////////
