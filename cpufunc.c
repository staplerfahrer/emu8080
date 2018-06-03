#include <stdlib.h>
#include <stdio.h>
#include "structs.h"

void unimplementedInstruction(State8080 *state)
{
	printf("Error: unimplemented instruction at PC %04x.\n",
		state->pc);
	exit(1);
}

u8_t parity(u8_t b)
{
	return
		(
			(
				((b & 0x01) == 0x01)
				+ ((b & 0x02) == 0x02)
				+ ((b & 0x04) == 0x04)
				+ ((b & 0x08) == 0x08)
				+ ((b & 0x10) == 0x10)
				+ ((b & 0x20) == 0x20)
				+ ((b & 0x40) == 0x40)
				+ ((b & 0x80) == 0x80)
			)
			& 0x01
		) == 0;
}
