#include "logicconfigmanager.hpp"

#include "servercommon/serverconfig/localconfig.hpp"
#include "config/rankconfigcheck.hpp"

#include "activedegreeconfig.hpp"
#include "other/chestshop/chestshop.hpp"
#include "other/commandspeaker/commandspeakerconfig.hpp"
#include "other/convertshop/convertshop.hpp"
#include "other/friendexpbottle/friendexpbottleconfig.hpp"
#include "guildconfig.hpp"
#include "guildactivityconfig.hpp"
#include "other/jingling/jinglingconfig.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "config/activityconfig/millionaireconfig.hpp"
#include "other/movechess/movechess.hpp"
#include "other/shop/mysteriousshopinmallconfig.hpp"
#include "config/activityconfig/questionconfig.hpp"
#include "config/activityconfig/qunxianluandouconfig.hpp"
#include "other/runesystem/runesystemconfig.hpp"
#include "other/shenzhouweapon/shenzhouweaponconfig.hpp"
#include "other/xiannv/xiannvconfig.hpp"
#include "config/activityconfig/yizhandaodiconfig.hpp"
#include "config/activityconfig/goldturntableconfig.hpp"
#include "gameworld/other/fb/fbtuituconfig.h"
#include "global/globalrole/fighting/fightingconfig.hpp"

