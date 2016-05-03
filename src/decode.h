#ifndef DECODE_H
#define DECODE_H

/* Instruction Field Decoding */
#define OPCODE_OFFSET			26
#define OPCODE_MASK			0x3F
#define GET_OPCODE(instr)		((instr >> OPCODE_OFFSET) & OPCODE_MASK)

#define RS_OFFSET			21
#define RS_MASK				0x1F
#define GET_RS(instr)			((instr >> RS_OFFSET) & RS_MASK)

#define RT_OFFSET			16
#define RT_MASK				0x1F
#define GET_RT(instr)			((instr >> RT_OFFSET) & RT_MASK)

#define RD_OFFSET			11
#define RD_MASK				0x1F
#define GET_RD(instr)			((instr >> RD_OFFSET) & RD_MASK)

#define SHAMT_OFFSET			6
#define SHAMT_MASK			0x1F
#define GET_SHAMT(instr)		((instr >> SHAMT_OFFSET) & SHAMT_MASK)

#define FUNCT_MASK			0x3F
#define GET_FUNCT(instr)		(instr & FUNCT_MASK)

#define IMMD_MASK			0xFFFF
#define GET_IMMD(instr)			(instr & IMMD_MASK)

#define JUMP_MASK			0x3FFFFFF
#define GET_JUMP(instr)			(instr & JUMP_MASK)

/* Opcode Encoding */
#define OP_RTYPE			0x00
#define OP_JUMP				0x02
#define OP_JAL				0x03
#define OP_BEQ				0x04
#define OP_BNE				0x05
#define OP_ADDI				0x08
#define OP_ADDIU			0x09
#define OP_SLTI				0x0A
#define OP_SLTIU			0x0B
#define OP_ANDI				0x0C
#define OP_ORI				0x0D
#define OP_XORI				0x0E
#define OP_LUI				0x0F
#define OP_LW				0x23	/* Not Implemented */
#define OP_SW				0x2B	/* Not Implemented */

/* Function Field Encoding */
#define FUNCT_SLL           	        0x00
#define FUNCT_SRL              	     	0x02
#define FUNCT_SRA              	     	0x03
#define FUNCT_JR                    	0x08    /* Not Implemented */
#define FUNCT_JALR                  	0x09    /* Not Implemented */
#define FUNCT_SYSCALL               	0x0C
#define FUNCT_ADD                   	0x20
#define FUNCT_ADDU                  	0x21
#define FUNCT_SUB                   	0x22
#define FUNCT_SUBU                  	0x23
#define FUNCT_AND                   	0x24
#define FUNCT_OR                    	0x25
#define FUNCT_XOR                   	0x26
#define FUNCT_NOR                   	0x27
#define FUNCT_SLT                   	0x2A
#define FUNCT_SLTU                  	0x2B

#endif /* DECODE_H */

