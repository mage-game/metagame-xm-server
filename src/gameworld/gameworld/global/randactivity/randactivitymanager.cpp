#include <memory>

#include "randactivitymanager.hpp"
#include "randactivity.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "globalconfig/dynamicconfig.hpp"
#include "servercommon/activitydef.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "globalconfig/globalconfig.h"
#include "global/randactivity/impl/randactivityluckroll.hpp"
#include "global/randactivity/impl/randactivitydaychongzhifanli.hpp"
#include "global/randactivity/impl/randactivitychongzhirank.hpp"
#include "global/randactivity/impl/randactivitychongzhirank2.hpp"
#include "global/randactivity/impl/randactivityserverpanicbuy.hpp"
#include "global/randactivity/impl/randactivitypersonalpanicbuy.hpp"
#include "global/randactivity/impl/randactivitydayconsumegold.hpp"
#include "global/randactivity/impl/randactivitystoneuplevel.hpp"
#include "global/randactivity/impl/randactivityconsumegoldrank.hpp"
#include "global/randactivity/impl/randactivityequipstrengthen.hpp"
#include "global/randactivity/impl/randactivityconsumegoldfanli.hpp"
#include "global/randactivity/impl/randactivitytotalconsumegold.hpp"
#include "global/randactivity/impl/randactivitydayactivedegree.hpp"
#include "global/randactivity/impl/randactivitydoufakuanghuan.hpp"
#include "global/randactivity/impl/randactivityzhanchangfanbei.hpp"
#include "global/randactivity/impl/randactivityxiannvchanmianuplevel.hpp"
#include "global/randactivity/impl/randactivitymountupgrade.hpp"
#include "global/randactivity/impl/randactivityqibingupgrade.hpp"
#include "global/randactivity/impl/randactivitymentalitytotallevel.hpp"
#include "global/randactivity/impl/randactivitywingupgrade.hpp"
#include "global/randactivity/impl/randactivitykillboss.hpp"
#include "global/randactivity/impl/randactivitydayonlinegift.hpp"
#include "global/randactivity/impl/randactivitychestshop.hpp"
#include "global/randactivity/impl/randactivityquanminqifu.hpp"
#include "global/randactivity/impl/randactivityxianmengbipin.hpp"
#include "global/randactivity/impl/randactivityxianmengjueqi.hpp"
#include "global/randactivity/impl/randactivitylogingift.hpp"
#include "global/randactivity/impl/randactivityshouyouyuxiang.hpp"
#include "global/randactivity/impl/randactivitybipincapabilitytotal.hpp"
#include "global/randactivity/impl/randactivitybipincapabilityequip.hpp"
#include "global/randactivity/impl/randactivitybipincapabilitymount.hpp"
#include "global/randactivity/impl/randactivitybipincapabilityxiannv.hpp"
#include "global/randactivity/impl/randactivitybipincapabilityjingmai.hpp"
#include "global/randactivity/impl/randactivitybipincapabilitywing.hpp"
#include "global/randactivity/impl/randactivitychargerepayment.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/randactivity/impl/randactivitysinglecharge.hpp"
#include "global/randactivity/impl/randactivitycornucopia.hpp"
#include "global/randactivity/impl/randactivitydaychongzhidouble.hpp"
#include "global/randactivity/impl/randactivitydanbichongzhi.hpp"
#include "global/randactivity/impl/randactivitytotalchongzhiday.hpp"
#include "global/randactivity/impl/randactivitytomorrowreward.hpp"
#include "global/randactivity/impl/randactivitydaychongzhidouble2.hpp"
#include "global/randactivity/impl/randactivitydaychongzhirank.hpp"
#include "global/randactivity/impl/randactivitydayconsumerank.hpp"
#include "global/randactivity/impl/randactivitytotalchongzhi.hpp"
#include "global/randactivity/impl/randactivitydoublescore.hpp"
#include "global/randactivity/impl/randactivitytimelimitexchange.hpp"
#include "global/randactivity/impl/randactivitylevellottery.hpp"
#include "global/randactivity/impl/randactivitychongzhiniuegg.hpp"
#include "global/randactivity/impl/randactivitybipincapabilityshenzhuang.h"
#include "global/randactivity/impl/randactivitybipincapabilityjingling.h"
#include "global/randactivity/impl/randactivitybipincapabilityjinglian.h"
#include "global/randactivity/impl/randactivityzhenbaoge.hpp"
#include "global/randactivity/impl/randactivitymijingxunbao.hpp"
#include "global/randactivity/impl/randactivityjisuchongzhan.h"
#include "global/randactivity/impl/randactivitymoneytree.hpp"
#include "global/randactivity/impl/randactivitydailylove.h"
#include "global/randactivity/impl/randactivitykingdraw.hpp"
#include "global/randactivity/impl/randactivitytotalcharge3.hpp"
#include "global/randactivity/impl/randactivityspecialappearancepassiverank.hpp"
#include "global/randactivity/impl/randactivityspecialappearancerank.hpp"
#include "global/randactivity/impl/randactivityanymonsterdrop.hpp"
#include "global/randactivity/impl/randactivitytotalcharge4.hpp"
#include "global/randactivity/impl/randactivitymine.hpp"
#include "global/randactivity/impl/randactivityguagua.hpp"
#include "global/randactivity/impl/randactivitytianmingdivination.hpp"
#include "global/randactivity/impl/randactivityfanfan.hpp"
#include "global/randactivity/impl/randactivitycontinueconsume.hpp"
#include "global/randactivity/impl/randactivitycontinuezhongzhi.hpp"
#include "global/randactivity/impl/randactivityarmyday.hpp"
#include "global/randactivity/impl/randactivityanymonsterdrop2.hpp"
#include "global/randactivity/impl/randactivitycirculationchongzhi.hpp"
#include "global/randactivity/impl/randactivityextremelucky.hpp"
#include "global/randactivity/impl/randactivitymijingxunbao2.hpp"
#include "global/randactivity/impl/randactivitymoneytree2.hpp"
#include "global/randactivity/impl/randactivitynationalday.hpp"
#include "global/randactivity/impl/randactivityzhenbaoge2.hpp"
#include "global/randactivity/impl/randactivitypromotingposition.hpp"
#include "global/randactivity/impl/randactivityblackmarket.hpp"
#include "global/randactivity/impl/randactivitytreasuresmall.h"
#include "global/randactivity/impl/randactivityitemcollection.hpp"
#include "global/randactivity/impl/randactivityredenvelopegift.hpp"
#include "global/randactivity/impl/randactivitymarryme.hpp"
#include "global/randactivity/impl/randactivitygoldenpig.hpp"
#include "global/randactivity/impl/randactivityrushbuying.hpp"
#include "global/randactivity/impl/randactivitymaphunt.h"
#include "global/randactivity/impl/randactivitylighttowerexplore.hpp"
#include "global/randactivity/impl/randactivitynewtotalchongzhi.hpp"
#include "global/randactivity/impl/randactivitymagicshop.hpp"
#include "global/randactivity/impl/randactivityfishing.hpp"
#include "global/randactivity/impl/randactivityplantingtree.hpp"
#include "global/randactivity/impl/randactivitylottery1.hpp"
#include "impl/randactivitycrystaldalaran.hpp"
#include "impl/randactivitychongzhicrazyrebate.hpp"
#include "impl/randactivityperfectlover.hpp"
#include "global/randactivity/impl/randactivityguildbattle.hpp"
#include "impl/randactivityconsumegift.hpp"
#include "impl/randactivitycriticalstrike.hpp"
#include "impl/randactivitylimitbuy.hpp"
#include "impl/randactivitycollecttreasure.hpp"
#include "impl/randactivityhappycumulchongzhi.hpp"
#include "impl/randactivityjinjiereturn.hpp"
#include "impl/randactivitygiftharvest.hpp"
#include "global/randactivity/impl/randactivitymijingxunbao3.hpp"
#include "global/randactivity/impl/randactivityhuanlezadan.hpp"
#include "global/randactivity/impl/randactivityhuanleyaojiang.hpp"
#include "impl/randactivityhuanleyaojiang2.hpp"
#include "global/randactivity/impl/randactivityholidayguard.hpp"
#include "impl/randactivityprofessrank.hpp"
#include "global/randactivity/impl/randactivityluckywish.hpp"
#include "config/activityconfig/activityswitchconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "config/activityconfig/randactivityopencfg.hpp"
#include "impl/randactivityluckycloudbuy.hpp"
#include "impl/randactivitycracybuy.hpp"

