// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2019 Dan Halbert for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#ifndef MICROPY_INCLUDED_NRF_COMMON_HAL_BLEIO_CHARACTERISTICBUFFER_H
#define MICROPY_INCLUDED_NRF_COMMON_HAL_BLEIO_CHARACTERISTICBUFFER_H

#include <stdbool.h>

#include "nrf_soc.h"

#include "py/ringbuf.h"
#include "shared-bindings/_bleio/Characteristic.h"

typedef struct {
    mp_obj_base_t base;
    bleio_characteristic_obj_t *characteristic;
    uint32_t timeout_ms;
    // Ring buffer storing consecutive incoming values.
    ringbuf_t ringbuf;
    bool watch_for_interrupt_char;
} bleio_characteristic_buffer_obj_t;

#endif // MICROPY_INCLUDED_NRF_COMMON_HAL_BLEIO_CHARACTERISTICBUFFER_H