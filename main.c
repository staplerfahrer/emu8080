#include <stdio.h>
#include <stdlib.h>


typedef struct ConditionCodes
{
	unsigned char z:1;
	unsigned char s:1;
	unsigned char p:1;
	unsigned char cy:1;
	unsigned char ac:1;
	unsigned char pad:1;
} ConditionCodes;

typedef struct State8080
{
	unsigned char b;
	unsigned char c;
	unsigned char d;
	unsigned char e;
	unsigned char h;
	unsigned char l;
	unsigned char a;
	short unsigned int sp;
	short unsigned int pc;
	unsigned char *memory;
	ConditionCodes cc;
	unsigned char int_enable;
} State8080;

void unimplementedInstruction(State8080 *state)
{
	// "pc will have advanced one, so undo that"
	printf("Error: unimplemented instruction at PC %04x.\n",
		state->pc);
	exit(1);
}

unsigned char parity(unsigned char b)
{
	unsigned char ctr = 0;
	ctr += (b & 0x01) != 0;
	ctr += (b & 0x02) != 0;
	ctr += (b & 0x04) != 0;
	ctr += (b & 0x08) != 0;
	ctr += (b & 0x10) != 0;
	ctr += (b & 0x20) != 0;
	ctr += (b & 0x40) != 0;
	ctr += (b & 0x80) != 0;
	return (ctr & 0x01) == 0;
}

