/******************************************************************************************
FileName: countrydef.hpp
Author: dreamwang
Description: 军团相关定义  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/09/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __GUILD_DEF_HPP__
#define __GUILD_DEF_HPP__

#include <memory>
#include <map>
#include <vector>

#include "servercommon.h"
	
static const GuildID INVALID_GUILD_ID = 0;						// 无效的军团ID
static const int MAX_ONCE_GUILD_COUNT = 100;					// 一次操作最大军团数量
static const int GUILD_SAVE_INTERVAL = 60;						// 军团保存间隔
static const int GUILD_CLEAR_MEMBER_INTERVAL = 3;				// 自动清理公会成员间隔

static const int GUILD_LEVEL_MAX = 10;							// 军团最高等级

const static int GUILD_MAX_UPLEVEL_STUFF_COUNT = 8;				// 军团升级所需材料

const static int GUILD_MAX_EXCHANGE_ITEM_COUNT = 30;			// 军团最大兑换物品种类

static const int GUILD_FULI_GIFT_MAX = 6;						// 最多6个福利礼包领取

static const int GUILD_WARNING_DISMISS_DAY = 1;					// 军团解散警告为1天

static const int MAX_MEMBER_COUNT = 60;							// 一个军团最多人数
static const int MAX_APPLY_FOR_COUNT = 40;						// 一个军团申请列表最大数量
static const int MAX_INVITE_COUNT = 40;							// 最多同时邀请人数

static const int MAX_COMMON_EVENT_COUNT = 50;					// 军团普通事件数量
static const int MAX_BIG_EVENT_COUNT = 0;						// 军团大事件数量
static const int MAX_STORAGE_EVENT_COUNT = 30;					// 仙盟仓库日志数量
// 仙盟事件总数量  存储顺序为普通事件+大事件+仓库日志事件
static const int MAX_ALL_GUILD_EVENT_COUNT = MAX_COMMON_EVENT_COUNT + MAX_BIG_EVENT_COUNT + MAX_STORAGE_EVENT_COUNT;

static const int GUILD_UNION_MAX_GUILD_NUM = 2;					// 军团联盟最大的军团个数
static const int GUILD_UNION_APPLYFOR_MAX = 10;					// 军团联盟申请列表大小
static const int GUILD_UNION_ONCE_OPER_COUNT = 512;				// 一次操作军团联盟数量
static const int GUILD_UNION_SAVE_INTERVAL = 60;				// 军团联盟保存间隔

static const int GUILD_STORAGE_MAX_GRID_NUM = 102;				// 军团仓库格子数
static const int GUILD_STORAGE_CFG_MAX_NUM = 10;				// 军团仓库最大配置数

static const int GUILD_LOCK_RECOVER_TIME = 3;					// 军团锁恢复时间

static const int GUILD_ACHIEVE_ID_MAX = 64;						// 军团成就ID最大值
static const int GUILD_ACHIEVE_REWARD_ITEM_MAX = 3;				// 军团成就奖励物品最大数

static const int GUILD_JIANSHE_COIN_ONCE_CONSUME = 1000;		// 军团建设度 铜币一次消耗1000

static const int GUILD_AUTO_KICKOUT_NEED_MEMBER_COUNT = 10;		// 军团自动踢人需要成员人数
static const int GUILD_AUTO_KICKOUT_OFFLINE_TIME = 1800;		// 军团自动踢人所需离线时间

static const int GUILD_BOSS_MAX_LEVEL = 20;						// 仙盟boss最大等级

static const int GUILD_DAY_INVITE_COUNT = 5;					// 仙盟每日成功邀请次数限制

static const int GUILD_SKILL_COUNT = 7;							// 仙盟技能数
static const int MAX_GUILD_SKILL_LEVEL = 200;					// 仙盟技能最大等级
static const int MAX_GUILD_TOTEM_LEVEL = 100;					// 仙盟图腾最大等级

static const int MAX_GUILD_SAIZI_REWARD = 10;					// 骰子奖励最大数
static const int GUILD_SEND_SAIZI_RANK_MSG_INTERVAL = 7200;		// 发送骰子的公会信息的时间间隔

static const int GUILD_ENEMY_RANK_MAX_COUNT = 100;				// 仙盟仇人排行最大人数

//static const char *GUILD_DEFAULT_PLAT_NAME = "default";				// 创建公会时的默认平台名

// 军团职位定义
enum GUILD_POST_TYPE
{
	GUILD_POST_INVALID = 0,

	GUILD_POST_CHENG_YUAN,										// 成员
	GUILD_POST_ZHANG_LAO,										// 长老
	GUILD_POST_FU_TUANGZHANG,									// 副团长
	GUILD_POST_TUANGZHANG,										// 团长
	GUILD_POST_JINGYING,										// 精英成员
	GUILD_POST_HUFA,											// 护法

	GUILD_POST_MAX,
};

// 获取信息类型
enum GUILD_INFO_TYPE
{
	GUILD_INFO_TYPE_INVALID,

	GUILD_INFO_TYPE_ALL_GUILD_BASE_INFO,						// 所有军团基本信息
	GUILD_INFO_TYPE_GUILD_APPLY_FOR_INFO,						// 军团申请列表
	GUILD_INFO_TYPE_GUILD_MEMBER_LIST,							// 军团成员列表
	GUILD_INFO_TYPE_GUILD_INFO,									// 军团信息
	GUILD_INFO_TYPE_GUILD_EVENT_LIST,							// 军团日志列表
	GUILD_INFO_TYPE_APPLY_FOR_JOIN_GUILD_LIST,					// 已申请加入的军团列表
	GUILD_INFO_TYPE_INVITE_LIST,								// 邀请列表
	GUILD_INFO_TYPE_GONGZI_LIST,								// 工资列表

	GUILD_INFO_TYPE_MAX
};

// 军团创建方式
enum GUILD_CREATE_TYPE
{
	GUILD_CREATE_TYPE_INVALID = 0,

	GUILD_CREATE_TYPE_GOLD,										// 元宝
	GUILD_CREATE_TYPE_ITEM,										// 建团令

	GUILD_CREATE_TYPE_MAX
};

// 军团申请设置
enum GUILD_APPLYFOR_SETUP_TYPE
{
	GUILD_APPLYFOR_SETUP_NEED_AGREE = 0,						// 需要同意
	GUILD_APPLYFOR_SETUP_FORBIT,								// 禁止申请
	GUILD_APPLYFOR_CONDITION,									// 满足条件通过
	
	GUILD_APPLYFOR_TYPE_MAX,
};

// 增加buff类型
enum GUILD_BUFF_TYPE											
{
	GUILD_BUFF_TYPE_INVALID = 0,

	GUILD_BUFF_TYPE_ADD_HP_PER,									// 增加气血上限
	GUILD_BUFF_TYPE_ADD_GONGJI_PER,								// 增加攻击
	GUILD_BUFF_TYPE_ADD_FANGYU_PER,								// 增加防御
	GUILD_BUFF_TYPE_ADD_HP_GONGJI_FANGYU_PER,					// 增加气血,攻击，防御

	GUILD_BUFF_TYPE_MAX,
};

// 角色公会红包状态类型
enum GUILD_ROLE_RED_PAPER_STATE
{
	ROLE_RED_PAPER_INVALID = 0,
	ROLE_RED_PAPER_HAS_AUTHORITY,									// 有发红包的权限,但未有公会，不能发
	ROLE_RED_PAPER_CAN_CREATE,										// 在公会，能够发送红包
};

// 公会红包状态类型
enum GUILD_RED_PAPER_STATE
{
	RED_PAPER_INVALID = 0,
	RED_PAPER_CAN_CREATE,											// 能够发的红包
	RED_PAPER_CAN_FETCH,											// 能够抢的红包
	RED_PAPER_HAS_FETCH,											// 已抢的红包
};

enum GUILD_CREATE_CONSUME_TYPE
{
	GUILD_CREATE_CONSUME_TYPE_BIND_GOLD = 0,						// 绑元(不足时可用元宝代替)
	GUILD_CREATE_CONSUME_TYPE_GOLD = 1,								// 元宝
};

// 角色仙盟相关
struct RoleGuildParam
{
	RoleGuildParam() { this->Reset(); }

	void Reset()
	{
		ClearExchangeRecord();

		memset(skill_level_list, 0, sizeof(skill_level_list));
		territorywar_reward_flag = 0;

		last_leave_guild_time = 0;
		last_join_guild_time = 0;
		daily_gongxian = 0;
		storage_score = 0;
		guild_boss_activity_totem_total_exp = 0;
		be_kill_last_notice_time = 0;
		maze_reward_layer = 0;
		
		memset(guild_red_paper_list, 0, sizeof(guild_red_paper_list));

		guild_tianci_tongbi_reward_limit = 0;
		today_paosaizi_times = 0;
		reserve_sh = 0;
		join_tiancitongbi_guild_id = 0;
		today_juanxian_gold = 0;

		world_ques_rank = -1;
		charm_rank = -1;
		profess_rank = -1;
		qingyuan_rank = -1;
		baby_rank = -1;
		li_pet_rank = -1;
		kill_count = 0;
		killed_count = 0;
		next_notice_level = 0;
	}

	void ClearExchangeRecord()
	{
		exchange_record_total_num = 0;
		memset(exchange_record_list, 0, sizeof(exchange_record_list));
	}

	struct ExchangeRecordItem
	{
		ExchangeRecordItem() : item_id(0), item_num(0) {}

		ItemID item_id;
		short item_num;
	};

	void SetTerritoryWelfFlag(int index)
	{
		territorywar_reward_flag |= (1 << index);
	}

	bool IsFetchtTerritoryWelfReward(int index)
	{
		return 0 != (territorywar_reward_flag & (1 << index));
	}

	struct GuildRedPaper
	{
		GuildRedPaper(): red_paper_state(0), red_paper_seq(0), invalid_time(0){}

		short red_paper_state;
		short red_paper_seq;
		unsigned int invalid_time;
	};

	int exchange_record_total_num;
	ExchangeRecordItem exchange_record_list[GUILD_MAX_EXCHANGE_ITEM_COUNT];

	short skill_level_list[GUILD_SKILL_COUNT];
	short territorywar_reward_flag;

	unsigned int last_leave_guild_time;
	unsigned int last_join_guild_time;
	int daily_gongxian;
	int storage_score;
	long long guild_boss_activity_totem_total_exp;						// 公会boss活动，总图腾经验
	unsigned int be_kill_last_notice_time;
	int maze_reward_layer;

	GuildRedPaper guild_red_paper_list[MAX_ROLE_GUILD_RED_PAPER];	// 公会红包列表

	char guild_tianci_tongbi_reward_limit;							// 天赐铜币采集获得经验次数
	unsigned char today_paosaizi_times;								// 今天掷骰子次数
	short reserve_sh;						
	GuildID join_tiancitongbi_guild_id;								// 当天参加过天赐铜币的帮派id
	int today_juanxian_gold;										// 当天已捐献元宝

	char world_ques_rank;											//个人最高世界答题排行
	char charm_rank;												//个人最高魅力排行
	char profess_rank;												//个人最高表白排行
	char qingyuan_rank;												//个人最高情缘排行
	char baby_rank;													//个人最高宝宝排行
	char li_pet_rank;												//个人最高小宠物排行
	short kill_count;												//野外击杀玩家次数
	short killed_count;												//野外被击杀次数
	short next_notice_level;										//下一次江湖传闻角色等级
};

typedef char RoleGuildParamHex[sizeof(RoleGuildParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleGuildParamHex) < 2048);

//神兽
static const int GUILD_BOSS_GRADE_MAX = 3;						// 神兽阶段数
static const int GUILD_BOSS_LEVEL_MAX = 200;					// 军团神兽最高等级

static const int GUILD_BOSS_LEVEL_CFG_MAX_NUM = 10;				// 神兽最多配置个数10等级段

static const int GUILD_BOSS_LIVE_INTERVAL_S = 1800;				// 军团神兽存活时间
static const int GUILD_BOSS_DROP_ITEM_CFG_MAX_NUM = 18;			// 掉落物品最大配置个数
static const int GUILD_BOSS_DROP_ITEM_MAX_NUM = 10;				// 军团神兽掉落物品最大个数
UNSTD_STATIC_CHECK(GUILD_BOSS_DROP_ITEM_MAX_NUM <= GUILD_BOSS_DROP_ITEM_CFG_MAX_NUM);

static const int GUILD_BOSS_GIVE_ITEM_ROLL_INTERVAL_S = 10;		// 神兽掉落物品摇点间隔
static const int GUILD_BOSS_ROLL_SCORE_MAX = 100;				// 神兽掉落摇点最大值
static const int GUILD_BOSS_DPS_RANK_NUM = 5;					// 神兽伤害排行数目
static const int GUILD_BOSS_TOP_DPS_REWARD_NUM = 3;				// 神兽伤害前三名有特殊经验奖励及物品掉落默认给予
static const int GUILD_CHANGE_NOTICE_CD_TIME = 2;				// 修改仙盟公告CD
static const int GUILD_MAIL_ALL_CD_TIME = 2;					// 仙盟邮件CD

// 神兽状态
enum GUILD_BOSS_STATE
{
	GUILD_BOSS_STATE_INVALID = 0,
	GUILD_BOSS_STATE_CALL,										// 被召唤状态中
	GUILD_BOSS_STATE_MAX,
};

// 神兽喂养方式
enum GUILD_BOSS_FEED_TYPE
{
	GUILD_BOSS_FEED_TYPE_INVALID = 0,
	GUILD_BOSS_FEED_TYPE_NORMAL,								// 普通喂养
	GUILD_BOSS_FEED_TYPE_GOLD,									// 元宝喂养
	GUILD_BOSS_FEED_TYPE_MAX,
};

struct GuildBossDPSUser
{
	GuildBossDPSUser() : obj_id(0), user_id(INVALID_USER_ID), user_level(0), user_camp(0), hurt(0), roll_timestamp(0), roll_score(0), rank_pos(0), have_got_item(false)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	ObjID obj_id;
	UserID user_id;
	GameName user_name;
	int user_level;
	int user_camp;
	int hurt;
	unsigned int roll_timestamp;
	int roll_score;
	int rank_pos;
	bool have_got_item;
};
typedef std::map<UserID, GuildBossDPSUser> GuildBossDPSUserMap;
typedef std::map<UserID, GuildBossDPSUser>::iterator GuildBossDPSUserMapIt;

struct GuildBossDPSRankItem
{
	GuildBossDPSRankItem() : uid(0), hurt(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;
	GameName user_name;
	int hurt;
};
typedef std::vector<GuildBossDPSRankItem> GuildBossDPSRankItemVec;						
typedef std::vector<GuildBossDPSRankItem>::iterator GuildBossDPSRankItemVecIt;

// 军团签到方式
enum GUILD_SIGNIN_TYPE
{
	GUILD_SIGNIN_TYPE_INVALID = 0,
	GUILD_SIGNIN_TYPE_PUTONG,									// 普通签到
	GUILD_SIGNIN_TYPE_BAIJIN,									// 白金签到
	GUILD_SIGNIN_TYPE_ZHIZUN,									// 至尊签到
	GUILD_SIGNIN_TYPE_MAX
};

// 军团日志类型
enum GUILD_EVENT_TYPE
{
	GUILD_EVENT_TYPE_INVALID = 0,

	GUILD_EVENT_TYPE_CREATE,									// 创建军团
	GUILD_EVENT_TYPE_MEMBER_ADD,								// 成员入团
	GUILD_EVENT_TYPE_MEMBER_QUIT,								// 退出军团
	GUILD_EVENT_TYPE_MEMBER_KICK,								// 踢出军团
	GUILD_EVENT_TYPE_MEMBER_APPOINT,							// 成员任命
	GUILD_EVENT_TYPE_MEMBER_LEAVE_POST,							// 卸任
	GUILD_EVENT_TYPE_TRANSFER_TUANZHANG,						// 转让军团长
	GUILD_EVENT_TYPE_DELATE_TUANZHANG,							// 弹劾军团长
	GUILD_EVENT_TYPE_UPGRADE_LEVEL,								// 军团升级
	GUILD_EVENT_TYPE_STORAGE_OPERATE,							// 仓库操作		// 2019-04-18 作为仓库操作日志事件
	GUILD_EVENT_TYPE_SET_STORAGESHOP_USE,						// 更改权限设置
	GUILD_EVENT_TYPE_CALL_BOSS,									// 召唤神兽
	GUILD_EVENT_TYPE_GUILD_UNION,								// 联盟结成 解除
	GUILD_EVENT_TYPE_FEED_BOSS,									// 喂养神兽
	GUILD_EVENT_TYPE_MEMBER_SIGNIN,								// 成员签到
	GUILD_EVENT_TYPE_DOWNGRADE_LEVEL,							// 军团降级
	GUILD_EVENT_TYPE_ADD_EXP,									// 成员捐献
	GUILD_EVENT_TYPE_RUMOR_UPLEVEL = 18,						// 江湖传闻-角色升级
	GUILD_EVENT_TYPE_RUMOR_ZHUANZHI = 19,						// 江湖传闻-角色转职
	GUILD_EVENT_TYPE_RUMOR_MARRY = 20,							// 江湖传闻-角色结婚
	GUILD_EVENT_TYPE_RUMOR_DIVORCE = 21,						// 江湖传闻-角色离婚
	GUILD_EVENT_TYPE_RUMOR_BABY = 22,							// 江湖传闻-生宝宝
	GUILD_EVENT_TYPE_RUMOR_BUY_HOUSE = 23,						// 江湖传闻-买房
	GUILD_EVENT_TYPE_RUMOR_WEAPON_FB = 24,						// 江湖传闻-武器本通关第X章
	GUILD_EVENT_TYPE_RUMOR_ARMDEFEND_FB = 25,					// 江湖传闻-防具本通关第X章
	GUILD_EVENT_TYPE_RUMOR_PATA_FB = 26,						// 江湖传闻-爬塔本通关第X层
	GUILD_EVENT_TYPE_RUMOR_RUNE_TOWER_FB = 27,					// 江湖传闻-战魂塔通关第X层
	GUILD_EVENT_TYPE_RUMOR_GET_RED_EQUIP = 28,					// 江湖传闻-获得了X装备
	GUILD_EVENT_TYPE_RUMOR_KILLED = 29,							// 江湖传闻-被杀X次
	GUILD_EVENT_TYPE_RUMOR_KILL = 30,							// 江湖传闻-击杀X次
	GUILD_EVENT_TYPE_RUMOR_BE_FAMOUS_MAN = 31,					// 江湖传闻-名人堂
	GUILD_EVENT_TYPE_RUMOR_BE_LOVER_RANK = 32,					// 江湖传闻-情侣榜
	GUILD_EVENT_TYPE_RUMOR_BE_BABY_RANK = 33,					// 江湖传闻-宝宝榜
	GUILD_EVENT_TYPE_RUMOR_LITTLE_PET_RANK = 34,				// 江湖传闻-小宠物榜
	GUILD_EVENT_TYPE_RUMOR_CHARM_RANK = 35,						// 江湖传闻-魅力榜
	GUILD_EVENT_TYPE_RUMOR_WORLD_QUESTION_RANK = 36,			// 江湖传闻-世界答题榜
	GUILD_EVENT_TYPE_RUMOR_EXPRESS_RANK = 37,					// 江湖传闻-表白榜
	GUILD_EVENT_TYPE_RUMOR_GUILD_FB = 38,						// 江湖传闻-仙盟试炼开始
	GUILD_EVENT_TYPE_RUMOR_GUILD_FB_RESULT = 39,				// 江湖传闻-仙盟试炼，帮主X带领完成Y关
	GUILD_EVENT_TYPE_RUMOR_GUILD_QUESTION = 40,					// 江湖传闻-仙盟答题开始
	GUILD_EVENT_TYPE_RUMOR_GUILD_QUESTION_RESULT = 41,			// 江湖传闻-仙盟答题，本仙盟取得第X名，Y是答对题最多的仙盟成员
	GUILD_EVENT_TYPE_RUMOR_GUILD_YUNBIAO = 42,					// 江湖传闻-仙盟运镖开始
	GUILD_EVENT_TYPE_RUMOR_YUNBIAO_SUCC = 43,					// 江湖传闻-运镖成功
	GUILD_EVENT_TYPE_RUMOR_YUNBIAO_ROBBED = 44,					// 江湖传闻-被劫镖
	GUILD_EVENT_TYPE_RUMOR_GUILD_QIANGQI = 45,					// 江湖传闻-仙盟抢旗开始
	GUILD_EVENT_TYPE_RUMOR_GUILD_QIANGQI_RESULT = 46,			// 江湖传闻-仙盟抢旗结果，本仙盟获得第X名，Y是本盟积分最多的
	GUILD_EVENT_TYPE_RUMOR_GONGCHENG_START = 47	,				// 江湖传闻-仙盟攻城开始
	GUILD_EVENT_TYPE_RUMOR_GONGCHENG_SUCC = 48,					// 江湖传闻-仙盟攻城成功，X成为城主，Y是积分最多成员
	GUILD_EVENT_TYPE_RUMOR_GONGCHENG_FAIL = 49,					// 江湖传闻-仙盟攻城失败,Y是积分最多成员
	GUILD_EVENT_TYPE_RUMOR_CROSS_GUILD_BATTLE = 50,				// 江湖传闻-跨服帮派战开始
	GUILD_EVENT_TYPE_RUMOR_CROSS_GUILD_BATTLE_SUCC = 51,		// 江湖传闻-跨服帮派战成功，成功占领X，Y是积分最多成员
	GUILD_EVENT_TYPE_RUMOR_CROSS_GUILD_BATTLE_FAIL = 52,		// 江湖传闻-跨服帮派战失败， Y是积分最多成员
	GUILD_EVENT_TYPE_RUMOR_FB_TOWER_DEFEND = 53,				// 江湖传闻-守护本
	GUILD_EVENT_TYPE_RUMOR_FB_CHALLENGE = 54,					// 江湖传闻-品质本

	GUILD_EVENT_TYPE_MAX
};

enum GUILD_EVENT_ADD_EXP_TYPE
{
	GUILD_EVENT_ADD_EXP_TYPE_INVALID = 0,

	GUILD_EVENT_ADD_EXP_TYPE_GOLD,								// 元宝捐献
	GUILD_EVENT_ADD_EXP_TYPE_ITEM,								// 物品捐献

	GUILD_EVENT_ADD_EXP_TYPE_MAX,
};

// 仙盟仓库操作类型
enum GUILD_STORE_OPTYPE
{
	GUILD_STORE_OPTYPE_TAKEOUT = 0,								// 从军团仓库取出
	GUILD_STORE_OPTYPE_PUTIN,									// 放入军团仓库
	//GUILD_STORE_OPTYPE_DESTROY,								// 销毁军团仓库物品
};

enum GUILD_JIANZHU_TYPE
{
	GUILD_JIANZHU_TYPE_SHENLU = 0,								// 军团建筑 神炉 必须从0开始
	GUILD_JIANZHU_TYPE_SHOP,									// 军团建筑 商店
	GUILD_JIANZHU_TYPE_CANGKU,									// 军团建筑 仓库
	GUILD_JIANZHU_TYPE_YAOJIANG,								// 军团建筑 摇奖

	GUILD_JIANZHU_TYPE_MAX,									
};

struct GuildJianzhuInfo
{
	GuildJianzhuInfo() : level(0), exp(0) {}

	int level;													// 建筑 等级
	int exp;													// 建筑 经验
};

// 军团成就类型
enum GUILD_ACHIEVE_TYPE
{
	GUILD_ACHIEVE_TYPE_INVALID = 0,

	GUILD_ACHIEVE_TYPE_GUILD_LEVEL,								// 军团等级
	GUILD_ACHIEVE_TYPE_MEMBER_COUNT,							// 军团成员数
	GUILD_ACHIEVE_TYPE_KILL_WORLD_BOSS,							// 击杀世界BOSS
	GUILD_ACHIEVE_TYPE_KILL_GUILD_BOSS,							// 击杀军团神兽
	GUILD_ACHIEVE_TYPE_MEMBER_SIGNIN,							// 军团成员累计签到次数
	GUILD_ACHIEVE_TYPE_FEED_GUILD_BOSS,							// 累计喂养军团神兽次数

	GUILD_ACHIEVE_TYPE_MAX
};

// 军团操作类型
enum GUILD_OPER_TYPE
{
	GUILD_OPER_TYPE_INVALID = 0,
	GUILD_OPER_TYPE_DISMISS,									// 解散
	GUILD_OPER_TYPE_QUIT,										// 退出
	GUILD_OPER_TYPE_KICK,										// 踢人
	GUILD_OPER_TYPE_CREATE,										// 创建
	GUILD_OPER_TYPE_APPOINT,									// 任命

	GUILD_OPER_TYPE_MAX
};

// 军团GM操作类型
enum GUILD_GM_OPER_TYPE
{
	GUILD_GM_OPER_TYPE_INVALID = 0,
	GUILD_GM_OPER_TYPE_DISMISS,									// 解散
	GUILD_GM_OPER_TYPE_CLEAR_NOTICE,							// 清除公告

	GUILD_GM_OPER_TYPE_MAX
};

// 军团解散类型
enum GUILD_DISMISS_TYPE
{
	GUILD_DISMISS_TYPE_DEFAULT = 0,								// 手动解散
	GUILD_DISMISS_TYPE_AUTO,									// 自动解散
	GUILD_DISMISS_TYPE_GM,										// GM 解散
};

typedef char GuildEventStrParam[32];

UNSTD_STATIC_CHECK(sizeof(GuildEventStrParam) >= sizeof(GameName));
UNSTD_STATIC_CHECK(sizeof(GuildEventStrParam) >= sizeof(GuildName));

enum GUILD_SOS_TYPE
{
	GUILD_SOS_TYPE_DEFAULT,										// 默认求救
	GUILD_SOS_TYPE_HUSONG,										// 护送求救
	GUILD_SOS_TYPE_HUSONG_BE_ATTACK,							// 护送求救 - 被攻击
	GUILD_SOS_TYPE_GUILD_BATTLE,								// 公会争霸
	GUILD_SOS_TYPE_GONGCHENGZHAN,								// 攻城战
	GUILD_SOS_TYPE_CROSS_GUILD_BATTLE,							// 六界
	GUILD_SOS_TYPE_CROSS_BIANJINGZHIDI,							// 跨服边境之地
	GUILD_SOS_TYPE_CROSS_BOSS,									// 跨服远古BOSS
	GUILD_SOS_TYPE_CROSS_MIZANG_BOSS,							// 跨服神域BOSS
	GUILD_SOS_TYPE_CROSS_VIP_BOSS,								// 跨服VIPBOSS
};

#pragma pack(push, 4)

struct GuildSyncInfo
{
	GuildSyncInfo() : guild_id(0), tuanzhang_uid(0), guild_level(0), camp(0), member_count(0), guild_task_score(0), 
		personal_tower_defend_max_wave(0), personal_equip_fb_max_layer(0), today_call_shenjiang_flag(0)
	{
		memset(guild_name, 0, sizeof(guild_name));
		memset(tuanzhang_name, 0, sizeof(tuanzhang_name));
	}

	GuildID guild_id;
	GuildName guild_name;
	int tuanzhang_uid;
	GameName tuanzhang_name;
	int guild_level;
	int camp;
	int member_count;
	int guild_task_score;
	int personal_tower_defend_max_wave;
	int personal_equip_fb_max_layer;
	int today_call_shenjiang_flag;

	GuildJianzhuInfo jianzhu_info_list[GUILD_JIANZHU_TYPE_MAX];		
};

struct GuildXianshuParam
{
	GuildXianshuParam() { this->Reset(); }

	void Reset()
	{
		xianshu_level = 0;
		reserve = 0;
	}

	short xianshu_level;														// 仙术等级
	short reserve;																// 保留
};

typedef char GuildXianshuParamHex[sizeof(GuildXianshuParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(GuildXianshuParamHex) < 16);

// 仙盟塔防
static const int GUILD_TOWER_DEFEND_DAY_COUNT_MAX = 1;							// 仙盟塔防每日次数
static const int MAX_TOWER_DEFEND_DIRECTION = 4;								// 仙盟塔防进攻方向
static const int MAX_TOWER_DEFEND_WAVE = 80;									// 仙盟塔防最大怪物波数
static const int MAX_TOWER_DEFEND_SKILL_COUNT = 4;								// 仙盟塔防技能数量

struct GuildTowerDefendUser
{
	GuildTowerDefendUser() : obj_id(0), user_id(INVALID_USER_ID), total_longhun(0)
	{
		memset(user_name, 0, sizeof(user_name));
		memset(skill_cd_list, 0, sizeof(skill_cd_list));
	}

	ObjID obj_id;
	UserID user_id;
	GameName user_name;
	int total_longhun;
	unsigned int skill_cd_list[MAX_TOWER_DEFEND_SKILL_COUNT];
};
typedef std::map<UserID, GuildTowerDefendUser> GuildTowerDefendUserMap;
typedef std::map<UserID, GuildTowerDefendUser>::iterator GuildTowerDefendUserMapIt;

struct GuildStuffItem
{
	GuildStuffItem() : item_id(0), reserve(0), count(0) {}

	ItemID item_id;
	short reserve;
	
	int count;
};

struct GuildBossItem
{
	GuildBossItem() : killer_roleid(0), reserve1(0)
	{
		memset(killer_name, 0, sizeof(killer_name));
	}

	bool IsBeKilled()
	{
		return killer_roleid != 0;
	}
	
	int killer_roleid;
	GameName killer_name;

	int reserve1;
};

// 仙盟宝箱
static const int MAX_GUILD_BOX_COUNT = 8;										// 仙盟宝箱最大数量
static const int MAX_GUILD_BOX_LEVEL = 4;										// 仙盟宝箱最大等级
static const int MAX_TERRITORY_WELF_COUNT = 10;									// 仙盟领地最大福利配置
static const int MAX_GUILD_BOX_BE_ASSIST_COUNT = 4;								// 仙盟宝箱可被协助最大次数

struct GuildBoxItem
{
	GuildBoxItem() : box_level(0), is_reward(0), open_time(0) 
	{
		memset(assist_uid, 0, sizeof(assist_uid));
		memset(is_thank_assist_uid, 0, sizeof(is_thank_assist_uid));
	}

	short box_level;
	short is_reward;								// 是否领取
	int assist_uid[MAX_GUILD_BOX_BE_ASSIST_COUNT];
	char is_thank_assist_uid[MAX_GUILD_BOX_BE_ASSIST_COUNT]; // 是否已感谢
	unsigned int open_time;
};

struct GuildBoxInfo
{
	GuildBoxInfo() : dayid(0), uplevel_count(0), assist_count(0), be_assist_count(0), reserve_sh(0), assist_cd_end_time(0) {}

	void Reset()
	{
		dayid = 0;
		uplevel_count = 0;
		assist_count = 0;
		assist_cd_end_time = 0;
		be_assist_count = 0;
		reserve_sh = 0;

		memset(&info_list, 0, sizeof(info_list));
	}

	unsigned int dayid;
	short uplevel_count;
	short assist_count;
	short be_assist_count;
	short reserve_sh;
	unsigned int assist_cd_end_time;
	GuildBoxItem info_list[MAX_GUILD_BOX_COUNT];
};
typedef char GuildBoxInfoHex[sizeof(GuildBoxInfo) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(GuildBoxInfoHex) < 512);

// 仙盟运镖-------------------------------------------------------------------------

enum GUILD_YUNBIAO_RESULT_TYPE
{
	GUILD_YUNBIAO_RESULT_TYPE_FAIL = 0,		// 运镖失败 
	GUILD_YUNBIAO_RESULT_TYPE_SUCC,			// 运镖成功
	GUILD_YUNBIAO_RESULT_TYPE_ROB_SUCC,		// 运镖抢劫成功
};

// 天赐铜币采集物类型---------------------------------------------------------------
enum GUILD_TIANCI_TONGBI_GATHER_TYPE
{
	GUILD_TIANCI_TONGBI_GATHER_TYPE_INVALID = 0,
	GUILD_TIANCI_TONGBI_GATHER_TYPE_XIAPING,					// 天赐铜币采集物下品
	GUILD_TIANCI_TONGBI_GATHER_TYPE_ZHONGPING,					// 天赐铜币采集物中品
	GUILD_TIANCI_TONGBI_GATHER_TYPE_SHANGPING,					// 天赐铜币采集物上品
	GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX,
};

static const int GUILD_TIANCI_TONGBI_DAY_COUNT_MAX = 1;				// 军团天赐铜币每日次数
static const int GUILD_TIANCI_TONGBI_GATHER_REFRESH_POS_MAX = 30;	// 军团天赐铜币采集物刷新点数量
static const int GUILD_TIANCI_TONGBI_BIGCOIN_FLUSH_POS_MAX = 30;	// 军团天赐铜币铜币刷新点数量
static const int GUILD_TIANCI_TONGBI_GAHTER_NUM_MAX = 30;			// 军团天赐铜币采集物最大数量
static const int GUILD_TIANCI_TONGBI_RANK_NUM = 5;					// 军团天赐铜币排行数目

struct GuildTianCiTongBiUser
{
	GuildTianCiTongBiUser() : obj_id(0), user_id(INVALID_USER_ID), user_level(0), user_camp(0), /*reward_item_id(0),*/ reward_mojing_num(0), gold_bind_num(0), total_gather_exp_count(0), total_exp(0)
	{
		memset(user_name, 0, sizeof(user_name));
		memset(gather_count_list, 0, sizeof(gather_count_list));
	}

	ObjID obj_id;
	UserID user_id;
	GameName user_name;
	int user_level;
	int user_camp;
	//int reward_item_id;
	int reward_mojing_num;
	int gold_bind_num;
	int total_gather_exp_count;
	long long total_exp;
	int gather_count_list[GUILD_TIANCI_TONGBI_GATHER_TYPE_MAX];
};
typedef std::map<UserID, GuildTianCiTongBiUser> GuildTianCiTongBiUserMap;
typedef std::map<UserID, GuildTianCiTongBiUser>::iterator GuildTianCiTongBiUserMapIt;

