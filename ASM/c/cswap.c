#include "cswap.h"

#define CSWAP_SCENE_ID (0x48)

uint8_t cswap_get_active_slot(uint8_t index) {
    uint8_t slot = z64_file.c_button_slots[index];
    if (slot == 0x03) { // Bow
        uint8_t item = z64_file.button_items[index + 1];
        if (item == 0x38) { // & FA
            return 0x04;
        } else if (item == 0x39) { // & IA
            return 0x0A;
        } else if (item == 0x3A) { // & LA
            return 0x10;
        }
    }

    return slot & 0x1F;
}

void cswap_set_active_slot(uint8_t index, uint8_t slot) {
    uint8_t item;

    slot &= 0x1F;
    if (slot >= 24) {
        slot = 0xFF;
        item = 0xFF;
    } else {
        item = z64_file.items[slot];
        if (item == 0xFF) {
            slot = 0xFF;
        } else if (item == 0x04) { // Fire Arrows
            slot = 0x03;
            item = 0x38; // Bow & FA
        } else if (item == 0x0C) { // Ice Arrows
            slot = 0x03;
            item = 0x39; // Bow & IA
        } else if (item == 0x12) { // Light Arrows
            slot = 0x03;
            item = 0x3A; // Bow & LA
        }
    }

    z64_file.c_button_slots[index] = slot;
    z64_file.button_items[index + 1] = item;

    z64_UpdateItemButton(&z64_game, index + 1);
}

uint16_t cswap_get_active_set() {
    return (cswap_get_active_slot(0) << 10) | (cswap_get_active_slot(1) << 5) | cswap_get_active_slot(2);
}

void cswap_set_active_set(uint16_t active_set) {
    if (!active_set) {
        // 0 means (Deku Sticks, Deku Sticks, Deku Sticks) and a set with the same non-null slot 3 times is invalid anyway.
        active_set = 0x7FFF;
    }
    cswap_set_active_slot(0, active_set >> 10);
    cswap_set_active_slot(1, active_set >> 5);
    cswap_set_active_slot(2, active_set);
}

uint16_t cswap_get_saved_set() {
    uint32_t bit_offset = z64_file.link_age ? 16 : 0;
    uint32_t bit_mask = 0xFFFF << bit_offset;
    uint32_t saved_sets = z64_file.scene_flags[CSWAP_SCENE_ID].unk_00_;

    uint16_t saved_set = (uint16_t)((saved_sets & bit_mask) >> bit_offset);
    if (!saved_set) {
        return 0x7FFF;
    }
    return saved_set;
}

void cswap_set_saved_set(uint16_t saved_set) {
    uint32_t bit_offset = z64_file.link_age ? 16 : 0;
    uint32_t bit_mask = 0xFFFF << bit_offset;
    uint32_t saved_sets = z64_file.scene_flags[CSWAP_SCENE_ID].unk_00_;
    saved_sets = (saved_sets & ~bit_mask) | ((uint32_t)saved_set) << bit_offset;
    z64_file.scene_flags[CSWAP_SCENE_ID].unk_00_ = saved_sets;
}
