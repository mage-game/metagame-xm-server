#ifndef __ROLE_H__
#define __ROLE_H__

#include <stdio.h>

#include "character.h"

#include "inetworkmodule.h"
#include "gamedef.h"
#include "servercommon/noticenum.h"
#include "servercommon/logindef.h"
#include "servercommon/chatserverdef.h"
#include "servercommon/maildef.hpp"
#include "gamelog.h"
#include "other/upgraderush/upgraderush.hpp"
#include "other/equipsuit/equipsuit.hpp"
#include "other/feixian/feixian.h"
#include "other/seal/seal.hpp"
#include "role_module/roleothermgr.h"

class IRoleModule;
struct UniqueUserID;

class RoleInitParam;
class RoleOtherInitParam;
class ItemListParam;
class HotkeyListParam;
class FriendListParam;
class BlacklistsParam;
class EnemyListParam;
class BusinessListParam;
class SkillListParam;
class DailyFindListParam;
class RoleAllParam;

class RoleShadow;

struct RoleModule;
struct CommonDataParam;
struct HistoryCapItem;

class UpgradeManager;
class EquipmentManager;
class FriendExpBottle;
class JingLingManager;
class MagicalPrecious;
class MysteriousShopInMall;
class RoleGuild;
class RoleFBTowerDefend;
class RuneSystem;
class ShenzhouWeapon;
class XiannvManager;
class RoleTuituFb;
class Knapsack;
class NewTaskManager;
class TaskRecorder;
class HotkeyRecorder;
class FriendManager;
class TeamInfo;
class RankShadow;
class Mentality;
class Vip;
class RoleChestShop;
class RoleShop;
class Capability;
class DayCounter;
class TitleManager;
class ActiveDegree;
class Welfare;
class Hangup;
class RoleEvaluate;
class RoleStatusManager;
class Monitor;
class SubLock;
class MountManager;
class FightMountManager;
class StoneManager;
class RoleActivity;
class DailyTask;
class HusongTask;
class RoleFBEquip;
class RoleFBArmorDefend;
class MazeManager;
class WingManager;
class HaloManager;
class ShengongManager;
class ShenyiManager;
class FootPrint;
class CloakManager;
class GuildTask;
class ZhuanZhiTask;
class MojieManager;
class EquipSuit;
class ShenZhuang;
class RoleYaojiang;
class SpecialAppearance;
class DatingTask;
class GuildXianshu;
class RoleSupply;
class RoleStoryFB;
class RoleMojieFB;
class RoleFBChallenge;
class RolePhaseFB;
class RolePataFB;
class RoleVipFB;
class RoleConvertRecord;
class DiscountBuy;
class ZeroGift;
class MonthCard;
class TurnTable;
class Wabao;
class CishaTask;
class Jilian;
class Qingyuan;
class Baby;
class TradeRegister;
class RoleZhanshenDian;
class Touzijihua;
class RollMoney;
class RoleWorldEvent;
class CommonActivityUnitManager;
class RoleGoal;
class Shizhuang;
class RoleCross;
class ShengWang;
class ChengJiu;
class RoleDailyFb;
class LifeSkillMgr;
class GongXun;
class PaoHuanTask;
class TuHaoJin;
class BigChatFace;
class Pet;
class MultiMount;
class PersonalizeWindow;
class XunZhangManager;
class ZhiBaoManager;
class ShenBingManager;
class Cardzu;
class LieMingManager;
class HuaShen;
class MagicCard;
class Mitama;
class XiannvShouhuManager;
class JinglingGuanghuanManager;
class JinglingFazhenManager;
class ZhuanShengEquip;
class CrossRoleGlobal;
class ChineseZodiac;
class FriendGiftManager;
class GoldVip;
class FamousMan;
class FollowBossManager;
class ShengeSystem;
class RolePreciousBoss;
class Shenshou;
class ImgFuling;
class CombineServerActivityEquip;
class ShenYin;
class ElemnetHeart;
class ShengxingzhuliManager;
class FabaoManager;
class RoleChallengeField;
class JingJie;
class Talent;
class JinJieTalent;
class HeShengLuoShu;
class Shengqi;
class ImpGuard;
class ZhuanzhiTask;
class RoleZhuanZhi;
class BossCard;
class YaoShiManager;
class TouShiManager;
class QiLinBiManager;
class MaskManager;
class ZhuanzhiEquip;
class Seal;
class DressingRoomMgr;
struct JinjieSysRewardParam;
class RoleBigSmallGoal;
class Shenqi;
class MojieSkill;
class RedEquipCollect;
class OrangeEquipCollect;
class Molong;
class XianZunKa;
class EquipBaptize;
class RolePersonBossFB;
class GreateSoldier;
class LotteryCount;
class RoleCrossRandActivity;
class UpgradeCardBuy;
class Huguozhili;
class CrossGoal;
class BaizhanEquip;
class CrossEquip;
class LittleHelper;
class Myth;
class Zodiac;

class Role : public Character
{
public:
	enum AUTHORITY_TYPE 
	{
		AUTHORITY_TYPE_INVALID = 0,									// 无任何权限
		AUTHORITY_TYPE_GUIDER,										// 新手指导员
		AUTHORITY_TYPE_GM,											// GM
		AUTHORITY_TYPE_TEST,										// 测试帐号（内部号）
		AUTHORITY_TYPE_MAX,
	};

	enum ATTACK_MODE
	{
		ATTACK_MODE_PEACE = 0,										// 和平模式
		ATTACK_MODE_TEAM,											// 可伤害非同队玩家
		ATTACK_MODE_GUILD,											// 军团模式
		ATTACK_MODE_ALL,											// 全体模式
		ATTACK_MODE_NAMECOLOR,										// 只攻击红名玩家
		ATTACK_MODE_CAMP,											// 阵营模式
		ATTACK_MODE_SAME_SERVER,									// 本服模式
		ATTACK_MODE_ENEMY,											// 仇人模式
		ATTACK_MODE_MAX,
	};

	enum _RoleStatus
	{
		ROLE_STATUS_CHARACTER = 0,									// Chanracter::m_action_status处于相应状态并相应的逻辑
																	// 注意 处于非ROLE_CHARACTER状态时Chanracter::m_action_status必须处于ACTION_IDLE 
																	// 并且CanCharacterAction()返回true
		ROLE_STATUS_GATHER,											// 采集状态
		ROLE_STATUS_REST,											// 打坐休息状态
	};

	enum MOVE_MODE
	{
		MOVE_MODE_NORMAL = 0,										// 正常
		MOVE_MODE_FLY,												// 飞行
		MOVE_MODE_JUMP,												// 跳跃
		MOVE_MODE_JUMP2,											// 跳跃
		MOVE_MODE_MAX
	};

	enum MOVE_MODE_FLY_PARAM
	{
		MOVE_MODE_FLY_PARAM_INVALID = 0,
		MOVE_MODE_FLY_PARAM_DRAGON = 1,								// 龙
		MOVE_MODE_FLY_PARAM_QILIN = 2,								// 麒麟
		
