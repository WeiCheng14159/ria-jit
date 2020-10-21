//
// Created by noah on 12.07.20.
//

#include "typedefs.h"

char *errorcode_to_string(int errorcode) {
    switch (errorcode) {
        case E_UNKNOWN:
            return "unknown";
        case E_f3_32:
            return "unknown funct3 code for OP_32";
        case E_f3_AMO:
            return "unknown funct3 code for OP_AMO";
        case E_f3_BRANCH:
            return "unknown funct3 code for OP_BRANCH";
        case E_f3_IMM:
            return "unknown funct3 code for OP_IMM";
        case E_f3_IMM_32:
            return "unknown funct3 code for OP_IMM_32";
        case E_f3_LOAD:
            return "unknown funct3 code for OP_LOAD";
        case E_f3_STORE:
            return "unknown funct3 code for OP_STORE";
        case E_f3_MISC_MEM:
            return "unknown funct3 code for OP_MISC_MEM";
        case E_f3_RV64M:
            return "unknown funct3 code for RV64M";
        case E_f3_SYSTEM:
            return "unknown funct3 code for OP_SYSTEM";
        case E_f3_OP:
            return "unknown funct3 code for OP_OP";
        case E_f7_AMO:
            return "unknown funct7 code for OP_AMO";
        default:
            return "unknown error code";
    }
}