struct GuildTianCiTongBiRankItem
{
	GuildTianCiTongBiRankItem() : uid(0), reward_mojing_num(0), coin_bind(0), rank_info(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;
	GameName user_name;
	int reward_mojing_num;
	int coin_bind;
	int total_gather_exp_count;
	int rank_info;
};
typedef std::vector<GuildTianCiTongBiRankItem> GuildTianCiTongBiRankItemVec;
typedef std::vector<GuildTianCiTongBiRankItem>::iterator GuildTianCiTongBiRankItemVecIt;

// 仙盟试炼---------------------------------------------------------------
struct HurtMonsterInfo
{
	HurtMonsterInfo() : role_id(0), hurt_val(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	bool operator <(const HurtMonsterInfo &other) const
	{
		return hurt_val > other.hurt_val;
	}

	int role_id;
	GameName user_name;
	long long hurt_val;
};

// 仙盟工资---------------------------------------------------------------

// 工资获得类型
enum GUILD_GONGZI_GIVE_TYPE
{
	GUILD_GONGZI_GIVE_TYPE_INVALID = 0,

	GUILD_GONGZI_GIVE_TYPE_NORMAL,						// 仙盟、个人
	GUILD_GONGZI_GIVE_TYPE_ONLY_GUILD,					// 仅仙盟

