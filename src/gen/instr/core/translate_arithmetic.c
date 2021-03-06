//
// Created by flo on 21.05.20.
//

#include "translate_arithmetic.h"
#include <util/util.h>
#include <fadec/fadec-enc.h>

/**
 * ADDIW adds the sign-extended 12-bit immediate to register rs1 and produces the
 * proper sign-extension of a 32-bit result in rd.
 * @param instr
 */
void translate_ADDIW(const t_risc_instr *instr, const register_info *r_info) {
    // mov rd, rs1
    // add rd, instr->imm
    log_asm_out("Translate ADDIW...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        err |= fe_enc64(&current, FE_LEA32rm, regDest, FE_MEM(regSrc1, 0, 0, instr->imm));
    } else if (instr->imm != 0) {
        err |= fe_enc64(&current, FE_ADD32ri, regDest, instr->imm);
    }
    err |= fe_enc64(&current, FE_MOVSXr64r32, regDest, regDest);
}

/**
 * SLLI is a logical left shift.
 * The operand to be shifted is in rs1, the shift amount is in the lower 6 bits of the I-immediate field.
 * @param instr
 */
void translate_SLLI(const t_risc_instr *instr, const register_info *r_info) {
    //mov rd, rs1
    //shl rd, (instr->imm & 0x3F)
    log_asm_out("Translate SLLI...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    t_risc_imm masked_imm = instr->imm & 0x3f;
    if (regDest != regSrc1 && (masked_imm == 1 || masked_imm == 2 || masked_imm == 3)) {
        err |= fe_enc64(&current, FE_LEA64rm, regDest, FE_MEM(0, 1 << masked_imm, regSrc1, 0));
    } else {
        if (regDest != regSrc1) {
            err |= fe_enc64(&current, FE_MOV64rr, regDest, regSrc1);
        }

        err |= fe_enc64(&current, FE_SHL64ri, regDest, masked_imm);
    }
}

/**
 * LUI places the 20-bit U-immediate into bits 31-12 of register rd and places zero in the lowest 12 bits.
 * The 32-bit result is sign-extended to 64 bits.
 * @param instr
 */
void translate_LUI(const t_risc_instr *instr, const register_info *r_info) {
    //mov rd, extended
    log_asm_out("Translate LUI...\n");

    FeReg regDest = getRd(instr, r_info);

    err |= fe_enc64(&current, FE_MOV64ri, regDest, instr->imm);
}

/**
 * ADDI adds the sign-extended 12-bit immediate to register rs1.
 * Overflow is ignored and the result is the low (in our case) 64 bit of the result.
 * The result is stored in rd.
 * @param instr
 */
void translate_ADDI(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate ADDI...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        err |= fe_enc64(&current, FE_LEA64rm, regDest, FE_MEM(regSrc1, 0, 0, instr->imm));
    } else if (instr->imm != 0) {
        err |= fe_enc64(&current, FE_ADD64ri, regDest, instr->imm);
    }
}

