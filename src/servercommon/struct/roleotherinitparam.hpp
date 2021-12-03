/******************************************************************************************
FileName: roleotherparam.hpp
Author: dreamwang
Description: 角色打包数据统一管理 用于存储各种打包的数据 各个系统自己做版本兼容 用于复杂的系统
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2012/02/3/ 10:49:05     1.0           创建
*****************************************************************************************/

#ifndef __ROLE_OTHER_INIT_PARAM_HPP__
#define __ROLE_OTHER_INIT_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/basedef.h"
#include "servercommon/chatserverdef.h"

#include "servercommon/mentalitydef.hpp"
#include "servercommon/vipdef.hpp"
#include "servercommon/roleshopdef.hpp"
#include "servercommon/activedegreedef.hpp"
#include "servercommon/welfaredef.hpp"
#include "servercommon/rolehangupdef.hpp"
#include "servercommon/itemcolddowndef.hpp"
#include "servercommon/evaluatedef.hpp"
#include "servercommon/monitordef.hpp"
#include "servercommon/titledef.hpp"
#include "servercommon/taskdef.hpp"
#include "servercommon/roleactivitydef.hpp"
#include "servercommon/fbdef.hpp"
#include "servercommon/commondata.hpp"
#include "servercommon/wingdef.hpp"

#include "servercommon/struct/mountparam.hpp"
#include "servercommon/struct/fightmountparam.hpp"
#include "servercommon/stonedef.hpp"
#include "servercommon/guilddef.hpp"
#include "servercommon/mazedef.hpp"
#include "servercommon/struct/xiannvparam.hpp"
#include "servercommon/struct/converrecordparam.hpp"

#include "servercommon/struct/discountbuyparam.hpp"
#include "servercommon/struct/othermoduledata.hpp"
#include "servercommon/qingyuandef.hpp"
#include "servercommon/babydef.hpp"
#include "servercommon/struct/rolegoalparam.hpp"
#include "servercommon/struct/shizhuangparam.hpp"
#include "servercommon/struct/historycapparam.hpp"
#include "servercommon/struct/skilllistparam.h"
#include "servercommon/rolecrossdef.hpp"
#include "servercommon/shengwangdef.hpp"
#include "servercommon/chengjiudef.hpp"
#include "servercommon/lifeskilldef.hpp"
#include "servercommon/equipsuitdef.hpp"
#include "servercommon/shenzhuangdef.hpp"

#include "servercommon/tuhaojindef.hpp"
#include "servercommon/bigchatfacedef.hpp"
#include "servercommon/petdef.hpp"
#include "servercommon/huashendef.hpp"
#include "servercommon/magiccarddef.hpp"
#include "servercommon/mitamadef.hpp"
#include "servercommon/chinesezodiacdef.hpp"
#include "servercommon/struct/friendgiftparam.hpp"
#include "servercommon/followbossdef.hpp"
#include "servercommon/goldvipdef.hpp"
#include "servercommon/struct/zerogiftparam.hpp"

#include "servercommon/multimountdef.hpp"
#include "servercommon/personalizewindowdef.hpp"
#include "servercommon/struct/xunzhangparam.hpp"
#include "servercommon/struct/zhibaoparam.hpp"
#include "servercommon/halodef.hpp"
#include "servercommon/shengongdef.hpp"
#include "servercommon/shenyidef.hpp"
#include "servercommon/footprintdef.hpp"
#include "servercommon/cloakdef.hpp"
#include "servercommon/jinglingdef.hpp"
#include "servercommon/xiannvshouhudef.hpp"
#include "servercommon/jinglingfazhendef.hpp"
#include "servercommon/jinglingguanghuandef.hpp"
#include "servercommon/shenzhouweapondef.hpp"
#include "servercommon/liemingdef.hpp"
#include "servercommon/cardzudef.hpp"
#include "servercommon/zhuanshengequipdef.hpp"
#include "servercommon/struct/mojieparam.hpp"
#include "servercommon/runesystemdef.hpp"
#include "servercommon/shengesystemdef.hpp"
#include "servercommon/struct/shenbingparam.hpp"
#include "servercommon/struct/commonactivityparam.hpp"
#include "servercommon/magicalpreciousdef.hpp"
#include "servercommon/mysteriousshopinmalldef.hpp"
#include "servercommon/struct/friendexpbottleparam.hpp"
#include "servercommon/shenshoudef.hpp"
#include "servercommon/imgfulingdef.hpp"
#include "servercommon/combineserverequipdef.hpp"
#include "servercommon/newmultimountdef.hpp"
#include "servercommon/shenyindef.hpp"
#include "servercommon/elementheartdef.hpp"
#include "servercommon/shengxingzhulidef.hpp"
#include "servercommon/struct/fabaoparam.hpp"
#include "servercommon/feixianparam.hpp"
#include "servercommon/talentdef.hpp"
#include "servercommon/jinjietalentdef.hpp"
#include "servercommon/heshenluoshudef.hpp"
#include "servercommon/struct/shengqiparam.hpp"
#include "servercommon/bosscarddef.hpp"
#include "servercommon/crossroleglobaldef.hpp"
#include "servercommon/struct/yaoshiparam.hpp"
#include "servercommon/struct/toushiparam.hpp"
#include "servercommon/struct/qilinbiparam.hpp"
#include "servercommon/struct/maskparam.hpp"
#include "servercommon/zhuanzhiequipdef.hpp"
#include "servercommon/struct/sealparam.hpp"
#include "servercommon/dressingroomdef.hpp"
#include "servercommon/struct/upgradeparam.hpp"
#include "servercommon/struct/jinjiesysrewardparam.hpp"
#include "rolebigsmallgoalparam.hpp"
#include "servercommon/shenqidef.hpp"
#include "redequipcollectdef.hpp"
#include "orangeequipcollectdef.hpp"
#include "servercommon/xianzunkadef.hpp"
#include "servercommon/equipbaptizedef.hpp"
#include "servercommon/greatesoldierdef.hpp"
#include "servercommon/rolelotterycountdef.hpp"
#include "servercommon/rolecrossrandactivitydef.hpp"
#include "servercommon/struct/upgradecardparam.hpp"
#include "servercommon/huguozhilidef.hpp"
#include "servercommon/crossgoaldef.hpp"
#include "servercommon/baizhanequipdef.hpp"
#include "servercommon/crossequipdef.hpp"
#include "servercommon/mythdef.hpp"
#include "zodiacparam.hpp"
#include "servercommon/tianshenhutidef.hpp"

class RoleOtherInitParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	void Reset();

	// 聊天监控
	unsigned int last_singlechat_monitor_time;
	char last_singlechat_monitor_msg[MAX_SINGLE_CHAT_MONITOR_LENGTH];

	unsigned int last_worldchat_monitor_time;
	char last_worldchat_monitor_msg[MAX_WORLD_CHAT_MONITOR_LENGTH];

	unsigned int last_mail_monitor_time;
	char last_mail_monitor_subject[MAX_MAIL_MONITOR_SUBJECT_LENGTH];

	// 寻宝
	static const int ROLE_CHEST_SHOP_BUFF_LENGTH = 12864;
	static const int ROLE_CHEST_SHOP_BUFF_LENGTH_HEX = ROLE_CHEST_SHOP_BUFF_LENGTH * 2 + 1;
	UNSTD_STATIC_CHECK(ROLE_CHEST_SHOP_BUFF_LENGTH_HEX <= 25800);

	int chest_buff_len;
	char chest_buff[ROLE_CHEST_SHOP_BUFF_LENGTH];

	// effect
	static const int MAX_EFFECT_NUM = 24;					// 同时存在的效果不可能超过24个
	static const int EFFECT_BUFFER_LENGTH = 128;				// 效果buff长度
	static const int EFFECT_BUFFER_LENGTH_HEX = MAX_EFFECT_NUM * EFFECT_BUFFER_LENGTH * 2 + 1;
	UNSTD_STATIC_CHECK(EFFECT_BUFFER_LENGTH_HEX <= 8192);

	int effect_buff_len;
	char effect_buff[MAX_EFFECT_NUM * EFFECT_BUFFER_LENGTH];

	// 任务记录
	static const int MAX_TASK_RECORD_BUFF_LEN_HEX = MAX_TASK_RECORD_BUFF_LEN * 2;
	UNSTD_STATIC_CHECK(MAX_TASK_RECORD_BUFF_LEN_HEX <= 4096)

	int taskrecord_buff_len;
	char taskrecord_buff[MAX_TASK_RECORD_BUFF_LEN];

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// 表1
	MentalityParam mentality_param;
	VipParam vip_param;
	RoleShopParam roleshop_param;
	ActiveDegreeParam active_degree_param;
	WelfareParam welfare_param;
	RoleHangupParam rolehangup_param;
	ItemColddownParam itemcolddown_param;
	EvaluateParam evaluate_param;
	MonitorParam monitor_param;
	TitleParam title_param;
	HusongTaskParam husong_param;
	RoleActivityParam roleactivity_param;
	TowerDefendFBParam towerdefend_param;
	CommonDataParam common_data_param;
	WingParam wing_param;

	// 表2
	EquipFBParam equipfb_param;
	GuildTaskParam guild_task_param;
	NewEquipFBParam newequipfb_param;
	StoneParam stone_param;
	GuildXianshuParam guild_xianshu_param;
	TaskParam task_param;
	RoleSupplyParam role_supply_param;
	MazeParam maze_param;
	XiannvParam xiannv_param;
	StoryFBParam storyfb_param;
	ConvertRecordParam convert_record_param;
	FightMountParam fight_mount_param;
	MojieFBParam mojiefb_param;
	EquipSuitParam equipsuit_param;
	ShenZhuangParam shenzhuang_param;
	CSAEquipParam csa_equip_param;

	// 表3
	OtherModuleDataParam other_module_param;
	QingyuanParam qingyuan_param;
	RoleGoalParam role_goal_param;
	ShizhuangParam shizhuang_param;
	HistoryCapParam history_camp_param;
	SkillOtherParam skill_other_param;
	RoleCrossParam role_cross_param;
	RoleGuildParam role_guild_param;
	ShengWangParam shengwang_param;
	ChengJiuParam chengjiu_param;
	DailyFbParam dailyfb_param;
	LifeSkillParam lifeskill_param;
	BabyParam baby_param;
	ShengeSystemParam shenge_system_param;
	ZeroGiftParam zero_gift_param;
	MysteriousShopInMallParam mysterious_shop_in_mall_param;
	FriendExpBottleParam friend_exp_bottle_param;
	ShenshouParam shenshou_param;
	ImgFulingParam img_fuling_param;

	// 表4
	TuHaoJinParam tuhaojin_param;
	BigChatFaceParam bigchatface_param;
	PetParam pet_param;
	PhaseFbParam phase_fb_param;
	VipFBParam vipfb_param;
	HuashenParam huashen_param;
	MagicCardParam magic_card_param;
	MitamaParam mitama_param;
	EquipmentGridParam equipment_param;
	ChineseZodiacParam chinese_zodiac_param;
	FriendGiftParam friend_gift_param;
	FollowBossParam follow_boss_param;
	GoldVipParam gold_vip_param;
	RuneSystemParam rune_system_param;
	MagicalPreciousParam magical_precious_param;

	// 表5
	MultiMountParam multi_mount_param;
	PersonalizeWindowParam personalize_window_param;
	XunZhangParam xunzhang_param;
	ZhiBaoParam zhibao_param;
	ShengongParam shengong_param;
	ShenyiParam shenyi_param;
	JingLingParam jingling_param;
	XiannvShouhuParam xiannv_shouhu_param;
	JinglingFazhenParam jingling_fazhen_param;
	JinglingGuanghuanParam jingling_guanghuan_param;
	ShenzhouWeaponParam shenzhou_weapon_param;
	LieMingParam lieming_param;
	LieMingHunshouBagParam lieming_bag_param;
	CardzuParam cardzu_param;
	ZhuanShengEquipParam zhuansheng_equip_param;
	ShenBingParam shenbing_param;
	TuituFbParam tuitu_fb_param;
	CloakParam cloak_param;
	PreciousBossParam precious_boss_param;

	// 表6
	CoupleMountParam couple_mount_param;
	ShenYinParam shenyin_param;
	ElementHeartParam element_heart_param;
	ShengxingzhuliParam shengxing_zhuli_param;
	FabaoParam fabao_param;
	FeiXianParam feixian_param; // 飞仙装备属性
	TalentParam talent_param; //天赋数据
	JinJieTalentParam jinjie_talent_param; // 进阶天赋系统
	ArmorDefendFBParam armor_fb_param; //防具材料副本
	HeShenLuoShuParam heshen_luoshu_param; // 河神洛书
	ShengqiParam shengqi_param;			   //圣器系统
	BossCardParam bosscard_param;      // boss图鉴
	ZhuanzhiTaskParam task_zhuanzhi_param;
	JinjieSysRewardParam jinjiesys_reward_param;	//进阶系统目标奖励
	RoleBigSmallGoalRewardParam role_big_small_goal_param;   // 角色大小目标
	DiscountBuyParam discount_buy_param;

	// 表7
	RoleCrossGlobalData cross_global_data_param;
	YaoShiParam yaoshi_param;
	TouShiParam toushi_param;
	QiLinBiParam qilinbi_param;
	MaskParam mask_param;
	ZhuanzhiEquipParam zhuanzhi_equip_param;
	SealParam seal_param;		//圣印系统
	DressingRoomParam dressing_room_param;
	ChallengeFBParam challengefb_param;
	RoleCrossRandActivityParam role_cross_rand_activity_param;
	CrossGoalParam cross_goal_param;

	// 表8
	UpgradeManagerParam upgrade_manager_param;
	ShenqiParam shenqi_param;
	LieMingParam2 lieming_param2;
	PersonProfessParam profess_param;
	XianZunKaParam xianzunka_param;
	EquipBaptizeParam equipbaptize_param;
	PersonBossParam personboss_param;
	MojieParam mojie_param;
	GreateSoldierParam greate_soldier_param;
	RoleLotteryCountParam role_lottery_count_param;
	UpgradeCardBuyParam upgrade_card_buy_param;
	MythParam myth_param;
	ZodiacParam zodiac_param;
	CrossEquipParam cross_equip_param;

	// 表9
	RedEquipCollectParam red_equip_collect_param;
	MountParam mount_param;
	FootprintParam footprint_param;
	HaloParam halo_param;
	HuguozhiliParam huguozhili_param;
	BaizhanEquipParam baizhan_equip_param;
	DisCountShopParam discountbuy_param;

	// 表10
	OrangeEquipCollectParam orange_equip_collect_param;
	mutable DataPageParam data_page_10;

	CommonActivityParam common_activity_param;
};

#endif // __ROLE_OTHER_INIT_PARAM_HPP__

