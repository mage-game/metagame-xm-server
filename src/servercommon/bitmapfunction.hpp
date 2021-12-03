#ifndef _BITMAPFUNCTION_HPP_
#define _BITMAPFUNCTION_HPP_

class BitmapFunction
{
public:
	static bool SetBit(unsigned char *bitmap, int byte_count, int which_bit);

	static bool ClearBit(unsigned char *bitmap, int byte_count, int which_bit);

	static bool IsBitSet(unsigned char *bitmap, int byte_count, int which_bit);
};

#endif
