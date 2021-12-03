/*************************************************************************************************
FileName: monsteraibase.hpp
Author: dreamwang
Description: ����AI���� 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/11/10/ 13:37:05     1.0           ����
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

	unsigned int run_range;						// ���ƶ���Χ
	unsigned int aoi_range;						// ������̽�ⷶΧ
	int bossskill_cond_id;						// Boss��������ID
	unsigned int walk_point_num;				// �������ɢ���ص���
	unsigned int walk_range;					// ����ɢ����Χ
};

#endif

