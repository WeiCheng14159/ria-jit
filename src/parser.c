//
// Created by noah on 29.04.20.
// Based on RISC-V-Spec.pdf ../documentation/RISC-V-Spec.pdf
// For register assembly conventions look at page 137
//

#include "util.h"
#include "parser.h"

#define N_OPCODE 32 //inst[6:2] 5 bit 2⁵ = 32

typedef void t_parse_result; //maybe some more return information later

// extract rd register number bit[11:7]
static inline int extract_rd(int32_t instr) { return instr >> 7 & 0b11111; }

// extract rs1 register number bit[19:15]
static inline int extract_rs1(int32_t instr) { return instr >> 15 & 0b11111; }

// extract rs2 register number bit[24:20]
static inline int extract_rs2(int32_t instr) { return instr >> 20 & 0b11111; }

// extract func3 bit [14:12]
static inline int extract_func3(int32_t instr) { return instr >> 12 & 0b111; }

// extract big_shamt bit[25:20]
static inline int extract_big_shamt(int32_t instr) { return instr >> 20 & 0b111111; }

// extract small_shamt bit[24:20] -> basically the same as rs2
static inline int extract_small_shamt(int32_t instr) { return instr >> 20 & 0b11111; }

// extract U-Type immediate bit[31:12] -> mask lower 12 bit [11:0] with zeros
static inline int extract_imm_U(int32_t instr) { return instr & ~(0xfff); }

// extract I-Type immediate bit[31:20]
static inline int extract_imm_I(int32_t instr) { return instr >> 20; } //sign extend!

// extract S-Type immediate bit[31:25] + [11:7] => 7bits + 5 bits
// TODO need sign extend?
static inline int extract_imm_S(int32_t instr) { return (instr >> 20 & 0b1111111) | (instr >> 7 & 0b11111); }

// extract J-Type immediate bits[31:12] order: [20|10:1|11|19:12]
// only for jumps so no sign extend?
// [20] => [31], [10:1] => [30:21], [11] => [20], [19:12] => [19:12]
static inline int extract_imm_J(int32_t instr) {
    return (instr & 0xff000) | (instr >> (20 - 11) & (1 << 11)) | (instr >> 11 & (1 << 20)) |
           (instr >> (30 - 10) & 0b11111111110);
}

// extract B-Type immediate bits[31:25],[11:7] order: [12|10:5],[4:1|11]
static inline int32_t extract_imm_B(int32_t instr) {
    return (instr >> (31 - 12) & 0xfffff000) | (instr << (11 - 7) & (1 << 11)) |
           (instr >> (30 - 10) & 0b11111100000) | (instr >> (11 - 4) & 0b11110);
}

//function prototypes DEPRECATED
//function prototypes DEPRECATED
t_parse_result parse_OP_IMM(int32_t* instruction);

t_parse_result parse_OP_IMM_32(int32_t* instruction);

t_parse_result parse_LUI(int32_t* instruction);

/* Deprecated! Usage of stdlib. Has since been moved to unit testing.
void test_parsing(void) {
     * dissassembly of li a0, 0xDEADBEEF
     *  00038537                lui     a0,0x38
     *  ab75051b                addiw   a0,a0,-1353
     *  00e51513                slli    a0,a0,0xe
     *  eef50513                addi    a0,a0,-273 # 37eef <__global_pointer$+0x26667>


    unsigned int *memory = (unsigned int *) malloc(0x10);
    //fill memory with the data we have little endian so 00038537 should be 37 85 38 00 in memory
    memory[0] = 0x38537;
    memory[1] = 0xab75051b;
    memory[2] = 0xe51513;
    memory[3] = 0xeef50513;

    log_verbose("First line of memory %#010x, first byte: %#x\n", memory[0], ((unsigned char *) memory)[0]);
    uint32_t data[32];
    for (int i = 0; i < 4; i++) {
        t_risc_instr instr;
        instr.addr = (uintptr_t)&memory[i];
        parse_instruction(&instr,data);
    }
}*/

/**
 *
 * @param p_instr_struct struct filled with the addr of the instruction to be translated
 */
