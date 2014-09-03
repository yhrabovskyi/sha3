#include "keccakH.h"

uint8_t state[LANES*DIV];

// Перший тестовий вектор, 144 біт
uint8_t msgBlock[KeccakPermutationSizeInBytes] = {
  0x75, 0x68, 0x3D, 0xCB, 0x55, 0x61, 0x40, 0xC5, 0x22, 0x54, 0x3B, 0xB6, 0xE9, 0x09, 0x8B, 0x21,
  0xA2, 0x1E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Результат до першого вектора
//  0x90, 0x73, 0xc6, 0x25, 0x55, 0xe6, 0x09, 0x5f, 0x17, 0xdf, 0x71, 0xad, 0x02, 0xba, 0xbb, 0x91,
//  0x00, 0x28, 0x86, 0x33, 0x89, 0x84, 0x89, 0xb2, 0x1c, 0x90, 0x6a, 0x31, 0x90, 0x87, 0x5b, 0xae,
//  0xac, 0xcc, 0x83, 0xbe, 0x80, 0xab, 0xd1, 0x14, 0x66, 0xfe, 0xc3, 0x71, 0xba, 0x2c, 0x46, 0x23,
//  0xd0, 0x7f, 0x01, 0x31, 0xde, 0xfa, 0xec, 0x13, 0xa8, 0xc7, 0x32, 0xa9, 0xf8, 0x41, 0x71, 0x63,
//  0x13, 0x1b, 0xc2, 0xb5, 0xea, 0x05, 0x44, 0xb2, 0x6d, 0x69, 0xcb, 0x1a, 0xe1, 0xf3, 0x95, 0xde,
//  0xad, 0x2c, 0x05, 0x86, 0xc1, 0x74, 0x8d, 0xce, 0xc0, 0x3f, 0xa4, 0xde, 0xf7, 0xc5, 0xc4, 0xe5,
//  0xd3, 0xd2, 0x13, 0x67, 0x32, 0x02, 0x7f, 0x38, 0xbf, 0xa6, 0x84, 0x2a, 0x21, 0xf2, 0x81, 0x07,
//  0xf6, 0xd3, 0x25, 0x9b, 0x57, 0xbe, 0x92, 0x60, 0x9d, 0x5c, 0x37, 0x5c, 0xf5, 0x8b, 0x04, 0xc5,
//  0x28, 0x97, 0x55, 0x65, 0xf1, 0x20, 0x43, 0x8f, 0xf2, 0x48, 0xf3, 0xd2, 0x7a, 0xb2, 0x68, 0x0f,
//  0xeb, 0x44, 0xee, 0x50, 0xdf, 0xcc, 0xc4, 0x40, 0x92, 0xe3, 0x98, 0xcf, 0xda, 0x58, 0xd5, 0x78,
//  0xcb, 0xb0, 0x65, 0x39, 0xe3, 0x58, 0x0b, 0x37, 0x03, 0x84, 0x62, 0x68, 0x08, 0x66, 0x42, 0x0f,
//  0x56, 0x19, 0xd3, 0xd9, 0xb7, 0x82, 0x3a, 0x94, 0x24, 0xf6, 0x39, 0x53, 0x01, 0x17, 0x94, 0xb1,
//  0x74, 0xad, 0x19, 0x4c, 0x33, 0x38, 0x5f, 0x66



//// Другий тестовий вектор, 408 біт
//uint8_t msgBlock[KeccakPermutationSizeInBytes] = {
//  0x9A, 0x21, 0x9B, 0xE4, 0x37, 0x13, 0xBD, 0x57, 0x80, 0x15, 0xE9, 0xFD, 0xA6, 0x6C, 0x0F, 0x2D,
//  0x83, 0xCA, 0xC5, 0x63, 0xB7, 0x76, 0xAB, 0x9F, 0x38, 0xF3, 0xE4, 0xF7, 0xEF, 0x22, 0x9C, 0xB4,
//  0x43, 0x30, 0x4F, 0xBA, 0x40, 0x1E, 0xFB, 0x2B, 0xDB, 0xD7, 0xEC, 0xE9, 0x39, 0x10, 0x22, 0x98,
//  0x65, 0x1C, 0x86, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
//};

// Результат до другого вектора
//  0xe4, 0xbb, 0xd5, 0x4b, 0xfb, 0x99, 0xd3, 0x45, 0x47, 0x1f, 0x8a, 0xb9, 0x42, 0x71, 0xb4, 0xb7,
//  0x48, 0xf5, 0xce, 0x70, 0xc2, 0x1c, 0x28, 0xae, 0x65, 0x59, 0xe0, 0x3e, 0xe7, 0x89, 0x0a, 0x2c,
//  0x81, 0x40, 0x43, 0xe6, 0x24, 0xa6, 0xbd, 0x29, 0x44, 0x35, 0x07, 0x56, 0xb3, 0x7f, 0xa8, 0x20,
//  0x8f, 0xc7, 0x47, 0x3a, 0x67, 0xb3, 0x10, 0xce, 0xeb, 0xc1, 0x7d, 0x96, 0x5e, 0xd6, 0x88, 0xb2,
//  0x43, 0xfc, 0x78, 0x2d, 0xfb, 0x6b, 0x59, 0x17, 0x4b, 0x06, 0x26, 0x6f, 0x53, 0x0b, 0xaf, 0xe6,
//  0x8e, 0x57, 0x6c, 0xd5, 0x61, 0x48, 0x21, 0x73, 0x00, 0x11, 0xa5, 0x5a, 0x1c, 0x3a, 0x60, 0x9e,
//  0x14, 0xf7, 0xb7, 0xed, 0xce, 0x3f, 0x17, 0x1b, 0x8b, 0xac, 0x17, 0x96, 0x8f, 0xb7, 0x16, 0x15,
//  0x61, 0xd9, 0x67, 0x81, 0x31, 0xf8, 0x9a, 0xfc, 0x79, 0x06, 0x40, 0x51, 0x67, 0x79, 0x27, 0x81,
//  0x78, 0x1e, 0xcd, 0x30, 0xd2, 0x5a, 0x76, 0x52, 0x38, 0x63, 0x56, 0x7d, 0x33, 0x4e, 0x20, 0x33,
//  0x47, 0x62, 0xf6, 0x69, 0x68, 0x14, 0x37, 0x3c, 0xfb, 0x03, 0xad, 0x9c, 0x11, 0xee, 0xe8, 0x3c,
//  0x92, 0x01, 0x16, 0x68, 0x1f, 0x1c, 0xb4, 0x86, 0xde, 0x33, 0xc7, 0x03, 0xab, 0xcb, 0x68, 0xc5,
//  0x09, 0x2f, 0x7f, 0xf9, 0x91, 0xea, 0x96, 0x64, 0xef, 0xff, 0xaf, 0x2a, 0xb5, 0xab, 0x16, 0xe4,
//  0x52, 0x81, 0x16, 0x7f, 0xa2, 0x31, 0x01, 0xec

// ////////////////////////////////////////////////////////////////////////////
void main(void)
{
  stateInit();
  for (uint32_t i = 0; i < 8 * 100; i++)
  {
    xor_block_to_state();
    KeccakF();
  }
  cpy_hash_to_block();
}
// ////////////////////////////////////////////////////////////////////////////