// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2024 Mark Komus
//
// SPDX-License-Identifier: MIT

#include <stdint.h>

#include "shared-bindings/audiodelays/Echo.h"
#include "shared-module/audiodelays/Echo.h"

#include "shared/runtime/context_manager_helpers.h"
#include "py/binary.h"
#include "py/objproperty.h"
#include "py/runtime.h"
#include "shared-bindings/util.h"

#define DECAY_DEFAULT 0.7f
#define MIX_DEFAULT 0.5f

//| class Echo:
//|     """An Echo effect"""
//|
static mp_obj_t audiodelays_echo_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    enum { ARG_delay_ms, ARG_decay, ARG_mix, ARG_buffer_size, ARG_sample_rate, ARG_bits_per_sample, ARG_samples_signed, ARG_channel_count, };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_delay_ms, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 50 } },
        { MP_QSTR_decay, MP_ARG_OBJ | MP_ARG_KW_ONLY,  {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_mix, MP_ARG_OBJ | MP_ARG_KW_ONLY,  {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_buffer_size, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 1024} },
        { MP_QSTR_sample_rate, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 8000} },
        { MP_QSTR_bits_per_sample, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 16} },
        { MP_QSTR_samples_signed, MP_ARG_BOOL | MP_ARG_KW_ONLY, {.u_bool = true} },
        { MP_QSTR_channel_count, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 1 } },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_int_t delay_ms = mp_arg_validate_int_range(args[ARG_delay_ms].u_int, 1, 4000, MP_QSTR_delay_ms);

    mp_float_t decay = (args[ARG_decay].u_obj == MP_OBJ_NULL)
        ? (mp_float_t)DECAY_DEFAULT
        : mp_obj_get_float(args[ARG_decay].u_obj);
    mp_arg_validate_float_range(decay, 0.0f, 1.0f, MP_QSTR_decay);

    mp_float_t mix = (args[ARG_mix].u_obj == MP_OBJ_NULL)
        ? (mp_float_t)MIX_DEFAULT
        : mp_obj_get_float(args[ARG_mix].u_obj);
    mp_arg_validate_float_range(mix, 0.0f, 1.0f, MP_QSTR_mix);

    mp_int_t channel_count = mp_arg_validate_int_range(args[ARG_channel_count].u_int, 1, 2, MP_QSTR_channel_count);
    mp_int_t sample_rate = mp_arg_validate_int_min(args[ARG_sample_rate].u_int, 1, MP_QSTR_sample_rate);
    mp_int_t bits_per_sample = args[ARG_bits_per_sample].u_int;
    if (bits_per_sample != 8 && bits_per_sample != 16) {
        mp_raise_ValueError(MP_ERROR_TEXT("bits_per_sample must be 8 or 16"));
    }

    audiodelays_echo_obj_t *self = mp_obj_malloc(audiodelays_echo_obj_t, &audiodelays_echo_type);
    common_hal_audiodelays_echo_construct(self, delay_ms, decay, mix, args[ARG_buffer_size].u_int, bits_per_sample, args[ARG_samples_signed].u_bool, channel_count, sample_rate);

    return MP_OBJ_FROM_PTR(self);
}

