#ifndef __ROLE_ACTIVITY_HPP__
#define __ROLE_ACTIVITY_HPP__

#include "servercommon/roleactivitydef.hpp"
#include "servercommon/activitydef.hpp"
#include "servercommon/internalprotocal/ggprotocal.h"

#include "gamedef.h"
#include "protocal/msgactivity.hpp"
#include "other/chestshop/chestshop.hpp"

class Role;
class GatherObj;
struct RandActivityOpenServerRewardItemConfig;

class RoleActivity
{
public:
	RoleActivity();
	~RoleActivity();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const RoleActivityParam &p);
	void GetInitParam(RoleActivityParam *p);
	RoleActivityParam * GetParam() { return &m_activity_info; }

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnWeekChange();
	void OnRoleLevelChange(int level);

	void OnDie(ObjID killer_objid);
	void OnRoleLogin();
	void Update(unsigned int now_second);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcAttrByFirstRecharge(CharIntAttrs &base_add);
	void SyncFirstRechargeBuffInfo();							//首充buff标记(假buff)

	// --------------------------------------------------------------------------------------------------------
	// 场景活动
	// --------------------------------------------------------------------------------------------------------
	void OnActivityEnterReq(Protocol::CSActivityEnterReq *aer);
	int GetActivityEnterInfoHelper(int activity_type, Posi *enterpos, int *limit_level, int *max_level);
	void CheckZhanChangParam();
	void OnGuildBattleSosReq(Protocol::CSSendGuildSosReq *sgsr);
	void OnGongChengZhanSosReq(Protocol::CSSendGuildSosReq *sgsr);
	void OnSendGuildSosReq(int sos_type);
	void OnReplyGuildBattleSosReq(Protocol::CSReplyGuildSosReq *sos_reply);
	void OnReplyGongChengZhanSosReq(Protocol::CSReplyGuildSosReq *sos_reply);
	void OnZhuXieFetchReward(int task_id);
	ZhanchangRoomInfo & GetZhanchangRoomInfo() { return m_activity_info.zhanchang_room_info; }
	void SetZhanchangRoomInfo(unsigned int leave_time, int room_index) { m_activity_info.zhanchang_room_info.SetRoomInfo(leave_time, room_index); }
	int GetTempZhanchangSide() { return m_temp_zhanzhang_side; }
	void SetTempZhanchangSide(int _side) { m_temp_zhanzhang_side = _side; }
	void OnXianMengZhanEnterReq();
	void XianMengZhanGuildCall(bool use_gold);
	void XianMengZhanGuildFollow();
	void OnBattleFieldPickUpShenShiReq(int shenshi_objid);
	int GetBattleFieldHonor() { return m_activity_info.battlefield_honor; }
	void AddBattleFieldHonor(int honor_val, const char *type, bool notify_client = false);
	void OnGetBattleFieldHonorReq(int delta_honor = 0);
	void SetXianMengZhanDefendArea(int defend_area) { m_xianmengzhan_defend_area = defend_area; }
	int  GetXianMengZhanDefendArea() { return m_xianmengzhan_defend_area; }
	void OnEnterGongchengzhan();
	void OnEnterGuildBattle();
	void OnEnterQuestion_2();

	void SendGuildFbInfo();
	void SyncGuildFbStatus();
	void SyncGuildBeastInfo();
	void SyncGuildBonfireStatus();

	unsigned int GetZhuxieRewardNextTime() { return m_activity_info.zhuxie_reward_next_time; }
	void SetZhuxieRewardNextTime(unsigned int next_time) { m_activity_info.zhuxie_reward_next_time = next_time; }
	short GetGuildMonsterFinishSeq() { return m_activity_info.guild_monster_finish_seq; }
	void SetGuildMonsterFinishSeq(short seq) { m_activity_info.guild_monster_finish_seq = seq; }
	void SendGuildMonsterInfo();
	void OnNationalBossBuyBuffReq();

	void SetYiZhanDaoDiJiShaCount(int title_id) { m_yizhandaodi_jisha_count = title_id; }
	int  GetYiZhanDaoDiJiShaCount() { return m_yizhandaodi_jisha_count; }
	void SetYiZhanDaoDiDeadTimes(int times) { m_activity_info.yizhandaodi_dead_times = times; }
	void SetYiZhanDaoDiLeaveInfo(unsigned int leave_time, int room_index);
	void CheckInitYiZhanDaoDiLeaveInfo();
	void YiZhanDaoDiGuWu(Protocol::CSYiZhanDaoDiGuwu *yzddgw);
	void WuXingGuaJiGetReward(Protocol::CSWuXingGuaJiGetReward *wxgjgr);
	void SetFirstEnterGuildFb(GuildID guild_id);
	bool CanEnterGuildFb();
	void OnCSGCZFetchDailyRewardReq();
	void OnEnterZhuXieZhanChang();
	void OnEnterXianmengzhan();
	void OnEnterTianJiangCaiBao();
	void OnEnterWangLingExplore();

	// 夜战王城
	void OnNightFightEnter();
	// 水晶活动
	void ResetShuijingData();

	// --------------------------------------------------------------------------------------------------------
	// 封测活动
	// --------------------------------------------------------------------------------------------------------
	bool OnCBAFetchMarryReward();
	bool OnCBAFetchGuildReward();
	bool OnCBAFetchJoinActivityReward(int seq);
	bool OnCBAFetchOnlineReward(int seq);
	bool OnCBAFetchUplevelReward(int seq);
	bool OnCBAFetchLoginReward();
	void OnCloseBetaActivityOperaReq(int opera_type, int param_1, int param_2, int param_3);
	void SendCloseBetaActivityInfo();
	void OnPassEquipFB(int chapter, int level);

	// --------------------------------------------------------------------------------------------------------
	// 合服活动
	// --------------------------------------------------------------------------------------------------------
	void OnCombiendServerActivityDayChange();
	void OnCombineServerActivityOpen();
	void SendCSARoleInfo();

	bool OnCSARankQianggou(int item_idx);
	bool OnCSARollReq();
	void OnCSARollBroadcast(int item_id);
	bool OnCSAFetchKillBossReward();
	void OnCSASingleChargeCheckCharge(long long gold_num);
	void ONCSAChargeRewardDoubleCheckCharge(long long gold_num);

	bool OnCSALoginGiftFetchCommonReward(int seq);
	bool OnCSALoginGiftFetchVipReward(int seq);
	bool OnCSALoginGiftFetchAccumulateReward();
	void CheckCSALoginGiftGiveReward();

	bool OnCSAServerPanicBuyReq(int item_idx);
	bool OnCSAPersonalPanicBuyReq(int item_idx);
	void OnCSAOperaReq(int sub_type, int param_1, int param_2);  // 操作请求
	void OnCSALoginGiftLogin();
	void OnCSABossEnter();
	void SendCSABossInfo();
	void SendCSABossRankInfo();
	void SendCSABossRoleInfo();

	//攻城战
	void OnAddCSAGongChengZhanWinTimes();
	void SendCSAGongChengZhanInfo();

	// 投资计划
	void OnCSATouzijihuaOpera(int opera_type, int param);
	bool OnCSATouzijihuaBuy(int chongzhi_num);
	void OnCSATouzijihuaFetch(int index);
	void CheckTouzijihuaFetch();
	void SendCSATouzijihuaInfo();

	// 成长基金
	void OnCSAFoundationOpera(int opera_type, int param);
	bool OnCSAFoundationBuy(int chongzhi_num);
	void OnCSAFoundationFetch(int index);
	void CheckFoundationFetch();
	void SendCSAFoundationInfo();

	// 经验炼制
	void OnCSARefineOperaReq(int opera_type, int param);
	void OnCSARefineExpReq(int seq);
	void SendCSAExpRefineInfo();

	// 检查补发合服奖励
	void CheckGiveCombineReward();

	// --------------------------------------------------------------------------------------------------------
	// 开服活动
	// --------------------------------------------------------------------------------------------------------
	void SendOpenGameActivityInfo();
	void OnFetchOpenGameActivityReward(int reward_type, int reward_seq);
	bool OnFetchOGAPutonEquipmentReward(int seq);
	bool OnFetchOGACapabilityReward(int seq);
	bool OnFetchOGARoleLevelReward(int seq);
	void OnOGABuyEquipmentGift(int seq);
	void SendBaiBeiFanLiInfo();
	void OnBaiBeiFanLiBuy();
	void SendBaiBeiFanLi2Info();	//百倍返利2 信息
	void OnBaiBeiFanLi2Buy();		//百倍返利2 购买
	void OnFetchOpenGameKillBossReward(int seq);
	void OnOpenGameGiftShopBuy(int seq);
	void OnOpenGameGiftShopBuy2(int seq);	//礼包限购2

	// ------------------------------
	// 完美情人
	// ------------------------------
	bool OnRAPerfectMarry(int marry_type, int lover_uid);
	void PerfectLoverCheckAndSendInfo();
	void Brocast();
	void SendPerfectLoverInfo();

	// --------------------------------------------------------------------------------------------------------
	// 帮派争霸（开服活动）
	// --------------------------------------------------------------------------------------------------------
	void OnNewGuildBattle(int opera_type);
	void OnGuidBattleResultUpdate();
	bool OnFetchOGATotalGuidBattleReward();
	void OnOGACheckGuildBattleReward();

	// --------------------------------------------------------------------------------------------------------
	// 充值
	// --------------------------------------------------------------------------------------------------------
	void CheckChongzhiExtraRewardByNum(int chongzhi_num);
	void CheckChongzhiExtraRewardByFlag(int chongzhi_flag);
	void OnAddChongZhi(long long chongzhi);
	long long GetHistoryChongZhi() { return m_activity_info.history_chongzhi; }
	void SendChongZhiInfo();
	int GetHistoryChongzhiCount() { return m_activity_info.history_chongzhi_count; }
	int GetDayChongzhi() { return m_activity_info.day_chongzhi; }
	void FetchSpecialFirstChongzhiReward(int index);
	void FetchDailyTotalChongzhiReward(int seq);
	void FetchDailyFirstChongzhiReward();
	void GetTotalChongzhiWantMoneyInfo();
	void FetchTotalChongzhiWantMoneyReward();
	void FetchZaiChongzhiReward();
	void FetchThirdChongzhiReward();
	bool CheckZaiChongzhiRewardState(long long gold_num);
	bool CheckThirdChongzhiRewardState(long long gold_num);
	void CheckDailyTotalChongzhiStage();
	void FetchDiffWeekdayTotalChongzhiRewad(int stage);
	void CheckDiffWeekdayTotalChongzhiRewrad();
	void FetchFirstChongzhiReward(int level, int seq);
	void FetchDailyChongzhiReward(int seq, int reward_index);
	void FetchDailyChongzhiTimesReward(int seq);
	void FetchChongzhi7DayReward();
	void FetchDailyChongzhiReward2(int seq);

	// --------------------------------------------------------------------------------------------------------
	// 随机活动
	// --------------------------------------------------------------------------------------------------------

	void SendCollectExchangeInfo();
	// ------------------------------
	// 巡游
	// ------------------------------
	void OnMarryReserveReq(int reserve_index, int marry_level, int rune_index);
	void OnMarryXunyouOpera(int opera_type);
	int GetMarryXunyouMeiguiUseTimes() { return m_activity_info.hunyan_meigui_use_times; }
	int GetMarryXunyouMeiguiBuyTimes() {return m_activity_info.hunyan_meigui_buy_times; }
	void AddMarryXunyouMeiguiUseTimes(int times);
	void OnBuyXunyouOpera(int op_type);
	void SendXunyouOperaInfo();

	// ------------------------------
	// 约会
	// ------------------------------
	void OnDatingInviteReq(const UserID &target_user_id, bool is_specific_invite);
	void OnDatingInviteCheckTarget(const UserID &req_usr_id, bool is_specific_invite);
	void OnDatingInviteAck(const UserID &req_user_id, bool is_agree);
	void OnDatingConsume(ggprotocal::DatingConsume *dc);
	void OnDatingReward(ggprotocal::DatingReward *dr);
	void OnDatingUpdateAddExp(ggprotocal::DatingUpdateAddExp *duae);
	void OnDatingDaycountCheck(ggprotocal::DatingDaycountCheck *ddc);

	// ------------------------------
	// 幸运转盘
	// ------------------------------
	void OnRALuckyRollOperaReq(int opera_type);
	void OnRALuckyRollRoll();
	void OnRALuckyRollFetchExtralReward();
	void SendRALuckyRollActivityInfo();
	void SendRALuckyRollRollResult(int reward_index);

	// ------------------------------
	// 全服疯狂抢购
	// ------------------------------
	void OnRAServerPanicBuyOperaReq(int opera_type, int param);
	void SendRAServerPanicBuyInfo();
	void OnRAServerPanicBuyReq(int item_idx);

	// ------------------------------
	// 个人疯狂抢购
	// ------------------------------
	void OnRAPersonalPanicBuyOperaReq(int opera_type, int param);
	void SendRAPersonalPanicBuyInfo();
	void OnRAPersonalPanicBuyReq(int item_idx);

	// ------------------------------
	// 单日充值返利
	// ------------------------------
	void OnRADayChongZhiFanLiOperaReq(int opera_type,int param);
	void OnRADayChongZhiFanLiFetchReward(int reward_seq);
	void RACheckDayChongZhiFanLiReward();
	void SendRADayChongZhiFanLiInfo();
	void OnRADayChongZhiFanLiAddChongZhi(long long chongzhi);

	// ------------------------------
	// 单日消费
	// ------------------------------
	void OnRADayConsumeGoldOperaReq(int opera_type,int param);
	void OnRADayConsumeGoldFetchReward(int reward_seq);
	void SendRADayConsumeGoldInfo();
	void OnRADayConsumeGoldAddConsume(long long consume_gold);
	void RACheckDayConsumeGoldReward();

	// ------------------------------
	// 奇珍异宝
	// ------------------------------
	void OnRAChestshopOperaReq(int opera_type,int param);
	void SendRAChestshopInfo();
	void OnRAChestshopFetchReward(int reward_seq);
	void RAChestshopAddXunbaoTimes(int times);

	// ------------------------------
	// 单日活跃奖励
	// ------------------------------
	void OnRADayActiveDegreeOperaReq(int opera_type,int param);
	void OnRADayActiveDegreeFetchReward(int reward_seq);
	void SendRADayActiveDegreeInfo();
	void OnRADayActiveDegreeActiveDegreeChange();
	void RACheckDayActiveDegreeGiveReward();
	void SetRADayActivityDegree(int degree);

	// ------------------------------
	// 宝石升级
	// ------------------------------
	void OnRAStoneUplevelOperaReq(int opera_type, int param);
	void SendRAStoneUplevelInfo();
	void OnRAStoneUplevelFetchReward(int reward_seq);
	void CheckRAStoneUplevelReward();

	// ------------------------------
	// 坐骑进阶
	// ------------------------------
	void OnRAMountUpgradeOperaReq(int opera_type, int param);
	void SendRAMountUpgradeInfo();
	void OnRAMountUpgradeFetchReward(int reward_seq);
	void CheckRAMountUpgradeReward();

	// ------------------------------
	// 根骨全身等级
	// ------------------------------
	void OnRAMentalityTotalLevelOperaReq(int opera_type, int param);
	void SendRAMentalityTotalLevelInfo();
	void OnRAMentalityTotalLevelFetchReward(int reward_seq);
	void CheckRAMentalityTotalLevelReward();

	// ------------------------------
	// 羽翼进化
	// ------------------------------
	void OnRAWingUpgradeOperaReq(int opera_type, int param);
	void SendRAWingUpgradeInfo();
	void OnRAWingUpgradeFetchReward(int reward_seq);
	void CheckRAWingUpgradeReward();

	// ------------------------------
	// Boss击杀
	// ------------------------------
	void OnRAKillBossOperaReq(int opera_type);
	void OnRAKillBossFetchReward();
	void SendRAKillBossInfo();
	void OnRAKillBossCountChange();

	// ------------------------------
	// 全民祈福
	// ------------------------------
	void OnRAQuanminQifuOperaReq(int opera_type, int param);
	void SendRAQuanminQifuInfo();
	void OnRAQuanminQifuFetchReward(int reward_seq);
	void RAQuanminQifuAddQifuTimes(int qifu_times);

	// ------------------------------
	// 手有余香
	// ------------------------------
	void OnRAShouYouYuXiangOperaReq(int opera_type, int param);
	void OnRAShouYouYuXiangFetchReward(int reward_seq);
	void SendRAShouYouYuXiangInfo();
	void OnRAShouYouYuXiangGiveFlower(int flower_num);
	void OnRAShouYouYuXiangOnDayChange();

	// ------------------------------
	// 比拼各种战力
	// ------------------------------
	void OnRABipinSendForenotice();
	void OnRABipinCapTotalOperaReq(int opera_type, int param);
	void OnRABipinCapEquipOperaReq(int opera_type, int param);
	void OnRABipinCapXianNvOperaReq(int opera_type, int param);
	void OnRABipinCapJingMaiOperaReq(int opera_type, int param);
	void OnRABipinCapMountOperaReq(int opera_type, int param);
	void OnRABipinCapWingOperaReq(int opera_type, int param);
	void SendRABipinCapabilityInfo(int activity_type);
	void OnRABipinCapTotalFetchReward(int seq);
	void OnRABipinCapEquipFetchReward(int seq);
	void OnRABipinCapXianNvFetchReward(int seq);
	void OnRABipinCapJingmaiFetchReward(int seq);
	void OnRABipinCapWingFetchReward(int seq);
	void OnRABipinCapMountFetchReward(int seq);
	void OnRACappabilityChange(int cap_type);
	void OnRABipinCapShenzhuangOperaReq(int opera_type, int param);
	void OnRABipinCapJinglingOperaReq(int opera_type, int param);
	void OnRABipinCapWashlOperaReq(int opera_type, int param);
	void OnRABipinCapShenzhuangFetchReward(int seq);
	void OnRABipinCapJinglingFetchReward(int seq);
	void OnRABipinCapWashlFetchReward(int seq);

	void GmResetBipinData();
	void GetBipinTopUserInfo(int rank_type, int max_user, Protocol::TopUserInfo user_list[Protocol::TOP_COUNT]);

	// ------------------------------
	// 充值回馈
	// ------------------------------
	void OnRAChargeRepaymentOperaReq(int opera_type, int param);
	void OnRAChargeRepaymentFetchReward(int seq);
	void OnRAChargeRewardActive(long long gold_num);
	void SendChargeRepaymentInfo();

	// ------------------------------
	// 聚宝盆
	// ------------------------------
	void OnRACornucopiaOperaReq(int opera_type);
	void SendRACornucopiaInfo();
	void OnRAFetchCornucopiaReward();
	void OnRAFetchCornucopiaRewardInfo();

	// ------------------------------
	// 新聚宝盆
	// ------------------------------
	void OnRANewCornucopiaOperaReq(int opera_type, int param);
	void SendRANewCornucopiaInfo();
	void OnRAFetchNewCornucopiaReward(int seq);
	void NewCornucopiaUpdateTaskInfo();
	void OnAttackWorldBoss();
	void OnCompleteDailyTask(int complete_times);
	void OnCompleteGuildTask();
	void OnEnterCrossTeamFb();
	void OnAcceptHusongTask();
	void OnEnterCross1v1();
	void OnEnterPhaseFB();
	void GmSetNewCornucopiaTaskValue(int cornucopia_task_type, int value);

	// ------------------------------
	// 单笔充值（不同天数奖励不同）
	// ------------------------------
	void OnRADanbiChongzhiOperaReq(int opera_type, int param);
	void OnRADanbiChongzhiCheckChongzhi(int chongzhi);
	void SendRADanbiChongzhiInfo();
	void OnRADanbiChongzhiFetchReward(int seq);

	// ------------------------------
	// 累计充值（天数）活动
	// ------------------------------
	void OnRATotalChargeDayOperaReq(int opera_type, int param);
	void OnRATotalChargeDayCheckChongzhi(long long chongzhi);
	void SendRATotalChargeDayInfo();
	void OnRATotalChargeDayFetchReward(int seq);

	// ------------------------------
	// 次日福利活动
	// ------------------------------
	void OnRATomorrwRewardOperaReq(int opera_type);
	void SendRATomorrowRewardInfo(int reward_index = 0);
	void OnRATomorrowRewardFetchReward();

	// ------------------------------
	// 充值双倍奖励
	// ------------------------------
	void OnRADoubleCharge(long long gold_num);

	// ------------------------------
	// 充值排行
	// ------------------------------
	void OnRAChongzhiRankChongzhi(int gold_num);
	void SendRAChongzhiRankInfo();
	int GetRAChongzhiRankChongzhiNum() { return m_activity_info.ra_chongzhi_rank_chongzhi_num; }
	unsigned int GetRAChongzhiRankChongzhiTime() { return m_activity_info.ra_chongzhi_rank_chongzhi_last_timestamp; }

	// ------------------------------
	// 充值排行2
	// ------------------------------
	void OnRAChongzhiRank2Chongzhi(int gold_num);
	void SendRAChongzhiRank2Info();
	int GetRAChongzhiRank2ChongzhiNum() { return m_activity_info.ra_chongzhi_rank2_chongzhi_num; }
	unsigned int GetRAChongzhiRank2ChongzhiTime() { return m_activity_info.ra_chongzhi_rank2_chongzhi_last_timestamp; }

	// ------------------------------
	// 消费排行
	// ------------------------------
	void OnRAConsumeGoldRankConsumeGold(int consume_gold);
	void SendRAConsumeGoldRankInfo();
	int GetRAConsumeGoldRankConsumeGoldNum() { return m_activity_info.ra_consume_gold_rank_consume_gold_num; }
	unsigned int GetRAConsumeGoldRankConsumeGoldConsumeTime(){ return m_activity_info.ra_consume_gold_rank_consume_last_timestamp; }

	// ------------------------------
	// 每日消费排行
	// ------------------------------
	void OnRADayConsumeRankConsumeGold(int gold_num);
	void SendRADayConsumeRankInfo();
	void RAResetDayConsumeRankGoldNum();
	int GetRADayConsumeRankGoldNum() { return m_activity_info.ra_day_consume_rank_gold_num; }
	int GetRADayConsumeRankLastConsumeTimestamp() { return m_activity_info.ra_day_consume_rank_consume_last_timestamp; }

	// ------------------------------
	// 消费返利
	// ------------------------------
	void OnRAConsumeGoldFanliConsumeGold(int consume_gold);
	void SendRAConsumeGoldFanliInfo();

	// ------------------------------
	// 登录奖励
	// ------------------------------
	void OnRALoginGiftOperaReq(int opera_type, int param);
	void OnRALoginGiftLogin();
	void SendRALoginGiftInfo();
	void OnRALoginGiftFetchCommonReward(int seq);
	void OnRALoginGiftFetchVipReward(int seq);
	void OnRALoginGiftFetchAccumulateReward();

	void OnRALoginGiftOperaReq_0(int opera_type, int param);
	void OnRALoginGiftLogin_0();
	void SendRALoginGiftInfo_0();
	void OnRALoginGiftFetchCommonReward_0(int seq);
	void OnRALoginGiftFetchVipReward_0(int seq);

	// 每日好礼
	void OnRAEverydayNiceGiftReq(int opera_type, int param1);
	void OnRAEverydayNiceGiftRecharge(int recharge_gold);		// 累计充值额度
	void SendRAEverydayNiceGiftInfo();
	void OnRAEverydayNiceGiftFetchReward(int seq);				// 领取奖励
	void OnRAEverydayNiceGiftReissueReward();					// 补发
	void OnRAEverydayNiceGiftGiveRewardOnDayChange();			// 切天时补发奖励

	// ------------------------------
	// 仙盟比拼
	// ------------------------------
	void SendRAXianMengBiPinInfo();

	// ------------------------------
	// 仙盟崛起
	// ------------------------------
	void SendRAXianMengJueQiInfo();

	// ------------------------------
	// 累计消费
	// ------------------------------
	void OnRATotalConsumeGoldOperaReq(int opera_type,int param);
	void OnRATotalConsumeGoldFetchReward(int reward_seq);
	void SendRATotalConsumeGoldInfo();
	void OnRATotalConsumeGoldAddConsume(long long consume_gold);

	// ------------------------------
	// 单笔充值（充值某个固定额度就给）
	// ------------------------------
	void OnRASingleChargeCheckCharge(long long gold_num);
	void OnRASingleChargeCheckCharge2(long long gold_num);
	void OnRASingleChargeCheckCharge3(long long gold_num);
	void OnRASingleChargeCheckCharge4(long long gold_num);
	void OnRASingleChargeCheckCharge5(long long gold_num);
	void OnRASingleChargeCheckChargeOne(long long gold_num);
	void SendSingleChargeInfoOne();
	void OnRASingleChargeCheckChargeTwo(long long gold_num);
	void SendSingleChargeInfoTwo();
	void OnRASingleChargeCheckChargeThree(long long gold_num);
	void SendSingleChargeInfoThree();

	// ------------------------------
	// 双倍元宝
	// ------------------------------
	void OnRADoubleGetCheckCharge(long long gold_num);
	void SendRADoubleGetInfo();

	// ------------------------------
	// 累计充值 (区分前7天，7-15天等）
	// ------------------------------
	void OnRATotalChargeOperaReq(int opera_type, int param);
	void OnRATotalChargeCheckChongzhi(int chongzhi);
	void RACheckTotalChargeGiveReward();
	void SendRATotalChargeInfo();
	void OnRATotalChargeFetchReward(int seq);

	// ------------------------------
	// 每日充值排行榜
	// ------------------------------
	void OnRADayChongzhiRankAddChongzhi(int gold_num);
	void SendRADayChongzhiRankInfo();
	void RAResetDayChongzhiRankGoldNum();
	int GetRADayChongzhiRankGoldNum() { return m_activity_info.ra_day_chongzhi_rank_gold_num; }
	int GetRADayChongzhiRankLastChongzhiTimestamp() { return m_activity_info.ra_day_chongzhi_rank_chongzhi_last_timestamp; }

	//--------------------------------
	// 限时积分兑换装备
	//--------------------------------
	void OnRATimeLimitExchangeEquiOperaReq(int oper_type, int seq);
	void SendRATimeLimitExchangeEquiInfo();

	void OnRATimeLimitExchangeJLOperaReq(int oper_type, int seq);
	void SendRATimeLimitExchangeJLInfo();

	void RAResetExchangTimes();

	// ------------------------------
	// 装备强化
	// ------------------------------
	void CheckRAEquipStrengthenReward(int strengthen_level);

	//--------------------------------
	// 金银塔抽奖活动
	//--------------------------------
	void OnRALevelLotteryOperaReq(int opera_type, int param);
	void SendRALevelLotteryInfo();
	void OnRALevelLotteryDoLottery(int times);
	void OnRALevelLotteryFetchTotalTimesReward(int times);
	void SendRALevelLotteryActivityInfo();

	//-------------------------------
	// 充值扭蛋
	//-------------------------------
	void OnRAChongZhiNiuEggOperaReq(int opera_type, int param);
	void SendRANiuEggInfo();
	void OnRANiuEggChou(int times);
	void OnRANiuEggFetchReward(int seq);

	//-------------------------------
	// 珍宝阁
	//-------------------------------
	void OnRaZhenbaogeOperReq(int opera, int param);
	void SendRaZhenbaogeInfo();
	void OnRaZhenbaogeBuy(int index);
	void OnRaZhenbaogeBuyAll();
	bool OnRaZhenbaogeFlush(bool is_free, bool is_rare_flush);
	void OnRaZhenbaogeRareFlush();
	void OnRazhenbaogeFetchReward(int seq);
	void OnRaZhenbaogeCheckAllBuyGift();
	void OnRaZhenbaogeAddWeight();
	void OnRaZhenbaogeCheckResetWeight();
	void OnRaZhenbageResetflushtimestamp() { m_activity_info.ra_zhenbaoge_next_free_flush_timestamp = 0; }

	//-------------------------------
	// 秘境寻宝
	//-------------------------------

	void OnRaMiJingXunBaoOperReq(int opera, int param);										// 秘境寻宝活动请求
	void OnRAMiJingXunBaoTao(int type);
	void OnRAMiJingXunBaoFetchPersonalReward(int seq);
	void SendRAMiJingXunBaoInfo();

	//-------------------------------
	// 急速冲战
	//-------------------------------
	void OnRaJiSuChongZhanGiveReward(const int charge_value);

	//-------------------------------
	// 摇钱树
	//-------------------------------
	void OnRAChongzhiMoneyTreeOperReq(int opera_type, int param);							// 摇钱树活动请求
	void SendRaMoneyTreeInfo();
	void OnRaMoneyTreeChou(int times);
	void OnRaMoneyTreeFetchReward(int seq);

	//-------------------------------
	// 每日爱一次
	//-------------------------------
	void OnRADailyLoveChongzhi(const int charge_value);
	void OnSendDailyLoveChongzhiInfo();

	//-------------------------------
	// 陛下请翻牌
	//-------------------------------
	void OnRAKingDrawOperaReq(int opera_type, int param1, int param2);						// 陛下请翻牌活动请求
	void SendKingDrawInfo();
	void OnKingDrawPlayOnce(int level, int index);
	void OnKingDrawPlayTimes(int level, int times);
	void OnKingDrawRefreshCard(int level);
	void OnKingDrawFetchReward(int level, int seq);
	void KingDrawAddWeight(int level);
	void KingDrawResetWeight(int level);

	//-------------------------------
	// 累计充值3 -- 三件套
	//-------------------------------
	void OnRATotalCharge3CheckChongzhi(int chongzhi);
	void RACheckTotalCharge3RewardCheck();
	void OnSendTotalCharge3Info();

	//-------------------------------
	// 变身排行榜
	//-------------------------------
	void OnPassiveUseSpecialAppearanceCard();
	void OnUseSpecialAppearanceCard();
	void SendRASpecialAppearancePassiveInfo();
	void SendRASpecialAppearanceInfo();
	void RACheckSpecialAppearacePassiveJoinReward();
	void RACheckSpecialAppearaceJoinReward();

	//-------------------------------
	// 全场怪物掉落
	//-------------------------------
	void OnKillAnyMonster(UInt16 monster_id);
	bool OnUseAnyMonsterDropActivityItem(ItemID item_id, int num);

	//-------------------------------
	// 累计充值4(新三件套)
	//-------------------------------
	void RACheckTotalCharge4RewardCheck();
	void OnRATotalCharge4CheckChongzhi(int chongzhi);
	void OnSendTotalCharge4Info();

	//-------------------------------
	// 趣味挖矿
	//-------------------------------
	void OnRAMineOperaReq(int opera_type, int param1);
	void SendRAMineAllInfo();
	void OnRAMineRefresh(bool is_use_gold = true);
	void OnRAGatherMine(int index);
	void OnRAMineFetchReward(int index);
	void OnRAMineExchangeReward(int index);

	//-------------------------------
	// 刮刮乐
	//-------------------------------
	void OnRAGuaGuaOperaReq(int opera_type, int param1);									// 顶刮刮活动请求
	void SendGuaGuaInfo();
	void OnGuaGuaPlayTimes(int type);
	void OnGuaGuaFetchReward(unsigned int reward_index);												// 领取累计抽奖奖励

	//-------------------------------
	// 天命卜卦
	//-------------------------------
	void OnRATianMingDivinationOperaReq(int opera_type, int param1);						// 天命卜卦活动请求
	int  GetRATianMingDivinationTotalAddLotTimes();					// 计算竹签总加注次数
	void SendRATianMingDivinationActivityInfo();		
	void OnRATianMingDivinationAddLotTimes(int lot_index);
	void OnRATianMingDivinationResetAddLotTimes();
	void OnRATianMingDivinationStartChou();
	void SendRATianMingDivinationStartChouResult(int reward_index);


	//-------------------------------
	// 翻翻转
	//-------------------------------
	void OnRAFanFanOperaReq(int opera_type, int param1, int param2);									// 翻翻转活动请求
	void SendRAFanFanAllInfo();
	void OnRAFanFanPlayOnce(int index);
	void OnRAFanFanGetAll();
	//void OnRAFanFanRefresh(bool is_use_gold);
	bool OnRAFanFanRefresh(bool is_use_gold, int target = -1, int *is_get_target = nullptr);
	void OnRAFanFanWordExchange(int index);
	void OnRAFanFanLeichouExchange(int index);

	//-------------------------------
	// 连续充值
	//-------------------------------
	void OnRAContinueChongzhiOperaReq(int opera_type, int param1);							// 连续充值活动请求	
	void OnRAContinueChongzhiAddChongzhi(int chongzhi);
	void SendRAContinueChongzhiActivityInfo();
	void OnRAContinueChongzhiFetchReward(int day_index);
	void OnRAContinueChongzhiFetchExtraReward();

	//-------------------------------
	// 连充特惠初
	//-------------------------------
	void OnRAContinueChongzhiOperaReqChu(int opera_type, int param1);							// 连续充值活动请求	
	void OnRAContinueChongzhiAddChongzhiChu(int chongzhi);
	void SendRAContinueChongzhiActivityInfoChu();
	void OnRAContinueChongzhiFetchRewardChu(int day_index);
	void OnRAContinueChongzhiChuFetchExtraReward();

	//-------------------------------
	// 连充特惠高
	//-------------------------------
	void OnRAContinueChongzhiOperaReqGao(int opera_type, int param1);							// 连续充值活动请求	
	void OnRAContinueChongzhiAddChongzhiGao(int chongzhi);
	void SendRAContinueChongzhiActivityInfoGao();
	void OnRAContinueChongzhiFetchRewardGao(int day_index);
	void OnRAContinueChongzhiGaoFetchExtraReward();

	//-------------------------------
	// 限时连充
	//-------------------------------
	void OnRAContinueChongzhiOperaReq2(int opera_type, int param1);							// 限时连充	
	void OnRAContinueChongzhiAddChongzhi2(int chongzhi);
	void SendRAContinueChongzhiActivityInfo2();
	void OnRAContinueChongzhiFetchReward2(int day_index);
	void OnRAContinueChongzhi2FetchExtraReward();

	//-------------------------------
	// 连续消费
	//-------------------------------
	void OnRAContinueConsumeOperaReq(int opera_type, int param1);							// 连续消费活动请求
	void OnRAContinueConsumeConsumeGold(int consume_gold);
	void SendRAContinueConsumeActivityInfo();
	void OnRAContinueConsumeFetchTodayReward();
	void OnRAContinueConsumeFetchExtraReward();

	//-------------------------------
	// 军歌嘹亮
	//-------------------------------
	void OnRaArmyDayOperaReq(int opera_type, int param1);
	void SendArmyDayInfo();
	void OnRaArmyDayExchangeFlag(int seq);
	void OnRaArmyDayExchangeItem(int army_side);
	void OnRaArmyDayCalBelongSide();

	//-------------------------------
	// 循环充值
	//-------------------------------
	void OnRACirculationChongzhiOperaReq(int opera_type, int param1);						// 循环充值活动请求
	void OnRACirculationChongzhiChongzhiGold(int chongzhi_gold);
	void SendRACirculationChongzhiActivityInfo();
	void OnRACirculationChongzhiFetchReward(int reward_seq);

	//-------------------------------
	// 至尊幸运
	//-------------------------------
	void OnRAExtremeLuckyOperaReq(int opera_type, int param1);
	void SendRAExtremeLuckyAllInfo();
	void SendRAExtremeLuckySingleInfo(int index);
	void OnRAExtremeLuckyFlush(bool is_free);				// 元宝刷新或时间到自动刷新	
	void OnRAExtremeLuckyForceFlush();						
	void ExtremeLuckyCheckAndFlush();						// 抽完9次自动刷
	void OnRAExtremeLuckyDraw();
	void RAExtremeLuckyCheckReturnReward(int seq);

	//-------------------------------
	// 秘境寻宝2
	//-------------------------------
	void OnRaMiJingXunBao2OperReq(int opera, int param);										// 秘境寻宝活动请求
	void OnRAMiJingXunBao2Tao(int type);
	void SendRAMiJingXunBao2Info();

	//-------------------------------
	// 摇钱树2
	//-------------------------------
	void OnRAChongzhiMoneyTree2OperReq(int opera_type, int param);							// 摇钱树活动请求
	void SendRaMoneyTree2Info();
	void OnRaMoneyTree2Chou(int times);
	void OnRaMoneyTree2FetchReward(int seq);

	//-------------------------------
	//欢度国庆
	//-------------------------------
	void OnRaNationalDayOpeReq(int oprea, int param);
	void OnRaNationalDayExchange(int seq);													// 欢度国庆兑换奖品

	//-------------------------------
	// 珍宝阁2
	//-------------------------------
	void OnRaZhenbaoge2OperReq(int opera, int param);
	void SendRaZhenbaoge2Info();
	void OnRaZhenbaoge2Buy(int index);
	void OnRaZhenbaoge2BuyAll();
	bool OnRaZhenbaoge2Flush(bool is_free, bool is_rare_flush);
	void OnRaZhenbaoge2RareFlush();
	void OnRazhenbaoge2FetchReward(int seq);
	void OnRaZhenbaoge2CheckAllBuyGift();
	void OnRaZhenbaoge2AddWeight();
	void OnRaZhenbaoge2CheckResetWeight();
	void OnRaZhenbage2Resetflushtimestamp() { m_activity_info.ra_zhenbaoge2_next_free_flush_timestamp = 0; }

	//-------------------------------
	// 步步高升
	//-------------------------------
	void OnRAPromotingPositionOperaReq(int opera_type, int param1);							// 步步高升活动请求
	void SendRAPromotingPositionAllInfo();
	void OnRAPromotingPositionPlay(int times);
	void RAPromotingPositionGiveRewardHelper(int seq, int circle_type, RAPromotingPositionRewardInfo reward_info_list[RA_PROMOTION_POSITION_MULTIPLAY_TIMES * 2], int &reward_count);
	void OnRAPromotingPositionChongzhiGold(int chongzhi_gold);
	void OnRAPromotingPositionFetchReward(int seq);

	//-------------------------------
	// 黑市竞拍
	//-------------------------------
	void OnRABlackMarketOperaReq(int opera_type, int param1, int param2);					// 黑市拍卖活动请求
	void SendRABlackMarketAllInfo();
	void OnRABlackMarketOfferItem(int index, int price);

	//-------------------------------
	// 珍宝商城
	//-------------------------------
	void OnRaTreasuresMallOperaReq(int opera_type, int param1, int param2);
	void OnRaTreasuresMallBuy(int item_index);
	void OnRaTreasuresMallExchange(int exchange_index);
	void SendTreasuresMallAllInfo();

	//-------------------------------
	// 开服活动
	//-------------------------------
	void OnRAOpenServerOperaReq(int ra_type, int opera_type, int param1, int param2);
	void SendRAOpenServerInfo(int ra_type, int reward_flag);
	void SendRAOpenServerSingleInfo(int ra_type);
	void SendRAOpenServerBossInfo();
	void SendOpenGameGiftShopBuyInfo();
	void SendOpenGameGiftShopBuy2Info();	//限购礼包2
	void SendRAOpenServerBattleInfo();
	bool RACheckOpenServerFetchCond(int ra_type, const RandActivityOpenServerRewardItemConfig *cfg);
	void RACheckOpenServerGiveReweard(int ra_type);
	void RAActivityOpenNotification(int ra_type);							//活动开启邮件通知
	void RACheckActivityOpenNotification();									//检查开服活动开启邮件通知
	void RACheckOpenServerBossKillerGiveReweard(int ra_type);

	bool RACheckOpenServerUpgradeCompleteFlag(const RandActivityOpenServerRewardItemConfig *cfg);
	bool RACheckOpenServerUpgradeGroupCompleteFlag(const RandActivityOpenServerRewardItemConfig *cfg);

	// 集字
	void OnRAItemCollectionOperaReq(int opera_type, int param1);
	void OnRAItemCollectionExchange(int seq);

	// 经验炼制
	void OnRaRefineOperaReq(int opera_type);
	void OnRARefineExpReq();
	void OnRAFetchRewardGold();
	void SendRAExpRefineInfo();

	// 红包好礼
	void OnRARedEnvelopeGiftConsumeGold(int consume_gold);
	void FetchRedEnvelopeGiftDiamond(int day_index);
	void SendRARedEnvelopeGiftInfo();

	//我们结婚吧
	void OnRAMarryMeOperaReq(int opera_type);
	void SendRAMarryMeAllInfo();
	void OnMarry(Role *target);

	// 元宝转盘
	void AddTurntableScore(long long consume_gold);
	void SendGoldTurntableInfo();
	void GoldTurntableChouJiang();

	// 金猪召唤
	void OnRAGoldenPigOperateReq(int opera_type, int param);
	void SendRAGoldenPigSummonInfo();
	void RAGoldenPigSendBossState();
	void OnRASummonGoldenPigReq(int param);
	void OnKillGoldenPigBoss(int summon_type);

	// 开服投资
	void OnRAOpenServerInvestOperateReq(int opera_type, int param1, int param2);
	void SendRAOpenServerInvestInfo();
	void OnRAInvest(int type);
	void OnRAOpenServerInvestFetchReward(int type, int seq);
	void AddOpenServerInvestFinishParam(int type, int param, bool err_notice = true);

	// 单返豪礼
	void OnRASingleChongZhiOperaReq(int opera_type, int param1);
	void OnRASingleChongZhiFetchReward(int seq);
	void OnRASingleChongZhiGiveReward();
	void SendSingleChongZhiAllInfo();
	void OnRASingleChongZhiAddChongZhi(int gold_num);

	// 聚划算
	void OnRaXianyuanTreasOperReq(int opera, int param);
	void SendRaXianyuanTreasInfo();
	void OnRaXianyuanTreasBuy(int seq);
	void OnRaXianyuanTreasBuyAll();
	void OnRaXianyuanTreasFetchReward(int seq);
	void OnRaXianyuanTreasFetchAllBuyGift();

	// 限时秒杀
	void OnRARushBuyingOperaReq(int opera_type, int param1);
	void SendRARushBuyingAllInfo();
	void OnRARushBuyingBuyItem(int index);

	//------------------------------------------
	// 地图寻宝
	//------------------------------------------
	void OnRaMapHuntOperaReq(int opera_type, int param1, int param2);
	void SendRAMapHuntInfo(int extern_reward_route_id = 0);
	bool OnRaMapHuntFlush(bool is_free, bool &is_get, const int city_id = -1);
	bool OnRaMapHuntQuicklyFlush(const int city_id, bool &is_get);
	void OnRaMapHuntXunbao(const int city_id);												// 寻宝
	void OnRaMapHuntFetchReturnReward(int index);

	//-------------------------------
	// 灯塔寻宝
	//-------------------------------
	void OnRaLightTowerExploreOperaReq(int opera_type, int param1);
	void OnRALightTowerExploreDraw(int times);
	void OnRALightTowerExploreFetchExternReward(int layer);
	void OnRALightTowerExploreFetchReturnReward(int index);
	bool CheckCanFetchLightTowerExploreExternReward(int layer);
	void SendLightTowerExploreInfo();

	// ------------------------------
	// 新累计充值
	// ------------------------------
	void OnRANewTotalChargeOperaReq(int opera_type, int param);
	void OnRANewTotalChargeCheckChongzhi(int chongzhi);
	void RACheckNewTotalChargeGiveReward();
	void SendRANewTotalChargeInfo();
	void OnRANewTotalChargeFetchReward(int seq);

	// ------------------------------
	// 新累计充值1
	// ------------------------------
	void OnRANewTotalCharge1OperaReq(int opera_type, int param);
	void OnRANewTotalChargeCheckChongzhi1(int chongzhi);
	void RACheckNewTotalCharge1GiveReward();
	void SendRANewTotalCharge1Info();
	void OnRANewTotalCharge1FetchReward(int seq);

	// ------------------------------
	// 新累计充值2
	// ------------------------------
	void OnRANewTotalCharge2OperaReq(int opera_type, int param);
	void OnRANewTotalChargeCheckChongzhi2(int chongzhi);
	void RACheckNewTotalCharge2GiveReward();
	void SendRANewTotalCharge2Info();
	void OnRANewTotalCharge2FetchReward(int seq);

	// ------------------------------
	// 新累计充值3
	// ------------------------------
	void OnRANewTotalCharge3OperaReq(int opera_type, int param);
	void OnRANewTotalChargeCheckChongzhi3(int chongzhi);
	void RACheckNewTotalCharge3GiveReward();
	void SendRANewTotalCharge3Info();
	void OnRANewTotalCharge3FetchReward(int seq);

	// ------------------------------
	// 幻装商店
	// ------------------------------
	void OnRAMagicShopOperaReq(int opera_type, int param_1, int param_2);
	void OnRAMagicShopBuyOpera(int type, int index);
	void OnRAMagicShopBuyOne(int index);
	void OnRAMagicShopBuyAll();
	void SendRAMagicShopAllInfo();

	// ------------------------------
	// 趣味钓鱼
	// ------------------------------
	void OnRAFishingOperaReq(int opera_type, int param1);
	void OnFishingReq(int param1);			//请求钓鱼
	void FishingAck();						//钓鱼请求返回
	void StealFishReq(int uid);				//请求偷鱼
	void StealFishAck(int uid);
	void OnExchangeFishReward(int reward_type);
	void CancelAutoFishing();				//取消自动钓鱼
	void ExitFishing();						//退出钓鱼
	void SendAllFishingInfo(short fish_type, short steal_fish_type);
	short GetFishingscore();
	short GetStealfishscore();
	unsigned int GetFishingFishingTimestamp();
	unsigned int GetFishingStealTimestamp();
	char GetFishingStation();

	// 战场乱斗
	void OnMessBattleEnter();

	// ------------------------------
	// 植树
	// ------------------------------
	int GetRAPlantingTreePlantingTimes() { return m_activity_info.ra_planting_tree_planting_times; }
	int GetRAPlantingTreeWateringTimes() { return m_activity_info.ra_planting_tree_watering_times; }
	void SendRAPlantingTreeRankInfo(int rank_type);
	void SendRAPlantingTreeTreeInfo(int scene_id, ObjID obj_id);
	void SendRAPlantingTreeMiniMapInfo(int scene_id);
	void OnRAPlantingTreeOperaReq(int opera_type, int param1, int param2);					// 植树活动请求
	bool OnRAPlantingTreePlanting(ItemID item_id);
	void OnRAPlantingTreeWatering(int gather_id, ObjID obj_id);
	bool CanRAPlantingTreeWatering(int gather_id, int obj_id);
	void RACheckPlantingTreeCanyuReward();

	// ------------------------------
	// 达拉然水晶
	// ------------------------------
	void OnRACrystalDalaranOperaReq(int opera_type, int prama1);
	void StealCrystalDalaran(int uid);
	const int GetStealTimesByUid(int uid);
	void SetBeStealInfoList(int uid);
	void SendStealCrystalDalaranInfo();
	unsigned int GetCrytstalDalaranCount() { return m_activity_info.ra_crystal_dalaran_count; }
	unsigned int GetCrystalDalaranStealCount() { return m_activity_info.ra_steal_dalaran_count; }
	unsigned int GetCrystalDalaranBeStealedCount() { return m_activity_info.ra_dalaran_be_stealed_count; }
	unsigned int GetCrystalDalaranStealTimestamp() { return m_activity_info.ra_steal_crystal_dalaran_times; }
	unsigned int GetCrystalDalaranBeStealedTimestamp() { return m_activity_info.ra_crystal_dalaran_be_stealed_times;  }
	void OnCrystalDalaranChange(int change_val, const char *reason);

	//-----------------------------
	// 狂返元宝
	//-----------------------------
	void OnRAChongzhiCrazyRebate(long long gold_num);
	void SendRaCrazyRebateChongzhiInfo();

	//------------------------------------------
	// 天尊卡             废弃
	//------------------------------------------
	void OnFairyBuddhaCardActivateReq(int card_type);
	void SendFairyBuddhaCardInfo();
	void OnFairyBuddhaCardUpdate(unsigned int now_second);
	int GetFairyBuddhaCardExtraExpPer();
	int GetFairyBuddhaCardExtraMoJingPer();
	void OnFairyBuddhaCardGetBindGoldReq(int card_type);
	bool IsFairyBuddhaActive(int card_type);
	void FairyBuddhaCardOnDayChange();

	// 钓鱼活动
	// --------------------------------------------------------------------------------------------------------
	void SyncFishingGearNumList(int gear_num_list[FISHING_GEAR_MAX_COUNT], bool is_init = false);
	int GetFishingScore();
	void AddFishingScore(int score);
	bool DecFishingScore(int score);
	void SendFishingScoreInfo();

	// ------------------------------
	// 消费领奖
	// ------------------------------
	void OnRAConsumeGiftConsumeGold(int consume_gold);
	void OnRAConsumeGiftOpera(int param1, int param2);
	void SendRAConsumeGiftInfo();
	void OnRAConsumeGiftFetchReward(int param);
	void OnRAConsumeGiftRoll();       // 开始摇奖
	void OnRAConsumeGiftRollTen();       // 开始摇奖十次
	void OnRAConsumeGiftRollReward(); // 领取奖励
	void OnRAConsumeGiftRollRewardTen(); // 领取十次奖励

	// ------------------------------
	// 每日限购
	// ------------------------------
	void OnRADailyLimitBuyOpera(int param1, int param2);
	void OnRADailyLimitBuyBuy(int seq);
	void SendRADailyLimitBuyInfo();

	// ------------------------------
	// 聚宝盆（新）
	// ------------------------------
	void OnRACollectTreasureOperaReq(int param1, int param2);
	void OnRACollectTreasureChongZhi(int chongzhi);
	void SendRACollectTreasureInfo();
	void OnRACollectTreasureRoll();
	void OnRACollectTreasureReward();

	// ------------------------------
	// 欢乐累充
	// ------------------------------
	void OnRAHappyCumulChongzhiGold(int chongzhi_gold);
	void OnRAHappyCumulChongzhiOpera(int param1, int param2);
	void SendRAHappyCumulChongzhiInfo();
	void OnRAHappyCumulChongzhiReward(int param);

	// ------------------------------
	// 暴击日
	// ------------------------------
	void OnRACriticalStrikeOperaReq();
	int GetCriticalStrikeDayMul(int type);       // 暴击日  暴击倍数  
	int GetRACriticalStrikeDayRate(int type);       // 暴击日  暴击几率，万分比 
	
	// ------------------------------
	// 进阶返还
	// ------------------------------
	void OnRaJinJieOpera(int type, short grade,short old_grade);
	void OnRAJinJieReturnOpera(int param1, int param2);
	void SendRAJinJieReturnInfo();
	void OnRAJinJieReturnReward(int param);
	void CheckRAJinJieOldAllReward(int theme,int old_grade);
	int JinJieActGetGradeByTheme(int theme);
	static int JinJieActThemeGetByUpgrade(int upgrade_type);

	//------------------------------------------
	// 全民进阶
	//------------------------------------------
	void OnQuanMinJinJie(int opear_type, int seq);
	bool OnFetchUpgradeReward(int seq);
	void SendQuanMinJinJieInfo(int seq);
	int GetPersonalGrade();

	//------------------------------------------
	// 全民总动员
	//------------------------------------------
	void OnUpgradeGroupe(int opear_type, int seq);
	bool OnFetchUpgradeGroupeReward(int seq);
	void SendUpgradeGroupeInfo(int type);

	//----------------------------------
	// 限时回馈
	//----------------------------------
	void SendLimitTimeRebateInfo();
	void OnRALimitTimeRebateOperaReq(int opera_type, int param);
	void OnRaLimitTimeRebateFetchReward(int seq);
	void OnRaLimitTimeRebateAddchongzhi(int chongzhi_count);

	//-----------------------------
	// 限时礼包
	//-----------------------------
	void OnRATimeLimitGiftCheckChongzhi(int chongzhi);
	void OnRATimeLimitGiftOperaReq(int opera_type, int param, int param2);
	void OnRATimeLimitGiftFetchReward(int seq, int fetsh_seq);
	void SendRATimeLimitGiftInfo();

	// ------------------------------
	//循环充值2
	// ------------------------------
	void OnRACirculationChongzhiSecondOperaReq(int opera_type);
	void OnRACirculationChongzhiSecondAddchongzhi(int chongzhi_gold);
	void SendCirculationChongzhiSecondInfo();
	void OnCirculationChongzhiSecondFetchReward();

	// ------------------------------
	// 疯狂摇钱树
	// ------------------------------
	void OnRAShakeMoneyOperaReq(int opera_type, int param);
	void OnRAShakeMoneyCheckChongzhi(int chongzhi);
	void OnRAShakeMoneyFetchGold();
	void SendRAShakeMoneyInfo();

	// ------------------------------
	// 限时豪礼（购买）
	// ------------------------------
	void OnRATimeLimitLuxuryGiftBagOperaReq(int opera_type, int param);
	void OnRATimeLimitLuxuryGiftBagBuy(int seq);
	void SendRATimeLimitLuxuryGiftBagInfo();

	// ------------------------------
	//普天同庆
	// ------------------------------
	void OnRADoubleChongzhiOpera(int opera_type);
	void OnRADoubleChongzhiChongzhi(int chongzhi);
	void SendRADoubleChongzhiInfo();

	// ------------------------------
	// RMB购买
	// ------------------------------
	void OnRARmbBugChestShopReq();
	bool OnRARmbBugChestShopChongzhi(const int chongzhi_num);
	void SendRARmbBugChestShopInfo();

	// ------------------------------
	// 消费返利
	// ------------------------------
	void OnRAConsumeGoldRewardOpera(int opera_type);
	void OnRAConsumeGoldRewardConsumeGold(int consume_gold);
	void SendRAConsumeGoldRewardInfo();
	void OnConsumeGoldRewardFetchReward();

	// ------------------------------
	// 集字2
	// ------------------------------
	void OnRAItemCollectionSecondOperaReq(int opera_type, int param1);
	void OnRAItemCollectionSecondExchange(int seq);
	void SendCollectSecondExchangeInfo();

	// ------------------------------
	// 版本累计充值
	// ------------------------------
	void OnRAVersionTotalChargeOperaReq(int opera_type, int param);
	void OnRAVersionTotalChargeCheckChongzhi(int chongzhi);
	void SendRAVersionTotalChargeInfo();
	void OnRAVersionTotalChargeFetchReward(int seq);

	// ------------------------------
	// 版本连续充值
	// ------------------------------
	void OnRAVersionContinueGhargeOperaReq(int opera_type, int param1);
	void OnRAVersionContinueChargeAddChongzhi(int chongzhi);
	void SendRAVersionContinueChargeActivityInfo();
	void OnRAVersionContinueChargeFetchReward(int day_index);

	// ------------------------------
	// 欢乐摇奖2
	// ------------------------------
	void OnRaHuanLeYaoJiang2OperReq(int opera_type, int param);
	void OnRAHuanLeYaoJiang2Tao(int type);
	void SendRAHuanLeYaoJiang2Info();
	void OnRAHuanLeYaoJiang2FetchPersonalReward(int seq);

	// ------------------------------
	// 线下单笔充值0
	// ------------------------------
	void SendRAOfflineSingleChargeInfo();
	void OnRAOfflineSingleChargeChongZhi(int chongzhi_num);

	// --------------------------------------------------------------------------------------------------------
	// 消费送礼
	// --------------------------------------------------------------------------------------------------------
	void OnRAConsumeForGiftOperaReq(int opera_type, int param1);
	void SendRAConsumeForGiftAllInfo();
	void OnRAConsumeForGiftExchangeItem(int index);
	void OnRAConsumeForGiftConsumeGold(int consume_gold);

	// 秘境寻宝3 
	void OnRaMiJingXunBao3OperReq(int opera_type, int param);
	void OnRAMiJingXunBao3Tao(int type);
	void SendRAMiJingXunBao3Info();
	void ReissueMiJingXunBao3FetchPersonalReward();
	void OnRAMiJingXunBao3FetchPersonalReward(int seq);

	// 欢乐砸蛋
	void OnRaHuanLeZaDanOperReq(int opera_type, int param);
	void OnRAHuanLeZaDanTao(int type);
	void SendRAHuanLeZaDanInfo();
	void ReissueHuanLeZaDanFetchPersonalReward();
	void OnRAHuanLeZaDanFetchPersonalReward(int seq);

	// 欢乐摇奖
	void OnRaHuanLeYaoJiangOperReq(int opera_type, int param);
	void OnRAHuanLeYaoJiangTao(int type);
	void SendRAHuanLeYaoJiangInfo();
	void ReissueHuanLeYaoJiangFetchPersonalReward();
	void OnRAHuanLeYaoJiangFetchPersonalReward(int seq);

	//-----------------------------
	// 买一送一
	//-----------------------------
	void OnRABuyOneGetOneFreeOperaReq(int opera_type, int param);
	void SendRABuyOneGetOneFreeInfo();
	void RABuyOneGetOneFreeBuy(int item_index);
	void RABuyOneGetOneFreeFetchReward(int item_index);

	// 累计充值5(版本吉祥三宝)
	void RACheckTotalCharge5RewardCheck();
	void OnRATotalCharge5CheckChongzhi(int chongzhi);
	void OnSendTotalCharge5Info();

	// 极限挑战
	void OnRAExtremeChallengeReq(int opera_type, int param1);
	void OnRAExtremeChallengeRefreshTask(int task_id);            // 换一个任务
	void OnRAExtremeChallengeRefreshTask();                       
	void OnRAExtremeChallengeFetchReward(int task_id);
	void OnRAExtremeChallengeFetchUltimateReward();
	void OnRAExtremeChallengeAddPlan(int opera_type, int param1, int param2 = RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2);
	void SendRAExtremeChallengeTaskInfo();

	// 你充我送
	void OnRAChongZhiGiftOperaReq(int opera_type, int param_1, int param_2);
	void OnRAChongZhiGiftFetchOpera(int index);
	void SendRAChongZhiGiftAllInfo();
	void RACheckChongZhiGiftGiveReward();
	void OnRAChongZhiGiftCheckChongzhi(int chongzhi);

	//进阶返还2
	void OnRAJinJieReturn2Opera(int param1, int param2);
	void SendRAJinJieReturn2Info();
	void OnRAJinJieReturn2Reward(int param);
	void CheckRAJinJieOldAllReward2(int theme, int old_grade);
	// 暴击日2
	void OnRACriticalStrike2OperaReq();

	// 组合购买
	void OnRACombineBuyOpera(int opera_type, int param_0);
	void OnRACombineBuyAddInBucket(int seq);
	void OnRACombineBuyRemoveBucket(int seq);
	void OnRACombineBuyItem();
	void SendRACombineBuyBucketInfo();
	void SendRACombineBuyItemInfo();

	// 累计天数充值(天天返利)
	void OnDayChongzhiRewardAddChongzhi();
	void OnDayChongzhiRewardOpera(short opera_type, short param_0);
	void OnFetchDayChongzhiReward(int seq);
	void OnFetchDayChongzhiRareReward(int seq);
	void SendDayChongzhiRewardInfo();

	// 幸运许愿
	void OnRALuckyWishOpera(int opera_type, int param_1);			//客户端请求
	void SendRALuckyWishInfo(const Protocol::SCLuckyWishInfo &info);//发协议
	void OnRALuckyWishResetLuckyValue();							//重设幸运值
	void OnRALuckyWishAddLuckyValue(int value);						//增加幸运值
	int GetRALuckyWishValue();										//获取幸运值
	bool OnRALuckyWishIsLucky();									//是否达到满幸运值
	bool OnRALuckyWishLottery(int repeat_times);					//开始许愿

	//------------------------------------------
	// 周末狂欢活动
	//------------------------------------------
	void OnLottery1OperaReq(int opera_type, int param1);
	void OnLottery1ChouReq(int chou_type);
	void OnFetchLottery1PersonRewardReq(int seq);
	void SendLottery1Info(const RALottery1ChouItem *chou_item_list, int size, int count);

	// ------------------------------
	// 零元购
	// ------------------------------
	void OnRAZeroBuyReturnOperaReq(int opera_type, int param1);
	void OnRAZeroBuyReturnOperaBuy(int buy_type);
	void OnRAZeroBuyReturnOperaFecth(int buy_type);
	void SendRAZeroBuyReturnInfo();

	// ------------------------------
	// 狂嗨庆典
	// ------------------------------
	void SendKuangHaiInfo();
	void KuangHaiGetReward(int reward_list_seq);
	void OnKuangHaiCommonOpera(int opera_type, int param_1);
	void OnKuangHaiQingDianRecore(int task_type, int param);
	void CheckRAKuangHaiQingDianGiveReward();

	// ------------------------------
	// 礼物收割
	// ------------------------------
	int GetRAGiftHarvestScore() { return m_activity_info.ra_gift_harvest_score; }
	int GetRAGiftHarvestGetScoreTimes() { return m_activity_info.ra_gift_harvest_get_score_times; }
	void RAGiftHarvestAddScore(int score) { m_activity_info.ra_gift_harvest_score += score;  m_activity_info.ra_gift_harvest_get_score_times++; }
	void OnEnterGiftHarvestScene();
	void OnGiftHarvestReq(int opera_type);
	void CheckRAGiftHarvestGiveReward();

	// --------------------------------------------------------------------------------------------------------
	// 随机活动结束时补发奖励
	// --------------------------------------------------------------------------------------------------------
	void RACheckBipinCapTotalGiveReweard();
	void RACheckBipinCapEquipGiveReweard();
	void RACheckBipinCapXiaNvGiveReweard();
	void RACheckBipinCapJingMaiGiveReweard();
	void RACheckBipinCapMountGiveReweard();
	void RACheckBipinCapWingGiveReweard();
	void RACheckKillBossReward();
	void RACheckConsumeGoldFanliGiveReward();
	void RACheckTotalConsumeGoldGiveReward();
	void RACheckQuanminQifuGiveReward();
	void RACheckChestshopGiveReward();
	void RACheckTotalChargeDayGiveReward();
	void RACheckLoginGiftGiveReward();
	void RACheckLoginGiftGiveReward_0();
	void RACheckStoneTotalLevelGiveReward();
	void RACheckMountUpgradeGiveReward();
	void RACheckMentalityTotalLevelGiveReward();
	void RACheckWingUpgradeGiveReward();
	void RACheckBipinCapShenzhuangGiveReweard();
	void RACheckBipinCapJinglingGiveReweard();
	void RACheckBipinCapWashGiveReweard();
	void RACheckFanFanGiveReward();
	void RACheckContinueChongzhiGiveReward();
	void RACheckContinueChongzhiGiveRewardChu();
	void RACheckContinueChongzhiGiveRewardGao();
	void RACheckContinueChongzhiGiveReward2();
	void RACheckContinueConsumeGiveReward(bool is_activity_over = true);
	void OnRaCheckArmyDayWinnerGiveReward();
	void RACheckCirculationChongzhiGiveReward();
	void RACheckXianyuanTreasMailReward();
	void RACheckKingdrawReturnReward();
	void RACheckLevelLotteryReward();
	void RACheckZhenbaogeReward();
	void RACheckMoneyTreeReward();
	void RACheckConsumGiftReward();
	void RACheckHappyCumulChongzhiReward();
	void RACheckLimitTimeRebateReward();
	void RACheckTimeLimitGiftReward();
	void RACheckCirculationChongzhiSecondReward();	// 这个在跨天的时候也要给奖励，在这里处理一下
	void RACheckShakeMoneyGiveReward();
	void RACheckConsumeGoldRewardAutoFetch();
	void RACheckVersionTotalChargeGiveReward();
	void RACheckVersionContinueChongzhiGiveReward();
	void RACheckHuanLeYaoJiang2FetchPersonalReward();
	void CheckRAOfflineSingleChargeReward0();
	void RACheckBuyOneGetOneFreeReward();
	void RACheckGuaGuaAccReward();
	void OnRAExtremeChallengeGiveReward();					// 极限挑战 补发奖励
	void RACheckChangeRepayment();

	// 开服活动
	void RACheckRoleUplevelGiveReweard();
	void RACheckPataGiveReweard();
	void RACheckExpFbGiveReweard();
	void RACheckUpgradeMountGiveReweard();
	void RACheckUpgradeHaloGiveReweard();
	void RACheckUpgradeWingGiveReweard();
	void RACheckUpgradeShengongGiveReweard();
	void RACheckUpgradeShenyiGiveReweard();
	void RACheckFirstChargeTuanUpgradeGiveReweard();
	void RACheckDayTotalChargeUpgradeGiveReweard();
	void RACheckMountUpgradeTotalGiveReweard();
	void RACheckHaloUpgradeTotalGiveReweard();
	void RACheckWingUpgradeTotalGiveReweard();
	void RACheckShengongUpgradeTotalGiveReweard();
	void RACheckShenyiUpgradeTotalGiveReweard();
	void RACheckMountUpgradeRankGiveReweard();
	void RACheckQilinbiUpgradeRankGiveReweard();
	void RACheckToushiUpgradeRankGiveReweard();
	void RACheckYaoshiUpgradeRankGiveReweard();
	void RACheckHaloUpgradeRankGiveReweard();
	void RACheckWingUpgradeRankGiveReweard();
	void RACheckShengongUpgradeRankGiveReweard();
	void RACheckShenyiUpgradeRankGiveReweard();
	void RACheckLingtongUpgradeRankGiveReweard();
	void RACheckLinggongUpgradeRankGiveReweard();
	void RACheckLingqiUpgradeRankGiveReweard();
	void RACheckEquipStrengthTotalLevelGiveReweard();
	void RACheckStoneTotalLevelGiveReweard();
	void RACheckEquipStrengthTotalLevelRankGiveReweard();
	void RACheckStoneTotalLevelRankGiveReweard();
	void RACheckBossKillerGiveReweard();
	void RACheckXianNvTotalRankGiveReweard();
	void RACheckJingLingTotalRankGiveReweard();
	void RACheckFightMountTotalRankGiveReweard();
	void RACheckRoleTotalCapRankGiveReweard();

	//新增时装、神兵、法宝、足迹
	void RACheckShizhuangUpgradeRankGiveReweard();
	void RACheckShenbinUpgradeRankGiveReweard();
	void RACheckFabaoUpgradeRankGiveReweard();
	void RACheckFootprintUpgradeRankGiveReweard();
	void RACheckFlypetUpgradeRankGiveReweard();
	void RACheckWeiyanUpgradeRankGiveReweard();

	void AddXingzuoYijiGatherBox(int type);

	// --------------------------------------------------------------------------------------------------------
	// 其他
	// --------------------------------------------------------------------------------------------------------
	void SendServerTime();
	void SendUpdateNoticeInfo();
	void OnFetchUpdateNoticeReward();

	void OnConsumeGold(long long consume_gold);
	void OnConsumeCoin(long long consume_coin);
	void OnMentalityGenguLevelUp();
	void OnMentalityBaseLevelUp();
	void OnPutonEquip();
	void OnStrengthenEquip(int equip_index, int old_level, int to_level);
	void OnChestShopXunBao(int count, bool is_free);
	void OnStoneUplevel(int stone_slot, int old_level, int to_level);
	void OnStrengthenMount(int strength_level);
	void OnStrengthenShiZhuang(int strength_level);
	void OnEquipUpLevel();
	void OnRoleUpgradeLevel(int ole_level, int level);
	void OnMountUpgrade(int old_grade, int to_grade);
	void OnWingUpgrade(int old_grade, int to_grade);
	void OnVipQifu(int qifu_type, int qifu_times);
	void OnJoinGuild(GuildID guild_id);

	void OnActiveDegreeComplete(int active_degree_type);
	void OnKillWorldBoss(Role *role);
	void OnKillBoss(int boss_id);
	void OnCombineServerKillBossActivity(int boss_id, int boss_level);
	void OnAddCombineKillBossCountInHidden();
	void OnSyncCombineKillBossCount(int count);
	void OnGiveFlower(int flower_num);
	void OnChangeCapability(int cap_type);
	void OnGetTitleOwnerInfo();

	// 节日守护
	void OnHolidayGuardKillMonsterGiveReward();
	void OnHolidayGuardAddKillMonsterCount(int kill_count);

	// 角色上线需要发送随机活动信息入口
	void SendRandActivityInfo();

	// 表白排行
	void OnRAProfessAddScore(int score, bool is_initiative);			// 记录积分
	void SendRAProfessRankInfo();
	int GetRAProfessScore() { return m_activity_info.ra_profess_score; }
	int GetRAProfessToNum() { return m_activity_info.ra_profess_to_num; }
	int GetRAProfessFromNum() { return m_activity_info.ra_profess_from_num; }
	unsigned int GetRAProfessTime() { return m_activity_info.ra_profess_rank_last_profess_timestamp; }

	// add by wwd
	void CheckRandActivity(int rand_act_type, bool is_force = false);

	void OnCheckCrossConsumeGold(long long consume_gold);						// 检查跨服里面消费
	void OnCheckCrossDailyConsumeGold(long long daily_consume_gold);			// 检查跨服里面每日消费

	void OnRALotteryPutChestStore(int activity_type, int list_num, ChestItemInfo item_list[MODE_MAX_COUNT], const char *reason);	// 用于进寻宝仓库的抽奖类活动

	void FetchTodayThemeReward(int seq);
	void SendFetchTodayThemeRewardInfo();

	// 每日一爱2
	void OnRADailyLove2Chongzhi(const int chongzhi_num);
	void OnSendDailyLove2ChongzhiInfo();

	// 幸运云购
	void OnOpenLuckyCloudBuyPanel(unsigned int timestamp);
	void OnCloseLuckyCloudBuyPanel();
	void SendLuckyCloudBuyInfo();
	bool LuckyCloudBuy();
	void OnLuckyCloudBuyOperaReq(int opera_type, int param1);
	void CheckRALuckyCloudBuyReturn();	// 检查是否有未返还的元宝
	void ResetLuckyCloudBuy();			// 重置云购个人数据
	//狂欢大乐购
	void OnRACracyBuyOperaReq(int opera_type, int seq);
	void OnRAACracyBuySendItemLimitMessage();
	void OnRAACracyBuySendTakePartInMessage();
	void OnRAACracyBuyToBuy(int seq);
	void OnRACracyBuyChongZhi(int money);