	GUILD_GONGZI_GIVE_TYPE_MAX,
};

struct MemberGongziInfo
{
	MemberGongziInfo()
	{
		memset(role_name, 0, sizeof(role_name));
	}

	bool operator <(const MemberGongziInfo &other) const
	{
		return gongzi > other.gongzi;
	}

	int				uid = 0;
	GameName		role_name;
	char			post = 0;
	char			reserve_ch = 0;
	short			gongzi_rate = 0;		// 发放比例
	int				gongzi = 0;				// 工资
	int				gold_bind = 0;			// 绑元
};


// 仙盟仇人 ---------------------------------------------------------------

// 仙盟仇人信息
struct GuildEnemyInfo
{
	GuildEnemyInfo() { this->Reset(); }

	void Reset()
	{
		enemy_uuid = 0;
		memset(enemy_name, 0, sizeof(enemy_name));
		kill_score = 0;
		last_kill_timestamp = 0;

		reserve_ll = 0;
		reserve_ll2 = 0;
	}

	long long		enemy_uuid;		// 仇人uuid
	GameName		enemy_name;		// 仇人名字
	int				kill_score;		// 仇恨值
	unsigned int	last_kill_timestamp; // 仇人上次击杀成员的时间

	long long reserve_ll;
	long long reserve_ll2;
};


#pragma pack(pop)

#endif  // __GUILD_DEF_HPP__

