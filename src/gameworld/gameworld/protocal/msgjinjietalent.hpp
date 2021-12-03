#ifndef __MSG_TALENT_HPP__
#define __MSG_TALENT_HPP__

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/jinjietalentdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/*
	请求所有天赋
	*/
	enum JINJIE_TALENT_OPERATE_TYPE
	{
		JINJIE_TALENT_OPERATE_TYPE_INFO = 0,
		JINJIE_TALENT_OPERATE_TYPE_CHOUJIANG_INFO,
		JINJIE_TALENT_OPERATE_TYPE_CHOUJIANG_REFRESH,	// param1:0刷新一次/1刷新全部
		JINJIE_TALENT_OPERATE_TYPE_AWAKE,				// param1:抽奖格子索引
		JINJIE_TALENT_OPERATE_TYPE_PUTON,				// param1:天赋类型, param2:天赋格子序号, param3:背包格子索引
		JINJIE_TALENT_OPERATE_TYPE_PUTOFF,				// param1:天赋类型, param2:天赋格子序号
		JINJIE_TALENT_OPERATE_TYPE_SKILL_UPGRADE_LEVEL, // param1:天赋类型, param2:天赋格子序号
		TALENT_OPERATE_TYPE_SAVE_ATTENTION_SKILL,		// param1:技能ID 关注技能
		TALENT_OPERATE_TYPE_CANCEL_ATTENTION_SKILL,		// param1:技能ID 取消关注技能
	};

	class CSJinJieTalentOperaReqAll
	{
	public:
		CSJinJieTalentOperaReqAll();
		MessageHeader header;			// 8720
		int operate_type;
		int param1;
		int param2;
		int param3;
	};

	/*
	所有天赋
	*/
	class SCJinJieTalentAllInfo
	{
	public:
		SCJinJieTalentAllInfo();
		MessageHeader header;			// 8721

		SCJinJieTalentSingle talent_array[JINJIE_TALENT_TYPE_MAX];
	};

	/*
	单个天赋格更新
	*/
	class SCJinJieTalentUpdateSingleGrid
	{
	public:
		SCJinJieTalentUpdateSingleGrid();
		MessageHeader header;			// 8722

		short talent_type;				//天赋类型		0~6
		short talent_index;				//天赋格子序号	0~12
		SCJinJieTalentGrid single_info;
	};

	/*
	抽奖页所有数据
	*/
	class SCJinJieTalentChoujiangPage
	{
	public:
		SCJinJieTalentChoujiangPage();
		MessageHeader header;			// 8723

		int free_chou_count;
		short cur_count;					//当前抽奖次数
		short choujiang_grid_skill_array[JINJIE_TALENT_CHOUJIANG_GRID_MAX_NUM];
	};

	/*
	保存的关注技能ID
	*/
	class SCTalentAttentionSkillID
	{
	public:
		SCTalentAttentionSkillID();
		MessageHeader header;			// 

		int count;
		short save_skill_id[TALENT_ATTENTION_SKILL_MAX_SAVE_NUM];					//  TALENT_ATTENTION_SKILL_MAX_SAVE_NUM = 8
	};
}

#pragma pack(pop)

#endif //__MSG_TALENT_HPP__


