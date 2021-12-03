#ifndef __ZHUANZHI_CONFIG_HPP__
#define __ZHUANZHI_CONFIG_HPP__

#include <map>
#include <string.h>
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/taskdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

static const int MAX_ZHUANZHI_TIANMING_LEVEL = 12;           // �������������
static const int MAX_ZHUANZHI_CANGLONG_LEVEL = 21;           // �������������
static const int MAX_ZHUANZHI_SHENGXIAO_LEVEL = 12;          // ��Ф���������
static const int MAX_ZHUANZHI_YUANQI_LEVEL = 12;             // Ԫ�����������
static const int MAX_ZHUANZHI_SHENQI_LEVEL = 12;             // �������������
static const int MAX_ZHUANZHI_XINGXIU_LEVEL = 28;            // �������������

static const int ONE_KEY_FINISH_TASK_PROF_LEVEL_LIMIT = 3;	// һ�����תְ�ȼ�����
static const int TIANMING_LIGHT_UP_PROF_LEVEL_LIMIT = 2;	// ��������תְ�ȼ�����

static const int CANGLONG_LIGHT_UP_PROF_LEVEL_LIMIT = 3;	// ��������תְ�ȼ�����
static const int MAX_CANGLONG_LEVEL_PER_GRADE = 7;          // �������׶�ÿ�׶��߸�
static const int UP_PROF6_ROLE_LEVEL_LIMIT = 500;			// תְ�ɹ���ɫ�ȼ�����

static const int SHENGXIAO_LIGHT_UP_PROF_LEVEL_LIMIT = 4;	// ��Ф����תְ�ȼ�����
static const int YUANQI_LIGHT_UP_PROF_LEVEL_LIMIT = 5;	    // Ԫ������תְ�ȼ�����
static const int SHENQI_LIGHT_UP_PROF_LEVEL_LIMIT = 6;	    // ��������תְ�ȼ�����
static const int XINGXIU_LIGHT_UP_PROF_LEVEL_LIMIT = 7;	    // ���޵���תְ�ȼ�����

static const int PER_PROF_MAX_FB_INDEX = 2;                 // תְ���� 

static const int ZHUANZHI_VIR_SKILL_TYPE_MAX_COUNT = 30;    // ���⼼�ܳ��������

struct RoleZhuanZhiOtherCfg
{
	RoleZhuanZhiOtherCfg() : change_prof_stuff_id(0), normal_skill_fix_hurt_per(0),
		team_injure_increase_per(0), hurt_boss_injure_add_per(0), one_key_level3_task_cost_gold(0), 
		role_level_to_prof_level5(0), one_key_item_price_1(0), one_key_item_price_2(0), 
		one_key_item_id_1(0), one_key_item_id_2(0), fb_kick_time_s(0){}

	ItemID change_prof_stuff_id;			// �����תְҵ��������Ʒ
	int normal_skill_fix_hurt_per;			// ��ת �չ��ٷֱ���80%���ӵ�100%
	int team_injure_increase_per;			// ��ת �⻷�����Ӷ����г�Ա10%�˺������ɵ���	
	int hurt_boss_injure_add_per;			// ��ת ����������10%��BOSS���˺�
	int one_key_level3_task_cost_gold;		// һ�������ת���񻨷ѵĽ��
	int role_level_to_prof_level5;			// ���5ת����Ľ�ɫ�ȼ�����

	int one_key_item_price_1;               // һ��תְ����1
	int one_key_item_price_2;				// һ��תְ����2
	int one_key_item_id_1;                  // һ��תְ����1
	int one_key_item_id_2;				    // һ��תְ����2

	int fb_kick_time_s;                     // תְ�����ӳ��߳�ʱ��
};

struct ZhuanzhiAttrCfg
{
	ZhuanzhiAttrCfg() : maxhp(0), gongji(0), fangyu(0), per_pojia(0)
	{

	}

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute per_pojia;
};

struct ZhuanZhiLevelSkillConfig
{
	ZhuanZhiLevelSkillConfig() {}

	static const int MAX_CHANGE_SKILL_NUM = 4;
	static const int MAX_BASE_PROF_TYPE = PROF_TYPE_PROF_NO_LIMIT - 1;

	struct ConfigItem
	{
		ConfigItem(): pvp_zengshang_per(0), pvp_jianshang_per(0), pve_zengshang_per(0), pve_jianshang_per(0), add_target_num(0), role_level_attr_per(0)
		{
			memset(add_skill_list, 0, sizeof(add_skill_list));
		}

		int pvp_zengshang_per;
		int pvp_jianshang_per;
		int pve_zengshang_per;
		int pve_jianshang_per;
		int add_target_num;
		int role_level_attr_per;
		int add_skill_list[MAX_CHANGE_SKILL_NUM];
	};

	ConfigItem config_list[MAX_BASE_PROF_TYPE + 1];
};

struct ZhuanzhiFbCfg
{
	ZhuanzhiFbCfg() : prof_level(0), level_limit(0), scene_id(0), boss_id(0), boss_pos(0, 0), enter_pos(0, 0) {}

