#include <libmap.h>
#include <stdlib.h>

#include "helper_functions.h"
#include "mips_arch.h"

#define MAX_OBM_SIZE 	512000

/* Debug related defines */
#define PR_SUCCESS	0
#define PR_FAILURE	-1

void subr (int64_t text_memory[], int64_t core_dump[], int64_t data_memory[], int64_t* pc_value, int64_t* instr_executed, int64_t* time, int mapnum);

int main (int argc, char *argv[]) 
{
    FILE 	*res_map, *res_cpu;
    int	 	i,j;

    /* 
     * Text memory routes to Bank A and it is the input to Map Processor
     * 	### Currently limited to 100 words - Bank A
     * Core Dump is the output from MAP processer and it contains the 
     * architecture state in the form of register file data
     *  ### Fixed at 32 words - Bank B
     * Data memory is also the output from MAP processor and it contains
     * the final state of processor's memory 
     *  ### Currently limited to 100 words - Bank D
     */
    int64_t 	*text_memory, *core_dump, *data_memory;
    int64_t 	tm, accum=0;
    int 	mapnum = 0;

    /* Statically initialize PC to the start of text memory */
    int64_t	pc_value = MEM_TEXT_START;

    /* Program File Variable */
    FILE	*program;
    int		instruction_word;
    int         instruction_count;
    int64_t     instructions_executed;

    /* Preliminary Error Checking */
    if (argc < 2) {
	/* Dislay correct usage information */
	printf ("[Error] usage: %s <program_file>\n", argv[0]);
	exit (1);
    }

    /* Open the program file for reading */
    program = fopen (argv[1], "r");

    /* Check for the validity of program file */
    if (program == NULL) {
	printf ("[Error] Can't open program file %s\n", argv[1]);
	exit (1);
    }

    /* Initialize the memories on co-processor */
    text_memory	= (int64_t*) Cache_Aligned_Allocate ((MEM_TEXT_SIZE)  * sizeof (int64_t));
    core_dump	= (int64_t*) Cache_Aligned_Allocate ((MIPS_ARCH_REGS) * sizeof (int64_t));
    data_memory	= (int64_t*) Cache_Aligned_Allocate ((MEM_DATA_SIZE)  * sizeof (int64_t));

    /* Read the program file */
    instruction_count = 0;
    while (fscanf (program, "%x\n", &instruction_word) != EOF) {
	text_memory[instruction_count] = instruction_word;
	instruction_count++;
    }

    /* Close the program file */
    fclose (program);

    printf ("MIPS Simulator\n\n");
    printf ("Read %d words from program file into instruction memory.\n", instruction_count);

    /* Initialize the remaining instructions as zeros */
    for (i = instruction_count; i < (MEM_TEXT_SIZE); i++) {
	text_memory[i] = 0;
    }

    /* Open the files to print data from mips processor */
    if ((res_map = fopen ("res_map", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_map'\n");
        exit (1);
	}

    if ((res_cpu = fopen ("res_cpu", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_cpu'\n");
        exit (1);
    }

    /* Run the program on simulator */
    simulate(argv[1]);

    /* Dump the simulation result to res_cpu file */
    fprintf (res_cpu, "Program                     : %s\n", argv[1]);
    fprintf (res_cpu, "Clocks                      : 0\n");
    rdump (res_cpu);
    mdump (res_cpu);

    /* Execute the program on MAP processor */
    map_allocate (1);

    subr (text_memory, core_dump, data_memory, &pc_value, &instructions_executed, &tm, mapnum);

    /* Free the map processor */
    map_free (1);
    
    /* Print execution results to output files */
    fprintf (res_map, "Program                     : %s\n", argv[1]);
    fprintf (res_map, "Clocks                      : %lld\n", tm);
    fprintf (res_map, "Instruction Count           : %d\n", (int)instructions_executed);
    fprintf (res_map, "Final PC Value              : 0x%08x\n", (int)pc_value);
    
    rdump_map(res_map, core_dump);
    mdump_map(res_map, data_memory);

    /* Close result files */
    fclose (res_cpu);
    fclose (res_map);

    exit(0);
}
