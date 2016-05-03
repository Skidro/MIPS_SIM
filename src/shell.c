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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "shell.h"

typedef struct {
    uint32_t start, size;
    uint32_t *mem;
} mem_region_t;

/* memory will be dynamically allocated at initialization */
mem_region_t MEM_REGIONS[] = {
    { MEM_TEXT_START, MEM_TEXT_SIZE, NULL },
    { MEM_DATA_START, MEM_DATA_SIZE, NULL },
};

#define MEM_NREGIONS (sizeof(MEM_REGIONS)/sizeof(mem_region_t))

/***************************************************************/
/* CPU State info.                                             */
/***************************************************************/

CPU_State 	CURRENT_STATE, NEXT_STATE;
int 		RUN_BIT;			/* run bit */
int 		INSTRUCTION_COUNT;

/*                                                          
 * Procedure: mem_read_32                                    
 * Purpose: Read a 32-bit word from memory                    
 */
uint32_t mem_read_32(uint32_t address)
{
    int i;
    for (i = 0; i < MEM_NREGIONS; i++) {
        if (address >= MEM_REGIONS[i].start &&
                address < (MEM_REGIONS[i].start + MEM_REGIONS[i].size)) {
            uint32_t offset = address - MEM_REGIONS[i].start;

            return
                ((i == 0)? MEM_REGIONS[i].mem[offset/4] : MEM_REGIONS[i].mem[offset]);
        }
    }

    return 0;
}

/*                                                           
 * Procedure: mem_write_32                                    
 * Purpose: Write a 32-bit word to memory                   
 */ 
void mem_write_32(uint32_t address, uint32_t value)
{
    int i;
    for (i = 0; i < MEM_NREGIONS; i++) {
        if (address >= MEM_REGIONS[i].start &&
                address < (MEM_REGIONS[i].start + MEM_REGIONS[i].size)) {
            uint32_t offset = address - MEM_REGIONS[i].start;

	    if (i == 0)
            	MEM_REGIONS[i].mem[offset/4] = value;
	    else
		MEM_REGIONS[i].mem[offset]   = value;
            return;
        }
    }
}

/*                                                           
 * Procedure : cycle                                          
 * Purpose   : Execute a cycle                              
 */
void cycle() {                                                

  process_instruction();
  CURRENT_STATE = NEXT_STATE;
  INSTRUCTION_COUNT++;
}

/*                                                          
 * Procedure : go                                            
 * Purpose   : Simulate MIPS until HALTed                     
 */
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  while (RUN_BIT)
    cycle();
}

/*                                                         
 * Procedure : mdump                                        
 * Purpose   : Dump a word-aligned region of memory to the   
 *             output file.                                   
 */
void mdump(FILE* dumpsim_file) {
  int start = MEM_REGIONS[1].start;
  int size  = MEM_REGIONS[1].size;
  int i, j;

  fprintf(dumpsim_file, "\nMemory content [0x%08x..0x%08x] :\n", start, start + size);
  fprintf(dumpsim_file, "-------------------------------------\n");

  for (i = 0; i < size/4; i++) {
    for (j = 0; j < 4; j++) {
      fprintf(dumpsim_file, "MEM[%03d] : 0x%08x\t", (i*4 + j), mem_read_32(start + (i*4 + j)));
    }
    fprintf(dumpsim_file, "\n");
  }

  fprintf(dumpsim_file, "\n");
}

/*                                                         
 * Procedure : rdump                                        
 * Purpose   : Dump current register and bus values to the   
 *             output file.                                   
 */ 
void rdump(FILE * dumpsim_file) {                               
  int i, j; 

  fprintf(dumpsim_file, "Instruction Count           : %u\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "Final PC Value              : 0x%08x\n", CURRENT_STATE.PC);
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Registers:\n");

  for (i = 0; i < MIPS_REGS/4; i++) {
    for (j = 0; j < 4; j++) {
      fprintf(dumpsim_file, "R%.2d: 0x%08x (%10d)", (i*4 + j), CURRENT_STATE.REGS[(i*4 + j)], CURRENT_STATE.REGS[(i*4 + j)]);
    }
    fprintf(dumpsim_file, "\n");
  }

  fprintf(dumpsim_file, "\n");
}

/*                                                         
 * Procedure : init_memory                                  
 * Purpose   : Allocate and zero memoryy                     
 */
void init_memory() {                                           
    int i;
    for (i = 0; i < MEM_NREGIONS; i++) {
        MEM_REGIONS[i].mem = malloc(MEM_REGIONS[i].size * sizeof(uint32_t));
        memset(MEM_REGIONS[i].mem, 0, MEM_REGIONS[i].size * sizeof(uint32_t));
    }
}

/*                                                       
 * Procedure : load_program                               
 * Purpose   : Load program and service routines into mem.   
 */                                                           
void load_program(FILE *prog) {
  int ii, word;

  /* Open program file. */
  if (prog == NULL) {
    printf("[Error] Can't open program file\n");
    exit(-1);
  }

  /* Read in the program. */
  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    mem_write_32(MEM_TEXT_START + ii, word);
    ii += 4;
  }

  CURRENT_STATE.PC = MEM_TEXT_START;

  return;
}

/*                                                          
 * Procedure : initialize                                  
 * Purpose   : Load machine language program              
 *             and set up initial state of the machine  
 */
void initialize(char* file_name) {
  int i;
  FILE* program_fd;

  program_fd = fopen (file_name, "r");

  init_memory();
  load_program(program_fd);

  /* Set the initial architectural state */
  NEXT_STATE = CURRENT_STATE;
  RUN_BIT = TRUE;
}

/*                                                           
 * Procedure : simulate
 * Purpose   : Run the simulator till the end of the given program
 */
void simulate(char* program_name) {
  /* Perform the necessary initializations */
  initialize(program_name);

  /* Run the simulator till the program finishes */
  go(); 
}