void parse_instruction(t_risc_instr *p_instr_struct, uint32_t* reg_count) {
    //TODO verify all commands, clean up textual output, add float and multiprocessor memory opcodes?

    // print out the line to parse in grouped binary as in the spec
    int32_t raw_instr = *(int32_t*)p_instr_struct->addr; //cast and dereference
    log_verbose("Parsing: 0x%x\n", raw_instr);

    //fill basic struct
    p_instr_struct->reg_dest = extract_rd(raw_instr);
    p_instr_struct->reg_src_1 = extract_rs1(raw_instr);
    //p_instr_struct->reg_src_2 = extract_rs2(raw_instr); NOT REALLY NEEDED MOST OF TIME

    //extract opcode bits[6:2]
    t_opcodes opcode = raw_instr >> 2 & 0x1f;
    switch (opcode) {
        case OP_LUI:
            parse_LUI(&raw_instr);
            p_instr_struct->optype = UPPER_IMMEDIATE;
            p_instr_struct->mnem = LUI;
            p_instr_struct->imm = extract_imm_U(raw_instr);
            reg_count[p_instr_struct->reg_dest]++;
            break;
        case OP_AUIPC:
            p_instr_struct->optype = IMMEDIATE;
            p_instr_struct->mnem = AUIPC;
            p_instr_struct->imm = extract_imm_U(raw_instr);
            reg_count[p_instr_struct->reg_dest]++;
            break;
        case OP_JAL:
            p_instr_struct->optype = JUMP;
            p_instr_struct->mnem = JAL;
            p_instr_struct->imm = extract_imm_J(raw_instr);
            reg_count[p_instr_struct->reg_dest]++;
            break;
        case OP_JALR:
            p_instr_struct->optype = JUMP;
            p_instr_struct->mnem = JALR;
            p_instr_struct->imm = extract_imm_I(raw_instr);
            reg_count[p_instr_struct->reg_dest]++;
            reg_count[p_instr_struct->reg_src_1]++;
            break;
        case OP_MISC_MEM:
            p_instr_struct->optype = SYSTEM;
            p_instr_struct->imm = extract_imm_I(raw_instr);
            switch(extract_func3(raw_instr)){
                case 0:
                    p_instr_struct->mnem = FENCE;
                    break;
                case 1:
                    p_instr_struct->mnem = FENCE_I;
                    break;
                default:
                    not_yet_implemented("Invalid func3 for OP_MISC_MEM Opcode");
            }
            break;
        case OP_BRANCH:
            // BEQ, BNE...
            p_instr_struct->optype = BRANCH;
            p_instr_struct->reg_src_2 = extract_rs2(raw_instr);
            reg_count[p_instr_struct->reg_src_1]++;
            reg_count[p_instr_struct->reg_src_2]++;
            p_instr_struct->imm = extract_imm_B(raw_instr);
            switch (extract_func3(raw_instr)) {
                case 0:
                    p_instr_struct->mnem = BEQ;
                    break;
                case 1:
                    p_instr_struct->mnem = BNE;
                    break;
                case 4:
                    p_instr_struct->mnem = BLT;
                    break;
                case 5:
                    p_instr_struct->mnem = BGE;
                    break;
                case 6:
                    p_instr_struct->mnem = BLTU;
                    break;
                case 7:
                    p_instr_struct->mnem = BGEU;
                    break;
                default:
                    not_yet_implemented("Invalid func3 for BRANCH Opcode");
            }
            break;
        case OP_LOAD:
            p_instr_struct->optype = IMMEDIATE;
            p_instr_struct->imm = extract_imm_I(raw_instr);
            reg_count[p_instr_struct->reg_dest]++;
            reg_count[p_instr_struct->reg_src_1]++;
            switch (extract_func3(raw_instr)) {
                case 0: {
                    p_instr_struct->mnem = LB;
                    break;
                }
                case 1: {
                    p_instr_struct->mnem = LH;
                    break;
                }
                case 2: {
                    p_instr_struct->mnem = LW;
                    break;
                }
                case 3: {
                    p_instr_struct->mnem = LD;
                    break;
                }
                case 4: {
                    p_instr_struct->mnem = LBU;
                    break;
                }
                case 5: {
                    p_instr_struct->mnem = LHU;
                    break;
                }
                case 6: {
                    p_instr_struct->mnem = LWU;
                    break;
                }
                default: {
                    int error = extract_func3(raw_instr);
                    not_yet_implemented("Invalid LOAD Instruction");
                }
            }
            break;
        case OP_STORE:
            p_instr_struct->optype = STORE;
            p_instr_struct->imm = extract_imm_S(raw_instr);
            p_instr_struct->reg_src_2 = extract_rs2(raw_instr);
            reg_count[p_instr_struct->reg_src_1]++;
            reg_count[p_instr_struct->reg_src_2]++;
            switch (extract_func3(raw_instr)) {
                case 0:
                    p_instr_struct->mnem = SB;
                    break;
                case 1:
                    p_instr_struct->mnem = SH;
                    break;
                case 2:
                    p_instr_struct->mnem = SW;
                    break;
                case 3:
                    p_instr_struct->mnem = SD;
                    break;
                default:
                    not_yet_implemented("Invalid OP_STORE Instruction");
            }
            break;
        case OP_OP:
            p_instr_struct->optype = REG_REG;
            p_instr_struct->reg_src_2 = extract_rs2(raw_instr);
            reg_count[p_instr_struct->reg_dest]++;
            reg_count[p_instr_struct->reg_src_1]++;
            reg_count[p_instr_struct->reg_src_2]++;
            if(raw_instr & (1<<25)) {
                switch (extract_func3(raw_instr)) {
                    case 0:
                        p_instr_struct->mnem = MUL;
                        break;
                    case 1:
                        p_instr_struct->mnem = MULH;
                        break;
                    case 2:
                        p_instr_struct->mnem = MULHSU;
                        break;
                    case 3:
                        p_instr_struct->mnem = MULHU;
                        break;
                    case 4:
                        p_instr_struct->mnem = DIV;
                        break;
                    case 5:
                        p_instr_struct->mnem = DIVU;
                        break;
                    case 6:
                        p_instr_struct->mnem = REM;
                        break;
                    case 7:
                        p_instr_struct->mnem = REMU;
                        break;
                }
            } else {
                switch (extract_func3(raw_instr)) {
                    case 0:
                        if (raw_instr & (1 << 30)) {
                            //SRAI
                            p_instr_struct->mnem = SUB;
                        } else {
                            //SRLI
                            p_instr_struct->mnem = ADD;
                        }
                        break;
                    case 1:
                        p_instr_struct->mnem = SLL;
                        break;
                    case 2:
                        p_instr_struct->mnem = SLT;
                        break;
                    case 3:
                        p_instr_struct->mnem = SLTU;
                        break;
                    case 4:
                        p_instr_struct->mnem = XOR;
                        break;
                    case 5:
                        if (raw_instr & (1 << 30)) {
                            //SRAI
                            p_instr_struct->mnem = SRA;
                        } else {
                            //SRLI
                            p_instr_struct->mnem = SRL;
                        }
                        break;
                    case 6:
                        p_instr_struct->mnem = OR;
                        break;
                    case 7:
                        p_instr_struct->mnem = AND;
                        break;
                }
            }
            break;
        case OP_SYSTEM:
            p_instr_struct->optype = SYSTEM;
            p_instr_struct->imm = extract_imm_I(raw_instr);
            reg_count[p_instr_struct->reg_dest]++;
            switch(extract_func3(raw_instr)){
                case 0:
                    if(raw_instr & (1<<20)){
                        p_instr_struct->mnem = ECALL;
                    } else {
                        p_instr_struct->mnem = EBREAK;
                    }
                    break;
                case 1:
                    p_instr_struct->mnem = CSRRW;
                    reg_count[p_instr_struct->reg_src_1]++;
                    break;
                case 2:
                    p_instr_struct->mnem = CSRRS;
                    reg_count[p_instr_struct->reg_src_1]++;
                    break;
                case 3:
                    p_instr_struct->mnem = CSRRC;
                    reg_count[p_instr_struct->reg_src_1]++;
                    break;
                case 5:
                    p_instr_struct->mnem = CSRRWI;
                    break;
                case 6:
                    p_instr_struct->mnem = CSRRSI;
                    break;
                case 7:
                    p_instr_struct->mnem = CSRRCI;
                    break;
                default:
                    not_yet_implemented("Invalid CSSR Instruction");
            }
            break;
        case OP_OP_IMM_32:
            parse_OP_IMM_32(&raw_instr);
            p_instr_struct->optype = IMMEDIATE;
            reg_count[p_instr_struct->reg_dest]++;
            reg_count[p_instr_struct->reg_src_1]++;
            switch (extract_func3(raw_instr)) {
                case 0:
                    p_instr_struct->mnem = ADDIW;
                    p_instr_struct->imm = extract_imm_I(raw_instr);
                    break;
                case 1: //SLLIW opcode and func3 are unique
                    p_instr_struct->mnem = SLLIW;
                    p_instr_struct->imm = extract_small_shamt(raw_instr);
                    break;
                case 5: //SRAIW / SRLIW
                    p_instr_struct->imm = extract_small_shamt(raw_instr);
                    if (raw_instr & (1 << 30)) {
                        //SRAI
                        p_instr_struct->mnem = SRAIW;
                    } else {
                        //SRLI
                        p_instr_struct->mnem = SRLIW;
                    }
                default:
                    not_yet_implemented("Invalid OP_IMM_32 Instruction");
            }
            break;
        case OP_OP_32:
            p_instr_struct->optype = REG_REG;
            p_instr_struct->reg_src_2 = extract_rs2(raw_instr);
            reg_count[p_instr_struct->reg_dest]++;
            reg_count[p_instr_struct->reg_src_1]++;
            reg_count[p_instr_struct->reg_src_2]++;
            if(raw_instr & (1<<25)){
                switch (extract_func3(raw_instr)) {
                    case 0:
                        p_instr_struct->mnem = MULW;
                        break;
                    case 4:
                        p_instr_struct->mnem = DIVW;
                        break;
                    case 5:
                        p_instr_struct->mnem = DIVUW;
                        break;
                    case 6:
                        p_instr_struct->mnem = REMW;
                        break;
                    case 7:
                        p_instr_struct->mnem = REMUW;
                        break;
                    default:
                        not_yet_implemented("Invalid OP_32 RV64M Instruction");
                }
            } else {
                switch (extract_func3(raw_instr)) {
                    case 0:
                        if (raw_instr & (1 << 30)) {
                            //SRAI
                            p_instr_struct->mnem = SUBW;
                        } else {
                            //SRLI
                            p_instr_struct->mnem = ADDW;
                        }
                        break;
                    case 1:
                        p_instr_struct->mnem = SLLW;
                        break;
                    case 5:
                        if (raw_instr & (1 << 30)) {
                            //SRAI
                            p_instr_struct->mnem = SRAW;
                        } else {
                            //SRLI
                            p_instr_struct->mnem = SRLW;
                        }
                        break;
                    default:
                        not_yet_implemented("Invalid OP_32 Instruction");
                }
            }
            break;
        case OP_OP_IMM:
            parse_OP_IMM(&raw_instr);
            p_instr_struct->optype = IMMEDIATE;
            reg_count[p_instr_struct->reg_dest]++;
            reg_count[p_instr_struct->reg_src_1]++;
            switch (extract_func3(raw_instr)) {
                case 0: //ADDI
                    p_instr_struct->mnem = ADDI;
                    p_instr_struct->imm = extract_imm_I(raw_instr);
                    break;
                case 1: //SLLI opcode and func3 are unique
                    p_instr_struct->mnem = SLLI;
                    p_instr_struct->imm = extract_big_shamt(raw_instr);
                    break;
                case 2: //SLTI
                    p_instr_struct->mnem = SLTI;
                    p_instr_struct->imm = extract_imm_I(raw_instr);
                    break;
                case 3: //SLTIU
                    p_instr_struct->mnem = SLTIU;
                    p_instr_struct->imm = extract_imm_I(raw_instr);
                    break;
                case 4:
                    p_instr_struct->mnem = XORI;
                    p_instr_struct->imm = extract_imm_I(raw_instr);
                    break;
                case 5: //SRAI / SRLI
                    p_instr_struct->imm = extract_big_shamt(raw_instr);
                    if (raw_instr & (1 << 30)) {
                        //SRAI
                        p_instr_struct->mnem = SRAI;
                    } else {
                        //SRLI
                        p_instr_struct->mnem = SRLI;
                    }
                    break;
                case 6:
                    p_instr_struct->mnem = XORI;
                    p_instr_struct->imm = extract_imm_I(raw_instr);
                    break;
                case 7:
                    p_instr_struct->mnem = ANDI;
                    p_instr_struct->imm = extract_imm_I(raw_instr);
                    break;
                default: {
                    not_yet_implemented("Invalid OP_IMM Instruction");
                }
            }
            break;
        default:
            not_yet_implemented("Instruction unknown");
    }
}