private:
	void RandActivityOnRoleLogin();
	void CheckInitRandActivity(int rand_act_type, bool is_force = false);

	Role *m_role;
	CharIntAttrs m_attrs_add;																// 各属性加成数值
	int m_temp_zhanzhang_side;																// 战场那一边 仅仅用户判定敌我关系使用 不保存
	RoleActivityParam m_activity_info;
	int m_xianmengzhan_defend_area;															// 仙盟战 - 守卫据点(不保存)
	bool m_is_first_update;																	// 是否第一次update
	int m_yizhandaodi_jisha_count;															// 一站到底 击杀数（不保存）

	bool m_is_auto_fishing;																	// 是否自动钓鱼
	bool m_is_fishing;																		// 是否在钓鱼状态
	ItemID m_csa_roll_item_id;																// 合服转盘roll到的物品id
};

#define RAND_CHECKINIT_AND_GIVE_REWRAD(rand_activity_type, give_rewared_fun)\
{\
	this->CheckInitRandActivity(rand_activity_type);\
	if (!ActivityShadow::Instance().IsRandActivityOpen(rand_activity_type))\
	{\
		this->give_rewared_fun();\
	}\
}

#define TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(rand_activity_type, as, give_rewared_fun)\
{\
	if (rand_activity_type == as->activity_type && ACTIVITY_STATUS_CLOSE == as->status)\
	{\
		for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)\
		{\
			Scene *scene = *i;\
			if (NULL == scene) continue;\
			for (int j = 0; j < (int)scene->RoleNum(); ++ j)\
			{\
				Role *temp_role = scene->GetRoleByIndex(j);\
				if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())\
				{\
					temp_role->GetRoleActivity()->give_rewared_fun();\
				}\
			}\
		}\
	}\
}\

#endif // __ROLE_ACTIVITY_HPP__