//|     def deinit(self) -> None:
//|         """Deinitialises the Echo and releases any hardware resources for reuse."""
//|         ...
static mp_obj_t audiodelays_echo_deinit(mp_obj_t self_in) {
    audiodelays_echo_obj_t *self = MP_OBJ_TO_PTR(self_in);
    common_hal_audiodelays_echo_deinit(self);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(audiodelays_echo_deinit_obj, audiodelays_echo_deinit);

static void check_for_deinit(audiodelays_echo_obj_t *self) {
    if (common_hal_audiodelays_echo_deinited(self)) {
        raise_deinited_error();
    }
}

//|     def __enter__(self) -> Echo:
//|         """No-op used by Context Managers."""
//|         ...
//  Provided by context manager helper.

//|     def __exit__(self) -> None:
//|         """Automatically deinitializes the hardware when exiting a context. See
//|         :ref:`lifetime-and-contextmanagers` for more info."""
//|         ...
static mp_obj_t audiodelays_echo_obj___exit__(size_t n_args, const mp_obj_t *args) {
    (void)n_args;
    common_hal_audiodelays_echo_deinit(args[0]);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(audiodelays_echo___exit___obj, 4, 4, audiodelays_echo_obj___exit__);


//|     delay_ms: int
//|     """Delay of the echo in microseconds. (read-only)"""
//|
static mp_obj_t audiodelays_echo_obj_get_delay_ms(mp_obj_t self_in) {
    audiodelays_echo_obj_t *self = MP_OBJ_TO_PTR(self_in);

    return mp_obj_new_float(common_hal_audiodelays_echo_get_delay_ms(self));

}
MP_DEFINE_CONST_FUN_OBJ_1(audiodelays_echo_get_delay_ms_obj, audiodelays_echo_obj_get_delay_ms);

static mp_obj_t audiodelays_echo_obj_set_delay_ms(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_delay_ms };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_delay_ms, MP_ARG_INT | MP_ARG_REQUIRED, {} },
    };
    audiodelays_echo_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_int_t delay_ms = mp_arg_validate_int_range(args[ARG_delay_ms].u_int, 1, 4000, MP_QSTR_delay_ms);

    common_hal_audiodelays_echo_set_delay_ms(self, delay_ms);

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(audiodelays_echo_set_delay_ms_obj, 1, audiodelays_echo_obj_set_delay_ms);

MP_PROPERTY_GETSET(audiodelays_echo_delay_ms_obj,
    (mp_obj_t)&audiodelays_echo_get_delay_ms_obj,
    (mp_obj_t)&audiodelays_echo_set_delay_ms_obj);

//|     decay: float
//|     """The rate the echo decays between 0 and 1."""
static mp_obj_t audiodelays_echo_obj_get_decay(mp_obj_t self_in) {
    return mp_obj_new_float(common_hal_audiodelays_echo_get_decay(self_in));
}
MP_DEFINE_CONST_FUN_OBJ_1(audiodelays_echo_get_decay_obj, audiodelays_echo_obj_get_decay);

static mp_obj_t audiodelays_echo_obj_set_decay(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_decay };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_decay,     MP_ARG_OBJ | MP_ARG_REQUIRED, {} },
    };
    audiodelays_echo_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_float_t decay = mp_obj_get_float(args[ARG_decay].u_obj);
    mp_arg_validate_float_range(decay, 0.0f, 1.0f, MP_QSTR_decay);

    common_hal_audiodelays_echo_set_decay(self, decay);

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(audiodelays_echo_set_decay_obj, 1, audiodelays_echo_obj_set_decay);

MP_PROPERTY_GETSET(audiodelays_echo_decay_obj,
    (mp_obj_t)&audiodelays_echo_get_decay_obj,
    (mp_obj_t)&audiodelays_echo_set_decay_obj);




//|     mix: float
//|     """The rate the echo mix between 0 and 1."""
static mp_obj_t audiodelays_echo_obj_get_mix(mp_obj_t self_in) {
    return mp_obj_new_float(common_hal_audiodelays_echo_get_mix(self_in));
}
MP_DEFINE_CONST_FUN_OBJ_1(audiodelays_echo_get_mix_obj, audiodelays_echo_obj_get_mix);

static mp_obj_t audiodelays_echo_obj_set_mix(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_mix };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_mix,     MP_ARG_OBJ | MP_ARG_REQUIRED, {} },
    };
    audiodelays_echo_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_float_t mix = mp_obj_get_float(args[ARG_mix].u_obj);
    mp_arg_validate_float_range(mix, 0.0f, 1.0f, MP_QSTR_mix);

    common_hal_audiodelays_echo_set_mix(self, mix);

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(audiodelays_echo_set_mix_obj, 1, audiodelays_echo_obj_set_mix);

MP_PROPERTY_GETSET(audiodelays_echo_mix_obj,
    (mp_obj_t)&audiodelays_echo_get_mix_obj,
    (mp_obj_t)&audiodelays_echo_set_mix_obj);



