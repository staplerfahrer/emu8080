#include "structs.h"
#include "cpufunc.h"

int emulate8080Op(State8080 *s)
{
	u8_t *opcode = &s->memory[s->pc];
	int pcAdd = 1;

	u16_t temp16 = 0; // For carry bit
	u8_t temp8 = 0;
	u16_t addy = 0;

	switch (*opcode)
	{
		case 0x00: /* NOP */
			break;
		case 0x01: /* LXI B, D16 */
			s->b = opcode[2];
			s->c = opcode[1];
			pcAdd = 3;
			break;
		case 0x02: /* STAX B */
			addy = s->b << 8 | s->c;
			s->memory[addy] = s->a;
			break;
		case 0x03: /* INX B */
			temp16 = (s->b << 8 | s->c) + 1;
			s->b = (temp16 & 0xff00) >> 8;
			s->c = temp16 & 0x00ff;
			break;
		case 0x04: /* INR B */
			temp16 = s->b + 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.ac = temp16 > 0xff;
			s->b = temp8;
			break;
		case 0x05: /* DCR B */
			temp16 = s->b - 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// if previously positive, now negative
			s->cc.ac = ((s->b & 0x80) == 0) && s->cc.s;
			s->b = temp8;
			break;
		case 0x06: /* MVI B, D8 */
			s->b = opcode[1];
			pcAdd = 2;
			break;
		case 0x07: /* RLC */
			s->cc.cy = (s->a & 0x80) == 0x80;
			s->a = s->a << 1 | s->cc.cy;
			break;
		case 0x08: /* - */ unimpl(s); break;
		case 0x09: /* DAD B */
			temp16 = (s->h << 8 | s->l) + (s->b << 8 | s->c);
			s->cc.cy =
				((unsigned int)(s->h << 8 | s->l) + (s->b << 8 | s->c))
				> 0xffff;
			s->h = (temp16 & 0xff00) >> 8;
			s->l = temp16 & 0x00ff;
			break;
		case 0x0a: /* LDAX B */
			addy = s->b << 8 | s->c;
			s->a = s->memory[addy];
			break;
		case 0x0b: /* DCX B */
			temp16 = (s->b << 8 | s->c) - 1;
			s->b = (temp16 & 0xff00) >> 8;
			s->c = temp16 & 0x00ff;
			break;
		case 0x0c: /* INR C */
			temp16 = s->c + 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.ac = temp16 > 0xff;
			s->c = temp8;
			break;
		case 0x0d: /* DCR C */
			temp16 = s->c - 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// if previously positive, now negative
			s->cc.ac = ((s->c & 0x80) == 0) && s->cc.s;
			s->c = temp8;
			break;
		case 0x0e: /* MVI C, D8 */
			s->c = opcode[1];
			pcAdd = 2;
			break;
		case 0x0f: /* RRC */
			s->cc.cy = (s->a & 0x01) == 0x01;
			s->a = s->a >> 1 | s->cc.cy << 7;
			break;
		case 0x10: /* - */ unimpl(s); break;
		case 0x11: /* LXI D, D16 */
			s->d = opcode[2];
			s->e = opcode[1];
			pcAdd = 3;
			break;
		case 0x12: /* STAX D */
			addy = s->d << 8 | s->e;
			s->memory[addy] = s->a;
			break;
		case 0x13: /* INX D */
			temp16 = (s->d << 8 | s->e) + 1;
			s->d = (temp16 & 0xff00) >> 8;
			s->e = temp16 & 0x00ff;
			break;
		case 0x14: /* INR D */
			temp16 = s->d + 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.ac = temp16 > 0xff;
			s->d = temp8;
			break;
		case 0x15: /* DCR D */
			temp16 = s->d - 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// if previously positive, now negative
			s->cc.ac = ((s->d & 0x80) == 0) && s->cc.s;
			s->d = temp8;
			break;
		case 0x16: /* MVI D, D8 */
			s->d = opcode[1];
			pcAdd = 2;
			break;
		case 0x17: /* RAL */
			temp8 = (s->a & 0x80) == 0x80;
			s->a = s->a << 1 | s->cc.cy;
			s->cc.cy = temp8;
			break;
		case 0x18: /* - */ unimpl(s); break;
		case 0x19: /* DAD D */
			temp16 = (s->h << 8 | s->l) + (s->d << 8 | s->e);
			s->cc.cy =
				((unsigned int)(s->h << 8 | s->l) + (s->d << 8 | s->e))
				> 0xffff;
			s->h = (temp16 & 0xff00) >> 8;
			s->l = temp16 & 0x00ff;
			break;
		case 0x1a: /* LDAX D */
			addy = s->d << 8 | s->e;
			s->a = s->memory[addy];
			break;
		case 0x1b: /* DCX D */
			temp16 = (s->d << 8 | s->e) - 1;
			s->d = (temp16 & 0xff00) >> 8;
			s->e = temp16 & 0x00ff;
			break;
		case 0x1c: /* INR E */
			temp16 = s->e + 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.ac = temp16 > 0xff;
			s->e = temp8;
			break;
		case 0x1d: /* DCR E */
			temp16 = s->e - 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// if previously positive, now negative
			s->cc.ac = ((s->e & 0x80) == 0) && s->cc.s;
			s->e = temp8;
			break;
		case 0x1e: /* MVI E, D8 */
			s->e = opcode[1];
			pcAdd = 2;
			break;
		case 0x1f: /* RAR */
			s->cc.cy = (s->a & 0x01) == 0x01;
			s->a = s->a >> 1 | (s->a & 0x80);
			break;
		case 0x20: /* RIM "SPECIAL" */
			unimpl(s);
			break;
		case 0x21: /* LXI H, D16 */
			s->h = opcode[2];
			s->l = opcode[1];
			pcAdd = 3;
			break;
		case 0x22: /* SHLD adr */
			s->memory[opcode[2]<<8 | opcode[1]] = s->l;
			s->memory[(opcode[2]<<8 | opcode[1]) + 1] = s->h;
			pcAdd = 3;
			break;
		case 0x23: /* INX H */
			temp16 = (s->h << 8 | s->l) + 1;
			s->h = (temp16 & 0xff00) >> 8;
			s->l = temp16 & 0x00ff;
			break;
		case 0x24: /* INR H */
			temp16 = s->h + 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.ac = temp16 > 0xff;
			s->h = temp8;
			break;
		case 0x25: /* DCR H */
			temp16 = s->h - 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// if previously positive, now negative
			s->cc.ac = ((s->h & 0x80) == 0) && s->cc.s;
			s->h = temp8;
			break;
		case 0x26: /* MVI H, D8 */
			s->h = opcode[1];
			pcAdd = 2;
			break;
		case 0x27: /* DAA "SPECIAL" */
			unimpl(s);
			break;
		case 0x28: /* - */ unimpl(s); break;
		case 0x29: /* DAD H */
			temp16 = s->h << 1 | s->l << 1;
			s->cc.cy = (s->h & 0x80) == 0x80;
			s->h = (temp16 & 0xff00) >> 8;
			s->l = temp16 & 0x00ff;
			break;
		case 0x2a: /* SHLD adr */
			s->l = s->memory[opcode[2]<<8 | opcode[1]];
			s->h = s->memory[(opcode[2]<<8 | opcode[1]) + 1];
			pcAdd = 3;
			break;
		case 0x2b: /* DCX H */
			temp16 = (s->h << 8 | s->l) - 1;
			s->h = (temp16 & 0xff00) >> 8;
			s->l = temp16 & 0x00ff;
			break;
		case 0x2c: /* INR L */
			temp16 = s->l + 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.ac = temp16 > 0xff;
			s->l = temp8;
			break;
		case 0x2d: /* DCR L */
			temp16 = s->l - 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// if previously positive, now negative
			s->cc.ac = ((s->l & 0x80) == 0) && s->cc.s;
			s->l = temp8;
			break;
		case 0x2e: /* MVI L, D8 */
			s->l = opcode[1];
			pcAdd = 2;
			break;
		case 0x2f: /* CMA */
			s->a = ~s->a;
			break;
		case 0x30: /* SIM "SPECIAL" */
			unimpl(s);
			break;
		case 0x31: /* LXI SP, D16 */
			s->sp = opcode[2] << 8 | opcode[1];
			pcAdd = 3;
			break;
		case 0x32: /* STA adr */
			s->memory[opcode[2]<<8 | opcode[1]] = s->a;
			pcAdd = 3;
			break;
		case 0x33: /* INX SP */
			s->sp++;
			break;
		case 0x34: /* INR M */
			addy = s->h << 8 | s->l;
			temp16 = s->memory[addy] + 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.ac = temp16 > 0xff;
			s->memory[addy] = temp8;
			break;
		case 0x35: /* DCR M */
			addy = s->h << 8 | s->l;
			temp16 = s->memory[addy] - 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// if previously positive, now negative
			s->cc.ac = ((s->memory[addy] & 0x80) == 0) && s->cc.s;
			s->memory[addy] = temp8;
			break;
		case 0x36: /* MVI M, D8 */
			addy = s->h << 8 | s->l;
			s->memory[addy] = opcode[1];
			pcAdd = 2;
			break;
		case 0x37: /* STC */
			s->cc.cy = 1;
			break;
		case 0x38: /* - */ unimpl(s); break;
		case 0x39: /* DAD SP */
			temp16 = (s->h << 8 | s->l) + (s->sp);
			s->cc.cy =
				((unsigned int)(s->h << 8 | s->l) + (s->sp))
				> 0xffff;
			s->h = (temp16 & 0xff00) >> 8;
			s->l = temp16 & 0x00ff;
			break;
		case 0x3a: /* LDA adr */
			s->a = s->memory[opcode[2]<<8 | opcode[1]];
			pcAdd = 3;
			break;
		case 0x3b: /* DCX SP */
			s->sp--;
			break;
		case 0x3c: /* INR A */
			temp16 = s->a + 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.ac = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x3d: /* DCR A */
			temp16 = s->a - 1;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// if previously positive, now negative
			s->cc.ac = ((s->a & 0x80) == 0) && s->cc.s;
			s->a = temp8;
			break;
		case 0x3e: /* MVI A, D8 */
			s->a = opcode[1];
			pcAdd = 2;
			break;
		case 0x3f: /* CMC */
			s->cc.cy = ~s->cc.cy;
			break;
		case 0x40: /* MOV B, B */
			s->b = s->b;
			break;
		case 0x41: /* MOV B, C */
			s->b = s->c;
			break;
		case 0x42: /* MOV B, D */
			s->b = s->d;
			break;
		case 0x43: /* MOV B, E */
			s->b = s->e;
			break;
		case 0x44: /* MOV B, H */
			s->b = s->h;
			break;
		case 0x45: /* MOV B, L */
			s->b = s->l;
			break;
		case 0x46: /* MOV B, M */
			addy = s->h << 8 | s->l;
			s->b = s->memory[addy];
			break;
		case 0x47: /* MOV B, A */
			s->b = s->a;
			break;
		case 0x48: /* MOV C, B */
			s->c = s->b;
			break;
		case 0x49: /* MOV C, C */
			s->c = s->c;
			break;
		case 0x4a: /* MOV C, D */
			s->c = s->d;
			break;
		case 0x4b: /* MOV C, E */
			s->c = s->e;
			break;
		case 0x4c: /* MOV C, H */
			s->c = s->h;
			break;
		case 0x4d: /* MOV C, L */
			s->c = s->l;
			break;
		case 0x4e: /* MOV C, M */
			addy = s->h << 8 | s->l;
			s->c = s->memory[addy];
			break;
		case 0x4f: /* MOV C, A */
			s->c = s->a;
			break;
		case 0x50: /* MOV D, B */
			s->d = s->b;
			break;
		case 0x51: /* MOV D, C */
			s->d = s->c;
			break;
		case 0x52: /* MOV D, D */
			s->d = s->d;
			break;
		case 0x53: /* MOV D, E */
			s->d = s->e;
			break;
		case 0x54: /* MOV D, H */
			s->d = s->h;
			break;
		case 0x55: /* MOV D, L */
			s->d = s->l;
			break;
		case 0x56: /* MOV D, M */
			addy = s->h << 8 | s->l;
			s->d = s->memory[addy];
			break;
		case 0x57: /* MOV D, A */
			s->d = s->a;
			break;
		case 0x58: /* MOV E, B */
			s->e = s->b;
			break;
		case 0x59: /* MOV E, C */
			s->e = s->c;
			break;
		case 0x5a: /* MOV E, D */
			s->e = s->d;
			break;
		case 0x5b: /* MOV E, E */
			s->e = s->e;
			break;
		case 0x5c: /* MOV E, H */
			s->e = s->h;
			break;
		case 0x5d: /* MOV E, L */
			s->e = s->l;
			break;
		case 0x5e: /* MOV E, M */
			addy = s->h << 8 | s->l;
			s->e = s->memory[addy];
			break;
		case 0x5f: /* MOV E, A */
			s->e = s->a;
			break;
		case 0x60: /* MOV H, B */
			s->h = s->b;
			break;
		case 0x61: /* MOV H, C */
			s->h = s->c;
			break;
		case 0x62: /* MOV H, D */
			s->h = s->d;
			break;
		case 0x63: /* MOV H, E */
			s->h = s->e;
			break;
		case 0x64: /* MOV H, H */
			s->h = s->h;
			break;
		case 0x65: /* MOV H, L */
			s->h = s->l;
			break;
		case 0x66: /* MOV H, M */
			addy = s->h << 8 | s->l;
			s->h = s->memory[addy];
			break;
		case 0x67: /* MOV H, A */
			s->h = s->a;
			break;
		case 0x68: /* MOV L, B */
			s->l = s->b;
			break;
		case 0x69: /* MOV L, C */
			s->l = s->c;
			break;
		case 0x6a: /* MOV L, D */
			s->l = s->d;
			break;
		case 0x6b: /* MOV L, E */
			s->l = s->e;
			break;
		case 0x6c: /* MOV L, H */
			s->l = s->h;
			break;
		case 0x6d: /* MOV L, L */
			s->l = s->l;
			break;
		case 0x6e: /* MOV L, M */
			addy = s->h << 8 | s->l;
			s->l = s->memory[addy];
			break;
		case 0x6f: /* MOV L, A */
			s->l = s->a;
			break;
		case 0x70: /* MOV M, B */
			addy = s->h << 8 | s->l;
			s->memory[addy] = s->b;
			break;
		case 0x71: /* MOV M, C */
			addy = s->h << 8 | s->l;
			s->memory[addy] = s->c;
			break;
		case 0x72: /* MOV M, D */
			addy = s->h << 8 | s->l;
			s->memory[addy] = s->d;
			break;
		case 0x73: /* MOV M, E */
			addy = s->h << 8 | s->l;
			s->memory[addy] = s->e;
			break;
		case 0x74: /* MOV M, H */
			addy = s->h << 8 | s->l;
			s->memory[addy] = s->h;
			break;
		case 0x75: /* MOV M, L */
			addy = s->h << 8 | s->l;
			s->memory[addy] = s->l;
			break;
		case 0x76: /* HLT */
			// "SPECIAL"
			break;
		case 0x77: /* MOV M, A */
			addy = s->h << 8 | s->l;
			s->memory[addy] = s->a;
			break;
		case 0x78: /* MOV A, B */
			s->a = s->b;
			break;
		case 0x79: /* MOV A, C */
			s->a = s->c;
			break;
		case 0x7a: /* MOV A, D */
			s->a = s->d;
			break;
		case 0x7b: /* MOV A, E */
			s->a = s->e;
			break;
		case 0x7c: /* MOV A, H */
			s->a = s->h;
			break;
		case 0x7d: /* MOV A, L */
			s->a = s->l;
			break;
		case 0x7e: /* MOV A, M */
			addy = s->h << 8 | s->l;
			s->a = s->memory[addy];
			break;
		case 0x7f: /* MOV A, A */
			s->a = s->a;
			break;
		case 0x80: /* ADD B */
			temp16 = (u16_t)s->a + s->b;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x81: /* ADD C */
			temp16 = (u16_t)s->a + s->c;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x82: /* ADD D */
			temp16 = (u16_t)s->a + s->d;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x83: /* ADD E */
			temp16 = (u16_t)s->a + s->e;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x84: /* ADD H */
			temp16 = (u16_t)s->a + s->h;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x85: /* ADD L */
			temp16 = (u16_t)s->a + s->l;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x86: /* ADD M (add byte at HL addy) */
			addy = s->h << 8 | s->l;
			temp16 = (u16_t)s->a + s->memory[addy];
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.cy = temp16 > 0xff;
			s->cc.p = parity(temp8);
			s->a = temp8;
			break;
		case 0x87: /* ADD A */
			temp16 = (u16_t)s->a + s->a;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x88: /* ADC B */
			temp16 = (u16_t)s->a + s->b + s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x89: /* ADC C */
			temp16 = (u16_t)s->a + s->c + s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x8a: /* ADC D */
			temp16 = (u16_t)s->a + s->d + s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x8b: /* ADC E */
			temp16 = (u16_t)s->a + s->e + s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x8c: /* ADC H */
			temp16 = (u16_t)s->a + s->h + s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x8d: /* ADC L */
			temp16 = (u16_t)s->a + s->l + s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x8e: /* ADC M */
			addy = s->h << 8 | s->l;
			temp16 = (u16_t)s->a + s->memory[addy] + s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x8f: /* ADC A */
			temp16 = (u16_t)s->a + s->a + s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			break;
		case 0x90: /* SUB B */
			temp8 = s->a - s->b;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = s->a < s->b;
			s->a = temp8;
			break;
		case 0x91: /* SUB C */
			temp8 = s->a - s->c;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = s->a < s->c;
			s->a = temp8;
			break;
		case 0x92: /* SUB D */
			temp8 = s->a - s->d;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = s->a < s->d;
			s->a = temp8;
			break;
		case 0x93: /* SUB E */
			temp8 = s->a - s->e;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = s->a < s->e;
			s->a = temp8;
			break;
		case 0x94: /* SUB H */
			temp8 = s->a - s->h;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = s->a < s->h;
			s->a = temp8;
			break;
		case 0x95: /* SUB L */
			temp8 = s->a - s->l;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = s->a < s->l;
			s->a = temp8;
			break;
		case 0x96: /* SUB M */
			addy = s->h << 8 | s->l;
			temp8 = s->a - s->memory[addy];
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = s->a < s->l;
			s->a = temp8;
			break;
		case 0x97: /* SUB A */
			temp8 = s->a - s->a;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = 0;
			s->a = temp8;
			break;
		case 0x98: /* SBB B */
			temp16 = (u16_t)s->a - s->b - s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = (u16_t)s->a < ((u16_t)s->b + s->cc.cy);
			s->a = temp8;
			break;
		case 0x99: /* SBB C */
			temp16 = (u16_t)s->a - s->c - s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = (u16_t)s->a < ((u16_t)s->c + s->cc.cy);
			s->a = temp8;
			break;
		case 0x9a: /* SBB D */
			temp16 = (u16_t)s->a - s->d - s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = (u16_t)s->a < ((u16_t)s->d + s->cc.cy);
			s->a = temp8;
			break;
		case 0x9b: /* SBB E */
			temp16 = (u16_t)s->a - s->e - s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = (u16_t)s->a < ((u16_t)s->e + s->cc.cy);
			s->a = temp8;
			break;
		case 0x9c: /* SBB H */
			temp16 = (u16_t)s->a - s->h - s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = (u16_t)s->a < ((u16_t)s->h + s->cc.cy);
			s->a = temp8;
			break;
		case 0x9d: /* SBB L */
			temp16 = (u16_t)s->a - s->l - s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = (u16_t)s->a < ((u16_t)s->l + s->cc.cy);
			s->a = temp8;
			break;
		case 0x9e: /* SBB M */
			addy = s->h << 8 | s->l;
			temp16 = (u16_t)s->a - s->memory[addy] - s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = (u16_t)s->a < ((u16_t)s->memory[addy] + s->cc.cy);
			s->a = temp8;
			break;
		case 0x9f: /* SBB A */
			temp16 = -s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			// TODO: HOW?
			s->cc.cy = s->cc.cy;
			s->a = temp8;
			break;
		case 0xa0: /* ANA B */
			s->a &= s->b;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xa1: /* ANA C */
			s->a &= s->c;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xa2: /* ANA D */
			s->a &= s->d;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xa3: /* ANA E */
			s->a &= s->e;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xa4: /* ANA H */
			s->a &= s->h;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xa5: /* ANA L */
			s->a &= s->l;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xa6: /* ANA M */
			s->a &= s->memory[s->h<<8 | s->l];
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xa7: /* ANA A */
			s->a &= s->a;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xa8: /* XRA B */
			s->a ^= s->b;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xa9: /* XRA C */
			s->a ^= s->c;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xaa: /* XRA D */
			s->a ^= s->d;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xab: /* XRA E */
			s->a ^= s->e;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xac: /* XRA H */
			s->a ^= s->h;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xad: /* XRA L */
			s->a ^= s->l;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xae: /* XRA M */
			s->a ^= s->memory[s->h<<8 | s->l];
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xaf: /* XRA A */
			s->a ^= s->a;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xb0: /* ORA B */
			s->a |= s->b;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xb1: /* ORA C */
			s->a |= s->c;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xb2: /* ORA D */
			s->a |= s->d;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xb3: /* ORA E */
			s->a |= s->e;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xb4: /* ORA H */
			s->a |= s->h;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xb5: /* ORA L */
			s->a |= s->l;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xb6: /* ORA M */
			s->a |= s->memory[s->h<<8 | s->l];
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xb7: /* ORA A */
			s->a |= s->a;
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xb8: /* CMP B */
			temp8 = s->a - s->b;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = s->a < s->b;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xb9: /* CMP C */
			temp8 = s->a - s->c;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = s->a < s->c;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xba: /* CMP D */
			temp8 = s->a - s->d;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = s->a < s->d;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xbb: /* CMP E */
			temp8 = s->a - s->e;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = s->a < s->e;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xbc: /* CMP H */
			temp8 = s->a - s->h;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = s->a < s->h;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xbd: /* CMP L */
			temp8 = s->a - s->l;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = s->a < s->l;
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xbe: /* CMP M */
			temp8 = s->a - s->memory[s->h<<8 | s->l];
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = s->a < s->memory[s->h<<8 | s->l];
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xbf: /* CMP A */
			temp8 = s->a - s->a;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->cc.cy = 0; // s->a < s->a
			// p 4-3: "set when carrying from (0-based) bit 3 to 4"
			// TODO
			s->cc.ac = 0;
			break;
		case 0xc0: /* RNZ */
			if (!s->cc.z)
			{
				s->pc = s->memory[s->sp+1]<<8 | s->memory[s->sp];
				pcAdd = 0;
				s->sp += 2;
			}
			break;
		case 0xc1: /* POP B */
			s->c = s->memory[s->sp];
			s->b = s->memory[s->sp + 1];
			s->sp += 2; // stack shrinks to the right
			break;
		case 0xc2: /* JNZ adr */
			pcAdd = 3;
			if (!s->cc.z)
			{
				s->pc = opcode[2]<<8 | opcode[1];
				pcAdd = 0;
			}
			break;
		case 0xc3: /* JMP adr */
			s->pc = opcode[2]<<8 | opcode[1];
			pcAdd = 0;
			break;
		case 0xc4: /* CNZ adr */
			pcAdd = 3;
			if (!s->cc.z)
			{
				temp16 = s->pc + 3;
				s->memory[--s->sp] = temp16 >> 8;   // H byte
				s->memory[--s->sp] = temp16 & 0xff; // L byte
				s->pc = opcode[2]<<8 | opcode[1];
				pcAdd = 0;
			}
			break;
		case 0xc5: /* PUSH B */
			s->memory[--s->sp] = s->b;
			s->memory[--s->sp] = s->c;
			break;
		case 0xc6: /* ADI byte */
			temp16 = (u16_t) s-> a +
				opcode[1];
			temp8 = temp16 & 0xff;
			s->cc.z = temp8 == 0;
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.cy = temp16 > 0xff;
			s->cc.p = parity(temp8);
			s->a = temp8;
			pcAdd = 2;
			break;
		case 0xc7: /* RST 0 */
			temp16 = s->pc + 1;
			s->memory[--s->sp] = temp16 >> 8;   // H byte
			s->memory[--s->sp] = temp16 & 0xff; // L byte
			s->pc = 0x0000;
			pcAdd = 0;
			break;
		case 0xc8: /* RZ */
			if (s->cc.z)
			{
				s->pc = s->memory[s->sp+1]<<8 | s->memory[s->sp];
				pcAdd = 0;
				s->sp += 2;
			}
			break;
		case 0xc9: /* RET */
			s->pc = s->memory[s->sp+1]<<8 | s->memory[s->sp];
			pcAdd = 0;
			s->sp += 2;
			break;
		case 0xca: /* JZ adr */
			pcAdd = 3;
			if (s->cc.z)
			{
				s->pc = s->memory[opcode[2]]<<8 | s->memory[opcode[1]];
				pcAdd = 0;
			}
			break;
		case 0xcb: /* - */ unimpl(s); break;
		case 0xcc: /* CZ adr */
			pcAdd = 3;
			if (s->cc.z)
			{
				temp16 = s->pc + 3;
				s->memory[--s->sp] = temp16 >> 8;   // H byte
				s->memory[--s->sp] = temp16 & 0xff; // L byte
				s->pc = opcode[2]<<8 | opcode[1];
				pcAdd = 0;
			}
			break;
		case 0xcd: /* CALL adr */
			temp16 = s->pc + 3;
			s->memory[--s->sp] = temp16 >> 8;   // H byte
			s->memory[--s->sp] = temp16 & 0xff; // L byte
			s->pc = opcode[2]<<8 | opcode[1];
			pcAdd = 0;
			break;
		case 0xce: /* ACI D8 */
			temp16 = s->a + opcode[1] + s->cc.cy;
			temp8 = temp16 & 0xff;
			s->cc.cy = temp16 > 0xff;
			s->a = temp8;
			pcAdd = 2;
			break;
		case 0xcf: /* RST 1 */
			temp16 = s->pc + 1;
			s->memory[--s->sp] = temp16 >> 8;   // H byte
			s->memory[--s->sp] = temp16 & 0xff; // L byte
			s->pc = 0x0008;
			pcAdd = 0;
			break;
		case 0xd0: /* RNC */
			if (!s->cc.cy)
			{
				s->pc = s->memory[s->sp+1]<<8 | s->memory[s->sp];
				pcAdd = 0;
				s->sp += 2;
			}
			break;
		case 0xd1: /* POP D */
			s->e = s->memory[s->sp];
			s->d = s->memory[s->sp + 1];
			s->sp += 2;
			break;
		case 0xd2: /* JNC adr */
			pcAdd = 3;
			if (!s->cc.cy)
			{
				s->pc = s->memory[opcode[2]]<<8 | s->memory[opcode[1]];
				pcAdd = 0;
			}
			break;
		case 0xd3: /* OUT D8 "SPECIAL" */
			unimpl(s);
			pcAdd = 2;
			break;
		case 0xd4: /* CNC adr */
			pcAdd = 3;
			if (!s->cc.cy)
			{
				temp16 = s->pc + 3;
				s->memory[--s->sp] = temp16 >> 8;   // H byte
				s->memory[--s->sp] = temp16 & 0xff; // L byte
				s->pc = opcode[2]<<8 | opcode[1];
				pcAdd = 0;
			}
			break;
		case 0xd5: /* PUSH D */
			s->memory[--s->sp] = s->d;
			s->memory[--s->sp] = s->e;
			break;
		case 0xd6: /* SUI D8 */
			temp8 = s->a - opcode[1];
			s->cc.z = temp8 == 0;
			s->cc.cy = !s->cc.s & ((temp8 & 0x80) == 0x80);
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->a = temp8;
			pcAdd = 2;
			break;
		case 0xd7: /* RST 2 */
			temp16 = s->pc + 1;
			s->memory[--s->sp] = temp16 >> 8;   // H byte
			s->memory[--s->sp] = temp16 & 0xff; // L byte
			s->pc = 0x0010;
			pcAdd = 0;
			break;
		case 0xd8: /* RC */
			if (s->cc.cy)
			{
				s->pc = s->memory[s->sp+1]<<8 | s->memory[s->sp];
				pcAdd = 0;
				s->sp += 2;
			}
			break;
		case 0xd9: /* - */ unimpl(s); break;
		case 0xda: /* JC adr */
			pcAdd = 3;
			if (s->cc.cy)
			{
				s->pc = s->memory[opcode[2]]<<8 | s->memory[opcode[1]];
				pcAdd = 0;
			}
			break;
		case 0xdb: /* IN D8 "SPECIAL" */
			unimpl(s);
			pcAdd = 2;
			break;
		case 0xdc: /* CC adr */
			pcAdd = 3;
			if (s->cc.cy)
			{
				temp16 = s->pc + 3;
				s->memory[--s->sp] = temp16 >> 8;   // H byte
				s->memory[--s->sp] = temp16 & 0xff; // L byte
				s->pc = opcode[2]<<8 | opcode[1];
				pcAdd = 0;
			}
			break;
		case 0xdd: /* - */ unimpl(s); break;
		case 0xde: /* SBI D8 */
			temp8 = s->a - opcode[1] - s->cc.cy;
			s->cc.z = temp8 == 0;
			s->cc.cy = !s->cc.s & ((temp8 & 0x80) == 0x80);
			s->cc.s = (temp8 & 0x80) == 0x80;
			s->cc.p = parity(temp8);
			s->a = temp8;
			pcAdd = 2;
			break;
		case 0xdf: /* RST 3 */
			temp16 = s->pc + 1;
			s->memory[--s->sp] = temp16 >> 8;   // H byte
			s->memory[--s->sp] = temp16 & 0xff; // L byte
			s->pc = 0x0018;
			pcAdd = 0;
			break;
		case 0xe0: /* RPO */
			if (!s->cc.p)
			{
				s->pc = s->memory[s->sp+1]<<8 | s->memory[s->sp];
				pcAdd = 0;
				s->sp += 2;
			}
			break;
		case 0xe1: /* POP H */
			s->l = s->memory[s->sp];
			s->h = s->memory[s->sp + 1];
			s->sp += 2;
			break;
		case 0xe2: /* JPO adr */
			pcAdd = 3;
			if (!s->cc.p)
			{
				s->pc = s->memory[opcode[2]]<<8 | s->memory[opcode[1]];
				pcAdd = 0;
			}
			break;
		case 0xe3: /* XTHL */
			temp16 = s->h<<8 | s->l;
			s->l = s->memory[s->sp];
			s->h = s->memory[s->sp+1];
			s->memory[s->sp] = temp16 & 0x00ff;
			s->memory[s->sp+1] = (temp16 & 0xff00) >> 8;
			break;
		case 0xe4: /* CPO adr */
			pcAdd = 3;
			if (!s->cc.p)
			{
				temp16 = s->pc + 3;
				s->memory[--s->sp] = temp16 >> 8;   // H byte
				s->memory[--s->sp] = temp16 & 0xff; // L byte
				s->pc = opcode[2]<<8 | opcode[1];
				pcAdd = 0;
			}
			break;
		case 0xe5: /* PUSH H */
			s->memory[--s->sp] = s->h;
			s->memory[--s->sp] = s->l;
			break;
		case 0xe6: /* ANI D8 */
			s->a &= opcode[1];
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0; // TODO still don't know what to do here
			s->cc.ac = 0; // TODO or here
			pcAdd = 2;
			break;
		case 0xe7: /* RST 4 */
			temp16 = s->pc + 1;
			s->memory[--s->sp] = temp16 >> 8;   // H byte
			s->memory[--s->sp] = temp16 & 0xff; // L byte
			s->pc = 0x0020;
			pcAdd = 0;
			break;
		case 0xe8: /* RPE */
			if (s->cc.p)
			{
				s->pc = s->memory[s->sp+1]<<8 | s->memory[s->sp];
				pcAdd = 0;
				s->sp += 2;
			}
			break;
		case 0xe9: /* PCHL */
			s->pc = s->h<<8 | s->l;
			pcAdd = 0;
			break;
		case 0xea: /* JPE adr */
			pcAdd = 3;
			if (s->cc.p)
			{
				s->pc = s->memory[opcode[2]]<<8 | s->memory[opcode[1]];
				pcAdd = 0;
			}
			break;
		case 0xeb: /* XCHG */
			temp8 = s->d;
			s->d = s->h;
			s->h = temp8;
			temp8 = s->e;
			s->e = s->l;
			s->l = temp8;
			break;
		case 0xec: /* CPE adr */
			pcAdd = 3;
			if (s->cc.p)
			{
				temp16 = s->pc + 3;
				s->memory[--s->sp] = temp16 >> 8;   // H byte
				s->memory[--s->sp] = temp16 & 0xff; // L byte
				s->pc = opcode[2]<<8 | opcode[1];
				pcAdd = 0;
			}
			break;
		case 0xed: /* - */ unimpl(s); break;
		case 0xee: /* XRI D8 */
			s->a ^= opcode[1];
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0; // TODO still don't know what to do here
			s->cc.ac = 0; // TODO or here
			pcAdd = 2;
			break;
		case 0xef: /* RST 5 */
			temp16 = s->pc + 1;
			s->memory[--s->sp] = temp16 >> 8;   // H byte
			s->memory[--s->sp] = temp16 & 0xff; // L byte
			s->pc = 0x0028;
			pcAdd = 0;
			break;
		case 0xf0: /* RP */
			if (!s->cc.s)
			{
				s->pc = s->memory[s->sp+1]<<8 | s->memory[s->sp];
				pcAdd = 0;
				s->sp += 2;
			}
			break;
		case 0xf1: /* POP PSW */
			// 7 6 5 4 3 2 1 0
			//
			// 0 0 0 0 0 0 0 0
			// x x x x x x x x
			// 8 4 2 1 0 0 0 0
			// 0 0 0 0 8 4 2 1
			temp8 = s->memory[s->sp];
			s->cc.s  = (temp8 & 0x80) == 0x80;
			s->cc.z  = (temp8 & 0x40) == 0x40;
			s->cc.ac = (temp8 & 0x10) == 0x10;
			s->cc.p  = (temp8 & 0x04) == 0x04;
			s->cc.cy = (temp8 & 0x01) == 0x01;

			s->a = s->memory[s->sp + 1];
			s->sp += 2;
			break;
		case 0xf2: /* JP adr */
			pcAdd = 3;
			if (!s->cc.s)
			{
				s->pc = s->memory[opcode[2]]<<8 | s->memory[opcode[1]];
				pcAdd = 0;
			}
			break;
		case 0xf3: /* DI "SPECIAL" */
			unimpl(s);
			break;
		case 0xf4: /* CP adr */
			pcAdd = 3;
			if (!s->cc.s)
			{
				temp16 = s->pc + 3;
				s->memory[--s->sp] = temp16 >> 8;   // H byte
				s->memory[--s->sp] = temp16 & 0xff; // L byte
				s->pc = opcode[2]<<8 | opcode[1];
				pcAdd = 0;
			}
			break;
		case 0xf5: /* PUSH PSW */
			s->memory[--s->sp] = s->a;

			temp8 =
				s->cc.s     << 7
				| s->cc.z   << 6
				| 0         << 5
				| s->cc.ac  << 4
				| 0         << 3
				| s->cc.p   << 2
				| 1         << 1
				| s->cc.cy;
			s->memory[--s->sp] = temp8;
			break;
		case 0xf6: /* ORI D8 */
			s->a |= opcode[1];
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.cy = 0; // TODO still don't know what to do here
			s->cc.ac = 0; // TODO or here
			pcAdd = 2;
			break;
		case 0xf7: /* RST 6 */
			temp16 = s->pc + 1;
			s->memory[--s->sp] = temp16 >> 8;   // H byte
			s->memory[--s->sp] = temp16 & 0xff; // L byte
			s->pc = 0x0030;
			pcAdd = 0;
			break;
		case 0xf8: /* RM */
			if (s->cc.s)
			{
				s->pc = s->memory[s->sp+1]<<8 | s->memory[s->sp];
				pcAdd = 0;
				s->sp += 2;
			}
			break;
		case 0xf9: /* SPHL */
			s->sp = s->h<<8 | s->l;
			break;
		case 0xfa: /* JM adr */
			pcAdd = 3;
			if (s->cc.s)
			{
				s->pc = s->memory[opcode[2]]<<8 | s->memory[opcode[1]];
				pcAdd = 0;
			}
			break;
		case 0xfb: /* EI "SPECIAL" */
			unimpl(s);
			break;
		case 0xfc: /* CM adr */
			pcAdd = 3;
			if (s->cc.s)
			{
				temp16 = s->pc + 3;
				s->memory[--s->sp] = temp16 >> 8;   // H byte
				s->memory[--s->sp] = temp16 & 0xff; // L byte
				s->pc = opcode[2]<<8 | opcode[1];
				pcAdd = 0;
			}
			break;
		case 0xfd: /* - */ unimpl(s); break;
		case 0xfe: /* CPI D8 */
			s->a -= opcode[1];
			// wasn't negative, now is
			s->cc.cy = !s->cc.s & ((s->a & 0x80) == 0x80);
			s->cc.z = s->a == 0;
			s->cc.s = (s->a & 0x80) == 0x80;
			s->cc.p = parity(s->a);
			s->cc.ac = 0; // TODO or here
			pcAdd = 2;
			break;
		case 0xff: /* RST 7 */
			temp16 = s->pc + 1;
			s->memory[--s->sp] = temp16 >> 8;   // H byte
			s->memory[--s->sp] = temp16 & 0xff; // L byte
			s->pc = 0x0038;
			pcAdd = 0;
			break;
	};

	s->pc += pcAdd;

	return 0;
}

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
