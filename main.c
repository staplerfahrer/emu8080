#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "structs.h"
#include "cpufunc.h"
#include "printState.h"

State8080 *newState(State8080 *stp, u8_t *memory)
{
	ConditionCodes conditionCodes =
		{
			.z   = 0,
			.s   = 0,
			.p   = 0,
			.cy  = 0,
			.ac  = 0,
			.pad = 0
		};

	*stp =
		(State8080) {
			.b          = 0,
			.c          = 0,
			.d          = 0,
			.e          = 0,
			.h          = 0,
			.l          = 0,
			.a          = 0,
			.sp         = 0,
			.pc         = 0,
			.memory     = memory,
			.cc         = conditionCodes,
			.int_enable = 0
		};

	return stp;
}

int emulate8080Op(State8080 *state)
{
	u8_t *opcode = &state->memory[state->pc];
	int opbytes = 1;

	// Higher precision math result
	u16_t answer = 0;
	u8_t answerChar = 0;
	u16_t offset = 0;

	switch (*opcode)
	{
		case 0x00: /* NOP */ 
			break;
		case 0x01: /* LXI B, D16 */ 
			state->b = opcode[2]; 
			state->c = opcode[1]; 
			opbytes = 3; 
			break;
		case 0x02: /* STAX B */ 
			offset = state->b << 8 | state->c;
			state->memory[offset] = state->a; 
			break;
		case 0x03: /* INX B */
			answer = (state->b << 8 | state->c) + 1;
			state->b = answer & 0xff00;
			state->c = answer & 0x00ff;
			break;
		case 0x04: /* INR B */
			answer = state->b + 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.ac = answer > 0xff;
			state->b = answerChar;
			break;
		case 0x05: /* DCR B */
			answer = state->b - 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			// if previously positive, now negative
			state->cc.ac = ((state->b & 0x80) == 0x00) && state->cc.s;
			state->b = answerChar;
			break;
		case 0x06: /* MVI B, D8 */ 
			state->b = opcode[1];
			opbytes = 2; 
			break;
		case 0x07: unimplementedInstruction(state); break;
		case 0x08: unimplementedInstruction(state); break;
		case 0x09: unimplementedInstruction(state); break;
		case 0x0a: unimplementedInstruction(state); break;
		case 0x0b: unimplementedInstruction(state); break;
		case 0x0c: /* INR C */
			answer = state->c + 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.ac = answer > 0xff;
			state->c = answerChar;
			break;
		case 0x0d: /* DCR C */
			answer = state->c - 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			// if previously positive, now negative
			state->cc.ac = ((state->c & 0x80) == 0x00) && state->cc.s;
			state->c = answerChar;
			break;
		case 0x0e: unimplementedInstruction(state); opbytes = 2; break;
		case 0x0f: unimplementedInstruction(state); break;
		case 0x10: unimplementedInstruction(state); break;
		case 0x11: /* LXI D, D16 */ 
			state->d = opcode[2]; 
			state->e = opcode[1]; 
			opbytes = 3; 
			break;
		case 0x12: /* STAX D */ 
			offset = state->d << 8 | state->e;
			state->memory[offset] = state->a; 
			break;
		case 0x13: /* INX D */
			answer = (state->d << 8 | state->e) + 1;
			state->d = answer & 0xff00;
			state->e = answer & 0x00ff;
			break;
		case 0x14: /* INR D */
			answer = state->d + 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.ac = answer > 0xff;
			state->d = answerChar;
			break;
		case 0x15: /* DCR D */
			answer = state->d - 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			// if previously positive, now negative
			state->cc.ac = ((state->d & 0x80) == 0x00) && state->cc.s;
			state->d = answerChar;
			break;
		case 0x16: unimplementedInstruction(state); opbytes = 2; break;
		case 0x17: unimplementedInstruction(state); break;
		case 0x18: unimplementedInstruction(state); break;
		case 0x19: unimplementedInstruction(state); break;
		case 0x1a: unimplementedInstruction(state); break;
		case 0x1b: unimplementedInstruction(state); break;
		case 0x1c: /* INR E */
			answer = state->e + 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.ac = answer > 0xff;
			state->e = answerChar;
			break;
		case 0x1d: /* DCR E */
			answer = state->e - 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			// if previously positive, now negative
			state->cc.ac = ((state->e & 0x80) == 0x00) && state->cc.s;
			state->e = answerChar;
			break;
		case 0x1e: unimplementedInstruction(state); opbytes = 2; break;
		case 0x1f: unimplementedInstruction(state); break;
		case 0x20: unimplementedInstruction(state); break;
		case 0x21: /* LXI H, D16 */ 
			state->h = opcode[2]; 
			state->l = opcode[1]; 
			opbytes = 3; 
			break;
		case 0x22: unimplementedInstruction(state); opbytes = 3; break;
		case 0x23: /* INX H */
			answer = (state->h << 8 | state->l) + 1;
			state->h = answer & 0xff00;
			state->l = answer & 0x00ff;
			break;
		case 0x24: /* INR H */
			answer = state->h + 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.ac = answer > 0xff;
			state->h = answerChar;
			break;
		case 0x25: /* DCR H */
			answer = state->h - 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			// if previously positive, now negative
			state->cc.ac = ((state->h & 0x80) == 0x00) && state->cc.s;
			state->h = answerChar;
			break;
		case 0x26: unimplementedInstruction(state); opbytes = 2; break;
		case 0x27: unimplementedInstruction(state); break;
		case 0x28: unimplementedInstruction(state); break;
		case 0x29: unimplementedInstruction(state); break;
		case 0x2a: unimplementedInstruction(state); opbytes = 3; break;
		case 0x2b: unimplementedInstruction(state); break;
		case 0x2c: /* INR L */
			answer = state->l + 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.ac = answer > 0xff;
			state->l = answerChar;
			break;
		case 0x2d: /* DCR L */
			answer = state->l - 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			// if previously positive, now negative
			state->cc.ac = ((state->l & 0x80) == 0x00) && state->cc.s;
			state->l = answerChar;
			break;
		case 0x2e: unimplementedInstruction(state); opbytes = 2; break;
		case 0x2f: unimplementedInstruction(state); break;
		case 0x30: unimplementedInstruction(state); break;
		case 0x31: /* LXI SP, D16 */ 
			state->sp = opcode[2] << 8 | opcode[1]; 
			opbytes = 3; 
			break;
		case 0x32: unimplementedInstruction(state); opbytes = 3; break;
		case 0x33: /* INX SP */
			state->sp++;
			break;
		case 0x34: /* INR M */
			offset = state->h << 8 | state->l;
			answer = state->memory[offset] + 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.ac = answer > 0xff;
			state->memory[offset] = answerChar;
			break;
		case 0x35: /* DCR M */
			offset = state->h << 8 | state->l;
			answer = state->memory[offset] - 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			// if previously positive, now negative
			state->cc.ac = ((state->memory[offset] & 0x80) == 0x00) && state->cc.s;
			state->memory[offset] = answerChar;
			break;
		case 0x36: unimplementedInstruction(state); opbytes = 2; break;
		case 0x37: unimplementedInstruction(state); break;
		case 0x38: unimplementedInstruction(state); break;
		case 0x39: unimplementedInstruction(state); break;
		case 0x3a: unimplementedInstruction(state); opbytes = 3; break;
		case 0x3b: unimplementedInstruction(state); break;
		case 0x3c: /* INR A */
			answer = state->a + 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.ac = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x3d: /* DCR A */
			answer = state->a - 1;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			// if previously positive, now negative
			state->cc.ac = ((state->a & 0x80) == 0x00) && state->cc.s;
			state->a = answerChar;
			break;
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
		case 0x46: /* MOV B, M */
			offset = state->h << 8 | state->l;
			state->b = state->memory[offset];
			break;
		case 0x47: /* MOV B, A */
			state->b = state->a;
			break;
		case 0x48: /* MOV C, B */
			state->c = state->b;
			break;
		case 0x49: /* MOV C, C */
			state->c = state->c;
			break;
		case 0x4a: /* MOV C, D */
			state->c = state->d;
			break;
		case 0x4b: /* MOV C, E */
			state->c = state->e;
			break;
		case 0x4c: /* MOV C, H */
			state->c = state->h;
			break;
		case 0x4d: /* MOV C, L */
			state->c = state->l;
			break;
		case 0x4e: /* MOV C, M */
			offset = state->h << 8 | state->l;
			state->c = state->memory[offset];
			break;
		case 0x4f: /* MOV C, A */
			state->c = state->a;
			break;
		case 0x50: /* MOV D, B */
			state->d = state->b;
			break;
		case 0x51: /* MOV D, C */
			state->d = state->c;
			break;
		case 0x52: /* MOV D, D */
			state->d = state->d;
			break;
		case 0x53: /* MOV D, E */
			state->d = state->e;
			break;
		case 0x54: /* MOV D, H */
			state->d = state->h;
			break;
		case 0x55: /* MOV D, L */
			state->d = state->l;
			break;
		case 0x56: /* MOV D, M */
			offset = state->h << 8 | state->l;
			state->d = state->memory[offset];
			break;
		case 0x57: /* MOV D, A */
			state->d = state->a;
			break;
		case 0x58: /* MOV E, B */
			state->e = state->b;
			break;
		case 0x59: /* MOV E, C */
			state->e = state->c;
			break;
		case 0x5a: /* MOV E, D */
			state->e = state->d;
			break;
		case 0x5b: /* MOV E, E */
			state->e = state->e;
			break;
		case 0x5c: /* MOV E, H */
			state->e = state->h;
			break;
		case 0x5d: /* MOV E, L */
			state->e = state->l;
			break;
		case 0x5e: /* MOV E, M */
			offset = state->h << 8 | state->l;
			state->e = state->memory[offset];
			break;
		case 0x5f: /* MOV E, A */
			state->e = state->a;
			break;
		case 0x60: /* MOV H, B */
			state->h = state->b;
			break;
		case 0x61: /* MOV H, C */
			state->h = state->c;
			break;
		case 0x62: /* MOV H, D */
			state->h = state->d;
			break;
		case 0x63: /* MOV H, E */
			state->h = state->e;
			break;
		case 0x64: /* MOV H, H */
			state->h = state->h;
			break;
		case 0x65: /* MOV H, L */
			state->h = state->l;
			break;
		case 0x66: /* MOV H, M */
			offset = state->h << 8 | state->l;
			state->h = state->memory[offset];
			break;
		case 0x67: /* MOV H, A */
			state->h = state->a;
			break;
		case 0x68: /* MOV L, B */
			state->l = state->b;
			break;
		case 0x69: /* MOV L, C */
			state->l = state->c;
			break;
		case 0x6a: /* MOV L, D */
			state->l = state->d;
			break;
		case 0x6b: /* MOV L, E */
			state->l = state->e;
			break;
		case 0x6c: /* MOV L, H */
			state->l = state->h;
			break;
		case 0x6d: /* MOV L, L */
			state->l = state->l;
			break;
		case 0x6e: /* MOV L, M */
			offset = state->h << 8 | state->l;
			state->l = state->memory[offset];
			break;
		case 0x6f: /* MOV L, A */
			state->l = state->a;
			break;
		case 0x70: /* MOV M, B */
			offset = state->h << 8 | state->l;
			state->memory[offset] = state->b;
			break;
		case 0x71: /* MOV M, C */
			offset = state->h << 8 | state->l;
			state->memory[offset] = state->c;
			break;
		case 0x72: /* MOV M, D */
			offset = state->h << 8 | state->l;
			state->memory[offset] = state->d;
			break;
		case 0x73: /* MOV M, E */
			offset = state->h << 8 | state->l;
			state->memory[offset] = state->e;
			break;
		case 0x74: /* MOV M, H */
			offset = state->h << 8 | state->l;
			state->memory[offset] = state->h;
			break;
		case 0x75: /* MOV M, L */
			offset = state->h << 8 | state->l;
			state->memory[offset] = state->l;
			break;
		case 0x76: /* HLT */
			// "SPECIAL"
			break;
		case 0x77: /* MOV M, A */
			offset = state->h << 8 | state->l;
			state->memory[offset] = state->a;
			break;
		case 0x78: /* MOV A, B */
			state->a = state->b;
			break;
		case 0x79: /* MOV A, C */
			state->a = state->c;
			break;
		case 0x7a: /* MOV A, D */
			state->a = state->d;
			break;
		case 0x7b: /* MOV A, E */
			state->a = state->e;
			break;
		case 0x7c: /* MOV A, H */
			state->a = state->h;
			break;
		case 0x7d: /* MOV A, L */
			state->a = state->l;
			break;
		case 0x7e: /* MOV A, M */
			offset = state->h << 8 | state->l;
			state->a = state->memory[offset];
			break;
		case 0x7f: /* MOV A, A */
			state->a = state->a;
			break;
		case 0x80: /* ADD B */
			answer = (u16_t)state->a + state->b;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x81: /* ADD C */
			answer = (u16_t)state->a + state->c;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x82: /* ADD D */
			answer = (u16_t)state->a + state->d;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x83: /* ADD E */
			answer = (u16_t)state->a + state->e;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x84: /* ADD H */
			answer = (u16_t)state->a + state->h;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x85: /* ADD L */
			answer = (u16_t)state->a + state->l;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x86: /* ADD M (add byte at HL offset) */
			offset = state->h << 8 | state->l;
			answer = (u16_t)state->a + state->memory[offset];
			answerChar = answer & 0xff; 
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.cy = answer > 0xff;
			state->cc.p = parity(answerChar);
			state->a = answerChar;
			break;
		case 0x87: /* ADD A */
			answer = (u16_t)state->a + state->a;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x88: /* ADC B */
			answer = (u16_t)state->a + state->b + state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x89: /* ADC C */
			answer = (u16_t)state->a + state->c + state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x8a: /* ADC D */
			answer = (u16_t)state->a + state->d + state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x8b: /* ADC E */
			answer = (u16_t)state->a + state->e + state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x8c: /* ADC H */
			answer = (u16_t)state->a + state->h + state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x8d: /* ADC L */
			answer = (u16_t)state->a + state->l + state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x8e: /* ADC M */
			offset = state->h << 8 | state->l;
			answer = (u16_t)state->a + state->memory[offset] + state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x8f: /* ADC A */
			answer = (u16_t)state->a + state->a + state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = answer > 0xff;
			state->a = answerChar;
			break;
		case 0x90: /* SUB B */
			answerChar = state->a - state->b;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = state->a < state->b;
			state->a = answerChar;
			break;
		case 0x91: /* SUB C */
			answerChar = state->a - state->c;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = state->a < state->c;
			state->a = answerChar;
			break;
		case 0x92: /* SUB D */
			answerChar = state->a - state->d;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = state->a < state->d;
			state->a = answerChar;
			break;
		case 0x93: /* SUB E */
			answerChar = state->a - state->e;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = state->a < state->e;
			state->a = answerChar;
			break;
		case 0x94: /* SUB H */
			answerChar = state->a - state->h;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = state->a < state->h;
			state->a = answerChar;
			break;
		case 0x95: /* SUB L */
			answerChar = state->a - state->l;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = state->a < state->l;
			state->a = answerChar;
			break;
		case 0x96: /* SUB M */
			offset = state->h << 8 | state->l;
			answerChar = state->a - state->memory[offset];
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = state->a < state->l;
			state->a = answerChar;
			break;
		case 0x97: /* SUB A */
			answerChar = state->a - state->a;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = 0;
			state->a = answerChar;
			break;
		case 0x98: /* SBB B */
			answer = (u16_t)state->a - state->b - state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = (u16_t)state->a < ((u16_t)state->b + state->cc.cy);
			state->a = answerChar;
			break;
		case 0x99: /* SBB C */
			answer = (u16_t)state->a - state->c - state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = (u16_t)state->a < ((u16_t)state->c + state->cc.cy);
			state->a = answerChar;
			break;
		case 0x9a: /* SBB D */
			answer = (u16_t)state->a - state->d - state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = (u16_t)state->a < ((u16_t)state->d + state->cc.cy);
			state->a = answerChar;
			break;
		case 0x9b: /* SBB E */
			answer = (u16_t)state->a - state->e - state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = (u16_t)state->a < ((u16_t)state->e + state->cc.cy);
			state->a = answerChar;
			break;
		case 0x9c: /* SBB H */
			answer = (u16_t)state->a - state->h - state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = (u16_t)state->a < ((u16_t)state->h + state->cc.cy);
			state->a = answerChar;
			break;
		case 0x9d: /* SBB L */
			answer = (u16_t)state->a - state->l - state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = (u16_t)state->a < ((u16_t)state->l + state->cc.cy);
			state->a = answerChar;
			break;
		case 0x9e: /* SBB M */
			offset = state->h << 8 | state->l;
			answer = (u16_t)state->a - state->memory[offset] - state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = (u16_t)state->a < ((u16_t)state->memory[offset] + state->cc.cy);
			state->a = answerChar;
			break;
		case 0x9f: /* SBB A */
			answer = -state->cc.cy;
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answerChar & 0x80) == 0x80;
			state->cc.p = parity(answerChar);
			state->cc.cy = state->cc.cy;
			state->a = answerChar;
			break;
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
		case 0xc6: /* ADI byte */
			answer = (u16_t) state-> a +
				opcode[1];
			answerChar = answer & 0xff;
			state->cc.z = answerChar == 0;
			state->cc.s = (answer & 0x80) == 0x80;
			state->cc.cy = answer > 0xff;
			state->cc.p = parity(answerChar);
			state->a = answerChar;
			opbytes = 2; 
			break;
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