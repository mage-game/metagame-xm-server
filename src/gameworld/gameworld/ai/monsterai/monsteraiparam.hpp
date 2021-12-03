/*************************************************************************************************
FileName: monsteraibase.hpp
Author: dreamwang
Description: 怪物AI参数 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/11/10/ 13:37:05     1.0           创建
************************************************************************************************/

#ifndef __MONSTER_AIPARAM_HPP_
#define __MONSTER_AIPARAM_HPP_

struct MonsterAIParam
{
	MonsterAIParam() : run_range(0), aoi_range(0), bossskill_cond_id(0), walk_point_num(0), walk_range(0)
	{

	}

	MonsterAIParam(unsigned int rr, unsigned int aoir, int bscid, unsigned int wpn, unsigned int wr)
		: run_range(rr), aoi_range(aoir), bossskill_cond_id(bscid), walk_point_num(wpn), walk_range(wr)
	{

	}

	unsigned int run_range;						// 可移动范围
	unsigned int aoi_range;						// 主动怪探测范围
	int bossskill_cond_id;						// Boss技能条件ID
	unsigned int walk_point_num;				// 怪物随机散步地点数
	unsigned int walk_range;					// 怪物散步范围
};

#endif

