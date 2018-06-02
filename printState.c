#include <stdio.h>
#include "structs.h"

void print(State8080 *state)
{
	printf("af:%04x", 
		state->a        << 8 
		/*| state->cc.z   << 7
		| state->cc.s   << 6
		| state->cc.p   << 5
		| state->cc.cy  << 4
		| state->cc.ac  << 3
		| state->cc.pad << 2*/);

	printf(" %c%c%c%c%c%c", 
		state->cc.s   ? '.' : 's', 
		state->cc.z   ? '.' : 'z', 
		state->cc.ac  ? '.' : '?', //h? 
		state->cc.pad ? '.' : '?', //h?
		state->cc.p   ? '.' : 'p', 
		state->cc.cy  ? '.' : 'c'); 

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
