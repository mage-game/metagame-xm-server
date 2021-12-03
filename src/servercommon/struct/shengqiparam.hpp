#ifndef __SHENGQI_PARAM_HPP__
#define __SHENGQI_PARAM_HPP__

#include "servercommon/servercommon.h"

static const int SHENGQI_MAX_NUM = 16;			// ʥ���������
static const int SHENGQI_SPIRIT_MAX = 500;		// ʥ����������
static const int SHENGQI_STRENGT_MAX = 120;		// ʥ��ǿ���ȼ�����
static const int SHENGQI_SPIRIT_MAX_NUM = 4;	// ʥ����������

enum SHENGQI_ATTR_TYPE
{
	SHENGQI_ATTR_TYPE_MAXHP = 0,									// ����
	SHENGQI_ATTR_TYPE_GONGJI = 1,									// ����
	SHENGQI_ATTR_TYPE_FANGYU = 2,									// ���
	SHENGQI_ATTR_TYPE_FA_FANGYU = 3,								// ���������Σ�
	SHENGQI_ATTR_TYPE_BAOJI = 4,									// ������
	SHENGQI_ATTR_TYPE_JIANREN = 5,									// ���ͣ�������
	SHENGQI_ATTR_TYPE_MINGZHONG = 6,								// ����
	SHENGQI_ATTR_TYPE_SHANBI = 7,									// ����
	SHENGQI_ATTR_TYPE_FUJIA_SHANGHAI = 8,							// �����˺�
	SHENGQI_ATTR_TYPE_DIKANG_SHANGHAI = 9,							// �ֿ��˺�
	SHENGQI_ATTR_TYPE_WUXING_GONGJI = 10,							// ���й��������Σ�
	SHENGQI_ATTR_TYPE_WUXING_FANGYU = 11,							// ���з��������Σ�
	SHENGQI_ATTR_TYPE_PER_BAOJI = 12,								// ��������ֱ�
	SHENGQI_ATTR_TYPE_PER_KANGBAO = 13,								// ��������ֱ�
	SHENGQI_ATTR_TYPE_PER_POFANG = 14,								// �Ʒ�����ֱ�
	SHENGQI_ATTR_TYPE_PER_MIANSHANG = 15,							// ��������ֱ�
	SHENGQI_ATTR_PVP_JIANSHANG_PER = 16,							// pvp������ֱ�
	SHENGQI_ATTR_PVP_ZENGSHANG_PER = 17,                            // pvp������ֱ�
	SHENGQI_ATTR_PVE_JIANSHANG_PER = 18,                            // pve������ֱ�
	SHENGQI_ATTR_PVE_ZENGSHANG_PER = 19,                            // pve������ֱ�
	SHENGQI_ATTR_SHANGHAI_JIACHENG_PER = 20,                        // �˺��ӳ���ֱȣ����Σ�
	SHENGQI_ATTR_SHANGHAI_JIANSHAO_PER = 21,                        // �˺�������ֱȣ����Σ�
	SHENGQI_ATTR_BASE_JIACHENG_PER = 22,							// �������Լӳ���ֱ�

	SHENGQI_ATTR_TYPE_COUNT,										// ʥ�������������
};

#pragma pack(push, 4)

struct ShengqiItem
{
	ShengqiItem()
	{
		this->Reset();
	}

	void Reset()
	{
		level = 0;

		memset(spirit_value, 0, sizeof(spirit_value));
		memset(per_spirit_value, 0, sizeof(per_spirit_value));

		spirit_flag = 0;
		spirit_max_flag = 0;
	}

	bool IsSpiritOpen(int spirit_index)
	{
		return 0 != (spirit_flag & (1 << spirit_index));
	}

	void OpenSpirit(int spirit_index)
	{
		spirit_flag |= (1 << spirit_index);
	}

	bool IsSpiritMax(int spirit_index)
	{
		return 0 != (spirit_max_flag & (1 << spirit_index));
	}

	void MaxSpirit(int spirit_index)
	{
		spirit_max_flag |= (1 << spirit_index);
	}

	short level;										// �ȼ�
	int spirit_value[SHENGQI_SPIRIT_MAX_NUM];			// ��������
	int per_spirit_value[SHENGQI_SPIRIT_MAX_NUM];		// ����������ֱ�
	short spirit_flag;									// ���鿪����־
	short spirit_max_flag;								// ����������־
};

struct ShengqiParam
{
	ShengqiParam() { this->Reset(); }

	void Reset()
	{
		activate_flag = 0;
		for (int i = 0; i < SHENGQI_MAX_NUM; ++i)
		{
			item_list[i].Reset();
		}
	}

	bool IsActivateShengqi()
	{
		return activate_flag != 0;
	}

	bool IsActivateShengqi(int shengqi_index)
	{
		return 0 != (activate_flag & (1 << shengqi_index));
	}

	void ActivateShengqi(int shengqi_index)
	{
		activate_flag |= (1 << shengqi_index);
	}

	int activate_flag;								// �������־
	ShengqiItem item_list[SHENGQI_MAX_NUM];
};

#pragma pack(pop)

typedef char ShengqiParamHex[sizeof(ShengqiParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ShengqiParamHex) < 2048);

#endif
