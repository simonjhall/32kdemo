#pragma once

//will change, but will be the same between both pads
#define PAD_UP_B	5
#define PAD_DOWN_B	6
#define PAD_LEFT_B	7
#define PAD_RIGHT_B	3
#define PAD_C_B		0
#define PAD_B_B		1

#define PAD_UP_SH		(1 << PAD_UP_B)
#define PAD_DOWN_SH		(1 << PAD_DOWN_B)
#define PAD_LEFT_SH		(1 << PAD_LEFT_B)
#define PAD_RIGHT_SH	(1 << PAD_RIGHT_B)
#define PAD_C_SH		(1 << PAD_C_B)
#define PAD_B_SH		(1 << PAD_B_B)

unsigned char read_pad(unsigned int id);