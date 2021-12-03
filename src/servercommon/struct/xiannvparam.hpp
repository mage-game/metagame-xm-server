#ifndef __XIANNV_PARAM_HPP__
#define __XIANNV_PARAM_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/bitmapfunction.hpp"

#pragma pack(push) 
#pragma pack(4)

static const int MAX_XIANNV_COUNT = 7;
static const int MAX_XIANNV_POS = 4;
static const int MAX_XIANNV_HUANHUA_COUNT = 128;						// �����Ů�û�����
static const int MAX_XIANNV_UPGRADE_STUFF_EXP = 100000;					// �����������׵������

const static int XIANNV_SHENGWU_MAX_ID = 3;								// ʥ�����ID
const static int XIANNV_SHENGWU_CHOU_COMSUEM_MAX_COUNT= 20;				// ʥ��龭�������������
const static int XIANNV_SHENGWU_GONGMING_MAX_GRID_ID = 28;				// ʥ�ﹲ���������ID
const static int XIANNV_SHENGWU_MILING_TYPE_COUNT = 5;					// ��Һ�������͸���
const static int XIANNV_SHENGWU_MILING_FONT_COUNT = 6;					// ��Һ���������ָ���
const static int XIANNV_SHENGWU_CHOU_EXP_COUNT = 6;						// �龭�飨���䣩������ľ�����Ƭ����
static const int MAX_XIANNV_ACTIVE_SPECIAL_IMAGE_BYTE = MAX_XIANNV_HUANHUA_COUNT / BIT_COUNT_OF_BYTE; //������������ֽ�

enum XiannvShengwuSkillType
{
	XIANNV_SHENGWU_SKILL_TYPE_1 = 1,									// 1������ֱ���������֮Y�㵱ǰʥ����������
	XIANNV_SHENGWU_SKILL_TYPE_2,										// 2������ֱ������Y������ʥ����������
	XIANNV_SHENGWU_SKILL_TYPE_3,										// 3����������ȫ��Ů�������֮Y
	XIANNV_SHENGWU_SKILL_TYPE_4,										// 4���ܵ��˺�ʱX%���ʴ��������ɵ�ǰ����ֵ���֮Y�Ļ��ܣ�����Z�룬��ȴA��

	XIANNV_SHENGWU_SKILL_TYPE_COUNT
};

struct XiannvItem
{
	XiannvItem() { this->Reset(); }

	void Reset()
	{
		xn_level = 0;
		xn_zizhi = 0;
	}

	short xn_level;
	short xn_zizhi;
};

struct NvshenShengwuItem
{
	NvshenShengwuItem() { this->Reset(); }

	void Reset()
	{
		level = 0;
		reserve_sh = 0;
		exp = 0;
	}

	short level;
	short reserve_sh;
	int exp;
};

struct XiannvParam
{
	bool IsXnActived()
	{
		return active_xiannv_flag != 0;
	}

	bool IsXiannvActive(int xiannv_id) 
	{
		return 0 != (active_xiannv_flag & (1 << xiannv_id));
	}

	void ActiveXiannv(int xiannv_id) 
	{
		active_xiannv_flag |= (1 << xiannv_id); 
	}

	bool IsHuanHuaActive(int huanhua_id) 
	{
		return BitmapFunction::IsBitSet(active_huanhua_flag, sizeof(active_huanhua_flag), huanhua_id);
	}

	void ActiveHuanHua(int huanhua_id) 
	{
		BitmapFunction::SetBit(active_huanhua_flag, sizeof(active_huanhua_flag), huanhua_id);
	}

	bool IsShengwuActive(int shengwu_id)
	{
		if (shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID)
		{
			return false;
		}
		return 0 != all_shengwu_list[shengwu_id].level;
	}

	void ActiveShengwu(int shengwu_id)
	{
		if (shengwu_id < 0 || shengwu_id > XIANNV_SHENGWU_MAX_ID)
		{
			return;
		}
		if (0 != all_shengwu_list[shengwu_id].level)
		{
			return;
		}
		all_shengwu_list[shengwu_id].level = 1;
	}

	void Reset()
	{
		active_xiannv_flag = 0;
		memset(active_huanhua_flag, 0, sizeof(active_huanhua_flag));
		huanhua_id = -1;
		reserved = 0;
		memset(xiannv_name, 0, sizeof(xiannv_name));
		reserve_1 = 0;
		reserve_2 = 0;

		for (int i = 0; i < MAX_XIANNV_HUANHUA_COUNT; ++i)
		{
			xiannv_huanhua_level[i] = 0;			
		}

		for (int i = 0; i < MAX_XIANNV_COUNT; ++i)
		{
			xn_item_list[i].Reset();
		}

		memset(pos_list, -1, sizeof(pos_list));

		shengwu_lingye = 0;
		cur_gold_miling_times = 0;
		shengwu_chou_id = -1;
		memset(shengwu_chou_exp_list, 0, sizeof(shengwu_chou_exp_list));
		upgrade_shengwu_id = -1;
		memset(upgrade_exp_list, 0, sizeof(upgrade_exp_list));
		memset(all_shengwu_list, 0, sizeof(all_shengwu_list));
		memset(grid_level_list, 0, sizeof(grid_level_list));
		day_free_miling_times = 0;
		day_fetch_ling_time = 0;
		for (int i = 0; i < XIANNV_SHENGWU_MILING_FONT_COUNT; ++i)
		{
			miling_list[i] = -1; // ��ʼ������Ч����
		}
		reserve_sh = 0;
		shengwu_essence = 0;

		capability = 0;
	}

	int active_xiannv_flag;							// �������Ů�б�
	unsigned char active_huanhua_flag[MAX_XIANNV_ACTIVE_SPECIAL_IMAGE_BYTE];							// ����Ļû��б�
	short huanhua_id;									// �û�id
	short reserved;
	GameName xiannv_name[MAX_XIANNV_COUNT];				// ����
	int reserve_1;										// ����1
	int reserve_2;										// ����2
	int xiannv_huanhua_level[MAX_XIANNV_HUANHUA_COUNT];	// ��Ů�û��ȼ�

	XiannvItem xn_item_list[MAX_XIANNV_COUNT];
	char pos_list[MAX_XIANNV_POS];						// ��Ů����λ�� 0Ϊ��սλ

	int shengwu_lingye;									// ʥ����Һ
	short cur_gold_miling_times;						// ��ǰ��Ԫ�������������ȡ���������㣩
	short shengwu_chou_id;								// δ��ȡ�ĳ齱exp����ʥ��id
	short shengwu_chou_exp_list[XIANNV_SHENGWU_CHOU_EXP_COUNT];		// δ��ȡ�ĳ齱exp����
	int upgrade_shengwu_id;							// δ��ȡ������exp����ʥ��id
	int upgrade_exp_list[XIANNV_SHENGWU_MAX_ID + 1];	// δ��ȡ������exp����
	NvshenShengwuItem all_shengwu_list[XIANNV_SHENGWU_MAX_ID + 1];

	short grid_level_list[XIANNV_SHENGWU_GONGMING_MAX_GRID_ID + 1];
	char day_free_miling_times;							// ��������������
	char day_fetch_ling_time;							// �����ȡ��Һ����

	char miling_list[XIANNV_SHENGWU_MILING_FONT_COUNT];	// ��ǰ�鵽���������б�
	short reserve_sh;
	int shengwu_essence;								// ���е���������
	int capability;										// ս����
};

#pragma pack(pop)

typedef char XiannvParamHex[sizeof(XiannvParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(XiannvParamHex) < 2048);

#endif // __XIANNV_PARAM_HPP__