/**
* Translate the AUIPC instruction.
* AUIPC (add upper immediate to pc) is used to build pc-relative addresses and uses the U-type
* format. AUIPC forms a 32-bit offset from the 20-bit U-immediate, filling in the lowest 12 bits with
* zeros, adds this offset to the address of the AUIPC instruction, then places the result in register
* rd.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_AUIPC(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate AUIPC...\n");
    if (instr->reg_dest != x0) {
        FeReg regDest = getRd(instr, r_info);

        err |= fe_enc64(&current, FE_MOV64ri, regDest, instr->addr + instr->imm);
    }
}

/**
* Translate the SLTI instruction.
* SLTI (set less than immediate) places the value 1 in register rd if register rs1 is less than the sign-
* extended 12-bit immediate when both are treated as signed numbers, else 0 is written to rd.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SLTI(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SLTI...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    err |= fe_enc64(&current, FE_CMP64ri, regSrc1, instr->imm);
    err |= fe_enc64(&current, FE_SETL8r, regDest);
    err |= fe_enc64(&current, FE_MOVZXr32r8, regDest, regDest);
}

/**
* Translate the SLTIU instruction.
* SLTIU (set less than immediate unsigned) places the value 1 in register rd if register rs1 is less than the sign-
* extended 12-bit immediate when both are treated as unsigned numbers, else 0 is written to rd.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SLTIU(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SLTIU...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    err |= fe_enc64(&current, FE_CMP64ri, regSrc1, instr->imm);
    err |= fe_enc64(&current, FE_SETC8r, regDest);
    err |= fe_enc64(&current, FE_MOVZXr32r8, regDest, regDest);
}

/**
* Translate the XORI instruction.
* Performs a bitwise XOR on the register rs1 and the sign-extended 12-bit immediate
* and places the result into rd.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_XORI(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate XORI...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        err |= fe_enc64(&current, FE_MOV64rr, regDest, regSrc1);
    }

    err |= fe_enc64(&current, FE_XOR64ri, regDest, instr->imm);
}

/**
* Translate the ORI instruction.
* Performs a bitwise OR on the register rs1 and the sign-extended 12-bit immediate
* and places the result into rd.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_ORI(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate ORI...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        err |= fe_enc64(&current, FE_MOV64rr, regDest, regSrc1);
    }

    err |= fe_enc64(&current, FE_OR64ri, regDest, instr->imm);
}

/**
* Translate the ANDI instruction.
* Performs a bitwise AND on the register rs1 and the sign-extended 12-bit immediate
* and places the result into rd.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_ANDI(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate ANDI...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if ((uint32_t) instr->imm == (uint64_t) instr->imm) {
        if (regDest != regSrc1) {
            err |= fe_enc64(&current, FE_MOV32rr, regDest, regSrc1);
        }

        err |= fe_enc64(&current, FE_AND32ri, regDest, instr->imm);
    } else {
        if (regDest != regSrc1) {
            err |= fe_enc64(&current, FE_MOV64rr, regDest, regSrc1);
        }

        err |= fe_enc64(&current, FE_AND64ri, regDest, instr->imm);
    }

}

/**
* Translate the SRLI instruction. SRLI is a logical right shift.
* The operand to be shifted is in rs1, the shift amount is in the lower 6 bits of the I-immediate field.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SRLI(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SRLI...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        err |= fe_enc64(&current, FE_MOV64rr, regDest, regSrc1);
    }

    err |= fe_enc64(&current, FE_SHR64ri, regDest, instr->imm & 0x3f);
}

/**
* Translate the SRAI instruction. SRAI is an arithmetic right shift.
* The operand to be shifted is in rs1, the shift amount is in the lower 6 bits of the I-immediate field.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SRAI(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SRAI...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        err |= fe_enc64(&current, FE_MOV64rr, regDest, regSrc1);
    }

    err |= fe_enc64(&current, FE_SAR64ri, regDest, instr->imm & 0x3f);
}

/**
* Translate the ADD instruction.
* ADD performs the addition of rs1 and rs2. Overflows
* are ignored and the low XLEN bits of results are written to the destination rd.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_ADD(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate ADD...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regSrc2 = getRs2(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regSrc1 != regDest && regSrc2 != regDest) {
        fe_enc64(&current, FE_LEA64rm, regDest, FE_MEM(regSrc1, 1, regSrc2, 0));
    } else {
        doArithmCommutative(regSrc1, regSrc2, regDest, FE_ADD64rr);
    }
}

/**
* Translate the SUB instruction.
* SUB performs the subtraction of rs2 from rs1. Overflows
* are ignored and the low XLEN bits of results are written to the destination rd.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SUB(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SUB...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regSrc2 = getRs2(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest == regSrc2) {
        if (regDest == regSrc1) {
            err |= fe_enc64(&current, FE_XOR32rr, regDest, regDest); // SUB same, same, same is equivalent to zeroing
        } else {
            err |= fe_enc64(&current, FE_NEG64r, regSrc2);
            err |= fe_enc64(&current, FE_ADD64rr, regSrc2, regSrc1); // equivalent to regDest, regSrc1
        }
    } else {
        doArithmCommutative(regSrc1, regSrc2, regDest, FE_SUB64rr);
    }
}

/**
* Translate the SLL instruction.
* SLL, SRL, and SRA perform logical left, logical right, and arithmetic right shifts on the value in
* register rs1 by the shift amount held in the lower 6 bits of register rs2.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SLL(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SLL...\n");

    FeReg regSrc2 = getRs2Into(instr, r_info, FE_CX);
    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        ///If rd and rs2 are the same, we need to load rd into another register (not rs1), to not override shift amount
        if (regDest == regSrc2) {
            if (regSrc1 == FE_AX) {
                invalidateReplacement(r_info, FE_DX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_DX, true);
            } else {
                invalidateReplacement(r_info, FE_AX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_AX, true);
            }
        }
        err |= fe_enc64(&current, FE_MOV64rr, regDest, regSrc1);
    }

    err |= fe_enc64(&current, FE_SHL64rr, regDest, regSrc2);
}

/**
* Translate the SLT instruction.
* SLT and SLTU perform signed and unsigned compares respectively, writing 1 to rd if rs1 < rs2, 0 otherwise.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SLT(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SLT...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regSrc2 = getRs2(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    err |= fe_enc64(&current, FE_CMP64rr, regSrc1, regSrc2);
    err |= fe_enc64(&current, FE_SETL8r, regDest);
    err |= fe_enc64(&current, FE_MOVZXr32r8, regDest, regDest);
}

/**
* Translate the SLTU instruction.
* SLT and SLTU perform signed and unsigned compares respectively, writing 1 to rd if rs1 < rs2, 0 otherwise.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SLTU(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SLTU...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regSrc2 = getRs2(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    err |= fe_enc64(&current, FE_CMP64rr, regSrc1, regSrc2);
    err |= fe_enc64(&current, FE_SETC8r, regDest);
    err |= fe_enc64(&current, FE_MOVZXr32r8, regDest, regDest);
}

/**
* Translate the XOR instruction.
* XOR performs the bitwise logical XOR of rs1 and rs2.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_XOR(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate XOR...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regSrc2 = getRs2(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    doArithmCommutative(regSrc1, regSrc2, regDest, FE_XOR64rr);
}

/**
* Translate the SRL instruction.
* SLL, SRL, and SRA perform logical left, logical right, and arithmetic right shifts on the value in
* register rs1 by the shift amount held in the lower 6 bits of register rs2.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SRL(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SRL...\n");

    FeReg regSrc2 = getRs2Into(instr, r_info, FE_CX);
    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        ///If rd and rs2 are the same, we need to load rd into another register (not rs1), to not override shift amount
        if (regDest == regSrc2) {
            if (regSrc1 == FE_AX) {
                invalidateReplacement(r_info, FE_DX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_DX, true);
            } else {
                invalidateReplacement(r_info, FE_AX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_AX, true);
            }
        }

        err |= fe_enc64(&current, FE_MOV64rr, regDest, regSrc1);
    }

    err |= fe_enc64(&current, FE_SHR64rr, regDest, regSrc2);
}

/**
* Translate the SRA instruction.
* SLL, SRL, and SRA perform logical left, logical right, and arithmetic right shifts on the value in
* register rs1 by the shift amount held in the lower 6 bits of register rs2.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SRA(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SRA...\n");

    FeReg regSrc2 = getRs2Into(instr, r_info, FE_CX);
    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        ///If rd and rs2 are the same, we need to load rd into another register (not rs1), to not override shift amount
        if (regDest == regSrc2) {
            if (regSrc1 == FE_AX) {
                invalidateReplacement(r_info, FE_DX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_DX, true);
            } else {
                invalidateReplacement(r_info, FE_AX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_AX, true);
            }
        }

        err |= fe_enc64(&current, FE_MOV64rr, regDest, regSrc1);
    }

    err |= fe_enc64(&current, FE_SAR64rr, regDest, regSrc2);
}

/**
* Translate the OR instruction.
* OR performs the bitwise logical OR of rs1 and rs2.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_OR(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate OR...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regSrc2 = getRs2(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    doArithmCommutative(regSrc1, regSrc2, regDest, FE_OR64rr);
}

/**
* Translate the AND instruction.
* AND performs the bitwise logical AND of rs1 and rs2.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_AND(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate AND...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regSrc2 = getRs2(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    doArithmCommutative(regSrc1, regSrc2, regDest, FE_AND64rr);
}

/**
* Translate the SLLIW instruction. It is defined analogous to SLLI, but operates on 32-bit values.
* The operand to be shifted is in rs1, the shift amount is in the lower 5 bits of the I-immediate field.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SLLIW(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SLLIW...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    t_risc_imm masked_imm = instr->imm & 0x1f;
    if (regDest != regSrc1 && (masked_imm == 1 || masked_imm == 2 || masked_imm == 3)) {
        err |= fe_enc64(&current, FE_LEA32rm, regDest, FE_MEM(0, 1 << masked_imm, regSrc1, 0));
    } else {
        if (regDest != regSrc1) {
            err |= fe_enc64(&current, FE_MOV32rr, regDest, regSrc1);
        }

        err |= fe_enc64(&current, FE_SHL32ri, regDest, masked_imm);
    }

    err |= fe_enc64(&current, FE_MOVSXr64r32, regDest, regDest);
}

/**
* Translate the SRLIW instruction. It is defined analogous to SRLI, but operates on 32-bit values.
* The operand to be shifted is in rs1, the shift amount is in the lower 5 bits of the I-immediate field.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SRLIW(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SRLIW...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        err |= fe_enc64(&current, FE_MOV32rr, regDest, regSrc1);
    }
    err |= fe_enc64(&current, FE_SHR32ri, regDest, instr->imm & 0x1f);
    err |= fe_enc64(&current, FE_MOVSXr64r32, regDest, regDest);
}

/**
* Translate the SRAIW instruction. It is defined analogous to SRAI, but operates on 32-bit values.
* The operand to be shifted is in rs1, the shift amount is in the lower 5 bits of the I-immediate field.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SRAIW(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SRAIW...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        err |= fe_enc64(&current, FE_MOV32rr, regDest, regSrc1);
    }
    err |= fe_enc64(&current, FE_SAR32ri, regDest, instr->imm & 0x1f);
    err |= fe_enc64(&current, FE_MOVSXr64r32, regDest, regDest);
}

/**
* Translate the ADDW instruction.
* ADDW and SUBW are RV64I-only instructions that are defined analogously to ADD and SUB
* but operate on 32-bit values and produce signed 32-bit results. Overflows are ignored, and the low
* 32-bits of the result is sign-extended to 64-bits and written to the destination register.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_ADDW(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate ADDW...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regSrc2 = getRs2(instr, r_info);
    FeReg regDest = getRd(instr, r_info);
    if (regSrc1 != regDest && regSrc2 != regDest) {
        fe_enc64(&current, FE_LEA32rm, regDest, FE_MEM(regSrc1, 1, regSrc2, 0));
    } else {
        doArithmCommutative(regSrc1, regSrc2, regDest, FE_ADD32rr);
    }
    err |= fe_enc64(&current, FE_MOVSXr64r32, regDest, regDest);
}

/**
* Translate the SUBW instruction.
* ADDW and SUBW are RV64I-only instructions that are defined analogously to ADD and SUB
* but operate on 32-bit values and produce signed 32-bit results. Overflows are ignored, and the low
* 32-bits of the result is sign-extended to 64-bits and written to the destination register.
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
*/
void translate_SUBW(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SUBW...\n");

    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regSrc2 = getRs2(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest == regSrc2) {
        if (regDest == regSrc1) {
            err |= fe_enc64(&current, FE_XOR32rr, regDest, regDest); // SUB same, same, same is equivalent to zeroing
        } else {
            err |= fe_enc64(&current, FE_NEG32r, regSrc2);
            err |= fe_enc64(&current, FE_ADD32rr, regSrc2, regSrc1); // equivalent to regDest, regSrc1
        }
    } else {
        doArithmCommutative(regSrc1, regSrc2, regDest, FE_SUB32rr);
    }

    err |= fe_enc64(&current, FE_MOVSXr64r32, regDest, regDest);
}

