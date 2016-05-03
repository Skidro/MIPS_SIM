#ifndef MIPS_ARCH
#define MIPS_ARCH

#define MIPS_ARCH_REGS	32

#define MEM_TEXT_START	0x00400000
#define	MEM_TEXT_SIZE	0x00000080		/* 128 words 			*/

#define MEM_DATA_START	0x10000000
#define MEM_DATA_SIZE	0x00000080		/* 128 words 			*/

#define MIPS_SYSCALL	0x0000000c		/* Terminate Instruction 	*/


/*********************************************************************
 * Helper Functions for Reading and Writing Data to Text and Data
 * Sections 
 ********************************************************************/

#define mem_read_text(address, bank, return_val)					\
	do {										\
		return_val = bank[((address - MEM_TEXT_START) / 4)];			\
	} while (0);

#define mem_read_data(address, bank, return_val)					\
	do {										\
		return_val = bank[address - MEM_DATA_START];				\
	} while (0);

#define mem_write_data(address, bank, value_to_write)					\
	do {										\
		bank[address - MEM_DATA_START] = value_to_write;			\
	} while (0);


#endif /* MIPS_ARCH */
