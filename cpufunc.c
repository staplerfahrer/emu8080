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
			(((b & 0x01) != 0)
			+ ((b & 0x02) != 0)
			+ ((b & 0x04) != 0)
			+ ((b & 0x08) != 0)
			+ ((b & 0x10) != 0)
			+ ((b & 0x20) != 0)
			+ ((b & 0x40) != 0)
			+ ((b & 0x80) != 0))
			& 0x01
		) == 0;
}
