//
// Created by noah on 12.07.20.
//

#include "typedefs.h"


char *mnem_to_string(int mnem) {
    switch(mnem) {
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
            return "AMOMAXW";
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
        default:
            return "UNKNOWN";
    }
}