RandActivityManager::RandActivityManager() : m_is_first(true)
{
	UNSTD_STATIC_CHECK(209 == RAND_ACTIVITY_TYPE_END - RAND_ACTIVITY_TYPE_BEGIN);

	memset(m_rand_activity_list, 0, sizeof(m_rand_activity_list));

	m_rand_activity_list[RAND_ACTIVITY_TYPE_LUCKY_ROLL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityLuckRoll(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDayChongZhiFanLi(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CHONGZHI_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityChongzhiRank(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityServerPanicBuy(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_PERSONAL_PANIC_BUY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityPersonalPanicBuy(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDayConsumeGold(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityConsumeGoldRank(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CONSUME_GOLD_FANLI - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityConsumeGoldFanli(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityTotalConsumeGold(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDayActiveDegree(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_EQUIP_STRENGTHEN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityEquipStrengthen(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CHESTSHOP - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityChestshop(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_STONE_UPLEVEL- RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityStoneUplevel(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_XN_CHANMIAN_UPLEVEL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityXiannvChanmianUplevel(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_MOUNT_UPGRADE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityMountUpgrade(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_QIBING_UPGRADE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityQibingUpgrade(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityMentalityTotalLevel(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_WING_UPGRADE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityWingUpgrade(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_QUANMIN_QIFU - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityQuanminQifu(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityShouYouYuXiang(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_XIANMENG_JUEQI - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityXianmengJueQi(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_XIANMENG_BIPIN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityXianmengBipin(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DAY_ONLINE_GIFT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDayOnlineGift(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_KILL_BOSS - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityKillBoss(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DOUFA_KUANGHUAN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDoufaKuanghuan(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_ZHANCHANG_FANBEI - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityZhanchangFanbei(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_LOGIN_GIFT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityLoginGift(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityBipinCapabilityTotal(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityBipinCapabilityEquip(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityBipinCapabilityXiannv(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityBipinCapabilityJingmai(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityBipinCapabilityMount(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityBipinCapabilityWing(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityChargeRepayment(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SINGLE_CHARGE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivitySingleCharge(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CORNUCOPIA - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityCornucopia(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DAY_CHONGZHI_DOUBLE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDayChongzhiDouble(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DANBI_CHONGZHI - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDanbiChongzhi(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityTotalChongzhiDay(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TOMORROW_REWARD - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityTomorrowReward(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DAY_CHONGZHI_DOUBLE_2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDayChongzhiDouble2(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DAY_CHONGZHI_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDayChongzhiRank(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDayConsumeRank(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TOTAL_CHARGE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityTotalChongzhi(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DOUBLE_SCORE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDoubleScore(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_EQUIP - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityTimeLimitExchangeEqui(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_JL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityTimeLimitExchangeJL(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_LEVEL_LOTTERY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityLevelLottery(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityChongzhiNiuEgg(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityBipinCapabilitySZ(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityBipinCapabilityJingling(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityBipinCapabilityWash(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_ZHENBAOGE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityZhenBaoGe(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_MIJINGXUNBAO - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityMiJingXunBao(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_JISUCHPONGZHAN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityJiSuChongZhan(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_MONEY_TREE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityMoneyTree(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DAILY_LOVE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityDailyLove(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_KING_DRAW - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityKingDraw(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityTotalCharge3(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivitySpecialAppearancePassiveRank(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivitySpecialAppearanceRank(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_ANY_MONSTER_DROP - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityAllMonsterDrop(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityTotalCharge4(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_MINE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityMine(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_GUAGUA - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityGuaGua(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TIANMING_DIVINATION - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityTianMingDivination(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_FANFAN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityFanFan(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CONTINUE_CONSUME - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityContinueConsume(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityContinueChongzhi(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_ARMY_DAY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityArmyDay(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_ANY_MONSTER_DROP2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityAllMonsterDrop2(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityCirculationChongzhi(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_EXTREME_LUCKY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityExtremeLucky(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_MIJINGXUNBAO2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityMiJingXunBao2(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_MONEY_TREE2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityMoneyTree2(this);	
	m_rand_activity_list[RAND_ACTIVITY_TYPE_NATIONAL_DAY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityNationalDay(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_ZHENBAOGE2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityZhenBaoGe2(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_PROMOTING_POSITION - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityPromotingPosition(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BLACK_MARKET - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityBlackMarket(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TREASURES_MALL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityTreasuresMall(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_ITEM_COLLECTION - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityItemCollection(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_RED_ENVELOPE_GIFT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityRedEnvelopeGift(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_MARRY_ME - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityMarryMe(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_ROLE_UPLEVEL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_ROLE_UPLEVEL);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_PATA - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_PATA);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_EXP_FB - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_EXP_FB);
	//m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_MOUNT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_MOUNT);
	//m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_HALO - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_HALO);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_WING - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_WING);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_SHENYI - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_SHENYI);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE);
	//m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_TOTAL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_TOTAL);
	//m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_HALO_TOTAL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_HALO_TOTAL);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_WING_TOTAL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_WING_TOTAL);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_TOTAL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_TOTAL);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_TOTAL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_TOTAL);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_NEW_CORNUCOPIA - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_NEW_CORNUCOPIA);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BOSS_KILLER - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_BOSS_KILLER);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SUPPER_GIFT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SUPPER_GIFT);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_OPEN_SERVER_BATTLE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_OPEN_SERVER_BATTLE);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_EXP_REFINE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_EXP_REFINE);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_XIANNV_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_XIANNV_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_JINGLING_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_JINGLING_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_GOLDEN_PIG - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityGoldenPig(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_OPEN_SERVER_INVEST - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_OPEN_SERVER_INVEST);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TREASURE_CONVERT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_TREASURE_CONVERT);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_XIANYUAN_TREAS - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_XIANYUAN_TREAS);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_RUSH_BUYING - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityRushBuying(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SINGLE_CHARGE_2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SINGLE_CHARGE_2);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SINGLE_CHARGE_3 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SINGLE_CHARGE_3);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SINGLE_CHARGE_4 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SINGLE_CHARGE_4);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SINGLE_CHARGE_5 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SINGLE_CHARGE_5);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_MAP_HUNT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityMapHunt(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityLightTowerExplore(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityNewTotalChongzhi(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_MAGIC_SHOP - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityMagicShop(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_PLANTING_TREE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityPlantingTree(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_FISHING - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityFishing(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityCrytalDalaran(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CHONGZHI_CRAZY_REBATE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityChongzhiCrazyRebate(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_PERFECT_LOVER - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityPerfectLover(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_QUANMINJINJIE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_QUANMINJINJIE);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_GUILDBATTLE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityGuildBattle(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CONSUM_GIFT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityConsumGift(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BUY_LIMIT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityLimitBuy(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_COLLECT_TREASURE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityCollectTreasure(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityHappyCumulChongzhi(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityCriticalStrike(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPLEVEL_RETURN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityJinjieReturn(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SHAKE_MONEY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SHAKE_MONEY);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_RMB_BUT_CHEST_SHOP - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_RMB_BUT_CHEST_SHOP);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_MIJINGXUNBAO3 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityMiJingXunBao3(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_HUANLE_ZADAN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityHuanLeZaDan(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityHuanLeYaoJiang(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityHuanLeYaoJiang2(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_LOGIN_GIFT_0 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_LOGIN_GIFT_0);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_HOLIDAY_GUARD - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityHolidayGuard(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SPECIAL_IMG_SUIT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SPECIAL_IMG_SUIT);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CHONGZHI_GIFT - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_CHONGZHI_GIFT);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_PROFESS_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityProfessRank(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY_2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY_2);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_COMBINE_BUY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_COMBINE_BUY);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_LUCKY_WISH - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityLuckyWish(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_IMAGE_COMPETITION - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_IMAGE_COMPETITION);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DISCOUNT_BUY_HOME - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_DISCOUNT_BUY_HOME);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DISCOUNT_BUY_FURNITURE - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_DISCOUNT_BUY_FURNITURE);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_COUPLE_HALO_SELL - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_COUPLE_HALO_SELL);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_WEST_WEDDING - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_WEST_WEDDING);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_LOTTERY_1 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityLottery1(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DISCOUNT_GET_BABY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_DISCOUNT_GET_BABY);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SUPPER_GIFT_2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SUPPER_GIFT_2);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_ZERO_BUY_RETURN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_ZERO_BUY_RETURN);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DAILY_LOVE_2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_DAILY_LOVE_2);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_GIFT_HARVEST - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityGiftHarvest(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityLuckyCloudBuy(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CRACYBUY - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityCracyBuyToAll(this);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_XIAN_ZUN_KA - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_XIAN_ZUN_KA);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SINGLE_CHARGE1 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SINGLE_CHARGE1);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SINGLE_CHARGE2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SINGLE_CHARGE2);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_SINGLE_CHARGE3 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_SINGLE_CHARGE3);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_DOUBLE_GET - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivity(this, RAND_ACTIVITY_TYPE_DOUBLE_GET);
	m_rand_activity_list[RAND_ACTIVITY_TYPE_CHONGZHI_RANK2 - RAND_ACTIVITY_TYPE_BEGIN] = new RandActivityChongzhiRank2(this);

}

RandActivityManager::~RandActivityManager()
{
	for (int i = 0; i < RAND_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_rand_activity_list[i])
		{
			delete m_rand_activity_list[i];
		}
	}

	memset(m_rand_activity_list, 0, sizeof(m_rand_activity_list));
}

RandActivityManager & RandActivityManager::Instance()
{
	static RandActivityManager ram;
	return ram;
}

void RandActivityManager::Init(const RandActivityParam &param)
{
	for (int i = 0; i < RAND_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_rand_activity_list[i])
		{
			m_rand_activity_list[i]->Init(param.data_list[i]);
		}
	}
}

void RandActivityManager::GetInitParam(RandActivityParam *param)
{
	if (NULL == param) return;

	for (int i = 0; i < RAND_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_rand_activity_list[i])
		{
			m_rand_activity_list[i]->GetInitParam(&param->data_list[i]);
		}
	}
}

void RandActivityManager::Update(unsigned long interval, time_t now_second)
{
	if (!WorldStatus::Instance().IsLoadFinish()) return;

	for (int i = 0; i < RAND_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_rand_activity_list[i])
		{
			m_rand_activity_list[i]->Update(interval, now_second);
		}
	}

	if (m_is_first)
	{
		m_is_first = false;

		this->CalActivityOpenTime();
	}

	{
		// 随机活动设置时间或者强制设置时间，这里做延迟执行，代码简单点，不必过分追求性能

		std::set<long long> aleady_set_time_keys; // 执行过的任务，用于删除
		for (RASetTimeItemIt item_it = m_ra_delay_settime_map.begin(); item_it != m_ra_delay_settime_map.end(); ++item_it)
		{
			if (now_second >= item_it->second.delay_to_time)
			{
				aleady_set_time_keys.insert(item_it->first);
				this->OnSetActivityTime(item_it->second.rand_activty_type, item_it->second.begin_time, item_it->second.end_time, 0);
			}
		}
		for (std::set<long long>::iterator key_it = aleady_set_time_keys.begin(); key_it != aleady_set_time_keys.end(); ++key_it)
		{
			m_ra_delay_settime_map.erase(*key_it);
		}

		aleady_set_time_keys.clear();
		for (RASetTimeItemIt item_it = m_ra_delay_force_settime_map.begin(); item_it != m_ra_delay_force_settime_map.end(); ++item_it)
		{
			if (now_second >= item_it->second.delay_to_time)
			{
				aleady_set_time_keys.insert(item_it->first);
				this->OnForceSetActivityTime(item_it->second.rand_activty_type, item_it->second.begin_time, item_it->second.end_time, item_it->second.open_type);
			}
		}
		for (std::set<long long>::iterator key_it = aleady_set_time_keys.begin(); key_it != aleady_set_time_keys.end(); ++key_it)
		{
			m_ra_delay_force_settime_map.erase(*key_it);
		}
	}
}

void RandActivityManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	for (int i = 0; i < RAND_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_rand_activity_list[i])
		{
			m_rand_activity_list[i]->OnDayChange(old_dayid, now_dayid);
		}
	}

	//unsigned int now_second = static_cast<unsigned int> (EngineAdapter::Instance().Time());
	//{
	//	// 写死的逻辑
	//	int open_server_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), now_second) + 1;
	//	if (8 == open_server_day) // 开服第八天，强制开启活动
	//	{
	//		OnForceSetActivityTime(2188, now_second, now_second + SECOND_PER_DAY * 3);
	//	}
	//}
}

void RandActivityManager::OnUserLogin(Role *user)
{
	for (int i = 0; i < RAND_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_rand_activity_list[i])
		{
			m_rand_activity_list[i]->OnUserLogin(user);
		}
	}
}

void RandActivityManager::OnUserLogout(Role *user)
{
	for (int i = 0; i < RAND_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_rand_activity_list[i])
		{
			m_rand_activity_list[i]->OnUserLogout(user);
		}
	}
}

bool RandActivityManager::IsRandActivityOpen(int rand_activity_type)
{
	rand_activity_type -= RAND_ACTIVITY_TYPE_BEGIN;
	if (rand_activity_type < 0 || rand_activity_type >= RAND_ACTIVITY_TYPE_MAX) 
	{
		return false;
	}

	bool is_open = false;
	if (NULL != m_rand_activity_list[rand_activity_type])
	{
		is_open = (ACTIVITY_STATUS_OPEN == m_rand_activity_list[rand_activity_type]->GetStatus());
	}

	return is_open;
}

int RandActivityManager::GetSevenDayBipinCurOpenType()
{
	if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK;
	}
	else if(this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK;
	}
	else if(this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK;
	}
	else if(this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK;
	}
	else if(this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK;
	}
	else if(this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK))
	{
		return RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK;
	}
	else if(this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK))
	{
		return RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_XIANNV_RANK))
	{
		return RAND_ACTIVITY_TYPE_XIANNV_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_JINGLING_RANK))
	{
		return RAND_ACTIVITY_TYPE_JINGLING_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK))
	{
		return RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK))
	{
		return RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK;
	}
	else if (this->IsRandActivityOpen(RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK))
	{
		return RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK;
	}
	return -1;
}

bool RandActivityManager::OnSetActivityTime(int rand_activity_type, unsigned int begin_time, unsigned int end_time, int limit_day)
{
	/*
		1.关闭活动：limit_day = ?, now_time > end_time > begin_time
		2.改变活动时间: limit_day = 0, end_time_> now_time, end_time > begin_time
		3.开服前n天不开:limit_day = n, end_time > begin_time, end_time ? now_time, begin_time ? now_time
	*/

	//////最新的。。可以插了。。屏掉。
	{
		// 开服多少天内不允许插入随机活动
		time_t server_real_start_time = SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime();
		int now_day_idx = GetDayIndex(server_real_start_time, EngineAdapter::Instance().Time());
		int allow_set_time_day_idx = LOGIC_CONFIG->GetRandActivityOpenCfg().GetAllowSetTimeDayIdx();
		if (now_day_idx + 1 < allow_set_time_day_idx)
		{
			return false;
		}
	}

	if (LOGIC_CONFIG->GetActivitySwitchConfig().IsBanActivity(rand_activity_type)) return false;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return false;
	}

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (limit_day > 0 && end_time > now_time)									// 结束时间小于当前时间为关闭活动，不判断limit_day
	{
		time_t server_real_start_time = SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime();
		if (GetDayIndex(server_real_start_time, begin_time) < limit_day)		// 修正活动开启时间
		{
			time_t zero_time = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime();

			begin_time = (unsigned int)(zero_time + limit_day * 24 * 3600);
		}
	}

	if (begin_time >= end_time)
	{
		return false;
	}

// 	rand_activity_type -= RAND_ACTIVITY_TYPE_BEGIN;
// 	if (rand_activity_type < 0 || rand_activity_type >= RAND_ACTIVITY_TYPE_MAX) return;
// 
// 	if (NULL != m_rand_activity_list[rand_activity_type])
// 	{
// 		//活动正在开启中时，开始时间大于当前活动的时间认为是要开活动，则此时需禁止这种行为，需先结束该活动才能再开
// 		if (ACTIVITY_STATUS_OPEN ==  m_rand_activity_list[rand_activity_type]->GetStatus() &&
// 			begin_time >= m_rand_activity_list[rand_activity_type]->GetBeginTime())
// 		{
// 			return;
// 		}
// 
// 		//版本活动开启中,则不给设置时间（除非关闭活动）
// 		if (m_rand_activity_list[rand_activity_type]->IsOpenActivityOfVersion())
// 		{
// 			if (end_time > EngineAdapter::Instance().Time())
// 			{
// 				return;
// 			}
// 		}
// 
// 		m_rand_activity_list[rand_activity_type]->SetRandActivityTime(begin_time, end_time);
// 	}

	// 2019-04-11 修改为延迟执行
	int rand_activity_idx = rand_activity_type - RAND_ACTIVITY_TYPE_BEGIN;
	if (rand_activity_idx < 0 || rand_activity_idx >= RAND_ACTIVITY_TYPE_MAX) return false;

	if (NULL == m_rand_activity_list[rand_activity_idx])
	{
		return false;
	}

	//活动正在开启中时且下次开启时间大于这次活动结束时间则放入延迟执行列表
	if (ACTIVITY_STATUS_OPEN == m_rand_activity_list[rand_activity_idx]->GetStatus())
	{
		// 活动结束后要加开活动，加入缓存里延迟执行
		if (begin_time >= m_rand_activity_list[rand_activity_idx]->GetEndTime())
		{
			long long key = ConvertParamToLongLong(rand_activity_idx, begin_time); // 用活动+开始时间做key，一般不会冲突，也不允许冲突
			if (m_ra_delay_settime_map.find(key) == m_ra_delay_settime_map.end())
			{
				RASetTimeItem &item = m_ra_delay_settime_map[key];
				item.rand_activty_type = rand_activity_type;
				item.begin_time = begin_time;
				item.end_time = end_time;

				// 设置到当前活动结束后再设置一次，防止结算跟重开活动冲突，推迟几秒
				item.delay_to_time = m_rand_activity_list[rand_activity_idx]->GetEndTime() + 3;
			}

			return true;
		}
		else if (begin_time > now_time) // 在当前时间之后并且活动未关闭前要重新开启，说明会导致当前活动不正常关闭，直接忽略请求
		{
			return false;
		}
	}
	//else  // 不在活动中允许设置
	//{
	//	m_rand_activity_list[rand_activity_idx]->SetRandActivityTime(begin_time, end_time);
	//}

	m_rand_activity_list[rand_activity_idx]->SetRandActivityTime(begin_time, end_time);

	return true;
}

void RandActivityManager::OnForceToNextState(int rand_activity_type)
{
	if (LOGIC_CONFIG->GetActivitySwitchConfig().IsBanActivity(rand_activity_type)) return;

	rand_activity_type -= RAND_ACTIVITY_TYPE_BEGIN;
	if (rand_activity_type < 0 || rand_activity_type >= RAND_ACTIVITY_TYPE_MAX) return;

	if (NULL != m_rand_activity_list[rand_activity_type])
	{
		m_rand_activity_list[rand_activity_type]->OnForceToNextState();
	}
}

bool RandActivityManager::OnForceSetActivityTime(int rand_activity_type, unsigned int begin_time, unsigned int end_time, int open_type)
{
	// 强制设置随机活动，只做参数检查，确定插命令设置时，参考一下OnSetActivityTime里面的检查条件

	if (LOGIC_CONFIG->GetActivitySwitchConfig().IsBanActivity(rand_activity_type)) return false;

	int rand_activity_idx = rand_activity_type - RAND_ACTIVITY_TYPE_BEGIN;
	if (rand_activity_idx < 0 || rand_activity_idx >= RAND_ACTIVITY_TYPE_MAX)
	{
		return false;
	}

	if (open_type < RAND_ACTIVITY_OPEN_TYPE_BEGIN || open_type >= RAND_ACTIVITY_OPEN_TYPE_END)
	{
		return false;
	}

	if (begin_time > end_time)
	{
		return false;
	}

// 	if (NULL != m_rand_activity_list[rand_activity_idx])
// 	{
// 		m_rand_activity_list[rand_activity_idx]->SetRandActivityTime(begin_time, end_time, open_type);
// 	}

	// 2019-04-11 修改为延迟执行
	if (NULL == m_rand_activity_list[rand_activity_idx])
	{
		return false;
	}

	// 活动结束后要加开活动，加入缓存里延迟执行
	if (ACTIVITY_STATUS_OPEN == m_rand_activity_list[rand_activity_idx]->GetStatus() && begin_time >= m_rand_activity_list[rand_activity_idx]->GetEndTime())
	{
		long long key = ConvertParamToLongLong(rand_activity_type, begin_time); // 用活动+开始时间做key，一般不会冲突，也不允许冲突
		if (m_ra_delay_force_settime_map.find(key) == m_ra_delay_force_settime_map.end())
		{
			RASetTimeItem &item = m_ra_delay_force_settime_map[key];
			item.rand_activty_type = rand_activity_type;
			item.begin_time = begin_time;
			item.end_time = end_time;
			item.open_type = open_type;

			// 设置到当前活动结束后再设置一次。防止结算跟重开活动冲突，推迟几秒
			item.delay_to_time = m_rand_activity_list[rand_activity_idx]->GetEndTime() + 3;

			return true;
		}
	}
	else
	{
		m_rand_activity_list[rand_activity_idx]->SetRandActivityTime(begin_time, end_time, open_type);
		return true;
	}

	return false;
}

void RandActivityManager::OnPersonRankSnapshot(int old_top_uid_list[PERSON_RANK_TYPE_MAX], int new_top_uid_list[PERSON_RANK_TYPE_MAX])
{
	for (int i = 0; i < RAND_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_rand_activity_list[i])
		{
			m_rand_activity_list[i]->OnPersonRankSnapshot(old_top_uid_list, new_top_uid_list);
		}
	}
}

bool RandActivityManager::IsSyncRankData(int rand_activity_type)
{
	rand_activity_type -= RAND_ACTIVITY_TYPE_BEGIN;
	if (rand_activity_type < 0 || rand_activity_type >= RAND_ACTIVITY_TYPE_MAX) 
	{
		return false;
	}

	if (NULL != m_rand_activity_list[rand_activity_type])
	{
		return m_rand_activity_list[rand_activity_type]->IsSyncRankData();
	}

	return false;
}

RandActivityCornucopia * RandActivityManager::GetRandActivityCornucopia(int activity_type)
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_CORNUCOPIA - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_CORNUCOPIA == activity->GetActivityType())
	{
		return (RandActivityCornucopia *)activity;
	}

	return NULL;
}

RandActivityLuckRoll * RandActivityManager::GetRandActivityLuckRoll()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_LUCKY_ROLL - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_LUCKY_ROLL == activity->GetActivityType())
	{
		return (RandActivityLuckRoll *)activity;
	}

	return NULL;
}

RandActivityServerPanicBuy * RandActivityManager::GetRandActivityServerPanicBuy()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY == activity->GetActivityType())
	{
		return (RandActivityServerPanicBuy *)activity;
	}

	return NULL;
}

RandActivityShouYouYuXiang * RandActivityManager::GetRandActivityShouYouYuXiang()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG == activity->GetActivityType())
	{
		return (RandActivityShouYouYuXiang *)activity;
	}

	return NULL;
}

RandActivityLevelLottery * RandActivityManager::GetRandActivityLevelLottery()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_LEVEL_LOTTERY - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_LEVEL_LOTTERY == activity->GetActivityType())
	{
		return (RandActivityLevelLottery *)activity;
	}

	return NULL;
}

RandActivityChongzhiNiuEgg * RandActivityManager::GetRandActivityNiuEgg()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG == activity->GetActivityType())
	{
		return (RandActivityChongzhiNiuEgg *)activity;
	}

	return NULL;
}

RandActivityZhenBaoGe * RandActivityManager::GetRandActivityZhenBaoGe()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_ZHENBAOGE - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_ZHENBAOGE == activity->GetActivityType())
	{
		return (RandActivityZhenBaoGe *)activity;
	}

	return NULL;
}

RandActivityZhenBaoGe2 * RandActivityManager::GetRandActivityZhenBaoGe2()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_ZHENBAOGE2 - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_ZHENBAOGE2 == activity->GetActivityType())
	{
		return (RandActivityZhenBaoGe2 *)activity;
	}

	return NULL;
}

RandActivityMoneyTree * RandActivityManager::GetRandActivityMoneyTree()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_MONEY_TREE - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_MONEY_TREE == activity->GetActivityType())
	{
		return (RandActivityMoneyTree *)activity;
	}

	return NULL;
}

RandActivityMoneyTree2 * RandActivityManager::GetRandActivityMoneyTree2()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_MONEY_TREE2 - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_MONEY_TREE2 == activity->GetActivityType())
	{
		return (RandActivityMoneyTree2 *)activity;
	}

	return NULL;
}

void RandActivityManager::CalActivityOpenTime()
{
	for (int i = 0; i < RAND_ACTIVITY_TYPE_MAX; i++)
	{
		if (NULL != m_rand_activity_list[i])
		{
			m_rand_activity_list[i]->CalActivityOpenTime();
		}
	}
}

void RandActivityManager::GetRandActivityBipinRank3(int activity_type, PersonRankItem top_user_list[Protocol::TOP_COUNT])
{
	if ((RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL <= activity_type && activity_type <= RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP)
		|| (RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT <= activity_type && activity_type<= RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING)
		|| (RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG <= activity_type && activity_type <= RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH))
	{
		int index = activity_type - RAND_ACTIVITY_TYPE_BEGIN;
		if (index < 0 || index >= RAND_ACTIVITY_TYPE_MAX)
		{
			return;
		}

		RandActivity *activity =  m_rand_activity_list[index];
		if (NULL == activity)
		{
			return;
		}

		activity->RankBipinHistory(top_user_list);
	}
}

RandActivity * RandActivityManager::ForceGetRandActivity(int activity_type)
{
	if (activity_type < RAND_ACTIVITY_TYPE_BEGIN || activity_type >= RAND_ACTIVITY_TYPE_END)
	{
		return NULL;
	}

	RandActivity *activity = m_rand_activity_list[activity_type - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && activity_type == activity->GetActivityType())
	{
		return activity;
	}

	return NULL;
}

unsigned int RandActivityManager::GetRanadActivityOpenTime(int rand_activity_type)
{
	if (rand_activity_type < RAND_ACTIVITY_TYPE_BEGIN || rand_activity_type >= RAND_ACTIVITY_TYPE_END)
	{
		return 0;
	}

	RandActivity *activity = m_rand_activity_list[rand_activity_type - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL == activity)
	{
		return 0;
	}

	return activity->GetBeginTime();
}

RandActivityPerfectLover * RandActivityManager::GetRandActivityPerfectLover()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_PERFECT_LOVER - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_PERFECT_LOVER == activity->GetActivityType())
	{
		return (RandActivityPerfectLover *)activity;
	}

	return NULL;
}
RandActivityMiJingXunBao3 * RandActivityManager::GetRandActivityMiJingXunBao3()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_MIJINGXUNBAO3 - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_MIJINGXUNBAO3 == activity->GetActivityType())
	{
		return (RandActivityMiJingXunBao3 *)activity;
	}

	return nullptr;
}

RandActivityHuanLeZaDan * RandActivityManager::GetRandActivityHuanLeZaDan()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_HUANLE_ZADAN - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_HUANLE_ZADAN == activity->GetActivityType())
	{
		return (RandActivityHuanLeZaDan *)activity;
	}

	return nullptr;
}

RandActivityHuanLeYaoJiang * RandActivityManager::GetRandActivityHuanLeYaoJiang()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG == activity->GetActivityType())
	{
		return (RandActivityHuanLeYaoJiang *)activity;
	}

	return nullptr;
}

RandActivityLottery1 * RandActivityManager::GetRandActivityLottery1()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_LOTTERY_1 - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_LOTTERY_1 == activity->GetActivityType())
	{
		return (RandActivityLottery1 *)activity;
	}

	return nullptr;
}

RandActivityGiftHarvest * RandActivityManager::GetRandActivityGiftHarvest()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_GIFT_HARVEST - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_GIFT_HARVEST == activity->GetActivityType())
	{
		return (RandActivityGiftHarvest *)activity;
	}

	return nullptr;
}

RandActivityHuanLeYaoJiang2 * RandActivityManager::GetRandActivityHuanLeYaoJiang2()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2 - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2 == activity->GetActivityType())
	{
		return (RandActivityHuanLeYaoJiang2 *)activity;
	}

	return nullptr;
}

RandActivityLuckyCloudBuy * RandActivityManager::GetRandActivityLuckyCloudBuy()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY - RAND_ACTIVITY_TYPE_BEGIN];
	if (NULL != activity && RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY == activity->GetActivityType())
	{
		return (RandActivityLuckyCloudBuy *)activity;
	}

	return nullptr;
}

int RandActivityManager::GetRanadActivityDayIndex(int rand_activity_type)
{
	rand_activity_type -= RAND_ACTIVITY_TYPE_BEGIN;
	if (rand_activity_type < 0 || rand_activity_type >= RAND_ACTIVITY_TYPE_MAX)
	{
		return 0;
	}
	if (nullptr == m_rand_activity_list[rand_activity_type] ||!this->IsRandActivityOpen(rand_activity_type))
	{
		return 0;
	}

	unsigned int begin_time = m_rand_activity_list[rand_activity_type]->GetBeginTime();
	unsigned int end_time = m_rand_activity_list[rand_activity_type]->GetEndTime();

	int day_index = ((begin_time - end_time) / (3600 * 24)) + 1;
	return day_index;
}

RandActivityCracyBuyToAll * RandActivityManager::GetRandActivityCracyBuy()
{
	RandActivity *activity = m_rand_activity_list[RAND_ACTIVITY_TYPE_CRACYBUY - RAND_ACTIVITY_TYPE_BEGIN];
	if (nullptr != activity && RAND_ACTIVITY_TYPE_CRACYBUY == activity->GetActivityType())
	{
		return (RandActivityCracyBuyToAll *)activity;
	}

	return nullptr;
}