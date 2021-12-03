#include "bitmapfunction.hpp"
#include "servercommon.h"

bool BitmapFunction::SetBit(unsigned char *bitmap, int byte_count, int which_bit)
{
	if (NULL == bitmap || byte_count <= 0) return false;

	int map_idx = 0;
	int bit_idx = 0;
	unsigned char flag = 0x80;

	UNSTD_STATIC_CHECK(BIT_COUNT_OF_BYTE > 0);

	map_idx = which_bit / BIT_COUNT_OF_BYTE;
	bit_idx = which_bit % BIT_COUNT_OF_BYTE;

	if (map_idx >= byte_count) return false;

	bitmap[map_idx] |= (flag >> bit_idx);

	return true;
}

bool BitmapFunction::ClearBit(unsigned char *bitmap, int byte_count, int which_bit)
{
	if (NULL == bitmap || byte_count <= 0) return false;

	int map_idx = 0;
	int bit_idx = 0;
	unsigned char flag = 0x80;

	UNSTD_STATIC_CHECK(BIT_COUNT_OF_BYTE > 0);

	map_idx = which_bit / BIT_COUNT_OF_BYTE;
	bit_idx = which_bit % BIT_COUNT_OF_BYTE;

	if (map_idx >= byte_count) return false;

	bitmap[map_idx] &= (0xff ^ (flag >> bit_idx));

	return true;
}

bool BitmapFunction::IsBitSet(unsigned char *bitmap, int byte_count, int which_bit)
{
	int map_idx = 0;
	int bit_idx = 0;
	unsigned char flag = 0x80;

	UNSTD_STATIC_CHECK(BIT_COUNT_OF_BYTE > 0);

	map_idx = which_bit / BIT_COUNT_OF_BYTE;
	bit_idx = which_bit % BIT_COUNT_OF_BYTE;

	if (map_idx >= byte_count) return false;

	if (bitmap[map_idx] & (flag >> bit_idx)) return true;

	return false;
}