/**
* Translate the SLLW instruction.
* SLLW, SRLW, and SRAW are RV64I-only instructions that are analogously defined but operate
* on 32-bit values and produce signed 32-bit results. The shift amount is given by rs2[4:0]
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
* @see SLL, SRL, SRA
*/
void translate_SLLW(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SLLW...\n");

    FeReg regSrc2 = getRs2Into(instr, r_info, FE_CX);
    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        ///If rd and rs2 are the same, we need to load rd into another register (not rs1), to not override shift amount
        if (regDest == regSrc2) {
            if (regSrc1 == FE_AX) {
                invalidateReplacement(r_info, FE_DX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_DX, true);
            } else {
                invalidateReplacement(r_info, FE_AX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_AX, true);
            }
        }

        err |= fe_enc64(&current, FE_MOV32rr, regDest, regSrc1);
    }

    err |= fe_enc64(&current, FE_SHL32rr, regDest, regSrc2);
    err |= fe_enc64(&current, FE_MOVSXr64r32, regDest, regDest);
}

/**
* Translate the SRLW instruction.
* SLLW, SRLW, and SRAW are RV64I-only instructions that are analogously defined but operate
* on 32-bit values and produce signed 32-bit results. The shift amount is given by rs2[4:0]
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
* @see SLL, SRL, SRA
*/
void translate_SRLW(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SRLW...\n");

    FeReg regSrc2 = getRs2Into(instr, r_info, FE_CX);
    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        ///If rd and rs2 are the same, we need to load rd into another register (not rs1), to not override shift amount
        if (regDest == regSrc2) {
            if (regSrc1 == FE_AX) {
                invalidateReplacement(r_info, FE_DX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_DX, true);
            } else {
                invalidateReplacement(r_info, FE_AX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_AX, true);
            }
        }

        err |= fe_enc64(&current, FE_MOV32rr, regDest, regSrc1);
    }

    err |= fe_enc64(&current, FE_SHR32rr, regDest, regSrc2);
    err |= fe_enc64(&current, FE_MOVSXr64r32, regDest, regDest);
}

