#ifndef HELPER_FUNCS_H
#define HELPER_FUNCS_H

/* 
 * precedure : rdump_map
 * purpose   : dump core register value of mips processor to given output file
 */
#define rdump_map(fd, core_dump) 											\
	do {														\
		fprintf(fd, "\nCurrent register/bus values :\n");							\
		fprintf(fd, "------------------------------------\n");							\
		fprintf(fd, "Register:\n");										\
															\
		for(i = 0; i < MIPS_ARCH_REGS/4; i++) {									\
			for (j = 0; j < 4; j++) {									\
                		fprintf(fd, "R%02d: 0x%08x (%10d)", ((i*4) + j), core_dump[((i*4)+j)], core_dump[((i*4)+j)]);		\
        		}												\
        		fprintf(fd, "\n");										\
    		}													\
															\
    		fprintf(fd, "\n");											\
	} while (0);												

/* 
 * precedure : mdump_map
 * purpose   : dump date-memory value of mips processor to given output file
 */
#define mdump_map(fd, data_memory) 											\
	do {														\
		fprintf(fd, "\nMemory content [0x%08x..0x%08x] :\n", MEM_DATA_START, MEM_DATA_START + MEM_DATA_SIZE);	\
		fprintf(fd, "-------------------------------------\n");							\
															\
		for (i = 0; i < MEM_DATA_SIZE/4; i++) {									\
			for (j = 0; j < 4; j++) {									\
				fprintf(fd, "MEM[%03d] : 0x%08x\t", ((i*4) + j), data_memory[((i*4)+j)]);		\
			}												\
			fprintf(fd, "\n");										\
		}													\
															\
		fprintf(fd, "\n");											\
	} while (0);

#endif /* HELPER_FUNCS_H */
