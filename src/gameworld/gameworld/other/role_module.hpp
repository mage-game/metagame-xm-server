#ifndef __ROLE_MODULE_HPP__
#define __ROLE_MODULE_HPP__

#include "servercommon/commondata.hpp"
#include "equipment/equipmentmanager.h"
#include "other/friendexpbottle/friendexpbottle.hpp"
#include "other/jingling/jinglingmanager.h"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/shop/mysteriousshopinmall.hpp"
#include "global/guild/roleguild.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/runesystem/runesystem.hpp"
#include "other/shenzhouweapon/shenzhouweapon.hpp"
#include "other/xiannv/xiannvmanager.hpp"
#include "other/fb/rolefbtuitu.h"

#include "item/knapsack.h"
#include "item/hotkeyrecorder.h"
#include "friend/friendmanager.h"
#include "task/newtaskmanager.h"
#include "task/taskrecorder.h"
#include "other/rank/rankshadow.h"
#include "other/rolegoal/rolegoal.hpp"
#include "friend/teaminfo.hpp"
#include "other/mentality/mentality.hpp"
#include "other/vip/vip.hpp"
#include "other/shop/roleshop.hpp"
#include "other/capability/capability.hpp"
#include "other/chestshop/rolechestshop.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/fb/rolestoryfb.hpp"
#include "other/fb/rolemojiefb.hpp"
#include "other/convertshop/roleconvertshop.hpp"
#include "other/discountbuy/discountbuy.hpp"
#include "other/footprint/footprint.hpp"
#include "other/activedegree/activedegree.hpp"
#include "other/welfare/welfare.hpp"
#include "other/hangup/hangup.hpp"
#include "other/evaluate/roleevaluate.hpp"
#include "other/maze/mazemanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/cloak/cloakmanager.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/monitor/monitor.hpp"
#include "other/sublock/sublock.hpp"
#include "other/monthcard/monthcard.h"
#include "other/p2ptrade/traderegister.h"
#include "other/fb/rolefbzhanshendian.hpp"
#include "other/fb/rolepatafb.hpp"
#include "other/fb/rolevipfb.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/upgrade/upgrademanager.hpp"
#include "equipment/stonemananger.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/wabao/wabao.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "other/baby/baby.hpp"
#include "other/touzijihua/touzijihua.hpp"
#include "other/rollmoney/rollmoney.h"
#include "other/commonactivity/commonactivityunit.hpp"
#include "other/fb/roledailyfb.hpp"
#include "other/lieming/liemingmanager.h"
#include "other/xiannvshouhu/xiannvshouhumanager.hpp"
#include "other/jinglingfazhen/jinglingfazhenmanager.hpp"
#include "other/jinglingguanghuan/jinglingguanghuanmanager.hpp"
#include "other/cardzu/cardzu.hpp"
#include "other/zhuanshengequip/zhuanshengequip.hpp"
#include "other/mojie/mojiemanager.hpp"
#include "other/shenbing/shenbingmanager.hpp"
#include "other/equipsuit/equipsuit.hpp"
#include "other/shenzhuang/shenzhuang.h"
#include "other/zerogift/zerogift.hpp"
#include "task/other/tumotask.hpp"
#include "task/other/husongtask.hpp"
#include "task/other/datingtask.hpp"
#include "task/other/cishatask.hpp"
#include "task/other/guildtask.hpp"
#include "other/fb/rolefbequip.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/yaojiang/roleyaojiang.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "other/guildxianshu/guildxianshu.hpp"
#include "other/supply/rolesupply.hpp"
#include "other/fb/rolefbchallenge.hpp"
#include "other/fb/rolefbphase.hpp"
#include "other/turntable/turntable.h"
#include "other/jilian/jilian.hpp"
#include "global/worldevent/roleworldevent.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/cross/rolecross.h"
#include "other/shengwang/shengwang.h"
#include "other/chengjiu/chengjiu.h"
#include "other/lifeskill/lifeskill.hpp"
#include "task/other/paohuantask.hpp"
#include "other/camp/gongxun.h"
#include "other/tuhaojin/tuhaojin.hpp"
#include "other/bigchatface/bigchatface.hpp"
#include "other/pet/pet.hpp"
#include "other/multimount/multimount.hpp"
#include "other/personalizewindow/personalizewindow.hpp"
#include "other/xunzhang/xunzhangmanager.hpp"
#include "other/zhibao/zhibaomanager.hpp"
#include "other/huashen/huashen.hpp"
#include "other/magiccard/magiccard.hpp"
#include "other/mitama/mitama.hpp"
#include "other/crossroleglobal/crossroleglobal.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/friendgift/friendgiftmanager.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/followboss/followbossmanager.hpp"
#include "other/goldvip/goldvip.hpp"
#include "other/famousman/famousman.hpp"
#include "other/shengesystem/shengesystem.hpp"
#include "other/fb/rolepreciousboss.hpp"
#include "other/shenshou/shenshou.hpp"
#include "other/imgfuling/imgfuling.hpp"
#include "other/feixian/feixian.h"
#include "global/combineserveractivity/combineserverequip.hpp"
#include "other/shenyin/shenyin.hpp"
#include "elementheart/elementheart.hpp"
#include "shengxingzhuli/shengxingzhulimanager.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/challenge/rolechallengefield.hpp"
#include "other/jingjie/jingjie.hpp"
#include "other/talent/talent.hpp"
#include "other/jinjietalent/jinjietalent.hpp"
#include "other/fb/rolefbarmordefend.hpp"
#include "other/heshenluoshu/heshenluoshu.hpp"
#include "other/shengqi/shengqi.hpp"
#include "impguard/impguard.hpp"
#include "task/other/zhuanzhitask.hpp"
#include "bosscard/bosscard.hpp"
#include "rolezhuanzhi/rolezhuanzhi.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "other/seal/seal.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "servercommon/struct/jinjiesysrewardparam.hpp"
#include "rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "other/shenqi/shenqi.hpp"
#include "mojie/mojieskill.hpp"
#include "redequipcollect/redequipcollect.hpp"
#include "orangeequipcollect/orangeequipcollect.hpp"
#include "molong/molong.h"
#include "xianzunka/xianzunka.hpp"
#include "equipbaptize/equipbaptize.hpp"
#include "fb/rolepersonbossfb.hpp"
#include "other/greatesoldier/greatesoldier.hpp"
#include "other/lotterycount/lotterycount.hpp"
#include "other/rolecrossrandactivity/rolecrossrandactivity.hpp"
#include "other/upgradecardbuy/upgradecardbuy.hpp"
#include "huguozhili/huguozhili.hpp"
#include "other/crossgoal/crossgoal.hpp"
#include "other/baizhanequip/baizhanequip.h"
#include "other/littlehelper/littlehelper.h"
#include "other/crossequip/crossequip.hpp"
#include "other/myth/myth.hpp"
#include "other/zodiac/zodiac.hpp"