#include "config/restconfig.hpp"
#include "config/mentalityconfig.hpp"
#include "other/footprint/footprintconfig.hpp"
#include "config/fbconfig.hpp"
#include "other/achieve/title/titleconfig.hpp"
#include "other/welfare/welfareconfig.hpp"
#include "other/welfare/sevendayloginconfig.hpp"
#include "other/shop/shop.hpp"
#include "other/shop/mysteriousshop.hpp"
#include "skill/bossskillconditionpool.hpp"
#include "config/activityconfig/monsterinvadeconfig.hpp"
#include "other/mount/mountopconfig.hpp"
#include "other/fightmount/fightmountconfig.hpp"
#include "other/fabao/fabaoconfig.hpp"
#include "other/maze/mazeconfig.hpp"
#include "other/wing/wingconfig.hpp"
#include "other/halo/haloconfig.hpp"
#include "other/shengong/shengongconfig.hpp"
#include "other/shenyi/shenyiconfig.hpp"
#include "other/cloak/cloakconfig.hpp"
#include "config/activityconfig/zhuxieconfig.hpp"
#include "other/discountbuy/discountbuyconfig.hpp"
#include "config/activityconfig/tianjiangcaibaoconfig.h"
#include "config/activityconfig/wanglingexploreconfig.h"
#include "config/taskconfig/cishataskconfig.hpp"
#include "config/taskconfig/husongconfig.hpp"
#include "other/fb/neqfbconfig.hpp"
#include "other/fb/storyfbconfig.hpp"
#include "other/fb/mojiefbconfig.hpp"
#include "other/mojie/mojieconfig.hpp"
#include "other/fb/dailyfbconfig.hpp"
#include "other/fb/vipfbconfig.hpp"
#include "other/fb/fbchallengeconfig.hpp"
#include "other/wabao/wabaoconfig.hpp"
#include "config/activityconfig/randactivityopencfg.hpp"
#include "config/zhuanzhiconfig.hpp"
#include "other/fb/migongxianfuconfig.hpp"
#include "other/lieming/liemingconfig.hpp"
#include "config/activityconfig/nationalbossconfig.hpp"
#include "config/activityconfig/qunxianluandouconfig.hpp"
#include "config/activityconfig/xianmengzhanconfig.hpp"
#include "config/activityconfig/multibattlefieldskillconfig.hpp"
#include "config/activityconfig/moshenjianglinconfig.hpp"
#include "config/activityconfig/guajiactivityconfig.hpp"
#include "config/activityconfig/shuijingconfig.hpp"
#include "global/worldboss/worldbossbaseconfig.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "other/fb/towerdefendconfig.hpp"
#include "config/activityconfig/gongchengzhanconfig.hpp"
#include "config/monsterlogicconfig.hpp"
#include "other/rollmoney/rollmoneyconfig.h"
#include "config/flycorrectionconfig.hpp"
#include "config/knapsackconfig.hpp"
#include "other/vip/vipconfig.hpp"
#include "other/fb/phasefbconfig.hpp"
#include "config/touzijihuaconfig.hpp"
#include "config/updatenoticeconfig.hpp"
#include "other/commonactivity/commonactivityconfig.hpp"
#include "other/shizhuang/shizhuangconfig.h"
#include "other/zerogift/zerogiftconfig.hpp"
#include "other/xiannvshouhu/xiannvshouhuconfig.hpp"
#include "other/jinglingfazhen/jinglingfazhenconfig.hpp"
#include "other/jinglingguanghuan/jinglingguanghuanconfig.hpp"
#include "other/fb/dailytaskfbcofig.hpp"
#include "config/guildtowerdefendconfig.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/yaojiang/yaojiangconfig.hpp"
#include "config/marryconfig.hpp"
#include "other/supply/supplyconfig.hpp"
#include "config/activityconfig/chongzhiconfig.hpp"
#include "globalconfig/composeconfig.h"
#include "globalconfig/namecreateconfig.hpp"
#include "equipment/suitbase.hpp"
#include "servercommon/serverconfig/datingcommonconfig.hpp"
#include "other/guildxianshu/guildxianshuconfig.hpp"
#include "config/activityconfig/opengameactivityconfig.hpp"
#include "skill/roleskillcondition.hpp"
#include "global/challengefield/challengefieldconfig.hpp"
#include "other/turntable/turntableconfig.h"
#include "other/fb/yaoshoujitanteamfbconfig.hpp"
#include "other/jilian/jilianconfig.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "other/baby/babyconfig.hpp"
#include "other/fb/zhanshendianconfig.hpp"
#include "config/activityconfig/closedbetaconfig.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "global/fishpool/fishpoolconfig.hpp"
#include "global/worldevent/worldeventconfig.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "global/roleboss/rolebossconfig.h"
#include "global/combineserveractivity/combineserveractivityconfig.hpp"
#include "config/activityconfig/guildfbconfig.hpp"
#include "config/activityconfig/wuxingguajiconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "other/fb/patafbconfig.hpp"
#include "config/crossrankweekrewardconfig.hpp"
#include "other/shengwang/shengwangconfig.hpp"
#include "other/chengjiu/chengjiuconfig.hpp"
#include "other/lifeskill/lifeskillconfig.hpp"
#include "config/activityconfig/yaoshouguangchangconfig.hpp"
#include "config/activityconfig/suoyaotaconfig.hpp"
#include "config/activityconfig/jinghuahusongconfig.hpp"
#include "config/activityconfig/huangchenghuizhanconfig.hpp"
#include "config/activityconfig/guildbonfireconfig.hpp"
#include "config/expbottleconfig.hpp"
#include "config/spaceconfig.hpp"
#include "config/monsterreplaceconfig.hpp"
#include "config/activityconfig/zhuaguiconfig.hpp"
#include "config/dropextendconfig.hpp"
#include "other/camp/campconfig.hpp"
#include "other/tuhaojin/tuhaojinconfig.hpp"
#include "other/bigchatface/bigchatfaceconfig.hpp"
#include "other/pet/petconfig.hpp"
#include "other/multimount/multimountconfig.hpp"
#include "other/personalizewindow/personalizewindowconfig.hpp"
#include "other/cardzu/cardzuconfig.hpp"
#include "globalconfig/otherconfig.h"
#include "config/activityconfig/hotspringconfig.hpp"
#include "other/xunzhang/xunzhangconfig.hpp"
#include "other/zhibao/zhibaoconfig.hpp"
#include "other/shenbing/shenbingconfig.hpp"
#include "config/activityconfig/guildbattleconfig.hpp"
#include "config/activityconfig/guildbattleconfignew.hpp"
#include "config/activityconfig/territorywarconfig.hpp"
#include "config/crossteamfbconfig.hpp"
#include "other/huashen/huashenconfig.hpp"
#include "other/magiccard/magiccardconfig.hpp"
#include "other/mitama/mitamaconfig.hpp"
#include "other/chinesezodiac/chinesezodiacconfig.hpp"
#include "other/zhuanshengequip/zhuanshengequipconfig.hpp"
#include "global/globalrole/litttlepet/littlepetconfig.hpp"
#include "global/globalrole/lovetree/lovetree.hpp"
#include "other/fb/funopenfbconfig.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"
#include "other/upgraderush/upgraderushconfig.hpp"
#include "other/friendgift/friendgiftconfig.hpp"
#include "other/equipsuit/equipsuitconfig.hpp"
#include "other/goldvip/goldvipconfig.hpp"
#include "config/fbsceneconfig.hpp"
#include "other/fb/fbguideconfig.hpp"
#include "other/famousman/famousmanconfig.hpp"
#include "other/runesystem/runetowerconfig.hpp"
#include "other/teamequipfb/teamequipfbconfig.hpp"
#include "other/shengesystem/shengesystemconfig.hpp"
#include "config/activityconfig/xingzuoyijiconfig.hpp"
#include "other/advancenotice/advancenoticeconfig.hpp"
#include "config/qingyuanshengdicconfig.h"
#include "other/jinglingadvantage/jinglingadvantageconfig.hpp"
#include "other/shenzhuang/shenzhuangconfig.hpp"
#include "other/shenshou/shenshouconfig.hpp"
#include "other/imgfuling/imgfulingconfig.hpp"
#include "global/combineserveractivity/combineserverequipconfig.hpp"
#include "global/combineserveractivity/combineserveractivitybossconfig.hpp"
#include "other/shenyin/shenyinconfig.hpp"
#include "other/elementheart/elementheartconfig.hpp"
#include "other/shengxingzhuli/shengxingzhuliconfig.hpp"
#include "other/feixian/reallyequipconfig.hpp"
#include "other/jingjie/jingjieconfig.hpp"
#include "config/activityconfig/fairybuddhacardconfig.hpp"
#include "other/talent/talentconfig.hpp"
#include "config/activityconfig/babybossconfig.hpp"
#include "other/jinjietalent/jinjietalentconfig.hpp"
#include "other/fb/armordefendconfig.hpp"
#include "other/fb/buildtowerfbconfig.hpp"
#include "config/dynamiccapadjustconfig.hpp"
#include "other/yaoshi/yaoshiconfig.hpp"
#include "other/toushi/toushiconfig.hpp"
#include "other/qilinbi/qilinbiconfig.hpp"
#include "other/mask/maskconfig.hpp"
#include "other/zhuanzhiequip/zhuanzhiequipconfig.hpp"
#include "other/fb/nightfight/nightfightfbconfig.hpp"
#include "config/activityconfig/guildquestionconfig.hpp"
#include "config/upgradeconfig/upgradeconfigmanager.hpp"
#include "other/heshenluoshu/heshenluoshuconfig.hpp"
#include "other/shengqi/shengqiconfig.hpp"
#include "global/bossshanggu/shanggubossconfig.hpp"
#include "other/impguard/impguardconfig.hpp"
#include "personbossconfig.hpp"
#include "other/bosscard/bosscardconfig.hpp"
#include "other/fb/messbattle/messbattleconfig.hpp"
#include "other/seal/sealconfig.hpp"
#include "other/fb/towerdefendteamconfig.hpp"
#include "other/fb/fbequipconfig.hpp"
#include "config/monster_hurt_hp_config.h"
#include "global/luckylog/luckylogconfig.hpp"
#include "config/dynamicadjustconfig.hpp"
#include "other/fb/holidayguardconfig.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/jinjiesysreward/jinjiesysrewardconfig.hpp"
#include "rolebigsmallgoalconfig.hpp"
#include "other/shenqi/shenqiconfig.hpp"
#include "other/mojie/mojieskillconfig.hpp"
#include "shenbingtuluconfig.h"
#include "other/worldlevelexpadd/worldlevelexpaddconfig.hpp"
#include "other/upgradesyscrit/upgradesyscritconfig.hpp"
#include "other/molong/molongconfig.h"
#include "other/xianzunka/xianzunkaconfig.hpp"
#include "gameworld/config/activitymsgconfig.hpp"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "global/globalrole/spousehome/spousehomeconfig.hpp"
#include "giftreplaceconfig.hpp"
#include "other/equipbaptize/equipbaptizeconfig.hpp"
#include "other/greatesoldier/greatesoldierconfig.hpp"
#include "config/lotterycountconfig.hpp"
#include "gameworld/config/agentadapterconfig.hpp"
#include "gameworld/config/activityconfig/giftharvestconfig.hpp"
#include "config/fallingmsgconfig.hpp"
#include "activityconfig/crossrandactivityconfig.hpp"
#include "other/upgradecardbuy/upgradecardbuyconfig.hpp"
#include "config/activityconfig/godmagicbossconfig.hpp"
#include "config/activityconfig/activityswitchconfig.hpp"
#include "other/huguozhili/huguozhiliconfig.hpp"
#include "other/todaythemereward/todaythemerewardconfig.hpp"
#include "other/crossgoal/crossgoalconfig.hpp"
#include "other/baizhanequip/baizhanequipconfig.hpp"
#include "other/tianshuxunzhu/tianshuxunzhuconfig.hpp"
#include "other/crossequip/crossequipconfig.hpp"
#include "config/littlehelperconfig.h"
#include "friend/friendconfig.hpp"
#include "config/worshipconfig.hpp"
#include "config/monsterextradropconfig.hpp"
#include "other/myth/mythconfig.hpp"
#include "other/zodiac/zodiacconfig.hpp"
#include "other/tianshenhuti/tianshenhuticonfig.hpp"

