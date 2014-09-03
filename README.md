This is a part of the diploma work. Here we have 2 implementations of SHA-3 based on Keccak specifications, version 2,
but with parameters r = 1024 and c = 576. Main purpose is to measure:
  1. How many clocks algorithm is needed for process one byte of a long message;
  2. How much memory is needed for the algorithm;
  3. Size of code.

There is only the source code without full projects. Measurement was carried out as iteration, which represents 102400
bytes of the message and without padding of the last block of the message, because it didn't have any effect on measurement of clocks.
Also it was carried out with using IAR EW simulator. Comments have been written in Ukrainian, so there can be some problems with encoding.

One implementation (sha3_arm) is for Cortex-M3/M4 microcontrollers. It has been written in assembler, only padding function - in C.
Bit interleaving was used. Some steps in the algorithm were merged.

Another implementation (sha3_avr) is for AVR microcontrollers. Most of the functions have been written in assembler, some - in C.
Bit interleaving wasn't used. For get smaller and faster code shifts in rho function were divided into several groups. Since
in these microcontrollers there is no free shift, only for one byte, it was easier to swap bytes and then perform shifts.
These groups are based on this condition.

April-June, 2013.