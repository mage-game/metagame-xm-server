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

	CommonDataParam common_data_param;							// ͨ������

	UpgradeManager upgrade_manager;								// ����ϵͳ������
	EquipmentManager equip_manager;								// װ��������
	FriendExpBottle friend_exp_bottle;							// ���Ѿ���ƿ
	JingLingManager jingling_mgr;								// ���������
	MagicalPrecious magical_precious_manager;					// ħ���ر�������
	MysteriousShopInMall mysterious_shop_in_mall;				// �����̵�
	RoleGuild role_guild;										// ���˸������
	RoleFBTowerDefend towerdefend_fb;							// ��������
	RuneSystem runesystem_manger;								// ����ϵͳ������
	ShenzhouWeapon shenzhou_weapon_manger;						// ��������������
	XiannvManager xiannv_manager;								// ��Ů������
	RoleTuituFb tuitu_fb;										// ��ͼ����

	HistoryCapParam history_cap_param;							// ��ʷս������
	Knapsack knapsack;											// ����������
	NewTaskManager newtask_manager;								// ���������
	TaskRecorder task_recorder;									// ��ʷ�����¼��
	HotkeyRecorder hotkey_recorder;								// ��ݷ�ʽ��¼��
	FriendManager friend_manager;								// ���ѹ�����
	TeamInfo team_info;											// �����Ϣ
	RankShadow rank_shadow;										// ���а�Ӱ��
	Mentality mentality;										// Ԫ��ϵͳ
	Vip vip;													// VIP��Ϣ
	RoleChestShop role_chestshop;								// ������Ϣ
	RoleShop roleshop;											// �̳���Ϣ
	Capability capablity;										// ս����
	DayCounter daycounter;										// ÿ�ռ�����
	TitleManager title_manager;									// �ƺŹ�����
	ActiveDegree active_degree;									// ��Ծ��
	Welfare welfare;											// ����
	Hangup role_hang_up;										// �һ�����
	MazeManager maze;											// �Թ�
	WingManager wing;											// ����
	HaloManager halo;											// �⻷
	ShengongManager shengong;									// ��
	ShenyiManager shenyi;										// ����
	FootPrint footprint;										// �㼣
	CloakManager cloak_manager;									// ����
	RoleEvaluate role_evaluate;									// ����
	RoleStatusManager role_status_manager;						// ״̬����
	Monitor monitor;											// ���
	SubLock sublock;											// ������
	MountManager mount_manager;									// ���������
	FightMountManager fightmount_manager;						// ս�����������
	StoneManager stone_manager;									// ��ʯ������
	RoleActivity role_activity;									// ��ɫ�
	DailyTask daily_task;										// ��ħ����	
	HusongTask husong_task;										// ��������
	RoleFBEquip equip_fb;										// װ������
	MojieManager mojie_manager;									// ħ�������
	EquipSuit equip_suit;										// ������װ
	ShenZhuang shen_zhuang;										// ��װ
	GuildTask guild_task;										// ��������
	RoleYaojiang yaojiang;										// ҡ��
	SpecialAppearance special_appearance;						// �������
	DatingTask dating_task;										// Լ������
	GuildXianshu guild_xianshu;									// ��������
	RoleSupply role_supply;										// ����
	RoleStoryFB story_fb;										// ���鸱��
	RoleMojieFB mojie_fb;										// ħ�丱��
	RoleFBChallenge challenge_fb;								// ��ս����
	RolePhaseFB role_phase_fb;									// �׶θ���
	RolePataFB patafb;											// ��������
	RoleVipFB vipfb;											// VIP����
	RoleConvertRecord convert_record;							// �һ��̵�һ���¼
	DiscountBuy discount_buy;									// һ������
	ZeroGift zero_gift;											// ��Ԫ���
	MonthCard month_card;										// �¿�
	TurnTable turntable;										// ת��
	Wabao wabao;												// �ڱ�
	CishaTask cisha_task;										// ��ɱ����
	Jilian jilian;												// ����
	Qingyuan qingyuan;											// ��Ե
	Baby baby;													// ����
	TradeRegister trade_register;								// P2P����
	RoleZhanshenDian zhanshendian;								// ս���
	Touzijihua touzijihua;										// Ͷ�ʼƻ�
	RollMoney roll_money;										// ҡǮ
	RoleWorldEvent role_world_event;							// �����¼�
	CommonActivityUnitManager common_activity_unit_mgr;			// ͨ�û
	RoleGoal role_goal;											// ��ɫĿ��
	Shizhuang shizhuang;										// ʱװ
	RoleCross role_cross;										// ���ģ��
	ShengWang shengwang_mgr;									// ����������
	ChengJiu chengjiu_mgr;										// �ɾ͹�����
	RoleDailyFb dailyfb;										// �ճ�����
	LifeSkillMgr lifeskill_mgr;									// �����
	PaoHuanTask paohuan_task;									// �ܻ�����
	GongXun gongxun_mgr;										// ��ѫ
	TuHaoJin tuhaojin;											// ������
	BigChatFace bigchatface;									// ��������
	Pet pet;													// ����
	MultiMount multi_mount;										// ˫������
	PersonalizeWindow personalize_window;						// ���������
	XunZhangManager xunzhang_manager;							// ѫ��
	ZhiBaoManager zhibao_manager;								// ����
	ShenBingManager shenbing_manager;							// ���
	Cardzu cardzu;												// �������
	LieMingManager lieming_mgr;									// ����
	HuaShen huashen;											// ����ϵͳ
	MagicCard magic_card;										// ħ��
	Mitama mitama;												// ����
	XiannvShouhuManager xiannvshouhu_manager;					// ��Ů�ػ�
	JinglingGuanghuanManager jinglingguanghuan_manager;			// ����⻷
	JinglingFazhenManager jinglingfazhen_manager;				// ���鷨��
	ZhuanShengEquip zhuansheng_equip;							// ת��
	CrossRoleGlobal cross_role_global;							// ���ڿ����global role���󣬹�����������
	ChineseZodiac chinese_zodiac;								// ��Фϵͳ
	FriendGiftManager friend_gift_manager;						// ��������
	GoldVip gold_vip;											// �ƽ�VIP
	FamousMan famous_man;										// ������
	FollowBossManager followboss_manager;						// Boss��ע
	ShengeSystem shenge_system;									// ���ϵͳ
	RolePreciousBoss precious_boss;								// �ز�boss
	Shenshou shenshou;											// ����
	ImgFuling img_fuling;										// ������
	CombineServerActivityEquip csa_equip;						// �Ϸ�װ��
	ShenYin shenyin;											// ��ӡϵͳ
	ElemnetHeart element_heart;									// Ԫ��֮��
	ShengxingzhuliManager shengxing_zhuli;						// ��������
	UpgradeRush	upgrade_rush;									// ���׳��
	FabaoManager fabao;                                         // ����

	FeixianManager feixian_manager;								// ����ϵͳ
	RoleChallengeField role_challenge_field;					// 1v1������

	JingJie jingjie;											// ����

	Talent  talent;												// �츳ϵͳ
	JinJieTalent jinjie_talent;									// �����츳ϵͳ
	RoleFBArmorDefend role_am_fb;								// ���߲��ϸ���
	HeShengLuoShu heshen_luoshu;								// ��������
	Shengqi shengqi;											// ʥ��ϵͳ	

	ImpGuard m_imp_guard;									    // С���ػ�
	ZhuanzhiTask zhuanzhi_task;                                 // תְ����
	RoleZhuanZhi role_zhuanzhi;                                 // ����תְ
	BossCard boss_card;                                         // Bossͼ��

	YaoShiManager yaoshi_manager;								// ����
	TouShiManager toushi_manager;								// ͷ��
	QiLinBiManager qilinbi_manager;								// �����
	MaskManager mask_manager;									// ���

	ZhuanzhiEquip zhuanzhi_equip;								// תְװ��
	Seal seal;													// ʥӡϵͳ
	DressingRoomMgr dressing_room_mgr;							// �¹�
	JinjieSysRewardParam jinjiesys_reward_param;				// ����ϵͳ����
	RoleBigSmallGoal role_big_small_goal;						// ��ɫ��СĿ��
	Shenqi shen_qi;												// ����

	MojieSkill mojie_skill;										// ħ�似��(�������ħ�������û�й�ϵ)

	RedEquipCollect redequip_collect;							// ��װ�ռ�
	OrangeEquipCollect orang_equip_collect;                     // ��װ�ռ�

	Molong molong;                                              //ħ��

	XianZunKa xianzunka;                                        // ����

	EquipBaptize equip_baptize;                                 // װ��ϴ��

	RolePersonBossFB role_personboss_fb;                        // ����Boss

	GreateSoldier m_greate_soldier;								// ���� 
	LotteryCount lottery_count_module;							// �齱��������

	RoleCrossRandActivity cross_rand_activity;					// ������� 

	UpgradeCardBuy upgrade_card_buy;							// ֱ��������
	Huguozhili huguozhili;										// ������
	
	CrossGoal cross_goal;										// ���Ŀ��
	BaizhanEquip baizhan_equip;									// ��սװ��
	LittleHelper little_helper;									// С����
	Myth myth;													// ��ϵͳ
	Zodiac zodiac;												// ����

	CrossEquip cross_equip;										// ����װ��
};

#endif
