//
// Created by flo on 24.04.20.
//

#include <stdio.h>
#include <stdbool.h>
#include "util.h"
#include "cache.h"
#include "translate.hpp"
#include "parser.h"
#include <register.h>
#include "loadElf.h"
#include <getopt.h>
#include <../test/test.h>

//just temporary - we need some way to control transcoding globally?
bool finalize = false;

//prototypes
int transcode_loop();

t_risc_addr init_entry_pc();

void execute_cached(t_cache_loc loc);

t_cache_loc translate_block(t_risc_addr risc_addr);

#ifndef TESTING
int main(int argc, char *argv[]) {
    int opt_index = 0;
    char *file_path = NULL;

    //read command line options (ex. -f for executable file, -v for verbose logging, etc.)
    while ((opt_index = getopt(argc, argv, ":f:vh")) != -1) {
        switch (opt_index) {
            case 'v':
                verbose = true;
                break;
            case 'f':
                file_path = optarg;
                break;
            case ':':
            case 'h':
            default:
                fprintf(stderr, "Usage: dynamic-translate -f <filename> [-v][…]\n");
                return 1;
        }
    }

    printf("Verbose: %d\n", verbose);
    printf("File path: %s\n", file_path);

    if (file_path == NULL) {
        fprintf(stderr, "Bad. Invalid file path.\n");
        return 2;
    }

    printf("Start program!\n");
    transcode_loop(file_path);
    return 0;
}

#endif //TESTING

int start_transcode(const char *file_path){
    printf("extern transcode start!\n");
    verbose = true;
    transcode_loop(file_path);
    return 0;
}

int transcode_loop(const char *file_path) {
    t_risc_elf_map_result result = mapIntoMemory(file_path);
    if(!result.valid){
        fprintf(stderr, "Bad. Failed to map into memory.\n");
    }

    t_risc_addr next_pc = result.entry;

    //allocate stack
    createStack(0,"",result);

    init_hash_table();

    while (!finalize) {
        //check our previously translated code
        t_cache_loc cache_loc = lookup_cache_entry(next_pc);

        //we have not seen this block before
        if (cache_loc == UNSEEN_CODE) {
            cache_loc = translate_block(next_pc);
            set_cache_entry(next_pc,cache_loc);
        }

        //execute the cached (or now newly generated code) and update the program counter
        execute_cached(next_pc);

        //store pc from registers in pc
        next_pc = get_value(pc);

        //tmp - programm should exit on syscall
        //finalize = true;
    }

    return 0;
}

/**
 * Moved to translate.cpp
 * Translate the basic block at the passed RISC-V pc address.
 * @param risc_addr the RISC-V address of the basic block in question
 * @return cache location of the translated code
 */
/*t_cache_loc translate_block(t_risc_addr risc_addr) {
    not_yet_implemented("Translate Block");

    return 0;
}*/

/**
 * Execute cached translated code at the passed location.
 * @param loc the cache address of that code
 * @return
 */
void execute_cached(t_cache_loc loc) {
    printf("Execute Cached");
    typedef void (*void_asm)(void);
    ((void_asm)loc)(); //call asm code
}

t_risc_instr *decode_next() {
    not_yet_implemented("Decode next");
    return NULL;
}

/**
 * Initialize the program counter to the program entry point address.
 * @return initial RISC-V program counter value
 */
t_risc_addr init_entry_pc() {
    not_yet_implemented("Init entry pc");
    return 0;
}
