#ifndef HELPER_PROC_H
#define HELPER_PROC_H

#include "mips_arch.h"
#include "decode.h"

#define MP_TRUE		1
#define MP_FALSE	0

#define instruction_fetch(status, instr, pc, inst_mem)								\
	do {													\
		mem_read_text(pc, inst_mem, instr);								\
		/* Advance the PC to next sequential value */							\
		pc += 4;											\
		status = 0;											\
	} while (0);

#define instruction_decode(status, inst, opc, rs, rt, rd, immd, j, sh, func, pc)				\
	do {													\
		opc 	= GET_OPCODE(inst);									\
		rs  	= GET_RS(inst);										\
		rt  	= GET_RT(inst);										\
		rd  	= GET_RD(inst);										\
		immd	= (int16_t)GET_IMMD(inst);								\
		j	= GET_JUMP(inst);									\
		sh	= GET_SHAMT(inst);									\
		func	= GET_FUNCT(inst);									\
														\
		/* Jump decision can be taken in the decode stage */						\
		if (opc == OP_JUMP || opc == OP_JAL) {								\
			if (opc == OP_JAL) {									\
				BL[31] = pc + 4;								\
			}											\
			pc = ((pc & 0xF0000000) | (j << 2)) + MEM_TEXT_START;					\
		}												\
														\
		status = 1;											\
	} while (0);

#define generate_control_signals(opcode, funct, mem_read, mem_write, reg_write, reg_dst, beq, bne, jump)	\
	do {													\
		switch (opcode)											\
		{												\
			case OP_RTYPE:										\
				mem_read 	= MP_FALSE;							\
				mem_write	= MP_FALSE;							\
				reg_write	= MP_TRUE;							\
				reg_dst		= MP_TRUE;							\
				beq		= MP_FALSE;							\
				bne		= MP_FALSE;							\
				jump		= MP_FALSE;							\
				break;										\
														\
			case OP_ADDI:										\
			case OP_ADDIU:										\
			case OP_SLTI:										\
			case OP_ANDI:										\
			case OP_ORI:										\
			case OP_LUI:										\
				mem_read	= MP_FALSE;							\
				mem_write	= MP_FALSE;							\
				reg_write	= MP_TRUE;							\
				reg_dst		= MP_FALSE;							\
				beq		= MP_FALSE;							\
				bne		= MP_FALSE;							\
				jump		= MP_FALSE;							\
				break;										\
														\
			case OP_LW:										\
				mem_read	= MP_TRUE;							\
				mem_write	= MP_FALSE;							\
				reg_write	= MP_TRUE;							\
				reg_dst		= MP_FALSE;							\
				beq		= MP_FALSE;							\
				bne		= MP_FALSE;							\
				jump		= MP_FALSE;							\
				break;										\
														\
			case OP_SW:										\
				mem_read 	= MP_FALSE;							\
				mem_write	= MP_TRUE;							\
				reg_write	= MP_FALSE;							\
				reg_dst		= MP_FALSE;							\
				beq		= MP_FALSE;							\
				bne		= MP_FALSE;							\
				jump		= MP_FALSE;							\
				break;										\
														\
			case OP_BEQ:										\
				mem_read	= MP_FALSE;							\
				mem_write	= MP_FALSE;							\
				reg_write	= MP_FALSE;							\
				reg_dst		= MP_FALSE;							\
				beq		= MP_TRUE;							\
				bne		= MP_FALSE;							\
				jump		= MP_FALSE;							\
				break;										\
														\
			case OP_BNE:										\
				mem_read	= MP_FALSE;							\
				mem_write	= MP_FALSE;							\
				reg_write	= MP_FALSE;							\
				reg_dst		= MP_FALSE;							\
				beq		= MP_FALSE;							\
				bne		= MP_TRUE;							\
				jump		= MP_FALSE;							\
				break;										\
														\
			case OP_JUMP:										\
			case OP_JAL:										\
				mem_read	= MP_FALSE;							\
				mem_write	= MP_FALSE;							\
				reg_write	= MP_FALSE;							\
				reg_dst		= MP_FALSE;							\
				beq		= MP_FALSE;							\
				bne		= MP_FALSE;							\
				jump		= MP_TRUE;							\
				break;										\
														\
			default:										\
				break;										\
		}												\
	} while (0);												

