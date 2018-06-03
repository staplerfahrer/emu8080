#include <stdio.h>
#include "structs.h"

void print(State8080 *state)
{
/*
	s 7
	z 6
	.
	ac 4
	.
	p 2
	.
	cy 0
*/


	printf("af:%04x", 
		state->a        << 8 
		| state->cc.s   << 7
		| state->cc.z   << 6
		| state->cc.ac  << 4
		| state->cc.p   << 2
		| state->cc.cy
		);

	printf(" %c%c%c%c%c%c%c%c", 
		state->cc.s   ? 's' : '.', 
		state->cc.z   ? 'z' : '.', 
		'_', // undefined
		state->cc.ac  ? 'a' : '.', //h in js emulator? 
		'_', // undefined
		state->cc.p   ? 'p' : '.', 
		'_', // undefined
		state->cc.cy  ? 'c' : '.'); 

	printf(" bc:%04x de:%04x hl:%04x (hl):%02x sp:%04x pc:%04x (pc):%02x",
		state->b << 8 | state->c, 
		state->d << 8 | state->e, 
		state->h << 8 | state->l,
		state->memory[state->h << 8 | state->l],
		state->sp, 
		state->pc,
		state->memory[state->pc]);

	printf("  iE:%d\n", state->int_enable);
}