//|     playing: bool
//|     """True when any voice is being output. (read-only)"""
static mp_obj_t audiodelays_echo_obj_get_playing(mp_obj_t self_in) {
    audiodelays_echo_obj_t *self = MP_OBJ_TO_PTR(self_in);
    check_for_deinit(self);
    return mp_obj_new_bool(common_hal_audiodelays_echo_get_playing(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(audiodelays_echo_get_playing_obj, audiodelays_echo_obj_get_playing);

MP_PROPERTY_GETTER(audiodelays_echo_playing_obj,
    (mp_obj_t)&audiodelays_echo_get_playing_obj);

//|     def play(
//|         self, sample: circuitpython_typing.AudioSample, *, voice: int = 0, loop: bool = False
//|     ) -> None:
//|         """Plays the sample once when loop=False and continuously when loop=True.
//|         Does not block. Use `playing` to block.
//|
//|         Sample must be an `audiocore.WaveFile`, `audiocore.RawSample`, `audiomixer.Mixer` or `audiomp3.MP3Decoder`.
//|
//|         The sample must match the Effect's encoding settings given in the constructor."""
//|         ...
static mp_obj_t audiodelays_echo_obj_play(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_sample, ARG_loop };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_sample,    MP_ARG_OBJ | MP_ARG_REQUIRED, {} },
        { MP_QSTR_loop,      MP_ARG_BOOL | MP_ARG_KW_ONLY, {.u_bool = false} },
    };
    audiodelays_echo_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);
    check_for_deinit(self);
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);


    mp_obj_t sample = args[ARG_sample].u_obj;
    common_hal_audiodelays_echo_play(self, sample, args[ARG_loop].u_bool);

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(audiodelays_echo_play_obj, 1, audiodelays_echo_obj_play);

//|     def stop(self) -> None:
//|         """Stops playback of the sample."""
//|         ...
//|
static mp_obj_t audiodelays_echo_obj_stop(mp_obj_t self_in) {
    audiodelays_echo_obj_t *self = MP_OBJ_TO_PTR(self_in);

    common_hal_audiodelays_echo_stop(self);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(audiodelays_echo_stop_obj, audiodelays_echo_obj_stop);



static const mp_rom_map_elem_t audiodelays_echo_locals_dict_table[] = {
    // Methods
    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&audiodelays_echo_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR___enter__), MP_ROM_PTR(&default___enter___obj) },
    { MP_ROM_QSTR(MP_QSTR___exit__), MP_ROM_PTR(&audiodelays_echo___exit___obj) },
    { MP_ROM_QSTR(MP_QSTR_play), MP_ROM_PTR(&audiodelays_echo_play_obj) },
    { MP_ROM_QSTR(MP_QSTR_stop), MP_ROM_PTR(&audiodelays_echo_stop_obj) },

    // Properties
    { MP_ROM_QSTR(MP_QSTR_playing), MP_ROM_PTR(&audiodelays_echo_playing_obj) },
    { MP_ROM_QSTR(MP_QSTR_delay_ms), MP_ROM_PTR(&audiodelays_echo_delay_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_decay), MP_ROM_PTR(&audiodelays_echo_decay_obj) },
    { MP_ROM_QSTR(MP_QSTR_mix), MP_ROM_PTR(&audiodelays_echo_mix_obj) },
};
static MP_DEFINE_CONST_DICT(audiodelays_echo_locals_dict, audiodelays_echo_locals_dict_table);

static const audiosample_p_t audiodelays_echo_proto = {
    MP_PROTO_IMPLEMENT(MP_QSTR_protocol_audiosample)
    .sample_rate = (audiosample_sample_rate_fun)common_hal_audiodelays_echo_get_sample_rate,
    .bits_per_sample = (audiosample_bits_per_sample_fun)common_hal_audiodelays_echo_get_bits_per_sample,
    .channel_count = (audiosample_channel_count_fun)common_hal_audiodelays_echo_get_channel_count,
    .reset_buffer = (audiosample_reset_buffer_fun)audiodelays_echo_reset_buffer,
    .get_buffer = (audiosample_get_buffer_fun)audiodelays_echo_get_buffer,
    .get_buffer_structure = (audiosample_get_buffer_structure_fun)audiodelays_echo_get_buffer_structure,
};

MP_DEFINE_CONST_OBJ_TYPE(
    audiodelays_echo_type,
    MP_QSTR_Echo,
    MP_TYPE_FLAG_HAS_SPECIAL_ACCESSORS,
    make_new, audiodelays_echo_make_new,
    locals_dict, &audiodelays_echo_locals_dict,
    protocol, &audiodelays_echo_proto
    );
