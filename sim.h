#ifndef __SIM_H__
#define __SIM_H__

/*
 * ==========================================================================
 * Global Definitions
 * ==========================================================================
 */

#define SIM_DEBUG(x)                x   

#define SUCCESS                     0
#define FAILURE                     1

#define CLK_LO                      0
#define CLK_HI                      1

/* R-Type, I-Type, J-Type */ 
#define OPCODE_OFFSET			    26
#define OPCODE_MASK                 0xFC000000
#define GET_INST_OPCODE(instr)		((instr & OPCODE_MASK) >> OPCODE_OFFSET)

/* R-Type, I-Type */
#define RS_OFFSET                   21
#define RS_MASK                     0x03E00000
#define GET_INST_RS(instr)          ((instr & RS_MASK) >> RS_OFFSET)

#define RT_OFFSET                   16
#define RT_MASK                     0x001F0000
#define GET_INST_RT(instr)          ((instr & RT_MASK) >> RT_OFFSET)

/* R-Type */
#define RD_OFFSET                   11
#define RD_MASK                     0x0000F800
#define GET_INST_RD(instr)          ((instr & RD_MASK) >> RD_OFFSET)

#define SHAMT_OFFSET                6
#define SHAMT_MASK                  0x000007C0
#define GET_INST_SHAMT(instr)       ((instr & SHAMT_MASK) >> SHAMT_OFFSET)

#define FUNCT_OFFSET                0
#define FUNCT_MASK                  0x0000003F
#define GET_INST_FUNCT(instr)       ((instr & FUNCT_MASK) >> FUNCT_OFFSET)

/* I-Type */
#define IMMD_OFFSET                 0
#define IMMD_MASK                   0x0000FFFF
#define GET_INST_IMMD(instr)        ((instr & IMMD_MASK) >> IMMD_OFFSET)

/* J-Type */
#define JUMP_OFFSET                 0
#define JUMP_MASK                   0x03FFFFFFF
#define GET_INST_JUMP(instr)        ((instr & JUMP_MASK) >> JUMP_OFFSET)

/* Opcode Encoding */
#define OP_RTYPE                    0x00
#define OP_JUMP                     0x02
#define OP_JAL                      0x03
#define OP_BRANCH                   0x04
#define OP_BNE                      0X05    /* Not Implemented */
#define OP_ADDI                     0x08
#define OP_ADDIU                    0x09
#define OP_SLTI                     0x0A
#define OP_SLTIU                    0x0B
#define OP_ANDI                     0x0C
#define OP_ORI                      0x0D
#define OP_XORI                     0x0E
#define OP_LUI                      0x0F
#define OP_LW                       0x23
#define OP_SW                       0x28

/* Function Field Encoding */
#define FUNCT_SLL                   0x00
#define FUNCT_SRL                   0x02
#define FUNCT_SRA                   0x03
#define FUNCT_JR                    0x08    /* Not Implemented */
#define FUNCT_JALR                  0x09    /* Not Implemented */
#define FUNCT_SYSCALL               0x0C
#define FUNCT_ADD                   0x20
#define FUNCT_ADDU                  0x21
#define FUNCT_SUB                   0x22
#define FUNCT_SUBU                  0x23
#define FUNCT_AND                   0x24
#define FUNCT_OR                    0x25
#define FUNCT_XOR                   0x26
#define FUNCT_NOR                   0x27
#define FUNCT_SLT                   0x2A
#define FUNCT_SLTU                  0x2B

/* ALU control signals */
#define ALU_AND                     0x0
#define ALU_OR                      0x1
#define ALU_ADD                     0x2
#define ALU_SUB                     0x6
#define ALU_SLT                     0x7
#define ALU_SLL                     0x8     /* Custom */
#define ALU_SRL                     0x9     /* Custom */
#define ALU_SRA                     0xA     /* Custom */
#define ALU_XOR                     0xB     /* Custom */
#define ALU_NOR                     0xC     /* Custom */

/*
 * ==========================================================================
 * Data Types
 * ==========================================================================
 */

struct sign_extend {
    int16_t    input;
    int32_t    output;
};

struct multiplexor {
    char        name[64];
    uint32_t    input_0;
    uint32_t    input_1;
    uint32_t    output;
    uint8_t     control_signal;
};

struct register_file {
    uint8_t     read_reg_1;
    uint8_t     read_reg_2;
    uint8_t     write_reg;
    uint32_t    write_data;
    uint32_t    read_data_1;
    uint32_t    read_data_2;
    uint8_t     ctl_reg_write;
}; 