char *mnem_to_string(int mnem) {
    switch (mnem) {
        case PATTERN_EMIT:
            return "PATTERN";
        case PC_NEXT_INST:
            return "PC_NEXT_INST";
        case MANUAL_CSRR:
            return "MANUAL_CSRR";
        case LUI:
            return "LUI";
        case AUIPC:
            return "AUIPC";
        case JAL:
            return "JAL";
        case JALR:
            return "JALR";
        case BEQ:
            return "BEQ";
        case BNE:
            return "BNE";
        case BLT:
            return "BLT";
        case BGE:
            return "BGE";
        case BLTU:
            return "BLTU";
        case BGEU:
            return "BGEU";
        case LB:
            return "LB";
        case LH:
            return "LH";
        case LW:
            return "LW";
        case LBU:
            return "LBU";
        case LHU:
            return "LHU";
        case SB:
            return "SB";
        case SH:
            return "SH";
        case SW:
            return "SW";
        case ADDI:
            return "ADDI";
        case SLTI:
            return "SLTI";
        case SLTIU:
            return "SLTIU";
        case XORI:
            return "XORI";
        case ORI:
            return "ORI";
        case ANDI:
            return "ANDI";
        case SLLI:
            return "SLLI";
        case SRLI:
            return "SRLI";
        case SRAI:
            return "SRAI";
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case SLL:
            return "SLL";
        case SLT:
            return "SLT";
        case SLTU:
            return "SLTU";
        case XOR:
            return "XOR";
        case SRL:
            return "SRL";
        case SRA:
            return "SRA";
        case OR:
            return "OR";
        case AND:
            return "AND";
        case FENCE:
            return "FENCE";
        case ECALL:
            return "ECALL";
        case EBREAK:
            return "EBREAK";
        case FENCE_I:
            return "FENCE_I";
        case CSRRW:
            return "CSRRW";
        case CSRRS:
            return "CSRRS";
        case CSRRC:
            return "CSRRC";
        case CSRRWI:
            return "CSRRWI";
        case CSRRSI:
            return "CSRRSI";
        case CSRRCI:
            return "CSRRCI";
        case LWU:
            return "LWU";
        case LD:
            return "LD";
        case SD:
            return "SD";
        case ADDIW:
            return "ADDIW";
        case SLLIW:
            return "SLLIW";
        case SRLIW:
            return "SRLIW";
        case SRAIW:
            return "SRAIW";
        case ADDW:
            return "ADDW";
        case SUBW:
            return "SUBW";
        case SLLW:
            return "SLLW";
        case SRLW:
            return "SRLW";
        case SRAW:
            return "SRAW";
        case MUL:
            return "MUL";
        case MULH:
            return "MULH";
        case MULHSU:
            return "MULHSU";
        case MULHU:
            return "MULHU";
        case DIV:
            return "DIV";
        case DIVU:
            return "DIVU";
        case REM:
            return "REM";
        case REMU:
            return "REMU";
        case MULW:
            return "MULW";
        case DIVW:
            return "DIVW";
        case DIVUW:
            return "DIVUW";
        case REMW:
            return "REMW";
        case REMUW:
            return "REMUW";
        case LRW:
            return "LRW";
        case SCW:
            return "SCW";
        case AMOSWAPW:
            return "AMOSWAPW";
        case AMOADDW:
            return "AMOADDW";
        case AMOXORW:
            return "AMOXORW";
        case AMOANDW:
            return "AMOANDW";
        case AMOORW:
            return "AMOORW";
        case AMOMINW:
            return "AMOMINW";
        case AMOMAXW:
            return "AMOMAXW";
        case AMOMINUW:
            return "AMOMINUW";
        case AMOMAXUW:
            return "AMOMAXUW";
        case LRD:
            return "LRD";
        case SCD:
            return "SCD";
        case AMOSWAPD:
            return "AMOSWAPD";
        case AMOADDD:
            return "AMOADDD";
        case AMOXORD:
            return "AMOXORD";
        case AMOANDD:
            return "AMOANDD";
        case AMOORD:
            return "AMOORD";
        case AMOMIND:
            return "AMOMIND";
        case AMOMAXD:
            return "AMOMAXD";
        case AMOMINUD:
            return "AMOMINUD";
        case AMOMAXUD:
            return "AMOMAXUD";
        case FLW:
            return "FLW";
        case FSW:
            return "FSW";
        case FMADDS:
            return "FMADDS";
        case FMSUBS:
            return "FMSUBS";
        case FNMSUBS:
            return "FNMSUBS";
        case FNMADDS:
            return "FNMADDS";
        case FADDS:
            return "FADDS";
        case FSUBS:
            return "FSUBS";
        case FMULS:
            return "FMULS";
        case FDIVS:
            return "FDIVS";
        case FSQRTS:
            return "FSQRTS";
        case FSGNJS:
            return "FSGNJS";
        case FSGNJNS:
            return "FSGNJNS";
        case FSGNJXS:
            return "FSGNJXS";
        case FMINS:
            return "FMINS";
        case FMAXS:
            return "FMAXS";
        case FCVTWS:
            return "FCVTWS";
        case FCVTWUS:
            return "FCVTWUS";
        case FMVXW:
            return "FMVXW";
        case FEQS:
            return "FEQS";
        case FLTS:
            return "FLTS";
        case FLES:
            return "FLES";
        case FCLASSS:
            return "FCLASSS";
        case FCVTSW:
            return "FCVTSW";
        case FCVTSWU:
            return "FCVTSWU";
        case FMVWX:
            return "FMVWX";
        case FCVTLS:
            return "FCVTLS";
        case FCVTLUS:
            return "FCVTLUS";
        case FCVTSL:
            return "FCVTSL";
        case FCVTSLU:
            return "FCVTSLU";
        case FLD:
            return "FLD";
        case FSD:
            return "FSD";
        case FMADDD:
            return "FMADDD";
        case FMSUBD:
            return "FMSUBD";
        case FNMSUBD:
            return "FNMSUBD";
        case FNMADDD:
            return "FNMADDD";
        case FADDD:
            return "FADDD";
        case FSUBD:
            return "FSUBD";
        case FMULD:
            return "FMULD";
        case FDIVD:
            return "FDIVD";
        case FSQRTD:
            return "FSQRTD";
        case FSGNJD:
            return "FSGNJD";
        case FSGNJND:
            return "FSGNJND";
        case FSGNJXD:
            return "FSGNJXD";
        case FMIND:
            return "FMIND";
        case FMAXD:
            return "FMAXD";
        case FCVTSD:
            return "FCVTSD";
        case FCVTDS:
            return "FCVTDS";
        case FEQD:
            return "FEQD";
        case FLTD:
            return "FLTD";
        case FLED:
            return "FLED";
        case FCLASSD:
            return "FCLASSD";
        case FCVTWD:
            return "FCVTWD";
        case FCVTWUD:
            return "FCVTWUD";
        case FCVTDW:
            return "FCVTDW";
        case FCVTDWU:
            return "FCVTDWU";
        case FCVTLD:
            return "FCVTLD";
        case FCVTLUD:
            return "FCVTLUD";
        case FMVXD:
            return "FMVXD";
        case FCVTDL:
            return "FCVTDL";
        case FCVTDLU:
            return "FCVTDLU";
        case FMVDX:
            return "FMVDX";
        case SILENT_NOP:
            return "SILENT_NOP";
        default:
            return "UNKNOWN";
    }
}