		MOVE_MODE_FLY_PARAM_MAX
	};

	enum EXP_ADD_REASON
	{
		EXP_ADD_REASON_INVALID = -1,

		EXP_ADD_REASON_DEFAULT = 0,									// 默认的
		EXP_ADD_REASON_KILL_MONSTER = 1,							// 杀怪
		EXP_ADD_REASON_ZHUAN_SHENG = 2,								// 转生
		EXP_ADD_REASON_EXP_ITEM = 3,								// 使用物品增加经验
		EXP_ADD_REASON_WATCH = 4,									// 查看经验加成
		EXP_ADD_REASON_HUSONG = 5,									// 护送
		EXP_ADD_EXP_FB = 6,											// 经验副本额外经验
		EXP_ADD_REASON_TRUNK_TASK = 7,								// 主线任务
		EXP_ADD_REASON_EXP_FB_BAODI = 8,							// 经验副本保底经验加成
		EXP_ADD_REASON_EXP_FRIEND_HELI = 9,							// 好友贺礼加经验
		EXP_ADD_REASON_EXP_FISH_POOL = 10,							// 捕鱼加经验
		EXP_ADD_FRIEND_EXP_BOTTLE = 11,								// 经验瓶
		EXP_ADD_WORSHIP = 12,										// 膜拜城主
		EXP_ADD_TIANCITONGBI = 13,									// 仙盟天赐铜币
		EXP_ADD_DAILY_TASK_EXP_BALL = 14,							// 日常任务经验球
		EXP_ADD_REASON_UPLEVEL_PILL = 15,							// 升级丹
		EXP_ADD_REASON_EXP_REFINE = 16,                             // 经验炼制
		EXP_ADD_REASON_EXP_BRANCH_TASK = 17,						// 支线任务,暂时没有用到,因为主线支线都是用的类型7加经验的,先预留着

		EXP_ADD_REASON_MAX,
	};

	enum CHANGE_MODE_TASK_TYPE
	{
		CHANGE_MODE_TASK_TYPE_INVALID = 0,
		CHANGE_MODE_TASK_TYPE_GATHER = 1,						// 采集类
		CHANGE_MODE_TASK_TYPE_TALK_TO_NPC = 2,					// 和NPC对话类
		CHANGE_MODE_TASK_TYPE_CHANGE_IMAGE = 3,					// 变身
		CHANGE_MODE_TASK_TYPE_FLY = 4,							// 飞行


		CHANGE_MODE_APPEARN_MAX
	};

	enum RESET_ROLE_STATUS_REASON
	{
		RESET_ROLE_STATUS_REASON_NONE,
		RESET_ROLE_STATUS_REASON_MOVE,						 // 移动
		RESET_ROLE_STATUS_REASON_GATHER_SUCC,				 // 采集成功
	};

	struct LastKillMeUserInfo
	{
		LastKillMeUserInfo() { this->Reset(); }

		void Reset()
		{
			user_id = INVALID_USER_ID;
			camp = 0;
			memset(user_name, 0, sizeof(user_name));
		}

		bool Invalid() const { return (INVALID_USER_ID == user_id); }

		UserID			user_id;
		GameName		user_name;
		char			camp;
	};

	Role();
	Role(const GSNetID &_netid, const GameName _name);  
	virtual ~Role();
	void InitModules();

	void SetSubSystemRole();

	//个模块数据初始化,本函数中的调用只允许处理当前调用模块数据. 不可以role->getothermodule()->dosomething();
	void				Init(bool is_micro_pc, 
							int plat_spid,
							PlatName pname, const UserID &user_id,
							const RoleInitParam &param, 
							const RoleOtherInitParam &other_p,
							const ItemListParam &item_list_param,
							const HotkeyListParam &hotkey,
							const FriendListParam &friends,
							const BlacklistsParam &balcklists_param,
							const EnemyListParam &enemy,
							const BusinessListParam &business_param,
							const SkillListParam &skill_param,
							const DailyFindListParam &daily_find_param);

	void				GetPlatName(PlatName pname);
	const char*			GetPlatName() { return m_plat_name; }

	const UserID  &		GetUserId() { return m_user_id; }
	int					GetUID() { return m_uid; }
	IP					GetIP() { return m_userip; }
	const GSNetID &		GetNetId() { return m_netid; }
	void				SetNetId(GSNetID netid) { m_netid = netid; }

	void				SetAvatarTimeStamp(long long timestamp);
	long long			GetAvatarTimeStamp() { return m_avator_timestamp; }
	char				GetAvatar() { return m_avatar; }
	char				GetSex() { return m_sex; }
	char				GetCamp() { return m_camp; }
	char				GetCampPost() { return m_camp_post; }
	int					GetLastScene(Posi *pos) { *pos = m_last_scene_pos; return m_last_scene_id; }
	unsigned long		GetEnterSceneTime() { return m_enter_scene_time; }
	const Posi &		GetBirthPos() { return m_birth_pos; }

	bool				IsInCross();
	UniqueUserID		GetUniqueUserID();
	void				GetCrossRoleParam(RoleAllParam *all_param);
	bool				AddCrossHonor(int honor_val, const char *type);
	int					GetCrossHonor();

	int					GetRightWorldAnswerNum();
	unsigned int		GetRightWorldAnswerNumChangeTime();

	void				OnLogin(long long last_save_time, IP userip, int plat_spid);
	void				OnLoginFinish(long long last_save_time);  // 登录完成

	virtual void		OnEnterScene();
	virtual void		OnLeaveScene();
	virtual void		OnAOIEnter(ObjID obj_id);
	virtual void		OnAOILeave(ObjID obj_id);

	virtual bool		CanCharacterAction(bool is_notify_client = false);
	virtual bool		Move(Axis _x, Axis _y, Dir _dir, Scalar distance, short height, bool check_valid = false, short press_onward = 0);
	virtual bool		SetDir(Dir _dir, Scalar distance);
	virtual	void		Update(unsigned long interval, time_t now_second, unsigned int now_dayid);
	void				IntervalRecoverHp(unsigned long interval, time_t now_second);
	bool				ResetPos(Axis x, Axis y, short reset_type = 0);
	UInt32				GetAoiHandler() { return m_aoi_handle; }

	long long			GetAddonExp(long long exp, int exp_add_reason, int *add_percent);
	bool				AddExp(long long exp, const char *type, int reason, long long *real_add_exp = NULL, int add_percent = 0);
	long long			GetExp() { return m_exp; }
	void				ReduceExp(long long exp, const char *type, long long *reduce_exp);
	double				GetExpExtraPer(int exp_addon_reason = EXP_ADD_REASON_DEFAULT);
	double				GetExpTotalPer(int exp_addon_reason, int *add_percent);
	long long			RecalcExp(long long exp);