/**
* Translate the SRAW instruction.
* SLLW, SRLW, and SRAW are RV64I-only instructions that are analogously defined but operate
* on 32-bit values and produce signed 32-bit results. The shift amount is given by rs2[4:0]
* @param instr the RISC-V instruction to translate
* @param r_info the runtime register mapping (RISC-V -> x86)
* @see SLL, SRL, SRA
*/
void translate_SRAW(const t_risc_instr *instr, const register_info *r_info) {
    log_asm_out("Translate SRAW...\n");

    FeReg regSrc2 = getRs2Into(instr, r_info, FE_CX);
    FeReg regSrc1 = getRs1(instr, r_info);
    FeReg regDest = getRd(instr, r_info);

    if (regDest != regSrc1) {
        ///If rd and rs2 are the same, we need to load rd into another register (not rs1), to not override shift amount
        if (regDest == regSrc2) {
            if (regSrc1 == FE_AX) {
                invalidateReplacement(r_info, FE_DX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_DX, true);
            } else {
                invalidateReplacement(r_info, FE_AX, true);
                regDest = loadIntoSpecific(r_info, instr->reg_dest, FE_AX, true);
            }
        }

        err |= fe_enc64(&current, FE_MOV32rr, regDest, regSrc1);
    }

    err |= fe_enc64(&current, FE_SAR32rr, regDest, regSrc2);
    err |= fe_enc64(&current, FE_MOVSXr64r32, regDest, regDest);
}
