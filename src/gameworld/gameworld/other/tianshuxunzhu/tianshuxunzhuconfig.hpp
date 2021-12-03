#ifndef __TIANSHUXUNZHU_CONFIG_HPP__
#define __TIANSHUXUNZHU_CONFIG_HPP__

#include <vector>
#include <set>

#include "servercommon/xml_adapter.hpp"
#include "servercommon/configcommon.h"
#include "gameworld/gameworld/item/itempool.h"
#include "servercommon/activitydef.hpp"

enum TIANSHUXZ_TYPE			//天数寻主类型
{
	TIANSHUXZ_TYPE_EXP = 0,	 // 经验天书，目标为转职装防御
	TIANSHUXZ_TYPE_TUMO,	 // 屠魔天书，目标为穿戴x件x级装备
	TIANSHUXZ_TYPE_SHIXUE,	 // 嗜血天书，目标成长
	TIANSHUXZ_TYPE_SHOUHU,	 // 守护天书，目标为锻造x阶套装部位
	TIANSHUXZ_TYPE_BAIZHAN,	 // 百战天书, 穿戴X件百战装备
	TIANSHUXZ_TYPE_SHENSHOU, // 龙器天书, 激活龙器

	TIANSHU_TYPE_MAX,
};

UNSTD_STATIC_CHECK(TIANSHU_TYPE_MAX <= TIANSHU_XUNZHU_MAX_TYPE_COUNT);

enum TIANSHUXZ_COND_TYPE	//天数寻主目标类型
{
	TIANSHUXZ_COND_INVILAD = 0,
	TIANSHUXZ_COND_ZHUANZI_EQUIP,	 //转职装防御(参数：防御数)
	TIANSHUXZ_COND_HAVE_EQUIP,		//穿戴x级装备y件（参数：x级|y件）
	TIANSHUXZ_COND_CHENGZHANG,		//成长（参数：子类型）
	TIANSHUXZ_COND_FORGE,			//锻造x阶套装部位（部位|x阶）
	TIANSHUXZ_COND_BAIZHAN,			//百战
	TIANSHUXZ_COND_SHENSHOU,		//龙器
	TIANSHUXZ_COND_MAX,
}; 

enum CHENGZHANG_TIANSHU_TYPE		//天书寻主成长子类型
{
	CHENGZHANG_TIANSHU_TYPE_FINISH_SHOUCHONG = 0,		// 完成首充
	CHENGZHANG_TIANSHU_TYPE_ZERO_BUY_RETURN,			// 0元购 388
	CHENGZHANG_TIANSHU_TYPE_BUY_IMPGUARD,				// 购买小熊猫
	CHENGZHANG_TIANSHU_TYPE_ACTIVE_WUQI_SHIZHUANG,		// 激活1件武器时装
	CHENGZHANG_TIANSHU_TYPE_TOUTZHIJIHUA,				// 参与投资计划(boss或副本)
	CHENGZHANG_TIANSHU_TYPE_ZEROGIFT,					// 三星红装
	CHENGZHANG_TIANSHU_TYPE_SUPPER_GIFT,				// 礼包抢购
	CHENGZHANG_TIANSHU_TYPE_ONMARRY,					// 结婚

	CHENGZHANG_TIANSHU_TYPE_MAX,
};

UNSTD_STATIC_CHECK(CHENGZHANG_TIANSHU_TYPE_MAX < 31);

struct TianShuXZOtherCfg
{
	TianShuXZOtherCfg() : shixue_open_day_end(0){}
	int shixue_open_day_end;
};

struct TianShuXZGoalItem
{
	TianShuXZGoalItem() :seq(0), cond_type(0)
	{
	}
	int seq;
	int cond_type;
	std::vector<int> cond_param;
	ItemConfigData goal_reward[MAX_ATTACHMENT_ITEM_NUM];//奖励
};

struct TianShuXZGoalConfig
{
	TianShuXZGoalConfig() :type(-1),count(0){}

	int type;
	int count;
	TianShuXZGoalItem golal_list[TIANSHU_XUNZHU_SIGNLE_MAX_NUM];
};

struct TianshuXZSkillCfg
{
	TianshuXZSkillCfg() : type(0), isvalid(false), effect_per_val(0){}

	int type;
	bool isvalid;
	int effect_per_val;	// 效果（%）
};

class TianshuXZConfig : public ConfigBase
{
public:

	TianshuXZConfig();
	~TianshuXZConfig();

	bool Init(const std::string &configname, std::string *err);
	const TianShuXZOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const TianShuXZGoalConfig * GetTianShuXZGoalConfig(int type) const;
	const TianShuXZGoalItem * GetTianShuShuXZGoalItem(int type, int seq) const;
	int GetTianShuCountByType(int type) const;
	const TianshuXZSkillCfg* GetSKillCfg(int type)const;
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitGoalCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	
	TianShuXZOtherCfg m_other_cfg;

	int m_goal_count;
	TianShuXZGoalConfig m_goal_config[TIANSHU_TYPE_MAX];
	
	TianshuXZSkillCfg m_skill_config[TIANSHU_TYPE_MAX];
};

#endif	// __TIANSHUXUNZHU_CONFIG_HPP__