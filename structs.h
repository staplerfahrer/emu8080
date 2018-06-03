#pragma once

typedef unsigned short u16_t;
typedef unsigned char u8_t;

typedef struct ConditionCodes
{
	u8_t z:1;
	u8_t s:1;
	u8_t p:1;
	u8_t cy:1;
	u8_t ac:1;
	u8_t pad:3;
} ConditionCodes;

typedef struct State8080
{
	u8_t b;
	u8_t c;
	u8_t d;
	u8_t e;
	u8_t h;
	u8_t l;
	u8_t a;
	u16_t sp;
	u16_t pc;
	u8_t *memory;
	ConditionCodes cc;
	u8_t int_enable;
} State8080;
