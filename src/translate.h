//
// Created by flo on 09.05.20.
//

#ifndef DYNAMICBINARYTRANSLATORRISCV64_X86_64_TRANSLATE_H
#define DYNAMICBINARYTRANSLATORRISCV64_X86_64_TRANSLATE_H

#include "cache.h"

#ifdef __cplusplus
extern "C" {
#endif

void test_generation();

//basic block translation management
void init_block();
t_cache_loc finalize_block();

//instruction translation
void translate_instr(t_risc_instr instr);

#ifdef __cplusplus
}
#endif

#endif //DYNAMICBINARYTRANSLATORRISCV64_X86_64_TRANSLATE_H
