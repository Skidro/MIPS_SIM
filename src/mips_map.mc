#include <libmap.h>
#include "pipeline_stages.h"

void subr (int64_t text_memory[], int64_t core_dump[], int64_t data_memory[], int64_t* pc_value, int64_t* instr_executed, int64_t* time, int mapnum) 
{
    /* Input Bank to read instruction file */
    OBM_BANK_A (AL, int64_t, MAX_OBM_SIZE)

    /* Output Banks | B - Architecture State ; C - Memory */
    OBM_BANK_B (BL, int64_t, MAX_OBM_SIZE)
    OBM_BANK_C (CL, int64_t, MAX_OBM_SIZE)

    /* Declare local variable for Program Counter - PC */
    uint32_t PC = *pc_value;			

    /* Declare intermediate variables for instruction processing */
    uint32_t opcode, rs, rt, rd, jump, shamt, funct;
    int16_t  immediate;

    /* Declare outputs for arithmatic logic unit i.e. ALU */
    int32_t alu_out, alu_zero;

    /* Declare outputs for memory unit */
    int32_t mem_out;

    /* Declare control flow signals */
    uint32_t ctl_mem_read, ctl_mem_write, ctl_reg_write, ctl_reg_dst, ctl_beq, ctl_bne, ctl_jump;

    int64_t t0, t1, status = 0;
    uint32_t instruction;
    int i;

    /* Transfer the instructions to execute to OBM Bank A */
    buffered_dma_cpu (CM2OBM, PATH_0, AL, MAP_OBM_stripe(1,"A"), text_memory, 1, (MEM_TEXT_SIZE) * 8);

    /* Initialize Bank-B (REG_FILE) to zero values */
    for (i = 0; i < (MIPS_ARCH_REGS); i++) 
    {
	BL[i] = 0;
    }

    /* Initialize Bank-C (Memory) to zero values */
    for (i = 0; i < (MEM_DATA_SIZE); i++)
    {
	CL[i] = 0;
    }

    /* Set the instruction-count to zero before starting the processor */
    i = 0;
    
    read_timer (&t0);
    
    while (1)
    {
	/* Increment the cycle count */
	i++;

	/* Execute the instruction-fetch stage 
	 * instruction_fetch stage will also advance the current PC to its next sequential value */
	instruction_fetch(status, instruction, PC, AL);

	if (status != 0) {
		break;
	}

	/* Update the runbit if the instruction is syscall */
	if (i == (MEM_TEXT_SIZE - 1) || instruction == MIPS_SYSCALL) {
		/* PC need not have advanced since this is syscall */
                PC -= 4;

		/* Break the execution */
		break;
	}

	/* Execute the instruction-decode stage 
	 * NOTE: I am leaving-out the generation of alu-control signals in here. The instruction
	 * execute stage will directly use the opcode and function field values to 
	 * determine the correct operation to execute in alu */
	instruction_decode(status, instruction, opcode, rs, rt, rd, immediate, jump, shamt, funct, PC);
	if (status != 1) {
		break;
	}

	/* This is a pseudo stage in instruction decode where we determine the control signals 
	 * based on the current instruction */
	generate_control_signals(opcode, funct, ctl_mem_read, ctl_mem_write, ctl_reg_write, ctl_reg_dst, ctl_beq, ctl_bne, ctl_jump);

	/* Execute the instruction-execute stage */
	instruction_execute(status, opcode, rs, rt, rd, immediate, shamt, funct, alu_out, alu_zero, BL);
	if (status != 2) {
		break;
	}

	/* This is a pseudo stage to process branch instructions since branch decision can only be taken after
	 * the instruction-execute stage */ 
	take_branch_decision(immediate, ctl_beq, ctl_bne, alu_zero, PC);
	

	/* Execute the memory-write stage 
	 * The memory write stage will use the OBM bank D which is declared as 'DL'
	 * to model the memory in MIPS processor */
	memory_write(status, ctl_mem_read, ctl_mem_write, BL[rt], alu_out, mem_out, CL);
	if (status != 3) {
		break;
	}

	/* Execute the write-back stage
	 * The write back stage uses the register file declared globally as 'BL' on 
	 * OBM bank B to write back the data from alu or memory to the MIPS core registers */
	write_back(status, ctl_mem_read, ctl_reg_write, ctl_reg_dst, rt, rd, alu_out, mem_out, BL);

	if (status != 4) {
		break;
	}
    }

    read_timer (&t1);

    /* Transfer the architectural state and memory state back to main processor */
    buffered_dma_cpu (OBM2CM, PATH_0, BL, MAP_OBM_stripe (1, "B"), core_dump, 1, (MIPS_ARCH_REGS) * 8);
    buffered_dma_cpu (OBM2CM, PATH_0, CL, MAP_OBM_stripe (1, "C"), data_memory, 1, (MEM_DATA_SIZE) * 8);

    *time 		= t1 - t0;
    *pc_value		= PC;
    *instr_executed	= i;

    return;
}
