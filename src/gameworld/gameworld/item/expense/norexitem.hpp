#ifndef __NOREX_ITEM_H__
#define __NOREX_ITEM_H__

#include "item/itembase.h"

class NorexItem : public ItemBase
{
public:
	enum NOREX_ITEM_TYPE
	{
		I_NOREX_ITEM_INVALID = 0,

		I_NOREX_ITEM_JINGYAN_DAN = 1,							// ���鵤
		I_NOREX_ITEM_BIND_COIN_CARD = 2,						// ��ͭǮ��
		I_NOREX_ITEM_TITLE_CARD = 3,							// �ƺſ� 
		I_NOREX_ITEM_RESERVE_1 = 4,								// ����
		I_NOREX_ITEM_XIANHUN_DAN = 5,							// �ɻ굤 
		I_NOREX_ITEM_SHENGJI_DAN = 6,							// ������ 
		I_NOREX_ITEM_YUANLI_DAN = 7,							// Ԫ���� 
		I_NOREX_ITEM_GOLD_BIND_CARD = 8,						// ��Ԫ����
		I_NOREX_ITEM_WING_BLESS_CARD = 9,						// �����鿨
		I_NOREX_ITEM_TILI_CARD = 10,							// ������
		I_NOREX_ITEM_FB_EQUIP_CARD = 11,						// ��װ������
		I_NOREX_ITEM_WING_SPECIAL_IMG_CARD = 12,				// ������������
		I_NOREX_ITEM_SPECIAL_MOUNT_IMG_CARD = 13,				// ������������
		I_NOREX_ITEM_SPECIAL_APPEARANCE_CARD = 14,				// ��������
		I_NOREX_ITEM_MOUNT_JINJIE_CARD = 15,					// ������׿�
		I_NOREX_ITEM_REDUCE_EVIL_CARD = 16,						// �������
		I_NOREX_ITEM_QIBING_JINJIE_CARD = 17,					// ������׿�
		I_NOREX_ITEM_SHIZHUANG_CARD = 18,						// ʱװ��
		I_NOREX_ITEM_RED_PAPER_CARD = 19,						// �����
		I_NOREX_ITEM_SHUXINGDAN = 20,							// ���Ե�
		I_NOREX_ITEM_HUNLI = 21,								// ����
		I_NOREX_ITEM_LINGJING = 22,								// �龫
		I_NOREX_ITEM_MOJING = 23,								// ħ��
		I_NOREX_ITEM_COIN_CARD = 24,							// �ǰ�ͭǮ��
		I_NOREX_ITEM_SHENGWANG = 25,							// ������
		I_NOREX_ITEM_CHENGJIU = 26,								// �ɾ͵�
		I_NOREX_ITEM_HP = 27,									// ��Ѫ˲��
		I_NOREX_ITEM_HP_CONTINUE = 28,							// ��Ѫ�����ظ�
		I_NOREX_ITEM_MP_CONTINUE = 29,							// ħ�������ظ�
		I_NOREX_ITEM_ADDJINGZHUN = 30,							// ����ҩˮ ���Ӿ�׼��
		I_NOREX_ITEM_ADDBAOJI = 31,								// ����ҩˮ ���ӱ�������
		I_NOREX_ITEM_INC_PUTON_LEVEL = 32,						// �߽�װ��ҩˮ
		I_NOREX_ITEM_INC_EXP_LEVEL = 33,						// ����ҩˮ �����ٷֱ� 
		I_NOREX_ITEM_EXP_BOTTLE = 34,							// ����ƿ
		I_NOREX_ITEM_FUMOFU = 35,								// ��ħ��
		I_NOREX_ITEM_HUOLI = 36,								// ����ҩˮ
		I_NOREX_ITEM_HP_DAN = 37,								// ��Ѫ��
		I_NOREX_ITEM_SZ_PROTECT = 38,							// ��װ������
		I_NOREX_ITEM_XIANZUNKA_CARD = 39,                       // ����
		I_NOREX_ITEM_JINGLING_HUNLI = 40,					    // �ɳ����
		I_NOREX_ITEM_LIEMING = 41,								// ����
		I_NOREX_ITEM_ANY_MONSTER_DROP_ACTIVITY_ITEM = 42,		// �����ӹ̶����Ե���Ʒ����ȫ������ֵ���ʱ���ڣ�
		I_NOREX_ITEM_PET_SPECIAL_IMG_CARD = 43,				    // ������������
		I_NOREX_ITEM_HUASHEN_CARD = 44,							// ����
		I_NOREX_ITEM_MULTI_MOUNT_CARD = 45,						// ˫�����Ｄ�(ʹ�ú󼤻�˫���������Ӧ���󣬲�����ָ������-��������)
		I_NOREX_ITEM_MULTI_MOUNT_SPECIAL_IMG_CARD = 46,			// ˫��������������
		I_NOREX_ITEM_MAGIC_CARD = 47,							// ħ��
		I_NOREX_ITEM_WUSHANG_EQUIP = 48,						// �������
		I_NOREX_ITEM_CROSS_BOSS_SCORE_CARD = 49,				// ���boss���ֿ�
		I_NOREX_ITEM_MAGIC_EQUIPMENT = 50,						// ħ��װ��
		I_NOREX_ITEM_SPECIAL_HALO_IMG_CARD = 51,				// �⻷��������
		I_NOREX_ITEM_SPECIAL_SHENGONG_IMG_CARD = 52,			// ����������
		I_NOREX_ITEM_SPECIAL_SHENYI_IMG_CARD = 53,				// ������������
		I_NOREX_ITEM_CHENGZHANGDAN = 54,						// �ɳ���
		I_NOREX_ITEM_ZHUANSHENG_EQUIP = 55,						// ת��װ��
		I_NOREX_ITEM_ZHUANSHENG_XIUWEI_DAN = 56,				// ת����Ϊ��
		I_NOREX_ITEM_XIAO_CHONG_WU	 = 57,						// С����
		I_NOREX_ITEM_CHINESE_ZODIAC = 58,                       // ����
		I_NOREX_ITEM_SUPPLY_XUEBAO = 59,                        // ����Ѫ��
		I_NOREX_ITEM_SPECIAL_FIGHTMOUNT_IMG_CARD = 60,          // ս��������������
		I_NOREX_ITEM_JINGLINGFAZHEN_IMG_CARD = 61,				// ���鷨����������
		I_NOREX_ITEM_JINGLINGGUANGHUAN_IMG_CARD = 62,			// ����⻷��������
		I_NOREX_ITEM_XIANNV_HUANHUA_ACTIVE_CARD = 63,			// ��Ů�û����󼤻
		I_NOREX_ITEM_ZHIBAO_HUANHUA_ACTIVE_CARD = 64,			// �����û���
		I_NOREX_ITEM_REDUCE_WEARY = 65,				            // ����bossƣ��ֵ
		I_NOREX_ITEM_SPECIAL_JINGLING_IMG_CARD = 66,			// ������������
		I_NOREX_ITEM_MOUNT_UP_ONE_GRADE_CARD = 67,				// ����ֱ��һ�׽��׿�
		I_NOREX_ITEM_FIGHT_MOUNT_UP_ONE_GRADE_CARD = 68,		// ս������ֱ��һ�׽��׿�
		I_NOREX_ITEM_WING_UP_ONE_GRADE_CARD = 69,				// ����ֱ��һ�׽��׿�
		I_NOREX_ITEM_HALO_UP_ONE_GRADE_CARD = 70,				// �⻷ֱ��һ�׽��׿�
		I_NOREX_ITEM_SHENGONG_UP_ONE_GRADE_CARD = 71,			// ��ֱ��һ�׽��׿�
		I_NOREX_ITEM_SHENYI_UP_ONE_GRADE_CARD = 72,				// ����ֱ��һ�׽��׿�
		I_NOREX_ITEM_RUNE_BOX = 73,								// ���ı���
		I_NOREX_ITEM_RUNE = 74,									// ����
		I_NOREX_ITEM_RUNE_TOWER_OFFLINE_TIME = 75,				// ����������ʱ�俨
		I_NOREX_ITEM_PUT_SHENGE = 76,							// �������ʱ�俨
		I_NOREX_ITEM_XINHUN = 77,								// �ǻ�
		I_NOREX_ITEM_SHENGE_SUIPIAN = 78,						// �����Ƭ
		I_NOREX_ITEM_LUCKY_RED_PAPER = 79,						// ���˺��
		I_NOREX_ITEM_GOLD_CAR = 80,								// Ԫ������ʹ�ú����Ƴ�ֵ�����������
		I_NOREX_ITEM_FB_TOWER_DEFEND_CARD = 81,					// ����������
		I_NOREX_ITEM_XIANNV_SHENGWU_ADD_LINGYE = 82,			// Ů��ʥ��-������Һ
		I_NOREX_ITEM_SPECIAL_FOOTPRINT_IMG_CARD = 83,			// �㼣��������
		I_NOREX_ITEM_FOOTPRINT_UP_ONE_GRADE_CARD = 84,			// �㼣ֱ��һ�׽��׿�
		I_NOREX_ITEM_SPECIAL_CLOAK_IMG_CARD = 85,				// ������������
		I_NOREX_ITEM_MARRY_EXP = 86,							// ���ӽ��ȼ�����
		I_NOREX_ITEM_SHENSHOU = 87,								// ����
		I_NOREX_ITEM_SHENYIN = 88,								// ��ӡϵͳ
		I_NOREX_ITEM_TIAN_XIANG = 89,							// ������Ʒ
		I_NOREX_ITEM_FABAO = 90,							    // �����������󼤻���Ʒ
		I_NOREX_ITEM_HUNYAN_YANHUA = 91,						// �̻�
		I_NOREX_ITEM_SHIZHUANG_UP_ONE_GRADE_CARD = 92,			// ʱװֱ��һ�׽��׿�
		I_NOREX_ITEM_SHENBING_UP_ONE_GRADE_CARD = 93,			// ���ֱ��һ�׽��׿�
		I_NOREX_ITEM_FABAO_UP_ONE_GRADE_CARD = 94,				// ����ֱ��һ�׽��׿�
		I_NOREX_ITEM_PVP_REWARD_ITEM = 95,                      // 1v1��3v3��������
		I_NOREX_ITEM_ITEM_SEAL = 96,							// ʥӡ
		I_NOREX_ITEM_YAOSHI_UP_ONE_GRADE_CARD = 97,				// ����ֱ��һ�׽��׿�
		I_NOREX_ITEM_TOUSHI_UP_ONE_GRADE_CARD = 98,				// ͷ��ֱ��һ�׽��׿�
		I_NOREX_ITEM_QILINBI_UP_ONE_GRADE_CARD = 99,			// �����ֱ��һ�׽��׿�
		I_NOREX_ITEM_MASK_UP_ONE_GRADE_CARD = 100,				// ���ֱ��һ�׽��׿�
		I_NOREX_ITEM_SPECIAL_YAOSHI_IMG_CARD = 101,				// ������������
		I_NOREX_ITEM_SPECIAL_TOUSHI_IMG_CARD = 102,				// ͷ����������
		I_NOREX_ITEM_SPECIAL_MASK_IMG_CARD = 103,				// �����������
		I_NOREX_ITEM_SPECIAL_QILINBI_IMG_CARD = 104,			// �������������
		I_NOREX_ITEM_PLANTING_TREE_SEED = 105,                  // ֲ������        
		I_NOREX_ITEM_UPGRADE_SYS_IMG_CARD = 106,				// ����ϵͳ��������
		I_NOREX_ITEM_UPGRADE_SYS_UP_ONE_GRADE_CARD = 107,		// ����ϵͳֱ��һ�׽��׿�
		I_NOREX_ITEM_BIG_SMALL_GOAL_SPECIAL_ATTR = 108,         // �����СĿ���������Լ��
		I_NOREX_ITEM_CROSS_HONOR = 109,							// �����ҫ
		I_NOREX_ITEM_TALENT_SKILL_BOOK = 110,					// �츳������
		I_NOREX_ITEM_GREATE_SOLDIER_CARD = 111,					// ������
		I_NOREX_ITEM_GREATE_SOLDIER_HUANHUA_CARD = 112,			// �����û���
		I_NOREX_ITEM_GREATE_SOLDIER_EXPE_CARD = 113,			// �������鿨
		I_NOREX_ITEM_SHENGWU_JINGHUA = 114,						// ��������
		I_NOREX_ITEM_GUILD_GONGZI = 115,						// ���˹���
		I_NOREX_ITEM_BOSS_REFRESH_CARD = 116,					// bossˢ�¿�
		I_NOREX_ITEM_DOUQIDAN = 117,							// ������ʹ��
		I_NOREX_ITEM_TIANSHENHUTI_EQUIP = 118,					// ��ĩװ��ʹ������װ�������ⱳ��
		I_NOREX_ITEM_MYTH = 119,								// ����Ʒ(���ȡ���ˣ������õġ��޸ĺ�ǵ�ɾ����ע)
		I_NOREX_ITEM_ZODIAC = 120,								// ����

		I_NOREX_ITEM_MAX,
	};

	NorexItem();
	~NorexItem();

	virtual bool Init(const char* path, std::string *err);
	virtual int Use(Role *role, const ItemGridData &item_data, short &num, short equip_index = -1) const;
	int GetParam1() const { return m_param1; }
	int GetParam2() const { return m_param2; }
	int GetParam3() const { return m_param3; }
	int GetParam4() const { return m_param4; }
	int GetParam5() const { return m_param5; }
	short GetUseType() const { return m_use_type; }

private:
	short m_use_type;
	short m_use_daytimes;
	int m_colddown_id;
	int m_server_colddown;
	int m_param1;
	int m_param2;
	int m_param3;
	int m_param4;
	int m_param5;
};

#endif //__NOREX_ITEM_H__
