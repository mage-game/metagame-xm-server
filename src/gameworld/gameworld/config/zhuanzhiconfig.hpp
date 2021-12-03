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

static const int MAX_ZHUANZHI_TIANMING_LEVEL = 12;           // 天命最大配置数
static const int MAX_ZHUANZHI_CANGLONG_LEVEL = 21;           // 苍龙最大配置数
static const int MAX_ZHUANZHI_SHENGXIAO_LEVEL = 12;          // 生肖最大配置数
static const int MAX_ZHUANZHI_YUANQI_LEVEL = 12;             // 元气最大配置数
static const int MAX_ZHUANZHI_SHENQI_LEVEL = 12;             // 神气最大配置数
static const int MAX_ZHUANZHI_XINGXIU_LEVEL = 28;            // 星宿最大配置数

static const int ONE_KEY_FINISH_TASK_PROF_LEVEL_LIMIT = 3;	// 一键完成转职等级限制
static const int TIANMING_LIGHT_UP_PROF_LEVEL_LIMIT = 2;	// 天命点亮转职等级限制

static const int CANGLONG_LIGHT_UP_PROF_LEVEL_LIMIT = 3;	// 苍龙点亮转职等级限制
static const int MAX_CANGLONG_LEVEL_PER_GRADE = 7;          // 苍龙三阶段每阶段七个
static const int UP_PROF6_ROLE_LEVEL_LIMIT = 500;			// 转职成功角色等级限制

static const int SHENGXIAO_LIGHT_UP_PROF_LEVEL_LIMIT = 4;	// 生肖点亮转职等级限制
static const int YUANQI_LIGHT_UP_PROF_LEVEL_LIMIT = 5;	    // 元气点亮转职等级限制
static const int SHENQI_LIGHT_UP_PROF_LEVEL_LIMIT = 6;	    // 神器点亮转职等级限制
static const int XINGXIU_LIGHT_UP_PROF_LEVEL_LIMIT = 7;	    // 星宿点亮转职等级限制

static const int PER_PROF_MAX_FB_INDEX = 2;                 // 转职副本 

static const int ZHUANZHI_VIR_SKILL_TYPE_MAX_COUNT = 30;    // 虚拟技能池最大数量

struct RoleZhuanZhiOtherCfg
{
	RoleZhuanZhiOtherCfg() : change_prof_stuff_id(0), normal_skill_fix_hurt_per(0),
		team_injure_increase_per(0), hurt_boss_injure_add_per(0), one_key_level3_task_cost_gold(0), 
		role_level_to_prof_level5(0), one_key_item_price_1(0), one_key_item_price_2(0), 
		one_key_item_id_1(0), one_key_item_id_2(0), fb_kick_time_s(0){}

	ItemID change_prof_stuff_id;			// 这个是转职业的消耗物品
	int normal_skill_fix_hurt_per;			// 二转 普攻百分比由80%增加到100%
	int team_injure_increase_per;			// 二转 光环：增加队伍中成员10%伤害，不可叠加	
	int hurt_boss_injure_add_per;			// 二转 被动：增加10%对BOSS的伤害
	int one_key_level3_task_cost_gold;		// 一键完成三转任务花费的金币
	int role_level_to_prof_level5;			// 完成5转任务的角色等级限制

	int one_key_item_price_1;               // 一键转职材料1
	int one_key_item_price_2;				// 一键转职材料2
	int one_key_item_id_1;                  // 一键转职材料1
	int one_key_item_id_2;				    // 一键转职材料2

	int fb_kick_time_s;                     // 转职副本延迟踢出时间
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
	ZHUANZHI_VIR_SKILL_TYPE_RECOVER_FIX,				// 固定回血p1
	ZHUANZHI_VIR_SKILL_TYPE_RECOVER,					// 自身血量>=p1万分比, 回血p2万分比，否则回血p3万分比，提高p4点防御，持续p5毫秒
	ZHUANZHI_VIR_SKILL_TYPE_PVE_RECOVER,				// 回血伤害的p1万分比，不超过p2
	ZHUANZHI_VIR_SKILL_TYPE_PVP_XUANYUN,				// 眩晕(只对玩家有效), 持续p1毫秒
	ZHUANZHI_VIR_SKILL_TYPE_NORMAL_HPSTORE,				// 普通护盾 当前HP的p1万分比，持续p2毫秒, 替换伤害p3万分比
	ZHUANZHI_VIR_SKILL_TYPE_WUDI_HPSTORE,				// 无敌护盾, 持续p1毫秒
	ZHUANZHI_VIR_SKILL_TYPE_UP_EXP_PER,					// 增加p1万分比经验

	ZHUANZHI_VIR_SKILL_TYPE_XUANYUN_MIANYI,				// 受到眩晕buff时，概率免疫眩晕，成功与否皆进CD
	ZHUANZHI_VIR_SKILL_TYPE_DEADLY_ATTACK_MIANYI,		// 免疫致命的一击, 恢复MAXHP的p1万分比血量

	ZHUANZHI_VIR_SKILL_TYPE_PVM_UP_SHANGHAI = 10,		// 增加对怪物伤害p1万分比
	ZHUANZHI_VIR_SKILL_TYPE_PVE_UP_SHANGHAI,			// 增加pve伤害p1万分比 持续p2毫秒
	ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI,			// 增加pvp对方血量p1万分比伤害, 持续p2毫秒
	ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI_2,			// 对方玩家血量少于p1万分比 增加PVP伤害p2万分比 持续p3毫秒
	ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI_3,			// 增加pvp伤害p1万分比, 持续p2毫秒

	ZHUANZHI_VIR_SKILL_TYPE_HURT_INTERVAL,				// 每秒造成攻击p1万分比的伤害, 持续p2毫秒
	ZHUANZHI_VIR_SKILL_TYPE_HURT_1,						// 每次攻击附带攻击p1万分比的伤害, 持续p2毫秒
	ZHUANZHI_VIR_SKILL_TYPE_HURT_2,						// 对玩家造成p1万分比真实伤害
	ZHUANZHI_VIR_SKILL_TYPE_HURT_3,						// 对敌人造成攻击p1万分比伤害，并使其无法移动，持续p2毫秒
	ZHUANZHI_VIR_SKILL_TYPE_HURT_4,						// 增加p1万分比伤害，持续p2毫秒
	ZHUANZHI_VIR_SKILL_TYPE_HURT_5 = 20,				// 减少p1万分比伤害，持续p2毫秒

	ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI_4,			// 击杀一个玩家，增加PVP伤害p1万分比，最多叠加p2层, 持续p3毫秒
	ZHUANZHI_VIR_SKILL_TYPE_PVP_DOWN_SHANGHAI,			// 每次受到攻击，减少PVP伤害p1万分比，最多叠加p2层，持续p3毫秒

	ZHUANZHI_VIR_SKILL_TYPE_PVP_REBOUND,				// 反弹伤害p1万分比(只对玩家有效)
	ZHUANZHI_VIR_SKILL_TYPE_PVP_CHENMO,					// 沉默(只对玩家有效), 持续p1毫秒

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