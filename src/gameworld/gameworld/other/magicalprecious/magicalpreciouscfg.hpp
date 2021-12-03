#ifndef __MAGICAL_PRECIOUS_CONFIG_HPP__
#define __MAGICAL_PRECIOUS_CONFIG_HPP__

#include "servercommon/fbdef.hpp"
#include "servercommon/maildef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "gamedef.h"
#include "servercommon/configbase.h"
#include "servercommon/magicalpreciousdef.hpp"
#include "servercommon/pugixml/pugixml_adapter.hpp"


static const int MAGICAL_PRECIOUS_MAX_CHAPTER = MAGICAL_PRECIOUS_CHAPTER_COUNT;
static const int MAGICAL_PRECIOUS_MAX_REWARD_INDEX = MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT;
//static const int MAGICAL_PRECIOUS_MAX_REWARD_INDEX = MAGICAL_PRECIOUS_CHAPTER_COUNT * MAGICAL_PRECIOUS_CHAPTER_FLAG_COUNT;

enum CHAPTER_CONDITION_TYPE
{
	CONDITION_TYPE_INVALID = 0,

	CONDITION_TYPE_TOTAL_GONGJI,					// 攻击属性达到X
	CONDITION_TYPE_TOTAL_FANGYU,					// 防御属性达到X
	CONDITION_TYPE_TOTAL_MAXHP,						// 生命属性达到X
	CONDITION_TYPE_TOTAL_SHANBI,					// 闪避属性达到X
	CONDITION_TYPE_TOTAL_MINGZHONG,					// 命中属性达到X
	CONDITION_TYPE_TOTAL_BAOJI,						// 暴击属性达到X
	CONDITION_TYPE_TOTAL_JIANREN,					// 坚韧属性达到X

	CONDITION_TYPE_MOUNT_GRADE = 11,				// 坐骑阶数达到X
	CONDITION_TYPE_WING_GRADE,						// 羽翼阶数达到X
	CONDITION_TYPE_HALO_GRADE,						// 光环阶数达到X
	CONDITION_TYPE_SHENGONG_GRADE,					// 仙女光环阶数达到X
	CONDITION_TYPE_SHENYI_GRADE,					// 仙女法阵阶数达到X
	CONDITION_TYPE_FIGHT_MOUNT_GRADE,				// 战斗坐骑阶数达到X
	CONDITION_TYPE_SHIZHUANG_GRADE,					// 时装阶数达到X
	CONDITION_TYPE_SHENBING_GRADE,					// 神兵阶数达到X
	CONDITION_TYPE_FABAO_GRADE,						// 法宝阶数达到X
	CONDITION_TYPE_FOOTPRINT_GRADE,					// 足迹阶数达到X
	CONDITION_TYPE_JINEJIE_LEVEL = 21,				// 境界阶数达到X
	CONDITION_TYPE_MOUNT_STAR,						// 坐骑星级达到X
	CONDITION_TYPE_WING_STAR,						// 羽翼星级达到X
	CONDITION_TYPE_HALO_STAR,						// 光环星级达到X
	CONDITION_TYPE_SHENGONG_STAR,					// 仙女光环星级达到X
	CONDITION_TYPE_SHENYI_STAR,						// 仙女法阵星级达到X
	CONDITION_TYPE_FIGHT_MOUNT_STAR,				// 战斗坐骑星级达到X

	CONDITION_TYPE_ALL_CAPACITY = 31,				// 综合战力达到X
	CONDITION_TYPE_RUNESYSTER_CAPACITY,				// 符文系统战力达到X
	CONDITION_TYPE_SHENGE_CAPACITY,					// 神格战力达到X
	CONDITION_TYPE_SHENZHOU_WEAPON_CAPACITY,		// 神州六器战力达到X
	CONDITION_TYPE_CHINESE_ZODIAC_CAPACITY,			// 星座战力达到X
	CONDITION_TYPE_JINGLING_CAPACITY,				// 精灵战力达到X
	CONDITION_TYPE_JINGLING_CHENGZHANG,				// 精灵成长达到X
	CONDITION_TYPE_JINGLING_WUXING,					// 精灵悟性达到X
	CONDITION_TYPE_XIANNV_SHENWU_CAP,				// 仙女仙器战力达到X