char *gp_to_string(int reg) {
    switch (reg) {
        case x0:
            return "x0";
        case x1:
            return "x1";
        case x2:
            return "x2";
        case x3:
            return "x3";
        case x4:
            return "x4";
        case x5:
            return "x5";
        case x6:
            return "x6";
        case x7:
            return "x7";
        case x8:
            return "x8";
        case x9:
            return "x9";
        case x10:
            return "x10";
        case x11:
            return "x11";
        case x12:
            return "x12";
        case x13:
            return "x13";
        case x14:
            return "x14";
        case x15:
            return "x15";
        case x16:
            return "x16";
        case x17:
            return "x17";
        case x18:
            return "x18";
        case x19:
            return "x19";
        case x20:
            return "x20";
        case x21:
            return "x21";
        case x22:
            return "x22";
        case x23:
            return "x23";
        case x24:
            return "x24";
        case x25:
            return "x25";
        case x26:
            return "x26";
        case x27:
            return "x27";
        case x28:
            return "x28";
        case x29:
            return "x29";
        case x30:
            return "x30";
        case x31:
            return "x31";
        default:
            return "INVALID";
    }
}

char *gp_to_alias(int reg) {
    switch (reg) {
        case zero:
            return "zero";
        case ra:
            return "ra";
        case sp:
            return "sp";
        case gp:
            return "gp";
        case tp:
            return "tp";
        case t0:
            return "t0";
        case t1:
            return "t1";
        case t2:
            return "t2";
        case s0:
            return "s0/fp";
        case s1:
            return "s1";
        case a0:
            return "a0";
        case a1:
            return "a1";
        case a2:
            return "a2";
        case a3:
            return "a3";
        case a4:
            return "a4";
        case a5:
            return "a5";
        case a6:
            return "a6";
        case a7:
            return "a7";
        case s2:
            return "s2";
        case s3:
            return "s3";
        case s4:
            return "s4";
        case s5:
            return "s5";
        case s6:
            return "s6";
        case s7:
            return "s7";
        case s8:
            return "s8";
        case s9:
            return "s9";
        case s10:
            return "s10";
        case s11:
            return "s11";
        case t3:
            return "t3";
        case t4:
            return "t4";
        case t5:
            return "t5";
        case t6:
            return "t6";
        default:
            return "INVALID";
    }
}

