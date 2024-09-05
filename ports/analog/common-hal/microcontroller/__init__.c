// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2016 Scott Shawcroft for Adafruit Industries
// SPDX-FileCopyrightText: Copyright (c) 2019 Lucian Copeland for Adafruit Industries
// SPDX-FileCopyrightText: Copyright (c) 2024 Brandon Hurst, Analog Devices, Inc
//
// SPDX-License-Identifier: MIT

#include "py/mphal.h"
#include "py/obj.h"
#include "py/runtime.h"

#include "common-hal/microcontroller/Pin.h"
#include "common-hal/microcontroller/Processor.h"

// #include "shared-bindings/nvm/ByteArray.h"
#include "shared-bindings/microcontroller/__init__.h"
#include "shared-bindings/microcontroller/Pin.h"
#include "shared-bindings/microcontroller/Processor.h"
#include "supervisor/port.h"
#include "supervisor/filesystem.h"
#include "supervisor/shared/safe_mode.h"


#include "max32690.h"
#include "mxc_delay.h"

/** NOTE: It is not advised to directly include the below!
 * These are includes taken care of by the core cmsis file.
 * e.g. "max32690.h". Since CMSIS is compiled as lib, these are
 * included there as <core_cm4.h> for example.
*/
// #include <core_cmFunc.h>    // For enable/disable interrupts
// #include <core_cm4.h>       // For NVIC_SystemReset
// #include <core_cmInstr.h>   // For __DMB Data Memory Barrier (flush DBUS activity)

void common_hal_mcu_delay_us(uint32_t delay) {

    MXC_Delay(MXC_DELAY_USEC(delay));
}

volatile uint32_t nesting_count = 0;

void common_hal_mcu_disable_interrupts(void) {
    __disable_irq();
    __DMB();
    nesting_count++;
}

void common_hal_mcu_enable_interrupts(void) {
    if (nesting_count == 0) {
        // This is very very bad because it means there was mismatched disable/enables.
        reset_into_safe_mode(SAFE_MODE_INTERRUPT_ERROR);
    }
    nesting_count--;
    if (nesting_count > 0) {
        return;
    }
    __DMB(); // flush internal DBUS before proceeding
    __enable_irq();
}

static bool next_reset_to_bootloader = false;

void common_hal_mcu_on_next_reset(mcu_runmode_t runmode) {
    if (runmode == RUNMODE_SAFE_MODE) {
        safe_mode_on_next_reset(SAFE_MODE_PROGRAMMATIC);
    }
    if (runmode == RUNMODE_BOOTLOADER) {
        next_reset_to_bootloader = true;
    }
}

void common_hal_mcu_reset(void) {
    if (next_reset_to_bootloader) {
        reset_to_bootloader();
    } else {
        NVIC_SystemReset();
    }
}

// The singleton microcontroller.Processor object, bound to microcontroller.cpu
// It currently only has properties, and no state.
const mcu_processor_obj_t common_hal_mcu_processor_obj = {
    .base = {
        .type = &mcu_processor_type,
    },
};

