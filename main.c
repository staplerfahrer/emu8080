#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "structs.h"
#include "emulate.h"
#include "cpufunc.h"
#include "printState.h"

int main()
{
	printf("Hello world.\n");

	u8_t memory[65536] = 
		// {0x01, 0x01, 0xff, 0x80, 0x81};
		{0x06 /*mvi b*/, 0x01, 0x90 /*sub b*/};

	State8080 st = {};
	State8080 *state = newState(&st, &memory[0]);

	print(state);
	for (int i = 0; i < 5; i++)
	{
		emulate8080Op(state);
		print(state);
	}
	return 0;
}