	bool				ZhuanSheng();
	void				TransformExp(int seq, int item_num);
	void				SendExpBottleInfo();
	void				SendMoveChessInfo();
	void				SendMillionaireInfo();
	void				SendWorldBossWearyInfo();
	bool                CanRealiveWithMiKuTire();
	void                DieOnMiKuScene(bool is_die = false);
	void				AddExpBottleUseCount(int num);
	void				ResetExpBottleDayCount();
	void				ResetMoveChessDayCount();

	void				GMSetLevel(int level);

	void				GMAddRoleOnLineTime(int days);

	//void				OnChangeScene(int target_scene_id, int target_scene_key, const Posi target_pos);

	void				ForbidTalk(time_t forbid_time, const char *reason, const char *last_msg);
	void				ForbidRole(const char *reason, int forbit_time = 3600 * 24 * 365);

	bool				SystemMsg(const char *msg, short length, short msg_type);
	
	virtual bool		IsEnemy(Obj *obj);
	virtual bool		IsFriend(Obj *obj, bool check_alive = true);
	virtual bool		CheckMapValidate();
	virtual bool		CheckMoveFrequency();
	virtual float		GetMoveRate();
	virtual int			GetLevel() { return m_level; }
	int					GetZhun() { return (m_level - 1) / ZHUANSHENG_LEVEL_STEP; }
	int					GetPutOnLevel();
	virtual void		NoticeNum(int notice_num);
	void				NoticeItemLack(int item_id, int nun);

	char				GetProf() { return m_prof; }
	void				SetCamp(char camp_type, bool is_random = false);
	void				SetProf(char prof);
	void				GMSetProf(char prof);
	bool				SetSex(char sex);
	GuildID				GetGuildID() { return m_guild_id; }
	void				GetGuildName(GuildName guild_name) { F_STRNCPY(guild_name, m_guild_name, sizeof(GuildName)); }
	char				GetGuildPost() { return m_guild_post; }
	int					GetGuildLuckyColor();
	unsigned long long	GetGuildAvatar();

	short				GetBaseProf() { return m_prof % PROF_TYPE_PROF_ZHUANZHI_BASE; }		// 职业
	short				GetProfLevel() { return m_prof / PROF_TYPE_PROF_ZHUANZHI_BASE; }	// 转职等级
	void				Zhuanzhi();

	int					GetGuildGongxian() { return m_guild_gongxian; }
	bool				AddGuildGongxian(int gongxian, const char *type);

	int					GetDeadTime() { return m_dead_time; }

	void *				operator new(size_t c);
	void				operator delete(void *m);

	void				OnRoleBaseInfoChange();
	void				SyncGuildInfoToGlobal();
	
	bool				OnUpgradeLevel(int to_level);
	bool				SetAuthority(char authority_type, bool save_now = true);
	char				GetAuthority() { return m_authority_type; }
	time_t				GetForbidTalkTime() { return m_forbid_talk_time; }
	bool				IsProfSuitable(char prof);
	bool				IsSexSuitable(char sex);

	virtual void		StopAttack();
	virtual bool		PerformSkill(int skill_index, const Posi &pos, ObjID target_id, bool is_specialskill = false, int skill_data = 0);

	virtual void		OnDie(ObjID killer);
	void				SendBeKillMail();	// 被杀邮件

