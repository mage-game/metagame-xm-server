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
	���������츳
	*/
	enum JINJIE_TALENT_OPERATE_TYPE
	{
		JINJIE_TALENT_OPERATE_TYPE_INFO = 0,
		JINJIE_TALENT_OPERATE_TYPE_CHOUJIANG_INFO,
		JINJIE_TALENT_OPERATE_TYPE_CHOUJIANG_REFRESH,	// param1:0ˢ��һ��/1ˢ��ȫ��
		JINJIE_TALENT_OPERATE_TYPE_AWAKE,				// param1:�齱��������
		JINJIE_TALENT_OPERATE_TYPE_PUTON,				// param1:�츳����, param2:�츳�������, param3:������������
		JINJIE_TALENT_OPERATE_TYPE_PUTOFF,				// param1:�츳����, param2:�츳�������
		JINJIE_TALENT_OPERATE_TYPE_SKILL_UPGRADE_LEVEL, // param1:�츳����, param2:�츳�������
		TALENT_OPERATE_TYPE_SAVE_ATTENTION_SKILL,		// param1:����ID ��ע����
		TALENT_OPERATE_TYPE_CANCEL_ATTENTION_SKILL,		// param1:����ID ȡ����ע����
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
	�����츳
	*/
	class SCJinJieTalentAllInfo
	{
	public:
		SCJinJieTalentAllInfo();
		MessageHeader header;			// 8721

		SCJinJieTalentSingle talent_array[JINJIE_TALENT_TYPE_MAX];
	};

	/*
	�����츳�����
	*/
	class SCJinJieTalentUpdateSingleGrid
	{
	public:
		SCJinJieTalentUpdateSingleGrid();
		MessageHeader header;			// 8722

		short talent_type;				//�츳����		0~6
		short talent_index;				//�츳�������	0~12
		SCJinJieTalentGrid single_info;
	};

	/*
	�齱ҳ��������
	*/
	class SCJinJieTalentChoujiangPage
	{
	public:
		SCJinJieTalentChoujiangPage();
		MessageHeader header;			// 8723

		int free_chou_count;
		short cur_count;					//��ǰ�齱����
		short choujiang_grid_skill_array[JINJIE_TALENT_CHOUJIANG_GRID_MAX_NUM];
	};

	/*
	����Ĺ�ע����ID
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


