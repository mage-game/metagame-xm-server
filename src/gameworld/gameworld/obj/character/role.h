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
		AUTHORITY_TYPE_INVALID = 0,									// ���κ�Ȩ��
		AUTHORITY_TYPE_GUIDER,										// ����ָ��Ա
		AUTHORITY_TYPE_GM,											// GM
		AUTHORITY_TYPE_TEST,										// �����ʺţ��ڲ��ţ�
		AUTHORITY_TYPE_MAX,
	};

	enum ATTACK_MODE
	{
		ATTACK_MODE_PEACE = 0,										// ��ƽģʽ
		ATTACK_MODE_TEAM,											// ���˺���ͬ�����
		ATTACK_MODE_GUILD,											// ����ģʽ
		ATTACK_MODE_ALL,											// ȫ��ģʽ
		ATTACK_MODE_NAMECOLOR,										// ֻ�����������
		ATTACK_MODE_CAMP,											// ��Ӫģʽ
		ATTACK_MODE_SAME_SERVER,									// ����ģʽ
		ATTACK_MODE_ENEMY,											// ����ģʽ
		ATTACK_MODE_MAX,
	};

	enum _RoleStatus
	{
		ROLE_STATUS_CHARACTER = 0,									// Chanracter::m_action_status������Ӧ״̬����Ӧ���߼�
																	// ע�� ���ڷ�ROLE_CHARACTER״̬ʱChanracter::m_action_status���봦��ACTION_IDLE 
																	// ����CanCharacterAction()����true
		ROLE_STATUS_GATHER,											// �ɼ�״̬
		ROLE_STATUS_REST,											// ������Ϣ״̬
	};

	enum MOVE_MODE
	{
		MOVE_MODE_NORMAL = 0,										// ����
		MOVE_MODE_FLY,												// ����
		MOVE_MODE_JUMP,												// ��Ծ
		MOVE_MODE_JUMP2,											// ��Ծ
		MOVE_MODE_MAX
	};

	enum MOVE_MODE_FLY_PARAM
	{
		MOVE_MODE_FLY_PARAM_INVALID = 0,
		MOVE_MODE_FLY_PARAM_DRAGON = 1,								// ��
		MOVE_MODE_FLY_PARAM_QILIN = 2,								// ����
		
		MOVE_MODE_FLY_PARAM_MAX
	};

	enum EXP_ADD_REASON
	{
		EXP_ADD_REASON_INVALID = -1,

		EXP_ADD_REASON_DEFAULT = 0,									// Ĭ�ϵ�
		EXP_ADD_REASON_KILL_MONSTER = 1,							// ɱ��
		EXP_ADD_REASON_ZHUAN_SHENG = 2,								// ת��
		EXP_ADD_REASON_EXP_ITEM = 3,								// ʹ����Ʒ���Ӿ���
		EXP_ADD_REASON_WATCH = 4,									// �鿴����ӳ�
		EXP_ADD_REASON_HUSONG = 5,									// ����
		EXP_ADD_EXP_FB = 6,											// ���鸱�����⾭��
		EXP_ADD_REASON_TRUNK_TASK = 7,								// ��������
		EXP_ADD_REASON_EXP_FB_BAODI = 8,							// ���鸱�����׾���ӳ�
		EXP_ADD_REASON_EXP_FRIEND_HELI = 9,							// ���Ѻ���Ӿ���
		EXP_ADD_REASON_EXP_FISH_POOL = 10,							// ����Ӿ���
		EXP_ADD_FRIEND_EXP_BOTTLE = 11,								// ����ƿ
		EXP_ADD_WORSHIP = 12,										// Ĥ�ݳ���
		EXP_ADD_TIANCITONGBI = 13,									// �������ͭ��
		EXP_ADD_DAILY_TASK_EXP_BALL = 14,							// �ճ���������
		EXP_ADD_REASON_UPLEVEL_PILL = 15,							// ������
		EXP_ADD_REASON_EXP_REFINE = 16,                             // ��������
		EXP_ADD_REASON_EXP_BRANCH_TASK = 17,						// ֧������,��ʱû���õ�,��Ϊ����֧�߶����õ�����7�Ӿ����,��Ԥ����

		EXP_ADD_REASON_MAX,
	};

	enum CHANGE_MODE_TASK_TYPE
	{
		CHANGE_MODE_TASK_TYPE_INVALID = 0,
		CHANGE_MODE_TASK_TYPE_GATHER = 1,						// �ɼ���
		CHANGE_MODE_TASK_TYPE_TALK_TO_NPC = 2,					// ��NPC�Ի���
		CHANGE_MODE_TASK_TYPE_CHANGE_IMAGE = 3,					// ����
		CHANGE_MODE_TASK_TYPE_FLY = 4,							// ����


		CHANGE_MODE_APPEARN_MAX
	};

	enum RESET_ROLE_STATUS_REASON
	{
		RESET_ROLE_STATUS_REASON_NONE,
		RESET_ROLE_STATUS_REASON_MOVE,						 // �ƶ�
		RESET_ROLE_STATUS_REASON_GATHER_SUCC,				 // �ɼ��ɹ�
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

	//��ģ�����ݳ�ʼ��,�������еĵ���ֻ������ǰ����ģ������. ������role->getothermodule()->dosomething();
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
	void				OnLoginFinish(long long last_save_time);  // ��¼���

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

	short				GetBaseProf() { return m_prof % PROF_TYPE_PROF_ZHUANZHI_BASE; }		// ְҵ
	short				GetProfLevel() { return m_prof / PROF_TYPE_PROF_ZHUANZHI_BASE; }	// תְ�ȼ�
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
	void				SendBeKillMail();	// ��ɱ�ʼ�

	virtual void		OnAttackCalcHurt(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill, int fight_type);
	virtual void		OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill);
	virtual int			OnHurtOther(Character *target, int skill_id, long long &injure, int fight_type, int target_index);
	void				OnAfterHurtOther(Character *target, int skill_id, long long injure, bool from_skill, int fight_type);  // �Ѿ��Ա���������˺�

	void				OnKillOtherRole(Role * target_role);

	void				OnStartFight(); // ����ս��״̬����
	void				OnStopFight(); // �˳�ս��״̬����

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

	// ������һ�ι����������ӶԵ����˺��ٷֱ�
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

	// ����������
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

	void				SendAddGuildGongziInfo(int gongzi);		// �������˹���������Ϣ

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
	void				SetGmChangeCapabilityAttr(Attribute cap) { m_gm_change_capability_attr = cap; }   // һ������ս���Ͷ�Ӧ���ԡ�С�ڵ���0������Ч
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

	virtual int GetHurtIncPercent(Character *target);		// �˺��ӳ�
	virtual int GetHurtDecPercent(Character *performer);	// �˺�����

	virtual bool		IsMianyiDeadlyAttack();				// �Ƿ�����������һ�ι���
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

	void				FetchAdvanceNoticReward(int id);				// ��ȡ�ȼ�Ԥ�潱��
	void				FetchAdvanceNoticDayReward(int id);				// ��ȡ����Ԥ�潱��
	void				AutoFetchAdvanceNoticDayReward();				// �Զ���ȡ��������

	void				SendAdvanceNoticInfo();							// �ȼ�Ԥ����Ϣ
	void				SendAdvanceNoticDayInfo();						// ����Ԥ����Ϣ

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

	bool                CanRealiveWithCurTire() const;						  // ����ƣ������
	void                DieOnReliveTireScene(bool is_die = false);            // �и���ƣ�͵ĳ�������
	void                SendReliveTireInfo();								  // ���ͷ���ƣ����Ϣ
	void                UpdateReliveTire(time_t now);						  // ���¸���ƣ��

	int                 GetTeamIndex();

	void				SetUseAvatarType(int avatar_type);					  // ����ʹ�õ�ͷ���
	void				SetUseBubbleType(int bubble_type);					  // ����ʹ�õ������

	void				SendContinueKillInfo();								  // ������ն��Ϣ

	void				OnInitFinish();										  // Init() �����Ѿ�����,���Ե�������ģ��

	void				SyncChengZhuSkill(int is_chengzhu);					  // ͬ������ս��������

	void				SyncKillCrossBossRankSkill();						  //����boss��ɱ���һ������������					  

	bool                CanCrossMizangBossRealiveWithCurTire() const;         // ����ƣ������
	void                DieOnCrossMizangBossScene(bool is_die = false);       // �и���ƣ�͵ĳ�������
	void                SendCrossMizangBossReliveTireInfo();

	bool                CanCrossYoumingBossRealiveWithCurTire() const;        // ����ƣ������
	void                DieOnCrossYoumingBossScene(bool is_die = false);      // �и���ƣ�͵ĳ�������
	void                SendCrossYoumingBossReliveTireInfo();

	int					GetKillBossNum(int boss_type);						  // ��ȡ�ѻ�ɱboss����
	void				AddKillBossNum(int boss_type, int num);				  // ��¼��ɱboss����    // 2019-04-02 �޸�Ϊ��������

	void				OnReqWorldAcquisition(Protocol::CSWorldAcquisitionLogReq *req); // �����չ�

	void				AddFightBackRole(ObjID attacker);					// ��鹥�����Ƿ���Է��뷴���б�

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

	UserID				m_user_id;										// ������ϷΨһ���û�ID
	int					m_uid;
	IP					m_userip;										// �û���IP
	GSNetID				m_netid;										// �����id
	char				m_avatar;										// ͷ��
	char				m_sex;											// �Ա�
	char				m_camp;											// ���� ��Ӫ
	char				m_camp_post;									// ����ְλ 
	char				m_prof;											// ְҵ
	int					m_level;										// �ȼ�
	long long			m_exp;											// ����
	long long			m_max_exp;										// �����
	long long			m_avator_timestamp;								// ͷ�����ʱ���
	char				m_is_change_avatar;								// �Ƿ�ı��ͷ��

	UInt32				m_aoi_handle;									// AOI���
	unsigned long		m_enter_scene_time;								// ���뵱ǰ����ʱ��

	PlatName			m_plat_name;									// ƽ̨��
	Posi				m_birth_pos;									// ������

	int					m_last_scene_id;								// ��һ������id
	Posi				m_last_scene_pos;								// ��һ������λ��
	int					m_online_time;									// ����ʱ��
	int					m_today_online_time;							// ��������ʱ��
	time_t				m_create_time;									// ��ɫ����ʱ��
	time_t				m_last_online_stati_time;						// �ϴμ�������ʱ����ʱ��
	time_t				m_last_today_online_stati_time;					// �ϴμ��㵱������ʱ����ʱ��

	int					m_offline_day_num;								// �����˶೤ʱ�� ���浵
	int					m_lastday_online_time;							// ��������ʱ�� ��Ҫ����m_offline_day_num���� 

	time_t				m_forbid_talk_time;								// ����ʱ���¼
	char				m_authority_type;								// ���� Ȩ�����ͣ�0��һ����� 1������ָ��Ա��
	char				m_attack_mode;									// ����ģʽ
	UserID				m_last_attack_me_user_id;						// ��󹥻��ҵĽ�ɫ
	LastKillMeUserInfo	m_last_kill_me_user_info;						// ���ɱ���ҵĽ�ɫ��Ϣ
	unsigned int		m_last_kill_me_time;							// ���ɱ���ҵ�ʱ��
	time_t				m_last_guild_sos_time;							// �����������ʱ��
	
	time_t				m_update_last_time;								// �ӳ�updateʱ��
	time_t				m_next_save_time;								// �´α���ʱ��	
	unsigned long		m_last_msg_from_client_time;					// �ϴοͻ�����Ϣʱ��
	unsigned int		m_last_msg_from_client_timestamp;				// �ϴοͻ�����Ϣʱ���
	unsigned int		m_last_online_dayid;							// ���һ������ID
	unsigned int		m_last_reset_module_data_dayid;					// ���һ����������dayid
	unsigned int		m_last_reset_module_data_timestamp;				// ���һ�����ý�ɫ���ݵ�ʱ���
	time_t				m_next_reset_module_data_timestamp;				// �´����ý�ɫ���ݵ�ʱ���

	RoleAppearance		m_appearance;									// ���

	GuildID				m_guild_id;										// ����ID
	GuildName			m_guild_name;									// ��������
	char				m_guild_post;									// ����ְλ

	int					m_guild_gongxian;								// �ﹱ
	int					m_guild_total_gongxian;

	int					m_evil;											// ���ֵ
	int					m_xianhun;										// �ɻ�
	int					m_yuanli;										// Ԫ��
	int					m_nv_wa_shi;									// Ů�ʯ
	int					m_lingjing;										// �龧
	int					m_chengjiu;										// �ɾ�
	int					m_hunli;										// �۳�
	
	char				m_role_status;									// ����״̬ �ɼ�״̬ �һ�����״̬
	ObjID				m_gather_obj_id;								// �ɼ�����ID
	int					m_gather_time;									// �ɼ�ʱ��
	int					m_gather_count;									// �����ɼ�����
	time_t				m_rest_next_recovery_time;						// �´δ����ָ�ʱ��
	time_t				m_rest_next_reward_time;						// �´δ�������ʱ��
	ObjID				m_rest_partner_obj_id;							// ˫�޶Է�objid
	time_t				m_shuangxiu_next_intimacy_time;					// �´�˫���������ܶ�ʱ��

	time_t				m_namecolor_last_dec_evil_time;					// �ϴμ��������ɫʱ��
	char				m_name_color;									// ������ɫ

	char				m_move_mode;									// �˶�ģʽ
	char				m_move_mode_param;								// �˶�ģʽ����

	Posi				m_jump_src_pos;									// ������
	unsigned int		m_jump_last_time;								// ����ʱ��

	long long			m_gm_change_maxhp;								// GM�������ӵ���Ѫ
	long long			m_gm_change_gongji;								// GM�������ӵĹ���
	long long			m_gm_change_movespeed;							// GM����ı��ƶ��ٶ�
	long long			m_gm_change_fangyu;								// GM����ı����
	long long			m_gm_change_mingzhong;							// GM����ı�����
	long long			m_gm_change_shanbi;								// GM����ı�����
	long long			m_gm_change_baoji;								// GM����ı䱩��
	long long			m_gm_change_jianren;							// GM����ı����
	long long			m_gm_change_fujia_shanghai;						// GM����ı丽���˺�
	long long			m_gm_change_dikang_shanghai;					// GM����ı�ֿ��˺�
	long long			m_gm_change_per_jingzhun;						// GM����ı侫׼
	long long			m_gm_change_per_baoji;							// GM����ı�����һ��
	long long			m_gm_change_per_kangbao;						// GM����ı�ֿ�����һ��
	long long			m_gm_change_per_pofang;							// GM����ı��Ʒ��ٷֱ�
	long long			m_gm_change_per_mianshang;						// GM����ı����˰ٷֱ�
	long long			m_gm_change_constant_zengshang;					// GM����ı�̶�����
	long long			m_gm_change_constant_mianshang;					// GM����ı�̶�����
	long long			m_gm_change_capability_attr;					// GM����ı�ս��,��������

	unsigned int		m_last_singlechat_monitor_time;									// �ϴ�˽��ͳ��һ���ӳ���ʮ�˵�ʱ��
	char				m_last_singlechat_monitor_msg[MAX_SINGLE_CHAT_MONITOR_LENGTH];	// �ϴ�˽��ͳ��һ���ӳ���ʮ�˵���Ϣ

	unsigned int		m_last_worldchat_monitor_time;
	char				m_last_worldchat_monitor_msg[MAX_WORLD_CHAT_MONITOR_LENGTH];	// ����Ƶ�����

	unsigned int		m_last_mail_monitor_time;
	char				m_last_mail_monitor_subject[MAX_MAIL_MONITOR_SUBJECT_LENGTH];	// �ʼ����

	static const int	MAIL_SEND_MONITOR_INTERVAL = 60 * 5;			// �ʼ���ؼ��
	static const int	MAIL_SEND_MONITOR_MAX_COUNT = 90;

	unsigned int		m_mail_monitor_start_time;
	int					m_mail_send_count;

	bool				m_has_send_all_info;							// �Ƿ�ִ�й�OnAllInfoReq

	bool				m_is_zombie;									// �Ƿ�ʬ�û�
	bool				m_isforce_kickdelay;							// �Ƿ�Ҫ�ӳ��������ң���Ҫ�Ƕ��ŵ�ʱ����

	int					m_honour;										// ��Ӫ����

	static const int	MOUNT_BIANSHEN_BEGIN_LEVEL = 40;
	static const int	MOUNT_BIANSHEN_END_LEVEL = 53;

	bool				m_is_mount_bianshen;							// �Ƿ��������
	unsigned int		m_mount_bianshen_time;							// �������ʱ��

	char				m_is_reserve_marry;								// �Ƿ�Ԥ��������
	unsigned int		m_last_hunyan_bless_marry_start_time;			// ��һ�λ���ף���Ļ���ʼʱ��
	unsigned int		m_last_marry_time;								// �ϴν��ʱ��
	unsigned int		m_last_divorce_time;							// �ϴ����ʱ��

	unsigned int		m_last_create_guild_time;						// ��һ�δ�������ʱ��
	int					m_speedup_hello_count;							// ���ټ���������
	int					m_speedup_hello_total_count;					// ���ټ��������� �ۼƵ� ���浵 
	time_t				m_first_speedup_hello_time;						// �ϴμ��ټ�����ʱ��

	unsigned int 		m_login_timestamp;								// ��¼ʱ���
	time_t				m_check_week_camp_buff_next_timestamp;			// �������Ӫbuffʱ���

	bool				m_top_dps_flag;
	bool				m_first_hit_flag;
	bool				m_first_hit_have_died;							// ��һ��֮���Ƿ�������
	bool				m_need_send_attrinfo;							// ���Եļ��㴥���ԡ� ����һ���¼�����N�μ��㡣���շ���N����Ϣ��ΪUpdate�жϱ������

	int					m_cooldown_owe_time;							// ȫ��cooldownǷ��ʱ��

	int					m_tick_attack_count;							// ͳ��һ֡����attack����. ����100�϶��쳣��  ��log
	int					m_tick_attack_recal_count;						// ͳ��һ֡����attack����. ����100�϶��쳣��  ��log

	int					m_next_attack_extra_hurt_percent;				// ��һ�ι����������ӶԵ����˺��ٷֱ�

	int					m_wait_scene_frame_num;							// ����ȴ�������ʱ����֡����������ת�ȴ�����һ��֡�����߳�������ߣ�

	// ��ģ�鶼������
	RoleModule			*m_role_module;									// ��ɫģ��

	RoleOtherMgr		*m_role_other_mgr;								// Role����ϵͳ������

	//std::vector<IRoleModule*>	m_role_module_list;						// ���н�ɫģ���б�
};

#endif

