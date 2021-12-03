#ifndef __LOVETREE_DEF_HPP__
#define __LOVETREE_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

struct LoveTreeData
{

	LoveTreeData(){ this->Reset(); }

	void Reset()
	{
		lover_id = -1;
		love_tree_star_level = 0;
		love_tree_cur_exp = 0;
		memset(tree_name, 0, sizeof(tree_name));
	}

	int lover_id;
	int love_tree_star_level;
	int love_tree_cur_exp;
	GameName tree_name;
};

typedef char LoveTreeDataHex[sizeof(LoveTreeData) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(LoveTreeDataHex) < 128);

#pragma pack(pop)

#endif // __LOVETREE_DEF_HPP__