int emulate8080Op(State8080 *state)
{
	unsigned char *opcode = &state->memory[state->pc];
	int opbytes = 1;

	// Higher precision math result
	short unsigned int answer = 0;
	unsigned char answerChar = 0;

	switch (*opcode)
	{
		case 0x00: /* NOP */ 
			break;
		case 0x01: /* LXI B, D16 */ 
			state->b = opcode[2]; 
			state->c = opcode[1]; 
			opbytes = 3; 
			break;
		case 0x02: 
			/* STAX */ 
			unimplementedInstruction(state); 
			break;
		case 0x03: unimplementedInstruction(state); break;
		case 0x04: unimplementedInstruction(state); break;
		case 0x05: unimplementedInstruction(state); break;
		case 0x06: unimplementedInstruction(state); opbytes = 2; break;
		case 0x07: unimplementedInstruction(state); break;
		case 0x08: unimplementedInstruction(state); break;
		case 0x09: unimplementedInstruction(state); break;
		case 0x0a: unimplementedInstruction(state); break;
		case 0x0b: unimplementedInstruction(state); break;
		case 0x0c: unimplementedInstruction(state); break;
		case 0x0d: unimplementedInstruction(state); break;
		case 0x0e: unimplementedInstruction(state); opbytes = 2; break;
		case 0x0f: unimplementedInstruction(state); break;
		case 0x10: unimplementedInstruction(state); break;
		case 0x11: unimplementedInstruction(state); opbytes = 3; break;
		case 0x12: unimplementedInstruction(state); break;
		case 0x13: unimplementedInstruction(state); break;
		case 0x14: unimplementedInstruction(state); break;
		case 0x15: unimplementedInstruction(state); break;
		case 0x16: unimplementedInstruction(state); opbytes = 2; break;
		case 0x17: unimplementedInstruction(state); break;
		case 0x18: unimplementedInstruction(state); break;
		case 0x19: unimplementedInstruction(state); break;
		case 0x1a: unimplementedInstruction(state); break;
		case 0x1b: unimplementedInstruction(state); break;
		case 0x1c: unimplementedInstruction(state); break;
		case 0x1d: unimplementedInstruction(state); break;
		case 0x1e: unimplementedInstruction(state); opbytes = 2; break;
		case 0x1f: unimplementedInstruction(state); break;
		case 0x20: unimplementedInstruction(state); break;
		case 0x21: unimplementedInstruction(state); opbytes = 3; break;
		case 0x22: unimplementedInstruction(state); opbytes = 3; break;
		case 0x23: unimplementedInstruction(state); break;
		case 0x24: unimplementedInstruction(state); break;
		case 0x25: unimplementedInstruction(state); break;
		case 0x26: unimplementedInstruction(state); opbytes = 2; break;
		case 0x27: unimplementedInstruction(state); break;
		case 0x28: unimplementedInstruction(state); break;
		case 0x29: unimplementedInstruction(state); break;
		case 0x2a: unimplementedInstruction(state); opbytes = 3; break;
		case 0x2b: unimplementedInstruction(state); break;
		case 0x2c: unimplementedInstruction(state); break;
		case 0x2d: unimplementedInstruction(state); break;
		case 0x2e: unimplementedInstruction(state); opbytes = 2; break;
		case 0x2f: unimplementedInstruction(state); break;
		case 0x30: unimplementedInstruction(state); break;
		case 0x31: unimplementedInstruction(state); opbytes = 3; break;
		case 0x32: unimplementedInstruction(state); opbytes = 3; break;
		case 0x33: unimplementedInstruction(state); break;
		case 0x34: unimplementedInstruction(state); break;
		case 0x35: unimplementedInstruction(state); break;
		case 0x36: unimplementedInstruction(state); opbytes = 2; break;
		case 0x37: unimplementedInstruction(state); break;
		case 0x38: unimplementedInstruction(state); break;
		case 0x39: unimplementedInstruction(state); break;
		case 0x3a: unimplementedInstruction(state); opbytes = 3; break;
		case 0x3b: unimplementedInstruction(state); break;
		case 0x3c: unimplementedInstruction(state); break;
		case 0x3d: unimplementedInstruction(state); break;
		case 0x3e: unimplementedInstruction(state); opbytes = 2; break;
		case 0x3f: unimplementedInstruction(state); break;
		case 0x40: /* MOV B, B */
			state->b = state->b; 
			break;
		case 0x41: /* MOV B, C */
			state->b = state->c;
			break;
		case 0x42: /* MOV B, D */
			state->b = state->d;
			break;
		case 0x43: /* MOV B, E */
			state->b = state->e;
			break;
		case 0x44: /* MOV B, H */
			state->b = state->h;
			break;
		case 0x45: /* MOV B, L */
			state->b = state->l;
			break;
		case 0x46: unimplementedInstruction(state); break;
		case 0x47: unimplementedInstruction(state); break;
		case 0x48: unimplementedInstruction(state); break;
		case 0x49: unimplementedInstruction(state); break;
		case 0x4a: unimplementedInstruction(state); break;
		case 0x4b: unimplementedInstruction(state); break;
		case 0x4c: unimplementedInstruction(state); break;
		case 0x4d: unimplementedInstruction(state); break;
		case 0x4e: unimplementedInstruction(state); break;
		case 0x4f: unimplementedInstruction(state); break;
		case 0x50: unimplementedInstruction(state); break;
		case 0x51: unimplementedInstruction(state); break;
		case 0x52: unimplementedInstruction(state); break;
		case 0x53: unimplementedInstruction(state); break;
		case 0x54: unimplementedInstruction(state); break;
		case 0x55: unimplementedInstruction(state); break;
		case 0x56: unimplementedInstruction(state); break;
		case 0x57: unimplementedInstruction(state); break;
		case 0x58: unimplementedInstruction(state); break;
		case 0x59: unimplementedInstruction(state); break;
		case 0x5a: unimplementedInstruction(state); break;
		case 0x5b: unimplementedInstruction(state); break;
		case 0x5c: unimplementedInstruction(state); break;
		case 0x5d: unimplementedInstruction(state); break;
		case 0x5e: unimplementedInstruction(state); break;
		case 0x5f: unimplementedInstruction(state); break;
		case 0x60: unimplementedInstruction(state); break;
		case 0x61: unimplementedInstruction(state); break;
		case 0x62: unimplementedInstruction(state); break;
		case 0x63: unimplementedInstruction(state); break;
		case 0x64: unimplementedInstruction(state); break;
		case 0x65: unimplementedInstruction(state); break;
		case 0x66: unimplementedInstruction(state); break;
		case 0x67: unimplementedInstruction(state); break;
		case 0x68: unimplementedInstruction(state); break;
		case 0x69: unimplementedInstruction(state); break;
		case 0x6a: unimplementedInstruction(state); break;
		case 0x6b: unimplementedInstruction(state); break;
		case 0x6c: unimplementedInstruction(state); break;
		case 0x6d: unimplementedInstruction(state); break;
		case 0x6e: unimplementedInstruction(state); break;
		case 0x6f: unimplementedInstruction(state); break;
		case 0x70: unimplementedInstruction(state); break;
		case 0x71: unimplementedInstruction(state); break;
		case 0x72: unimplementedInstruction(state); break;
		case 0x73: unimplementedInstruction(state); break;
		case 0x74: unimplementedInstruction(state); break;
		case 0x75: unimplementedInstruction(state); break;
		case 0x76: unimplementedInstruction(state); break;
		case 0x77: unimplementedInstruction(state); break;
		case 0x78: unimplementedInstruction(state); break;
		case 0x79: unimplementedInstruction(state); break;
		case 0x7a: unimplementedInstruction(state); break;
		case 0x7b: unimplementedInstruction(state); break;
		case 0x7c: unimplementedInstruction(state); break;
		case 0x7d: unimplementedInstruction(state); break;
		case 0x7e: unimplementedInstruction(state); break;
		case 0x7f: unimplementedInstruction(state); break;
		case 0x80: /* ADD B */
			answer = (short unsigned int)state->a + (short unsigned int)state->b;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x81: /* ADD C */
			answer = (short unsigned int)state->a + (short unsigned int)state->c;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x82: unimplementedInstruction(state); break;
		case 0x83: unimplementedInstruction(state); break;
		case 0x84: unimplementedInstruction(state); break;
		case 0x85: unimplementedInstruction(state); break;
		case 0x86: unimplementedInstruction(state); break;
		case 0x87: unimplementedInstruction(state); break;
		case 0x88: unimplementedInstruction(state); break;
		case 0x89: unimplementedInstruction(state); break;
		case 0x8a: unimplementedInstruction(state); break;
		case 0x8b: unimplementedInstruction(state); break;
		case 0x8c: unimplementedInstruction(state); break;
		case 0x8d: unimplementedInstruction(state); break;
		case 0x8e: unimplementedInstruction(state); break;
		case 0x8f: unimplementedInstruction(state); break;
		case 0x90: unimplementedInstruction(state); break;
		case 0x91: unimplementedInstruction(state); break;
		case 0x92: unimplementedInstruction(state); break;
		case 0x93: unimplementedInstruction(state); break;
		case 0x94: unimplementedInstruction(state); break;
		case 0x95: unimplementedInstruction(state); break;
		case 0x96: unimplementedInstruction(state); break;
		case 0x97: unimplementedInstruction(state); break;
		case 0x98: unimplementedInstruction(state); break;
		case 0x99: unimplementedInstruction(state); break;
		case 0x9a: unimplementedInstruction(state); break;
		case 0x9b: unimplementedInstruction(state); break;
		case 0x9c: unimplementedInstruction(state); break;
		case 0x9d: unimplementedInstruction(state); break;
		case 0x9e: unimplementedInstruction(state); break;
		case 0x9f: unimplementedInstruction(state); break;
		case 0xa0: unimplementedInstruction(state); break;
		case 0xa1: unimplementedInstruction(state); break;
		case 0xa2: unimplementedInstruction(state); break;
		case 0xa3: unimplementedInstruction(state); break;
		case 0xa4: unimplementedInstruction(state); break;
		case 0xa5: unimplementedInstruction(state); break;
		case 0xa6: unimplementedInstruction(state); break;
		case 0xa7: unimplementedInstruction(state); break;
		case 0xa8: unimplementedInstruction(state); break;
		case 0xa9: unimplementedInstruction(state); break;
		case 0xaa: unimplementedInstruction(state); break;
		case 0xab: unimplementedInstruction(state); break;
		case 0xac: unimplementedInstruction(state); break;
		case 0xad: unimplementedInstruction(state); break;
		case 0xae: unimplementedInstruction(state); break;
		case 0xaf: unimplementedInstruction(state); break;
		case 0xb0: unimplementedInstruction(state); break;
		case 0xb1: unimplementedInstruction(state); break;
		case 0xb2: unimplementedInstruction(state); break;
		case 0xb3: unimplementedInstruction(state); break;
		case 0xb4: unimplementedInstruction(state); break;
		case 0xb5: unimplementedInstruction(state); break;
		case 0xb6: unimplementedInstruction(state); break;
		case 0xb7: unimplementedInstruction(state); break;
		case 0xb8: unimplementedInstruction(state); break;
		case 0xb9: unimplementedInstruction(state); break;
		case 0xba: unimplementedInstruction(state); break;
		case 0xbb: unimplementedInstruction(state); break;
		case 0xbc: unimplementedInstruction(state); break;
		case 0xbd: unimplementedInstruction(state); break;
		case 0xbe: unimplementedInstruction(state); break;
		case 0xbf: unimplementedInstruction(state); break;
		case 0xc0: unimplementedInstruction(state); break;
		case 0xc1: unimplementedInstruction(state); break;
		case 0xc2: unimplementedInstruction(state); opbytes = 3; break;
		case 0xc3: unimplementedInstruction(state); opbytes = 3; break;
		case 0xc4: unimplementedInstruction(state); opbytes = 3; break;
		case 0xc5: unimplementedInstruction(state); break;
		case 0xc6: unimplementedInstruction(state); opbytes = 2; break;
		case 0xc7: unimplementedInstruction(state); break;
		case 0xc8: unimplementedInstruction(state); break;
		case 0xc9: unimplementedInstruction(state); break;
		case 0xca: unimplementedInstruction(state); opbytes = 3; break;
		case 0xcb: unimplementedInstruction(state); break;
		case 0xcc: unimplementedInstruction(state); opbytes = 3; break;
		case 0xcd: unimplementedInstruction(state); opbytes = 3; break;
		case 0xce: unimplementedInstruction(state); opbytes = 2; break;
		case 0xcf: unimplementedInstruction(state); break;
		case 0xd0: unimplementedInstruction(state); break;
		case 0xd1: unimplementedInstruction(state); break;
		case 0xd2: unimplementedInstruction(state); opbytes = 3; break;
		case 0xd3: unimplementedInstruction(state); opbytes = 2; break;
		case 0xd4: unimplementedInstruction(state); opbytes = 3; break;
		case 0xd5: unimplementedInstruction(state); break;
		case 0xd6: unimplementedInstruction(state); opbytes = 2; break;
		case 0xd7: unimplementedInstruction(state); break;
		case 0xd8: unimplementedInstruction(state); break;
		case 0xd9: unimplementedInstruction(state); break;
		case 0xda: unimplementedInstruction(state); opbytes = 3; break;
		case 0xdb: unimplementedInstruction(state); opbytes = 2; break;
		case 0xdc: unimplementedInstruction(state); opbytes = 3; break;
		case 0xdd: unimplementedInstruction(state); break;
		case 0xde: unimplementedInstruction(state); opbytes = 2; break;
		case 0xdf: unimplementedInstruction(state); break;
		case 0xe0: unimplementedInstruction(state); break;
		case 0xe1: unimplementedInstruction(state); break;
		case 0xe2: unimplementedInstruction(state); opbytes = 3; break;
		case 0xe3: unimplementedInstruction(state); break;
		case 0xe4: unimplementedInstruction(state); opbytes = 3; break;
		case 0xe5: unimplementedInstruction(state); break;
		case 0xe6: unimplementedInstruction(state); opbytes = 2; break;
		case 0xe7: unimplementedInstruction(state); break;
		case 0xe8: unimplementedInstruction(state); break;
		case 0xe9: unimplementedInstruction(state); break;
		case 0xea: unimplementedInstruction(state); opbytes = 3; break;
		case 0xeb: unimplementedInstruction(state); break;
		case 0xec: unimplementedInstruction(state); opbytes = 3; break;
		case 0xed: unimplementedInstruction(state); break;
		case 0xee: unimplementedInstruction(state); opbytes = 2; break;
		case 0xef: unimplementedInstruction(state); break;
		case 0xf0: unimplementedInstruction(state); break;
		case 0xf1: unimplementedInstruction(state); break;
		case 0xf2: unimplementedInstruction(state); opbytes = 3; break;
		case 0xf3: unimplementedInstruction(state); break;
		case 0xf4: unimplementedInstruction(state); opbytes = 3; break;
		case 0xf5: unimplementedInstruction(state); break;
		case 0xf6: unimplementedInstruction(state); opbytes = 2; break;
		case 0xf7: unimplementedInstruction(state); break;
		case 0xf8: unimplementedInstruction(state); break;
		case 0xf9: unimplementedInstruction(state); break;
		case 0xfa: unimplementedInstruction(state); opbytes = 3; break;
		case 0xfb: unimplementedInstruction(state); break;
		case 0xfc: unimplementedInstruction(state); opbytes = 2; break;
		case 0xfd: unimplementedInstruction(state); break;
		case 0xfe: unimplementedInstruction(state); opbytes = 2; break;
		case 0xff: unimplementedInstruction(state); break;
	};

	state->pc += opbytes;

	return 0;
}