t_parse_result parse_LUI(int32_t* instruction) {
    //extract imm [31:12] 20 bits
    // fills rd with the upper 20 bits and fills lower 12 bits with zeros
    log_verbose("LUI rd: %d, imm32: %#010x\n", extract_rd(*instruction), extract_imm_U(*instruction));
}

t_parse_result parse_OP_IMM(int32_t* instruction) {
    switch (extract_func3(*instruction)) {
        case 0: { //ADDI
            log_verbose("ADDI rd %d, rs1 %d, imm %d\n", extract_rd(*instruction), extract_rs1(*instruction),
                   extract_imm_I(*instruction));
            break;
        }
        case 1: { //SLLI opcode and func3 are unique
            log_verbose("SLLI rd %d, rs1 %d, shamt %d\n", extract_rd(*instruction), extract_rs1(*instruction),
                   extract_big_shamt(*instruction));
            break;
        }
        default: {
            not_yet_implemented("Instruction unknown");
        }
    }
}

t_parse_result parse_OP_IMM_32(int32_t* instruction) {
    // extract func 3 bits[14:12]
    switch (extract_func3(*instruction)) {
        case 0: {
            //extract imm bits[31:20] 12 bits we need sign extension!
            log_verbose("ADDIW rd: %d, rs1 %d, imm %d\n", extract_rd(*instruction), extract_rs1(*instruction),
                   extract_imm_I(*instruction));
            break;
        }
        default: {
            not_yet_implemented("Instruction unknown");
            break;
        }
    }
}