#define CONFIG_MODULE_DEF_CPP(class_name, file_name, is_init) \
{ \
	m_##class_name = new class_name(); \
	if (m_module_index >= MAX_CONFIG_MODULE_COUNT) \
	{ \
		*err = "config module count too small.\n"; \
		return false; \
	} \
	m_module_list[m_module_index++] = m_##class_name; \
	if (is_init && !m_##class_name->Init(path + file_name, err)) return false; \
}

LogicConfigManager::LogicConfigManager() : m_module_index(0)
{
	memset(m_module_list, 0, sizeof(m_module_list));
}

LogicConfigManager::~LogicConfigManager()
{
	for (int i = 0; i < m_module_index && i < MAX_CONFIG_MODULE_COUNT; i++)
	{
		delete m_module_list[i];
	}
}

LogicConfigManager *gs_logicconfig_manager = NULL;

LogicConfigManager * LogicConfigManager::Instance()
{
	if (NULL == gs_logicconfig_manager)
	{
		gs_logicconfig_manager = new LogicConfigManager();
	}

	return gs_logicconfig_manager;
}

bool LogicConfigManager::Init(const std::string &path, std::string *err)
{
	// ���а�������֤
	RankConfigCheck temp_rank_cfg;
	if (!temp_rank_cfg.Init(path + "logicconfig/rankconfig.xml", err))
	{
		return false;
	}

	// �������ù���
	CONFIG_MODULE_DEF_CPP(UpgradeconfigManager, "logicconfig/", true);

	// ��Ծ������
	CONFIG_MODULE_DEF_CPP(ActiveDegreeConfig, "logicconfig/activedegree.xml", true);
	// Ѱ���̵�����
	CONFIG_MODULE_DEF_CPP(ChestShop, "logicconfig/chestshop.xml", true);
	// ������������
	CONFIG_MODULE_DEF_CPP(CommandSpeakerConfig, "logicconfig/commandspeaker.xml", true);
	// �һ��̵�����
	CONFIG_MODULE_DEF_CPP(ConvertShop, "logicconfig/convertshop.xml", true);
	// ������ϵ������
	CONFIG_MODULE_DEF_CPP(FightingConfig, "logicconfig/fighting_cfg.xml", true);
	// ���Ѿ���ƿ
	CONFIG_MODULE_DEF_CPP(FriendExpBottleConfig, "logicconfig/friend_expbottle_config.xml", true);
	// Ԫ��ת������
	CONFIG_MODULE_DEF_CPP(GoldTurntableConfig, "logicconfig/zhuanpan.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(GuildConfig, "logicconfig/guildconfig.xml", true);
	// ���Ż����
	CONFIG_MODULE_DEF_CPP(GuildActivityConfig, "logicconfig/guild_active.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(JingLingConfig, "logicconfig/jingling.xml", true);
	// ħ���ر�����
	CONFIG_MODULE_DEF_CPP(MagicalPreciousCfg, "logicconfig/magicalprecious.xml", true);
	// �󸻺�����
	CONFIG_MODULE_DEF_CPP(MillionaireConfig, "logicconfig/millionaire.xml", true);
	// ����������
	CONFIG_MODULE_DEF_CPP(MoveChess, "logicconfig/movechess.xml", true);
	// �����̵�
	CONFIG_MODULE_DEF_CPP(MysteriousShopInMallConfig, "logicconfig/mysterious_shop_in_mall.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(QuestionConfig, "logicconfig/question.xml", true);
	// Ⱥ���Ҷ�����
	CONFIG_MODULE_DEF_CPP(QunXianLuanDouConfig, "logicconfig/qunxianlundouconfig.xml", true);
	// ����ϵͳ����
	CONFIG_MODULE_DEF_CPP(RuneSystemCfg, "logicconfig/rune_system_cfg.xml", true);
	// ��������ϵͳ����
	CONFIG_MODULE_DEF_CPP(ShenzhouWeaponConfig, "logicconfig/shenzhou_weapon.xml", true);
	// ��Ů����
	CONFIG_MODULE_DEF_CPP(XiannvConfig, "logicconfig/xiannvconfig.xml", true);
	// һս��������
	CONFIG_MODULE_DEF_CPP(YiZhanDaoDiConfig, "logicconfig/yizhandaodiconfig.xml", true);
	// ��ͼ����
	CONFIG_MODULE_DEF_CPP(TuituFbConfig, "logicconfig/tuitu_fb.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(RestConfig, "logicconfig/rest.xml", true);
	// Ԫ������
	CONFIG_MODULE_DEF_CPP(MentalityConfig, "logicconfig/mentality.xml", true);
	// �㼣����
	CONFIG_MODULE_DEF_CPP(FootPrintConfig, "logicconfig/footprint.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(CloakConfig, "logicconfig/cloak.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(FBConfig, "logicconfig/fbconfig.xml", true);
	// �ƺ�����
	CONFIG_MODULE_DEF_CPP(TitlePool, "logicconfig/titleconfig.xml", true);
	// �������� Ҫ�ڸ�������֮��
	CONFIG_MODULE_DEF_CPP(WelfareConfig, "logicconfig/welfare.xml", true);
	// �����¼����
	CONFIG_MODULE_DEF_CPP(SevenDayLoginConfig, "logicconfig/sevendaylogincfg.xml", true);
	// �̳�����
	CONFIG_MODULE_DEF_CPP(Shop, "logicconfig/shop.xml", true);
	// �����̵�
	CONFIG_MODULE_DEF_CPP(Mysteriousshop, "logicconfig/mysteriousshop.xml", true);
	// �ϳ�����
	CONFIG_MODULE_DEF_CPP(ComposeConfig, "logicconfig/compose.xml", true);
	// �Թ�����
	CONFIG_MODULE_DEF_CPP(MazeConfig, "logicconfig/mazeconfig.xml", false);
	// boss������������
	CONFIG_MODULE_DEF_CPP(BossSkillConditionPool, "", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(MountOpConfig, "logicconfig/mount.xml", true);
	// ս����������
	CONFIG_MODULE_DEF_CPP(FightMountOpConfig, "logicconfig/fight_mount.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(FabaoConfig, "logicconfig/fabao.xml", true);
	// ��аս������
	CONFIG_MODULE_DEF_CPP(ZhuXieConfig, "logicconfig/activityzhuxie.xml", true);
	// �콵Ԫ������
	CONFIG_MODULE_DEF_CPP(TianJiangCaiBaoConfig, "logicconfig/activitytianjiangcaibao.xml", true);
	// ����̽��
	CONFIG_MODULE_DEF_CPP(WangLingExploreConfig, "logicconfig/activitywanglingexplore.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(HuSongConfig, "logicconfig/husongcfg.xml", true);
	// ��װ��������
	CONFIG_MODULE_DEF_CPP(NeqFBConfig, "logicconfig/newequipfb.xml", true);
	// �׶θ�������
	CONFIG_MODULE_DEF_CPP(PhaseFBConfig, "logicconfig/phasefb.xml", true);
	// ���鸱������
	CONFIG_MODULE_DEF_CPP(StoryFBConfig, "logicconfig/storyfbconfig.xml", true);
	// ħ�丱������
	CONFIG_MODULE_DEF_CPP(MoJieFbConfig, "logicconfig/mojiefbconfig.xml", true);
	// ħ������
	CONFIG_MODULE_DEF_CPP(MojieConfig, "logicconfig/mojieconfig.xml", true);
	// ս�������
	CONFIG_MODULE_DEF_CPP(ZhanShenDianConfig, "logicconfig/zhanshendianconfig.xml", false);
	// ����ս
	CONFIG_MODULE_DEF_CPP(XianMengZhanConfig, "logicconfig/xianmengzhan.xml", true);
	// �¹�������
	CONFIG_MODULE_DEF_CPP(GuildBattleConfigNew, "logicconfig/guildbattle_new.xml", true);
	// ����ս
	CONFIG_MODULE_DEF_CPP(TerritoryWarConfig, "logicconfig/territorywar.xml", true);
	// ����ս������
	CONFIG_MODULE_DEF_CPP(MultiBattleFieldSkillConfig, "logicconfig/multibattlefieldskill.xml", true);
	//����boss
	CONFIG_MODULE_DEF_CPP(WorldBossBaseConfig, "logicconfig/worldboss.xml", true);
	//boss֮��
	CONFIG_MODULE_DEF_CPP(BossFamilyConfig, "logicconfig/bossfamily.xml", true);
	// ����
	CONFIG_MODULE_DEF_CPP(TowerDefendConfig, "logicconfig/towerdefendteam.xml", true);
	// ȫ��boss
	CONFIG_MODULE_DEF_CPP(NationalBossConfig, "logicconfig/nationalboss.xml", true);
	// VIP
	CONFIG_MODULE_DEF_CPP(VipConfig, "logicconfig/vip.xml", true);
	// monster logic
	CONFIG_MODULE_DEF_CPP(MonsterSpecialLogicConfig, "logicconfig/monsterlogicconfig.xml", true);
	// װ����װ����
	CONFIG_MODULE_DEF_CPP(EquipOtherConfig, "logicconfig/equipsuit.xml", true);
	// ����ս
	CONFIG_MODULE_DEF_CPP(GongChengZhanConfig, "logicconfig/gongchengzhan.xml", true);
	// С��Ь����
	CONFIG_MODULE_DEF_CPP(FlyCorrectionConfig, "logicconfig/flycorrection.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(KnapsackConfig, "logicconfig/knapsack.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(WingConfig, "logicconfig/wing.xml", true);
	// �⻷����
	CONFIG_MODULE_DEF_CPP(HaloConfig, "logicconfig/halo.xml", true);
	// ������
	CONFIG_MODULE_DEF_CPP(ShengongConfig, "logicconfig/shengong.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(ShenyiConfig, "logicconfig/shenyi.xml", true);
	// ��Ե����
	CONFIG_MODULE_DEF_CPP(QingyuanConfig, "logicconfig/qingyuanconfig.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(BabyConfig, "logicconfig/baby_cfg.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(MonsterInvadeConfig, "logicconfig/monsterinvade.xml", true);
	// ��ֵ����
	if (LocalConfig::Instance().IsIOSPlatForm())
	{
		CONFIG_MODULE_DEF_CPP(ChongzhiConfig, "logicconfig/chongzhirewardappstore.xml", true);
	}
	else
	{
		CONFIG_MODULE_DEF_CPP(ChongzhiConfig, "logicconfig/chongzhireward.xml", true);
	}
	// ������ʾ����
	CONFIG_MODULE_DEF_CPP(FbSceneCfg, "logicconfig/fb_scene_config.xml", true);
	// ҡ������
	CONFIG_MODULE_DEF_CPP(YaojiangConfig, "logicconfig/yaojiangconfig.xml", true);
	// �������
	CONFIG_MODULE_DEF_CPP(MarryConfig, "logicconfig/marry.xml", true);
	// Լ��
	CONFIG_MODULE_DEF_CPP(DatingCommonConfig, "logicconfig/dating.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(GuildXianshuConfig, "logicconfig/guild_xianshu.xml", false);
	// ��������
	CONFIG_MODULE_DEF_CPP(SupplyConfig, "logicconfig/supplyconfig.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(JoinLimitConfig, "logicconfig/joinlimitconfig.xml", true);
	// ��ս��������
	CONFIG_MODULE_DEF_CPP(ChallengeFBConfig, "logicconfig/challengefbcfg.xml", true);
	// ���������
	CONFIG_MODULE_DEF_CPP(OpenGameActivityConfig, "logicconfig/opengameactivity.xml", true);
	// һ������
	CONFIG_MODULE_DEF_CPP(DiscountBuyConfig, "logicconfig/discountbuycfg.xml", true);
	// ��Ԫ���
	CONFIG_MODULE_DEF_CPP(ZeroGifgConfig, "logicconfig/zerogift.xml", true);
	// ��ɫӰ�Ӽ����ͷ�����
	CONFIG_MODULE_DEF_CPP(RoleSkillCondition, "logicconfig/role_shadow_skill_cond.xml", true);
	// ������1v1����
	CONFIG_MODULE_DEF_CPP(ChallengeFieldConfig, "logicconfig/challengefield.xml", true);
	// �ڱ�����
	CONFIG_MODULE_DEF_CPP(WabaoConfig, "logicconfig/wabaoconfig.xml", true);
	// ���޼�̳����
	CONFIG_MODULE_DEF_CPP(YaoShouJiTanTeamFbConfig, "logicconfig/yaoshoujitanteamfbconfig.xml", true);
	// �Թ��ɷ�����
	CONFIG_MODULE_DEF_CPP(MiGongXianFuTeamFbConfig, "logicconfig/migongxianfuteamfbconfig.xml", true);
	//ת������
	CONFIG_MODULE_DEF_CPP(TurnTableConfig, "logicconfig/turntable.xml", true);
	// ��ɱ��������
	CONFIG_MODULE_DEF_CPP(CishaTaskConfig, "logicconfig/cisha_task.xml", false);
	// ��������
	CONFIG_MODULE_DEF_CPP(JilianConfig, "logicconfig/jilianxiandan.xml", false);
	// �������
	CONFIG_MODULE_DEF_CPP(ClosedBetaConfig, "logicconfig/closedbetaconfig.xml", true);
	{
		//��������
		char rand_cfg_path[256] = { 0 };
		SNPRINTF(rand_cfg_path, sizeof(rand_cfg_path), "logicconfig/%s.xml", LocalConfig::Instance().GetRandCfgName());
		CONFIG_MODULE_DEF_CPP(RandActivityConfig, rand_cfg_path, true);
		CONFIG_MODULE_DEF_CPP(RandActivityOpenCfg, "logicconfig/randactivityopencfg.xml", true);
	}
	// ���¹�������
	CONFIG_MODULE_DEF_CPP(UpdateNoticeConfig, "logicconfig/updatenotice.xml", true);
	// Ͷ�ʼƻ�
	CONFIG_MODULE_DEF_CPP(TouzijihuaConfig, "logicconfig/touzijihua.xml", true);
	// ת������
	CONFIG_MODULE_DEF_CPP(RollMoneyConfig, "logicconfig/rollmoneyconfig.xml", true);
	// ��ز���
	CONFIG_MODULE_DEF_CPP(FishPoolConfig, "logicconfig/lingchibuyu.xml", true);
	// �����¼�
	CONFIG_MODULE_DEF_CPP(WorldEventConfig, "logicconfig/worldeventconfig.xml", true);
	// ͨ�û
	CONFIG_MODULE_DEF_CPP(CommonActivityConfig, "logicconfig/commonactivitycfg.xml", true);
	// ��ɫĿ��
	CONFIG_MODULE_DEF_CPP(RoleGoalConfig, "logicconfig/rolegoalconfig.xml", true);
	// ħ����
	CONFIG_MODULE_DEF_CPP(MoShenJiangLinConfig, "logicconfig/moshenjianglinconfig.xml", false);
	// �һ��
	CONFIG_MODULE_DEF_CPP(GuaJiActivityConfig, "logicconfig/guajiactivityconfig.xml", true);
	// ���˸���
	CONFIG_MODULE_DEF_CPP(GuildFbConfig, "logicconfig/guildfb.xml", true);
	// ���йһ�
	CONFIG_MODULE_DEF_CPP(WuXingGuaJiConfig, "logicconfig/wuxingguajiconfig.xml", false);
	// ˮ���þ�
	CONFIG_MODULE_DEF_CPP(ShuiJingConfig, "logicconfig/activityshuijing.xml", true);
	// ʱװ
	CONFIG_MODULE_DEF_CPP(ShizhuangConfig, "logicconfig/shizhuangcfg.xml", true);
	// תְ
	CONFIG_MODULE_DEF_CPP(ZhuanzhiConfig, "logicconfig/zhuanzhicfg.xml", true);
	// Ӱ��bossˢ��
	CONFIG_MODULE_DEF_CPP(RoleBossConfig, "logicconfig/roleshadow_boss.xml", false);
	// ���Ե�
	CONFIG_MODULE_DEF_CPP(ShuxingdanConfig, "logicconfig/shuxingdan_cfg.xml", true);
	{
		// �Ϸ��
		if (LocalConfig::Instance().IsIOSPlatForm())
		{
			CONFIG_MODULE_DEF_CPP(CombineServerActivityConfig, "logicconfig/combineserveractivity_ios.xml", true);
		}
		else
		{
			CONFIG_MODULE_DEF_CPP(CombineServerActivityConfig, "logicconfig/combineserveractivity.xml", true);
		}
	}
	// ��������ܽ�������
	CONFIG_MODULE_DEF_CPP(CrossRankWeekRewardConfig, "logicconfig/cross_rank_week_reward.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(ShengWangConfig, "logicconfig/shengwang.xml", true);
	// �ɾ�����
	CONFIG_MODULE_DEF_CPP(ChengJiuConfig, "logicconfig/chengjiu.xml", true);
	// ������������
	CONFIG_MODULE_DEF_CPP(PataFbConfig, "logicconfig/patafbconfig.xml", true);
	// �ճ���������
	CONFIG_MODULE_DEF_CPP(DailyFBConfig, "logicconfig/dailyfbconfig.xml", true);
	// VIP��������
	CONFIG_MODULE_DEF_CPP(VipFbConfig, "logicconfig/vipfbconfig.xml", true);
	// ���޹㳡����
	CONFIG_MODULE_DEF_CPP(YaoShouGuangChangConfig, "logicconfig/yaoshouguangchang.xml", true);
	// ����������
	CONFIG_MODULE_DEF_CPP(SuoYaoTaConfig, "logicconfig/suoyaota.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(GuildBonfireConfig, "logicconfig/guildbonfire.xml", true);
	// ����ƿ
	CONFIG_MODULE_DEF_CPP(ExpBottleConfig, "logicconfig/expbottle.xml", true);
	// �ռ�����
	CONFIG_MODULE_DEF_CPP(SpaceConfig, "logicconfig/spaceconfig.xml", true);
	// ���������
	CONFIG_MODULE_DEF_CPP(LifeSkillConfig, "logicconfig/lifeskill.xml", false);
	// �����滻����
	CONFIG_MODULE_DEF_CPP(MonsterReplaceConfig, "logicconfig/replacemonsterconfig.xml", true);
	// ץ������
	CONFIG_MODULE_DEF_CPP(ZhuaGuiConfig, "logicconfig/zhuagui.xml", false);
	// ������չ����
	CONFIG_MODULE_DEF_CPP(DropExtendConfig, "logicconfig/dropextendconfig.xml", true);
	// ������������
	CONFIG_MODULE_DEF_CPP(JingHuaHusongConfig, "logicconfig/jinghuahusong.xml", true);
	// �ʳǻ�ս����
	CONFIG_MODULE_DEF_CPP(HuangchenghuizhanConfig, "logicconfig/huangchenghuizhancfg.xml", true);
	// ��Ӫ�������
	CONFIG_MODULE_DEF_CPP(CampConfig, "logicconfig/campconfg.xml", false);
	// ����������
	CONFIG_MODULE_DEF_CPP(TuHaoJinConfig, "logicconfig/tuhaojin.xml", true);
	// ������������
	CONFIG_MODULE_DEF_CPP(BigChatFaceConfig, "logicconfig/bigchatface.xml", true);
	// ����ϵͳ����(����)
	CONFIG_MODULE_DEF_CPP(PetConfig, "logicconfig/pet_cfg.xml", false);
	// ˫����������
	CONFIG_MODULE_DEF_CPP(MultiMountConfig, "logicconfig/multi_mount.xml", true);
	// ���Ի���������
	CONFIG_MODULE_DEF_CPP(PersonalizeWindowConfig, "logicconfig/personalize_window.xml", true);
	// �����������
	CONFIG_MODULE_DEF_CPP(CardzuConfig, "logicconfig/cardzu_config.xml", true);
	// ������������
	CONFIG_MODULE_DEF_CPP(OtherConfig, "logicconfig/other_config.xml", true);
	// ��Ȫ����
	CONFIG_MODULE_DEF_CPP(HotspringConfig, "logicconfig/hotspring.xml", true);
	// ѫ������
	CONFIG_MODULE_DEF_CPP(XunZhangConfig, "logicconfig/xunzhangconfig.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(ZhiBaoConfig, "logicconfig/zhibaoconfig.xml", true);
	// �������
	CONFIG_MODULE_DEF_CPP(ShenBingConfig, "logicconfig/shenbingconfig.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(LieMingConfig, "logicconfig/lieming.xml", true);
	// �����������
	CONFIG_MODULE_DEF_CPP(CrossTeamFBConfig, "logicconfig/cross_teamfb.xml", true);
	// ���װ����������
	CONFIG_MODULE_DEF_CPP(TeamEquipFBConfig, "logicconfig/team_equip_fb.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(HuaShenConfig, "logicconfig/huashen_cfg.xml", true);
	// ħ������
	CONFIG_MODULE_DEF_CPP(MagicCardConfig, "logicconfig/magic_card.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(MitamaConfig, "logicconfig/mitama.xml", true);
	// ��Ů�ػ�����
	CONFIG_MODULE_DEF_CPP(XiannvShouhuConfig, "logicconfig/xiannv_shouhu.xml", true);
	// ���鷨������
	CONFIG_MODULE_DEF_CPP(JinglingFazhenConfig, "logicconfig/jingling_fazhen.xml", true);
	// ����⻷����
	CONFIG_MODULE_DEF_CPP(JinglingGuanghuanConfig, "logicconfig/jingling_guanghuan.xml", true);
	// ���ܿ�����������
	CONFIG_MODULE_DEF_CPP(FunOpenFBConfig, "logicconfig/funopenfbconfig.xml", false);
	// �ճ����񸱱�����
	CONFIG_MODULE_DEF_CPP(DailyTaskFBConfig, "logicconfig/dailytaskfbconfig.xml", true);
	// ת������
	CONFIG_MODULE_DEF_CPP(ZhuanShengEquipConfig, "logicconfig/zhuansheng_cfg.xml", false);
	//С��������
	CONFIG_MODULE_DEF_CPP(LittlePetCfg, "logicconfig/littlepet.xml", true);
	// ��˼������
	CONFIG_MODULE_DEF_CPP(LoveTreeCfg, "logicconfig/lovetreeconfig.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(ChineseZodiacConfig, "logicconfig/chinese_zodiac_cfg.xml", true);
	// �����ż�����
	CONFIG_MODULE_DEF_CPP(XingzuoYiJiConfig, "logicconfig/xingzuoyijiconfig.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(FriendGiftConfig, "logicconfig/friendgift.xml", true);
	// ������װ����
	CONFIG_MODULE_DEF_CPP(EquipSuitConfig, "logicconfig/duanzaosuit.xml", true);
	// �ƽ��Ա
	CONFIG_MODULE_DEF_CPP(GoldVipConfig, "logicconfig/goldvip.xml", true);
	// ����ϵͳ������
	CONFIG_MODULE_DEF_CPP(UpGradeSkillConfig, "logicconfig/upgradeskill.xml", true);
	// ����ϵͳ���ײ���
	CONFIG_MODULE_DEF_CPP(UpgradeRushConfig, "logicconfig/upgraderush.xml", true);
	// ������
	CONFIG_MODULE_DEF_CPP(FamousManConfig, "logicconfig/famousman.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(FbGuideConfig, "logicconfig/fb_guide.xml", true);
	// ������
	CONFIG_MODULE_DEF_CPP(RuneTowerConfig, "logicconfig/rune_tower_cfg.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(NameCreateConfig, "randname/randname.xml", true);
	// ���
	CONFIG_MODULE_DEF_CPP(ShengeSystemCfg, "logicconfig/shenge_system_cfg.xml", true);
	// Ԥ������
	CONFIG_MODULE_DEF_CPP(AdvanceNoticeConfig, "logicconfig/notice.xml", true);
	// ��Եʥ��
	CONFIG_MODULE_DEF_CPP(QingYuanShengDiConfig, "logicconfig/qingyuanshengdiconfig.xml", true);
	// ������������
	CONFIG_MODULE_DEF_CPP(JinglingAdvantageConfig, "logicconfig/jingling_advantage_cfg.xml", true);
	// ��װ
	CONFIG_MODULE_DEF_CPP(ShenZhuangConfig, "logicconfig/shenzhuangcfg.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(ShenshouConfig, "logicconfig/shenshou_cfg.xml", true);
	// ����������
	CONFIG_MODULE_DEF_CPP(ImgFulingConfig, "logicconfig/img_fuling_cfg.xml", true);
	// �Ϸ�װ������
	CONFIG_MODULE_DEF_CPP(CombineServerActivityEquipConfig, "logicconfig/combine_server_equip_cfg.xml", true);
	// �Ϸ�boss����
	CONFIG_MODULE_DEF_CPP(CombineServerActivityBossConfig, "logicconfig/combine_server_boss_cfg.xml", true);
	// ��ӡϵͳ����
	CONFIG_MODULE_DEF_CPP(ShenYinConfig, "logicconfig/tian_xiang_cfg.xml", true);
	// Ԫ��֮������
	CONFIG_MODULE_DEF_CPP(ElementHeartConfig, "logicconfig/element_heart_cfg.xml", true);
	// ������������
	CONFIG_MODULE_DEF_CPP(ShengxingzhuliConfig, "logicconfig/shengxingzhuli_config.xml", true);
	// ���װ������
	CONFIG_MODULE_DEF_CPP(ReallyequipComposeConfig, "logicconfig/feixianequip.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(JingJieConfig, "logicconfig/rolejingjie.xml", true);
	// �������� 
	CONFIG_MODULE_DEF_CPP(FairyBuddhaCardConfig, "logicconfig/fairybuddhacardcfg.xml",false);
	// ����boss
	CONFIG_MODULE_DEF_CPP(BabyBossConfig, "logicconfig/baby_boss_config.xml", true);
	// ��̬ս������
	CONFIG_MODULE_DEF_CPP(DynamicCapAdjustConfig, "logicconfig/dynamic_cap_adjust.xml", true);
	// �츳����
	CONFIG_MODULE_DEF_CPP(TalentConfig, "logicconfig/role_talent.xml", true);
	// �����츳����
	CONFIG_MODULE_DEF_CPP(JinJieTalentConfig, "logicconfig/talent.xml", true);
	// ����װ����
	CONFIG_MODULE_DEF_CPP(ArmorDefendConfig, "logicconfig/fb_armor_defend.xml", true);
	// ��������������
	CONFIG_MODULE_DEF_CPP(BuildTowerFbConfig, "logicconfig/build_tower_fb.xml", true);
	// ����
	CONFIG_MODULE_DEF_CPP(YaoShiConfig, "logicconfig/yaoshi.xml", true);
	// ͷ��
	CONFIG_MODULE_DEF_CPP(TouShiConfig, "logicconfig/toushi.xml", true);
	// �����
	CONFIG_MODULE_DEF_CPP(QiLinBiConfig, "logicconfig/qilinbi.xml", true);
	// ���
	CONFIG_MODULE_DEF_CPP(MaskConfig, "logicconfig/mask.xml", true);
	// תְװ��
	CONFIG_MODULE_DEF_CPP(ZhuanzhiEquipConfig, "logicconfig/zhuanzhiequipcfg.xml", true);
	// ���ɴ���
	CONFIG_MODULE_DEF_CPP(GuildQuestionConfig, "logicconfig/guild_question.xml", true);
	// ҹս����
	CONFIG_MODULE_DEF_CPP(NightFightFbConfig, "logicconfig/nightfightfb.xml", true);
	//��������
	CONFIG_MODULE_DEF_CPP(HeShenLuoShuConfig, "logicconfig/heshen_luoshu_cfg.xml", true);
	// ʥ��ϵͳ����
	CONFIG_MODULE_DEF_CPP(ShengqiConfig, "logicconfig/shengqi_cfg.xml", true);
	//�Ϲ�boss
	CONFIG_MODULE_DEF_CPP(ShangGuBossConfig, "logicconfig/shangguboss.xml", true);
	//����boss
	CONFIG_MODULE_DEF_CPP(PersonBossConfig, "logicconfig/person_boss.xml", true);
	//С���ػ�
	CONFIG_MODULE_DEF_CPP(ImpGuardConfig, "logicconfig/impguard.xml", true);
	//bossͼ��
	CONFIG_MODULE_DEF_CPP(BossCardConfig, "logicconfig/bosscardcfg.xml", true);
	//�Ҷ�ս��
	CONFIG_MODULE_DEF_CPP(MessBattleFbConfig, "logicconfig/messbattlefb.xml", true);
	// ʥӡϵͳ
	CONFIG_MODULE_DEF_CPP(SealConfig, "logicconfig/seal_cfg.xml", true);
	// ����ػ�����
	CONFIG_MODULE_DEF_CPP(TeamTowerDefendConfig, "logicconfig/teamdefend.xml", true);
	// ��Ӣ���װ������
	CONFIG_MODULE_DEF_CPP(FBEquipConfig, "logicconfig/fbequip.xml", true);
	// �����˺����ֵ����
	CONFIG_MODULE_DEF_CPP(MonsterHurtHpConfig, "logicconfig/monster_hurt_maxhp.xml", true);
	// ���˶���־
	CONFIG_MODULE_DEF_CPP(LuckyLogConfig, "logicconfig/lucky_log_config.xml", true);
	// ��̬����
	CONFIG_MODULE_DEF_CPP(DynamicAdjustConfig, "logicconfig/dynamic_adjust.xml", true);
	// �¹�����
	CONFIG_MODULE_DEF_CPP(DressingRoomConfig, "logicconfig/dressing_room.xml", true);
	//�����ػ�
	CONFIG_MODULE_DEF_CPP(HolidayGuardConfig, "logicconfig/holidayguardconfig.xml", true);
	// ����ϵͳ��������
	CONFIG_MODULE_DEF_CPP(JinjiesysRewardConfig, "logicconfig/jinjiesys_reward.xml", true);
	//��ɫ��СĿ��
	CONFIG_MODULE_DEF_CPP(RoleBigSmallGoalConfig, "logicconfig/role_big_small_goal.xml", true);
	//����ʥ��
	CONFIG_MODULE_DEF_CPP(ShenqiConfig, "logicconfig/shenqi_cfg.xml", true);
	// ħ�似��
	CONFIG_MODULE_DEF_CPP(MojieSkillConfig, "logicconfig/mojie_skill_config.xml", true);
	// ���ͼ¼
	CONFIG_MODULE_DEF_CPP(ShenBingTuLuConfig, "logicconfig/shenbing_tulu_config.xml", true);
	//����ȼ�����ӳ�
	CONFIG_MODULE_DEF_CPP(WorldLevelDifferenceExpAddConfig, "logicconfig/world_level_difference_exp_add_config.xml", true);
	//����ϵͳף��ֵ����
	CONFIG_MODULE_DEF_CPP(UpgradeSysCritConfig, "logicconfig/upgrade_sys_crit_config.xml", true);
	//ħ��
	CONFIG_MODULE_DEF_CPP(MolongConfig, "logicconfig/molong.xml", true);
	//����
	CONFIG_MODULE_DEF_CPP(XianZunKaConfig, "logicconfig/xianzunka.xml", true);
	//ս������
	CONFIG_MODULE_DEF_CPP(ActivityMsgConfig, "logicconfig/activity_msg_cfg.xml", true);
	//�ȼ���������
	CONFIG_MODULE_DEF_CPP(ItemLevelRewardConfig, "logicconfig/role_level_reward.xml", true);
	// ���޼�԰����
	CONFIG_MODULE_DEF_CPP(SpouseHomeConfig, "logicconfig/spouse_home_cfg.xml", true);
    // ����滻
	CONFIG_MODULE_DEF_CPP(GiftReplaceConfig, "logicconfig/replacegiftconfig.xml", true);
	// װ��ϴ��
	CONFIG_MODULE_DEF_CPP(EquipBaptizeConfig, "logicconfig/equip_baptize_cfg.xml", true);
	// ����ϵͳ
	CONFIG_MODULE_DEF_CPP(GreateSoldierConfig, "logicconfig/greate_soldier_config.xml", true);
	// �齱���״���
	CONFIG_MODULE_DEF_CPP(LotteryCountConfig, "logicconfig/lottery_count_config.xml", true);
	// ������Ʒ�㲥
	CONFIG_MODULE_DEF_CPP(FallingMSGConfig, "logicconfig/falling_msg_config.xml", true);
	// ��������
	CONFIG_MODULE_DEF_CPP(AgentAdapterConfig, "logicconfig/agent_adapter.xml", true);
	// ��ħboss
	CONFIG_MODULE_DEF_CPP(GodmagicBossConfig, "logicconfig/godmagicboss.xml", true);

	// ����������ã����ݲ�ͬר��������
	char rand_cfg_path[256] = { 0 };
	SNPRINTF(rand_cfg_path, sizeof(rand_cfg_path), "logicconfig/cross_randactivity_cfg_%d.xml", LocalConfig::Instance().GetRandActZhuanfuType());
	CONFIG_MODULE_DEF_CPP(CrossRandActivityConfig, rand_cfg_path, true);
	
	// ֱ��������
	CONFIG_MODULE_DEF_CPP(UpgradeCardBuyConfig, "logicconfig/upgrade_card_buy_cfg.xml", true);

	// ���������
	CONFIG_MODULE_DEF_CPP(ActivitySwitchConfig, "logicconfig/act_switch_cfg.xml", true);
	// ������
	CONFIG_MODULE_DEF_CPP(HuguozhiliConfig, "logicconfig/huguozhili.xml", true);

	//�������⽱������
	CONFIG_MODULE_DEF_CPP(TodayThemeRewardConfig, "logicconfig/today_theme_reward.xml", true);

	//
	CONFIG_MODULE_DEF_CPP(CrossGoalConfig, "logicconfig/cross_goal.xml", true);

	// �����ո�
	CONFIG_MODULE_DEF_CPP(GiftHarvestConfig, "logicconfig/gift_harvest.xml", true);

	// ��սװ��
	CONFIG_MODULE_DEF_CPP(BaizhanEquipConfig, "logicconfig/baizhanequipcfg.xml", true);

	// ����Ѱ��
	CONFIG_MODULE_DEF_CPP(TianshuXZConfig, "logicconfig/tianshuxunzhu_cfg.xml", true);

	// ����װ��
	CONFIG_MODULE_DEF_CPP(CrossEquipConfig, "logicconfig/cross_equip.xml", true);
	
	//С����
	CONFIG_MODULE_DEF_CPP(LittleHelperConfig, "logicconfig/little_helper.xml", true);

	// ����
	CONFIG_MODULE_DEF_CPP(FriendConfig, "logicconfig/friendcfg.xml", true);

	// ����
	CONFIG_MODULE_DEF_CPP(WorshipConfig, "logicconfig/worship.xml", true);

	// ����������
	CONFIG_MODULE_DEF_CPP(MonsterExtraDropConfig, "logicconfig/monster_extra_drop.xml", true);

	//������
	CONFIG_MODULE_DEF_CPP(MythConfig, "logicconfig/myth.xml", true);
	//����
	CONFIG_MODULE_DEF_CPP(ZodiacConfig, "logicconfig/zodiac_cfg.xml", true);

	// ��ĩװ��
	CONFIG_MODULE_DEF_CPP(TianshenHutiConfig, "logicconfig/tianshenhuti.xml", true);

	return true;
}

bool LogicConfigManager::Reload(const std::string &path, std::string *err)
{
	LogicConfigManager *temp = new LogicConfigManager();

	if (!temp->Init(path, err))
	{
		delete temp; temp = NULL;
		return false;
	}

	if (NULL != gs_logicconfig_manager) delete gs_logicconfig_manager;

	gs_logicconfig_manager = temp;

	return true;
}