void print(State8080 *state)
{
	printf("pc:%04x sp:%04x b:%02x c:%02x d:%02x e:%02x h:%02x l:%02x a:%02x",
		state->pc, state->sp, state->b, state->c, 
		state->d, state->e, state->h, state->l, 
		state->a);

	printf("  iE:%d\n", state->int_enable);
	
	printf("z:%d s:%d p:%d cy:%d ac:%d pad:%d\n",
		state->cc.z, state->cc.s, state->cc.p, 
		state->cc.cy, state->cc.ac, state->cc.pad);
}

int main()
{
	printf("Hello world.\n");
	unsigned char memory[65536] = 
		{0x01, 0x02, 0x03, 0x80, 0x00};

	ConditionCodes conditionCodes =
		{
			.z = 1,
			.s = 1,
			.p = 1,
			.cy = 1,
			.ac = 1,
			.pad = 1
		};

	State8080 state =
		{
			.b = 0,
			.c = 0,
			.d = 0,
			.e = 0,
			.h = 0,
			.l = 0,
			.a = 0,
			.sp = 0,
			.pc = 0,
			.memory = &memory[0],
			.cc = conditionCodes,
			.int_enable = 0
		};

	print(&state);
	for (int i = 0; i < 5; i++)
	{
		emulate8080Op(&state);
		print(&state);
	}
	return 0;
}