struct RoleModule
{
	void * operator new(size_t c);
	void operator delete(void *m);

	CommonDataParam common_data_param;							// 通用数据

	UpgradeManager upgrade_manager;								// 进阶系统管理器
	EquipmentManager equip_manager;								// 装备管理器
	FriendExpBottle friend_exp_bottle;							// 好友经验瓶
	JingLingManager jingling_mgr;								// 精灵管理器
	MagicalPrecious magical_precious_manager;					// 魔龙秘宝管理器
	MysteriousShopInMall mysterious_shop_in_mall;				// 神秘商店
	RoleGuild role_guild;										// 仙盟个人相关
	RoleFBTowerDefend towerdefend_fb;							// 塔防副本
	RuneSystem runesystem_manger;								// 符文系统管理器
	ShenzhouWeapon shenzhou_weapon_manger;						// 神州六器管理器
	XiannvManager xiannv_manager;								// 仙女管理器
	RoleTuituFb tuitu_fb;										// 推图副本

	HistoryCapParam history_cap_param;							// 历史战力数据
	Knapsack knapsack;											// 背包管理器
	NewTaskManager newtask_manager;								// 任务管理器
	TaskRecorder task_recorder;									// 历史任务记录器
	HotkeyRecorder hotkey_recorder;								// 快捷方式记录器
	FriendManager friend_manager;								// 好友管理器
	TeamInfo team_info;											// 组队信息
	RankShadow rank_shadow;										// 排行榜影子
	Mentality mentality;										// 元神系统
	Vip vip;													// VIP信息
	RoleChestShop role_chestshop;								// 宝箱信息
	RoleShop roleshop;											// 商城信息
	Capability capablity;										// 战斗力
	DayCounter daycounter;										// 每日计数器
	TitleManager title_manager;									// 称号管理器
	ActiveDegree active_degree;									// 活跃度
	Welfare welfare;											// 福利
	Hangup role_hang_up;										// 挂机设置
	MazeManager maze;											// 迷宫
	WingManager wing;											// 羽翼
	HaloManager halo;											// 光环
	ShengongManager shengong;									// 神弓
	ShenyiManager shenyi;										// 神翼
	FootPrint footprint;										// 足迹
	CloakManager cloak_manager;									// 披风
	RoleEvaluate role_evaluate;									// 评价
	RoleStatusManager role_status_manager;						// 状态管理
	Monitor monitor;											// 监控
	SubLock sublock;											// 二级锁
	MountManager mount_manager;									// 坐骑管理器
	FightMountManager fightmount_manager;						// 战斗坐骑管理器
	StoneManager stone_manager;									// 宝石管理器
	RoleActivity role_activity;									// 角色活动
	DailyTask daily_task;										// 屠魔任务	
	HusongTask husong_task;										// 护送任务
	RoleFBEquip equip_fb;										// 装备副本
	MojieManager mojie_manager;									// 魔戒管理器
	EquipSuit equip_suit;										// 锻造套装
	ShenZhuang shen_zhuang;										// 神装
	GuildTask guild_task;										// 仙盟任务
	RoleYaojiang yaojiang;										// 摇奖
	SpecialAppearance special_appearance;						// 特殊外观
	DatingTask dating_task;										// 约会任务
	GuildXianshu guild_xianshu;									// 仙盟仙术
	RoleSupply role_supply;										// 补给
	RoleStoryFB story_fb;										// 剧情副本
	RoleMojieFB mojie_fb;										// 魔戒副本
	RoleFBChallenge challenge_fb;								// 挑战副本
	RolePhaseFB role_phase_fb;									// 阶段副本
	RolePataFB patafb;											// 爬塔副本
	RoleVipFB vipfb;											// VIP副本
	RoleConvertRecord convert_record;							// 兑换商店兑换记录
	DiscountBuy discount_buy;									// 一折抢购
	ZeroGift zero_gift;											// 零元礼包
	MonthCard month_card;										// 月卡
	TurnTable turntable;										// 转盘
	Wabao wabao;												// 挖宝
	CishaTask cisha_task;										// 刺杀任务
	Jilian jilian;												// 祭炼
	Qingyuan qingyuan;											// 情缘
	Baby baby;													// 宝宝
	TradeRegister trade_register;								// P2P交易
	RoleZhanshenDian zhanshendian;								// 战神殿
	Touzijihua touzijihua;										// 投资计划
	RollMoney roll_money;										// 摇钱
	RoleWorldEvent role_world_event;							// 世界事件
	CommonActivityUnitManager common_activity_unit_mgr;			// 通用活动
	RoleGoal role_goal;											// 角色目标
	Shizhuang shizhuang;										// 时装
	RoleCross role_cross;										// 跨服模块
	ShengWang shengwang_mgr;									// 声望管理器
	ChengJiu chengjiu_mgr;										// 成就管理器
	RoleDailyFb dailyfb;										// 日常副本
	LifeSkillMgr lifeskill_mgr;									// 生活技能
	PaoHuanTask paohuan_task;									// 跑环任务
	GongXun gongxun_mgr;										// 功勋
	TuHaoJin tuhaojin;											// 土豪金
	BigChatFace bigchatface;									// 聊天大表情
	Pet pet;													// 宠物
	MultiMount multi_mount;										// 双人坐骑
	PersonalizeWindow personalize_window;						// 个性聊天框
	XunZhangManager xunzhang_manager;							// 勋章
	ZhiBaoManager zhibao_manager;								// 至宝
	ShenBingManager shenbing_manager;							// 神兵
	Cardzu cardzu;												// 卡牌组合
	LieMingManager lieming_mgr;									// 猎命
	HuaShen huashen;											// 化神系统
	MagicCard magic_card;										// 魔卡
	Mitama mitama;												// 御魂
	XiannvShouhuManager xiannvshouhu_manager;					// 仙女守护
	JinglingGuanghuanManager jinglingguanghuan_manager;			// 精灵光环
	JinglingFazhenManager jinglingfazhen_manager;				// 精灵法阵
	ZhuanShengEquip zhuansheng_equip;							// 转生
	CrossRoleGlobal cross_role_global;							// 用于跨服的global role对象，挂在人物身上
	ChineseZodiac chinese_zodiac;								// 生肖系统
	FriendGiftManager friend_gift_manager;						// 好友送礼
	GoldVip gold_vip;											// 黄金VIP
	FamousMan famous_man;										// 名人堂
	FollowBossManager followboss_manager;						// Boss关注
	ShengeSystem shenge_system;									// 神格系统
	RolePreciousBoss precious_boss;								// 秘藏boss
	Shenshou shenshou;											// 神兽
	ImgFuling img_fuling;										// 形象赋灵
	CombineServerActivityEquip csa_equip;						// 合服装备
	ShenYin shenyin;											// 神印系统
	ElemnetHeart element_heart;									// 元素之心
	ShengxingzhuliManager shengxing_zhuli;						// 升星助力
	UpgradeRush	upgrade_rush;									// 升阶冲刺
	FabaoManager fabao;                                         // 法宝