// This maps MCU pin names to pin objects.
static const mp_rom_map_elem_t mcu_pin_global_dict_table[] = {
    #if defined(PIN_PA01) && !defined(IGNORE_PIN_PA01)
    { MP_ROM_QSTR(MP_QSTR_PA01), MP_ROM_PTR(&pin_PA01) },
    #endif
    #if defined(PIN_PA02) && !defined(IGNORE_PIN_PA02)
    { MP_ROM_QSTR(MP_QSTR_PA02), MP_ROM_PTR(&pin_PA02) },
    #endif
    #if defined(PIN_PA03) && !defined(IGNORE_PIN_PA03)
    { MP_ROM_QSTR(MP_QSTR_PA03), MP_ROM_PTR(&pin_PA03) },
    #endif
    #if defined(PIN_PA04) && !defined(IGNORE_PIN_PA04)
    { MP_ROM_QSTR(MP_QSTR_PA04), MP_ROM_PTR(&pin_PA04) },
    #endif
    #if defined(PIN_PA05) && !defined(IGNORE_PIN_PA05)
    { MP_ROM_QSTR(MP_QSTR_PA05), MP_ROM_PTR(&pin_PA05) },
    #endif
    #if defined(PIN_PA06) && !defined(IGNORE_PIN_PA06)
    { MP_ROM_QSTR(MP_QSTR_PA06), MP_ROM_PTR(&pin_PA06) },
    #endif
    #if defined(PIN_PA07) && !defined(IGNORE_PIN_PA07)
    { MP_ROM_QSTR(MP_QSTR_PA07), MP_ROM_PTR(&pin_PA07) },
    #endif
    #if defined(PIN_PA08) && !defined(IGNORE_PIN_PA08)
    { MP_ROM_QSTR(MP_QSTR_PA08), MP_ROM_PTR(&pin_PA08) },
    #endif
    #if defined(PIN_PA09) && !defined(IGNORE_PIN_PA09)
    { MP_ROM_QSTR(MP_QSTR_PA09), MP_ROM_PTR(&pin_PA09) },
    #endif
    #if defined(PIN_PA10) && !defined(IGNORE_PIN_PA10)
    { MP_ROM_QSTR(MP_QSTR_PA10), MP_ROM_PTR(&pin_PA10) },
    #endif
    #if defined(PIN_PA11) && !defined(IGNORE_PIN_PA11)
    { MP_ROM_QSTR(MP_QSTR_PA11), MP_ROM_PTR(&pin_PA11) },
    #endif
    #if defined(PIN_PA12) && !defined(IGNORE_PIN_PA12)
    { MP_ROM_QSTR(MP_QSTR_PA12), MP_ROM_PTR(&pin_PA12) },
    #endif
    #if defined(PIN_PA13) && !defined(IGNORE_PIN_PA13)
    { MP_ROM_QSTR(MP_QSTR_PA13), MP_ROM_PTR(&pin_PA13) },
    #endif
    #if defined(PIN_PA14) && !defined(IGNORE_PIN_PA14)
    { MP_ROM_QSTR(MP_QSTR_PA14), MP_ROM_PTR(&pin_PA14) },
    #endif
    #if defined(PIN_PA15) && !defined(IGNORE_PIN_PA15)
    { MP_ROM_QSTR(MP_QSTR_PA15), MP_ROM_PTR(&pin_PA15) },
    #endif
    #if defined(PIN_PA16) && !defined(IGNORE_PIN_PA16)
    { MP_ROM_QSTR(MP_QSTR_PA16), MP_ROM_PTR(&pin_PA16) },
    #endif
    #if defined(PIN_PA17) && !defined(IGNORE_PIN_PA17)
    { MP_ROM_QSTR(MP_QSTR_PA17), MP_ROM_PTR(&pin_PA17) },
    #endif
    #if defined(PIN_PA18) && !defined(IGNORE_PIN_PA18)
    { MP_ROM_QSTR(MP_QSTR_PA18), MP_ROM_PTR(&pin_PA18) },
    #endif
    #if defined(PIN_PA19) && !defined(IGNORE_PIN_PA19)
    { MP_ROM_QSTR(MP_QSTR_PA19), MP_ROM_PTR(&pin_PA19) },
    #endif
    #if defined(PIN_PA20) && !defined(IGNORE_PIN_PA20)
    { MP_ROM_QSTR(MP_QSTR_PA20), MP_ROM_PTR(&pin_PA20) },
    #endif
    #if defined(PIN_PA21) && !defined(IGNORE_PIN_PA21)
    { MP_ROM_QSTR(MP_QSTR_PA21), MP_ROM_PTR(&pin_PA21) },
    #endif
    #if defined(PIN_PA22) && !defined(IGNORE_PIN_PA22)
    { MP_ROM_QSTR(MP_QSTR_PA22), MP_ROM_PTR(&pin_PA22) },
    #endif
    #if defined(PIN_PA23) && !defined(IGNORE_PIN_PA23)
    { MP_ROM_QSTR(MP_QSTR_PA23), MP_ROM_PTR(&pin_PA23) },
    #endif
    #if defined(PIN_PA24) && !defined(IGNORE_PIN_PA24)
    { MP_ROM_QSTR(MP_QSTR_PA24), MP_ROM_PTR(&pin_PA24) },
    #endif
    #if defined(PIN_PA25) && !defined(IGNORE_PIN_PA25)
    { MP_ROM_QSTR(MP_QSTR_PA25), MP_ROM_PTR(&pin_PA25) },
    #endif
    #if defined(PIN_PA27) && !defined(IGNORE_PIN_PA27)
    { MP_ROM_QSTR(MP_QSTR_PA27), MP_ROM_PTR(&pin_PA27) },
    #endif
    #if defined(PIN_PA28) && !defined(IGNORE_PIN_PA28)
    { MP_ROM_QSTR(MP_QSTR_PA28), MP_ROM_PTR(&pin_PA28) },
    #endif
    #if defined(PIN_PA30) && !defined(IGNORE_PIN_PA30)
    { MP_ROM_QSTR(MP_QSTR_PA30), MP_ROM_PTR(&pin_PA30) },
    #endif
    #if defined(PIN_PA31) && !defined(IGNORE_PIN_PA31)
    { MP_ROM_QSTR(MP_QSTR_PA31), MP_ROM_PTR(&pin_PA31) },
    #endif

    #if defined(PIN_PB01) && !defined(IGNORE_PIN_PB01)
    { MP_ROM_QSTR(MP_QSTR_PB01), MP_ROM_PTR(&pin_PB01) },
    #endif
    #if defined(PIN_PB02) && !defined(IGNORE_PIN_PB02)
    { MP_ROM_QSTR(MP_QSTR_PB02), MP_ROM_PTR(&pin_PB02) },
    #endif
    #if defined(PIN_PB03) && !defined(IGNORE_PIN_PB03)
    { MP_ROM_QSTR(MP_QSTR_PB03), MP_ROM_PTR(&pin_PB03) },
    #endif
    #if defined(PIN_PB04) && !defined(IGNORE_PIN_PB04)
    { MP_ROM_QSTR(MP_QSTR_PB04), MP_ROM_PTR(&pin_PB04) },
    #endif
    #if defined(PIN_PB05) && !defined(IGNORE_PIN_PB05)
    { MP_ROM_QSTR(MP_QSTR_PB05), MP_ROM_PTR(&pin_PB05) },
    #endif
    #if defined(PIN_PB06) && !defined(IGNORE_PIN_PB06)
    { MP_ROM_QSTR(MP_QSTR_PB06), MP_ROM_PTR(&pin_PB06) },
    #endif
    #if defined(PIN_PB07) && !defined(IGNORE_PIN_PB07)
    { MP_ROM_QSTR(MP_QSTR_PB07), MP_ROM_PTR(&pin_PB07) },
    #endif
    #if defined(PIN_PB08) && !defined(IGNORE_PIN_PB08)
    { MP_ROM_QSTR(MP_QSTR_PB08), MP_ROM_PTR(&pin_PB08) },
    #endif
    #if defined(PIN_PB09) && !defined(IGNORE_PIN_PB09)
    { MP_ROM_QSTR(MP_QSTR_PB09), MP_ROM_PTR(&pin_PB09) },
    #endif
    #if defined(PIN_PB10) && !defined(IGNORE_PIN_PB10)
    { MP_ROM_QSTR(MP_QSTR_PB10), MP_ROM_PTR(&pin_PB10) },
    #endif
    #if defined(PIN_PB11) && !defined(IGNORE_PIN_PB11)
    { MP_ROM_QSTR(MP_QSTR_PB11), MP_ROM_PTR(&pin_PB11) },
    #endif
    #if defined(PIN_PB12) && !defined(IGNORE_PIN_PB12)
    { MP_ROM_QSTR(MP_QSTR_PB12), MP_ROM_PTR(&pin_PB12) },
    #endif
    #if defined(PIN_PB13) && !defined(IGNORE_PIN_PB13)
    { MP_ROM_QSTR(MP_QSTR_PB13), MP_ROM_PTR(&pin_PB13) },
    #endif
    #if defined(PIN_PB14) && !defined(IGNORE_PIN_PB14)
    { MP_ROM_QSTR(MP_QSTR_PB14), MP_ROM_PTR(&pin_PB14) },
    #endif
    #if defined(PIN_PB15) && !defined(IGNORE_PIN_PB15)
    { MP_ROM_QSTR(MP_QSTR_PB15), MP_ROM_PTR(&pin_PB15) },
    #endif
    #if defined(PIN_PB16) && !defined(IGNORE_PIN_PB16)
    { MP_ROM_QSTR(MP_QSTR_PB16), MP_ROM_PTR(&pin_PB16) },
    #endif
    #if defined(PIN_PB17) && !defined(IGNORE_PIN_PB17)
    { MP_ROM_QSTR(MP_QSTR_PB17), MP_ROM_PTR(&pin_PB17) },
    #endif
    #if defined(PIN_PB18) && !defined(IGNORE_PIN_PB18)
    { MP_ROM_QSTR(MP_QSTR_PB18), MP_ROM_PTR(&pin_PB18) },
    #endif
    #if defined(PIN_PB19) && !defined(IGNORE_PIN_PB19)
    { MP_ROM_QSTR(MP_QSTR_PB19), MP_ROM_PTR(&pin_PB19) },
    #endif
    #if defined(PIN_PB20) && !defined(IGNORE_PIN_PB20)
    { MP_ROM_QSTR(MP_QSTR_PB20), MP_ROM_PTR(&pin_PB20) },
    #endif
    #if defined(PIN_PB21) && !defined(IGNORE_PIN_PB21)
    { MP_ROM_QSTR(MP_QSTR_PB21), MP_ROM_PTR(&pin_PB21) },
    #endif
    #if defined(PIN_PB22) && !defined(IGNORE_PIN_PB22)
    { MP_ROM_QSTR(MP_QSTR_PB22), MP_ROM_PTR(&pin_PB22) },
    #endif
    #if defined(PIN_PB23) && !defined(IGNORE_PIN_PB23)
    { MP_ROM_QSTR(MP_QSTR_PB23), MP_ROM_PTR(&pin_PB23) },
    #endif
    #if defined(PIN_PB24) && !defined(IGNORE_PIN_PB24)
    { MP_ROM_QSTR(MP_QSTR_PB24), MP_ROM_PTR(&pin_PB24) },
    #endif
    #if defined(PIN_PB25) && !defined(IGNORE_PIN_PB25)
    { MP_ROM_QSTR(MP_QSTR_PB25), MP_ROM_PTR(&pin_PB25) },
    #endif
    #if defined(PIN_PB26) && !defined(IGNORE_PIN_PB26)
    { MP_ROM_QSTR(MP_QSTR_PB26), MP_ROM_PTR(&pin_PB26) },
    #endif
    #if defined(PIN_PB27) && !defined(IGNORE_PIN_PB27)
    { MP_ROM_QSTR(MP_QSTR_PB27), MP_ROM_PTR(&pin_PB27) },
    #endif
    #if defined(PIN_PB28) && !defined(IGNORE_PIN_PB28)
    { MP_ROM_QSTR(MP_QSTR_PB28), MP_ROM_PTR(&pin_PB28) },
    #endif
    #if defined(PIN_PB29) && !defined(IGNORE_PIN_PB29)
    { MP_ROM_QSTR(MP_QSTR_PB29), MP_ROM_PTR(&pin_PB29) },
    #endif
    #if defined(PIN_PB30) && !defined(IGNORE_PIN_PB30)
    { MP_ROM_QSTR(MP_QSTR_PB30), MP_ROM_PTR(&pin_PB30) },
    #endif
    #if defined(PIN_PB31) && !defined(IGNORE_PIN_PB31)
    { MP_ROM_QSTR(MP_QSTR_PB31), MP_ROM_PTR(&pin_PB31) },
    #endif

    #if defined(PIN_PC01) && !defined(IGNORE_PIN_PC01)
    { MP_ROM_QSTR(MP_QSTR_PC01), MP_ROM_PTR(&pin_PC01) },
    #endif
    #if defined(PIN_PC02) && !defined(IGNORE_PIN_PC02)
    { MP_ROM_QSTR(MP_QSTR_PC02), MP_ROM_PTR(&pin_PC02) },
    #endif
    #if defined(PIN_PC03) && !defined(IGNORE_PIN_PC03)
    { MP_ROM_QSTR(MP_QSTR_PC03), MP_ROM_PTR(&pin_PC03) },
    #endif
    #if defined(PIN_PC04) && !defined(IGNORE_PIN_PC04)
    { MP_ROM_QSTR(MP_QSTR_PC04), MP_ROM_PTR(&pin_PC04) },
    #endif
    #if defined(PIN_PC05) && !defined(IGNORE_PIN_PC05)
    { MP_ROM_QSTR(MP_QSTR_PC05), MP_ROM_PTR(&pin_PC05) },
    #endif
    #if defined(PIN_PC06) && !defined(IGNORE_PIN_PC06)
    { MP_ROM_QSTR(MP_QSTR_PC06), MP_ROM_PTR(&pin_PC06) },
    #endif
    #if defined(PIN_PC07) && !defined(IGNORE_PIN_PC07)
    { MP_ROM_QSTR(MP_QSTR_PC07), MP_ROM_PTR(&pin_PC07) },
    #endif
    #if defined(PIN_PC10) && !defined(IGNORE_PIN_PC10)
    { MP_ROM_QSTR(MP_QSTR_PC10), MP_ROM_PTR(&pin_PC10) },
    #endif
    #if defined(PIN_PC11) && !defined(IGNORE_PIN_PC11)
    { MP_ROM_QSTR(MP_QSTR_PC11), MP_ROM_PTR(&pin_PC11) },
    #endif
    #if defined(PIN_PC12) && !defined(IGNORE_PIN_PC12)
    { MP_ROM_QSTR(MP_QSTR_PC12), MP_ROM_PTR(&pin_PC12) },
    #endif
    #if defined(PIN_PC13) && !defined(IGNORE_PIN_PC13)
    { MP_ROM_QSTR(MP_QSTR_PC13), MP_ROM_PTR(&pin_PC13) },
    #endif
    #if defined(PIN_PC14) && !defined(IGNORE_PIN_PC14)
    { MP_ROM_QSTR(MP_QSTR_PC14), MP_ROM_PTR(&pin_PC14) },
    #endif
    #if defined(PIN_PC15) && !defined(IGNORE_PIN_PC15)
    { MP_ROM_QSTR(MP_QSTR_PC15), MP_ROM_PTR(&pin_PC15) },
    #endif
    #if defined(PIN_PC16) && !defined(IGNORE_PIN_PC16)
    { MP_ROM_QSTR(MP_QSTR_PC16), MP_ROM_PTR(&pin_PC16) },
    #endif
    #if defined(PIN_PC17) && !defined(IGNORE_PIN_PC17)
    { MP_ROM_QSTR(MP_QSTR_PC17), MP_ROM_PTR(&pin_PC17) },
    #endif
    #if defined(PIN_PC18) && !defined(IGNORE_PIN_PC18)
    { MP_ROM_QSTR(MP_QSTR_PC18), MP_ROM_PTR(&pin_PC18) },
    #endif
    #if defined(PIN_PC19) && !defined(IGNORE_PIN_PC19)
    { MP_ROM_QSTR(MP_QSTR_PC19), MP_ROM_PTR(&pin_PC19) },
    #endif
    #if defined(PIN_PC20) && !defined(IGNORE_PIN_PC20)
    { MP_ROM_QSTR(MP_QSTR_PC20), MP_ROM_PTR(&pin_PC20) },
    #endif
    #if defined(PIN_PC21) && !defined(IGNORE_PIN_PC21)
    { MP_ROM_QSTR(MP_QSTR_PC21), MP_ROM_PTR(&pin_PC21) },
    #endif
    #if defined(PIN_PC22) && !defined(IGNORE_PIN_PC22)
    { MP_ROM_QSTR(MP_QSTR_PC22), MP_ROM_PTR(&pin_PC22) },
    #endif
    #if defined(PIN_PC23) && !defined(IGNORE_PIN_PC23)
    { MP_ROM_QSTR(MP_QSTR_PC23), MP_ROM_PTR(&pin_PC23) },
    #endif
    #if defined(PIN_PC24) && !defined(IGNORE_PIN_PC24)
    { MP_ROM_QSTR(MP_QSTR_PC24), MP_ROM_PTR(&pin_PC24) },
    #endif
    #if defined(PIN_PC25) && !defined(IGNORE_PIN_PC25)
    { MP_ROM_QSTR(MP_QSTR_PC25), MP_ROM_PTR(&pin_PC25) },
    #endif
    #if defined(PIN_PC26) && !defined(IGNORE_PIN_PC26)
    { MP_ROM_QSTR(MP_QSTR_PC26), MP_ROM_PTR(&pin_PC26) },
    #endif
    #if defined(PIN_PC27) && !defined(IGNORE_PIN_PC27)
    { MP_ROM_QSTR(MP_QSTR_PC27), MP_ROM_PTR(&pin_PC27) },
    #endif
    #if defined(PIN_PC28) && !defined(IGNORE_PIN_PC28)
    { MP_ROM_QSTR(MP_QSTR_PC28), MP_ROM_PTR(&pin_PC28) },
    #endif
    #if defined(PIN_PC30) && !defined(IGNORE_PIN_PC30)
    { MP_ROM_QSTR(MP_QSTR_PC30), MP_ROM_PTR(&pin_PC30) },
    #endif
    #if defined(PIN_PC31) && !defined(IGNORE_PIN_PC31)
    { MP_ROM_QSTR(MP_QSTR_PC31), MP_ROM_PTR(&pin_PC31) },
    #endif

    #if defined(PIN_PD01) && !defined(IGNORE_PIN_PD01)
    { MP_ROM_QSTR(MP_QSTR_PD01), MP_ROM_PTR(&pin_PD01) },
    #endif
    #if defined(PIN_PD02) && !defined(IGNORE_PIN_PD02)
    { MP_ROM_QSTR(MP_QSTR_PD02), MP_ROM_PTR(&pin_PD02) },
    #endif
    #if defined(PIN_PD03) && !defined(IGNORE_PIN_PD03)
    { MP_ROM_QSTR(MP_QSTR_PD03), MP_ROM_PTR(&pin_PD03) },
    #endif
    #if defined(PIN_PD04) && !defined(IGNORE_PIN_PD04)
    { MP_ROM_QSTR(MP_QSTR_PD04), MP_ROM_PTR(&pin_PD04) },
    #endif
    #if defined(PIN_PD05) && !defined(IGNORE_PIN_PD05)
    { MP_ROM_QSTR(MP_QSTR_PD05), MP_ROM_PTR(&pin_PD05) },
    #endif
    #if defined(PIN_PD06) && !defined(IGNORE_PIN_PD06)
    { MP_ROM_QSTR(MP_QSTR_PD06), MP_ROM_PTR(&pin_PD06) },
    #endif
    #if defined(PIN_PD07) && !defined(IGNORE_PIN_PD07)
    { MP_ROM_QSTR(MP_QSTR_PD07), MP_ROM_PTR(&pin_PD07) },
    #endif
    #if defined(PIN_PD08) && !defined(IGNORE_PIN_PD08)
    { MP_ROM_QSTR(MP_QSTR_PD08), MP_ROM_PTR(&pin_PD08) },
    #endif
    #if defined(PIN_PD09) && !defined(IGNORE_PIN_PD09)
    { MP_ROM_QSTR(MP_QSTR_PD09), MP_ROM_PTR(&pin_PD09) },
    #endif

    #if defined(PIN_PE01) && !defined(IGNORE_PIN_PE01)
    { MP_ROM_QSTR(MP_QSTR_PD01), MP_ROM_PTR(&pin_PD02) },
    #endif
    #if defined(PIN_PE02) && !defined(IGNORE_PIN_PD02)
    { MP_ROM_QSTR(MP_QSTR_PD01), MP_ROM_PTR(&pin_PD02) },
    #endif

};
MP_DEFINE_CONST_DICT(mcu_pin_globals, mcu_pin_global_dict_table);


/** NOTE: Not implemented yet */
// #if CIRCUITPY_INTERNAL_NVM_SIZE > 0
// // The singleton nvm.ByteArray object.
// const nvm_bytearray_obj_t common_hal_mcu_nvm_obj = {
//     .base = {
//         .type = &nvm_bytearray_type,
//     },
//     .len = NVM_BYTEARRAY_BUFFER_SIZE,
//     .start_address = (uint8_t *)(CIRCUITPY_INTERNAL_NVM_START_ADDR)
// };
// #endif