struct control_unit {
    uint8_t     opcode;
    uint8_t     funct;
    uint8_t     reg_dst;
    uint8_t     alu_src_1;
    uint8_t     alu_src_2;
    uint8_t     mem_to_reg;
    uint8_t     reg_write;
    uint8_t     mem_read;
    uint8_t     mem_write;
    uint8_t     branch;
    uint8_t     jump;
};

struct alu_control {
    uint8_t     opcode;
    uint8_t     funct;
    uint8_t     alu_ctl_in;
};

struct alu {
    int32_t     input_1;
    int32_t     input_2;
    uint8_t     alu_ctl;
    uint32_t    output;
    uint8_t     zero;
};

struct memory {
    uint32_t    address;
    uint32_t    write_data;
    uint8_t     ctl_mem_read;
    uint8_t     ctl_mem_write;
    uint32_t    output;
};

/*
 * ==========================================================================
 * Global Data
 * ==========================================================================
 */

/* General Purpose wire for don't care connections */
static uint8_t  w_ground;

static uint32_t instruction;
static uint8_t  opcode;
static uint8_t  rs, rt, rd;
static uint8_t  shamt;
static uint8_t  funct;
static uint16_t immediate;
static uint32_t jump;

/* Wire out of pc-adder */
static uint32_t w_pc_adder;

/* Wires out of control-unit */
static uint8_t  w_ctl_reg_dst;
static uint8_t  w_ctl_alu_src_1;
static uint8_t  w_ctl_alu_src_2;
static uint8_t  w_ctl_mem_to_reg;
static uint8_t  w_ctl_reg_write;
static uint8_t  w_ctl_mem_read;
static uint8_t  w_ctl_mem_write;
static uint8_t  w_ctl_branch;
static uint8_t  w_ctl_jump;

/* Wire out of jump-shifter */
static uint32_t w_jump_shift;

/* Wire out of immd-shifter */
static uint32_t w_immd_shift;

/* Wire out of branch adder */
static uint32_t w_branch_adder;

/* Wire out of branch control alu */
static uint32_t w_branch_control;

/* Wires out of different multiplexors in the processor */
static uint32_t w_mux_reg_file_in;
static uint32_t w_mux_alu_in_1;
static uint32_t w_mux_alu_in_2;
static uint32_t w_mux_beq;
static uint32_t w_mux_next_pc;

/* Wires out of register file */
static uint32_t w_reg_file_data_1;
static uint32_t w_reg_file_data_2;

/* Wire input of register file */
static uint32_t w_mux_reg_file_write_data;

/* Wire out of alu-control unit which determines alu-operation */
static uint8_t  w_alu_ctl_in;

/* Wires out of alu */
static uint32_t w_alu_out_1;
static uint8_t  w_zero;

/* Wire out of memory */
static uint32_t w_mem_out;

static struct sign_extend   extend_immd;
static struct control_unit  ctl_unit;
static struct register_file reg_file;
static struct alu_control   alu_ctl;
static struct alu           alu;
static struct alu           pc_adder;
static struct alu           jump_shift;
static struct alu           immd_shift;
static struct alu           branch_adder;
static struct alu           branch_control;
static struct memory        memory;

/* Wire out of sign-extension unit */
static uint32_t w_signext_immd;

static struct multiplexor   mux_reg_file_in = 
    { 
        .name   = "mux_reg_file_in",
    };

static struct multiplexor   mux_alu_in_1 =
    {
        .name   = "mux_alu_in_1",
    };

static struct multiplexor   mux_alu_in_2 =
    {
        .name   = "mux_alu_in_2",
    };

static struct multiplexor   mux_reg_file_data = 
    {
        .name   = "mux_reg_file_data",
    };

static struct multiplexor   mux_beq =
    {
        .name   = "mux_beq",
    };

static struct multiplexor   mux_next_pc =
    {
        .name   = "mux_next_pc",
    };

/*
 * ==========================================================================
 * Funtion Prototypes
 * ==========================================================================
 */
uint8_t instruction_fetch(void);
uint8_t instruction_decode(void);
uint8_t instruction_execute(void);
uint8_t memory_write(void);
uint8_t write_back(void);

/* Helper Functions */
uint8_t generate_control_signals(void);
uint8_t multiplexor_process(struct multiplexor *mux, uint32_t *w_mux_out);
uint8_t reg_file_process(uint8_t clock);
void    sign_extend_process(void);
uint8_t alu_control_process(struct alu_control *alu_ctl, uint8_t *w_alu_ctl_in);
uint8_t alu_process(struct alu *alu, uint32_t *w_alu_out_1, uint8_t *w_zero);
uint8_t memory_process(struct memory *mem, uint32_t *w_mem_out);

/* Debug Functions */
void    control_debug(void);

#endif /* __SIM_H__ */