	FeixianManager feixian_manager;								// 飞仙系统
	RoleChallengeField role_challenge_field;					// 1v1竞技场

	JingJie jingjie;											// 境界

	Talent  talent;												// 天赋系统
	JinJieTalent jinjie_talent;									// 进阶天赋系统
	RoleFBArmorDefend role_am_fb;								// 防具材料副本
	HeShengLuoShu heshen_luoshu;								// 河神洛书
	Shengqi shengqi;											// 圣器系统	

	ImpGuard m_imp_guard;									    // 小鬼守护
	ZhuanzhiTask zhuanzhi_task;                                 // 转职任务
	RoleZhuanZhi role_zhuanzhi;                                 // 人物转职
	BossCard boss_card;                                         // Boss图鉴

	YaoShiManager yaoshi_manager;								// 腰饰
	TouShiManager toushi_manager;								// 头饰
	QiLinBiManager qilinbi_manager;								// 麒麟臂
	MaskManager mask_manager;									// 面具

	ZhuanzhiEquip zhuanzhi_equip;								// 转职装备
	Seal seal;													// 圣印系统
	DressingRoomMgr dressing_room_mgr;							// 衣柜
	JinjieSysRewardParam jinjiesys_reward_param;				// 进阶系统奖励
	RoleBigSmallGoal role_big_small_goal;						// 角色大小目标
	Shenqi shen_qi;												// 神器

	MojieSkill mojie_skill;										// 魔戒技能(与上面的魔戒管理器没有关系)

	RedEquipCollect redequip_collect;							// 红装收集
	OrangeEquipCollect orang_equip_collect;                     // 橙装收集

	Molong molong;                                              //魔龙

	XianZunKa xianzunka;                                        // 仙尊卡

	EquipBaptize equip_baptize;                                 // 装备洗炼

	RolePersonBossFB role_personboss_fb;                        // 个人Boss

	GreateSoldier m_greate_soldier;								// 名将 
	LotteryCount lottery_count_module;							// 抽奖次数保底

	RoleCrossRandActivity cross_rand_activity;					// 跨服随机活动 

	UpgradeCardBuy upgrade_card_buy;							// 直升丹购买
	Huguozhili huguozhili;										// 天神护体
	
	CrossGoal cross_goal;										// 跨服目标
	BaizhanEquip baizhan_equip;									// 百战装备
	LittleHelper little_helper;									// 小助手
	Myth myth;													// 神话系统
	Zodiac zodiac;												// 星象

	CrossEquip cross_equip;										// 斗气装备
};

#endif
