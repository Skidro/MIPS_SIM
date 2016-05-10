#include <libmap.h>
#include "pipeline_stages.h"

void subr (int64_t text_memory[], int64_t core_dump[], int64_t data_memory[], int program_size, int64_t* pc_value, int64_t* time, int mapnum) 
{
    /* Create streams for sections to be pipelined */
    Stream_32 SA1;
    Stream_64 SB1, SB2, SB3, SB4;
    Stream_64 SC1, SC2, SC3, SC4;
    Stream_64 SD1, SD2;

    /* Input Bank to read instruction file */
    OBM_BANK_A (AL, int64_t, MAX_OBM_SIZE)

    /* Output Banks | B - Architecture State ; C - Memory */
    OBM_BANK_B (BL, int64_t, MAX_OBM_SIZE)
    OBM_BANK_C (CL, int64_t, MAX_OBM_SIZE)

    /* Declare local variable for Program Counter - PC */
    uint32_t PC = *pc_value;			

    int64_t t0, t1;
    int status_if, status_id, status_ie, status_mw, status_wb;
    int i;

    /* Get the instruction stream from co-processor */
    buffered_dma_cpu (CM2OBM, PATH_0, AL, MAP_OBM_stripe (1, "A"), text_memory, 1, (program_size) * 8);

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

    /* Benchmark the start-time */
    read_timer (&t0);

    #pragma src parallel sections
    {
	#pragma src section
	{
	    uint32_t p_instruction_if;
	    int iter;

	    for (iter = 0; iter < (program_size); iter++) {
		instruction_fetch(status_if, p_instruction_if, PC, AL);

		/* Break execution if the instruction is syscall */
		put_stream_32(&SA1, p_instruction_if, 1);
	    }
	}

	#pragma src section
	{
	    uint32_t p_instruction_id;
	    uint64_t opcode_id, rs_id, rt_id, rd_id, immediate_id, jump_id, shamt_id, funct_id;
	    uint64_t interm1_id, interm2_id, interm3_id, interm4_id;
	    int iter;

	    for (iter = 0; iter < (program_size); iter++) {
		get_stream_32(&SA1, &p_instruction_id);
		
		/* Decode the instruction */
		/* FIXME: Jump isn't required to be propagated further then this stage */
		instruction_decode(status_id, p_instruction_id, opcode_id, rs_id, rt_id, rd_id, immediate_id, jump_id, shamt_id, funct_id, PC);
	
		/* Populate intermediate variables for data-packing */
		interm1_id = (opcode_id << 32) | rs_id;
		interm2_id = (rt_id << 32) | rd_id;
		interm3_id = (immediate_id << 32) | shamt_id;
		interm4_id = (uint64_t)funct_id;
		
		/* Populate the next streams */
		put_stream_64(&SB1, interm1_id, 1);
		put_stream_64(&SB2, interm2_id, 1);
		put_stream_64(&SB3, interm3_id, 1);
		put_stream_64(&SB4, interm4_id, 1);
	    }
	}

	#pragma src section
	{
	    uint64_t opcode_ie, funct_ie, rs_ie, rt_ie, rd_ie, immediate_ie, shamt_ie; 
	    uint64_t ctl_mem_read_ie, ctl_mem_write_ie, ctl_reg_write_ie, ctl_reg_dst_ie, ctl_beq_ie, ctl_bne_ie;
	    uint64_t alu_out_ie, alu_zero_ie;
	    uint64_t interm1_ie, interm2_ie, interm3_ie, interm4_ie;
	    uint64_t interm5_ie, interm6_ie, interm7_ie, interm8_ie;
	    int iter;

	    for (iter = 0; iter < (program_size); iter++) {
		/* Get the input variables from previous pipeline stage */
		get_stream_64(&SB1, &interm1_ie);
		get_stream_64(&SB2, &interm2_ie);
		get_stream_64(&SB3, &interm3_ie);
		get_stream_64(&SB4, &interm4_ie);

		/* Unpack streamed data */
		opcode_ie	= (interm1_ie >> 32); 
		rs_ie     	= (interm1_ie & 0x00000000FFFFFFFF);
		rt_ie		= (interm2_ie >> 32); 
		rd_ie     	= (interm2_ie & 0x00000000FFFFFFFF);
		immediate_ie	= (interm3_ie >> 32); 
		shamt_ie     	= (interm3_ie & 0x00000000FFFFFFFF);
		funct_ie     	= (interm4_ie & 0x00000000FFFFFFFF);

		/* Generate the requisite signals for pipeline control path */
		generate_control_signals(opcode_ie, funct_ie, ctl_mem_read_ie, ctl_mem_write_ie, ctl_reg_write_ie, ctl_reg_dst_ie, ctl_beq_ie, ctl_bne_ie);

		/* Execute the instruction */
		instruction_execute(status_ie, opcode_ie, rs_ie, rt_ie, rd_ie, immediate_ie, shamt_ie, funct_ie, alu_out_ie, alu_zero_ie, BL);

		/* Take the branch decision in this pseudo stage */
		take_branch_decision(immediate_ie, ctl_beq_ie, ctl_bne_ie, alu_zero_ie, PC);

		/* Pack the data for the next stream */
		interm5_ie = (rt_ie << 32) | alu_out_ie;
		interm6_ie = (ctl_mem_read_ie << 32) | ctl_mem_write_ie;
		interm7_ie = (ctl_reg_write_ie << 32) | ctl_reg_dst_ie;
		interm8_ie = rd_ie;
		
		/* Populate the streams for next pipeline stage*/
		put_stream_64(&SC1, interm5_ie, 1);
		put_stream_64(&SC2, interm6_ie, 1);
		put_stream_64(&SC3, interm7_ie, 1);
		put_stream_64(&SC4, interm8_ie, 1);
	    }
	}

	#pragma src section
	{
	    uint64_t rt_mw, alu_out_mw, ctl_mem_read_mw, ctl_mem_write_mw, mem_out_mw;
	    uint64_t interm1_mw, interm2_mw;
	    uint64_t interm3_mw, interm4_mw;
	    uint iter;

	    for (iter = 0; iter < (program_size); iter++) {
		get_stream_64(&SC1, &interm1_mw);
		get_stream_64(&SC2, &interm2_mw);

		/* Unpack streamed data */
		rt_mw			= (interm1_mw >> 32);
		alu_out_mw		= (interm1_mw & 0x00000000FFFFFFFF);
		ctl_mem_read_mw		= (interm2_mw >> 32);
		ctl_mem_write_mw	= (interm2_mw & 0x00000000FFFFFFFF);

		/* Execute the memory write stage */
		memory_write(status_mw, ctl_mem_read_mw, ctl_mem_write_mw, BL[rt_mw], alu_out_mw, mem_out_mw, CL);
		
		/* Pack the data for streams */
		interm3_mw = (alu_out_mw << 32) | mem_out_mw;
		interm4_mw = (ctl_mem_read_mw << 32) | rt_mw;

		/* Populate the streams for next pipeline stage  */
		put_stream_64(&SD1, interm3_mw, 1);
		put_stream_64(&SD2, interm4_mw, 1);
	    }
	}	

	#pragma src section
	{
	    uint64_t ctl_mem_read_wb, ctl_reg_write_wb, ctl_reg_dst_wb, rt_wb, rd_wb, alu_out_wb, mem_out_wb;
	    uint64_t interm1_wb, interm2_wb, interm3_wb, interm4_wb;
	    int iter;

	    for (iter = 0; iter < (program_size); iter++) {
		get_stream_64(&SD1, &interm1_wb);
		get_stream_64(&SD2, &interm2_wb);
		get_stream_64(&SC3, &interm3_wb);
		get_stream_64(&SC4, &interm4_wb);

		/* Unpack streamed data */
		alu_out_wb 		= (interm1_wb >> 32);
		mem_out_wb		= (interm1_wb & 0x00000000FFFFFFFF);
		ctl_mem_read_wb		= (interm2_wb >> 32);
		rt_wb			= (interm2_wb & 0x00000000FFFFFFFF);
		ctl_reg_write_wb	= (interm3_wb >> 32);
		ctl_reg_dst_wb		= (interm3_wb & 0x00000000FFFFFFFF);
		rd_wb			= interm4_wb;
		
		/* Execute the write back stage */
		write_back(status_wb, ctl_mem_read_wb, ctl_reg_write_wb, ctl_reg_dst_wb, rt_wb, rd_wb, alu_out_wb, mem_out_wb, BL);
	    }
	}
    }

    /* Benchmark the end-time */
    read_timer (&t1);

    /* Transfer the architectural state and memory state back to main processor */
    buffered_dma_cpu (OBM2CM, PATH_0, BL, MAP_OBM_stripe (1, "B"), core_dump, 1, (MIPS_ARCH_REGS) * 8);
    buffered_dma_cpu (OBM2CM, PATH_0, CL, MAP_OBM_stripe (1, "C"), data_memory, 1, (MEM_DATA_SIZE) * 8);

    *time 	= t1 - t0;
    *pc_value	= PC;

    return;
}