	CONDITION_TYPE_EQUIP_STRENGTHEN = 41,			// 装备强化总等级达到X
	CONDITION_TYPE_EQUIP_STAR,						// 装备星级总等级达到X
	CONDITION_TYPE_EQUIP_STONE,						// 装备宝石总等级达到X
	CONDITION_TYPE_ROLE_LEVEL,						// 角色等级达到X
	CONDITION_TYPE_XIANNV_ZIZHI_LEVEL,				// 仙女资质等级达到X
	CONDITION_TYPE_RUNETOWER_LEVEL,					// 符文塔层数达到X
	CONDITION_TYPE_PATA_FB_LEVEL,					// 爬塔副本层数达到X
	CONDITION_TYPE_TEAMEQUIP_FB_LEVEL,				// 组队爬塔副本层数达到X
	CONDITION_TYPE_TUITU_FB_NORMAL,					// 普通推图本达到x章x节
	CONDITION_TYPE_TUITU_FB_HARD,					// 精英推图本达到x章x节
	CONDITION_TYPE_ARMORDEFEND_FB_LEVEL = 51,		// 防具材料本达到x章
	CONDITION_TYPE_TOWERDEFEND_FB_LEVEL,			// 单人塔防本达到x章
	CONDITION_TYPE_TOWERDEFEND_TEAM_FB_WAVE,		// 组队守护本通关x波
	CONDITION_TYPE_CHALLENGE_RANK,					// 竞技场达到前x名
	CONDITION_TYPE_NEWEQUIP_FB_LEVEL,				// 武器本达到x关

	CONDITION_TYPE_GUILD_TASK = 61,					// 完成公会任务总个数X
	CONDITION_TYPE_DAILY_TASK,						// 完成日常任务总个数X
	CONDITION_TYPE_WABAO,							// 挖宝总次数X
	CONDITION_TYPE_OPEN_GUILD_BOX,					// 开启公会宝箱总次数X
	CONDITION_TYPE_HOTSPRING_JOIN,					// 参与跨服温泉总次数X
	CONDITION_TYPE_HOTSPRING_ANSWER_RIGHT,			// 跨服温泉答题正确总次数X
	CONDITION_TYPE_MILLIONAIRE_BOX,					// 大富豪采集宝箱总次数X
	CONDITION_TYPE_HUSONG,							// 运镖总次数X
	CONDITION_TYPE_ENTER_EXP_FB,					// 进入经验副本总次数X
	CONDITION_TYPE_PASS_PHASE_FB,					// 通关阶段副本总次数X
	CONDITION_TYPE_WANGLING_TASK = 71,				// 完成王陵探险任务总次数X
	CONDITION_TYPE_TIANJIANGCAIBAO_TASK,			// 完成天降财宝任务总次数X
	CONDITION_TYPE_SHUIJING_TASK,					// 完成水晶幻境任务总次数X
	CONDITION_TYPE_SHUIJING_GATHER,					// 水晶幻境采集水晶总次数X
	CONDITION_TYPE_GONGCHENG_ZHAN,					// 参加攻城战总次数X
	CONDITION_TYPE_KILL_MIKU_BOSS,					// 击杀精英boss总次数X
	CONDITION_TYPE_KILL_WORLD_BOSS,					// 击杀世界boss总次数X
	CONDITION_TYPE_KILL_ACTIVE_BOSS,				// 击杀活跃boss总次数X
	CONDITION_TYPE_KILL_DABAO_BOSS,					// 击杀打宝boss总次数X
	CONDITION_TYPE_KILL_VIP_BOSS,					// 击杀VIPboss总次数X

