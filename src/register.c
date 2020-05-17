//
// Created by flo on 10.05.20.
//

#include "register.h"

/**
 * The raw register contents stored in memory.
 * Access will be relatively expensive, so optimisation is needed.
 * contents[0] is undefined, as x0 is hardwired to 0 -
 * it is left unused in order to avoid off-by-one errors in indexing.
 */
t_risc_reg_val contents[33];

t_risc_reg_val *get_reg_data() {
    return contents;
}

/**
 * Get the value currently in the passed register.
 * @param reg the register to lookup
 * @return value in register reg
 */
t_risc_reg_val get_value(t_risc_reg reg) {
    if (reg == x0) {
        //an access to x0 always yields 0
        return 0;
    } else {
        return contents[reg];
    }

}

/**
 * Set the value of the passed register.
 * @param reg the register to update
 * @param val the new value
 */
void set_value(t_risc_reg reg, t_risc_reg_val val) {
    //a write to x0 is ignored, hardwired zero
    if (reg != x0) {
        contents[reg] = val;
    }
}