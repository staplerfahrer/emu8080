#pragma once
#include "structs.h"

State8080 *newState(State8080 *stp, u8_t *memory);

int emulate8080Op(State8080 *s);