	CONDITION_TYPE_KILL_VIP_MIKU_BOSS1 = 81,		// 击杀VIP + 精英bossX等级总次数Y
	CONDITION_TYPE_KILL_VIP_MIKU_BOSS2,				// 击杀VIP + 精英bossX等级总次数Y
	CONDITION_TYPE_KILL_VIP_MIKU_BOSS3,				// 击杀VIP + 精英bossX等级总次数Y
	CONDITION_TYPE_KILL_ROLE_IN_FIGHTING,			// 击杀决斗场中附近的人X
	CONDITION_TYPE_MINE_TIMES,						// 挖矿X次
	CONDITION_TYPE_FISH_NUM,						// 鱼塘养鱼X次
	CONDITION_TYPE_SAILING_NUM,						// 航海X次
	CONDITION_TYPE_ONLINE_TIME,						// 在线X小时
	CONDITION_TYPE_KILL_THREE_TYPE_BOSS,			// 击杀活跃BOSS、世界BOSS、精英BOSS总数达X
	CONDITION_TYPE_KILL_MONSTER_NUM,				// 击杀怪物X只
	CONDITION_TYPE_ACTIVE_DEGREE,					// 活跃度到达X
	CONDITION_TYPE_MARRY_SUCC,						// 成功结婚
	CONDITION_TYPE_MARRY_SUCC_TYPE,					// 成功结婚X档次
	CONDITION_TYPE_PROF_LEVEL,						// 达到转职X转
	CONDITION_TYPE_GONGCHENG_ZHAN_WINER,			// 所在帮派成为攻城战盟主

	ZHUANGBEI_CONDITION_TYPE = 100,					// 装备X阶的X颜色X部位装备
	STONE_CONDITION_TYPE,							// 穿戴一颗X等级X属性的宝石
	DUANZAO_CONDITION_TYPE,							// 锻造X阶的X部位套装
	CONDITION_TYPE_WEAR_EQUIPMENT,					// 装备X件Y阶Z永恒等级装备
	CONDITION_TYPE_WEAR_ZHUANZHI_EQUIPMENT,			// 装备X件Y阶Z品质转职装备
	CONDITION_TYPE_WEAR_MARRY_EQUIPMENT,			// 穿戴X件Y阶Z品质结婚装备
	CONDITION_TYPE_COLLECT_JINGLING,				// 仙宠装备栏中有X个Y品质以上仙宠
	CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT_FANG,		// 激活X件防具诛神Y阶套装
	CONDITION_TYPE_ACTIVE_SHENZHOU_WEAPON,			// 激活X个异火
	CONDITION_TYPE_XIANNV_GONGMING_LEVEL,			// 仙女伙伴--共鸣总等级达到X级
	CONDITION_TYPE_XIANNV_SHENGWU_LEVEL,			// 仙女伙伴--神器总等级达到X级

	CONDITION_TYPE_LIEMING_COUNT = 111,				// 镶嵌X个Y品质命魂
	CONDITION_TYPE_SHENGE_COUNT,					// 镶嵌X个Y品质神格
	CONDITION_TYPE_SHENSHOU_COUNT,					// 激活X个龙器
	CONDITION_TYPE_CHINESE_ZODIAC_LEVEL,			// 生肖总等级达到X级
	CONDITION_TYPE_SHENGQI_STRENGTH_LEVEL,			// 圣器总强化等级达到X级
	CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT,			// 激活转职X件Y套装
	CONDITION_TYPE_ACTIVE_QINGYUAN_EQUIP2,			// 激活X件情饰
	CONDITION_TYPE_ACTIVE_XIANNV,					// 激活X个Y资质等级的仙女
	CONDITION_TYPE_ZHUANZHI_STONE_TOTAL_LEVEL,		// 玉石总等级达到X级
	CONDITION_TYPE_TOTAL_SHENGWANG,					// 累计获得X点声望

