#ifndef __TITLE_CONFIG_HPP__
#define __TITLE_CONFIG_HPP__

#include <map>
#include <vector>

#include "servercommon/servercommon.h"
#include "servercommon/rankdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

enum TITLESOURCETYPE
{
	TITLESOURCETYPE_INVALID = 0,					

	TITLESOURCETYPE_TITLECARD = 1,					// 称号卡
	TITLESOURCETYPE_RANK = 2,						// 排行榜
	TITLESOURCETYPE_ACTIVITY = 3,					// 活动
	TITLESOURCETYPE_OTHER = 5,						// 其他杂项
	TITLESOURCETYPE_CROSS_RANK = 6,					// 跨服排行榜
	TITLESOURCETYPE_PATAFB = 7,						// 爬塔
	TITLESOURCETYPE_JINGLINGCARD = 8,				// 精灵卡牌
	TITLESOURCETYPE_FAMOUS_MAN = 9,					// 名人堂
	TITLESOURCETYPE_CHALLENGE_FIELD = 10,			// 竞技场
	TITLESOURCETYPE_CHALLENGE_OUTSIDE_CFG = 11,		// 外部称号，不在TitlePool中检查
	TITLESOURCETYPE_RED_EQUIP_COLLECT = 12,			// 红装收集
	TITLESOURCETYPE_CROSS_GUILD_RANK = 13,			// 跨服公会榜
	TITLESOURCETYPE_CROSS_CHALLENGE_FIELD = 14,		// 跨服竞技场

	TITLESOURCETYPE_MAX,
};

enum TITLE_OTHER_TYPE
{
	TITLE_OTHER_TYPE_INVALID = 0, 

	TITLE_OTHER_TYPE_MAX,
};

enum TITLE_TYPE_CONDITION
{
	TITLE_TYPE_BEGIN = 0,
	TITLE_TYPE_XUNBAO = TITLE_TYPE_BEGIN,				// 装备寻宝达多少次
	TITLE_TYPE_LIEQU = 1,								// 精灵寻宝达多少次
	TITLE_TYPE_SHIZHUANG = 2,							// 拥有时装x件
	TITLE_TYPE_WEAPON = 3,								// 拥有武器x件
	TITILE_TYPE_PHANTOM = 4,							// 拥有精灵幻化x个
	TITLE_TYPE_HCHZ_KILLER = 5,							// 皇城会战杀x个
	TITLE_TYPE_GCZ_100_KILLER = 6,						// 攻城战100
	TITLE_TYPE_GCZ_1000_KILLER = 7,						// 攻城战1000
	TITLE_TYPE_GCZ_10000_KILLER = 8,					// 攻城战10000

	TITLE_TYPE_MAX,	
};

struct TitleCfg
{
	TitleCfg() : title_id(0), source_type(0), valid_secs(0),
				 maxhp(0), gongji(0), fangyu(0), gongji_add_per(0), fangyu_add_per(0), max_hp_add_per(0),is_broadcast(false)
	{}

	unsigned short title_id;							// 称号ID
	short source_type;									// 称号来源
	int valid_secs;										// 有效期 秒
	Attribute maxhp;									// 气血
	Attribute gongji;									// 攻击
	Attribute fangyu;									// 防御
	Attribute gongji_add_per;							// 全身攻击万分比加成
	Attribute fangyu_add_per;							// 全身防御万分比加成
	Attribute max_hp_add_per;							// 全身气血万分比加成
	TitleName title_name;								// 称号名称
	bool is_broadcast;									// 是否传闻
};

struct MingrentangCfg
{
	MingrentangCfg() : mingrentang_id(0), title_id(0) {}

	unsigned short mingrentang_id;						// 名人堂ID
	unsigned short title_id;							// 给的称号ID
};

struct RankTitleCfg
{
	RankTitleCfg() : is_valid(false),level_limit(0)
	{
		memset(title_id_list, 0, sizeof(title_id_list));
	}

	bool is_valid;
	int level_limit;
	unsigned short title_id_list[TOP_TEN + 1];			// 给的称号ID， 下标即名次
};

struct CrossRankTitleCfg 
{
	const static int CROSS_RANK_TITLE_MAX_RANK = 10;

	CrossRankTitleCfg() : is_valid(false)
	{
		memset(title_id_list, 0, sizeof(title_id_list));
	}

	bool is_valid;
	unsigned short title_id_list[CROSS_RANK_TITLE_MAX_RANK + 1];// 给的称号ID， 下标即名次
};

const static int ZHANSHENDIAN_TITLE_MAX_COUNT = 100;

struct ZhanShenDianTitleCfg
{
	ZhanShenDianTitleCfg() : pass_level(0), title_id(0) {}

	int pass_level;
	unsigned short title_id;
};

const static int PATA_FB_TITLE_MAX_COUNT = 50;

struct PataFbTitleCfg
{
	PataFbTitleCfg() : pass_level(0), title_id(0) {}

	int pass_level;
	unsigned short title_id;
};

const static int JINGLINGCARD_TITLE_MAX_COUNT = 30;
struct JingLingCardTitleCfg
{
	JingLingCardTitleCfg() : level(0), title_id(0) {}

