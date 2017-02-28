/*
 * Kinetis - EWM (External Watchdog Monitor) emulation.
 *
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <hw/cortexm/kinetis/ewm.h>
#include <hw/cortexm/kinetis/mcu.h>
#include <hw/cortexm/helper.h>
#include <hw/cortexm/svd.h>

// ----- Generated code -------------------------------------------------------
//
// ----- 8< ----- 8< -----  8< ----- 8< ----- 8< ----- 8< ----- 8< -----

// DO NOT EDIT! Automatically generated!
static void mk64f12_ewm_create_objects(Object *obj, JSON_Object *svd, const char *name)
{
    KinetisEWMState *state = KINETIS_EWM_STATE(obj);

    JSON_Object *periph = svd_get_peripheral_by_name(svd, name);
    svd_add_peripheral_properties_and_children(obj, periph, svd);

    // Registers. 
    state->u.k6.reg.ctrl = cm_object_get_child_by_name(obj, "CTRL");
    state->u.k6.reg.serv = cm_object_get_child_by_name(obj, "SERV");
    state->u.k6.reg.cmpl = cm_object_get_child_by_name(obj, "CMPL");
    state->u.k6.reg.cmph = cm_object_get_child_by_name(obj, "CMPH");

    // CTRL bitfields.
    state->u.k6.fld.ctrl.ewmen = cm_object_get_child_by_name(state->u.k6.reg.ctrl, "EWMEN");
    state->u.k6.fld.ctrl.assin = cm_object_get_child_by_name(state->u.k6.reg.ctrl, "ASSIN");
    state->u.k6.fld.ctrl.inen = cm_object_get_child_by_name(state->u.k6.reg.ctrl, "INEN");
    state->u.k6.fld.ctrl.inten = cm_object_get_child_by_name(state->u.k6.reg.ctrl, "INTEN");

    // SERV bitfields.
    state->u.k6.fld.serv.service = cm_object_get_child_by_name(state->u.k6.reg.serv, "SERVICE");

    // CMPL bitfields.
    state->u.k6.fld.cmpl.comparel = cm_object_get_child_by_name(state->u.k6.reg.cmpl, "COMPAREL");

    // CMPH bitfields.
    state->u.k6.fld.cmph.compareh = cm_object_get_child_by_name(state->u.k6.reg.cmph, "COMPAREH");
}

// ----- 8< ----- 8< -----  8< ----- 8< ----- 8< ----- 8< ----- 8< -----

// ----- Private --------------------------------------------------------------

#if 0
static peripheral_register_t kinetis_ewm_xxx_pre_write_callback(Object *reg,
        Object *periph, uint32_t addr, uint32_t offset, unsigned size,
        peripheral_register_t value, peripheral_register_t full_value)
{
    KinetisEWMState *state = KINETIS_EWM_STATE(periph);

    peripheral_register_t prev_value = peripheral_register_get_raw_prev_value(reg);

    // TODO: Add code to adjust the value (like applying some masks,
    // for example prevent setting interrupts not enabled).
    // Also process cleared/set by write 1/0 bits.
    // The returned value will be stored in the register value.

    return full_value;
}

static void kinetis_ewm_xxx_post_write_callback(Object *reg, Object *periph,
        uint32_t addr, uint32_t offset, unsigned size,
        peripheral_register_t value, peripheral_register_t full_value)
{
    KinetisEWMState *state = KINETIS_EWM_STATE(periph);

    peripheral_register_t prev_value = peripheral_register_get_raw_prev_value(reg);
    // The new register value is full_value, the old one is prev_value.

    // TODO: Add code to send the value to the consumer.
}

static peripheral_register_t kinetis_ewm_xxx_pre_read_callback(Object *reg,
        Object *periph, uint32_t addr, uint32_t offset, unsigned size)
{
    KinetisEWMState *state = KINETIS_EWM_STATE(periph);

    peripheral_register_t value = 0;

    // Add code to get the value from the producer, and return it.

    // This value, possibly masked, will be stored in the register
    // and returned when the register is read.
    return value;
}

static void kinetis_ewm_xxx_post_read_callback(Object *reg, Object *periph,
        uint32_t addr, uint32_t offset, unsigned size)
{
    KinetisEWMState *state = KINETIS_EWM_STATE(periph);

    peripheral_register_t value = peripheral_register_get_raw_value(reg);

    // TODO: add code to perform the post read actions, like clearing some bits.
}
#endif

// ----------------------------------------------------------------------------

// TODO: remove this if the peripheral is always enabled.
static bool kinetis_ewm_is_enabled(Object *obj)
{
    KinetisEWMState *state = KINETIS_EWM_STATE(obj);

    if (register_bitfield_is_non_zero(state->enabling_bit)) {
        return true; // Positive logic, bit == 1 means enabled.
    }

    // Not enabled
    return false;
}

static void kinetis_ewm_instance_init_callback(Object *obj)
{
    qemu_log_function_name();

    KinetisEWMState *state = KINETIS_EWM_STATE(obj);

    // Capabilities are not yet available.

    // TODO: remove this if the peripheral is always enabled.
    state->enabling_bit = NULL;
    
    // TODO: Add code to initialise all members.
}

static void kinetis_ewm_realize_callback(DeviceState *dev, Error **errp)
{
    qemu_log_function_name();

    // Call parent realize().
    if (!cm_device_parent_realize(dev, errp, TYPE_KINETIS_EWM)) {
        return;
    }

    KinetisMCUState *mcu = kinetis_mcu_get();
    CortexMState *cm_state = CORTEXM_MCU_STATE(mcu);

    KinetisEWMState *state = KINETIS_EWM_STATE(dev);
    // First thing first: get capabilities from MCU, needed everywhere.
    state->capabilities = mcu->capabilities;

    // Also keep a local pointer, to access them easier.
    const KinetisCapabilities *capabilities = state->capabilities;
    assert(capabilities != NULL);

    Object *obj = OBJECT(dev);

    const char *periph_name = "EWM";

    svd_set_peripheral_address_block(cm_state->svd_json, periph_name, obj);
    peripheral_create_memory_region(obj);

    // TODO: remove this if the peripheral is always enabled.
    char enabling_bit_name[KINETIS_RCC_SIZEOF_ENABLING_BITFIELD];

    switch (capabilities->family) {
    case KINETIS_FAMILY_K6:


            mk64f12_ewm_create_objects(obj, cm_state->svd_json, periph_name);

            // TODO: add actions.
            // cm_object_property_set_str(state->u.k6.fld.xxx.fff, "GGG", "follows");
            // cm_object_property_set_str(state->u.k6.fld.xxx.fff, "GGG", "cleared-by");

            // TODO: add callbacks.
            // peripheral_register_set_pre_read(state->k6.reg.xxx, &kinetis_ewm_xxx_pre_read_callback);
            // peripheral_register_set_post_read(state->k6.reg.xxx, &kinetis_ewm_xxx_post_read_callback);
            // peripheral_register_set_pre_read(state->k6.reg.xxx, &kinetis_ewm_xxx_pret_read_callback);
            // peripheral_register_set_post_write(state->k6.reg.xxx, &kinetis_ewm_xxx_post_write_callback);

            // TODO: add interrupts.

            // TODO: remove this if the peripheral is always enabled.
            snprintf(enabling_bit_name, sizeof(enabling_bit_name) - 1,
                DEVICE_PATH_KINETIS_RCC "/AHB1ENR/EWMEN");


        break;

    default:
        assert(false);
        break;
    }

    // TODO: remove this if the peripheral is always enabled.
    state->enabling_bit = OBJECT(cm_device_by_name(enabling_bit_name));

    peripheral_prepare_registers(obj);
}

static void kinetis_ewm_reset_callback(DeviceState *dev)
{
    qemu_log_function_name();

    // Call parent reset(); this will reset all children registers.
    cm_device_parent_reset(dev, TYPE_KINETIS_EWM);
}

static void kinetis_ewm_class_init_callback(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = kinetis_ewm_reset_callback;
    dc->realize = kinetis_ewm_realize_callback;

    // TODO: remove this if the peripheral is always enabled.
    PeripheralClass *per_class = PERIPHERAL_CLASS(klass);
    per_class->is_enabled = kinetis_ewm_is_enabled;
}

static const TypeInfo kinetis_ewm_type_info = {
    .name = TYPE_KINETIS_EWM,
    .parent = TYPE_KINETIS_EWM_PARENT,
    .instance_init = kinetis_ewm_instance_init_callback,
    .instance_size = sizeof(KinetisEWMState),
    .class_init = kinetis_ewm_class_init_callback,
    .class_size = sizeof(KinetisEWMClass) };

static void kinetis_ewm_register_types(void)
{
    type_register_static(&kinetis_ewm_type_info);
}

type_init(kinetis_ewm_register_types);

// ----------------------------------------------------------------------------