	CONDITION_TYPE_ACTIVE_LUOSHU = 121,				// 收集X张Y品洛书卡牌
	CONDITION_TYPE_EQUIP_BAPTIZE,					// 洗炼获得X条Y品质及以上属性
	CONDITION_TYPE_GREATESOLDIER_ACTIVE,			// 激活X个Y品质及以上天神
	CONDITION_TYPE_SHENZHOU_WEAPON_INLAY_COUNT,		// 镶嵌X个Y品质魂石
	CONDITION_TYPE_SHENSHOU_EQUIP_COUNT,			// 穿戴X个Y品质龙器装备
	CONDITION_TYPE_UPGRADE_SYS_GRADE,				// X进阶系统阶数达到Y
	CONDITION_TYPE_SHENQI_INLAY_COUNT,				// 穿戴X件Y品质奇兵或遁甲
	CONDITION_TYPE_UPLEVEL_QINGYUAN_HALO,			// 升到X级,情缘光环Y
	CONDITION_TYPE_GREATESOLDIER_UPLEVEL,			// 升级X个以上天神潜能到Y级
	CONDITION_TYPE_SHENGQI_LEVEL_COUNT,				// X个圣器强化等级达到Y级
	CONDITION_TYPE_CHINESE_ZODIAC_LEVEL_COUNT,		// X个生肖等级达到Y级
	CONDITION_TYPE_CHINESE_ZODIAC_ACTIVE_COUNT,		// 激活X个Y品质生肖
	CONDITION_TYPE_SEAL_INLAY_COUNT,				// 穿戴X件Y品质以上圣印
	CONDITION_TYPE_COLLECT_JINGLING_SKILL,			// 装备X个Y品质以上仙宠技能
	CONDITION_TYPE_QILINGBI_GRADE,					// 麒麟臂阶数达到X
	CONDITION_TYPE_MASK_GRADE,						// 面具阶数达到X
	CONDITION_TYPE_TOUSHI_GRADE,					// 头饰阶数达到X
	CONDITION_TYPE_YAOSHI_GRADE,					// 腰饰阶数达到X
	CONDITION_TYPE_ACTIVE_QINGYUAN_HALO,			// 激活X颜色以上情缘光环Y个

	MAX_CONDITION_TYPE,
};

struct MagicalPreciousOtherCfg
{
	MagicalPreciousOtherCfg(): open_level(0)
	{}

	int open_level;
};

struct MagicalPreciousChapterCfg
{
	MagicalPreciousChapterCfg():chapter_id(0), reward_index(0), condition_type(0), shenwang_reward(0), mojing_reward(0), bind_gold_reward(0), param_a(0), param_b(0), param_c(0), get_score(0) {}

	short chapter_id;
	short reward_index;
	short condition_type;
	int shenwang_reward;
	int mojing_reward;
	int bind_gold_reward;
	int param_a;
	int param_b;
	int param_c;
	int get_score;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct MagicalPreciousFinishChapterRewardCfg
{
	static const int MAX_REWARD_ITEM_NUM = 6;

	MagicalPreciousFinishChapterRewardCfg():chapter_id(0), seq(0), need_score(0), reward_item_count(0) {}

	short chapter_id;
	short seq;
	int need_score;

	int reward_item_count;
	ItemConfigData reward_item[MAX_REWARD_ITEM_NUM];
};

class MagicalPreciousCfg : public ConfigBase
{
public:
	MagicalPreciousCfg();
	~MagicalPreciousCfg();

	bool Init(const std::string &configname, std::string *err);

	const int GetMaxChapter(){return m_max_chapter_count;}
	const int GetChapterMaxRewardIndex(int chapter);
	const MagicalPreciousOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const MagicalPreciousChapterCfg * GetChapterTaskCfg(short chapter, short reward_index);
	const MagicalPreciousFinishChapterRewardCfg * GetFinishChapterRewardCfg(int chapter, int seq);
	std::vector<const MagicalPreciousChapterCfg *> GetChapterCfgByCondition(int condition_type);

private:

	int InitOtherCfg(PugiXmlNode RootElement);
	int InitChapterTaskCfg(PugiXmlNode RootElement);
	int InitFinishChapterRewardCfg(PugiXmlNode RootElement);

	MagicalPreciousOtherCfg m_other_cfg;

	int m_max_chapter_count;
	int m_chapter_task_count[MAGICAL_PRECIOUS_MAX_CHAPTER];
	MagicalPreciousChapterCfg m_chapter_cfg[MAGICAL_PRECIOUS_MAX_CHAPTER][MAGICAL_PRECIOUS_MAX_REWARD_INDEX];

	std::vector<MagicalPreciousFinishChapterRewardCfg> m_chapter_finish_cfg_vec;
};

#endif // __MAGICAL_PRECIOUS_CONFIG_HPP__