#define instruction_execute(status, opc, rs, rt, rd, immd, sh, func, aluOut, zerOut, REGS)			\
	do {													\
		switch (opc)											\
		{												\
			case OP_ADDIU:										\
			case OP_ADDI:										\
				aluOut = REGS[rs] + immd;							\
				break;										\
														\
			case OP_ANDI:										\
				aluOut = REGS[rs] & immd;							\
				break;										\
														\
			case OP_LUI:										\
				aluOut = immd << 16;								\
				break;										\
														\
			case OP_ORI:										\
				aluOut = REGS[rs] | immd;							\
				break;										\
														\
			case OP_SLTI:										\
				aluOut = (REGS[rs] < immd)? 1 : 0;						\
				break;										\
														\
			case OP_LW:										\
			case OP_SW:										\
				aluOut = REGS[rs] + immd;							\
				break;										\
														\
			case OP_BEQ:										\
			case OP_BNE:										\
				zerOut= ((REGS[rs] - REGS[rt]) == 0)? 1 : 0;					\
				break;										\
														\
			case OP_RTYPE:										\
				switch (func)									\
				{										\
					case FUNCT_ADD:								\
					case FUNCT_ADDU:							\
						aluOut = REGS[rs] + REGS[rt];					\
						break;								\
														\
					case FUNCT_SUB:								\
					case FUNCT_SUBU:							\
						aluOut = REGS[rs] - REGS[rt];					\
						break;								\
														\
					case FUNCT_AND:								\
						aluOut = REGS[rs] & REGS[rt];					\
						break;								\
														\
					case FUNCT_OR:								\
						aluOut = REGS[rs] | REGS[rt];					\
						break;								\
														\
					case FUNCT_SLL:								\
						aluOut = REGS[rt] << sh;					\
						break;								\
														\
					case FUNCT_SRL:								\
						aluOut = REGS[rt] >> sh;					\
						break;								\
														\
					case FUNCT_SLT:								\
						aluOut = (REGS[rs] < REGS[rt])? 1 : 0;				\
						break;								\
														\
					case FUNCT_NOR:								\
						aluOut = ~(REGS[rs] | REGS[rt]);				\
						break;								\
														\
					default:								\
						break;								\
				}										\
			default:										\
				break;										\
		}												\
														\
		status = 2;											\
	} while (0);

#define take_branch_decision(offset, beq, bne, zero, pc)							\
	do {													\
		if (beq == MP_TRUE && zero == MP_TRUE) {							\
			pc += (offset << 2);									\
		} else if (bne == MP_TRUE && zero == MP_FALSE) {						\
			pc += (offset << 2);									\
		}												\
	} while (0);
		

#define memory_write(status, mem_read, mem_write, rt_val, alu_out, mem_out, memory)				\
	do {													\
		/* Write to or read from the memory based on control signals*/					\
		if (mem_write) {										\
			mem_write_data(alu_out, memory, rt_val); 						\
		} else if (mem_read) {										\
			mem_read_data(alu_out, memory, mem_out); 						\
		}												\
														\
		status = 3;											\
	} while (0);

#define write_back(status, mem_read, reg_write, reg_dst, rt, rd, alu_out, mem_out, REGS)			\
	do {													\
		/* Write to the register file based on the control signals */					\
		if (reg_write) {										\
			if (mem_read) {										\
				if (reg_dst) {									\
					REGS[rd] = mem_out;							\
				} else {									\
					REGS[rt] = mem_out;							\
				}										\
			} else {										\
				if (reg_dst) {									\
					REGS[rd] = alu_out;							\
				} else {									\
					REGS[rt] = alu_out;							\
				}										\
			}											\
		}												\
														\
		status = 4;											\
	} while (0);


#endif /* HELPER_PROC_H */