	int prof_level;
	int level_limit;
	int scene_id;
	int boss_id;
	Posi boss_pos;
	Posi enter_pos;
};

enum ZHUANZHI_VIR_SKILL_TYPE
{
	ZHUANZHI_VIR_SKILL_TYPE_INVALID = 0,
	ZHUANZHI_VIR_SKILL_TYPE_RECOVER_FIX,				// �̶���Ѫp1
	ZHUANZHI_VIR_SKILL_TYPE_RECOVER,					// ����Ѫ��>=p1��ֱ�, ��Ѫp2��ֱȣ������Ѫp3��ֱȣ����p4�����������p5����
	ZHUANZHI_VIR_SKILL_TYPE_PVE_RECOVER,				// ��Ѫ�˺���p1��ֱȣ�������p2
	ZHUANZHI_VIR_SKILL_TYPE_PVP_XUANYUN,				// ѣ��(ֻ�������Ч), ����p1����
	ZHUANZHI_VIR_SKILL_TYPE_NORMAL_HPSTORE,				// ��ͨ���� ��ǰHP��p1��ֱȣ�����p2����, �滻�˺�p3��ֱ�
	ZHUANZHI_VIR_SKILL_TYPE_WUDI_HPSTORE,				// �޵л���, ����p1����
	ZHUANZHI_VIR_SKILL_TYPE_UP_EXP_PER,					// ����p1��ֱȾ���

	ZHUANZHI_VIR_SKILL_TYPE_XUANYUN_MIANYI,				// �ܵ�ѣ��buffʱ����������ѣ�Σ��ɹ����Խ�CD
	ZHUANZHI_VIR_SKILL_TYPE_DEADLY_ATTACK_MIANYI,		// ����������һ��, �ָ�MAXHP��p1��ֱ�Ѫ��

	ZHUANZHI_VIR_SKILL_TYPE_PVM_UP_SHANGHAI = 10,		// ���ӶԹ����˺�p1��ֱ�
	ZHUANZHI_VIR_SKILL_TYPE_PVE_UP_SHANGHAI,			// ����pve�˺�p1��ֱ� ����p2����
	ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI,			// ����pvp�Է�Ѫ��p1��ֱ��˺�, ����p2����
	ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI_2,			// �Է����Ѫ������p1��ֱ� ����PVP�˺�p2��ֱ� ����p3����
	ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI_3,			// ����pvp�˺�p1��ֱ�, ����p2����

	ZHUANZHI_VIR_SKILL_TYPE_HURT_INTERVAL,				// ÿ����ɹ���p1��ֱȵ��˺�, ����p2����
	ZHUANZHI_VIR_SKILL_TYPE_HURT_1,						// ÿ�ι�����������p1��ֱȵ��˺�, ����p2����
	ZHUANZHI_VIR_SKILL_TYPE_HURT_2,						// ��������p1��ֱ���ʵ�˺�
	ZHUANZHI_VIR_SKILL_TYPE_HURT_3,						// �Ե�����ɹ���p1��ֱ��˺�����ʹ���޷��ƶ�������p2����
	ZHUANZHI_VIR_SKILL_TYPE_HURT_4,						// ����p1��ֱ��˺�������p2����
	ZHUANZHI_VIR_SKILL_TYPE_HURT_5 = 20,				// ����p1��ֱ��˺�������p2����

	ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI_4,			// ��ɱһ����ң�����PVP�˺�p1��ֱȣ�������p2��, ����p3����
	ZHUANZHI_VIR_SKILL_TYPE_PVP_DOWN_SHANGHAI,			// ÿ���ܵ�����������PVP�˺�p1��ֱȣ�������p2�㣬����p3����

	ZHUANZHI_VIR_SKILL_TYPE_PVP_REBOUND,				// �����˺�p1��ֱ�(ֻ�������Ч)
	ZHUANZHI_VIR_SKILL_TYPE_PVP_CHENMO,					// ��Ĭ(ֻ�������Ч), ����p1����

	ZHUANZHI_VIR_SKILL_TYPE_MAX,
};

struct ZhuanzhiVirSkillCfg
{
	ZhuanzhiVirSkillCfg() : vir_skill_seq(0), vir_skill_type(0), follow_perform_skill_id(0), prof_level(0), cd_time(0), rate(0), param_1(0), param_2(0), param_3(0), param_4(0), param_5(0){}

	bool IsValid()
	{
		return (vir_skill_type > ZHUANZHI_VIR_SKILL_TYPE_INVALID && vir_skill_type < ZHUANZHI_VIR_SKILL_TYPE_MAX);
	}

	int vir_skill_seq;
	int vir_skill_type;
	int follow_perform_skill_id;
	int prof_level;
	int cd_time;
	int rate;
	int param_1;
	int param_2;
	int param_3;
	int param_4;
	int param_5;
};

struct ZhuanzhiStruct
{
	ZhuanzhiStruct() : level(0), pretaskid(INVALID_TASK_ID), need_stuff_id(0), need_stuff_num(0), need_exp(0), gongji(0), maxhp(0), fangyu(0), per_pojia(0), exp_factor(0){}