	int level;
	unsigned short title_id;
};

const static int FAMOUS_MAN_TITLE_MAX_COUNT = 30;
struct FamousManTitleCfg
{
	FamousManTitleCfg() : famous_type(0), title_id(0) {}

	int famous_type;
	unsigned short title_id;
};

const static int CHALLENGE_FIELD_TITLE_MAX_COUNT = 4;
struct ChallengeFieldTitleCfg
{
	ChallengeFieldTitleCfg() : min_rank_pos(0), max_rank_pos(0), title_id(0), reverse(0)
	{}

	short min_rank_pos;
	short max_rank_pos;
	unsigned short title_id;
	short reverse;
};

const static int JINGLING_BINGFENG_ID = 15015;
const static int JINGLING_HUOFENG_ID = 15016;
const static unsigned short JINGLINGWANG_TITLE_ID = 6020;
struct TitleConditonCfg
{
	TitleConditonCfg(): type(0), condition(0), title_id(0)
	{}

	int type;
	int condition;
	int title_id;
};

struct TitleUpgradeConfig
{
	TitleUpgradeConfig() { this->Reset(); }

	void Reset()
	{
		title_id = 0;
		grade = 0;
		stuff_id = 0;
		gongji = 0;
		fangyu = 0;
		max_hp = 0;
		gongji_add_per = 0;
		fangyu_add_per = 0;
		max_hp_add_per = 0;
	}

	short title_id;
	int grade;
	ItemID stuff_id;
	int stuff_num;
	Attribute gongji;
	Attribute fangyu;
	Attribute max_hp;
	int gongji_add_per;
	int fangyu_add_per;
	int max_hp_add_per;
};

class TitlePool : public ConfigBase
{
public:
	TitlePool();
	~TitlePool();

	bool Init(const std::string &configname, std::string *err);

	const TitleCfg * GetTitleCfg(unsigned short title_id) const;
	unsigned short GetTopRankTitle(int rank_top_type, int rank,int level) const;
	unsigned short GetActivityTitle(int activity_type) const;
	unsigned short GetOtherTitle(int other_title_type) const;
	unsigned short GetCrossTopRankTitle(int rank_top_type, int rank) const;
	unsigned short GetPataFbTitle(int max_pass_level) const;
	unsigned short GetJinglingCardTitle(int card_level) const;
	unsigned short GetConditionTitle(int type, int condition) const;
	const TitleUpgradeConfig * GetUpgradeCfg(int title_id, int grade);
	unsigned short GetFamousManTitle(int famous_type) const;
	unsigned short GetChallengeFieldRankTitle(int rank_pos) const;

private:
	int InitTitle(PugiXmlNode RootElement);
	int InitRankTitle(PugiXmlNode RootElement);
	int InitActivityTitle(PugiXmlNode RootElement);
	int InitOtherTitle(PugiXmlNode RootElement);
	int InitCrossRankTitle(PugiXmlNode RootElement);
	int InitPataFbTitle(PugiXmlNode RootElement);
	int InitJingLingCardTitle(PugiXmlNode RootElement);
	int InitCondition(PugiXmlNode RootElement);
	int InitTitleUpgrade(PugiXmlNode RootElement);
	int InitFamousManTitle(PugiXmlNode RootElement);
	int InitChallengeFieldTitle(PugiXmlNode RootElement);

	bool IsTitleInRankList(unsigned short title_id);
	bool IsTitleInActivity(unsigned short title_id);
	bool IsTitleInOther(unsigned short title_id);
	bool IsTitleInPataFb(unsigned short title_id);
	bool IsTitleInJinglingCard(unsigned short title_id);
	bool IsTitleInFamousMan(unsigned short title_id);

	std::map<unsigned short, TitleCfg> m_title_map;
	std::map<unsigned short, MingrentangCfg> m_mrt_map;
	RankTitleCfg m_rank_title_list[RANK_TOP_TYPE_MAX];
	CrossRankTitleCfg m_cross_rank_title_list[CROSS_RANK_TOP_TYPE_MAX];
	std::map<int, unsigned short> m_activity_title_map;
	std::map<int, unsigned short> m_other_title_map;

	int m_pata_title_count;
	PataFbTitleCfg m_patafb_title_list[PATA_FB_TITLE_MAX_COUNT];

	int m_jingling_title_count;
	JingLingCardTitleCfg m_jinglingcard_title_list[JINGLINGCARD_TITLE_MAX_COUNT];

	int condition_count;
	std::map<int, TitleConditonCfg> m_condition_map;

	typedef std::vector<TitleUpgradeConfig> TitleGradeVec;
	TitleGradeVec m_upgrade_cfg_vec;

	int m_famous_man_title_count;
	FamousManTitleCfg m_famous_man_title_list[FAMOUS_MAN_TITLE_MAX_COUNT];

	int m_challenge_field_title_count;
	ChallengeFieldTitleCfg m_challenge_field_title_list[CHALLENGE_FIELD_TITLE_MAX_COUNT];   // 竞技场现在直接发送邮件，这个暂时无用，不再读取配置
};

#endif // __TITLE_CONFIG_HPP__

