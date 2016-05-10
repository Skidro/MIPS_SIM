/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   ACKNOWLDGMENT: Most part of this file and the listed      */
/*                  routines has been taken from the CMU       */
/*                  Course ECE-447 Assignment-1                */
/*                  The original source can be found here:     */
/*   http://www.ece.cmu.edu/~ece447/s15/doku.php?id=labs       */
/***************************************************************/

#ifndef _SIM_SHELL_H_
#define _SIM_SHELL_H_

#include <stdint.h>
#include "mips_arch.h"

#define FALSE 0
#define TRUE  1

#define MIPS_REGS 	MIPS_ARCH_REGS

typedef struct CPU_State_Struct {

  uint32_t PC;		/* program counter */
  uint32_t REGS[MIPS_REGS]; /* register file. */
  uint32_t HI, LO;          /* special regs for mult/div. */
} CPU_State;

/* Data Structure for Latch */

extern CPU_State CURRENT_STATE, NEXT_STATE;

extern int RUN_BIT;	/* run bit */

uint32_t mem_read_32(uint32_t address);
void     mem_write_32(uint32_t address, uint32_t value);

/* Simulation Functions */
void rdump(FILE* dumpsim);
void mdump(FILE* dimpsim);
void simulate(char* program_fd);

/* Primary function which executes a single hex instruction */
void process_instruction();

#endif