	int level;
	TaskID pretaskid;
	int need_stuff_id;
	int need_stuff_num;
	long long need_exp;
	Attribute gongji;
	Attribute maxhp;
	Attribute fangyu;
	Attribute per_pojia;
	double exp_factor;
};

class ZhuanzhiConfig : public ConfigBase
{
public:
	ZhuanzhiConfig();
	~ZhuanzhiConfig();

	bool Init(const std::string &path, std::string *err);

	const RoleZhuanZhiOtherCfg &GetRoleZhuanZhiOtherCfg() { return m_other_cfg; }

	bool GetChangeItemID(ItemID old_itemid, int to_prof, ItemID *new_itemid);

	const ZhuanzhiAttrCfg *GetZhuanzhiAttrCfg(int complete_count) const;
	const ZhuanZhiLevelSkillConfig::ConfigItem *GetZhuanzhiSkillCfg(int prof_level, int prof_type) const;

	const ZhuanzhiVirSkillCfg *GetZhuanzhiVirSkillCfg(int vir_skill_seq) const;

	const ZhuanzhiFbCfg *GetZhuanzhiFbCfg(int prof_level, int index) const;

	const ZhuanzhiStruct *GetZhuanzhiTianMingCfg(short level) const;

	const ZhuanzhiStruct *GetZhuanzhiCangLongCfg(short level) const;

	const ZhuanzhiStruct *GetZhuanzhiShengXiaoCfg(short level) const;
	const ZhuanzhiStruct *GetZhuanzhiYuanQiCfg(short level) const;
	const ZhuanzhiStruct *GetZhuanzhiShenQiCfg(short level) const;
	const ZhuanzhiStruct *GetZhuanzhiXingXiuCfg(short level) const;

	ItemID GetZhuanzhiStuffID() { return m_other_cfg.change_prof_stuff_id; }

	int GetZhuanzhiSkillMAxCount() { return m_max_vir_skill_count; }

private:
	int InitItemChangeCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitZhuanzhiAttrCfg(PugiXmlNode RootElement);
	int InitZhuanzhiSkillCfg(PugiXmlNode RootElement);
	int InitZhuanzhiVirSkillCfg(PugiXmlNode RootElement);
	int InitZhuanzhiFbCfg(PugiXmlNode RootElement);
	int InitZhuanzhiTianMingCfg(PugiXmlNode RootElement);
	int InitZhuanzhiCangLongCfg(PugiXmlNode RootElement);
	int InitZhuanzhiShengXiaoCfg(PugiXmlNode RootElement);
	int InitZhuanzhiYuanQiCfg(PugiXmlNode RootElement);
	int InitZhuanzhiShenQiCfg(PugiXmlNode RootElement);
	int InitZhuanzhiXingXiuCfg(PugiXmlNode RootElement);

	const static int CHANGE_ITEM_CFG_MAX_COUNT = 1024;

	struct ProfChangeItemCfg
	{
		ProfChangeItemCfg()
		{
			memset(item_list, 0, sizeof(item_list));
		}

		ItemID item_list[PROF_TYPE_PROF_NO_LIMIT];
	};

	int m_change_item_cfgcount;
	ProfChangeItemCfg m_change_item_cfglist[CHANGE_ITEM_CFG_MAX_COUNT];
	std::map<ItemID, int> m_itemid_cfgindex_map;

	RoleZhuanZhiOtherCfg m_other_cfg;

	ZhuanzhiAttrCfg m_zhuanzhi_attr_cfg[ROLE_ZHUANZHI_TASK_TOTAL_COUNT_MAX + 1];

	ZhuanZhiLevelSkillConfig m_zhuanzhi_level_skill_cfg[MAX_PROF_LEVEL + 1];

	int m_max_vir_skill_count;
	ZhuanzhiVirSkillCfg m_zhuanzhi_vir_skill_cfg_list[ZHUANZHI_VIR_SKILL_TYPE_MAX_COUNT];

	ZhuanzhiFbCfg m_zhuanzhi_fb_cfg[MAX_PROF_LEVEL + 1][PER_PROF_MAX_FB_INDEX + 1];

	ZhuanzhiStruct m_zhuanzhi_tianming_cfg[MAX_ZHUANZHI_TIANMING_LEVEL];

	ZhuanzhiStruct m_zhuanzhi_canglong_cfg[MAX_ZHUANZHI_CANGLONG_LEVEL];

	ZhuanzhiStruct m_zhuanzhi_shengxiao_cfg[MAX_ZHUANZHI_SHENGXIAO_LEVEL];

	ZhuanzhiStruct m_zhuanzhi_yuanqi_cfg[MAX_ZHUANZHI_YUANQI_LEVEL];

	ZhuanzhiStruct m_zhuanzhi_shenqi_cfg[MAX_ZHUANZHI_SHENQI_LEVEL];

	ZhuanzhiStruct m_zhuanzhi_xingxiu_cfg[MAX_ZHUANZHI_XINGXIU_LEVEL];
};

#endif