	virtual void		OnAttackCalcHurt(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill, int fight_type);
	virtual void		OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill);
	virtual int			OnHurtOther(Character *target, int skill_id, long long &injure, int fight_type, int target_index);
	void				OnAfterHurtOther(Character *target, int skill_id, long long injure, bool from_skill, int fight_type);  // 已经对别人造成了伤害

	void				OnKillOtherRole(Role * target_role);

	void				OnStartFight(); // 进入战斗状态触发
	void				OnStopFight(); // 退出战斗状态触发

	void				CheckFakePrivateChat(time_t now_second);

	bool				UseGoldCard(int gold_num);

	bool				ReAlive(int hp_per, int mp_per = 0, Posi realive_pos = Posi(-1, -1), bool is_immediately = false); 
	bool				ReAliveBackHome();
	bool				ReAliveTargetScene(int scene_id, const Posi &pos);
	bool				ForceReturnReAlivePosi();

	void				SetAttackMode(int attack_mode, bool is_force, bool is_fanji);
	char				GetAttackMode() { return m_attack_mode; }

	const time_t		GetCreateRoleTimestamp() { return m_create_time; }

	LastKillMeUserInfo *GetLastKillMeUserInfo() { return &m_last_kill_me_user_info; }
	
	void				SendInfoMsg();
	void				BackToOriginalServer();

	void				SetMarryInfo(int lover_uid, GameName lover_name, time_t marry_time = 0);
	void				SetIsReserveMarry(char is_reserve_marry) { m_is_reserve_marry = is_reserve_marry; }
	void				SetLastHunyanBlessMarryStartTime(unsigned int marry_start_time) { m_last_hunyan_bless_marry_start_time = marry_start_time; }
	bool				IsMarry();
	int					GetLoverUid();
	void				GetLoverName(GameName lover_name);
	unsigned int		GetLastHunyanBlessMarryStartTime() { return m_last_hunyan_bless_marry_start_time; }
	bool				IsReserveMarry() { return 0 != m_is_reserve_marry; }
	unsigned int		GetLastMarryTime() { return m_last_marry_time; }
	void				SetDivorce(unsigned int divorce_time);
	unsigned int		GetLastDivorceTime() { return m_last_divorce_time; }
	bool				IsFetchWorldBossWeekReward();
	void				SetFetchWorldBossWeekReward(char flag);
	static void			GenerateShenheNewRoleParam(const RoleInitParam& role_init_param, const RoleOtherInitParam& other_p);
	void				ShenheServerRandomRole();

	// 设置下一次攻击额外增加对敌人伤害百分比
	void				NextAttackExtHurtPercent(int hurt_percent);

	CommonDataParam *GetCommonDataParam();
	void GetCommonDataParam(CommonDataParam *param);

	UpgradeManager *GetUpgradeManager();
	EquipmentManager *GetEquipmentManager();
	FeixianManager *GetFeixianManager();
	FriendExpBottle *GetFriendExpBottle();
	JingLingManager *GetJingLingMgr();
	MagicalPrecious	*GetMagicalPreciousManager();
	MysteriousShopInMall *GetMysteriousShopInMall();
	RoleGuild *GetRoleGuild();
	RoleFBTowerDefend *GetFBTowerDefend();
	RuneSystem *GetRuneSystem();
	ShenzhouWeapon *GetShenzhouWeapon();
	XiannvManager *GetXiannvManager();
	RoleTuituFb	*GetTuituFb();

	Knapsack *GetKnapsack();
	NewTaskManager *GetTaskManager();
	TaskRecorder *GetTaskRecorder();
	HotkeyRecorder *GetHotkeyRecorder();
	FriendManager *GetFriendManager();
	TeamInfo *GetTeamInfo();
	RankShadow *GetRankShadow();
	Mentality *GetMentality();
	Vip *GetVip();
	RoleChestShop *GetRoleChestShop();
	RoleShop *GetRoleShop();
	Capability *GetCapability();
	DayCounter *GetDayCounter();
	TitleManager *GetTitleManager();
	ActiveDegree *GetActiveDegree();
	Welfare *GetWelfare();
	Hangup *GetHangup();
	RoleEvaluate *GetRoleEvaluate();
	RoleStatusManager *GetRoleStatusManager();
	Monitor *GetMonitor();
	SubLock *GetSublock();
	MountManager *GetMountManager();
	FightMountManager *GetFightMountManager();
	StoneManager *GetStoneManager();
	RoleActivity *GetRoleActivity();
	DailyTask *GetDailyTask();
	HusongTask *GetHusongTask();
	RoleFBEquip *GetFBEquip();
	MazeManager *GetMaze();
	WingManager *GetWing();
	HaloManager *GetHalo();
	ShengongManager *GetShengong();
	ShenyiManager *GetShenyi();
	FootPrint *GetFootPrint();
	CloakManager *GetCloakManager();
	GuildTask *GetGuildTask();
	ZhuanzhiTask *GetZhuanzhiTask();
	MojieManager *GetMojie();
	EquipSuit *GetEquipSuit();
	ShenZhuang *GetShenZhuang();
	RoleYaojiang *GetRoleYaojiang();
	SpecialAppearance *GetSpecialAppearance();
	DatingTask *GetDatingTask();
	GuildXianshu *GetGuildXianshu();
	RoleSupply *GetRoleSupply();
	RoleStoryFB *GetFBStory();
	RoleMojieFB *GetFBMojie();
	RoleFBChallenge *GetFBChallenge();
	RolePhaseFB *GetRoleFBPhase();
	RolePataFB *GetRolePataFB();
	RoleVipFB *GetRoleVipFB();
	RoleConvertRecord *GetConvertRecord();
	DiscountBuy *GetDiscountBuy();
	ZeroGift *GetZeroGift();
	MonthCard *GetMonthCard();
	TurnTable *GetTurnTable();
	Wabao *GetWabao();
	CishaTask *GetCishaTask();
	Jilian *GetJilian();
	Qingyuan *GetQingyuan();
	Baby *GetBaby();
	TradeRegister *GetTradeRegister();
	RoleZhanshenDian *GetZhanShenDian();
	Touzijihua *GetTouzijihua();
	RollMoney *GetRollMoney();
	RoleWorldEvent *GetRoleWorldEvent();
	CommonActivityUnitManager *GetCommonActivityUnitMgr();
	RoleGoal *GetRoleGoal();
	Shizhuang *GetShizhuang();
	RoleCross *GetRoleCross();
	ShengWang *GetShengWangMgr();
	ChengJiu *GetChengJiuMgr();
	RoleDailyFb *GetRoleDailyFb();
	LifeSkillMgr *GetLifeSkillMgr();
	GongXun *GetGongXunMgr();
	PaoHuanTask *GetPaoHuanTask();
	TuHaoJin *GetTuHaoJin();
	BigChatFace *GetBigChatFace();
	Pet *GetPet();
	MultiMount *GetMultiMount();
	PersonalizeWindow *GetPersonalizeWindow();
	XunZhangManager *GetXunZhangManager();
	ZhiBaoManager *GetZhiBaoManager();
	ShenBingManager *GetShenBingManager();
	Cardzu *GetCardzu();
	LieMingManager *GetLieMingMgr();
	HuaShen *GetHuaShen();
	MagicCard *GetMagicCard();
	Mitama *GetMitama();
	XiannvShouhuManager *GetXiannvShouhu();
	JinglingGuanghuanManager *GetJinglingGuanghuan();
	JinglingFazhenManager *GetJinglingFazhen();
	ZhuanShengEquip *GetZhuanShengEquip();
	CrossRoleGlobal *GetCrossRoleGlobal();
	ChineseZodiac *GetChineseZodiac();
	FriendGiftManager *GetFriendGiftManager();
	GoldVip *GetGoldVip();
	FamousMan *GetFamousMan();
	FollowBossManager *GetFollowBossManager();
	ShengeSystem *GetShengeSystem();
	RolePreciousBoss *GetRolePreciousBoss();
	Shenshou *GetShenshou();
	ImgFuling *GetImgFuling();
	CombineServerActivityEquip *GetCSAEquip();
	ShenYin *GetShenYin();
	ElemnetHeart *GetElementHeart();
	ShengxingzhuliManager *GetShengxingzhuliManager();
	FabaoManager *GetFabaoManager();
	RoleChallengeField* GetRoleChallengeField();
	RoleFBArmorDefend * GetRoleFbArmorDefend();
	Shengqi *GetShengqi();
	ImpGuard *GetImpGuard();
	RoleZhuanZhi *GetRoleZhuanZhi();
	BossCard *GetBossCard();
	YaoShiManager * GetYaoShiManager();
	TouShiManager * GetTouShiManager();
	QiLinBiManager * GetQiLinBiManager();
	MaskManager * GetMaskManager();
	ZhuanzhiEquip * GetZhuanzhiEquip();
	Seal* GetSeal();
	DressingRoomMgr * GetDressingRoomMgr();
	JinjieSysRewardParam * GetJinjieSysRewardParam();
	RoleBigSmallGoal * GetRoleBigSmallGoal();
	Shenqi *GetShenqi();
	MojieSkill * GetMojieSkill();
	RedEquipCollect * GetRedEquipCollect();
	OrangeEquipCollect * GetOrangeEquipCollect();
	Molong * GetMolong();
	XianZunKa * GetXianZunKa();
	EquipBaptize * GetEquipBaptize();
	RolePersonBossFB * GetRolePersonBossFb();
	GreateSoldier * GetGreateSoldier();
	LotteryCount * GetLotteryCount();
	RoleCrossRandActivity * GetRoleCrossRandActivity();
	UpgradeCardBuy *GetUpgradeCardBuy();
	Huguozhili * GetHuguozhili();
	CrossGoal * GetCrossGoal();
	BaizhanEquip * GetBaizhanEquip();
	CrossEquip * GetCrossEquip();
	LittleHelper * GetLittleHelper();
	Myth * GetMyth();
	Zodiac * GetZodiac();

	// 其他管理器
	RoleOtherMgr		*GetOtherMgr() { return m_role_other_mgr; }
	template<class T>
	T *GetOtherModule(int module_type) const { return dynamic_cast<T*>(m_role_other_mgr->GetRoleModule(module_type)); }

	//////////////////////////////////////////////////////////////////////////
	bool				Save(bool is_logout = false);
	void				OnRoleLogout();
	void				GetRoleInitParam(RoleInitParam *param);
	void				InitOtherParam(const RoleOtherInitParam &other_p);
	void				GetRoleOtherInitParam(RoleOtherInitParam *other_p);
	void				SetMsgTime(unsigned long now, IP userip);
	unsigned long		GetLastMsgFromClientTime() { return m_last_msg_from_client_time; }
	unsigned int		GetLastMsgFromClientTimestamp() { return m_last_msg_from_client_timestamp; }
	unsigned int		GetLastOnlineDayID() { return m_last_online_dayid; }
	
	char				GetRoleStatus() { return m_role_status; }
	bool				StartGather(ObjID obj_id, int gather_count);
	bool				OnRest(bool is_force = false);
	void				OnRestInvite(ObjID obj_id);
	void				OnRestInviteAck(ObjID obj_id, bool is_accept);
	void				OnRestStartAffirm(ObjID obj_id);
	bool				CanRoleAction() { return m_role_status == ROLE_STATUS_CHARACTER; }
	
	bool				HasRestPartner() { return INVALID_OBJ_ID != m_rest_partner_obj_id; }
	void				SetRestPartner(ObjID obj_id, int start_or_stop = 1);
	ObjID				GetRestPartnerID() { return m_rest_partner_obj_id; }
	void				SetShuangxiuNextIntimacyTime(time_t next_intimacy_time) { m_shuangxiu_next_intimacy_time = next_intimacy_time; }

	void				OnAOIRoleEnter(Obj *obj);
	void				OnAOIMonsterEnter(Obj *obj);
	void				OnAOIFallingEnter(Obj *obj);
	void				OnAOIGatherEnter(Obj *obj);
	void				OnAOISceneAreaEnter(Obj *obj);
	void				OnAOIEffectEnter(Obj *obj);
	void				OnAOITriggerEnter(Obj *obj);
	void				OnAOIBigCoinEnter(Obj *obj);
	void				OnAOIBattleFieldShenShiEnter(Obj *obj);
	void				OnAOIMonsterHaloEnter(Obj *obj);
	void				OnAOIMarryObjEnter(Obj *obj);
	void				OnAOIRoleShadowEnter(Obj *obj);
	void				OnAOIWorldEventObjEnter(Obj *obj);
	
	void				ReCalcAttr(int recalc_reason = CharIntAttrs::RECALC_REASON_INVALID, bool is_init = false, bool is_recover = false, unsigned char attr_notify_reason = CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL, bool is_time_limit = false);
	void				ReCalcSelfAttr(CharIntAttrs &base_add);
	
	int					GetEvil() { return m_evil; }
	bool				AddEvil(int evil);
	void				SetNameColor(int name_color);

	int					GetXianhun() { return m_xianhun; }
	bool				AddXianhun(int xianhun, const char *type);

	int					GetYuanli() { return m_yuanli; }
	bool				AddYuanli(int yuanli, const char *type);

	int					GetNvWaShi() { return m_nv_wa_shi; }
	bool				AddNvWaShi(int nv_wa_shi, const char *type);

	int					GetLingJing() { return m_lingjing; }
	bool				AddLingJing(int lingjing, const char *type);

	int					GetChengJiu() { return m_chengjiu; }
	bool				AddChengJiu(int chengjiu, const char *type);

	void				AddOrResetDayRevivalTimes(bool is_reset = false);
	unsigned short		GetDayRevivalTimes();

	int					GetHunli() { return m_hunli; }
	bool				AddHunli(int hunli, const char *type);

	int					GetNuqi();
	void				AddNuqi(int nuqi);

	int					GetShengwang();
	bool				AddShengwang(int shengwang, const char *type);

	int					GetGongXun();
	bool				AddGongxun(const int gongxun, const char *type);
	void				ResetGongXun();
	void				SyncGongXunInfo();

	void				SendAddGuildGongziInfo(int gongzi);		// 发送仙盟工资增加信息

	int					GetEnergy();
	void				AddEnergy(int energy, const char *type);
	bool				AddEnergyIgnoreOverRange(int energy, const char *type);
	void				AddEnergyForce(int energy, const char *type);

	void				SetMoveMode(int move_mode, const Posi &longjump_dest_pos = Posi(0, 0), char move_mode_param = 0);
	char				GetMoveMode() { return m_move_mode; }
	char				GetMoveModeParam() { return m_move_mode_param; }
	void				FixMoveMode();
	const Posi&			GetJumpSrcPos() { return m_jump_src_pos; }
	void				SetAcceptTaskAppearn(char appearn, int appearn_param);
	char				GetAcceptTaskAppearn();
	int					GetAcceptTaskAppearnParam();
	void				NotifyAcceptTaskAppearn();
	bool				IsTaskAppearnStatus();
	void				CheckTaskAppearnStatusWithEnterScene();

	short				GetHeight() { return m_height; }
	short				GetFlyHeight() { return m_fly_height; }

	char				GetNameColor() { return m_name_color; }

	void				RefreshApperance(bool notify, short wuqi_use_type = APPEARANCE_WUQI_USE_TYPE_INVALID, short body_use_type = APPEARANCE_BODY_USE_TYPE_INVALID);
	void				BroadcastAppearanceInfo();
	const RoleAppearance& GetAppearance() { return m_appearance; }

	void				ResetRoleStatus(ObjID attacker = INVALID_OBJ_ID, Attribute delta_hp = 0, short reset_reason = RESET_ROLE_STATUS_REASON_NONE);
	
	void				SyncSCGulidBossRedbagInfo();
	void				OnSyncGuildInfo(GuildID guild_id, const GuildName guild_name, int guild_post);
	void				OnJoinGuild(GuildID guild_id);
	void				OnLeaveGuild(GuildID old_guild_id);
	void				NotifyGuildInfoToClient(bool boardcast = false);
	void				SendQuestionRightAnswerNum();

	bool				RoleIsEnemy(Role *role);
	bool				RoleIsFriend(Role *role);

	bool				RoleShadowIsEnemy(RoleShadow *role_shadow);

	int					GetOnlineTime() { return m_online_time; }
	int					GetTodayOnlineTime() { return m_today_online_time; }

	void				SetTopDpsFlag(bool top_dps_flag);

	void				SetFirstHitFlag(bool first_hurt_flag);
	bool				GetFirstHitHaveDead() { return m_first_hit_have_died; }

	void				NotifyFBFinish();

	void				AddGmChangeMaxHp(Attribute max_hp) { m_gm_change_maxhp += max_hp; }
	void				AddGmChangeGongji(Attribute gongji) { m_gm_change_gongji += gongji; }
	void				AddGmMoveSpeed(Attribute move_speed) { m_gm_change_movespeed += move_speed; }
	void				AddGmChangeFangYu(Attribute fang_yu) { m_gm_change_fangyu += fang_yu; }
	void				AddGmChangeMingZhong(Attribute ming_zhong) { m_gm_change_mingzhong += ming_zhong; }
	void				AddGmChangeShanBi(Attribute shan_bi) { m_gm_change_shanbi += shan_bi; }
	void				AddGmChangeBaoJi(Attribute bao_ji) { m_gm_change_baoji += bao_ji; }
	void				AddGmChangeJianRen(Attribute jian_ren) { m_gm_change_jianren += jian_ren; }
	void				AddGmChangeFujiaShanghai(Attribute fujia_shanghai) { m_gm_change_fujia_shanghai += fujia_shanghai; }
	void				AddGmChangeDikangShanghai(Attribute dikang_shanghai) { m_gm_change_dikang_shanghai += dikang_shanghai; }
	void				AddGmChangePerJingzhun(Attribute jingzhun) { m_gm_change_per_jingzhun += jingzhun; }
	void				AddGmChangePerBaoji(Attribute per_baoji) { m_gm_change_per_baoji += per_baoji; }
	void				AddGmChangePerKangbaoi(Attribute per_kangbao) { m_gm_change_per_kangbao += per_kangbao; }
	void				AddGmChangePerPofang(Attribute per_pofang) { m_gm_change_per_pofang += per_pofang; }
	void				AddGmChangePerMianshang(Attribute per_mianshang) { m_gm_change_per_mianshang += per_mianshang; }
	void				SetGmChangeCapabilityAttr(Attribute cap) { m_gm_change_capability_attr = cap; }   // 一键设置战力和对应属性。小于等于0则不再生效
	void				ResetGMChangeAttribute();
	void				SendAttributeInfo(int notify_reason = CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL);

	void				OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void				OnWeekChange();
	void				OnMonthChange();
	void				OnResetModuleData(unsigned int old_dayid, unsigned int now_dayid);
	void				OnExtraResetModuleData(unsigned int old_dayid, unsigned int now_dayid);

	void				SendPersonalMsg(int str_len, const char *str);
	void				SendLackItemMsg(ItemID item_id);

	bool				CheckEnterSceneLevelLimit(int require_level);
	bool				CheckEnterFbLevelLimit(int require_level);

	void				OnResetNameReq(GameName name, bool is_item_reset = false);
	bool				ResetName(GameName name, bool is_item_reset);
	void				ChangeCamp(int camp);
	void				ChangeProf(char prof, char sex);

	bool				IsForbidTalk();

	void				OnSingleChatMonitor(unsigned int msg_time, int msg_length, const char *msg);
	void				OnWorldChatMonitor(unsigned int msg_time, int msg_length, const char *msg);
	void				OnGuildChatMonitor(unsigned int msg_time, int msg_length, const char *msg);

	void				OnMailMonitor(MailSubject subject);

	void				OnKillOtherCampGetHonour(const UserID &bekill_user_id, int delta_level);

	void				OnSendAllInfo() { m_has_send_all_info = true; }
	int					HasSendAllInfo() { return m_has_send_all_info; }

	void				SetZombie(bool is_zombie);
	bool				IsZombie() { return m_is_zombie; }

	void				SetForceKickDelay() { m_isforce_kickdelay = true; }
	bool				IsForceKickDelay() { return m_isforce_kickdelay; }

	int					GetHonour() { return m_honour; }
	void				SetHonour(int honour, int reason = 0);

	void				SetLastCreateGuildTime(unsigned int time_s) { m_last_create_guild_time = time_s; }
	unsigned int		GetLastCreateGuildTime() { return m_last_create_guild_time; }

	virtual void		OnQTEEffectDone();
	virtual void		CheckQTEEffect(Character *target, bool is_gongji = true);

	virtual int GetHurtIncPercent(Character *target);		// 伤害加成
	virtual int GetHurtDecPercent(Character *performer);	// 伤害减免

	virtual bool		IsMianyiDeadlyAttack();				// 是否免疫致命的一次攻击
	void				SendNewhandGuideEndtime();

	void				OnMountBianshenOpera(bool is_bianshen);
	void				SendMountBianshenInfo(bool is_brocast = false);
	bool				IsMountBianshen() { return m_is_mount_bianshen; }

	void				StatMoveReq();
	
	void				ClearSpeedupHello();
	int					GetSpeedupHelloCount();
	int					GetSpeedupHelloTotalCount();
	time_t				GetFirstSpeedupHelloTime();
	void				OnSpeedupHello();

	void				OnWeddingRingSkill(int ring_level, int target_obj_id);
	void				OnQueryMyPosition(short reason, int from_uid);
	void				OnRspPosition(short reason, bool is_static_scene, int my_scene_id, int my_scene_key, int pos_x, int pos_y);
	void				SendWeddingRingSkillInfo();

	void				SendOperateResult(short oprate, short result, int param1 = 0, int param2 = 0);

	void				UpdateUserCache();
	int					GetRealTimeOnlineS();
	int					GetTodayRealTimeOnlineS();

	bool				IsTeamEquipFBPass(int layer);
	void				OnTeamEquipFBPassLayer(int layer);
	int					GetTeamEquipFBDayCount();
	int					GetTeamEquipFBDayBuyCount();

	void				OnKillWorldBoss();
	int					GetKillWorldBossCount();

	void				AddRedPaperConsumeGold(int gold_num, const char *type);
	long long			GetRedPaperConsumeGold();
	void				AddRedPaperLuckyValue(int add_value);
	int					GetRedPaperLuckyValue();

	void				AddDayHunyanGatherBindCoin(int add_val);
	int					GetDayHunyanGatherBindCoin();

	void				AddDayHunyanGatherNvWaShi(int add_val);
	int					GetDayHunyanGatherNvWaShi();

	void				OnChangeCapability(int cap_type);
	const HistoryCapItem * GetHistoryMaxCapability(int cap_type);

	int					GetPlatSpid();
	
	short				GetUseNorexItemDayTimes(ItemID item_id);
	void				AddUseNorexItemDayTimes(ItemID item_id, short times);
	void				SyncNorexItemDayTimes();

	bool				IsIgnoreChatOrMailLevelLimit();

	void				SpaceCaiOrBeCai(int cai_count, bool is_caier);
	void				SpaceGiftOrBeGift(int gift_count, bool is_sender);
	bool				HuoliMoreThan(int huoli_cost);
	bool				UseHuoli(int huoli_cost, const char *reason);
	bool				AddHuoli(int huoli, const char *reason);
	void				SendSpaceSelfInfo();
	void				UpdateSpaceXinqing(SpaceMessage xinqing);
	void				SyncCommonInfo(int info_type, int param1, int param2 = 0, int param3 = 0, int param4 = 0);
	bool				IsInCanTradingTime();

	unsigned short		GetEquipXunbaoTimes();
	unsigned short		GetJLXunbaoTimes();
	unsigned short		GetHCHZKillCount();
	unsigned short		GetGCZKillCount();
	int					GetRareChestShopTimes();

	void				AddEquipXunBaoTimes(const short times);
	void				AddJLXunbaoTimes(const short times);
	void				AddHCHZKillCount(const short count);
	void				AddGCZKillCount(const short count);
	void				AddRareChestShopTimes(const int times);

	void				ResetRareChestShopTimes();

	bool				AddZhuaGuiScore(int score, const char *type);
	bool				ReduceBossWeary(int boss_type, int reduce_weary);
	
	int					GetDailyUseGuildReliveTimes();
	short				GetDailyBossRedbagRewardFetchFlag();

	void				AddDailyUseGuildReliveTimes();
	void				ChangeDailyBossRedbagRewardFetchFlag(int index);

	void				FetchAdvanceNoticReward(int id);				// 领取等级预告奖励
	void				FetchAdvanceNoticDayReward(int id);				// 领取天数预告奖励
	void				AutoFetchAdvanceNoticDayReward();				// 自动领取天数奖励

	void				SendAdvanceNoticInfo();							// 等级预告信息
	void				SendAdvanceNoticDayInfo();						// 天数预告信息

	void				SendFishingStation();

	UpgradeRush*		GetUpgradeRush();

	int					GetJingJieLevel();
	bool				SetJingJieLevel(int level);
	JingJie*			GetJingJie();

	Talent*				GetTalent();
	JinJieTalent*		GetJinJieTalent();
	HeShengLuoShu*		GetHeShengLuoShu();

	bool				GetShangGuBossConcern(int layer, int index);
	void				SetShangGuBossConcern(int layer, int index, bool is_set);
	void				MsgReply(noticenum::reply_type typ, int value);

	bool                CanRealiveWithCurTire() const;						  // 复活疲劳限制
	void                DieOnReliveTireScene(bool is_die = false);            // 有复活疲劳的场景死亡
	void                SendReliveTireInfo();								  // 发送符合疲劳信息
	void                UpdateReliveTire(time_t now);						  // 更新复活疲劳

	int                 GetTeamIndex();

	void				SetUseAvatarType(int avatar_type);					  // 设置使用的头像框
	void				SetUseBubbleType(int bubble_type);					  // 设置使用的聊天框

	void				SendContinueKillInfo();								  // 发送连斩信息

	void				OnInitFinish();										  // Init() 函数已经返回,可以调用其他模块

	void				SyncChengZhuSkill(int is_chengzhu);					  // 同步攻城战城主技能

	void				SyncKillCrossBossRankSkill();						  //神域boss击杀榜第一仙盟盟主技能					  

	bool                CanCrossMizangBossRealiveWithCurTire() const;         // 复活疲劳限制
	void                DieOnCrossMizangBossScene(bool is_die = false);       // 有复活疲劳的场景死亡
	void                SendCrossMizangBossReliveTireInfo();

	bool                CanCrossYoumingBossRealiveWithCurTire() const;        // 复活疲劳限制
	void                DieOnCrossYoumingBossScene(bool is_die = false);      // 有复活疲劳的场景死亡
	void                SendCrossYoumingBossReliveTireInfo();

	int					GetKillBossNum(int boss_type);						  // 获取已击杀boss个数
	void				AddKillBossNum(int boss_type, int num);				  // 记录击杀boss个数    // 2019-04-02 修改为归属触发

	void				OnReqWorldAcquisition(Protocol::CSWorldAcquisitionLogReq *req); // 请求收购

	void				AddFightBackRole(ObjID attacker);					// 检查攻击者是否可以放入反击列表

	bool				AddActiveBossWeary(short weary);
	bool				DecActiveBossWeary(short weary);
	short				GetActiveBossWeary();

	bool				AddMikuBossWeary(short weary);
	bool				DecMikuBossWeary(short weary);
	short				GetMikuBossWeary();

	bool				OnUserRefreshBossCard(int boss_id);
	bool				GetBossFamilyKillCount(unsigned short monster_id, int &index, unsigned short &kill_count);
	bool				AddBossFamilyKillCount(unsigned short monster_id, int index, unsigned short count);
	int					GetOpenServerDay();
	int					GetOpenServerDay(const time_t time);
	time_t				GetServerStartTime();
	bool				SetHasXianzunkaFlag(int card_type);
	short				GetHasXianzunkaFlag();

