#ifndef __HUASHEN_DEF_HPP__
#define __HUASHEN_DEF_HPP__

#include "servercommon.h"

static const int HUASHEN_MAX_ID_LIMIT = 9;			// �������������
static const int HUASHEN_MAX_LEVEL_LIMIT = 30;		// �����ȼ�������
static const int HUASHEN_MAX_GRADE_LIMIT = 30;		// ������
static const int HUASHEN_SPIRIT_MAX_ID_LIMIT= 4;	// �����ID
static const int HUASHEN_SPIRIT_MAX_LEVEL = 10;		// �����ȼ�

#pragma pack(push, 4)

struct HuaShenLevelInfo
{
	HuaShenLevelInfo() : level(0), jinhua_val(0) {}

	short level;			// �ȼ�
	short jinhua_val;		// ��ǰ�Ľ���ֵ
};

struct HuaShenSpiritInfo
{
	struct SingleSpirit
	{
		SingleSpirit() : level(0), exp_val(0) {}

		short level;
		unsigned short exp_val;
	};

	HuaShenSpiritInfo() { this->Reset(); }

	void Reset()
	{
		memset(spirit_list, 0, sizeof(spirit_list));
	}

	SingleSpirit spirit_list[HUASHEN_SPIRIT_MAX_ID_LIMIT + 1];
};

struct HuashenParam
{
	enum HUASHEN_ID
	{
		HUASHEN_ID_INVALID = -1,
	};

	HuashenParam() { this->Reset(); }

	void Reset()
	{
		active_flag = 0;
		cur_huashen_id = HUASHEN_ID_INVALID;
		reverse_sh = 0;

		memset(level_info_list, 0, sizeof(level_info_list));
		memset(spirit_info_list, 0, sizeof(spirit_info_list));
		memset(grade_list, 0, sizeof(grade_list));
	}

	char active_flag;										// ����״̬
	char cur_huashen_id;									// ��ǰʹ�õĻ���-1����ûʹ��
	short reverse_sh;

	HuaShenLevelInfo level_info_list[HUASHEN_MAX_ID_LIMIT + 1];
	HuaShenSpiritInfo spirit_info_list[HUASHEN_MAX_ID_LIMIT + 1];
	short grade_list[HUASHEN_MAX_ID_LIMIT + 1];
};

typedef char HuashenParamHex[sizeof(HuashenParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(HuashenParamHex) < 1024);

#pragma pack(pop)

#endif
