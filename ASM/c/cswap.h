#ifndef CSWAP_H
#define CSWAP_H

#include "z64.h"

uint8_t cswap_get_active_slot(uint8_t index);
void cswap_set_active_slot(uint8_t index, uint8_t slot);

uint16_t cswap_get_active_set();
void cswap_set_active_set(uint16_t active_set);

uint16_t cswap_get_saved_set();
void cswap_set_saved_set(uint16_t saved_set);

#endif