char *reg_x86_to_string(FeReg reg) {
    switch (reg) {
        case FE_AX:
            return "AX";
        case FE_CX:
            return "CX";
        case FE_DX:
            return "DX";
        case FE_BX:
            return "BX";
        case FE_SP:
            return "SP";
        case FE_BP:
            return "BP";
        case FE_SI:
            return "SI";
        case FE_DI:
            return "DI";
        case FE_R8:
            return "R8";
        case FE_R9:
            return "R9";
        case FE_R10:
            return "R10";
        case FE_R11:
            return "R11";
        case FE_R12:
            return "R12";
        case FE_R13:
            return "R13";
        case FE_R14:
            return "R14";
        case FE_R15:
            return "R15";
        case FE_IP:
            return "IP";
        case FE_AH:
            return "AH";
        case FE_CH:
            return "CH";
        case FE_DH:
            return "DH";
        case FE_BH:
            return "BH";
        case FE_ES:
            return "ES";
        case FE_CS:
            return "CS";
        case FE_SS:
            return "SS";
        case FE_DS:
            return "DS";
        case FE_FS:
            return "FS";
        case FE_GS:
            return "GS";
        case FE_ST0:
            return "ST0";
        case FE_ST1:
            return "ST1";
        case FE_ST2:
            return "ST2";
        case FE_ST3:
            return "ST3";
        case FE_ST4:
            return "ST4";
        case FE_ST5:
            return "ST5";
        case FE_ST6:
            return "ST6";
        case FE_ST7:
            return "ST7";
        case FE_MM0:
            return "MM0";
        case FE_MM1:
            return "MM1";
        case FE_MM2:
            return "MM2";
        case FE_MM3:
            return "MM3";
        case FE_MM4:
            return "MM4";
        case FE_MM5:
            return "MM5";
        case FE_MM6:
            return "MM6";
        case FE_MM7:
            return "MM7";
        case FE_XMM0:
            return "XMM0";
        case FE_XMM1:
            return "XMM1";
        case FE_XMM2:
            return "XMM2";
        case FE_XMM3:
            return "XMM3";
        case FE_XMM4:
            return "XMM4";
        case FE_XMM5:
            return "XMM5";
        case FE_XMM6:
            return "XMM6";
        case FE_XMM7:
            return "XMM7";
        case FE_XMM8:
            return "XMM8";
        case FE_XMM9:
            return "XMM9";
        case FE_XMM10:
            return "XMM10";
        case FE_XMM11:
            return "XMM11";
        case FE_XMM12:
            return "XMM12";
        case FE_XMM13:
            return "XMM13";
        case FE_XMM14:
            return "XMM14";
        case FE_XMM15:
            return "XMM15";
        default:
            return "INVALID";
    }
}

char *fp_to_string(int reg) {
    switch (reg) {
        case f0:
            return "f0";
        case f1:
            return "f1";
        case f2:
            return "f2";
        case f3:
            return "f3";
        case f4:
            return "f4";
        case f5:
            return "f5";
        case f6:
            return "f6";
        case f7:
            return "f7";
        case f8:
            return "f8";
        case f9:
            return "f9";
        case f10:
            return "f10";
        case f11:
            return "f11";
        case f12:
            return "f12";
        case f13:
            return "f13";
        case f14:
            return "f14";
        case f15:
            return "f15";
        case f16:
            return "f16";
        case f17:
            return "f17";
        case f18:
            return "f18";
        case f19:
            return "f19";
        case f20:
            return "f20";
        case f21:
            return "f21";
        case f22:
            return "f22";
        case f23:
            return "f23";
        case f24:
            return "f24";
        case f25:
            return "f25";
        case f26:
            return "f26";
        case f27:
            return "f27";
        case f28:
            return "f28";
        case f29:
            return "f29";
        case f30:
            return "f30";
        case f31:
            return "f31";
        default:
            return "INVALID";
    }
}

char *fp_to_alias(int reg) {
    switch (reg) {
        case f0:
            return "ft0";
        case f1:
            return "ft1";
        case f2:
            return "ft2";
        case f3:
            return "ft3";
        case f4:
            return "ft4";
        case f5:
            return "ft5";
        case f6:
            return "ft6";
        case f7:
            return "ft7";
        case f8:
            return "fs0";
        case f9:
            return "fs1";
        case f10:
            return "fa0";
        case f11:
            return "fa1";
        case f12:
            return "fa2";
        case f13:
            return "fa3";
        case f14:
            return "fa4";
        case f15:
            return "fa5";
        case f16:
            return "fa6";
        case f17:
            return "fa7";
        case f18:
            return "fs2";
        case f19:
            return "fs3";
        case f20:
            return "fs4";
        case f21:
            return "fs5";
        case f22:
            return "fs6";
        case f23:
            return "fs7";
        case f24:
            return "fs8";
        case f25:
            return "fs9";
        case f26:
            return "fs10";
        case f27:
            return "fs11";
        case f28:
            return "ft8";
        case f29:
            return "ft9";
        case f30:
            return "ft10";
        case f31:
            return "ft11";
        default:
            return "INVALID";
    }
}