protected:
	void				InitCommonData();

	void				UpdateGather(unsigned int interval);
	void				UpdateRest(time_t now);
	void				UpdateNameColor(time_t now);
	void				UpdateNewhandGuide(unsigned int now);
	void				UpdateEnergy(time_t now);

	bool				GetRoleStatusRealivePos(int *scene_id, Posi *pos);

	void				UpdateSpecialAppearance(time_t now);

	void				SendEnterInfoToRole(Role *role);
	void				RecalNextResetDataTimeStamp();
	void				CheckWeekCampBuff(time_t now_second);

	void				CheckGmTestAccount(long long last_save_timestamp);

	UserID				m_user_id;										// 整个游戏唯一的用户ID
	int					m_uid;
	IP					m_userip;										// 用户的IP
	GSNetID				m_netid;										// 网络层id
	char				m_avatar;										// 头像
	char				m_sex;											// 性别
	char				m_camp;											// 国家 阵营
	char				m_camp_post;									// 国家职位 
	char				m_prof;											// 职业
	int					m_level;										// 等级
	long long			m_exp;											// 经验
	long long			m_max_exp;										// 最大经验
	long long			m_avator_timestamp;								// 头像更改时间戳
	char				m_is_change_avatar;								// 是否改变过头像

	UInt32				m_aoi_handle;									// AOI句柄
	unsigned long		m_enter_scene_time;								// 进入当前场景时间

	PlatName			m_plat_name;									// 平台名
	Posi				m_birth_pos;									// 出生点

	int					m_last_scene_id;								// 上一个场景id
	Posi				m_last_scene_pos;								// 上一个场景位置
	int					m_online_time;									// 在线时长
	int					m_today_online_time;							// 今日在线时长
	time_t				m_create_time;									// 角色创建时间
	time_t				m_last_online_stati_time;						// 上次计算在线时长的时间
	time_t				m_last_today_online_stati_time;					// 上次计算当日在线时长的时间

	int					m_offline_day_num;								// 离线了多长时间 不存档
	int					m_lastday_online_time;							// 昨日在线时长 需要根据m_offline_day_num来算 

	time_t				m_forbid_talk_time;								// 禁言时间记录
	char				m_authority_type;								// 新增 权限类型（0：一般玩家 1：新手指导员）
	char				m_attack_mode;									// 攻击模式
	UserID				m_last_attack_me_user_id;						// 最后攻击我的角色
	LastKillMeUserInfo	m_last_kill_me_user_info;						// 最后杀死我的角色信息
	unsigned int		m_last_kill_me_time;							// 最后杀死我的时间
	time_t				m_last_guild_sos_time;							// 最后向军团求救时间
	
	time_t				m_update_last_time;								// 延迟update时间
	time_t				m_next_save_time;								// 下次保存时间	
	unsigned long		m_last_msg_from_client_time;					// 上次客户端消息时间
	unsigned int		m_last_msg_from_client_timestamp;				// 上次客户端消息时间戳
	unsigned int		m_last_online_dayid;							// 最后一天在线ID
	unsigned int		m_last_reset_module_data_dayid;					// 最后一天重置数据dayid
	unsigned int		m_last_reset_module_data_timestamp;				// 最后一天重置角色数据的时间戳
	time_t				m_next_reset_module_data_timestamp;				// 下次重置角色数据的时间戳

	RoleAppearance		m_appearance;									// 外观

	GuildID				m_guild_id;										// 军团ID
	GuildName			m_guild_name;									// 军团名字
	char				m_guild_post;									// 军团职位

	int					m_guild_gongxian;								// 帮贡
	int					m_guild_total_gongxian;

	int					m_evil;											// 罪恶值
	int					m_xianhun;										// 仙魂
	int					m_yuanli;										// 元力
	int					m_nv_wa_shi;									// 女娲石
	int					m_lingjing;										// 灵晶
	int					m_chengjiu;										// 成就
	int					m_hunli;										// 粉尘
	
	char				m_role_status;									// 正常状态 采集状态 挂机修炼状态
	ObjID				m_gather_obj_id;								// 采集对象ID
	int					m_gather_time;									// 采集时间
	int					m_gather_count;									// 连续采集次数
	time_t				m_rest_next_recovery_time;						// 下次打坐恢复时间
	time_t				m_rest_next_reward_time;						// 下次打坐奖励时间
	ObjID				m_rest_partner_obj_id;							// 双修对方objid
	time_t				m_shuangxiu_next_intimacy_time;					// 下次双修增加亲密度时间

	time_t				m_namecolor_last_dec_evil_time;					// 上次检查名字颜色时间
	char				m_name_color;									// 名字颜色

	char				m_move_mode;									// 运动模式
	char				m_move_mode_param;								// 运动模式参数

	Posi				m_jump_src_pos;									// 起跳点
	unsigned int		m_jump_last_time;								// 起跳时间

	long long			m_gm_change_maxhp;								// GM命令增加的气血
	long long			m_gm_change_gongji;								// GM命令增加的攻击
	long long			m_gm_change_movespeed;							// GM命令改变移动速度
	long long			m_gm_change_fangyu;								// GM命令改变防御
	long long			m_gm_change_mingzhong;							// GM命令改变命中
	long long			m_gm_change_shanbi;								// GM命令改变闪避
	long long			m_gm_change_baoji;								// GM命令改变暴击
	long long			m_gm_change_jianren;							// GM命令改变坚韧
	long long			m_gm_change_fujia_shanghai;						// GM命令改变附加伤害
	long long			m_gm_change_dikang_shanghai;					// GM命令改变抵抗伤害
	long long			m_gm_change_per_jingzhun;						// GM命令改变精准
	long long			m_gm_change_per_baoji;							// GM命令改变幸运一击
	long long			m_gm_change_per_kangbao;						// GM命令改变抵抗幸运一击
	long long			m_gm_change_per_pofang;							// GM命令改变破防百分比
	long long			m_gm_change_per_mianshang;						// GM命令改变免伤百分比
	long long			m_gm_change_constant_zengshang;					// GM命令改变固定增伤
	long long			m_gm_change_constant_mianshang;					// GM命令改变固定免伤
	long long			m_gm_change_capability_attr;					// GM命令改变战力,设置属性

	unsigned int		m_last_singlechat_monitor_time;									// 上次私聊统计一分钟超过十人的时间
	char				m_last_singlechat_monitor_msg[MAX_SINGLE_CHAT_MONITOR_LENGTH];	// 上次私聊统计一分钟超过十人的消息

	unsigned int		m_last_worldchat_monitor_time;
	char				m_last_worldchat_monitor_msg[MAX_WORLD_CHAT_MONITOR_LENGTH];	// 世界频道监控

	unsigned int		m_last_mail_monitor_time;
	char				m_last_mail_monitor_subject[MAX_MAIL_MONITOR_SUBJECT_LENGTH];	// 邮件监控

	static const int	MAIL_SEND_MONITOR_INTERVAL = 60 * 5;			// 邮件监控间隔
	static const int	MAIL_SEND_MONITOR_MAX_COUNT = 90;

	unsigned int		m_mail_monitor_start_time;
	int					m_mail_send_count;

	bool				m_has_send_all_info;							// 是否执行过OnAllInfoReq

	bool				m_is_zombie;									// 是否僵尸用户
	bool				m_isforce_kickdelay;							// 是否要延迟踢这个玩家，主要是顶号的时候用

	int					m_honour;										// 阵营荣誉

	static const int	MOUNT_BIANSHEN_BEGIN_LEVEL = 40;
	static const int	MOUNT_BIANSHEN_END_LEVEL = 53;

	bool				m_is_mount_bianshen;							// 是否坐骑变身
	unsigned int		m_mount_bianshen_time;							// 变身结束时间

	char				m_is_reserve_marry;								// 是否预定过婚礼
	unsigned int		m_last_hunyan_bless_marry_start_time;			// 上一次婚宴祝福的婚礼开始时间
	unsigned int		m_last_marry_time;								// 上次结婚时间
	unsigned int		m_last_divorce_time;							// 上次离婚时间

	unsigned int		m_last_create_guild_time;						// 上一次创建军团时间
	int					m_speedup_hello_count;							// 加速检测包的数量
	int					m_speedup_hello_total_count;					// 加速检测包的数量 累计的 不存档 
	time_t				m_first_speedup_hello_time;						// 上次加速检查包的时间

	unsigned int 		m_login_timestamp;								// 登录时间戳
	time_t				m_check_week_camp_buff_next_timestamp;			// 检查弱阵营buff时间戳

	bool				m_top_dps_flag;
	bool				m_first_hit_flag;
	bool				m_first_hit_have_died;							// 第一刀之后是否死亡过
	bool				m_need_send_attrinfo;							// 属性的计算触发性。 可能一个事件调用N次计算。最终发送N次消息改为Update判断标记推送

	int					m_cooldown_owe_time;							// 全局cooldown欠的时间

	int					m_tick_attack_count;							// 统计一帧触发attack次数. 超过100肯定异常了  打log
	int					m_tick_attack_recal_count;						// 统计一帧触发attack次数. 超过100肯定异常了  打log

	int					m_next_attack_extra_hurt_percent;				// 下一次攻击额外增加对敌人伤害百分比

	int					m_wait_scene_frame_num;							// 跨服等待场景超时下线帧数（卡在中转等待场景一定帧数则踢出玩家下线）

	// 新模块都加这里
	RoleModule			*m_role_module;									// 角色模块

	RoleOtherMgr		*m_role_other_mgr;								// Role其他系统管理器

	//std::vector<IRoleModule*>	m_role_module_list;						// 所有角色模块列表
};

#endif

