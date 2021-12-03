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
	void SyncFirstRechargeBuffInfo();							//�׳�buff���(��buff)

	// --------------------------------------------------------------------------------------------------------
	// �����
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

	// ҹս����
	void OnNightFightEnter();
	// ˮ���
	void ResetShuijingData();

	// --------------------------------------------------------------------------------------------------------
	// ���
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
	// �Ϸ��
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
	void OnCSAOperaReq(int sub_type, int param_1, int param_2);  // ��������
	void OnCSALoginGiftLogin();
	void OnCSABossEnter();
	void SendCSABossInfo();
	void SendCSABossRankInfo();
	void SendCSABossRoleInfo();

	//����ս
	void OnAddCSAGongChengZhanWinTimes();
	void SendCSAGongChengZhanInfo();

	// Ͷ�ʼƻ�
	void OnCSATouzijihuaOpera(int opera_type, int param);
	bool OnCSATouzijihuaBuy(int chongzhi_num);
	void OnCSATouzijihuaFetch(int index);
	void CheckTouzijihuaFetch();
	void SendCSATouzijihuaInfo();

	// �ɳ�����
	void OnCSAFoundationOpera(int opera_type, int param);
	bool OnCSAFoundationBuy(int chongzhi_num);
	void OnCSAFoundationFetch(int index);
	void CheckFoundationFetch();
	void SendCSAFoundationInfo();

	// ��������
	void OnCSARefineOperaReq(int opera_type, int param);
	void OnCSARefineExpReq(int seq);
	void SendCSAExpRefineInfo();

	// ��鲹���Ϸ�����
	void CheckGiveCombineReward();

	// --------------------------------------------------------------------------------------------------------
	// �����
	// --------------------------------------------------------------------------------------------------------
	void SendOpenGameActivityInfo();
	void OnFetchOpenGameActivityReward(int reward_type, int reward_seq);
	bool OnFetchOGAPutonEquipmentReward(int seq);
	bool OnFetchOGACapabilityReward(int seq);
	bool OnFetchOGARoleLevelReward(int seq);
	void OnOGABuyEquipmentGift(int seq);
	void SendBaiBeiFanLiInfo();
	void OnBaiBeiFanLiBuy();
	void SendBaiBeiFanLi2Info();	//�ٱ�����2 ��Ϣ
	void OnBaiBeiFanLi2Buy();		//�ٱ�����2 ����
	void OnFetchOpenGameKillBossReward(int seq);
	void OnOpenGameGiftShopBuy(int seq);
	void OnOpenGameGiftShopBuy2(int seq);	//����޹�2

	// ------------------------------
	// ��������
	// ------------------------------
	bool OnRAPerfectMarry(int marry_type, int lover_uid);
	void PerfectLoverCheckAndSendInfo();
	void Brocast();
	void SendPerfectLoverInfo();

	// --------------------------------------------------------------------------------------------------------
	// �������ԣ��������
	// --------------------------------------------------------------------------------------------------------
	void OnNewGuildBattle(int opera_type);
	void OnGuidBattleResultUpdate();
	bool OnFetchOGATotalGuidBattleReward();
	void OnOGACheckGuildBattleReward();

	// --------------------------------------------------------------------------------------------------------
	// ��ֵ
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
	// ����
	// --------------------------------------------------------------------------------------------------------

	void SendCollectExchangeInfo();
	// ------------------------------
	// Ѳ��
	// ------------------------------
	void OnMarryReserveReq(int reserve_index, int marry_level, int rune_index);
	void OnMarryXunyouOpera(int opera_type);
	int GetMarryXunyouMeiguiUseTimes() { return m_activity_info.hunyan_meigui_use_times; }
	int GetMarryXunyouMeiguiBuyTimes() {return m_activity_info.hunyan_meigui_buy_times; }
	void AddMarryXunyouMeiguiUseTimes(int times);
	void OnBuyXunyouOpera(int op_type);
	void SendXunyouOperaInfo();

	// ------------------------------
	// Լ��
	// ------------------------------
	void OnDatingInviteReq(const UserID &target_user_id, bool is_specific_invite);
	void OnDatingInviteCheckTarget(const UserID &req_usr_id, bool is_specific_invite);
	void OnDatingInviteAck(const UserID &req_user_id, bool is_agree);
	void OnDatingConsume(ggprotocal::DatingConsume *dc);
	void OnDatingReward(ggprotocal::DatingReward *dr);
	void OnDatingUpdateAddExp(ggprotocal::DatingUpdateAddExp *duae);
	void OnDatingDaycountCheck(ggprotocal::DatingDaycountCheck *ddc);

	// ------------------------------
	// ����ת��
	// ------------------------------
	void OnRALuckyRollOperaReq(int opera_type);
	void OnRALuckyRollRoll();
	void OnRALuckyRollFetchExtralReward();
	void SendRALuckyRollActivityInfo();
	void SendRALuckyRollRollResult(int reward_index);

	// ------------------------------
	// ȫ���������
	// ------------------------------
	void OnRAServerPanicBuyOperaReq(int opera_type, int param);
	void SendRAServerPanicBuyInfo();
	void OnRAServerPanicBuyReq(int item_idx);

	// ------------------------------
	// ���˷������
	// ------------------------------
	void OnRAPersonalPanicBuyOperaReq(int opera_type, int param);
	void SendRAPersonalPanicBuyInfo();
	void OnRAPersonalPanicBuyReq(int item_idx);

	// ------------------------------
	// ���ճ�ֵ����
	// ------------------------------
	void OnRADayChongZhiFanLiOperaReq(int opera_type,int param);
	void OnRADayChongZhiFanLiFetchReward(int reward_seq);
	void RACheckDayChongZhiFanLiReward();
	void SendRADayChongZhiFanLiInfo();
	void OnRADayChongZhiFanLiAddChongZhi(long long chongzhi);

	// ------------------------------
	// ��������
	// ------------------------------
	void OnRADayConsumeGoldOperaReq(int opera_type,int param);
	void OnRADayConsumeGoldFetchReward(int reward_seq);
	void SendRADayConsumeGoldInfo();
	void OnRADayConsumeGoldAddConsume(long long consume_gold);
	void RACheckDayConsumeGoldReward();

	// ------------------------------
	// �����챦
	// ------------------------------
	void OnRAChestshopOperaReq(int opera_type,int param);
	void SendRAChestshopInfo();
	void OnRAChestshopFetchReward(int reward_seq);
	void RAChestshopAddXunbaoTimes(int times);

	// ------------------------------
	// ���ջ�Ծ����
	// ------------------------------
	void OnRADayActiveDegreeOperaReq(int opera_type,int param);
	void OnRADayActiveDegreeFetchReward(int reward_seq);
	void SendRADayActiveDegreeInfo();
	void OnRADayActiveDegreeActiveDegreeChange();
	void RACheckDayActiveDegreeGiveReward();
	void SetRADayActivityDegree(int degree);

	// ------------------------------
	// ��ʯ����
	// ------------------------------
	void OnRAStoneUplevelOperaReq(int opera_type, int param);
	void SendRAStoneUplevelInfo();
	void OnRAStoneUplevelFetchReward(int reward_seq);
	void CheckRAStoneUplevelReward();

	// ------------------------------
	// �������
	// ------------------------------
	void OnRAMountUpgradeOperaReq(int opera_type, int param);
	void SendRAMountUpgradeInfo();
	void OnRAMountUpgradeFetchReward(int reward_seq);
	void CheckRAMountUpgradeReward();

	// ------------------------------
	// ����ȫ��ȼ�
	// ------------------------------
	void OnRAMentalityTotalLevelOperaReq(int opera_type, int param);
	void SendRAMentalityTotalLevelInfo();
	void OnRAMentalityTotalLevelFetchReward(int reward_seq);
	void CheckRAMentalityTotalLevelReward();

	// ------------------------------
	// �������
	// ------------------------------
	void OnRAWingUpgradeOperaReq(int opera_type, int param);
	void SendRAWingUpgradeInfo();
	void OnRAWingUpgradeFetchReward(int reward_seq);
	void CheckRAWingUpgradeReward();

	// ------------------------------
	// Boss��ɱ
	// ------------------------------
	void OnRAKillBossOperaReq(int opera_type);
	void OnRAKillBossFetchReward();
	void SendRAKillBossInfo();
	void OnRAKillBossCountChange();

	// ------------------------------
	// ȫ����
	// ------------------------------
	void OnRAQuanminQifuOperaReq(int opera_type, int param);
	void SendRAQuanminQifuInfo();
	void OnRAQuanminQifuFetchReward(int reward_seq);
	void RAQuanminQifuAddQifuTimes(int qifu_times);

	// ------------------------------
	// ��������
	// ------------------------------
	void OnRAShouYouYuXiangOperaReq(int opera_type, int param);
	void OnRAShouYouYuXiangFetchReward(int reward_seq);
	void SendRAShouYouYuXiangInfo();
	void OnRAShouYouYuXiangGiveFlower(int flower_num);
	void OnRAShouYouYuXiangOnDayChange();

	// ------------------------------
	// ��ƴ����ս��
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
	// ��ֵ����
	// ------------------------------
	void OnRAChargeRepaymentOperaReq(int opera_type, int param);
	void OnRAChargeRepaymentFetchReward(int seq);
	void OnRAChargeRewardActive(long long gold_num);
	void SendChargeRepaymentInfo();

	// ------------------------------
	// �۱���
	// ------------------------------
	void OnRACornucopiaOperaReq(int opera_type);
	void SendRACornucopiaInfo();
	void OnRAFetchCornucopiaReward();
	void OnRAFetchCornucopiaRewardInfo();

	// ------------------------------
	// �¾۱���
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
	// ���ʳ�ֵ����ͬ����������ͬ��
	// ------------------------------
	void OnRADanbiChongzhiOperaReq(int opera_type, int param);
	void OnRADanbiChongzhiCheckChongzhi(int chongzhi);
	void SendRADanbiChongzhiInfo();
	void OnRADanbiChongzhiFetchReward(int seq);

	// ------------------------------
	// �ۼƳ�ֵ���������
	// ------------------------------
	void OnRATotalChargeDayOperaReq(int opera_type, int param);
	void OnRATotalChargeDayCheckChongzhi(long long chongzhi);
	void SendRATotalChargeDayInfo();
	void OnRATotalChargeDayFetchReward(int seq);

	// ------------------------------
	// ���ո����
	// ------------------------------
	void OnRATomorrwRewardOperaReq(int opera_type);
	void SendRATomorrowRewardInfo(int reward_index = 0);
	void OnRATomorrowRewardFetchReward();

	// ------------------------------
	// ��ֵ˫������
	// ------------------------------
	void OnRADoubleCharge(long long gold_num);

	// ------------------------------
	// ��ֵ����
	// ------------------------------
	void OnRAChongzhiRankChongzhi(int gold_num);
	void SendRAChongzhiRankInfo();
	int GetRAChongzhiRankChongzhiNum() { return m_activity_info.ra_chongzhi_rank_chongzhi_num; }
	unsigned int GetRAChongzhiRankChongzhiTime() { return m_activity_info.ra_chongzhi_rank_chongzhi_last_timestamp; }

	// ------------------------------
	// ��ֵ����2
	// ------------------------------
	void OnRAChongzhiRank2Chongzhi(int gold_num);
	void SendRAChongzhiRank2Info();
	int GetRAChongzhiRank2ChongzhiNum() { return m_activity_info.ra_chongzhi_rank2_chongzhi_num; }
	unsigned int GetRAChongzhiRank2ChongzhiTime() { return m_activity_info.ra_chongzhi_rank2_chongzhi_last_timestamp; }

	// ------------------------------
	// ��������
	// ------------------------------
	void OnRAConsumeGoldRankConsumeGold(int consume_gold);
	void SendRAConsumeGoldRankInfo();
	int GetRAConsumeGoldRankConsumeGoldNum() { return m_activity_info.ra_consume_gold_rank_consume_gold_num; }
	unsigned int GetRAConsumeGoldRankConsumeGoldConsumeTime(){ return m_activity_info.ra_consume_gold_rank_consume_last_timestamp; }

	// ------------------------------
	// ÿ����������
	// ------------------------------
	void OnRADayConsumeRankConsumeGold(int gold_num);
	void SendRADayConsumeRankInfo();
	void RAResetDayConsumeRankGoldNum();
	int GetRADayConsumeRankGoldNum() { return m_activity_info.ra_day_consume_rank_gold_num; }
	int GetRADayConsumeRankLastConsumeTimestamp() { return m_activity_info.ra_day_consume_rank_consume_last_timestamp; }

	// ------------------------------
	// ���ѷ���
	// ------------------------------
	void OnRAConsumeGoldFanliConsumeGold(int consume_gold);
	void SendRAConsumeGoldFanliInfo();

	// ------------------------------
	// ��¼����
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

	// ÿ�պ���
	void OnRAEverydayNiceGiftReq(int opera_type, int param1);
	void OnRAEverydayNiceGiftRecharge(int recharge_gold);		// �ۼƳ�ֵ���
	void SendRAEverydayNiceGiftInfo();
	void OnRAEverydayNiceGiftFetchReward(int seq);				// ��ȡ����
	void OnRAEverydayNiceGiftReissueReward();					// ����
	void OnRAEverydayNiceGiftGiveRewardOnDayChange();			// ����ʱ��������

	// ------------------------------
	// ���˱�ƴ
	// ------------------------------
	void SendRAXianMengBiPinInfo();

	// ------------------------------
	// ��������
	// ------------------------------
	void SendRAXianMengJueQiInfo();

	// ------------------------------
	// �ۼ�����
	// ------------------------------
	void OnRATotalConsumeGoldOperaReq(int opera_type,int param);
	void OnRATotalConsumeGoldFetchReward(int reward_seq);
	void SendRATotalConsumeGoldInfo();
	void OnRATotalConsumeGoldAddConsume(long long consume_gold);

	// ------------------------------
	// ���ʳ�ֵ����ֵĳ���̶���Ⱦ͸���
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
	// ˫��Ԫ��
	// ------------------------------
	void OnRADoubleGetCheckCharge(long long gold_num);
	void SendRADoubleGetInfo();

	// ------------------------------
	// �ۼƳ�ֵ (����ǰ7�죬7-15��ȣ�
	// ------------------------------
	void OnRATotalChargeOperaReq(int opera_type, int param);
	void OnRATotalChargeCheckChongzhi(int chongzhi);
	void RACheckTotalChargeGiveReward();
	void SendRATotalChargeInfo();
	void OnRATotalChargeFetchReward(int seq);

	// ------------------------------
	// ÿ�ճ�ֵ���а�
	// ------------------------------
	void OnRADayChongzhiRankAddChongzhi(int gold_num);
	void SendRADayChongzhiRankInfo();
	void RAResetDayChongzhiRankGoldNum();
	int GetRADayChongzhiRankGoldNum() { return m_activity_info.ra_day_chongzhi_rank_gold_num; }
	int GetRADayChongzhiRankLastChongzhiTimestamp() { return m_activity_info.ra_day_chongzhi_rank_chongzhi_last_timestamp; }

	//--------------------------------
	// ��ʱ���ֶһ�װ��
	//--------------------------------
	void OnRATimeLimitExchangeEquiOperaReq(int oper_type, int seq);
	void SendRATimeLimitExchangeEquiInfo();

	void OnRATimeLimitExchangeJLOperaReq(int oper_type, int seq);
	void SendRATimeLimitExchangeJLInfo();

	void RAResetExchangTimes();

	// ------------------------------
	// װ��ǿ��
	// ------------------------------
	void CheckRAEquipStrengthenReward(int strengthen_level);

	//--------------------------------
	// �������齱�
	//--------------------------------
	void OnRALevelLotteryOperaReq(int opera_type, int param);
	void SendRALevelLotteryInfo();
	void OnRALevelLotteryDoLottery(int times);
	void OnRALevelLotteryFetchTotalTimesReward(int times);
	void SendRALevelLotteryActivityInfo();

	//-------------------------------
	// ��ֵŤ��
	//-------------------------------
	void OnRAChongZhiNiuEggOperaReq(int opera_type, int param);
	void SendRANiuEggInfo();
	void OnRANiuEggChou(int times);
	void OnRANiuEggFetchReward(int seq);

	//-------------------------------
	// �䱦��
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
	// �ؾ�Ѱ��
	//-------------------------------

	void OnRaMiJingXunBaoOperReq(int opera, int param);										// �ؾ�Ѱ�������
	void OnRAMiJingXunBaoTao(int type);
	void OnRAMiJingXunBaoFetchPersonalReward(int seq);
	void SendRAMiJingXunBaoInfo();

	//-------------------------------
	// ���ٳ�ս
	//-------------------------------
	void OnRaJiSuChongZhanGiveReward(const int charge_value);

	//-------------------------------
	// ҡǮ��
	//-------------------------------
	void OnRAChongzhiMoneyTreeOperReq(int opera_type, int param);							// ҡǮ�������
	void SendRaMoneyTreeInfo();
	void OnRaMoneyTreeChou(int times);
	void OnRaMoneyTreeFetchReward(int seq);

	//-------------------------------
	// ÿ�հ�һ��
	//-------------------------------
	void OnRADailyLoveChongzhi(const int charge_value);
	void OnSendDailyLoveChongzhiInfo();

	//-------------------------------
	// �����뷭��
	//-------------------------------
	void OnRAKingDrawOperaReq(int opera_type, int param1, int param2);						// �����뷭�ƻ����
	void SendKingDrawInfo();
	void OnKingDrawPlayOnce(int level, int index);
	void OnKingDrawPlayTimes(int level, int times);
	void OnKingDrawRefreshCard(int level);
	void OnKingDrawFetchReward(int level, int seq);
	void KingDrawAddWeight(int level);
	void KingDrawResetWeight(int level);

	//-------------------------------
	// �ۼƳ�ֵ3 -- ������
	//-------------------------------
	void OnRATotalCharge3CheckChongzhi(int chongzhi);
	void RACheckTotalCharge3RewardCheck();
	void OnSendTotalCharge3Info();

	//-------------------------------
	// �������а�
	//-------------------------------
	void OnPassiveUseSpecialAppearanceCard();
	void OnUseSpecialAppearanceCard();
	void SendRASpecialAppearancePassiveInfo();
	void SendRASpecialAppearanceInfo();
	void RACheckSpecialAppearacePassiveJoinReward();
	void RACheckSpecialAppearaceJoinReward();

	//-------------------------------
	// ȫ���������
	//-------------------------------
	void OnKillAnyMonster(UInt16 monster_id);
	bool OnUseAnyMonsterDropActivityItem(ItemID item_id, int num);

	//-------------------------------
	// �ۼƳ�ֵ4(��������)
	//-------------------------------
	void RACheckTotalCharge4RewardCheck();
	void OnRATotalCharge4CheckChongzhi(int chongzhi);
	void OnSendTotalCharge4Info();

	//-------------------------------
	// Ȥζ�ڿ�
	//-------------------------------
	void OnRAMineOperaReq(int opera_type, int param1);
	void SendRAMineAllInfo();
	void OnRAMineRefresh(bool is_use_gold = true);
	void OnRAGatherMine(int index);
	void OnRAMineFetchReward(int index);
	void OnRAMineExchangeReward(int index);

	//-------------------------------
	// �ι���
	//-------------------------------
	void OnRAGuaGuaOperaReq(int opera_type, int param1);									// ���ιλ����
	void SendGuaGuaInfo();
	void OnGuaGuaPlayTimes(int type);
	void OnGuaGuaFetchReward(unsigned int reward_index);												// ��ȡ�ۼƳ齱����

	//-------------------------------
	// ��������
	//-------------------------------
	void OnRATianMingDivinationOperaReq(int opera_type, int param1);						// �������Ի����
	int  GetRATianMingDivinationTotalAddLotTimes();					// ������ǩ�ܼ�ע����
	void SendRATianMingDivinationActivityInfo();		
	void OnRATianMingDivinationAddLotTimes(int lot_index);
	void OnRATianMingDivinationResetAddLotTimes();
	void OnRATianMingDivinationStartChou();
	void SendRATianMingDivinationStartChouResult(int reward_index);


	//-------------------------------
	// ����ת
	//-------------------------------
	void OnRAFanFanOperaReq(int opera_type, int param1, int param2);									// ����ת�����
	void SendRAFanFanAllInfo();
	void OnRAFanFanPlayOnce(int index);
	void OnRAFanFanGetAll();
	//void OnRAFanFanRefresh(bool is_use_gold);
	bool OnRAFanFanRefresh(bool is_use_gold, int target = -1, int *is_get_target = nullptr);
	void OnRAFanFanWordExchange(int index);
	void OnRAFanFanLeichouExchange(int index);

	//-------------------------------
	// ������ֵ
	//-------------------------------
	void OnRAContinueChongzhiOperaReq(int opera_type, int param1);							// ������ֵ�����	
	void OnRAContinueChongzhiAddChongzhi(int chongzhi);
	void SendRAContinueChongzhiActivityInfo();
	void OnRAContinueChongzhiFetchReward(int day_index);
	void OnRAContinueChongzhiFetchExtraReward();

	//-------------------------------
	// �����ػݳ�
	//-------------------------------
	void OnRAContinueChongzhiOperaReqChu(int opera_type, int param1);							// ������ֵ�����	
	void OnRAContinueChongzhiAddChongzhiChu(int chongzhi);
	void SendRAContinueChongzhiActivityInfoChu();
	void OnRAContinueChongzhiFetchRewardChu(int day_index);
	void OnRAContinueChongzhiChuFetchExtraReward();

	//-------------------------------
	// �����ػݸ�
	//-------------------------------
	void OnRAContinueChongzhiOperaReqGao(int opera_type, int param1);							// ������ֵ�����	
	void OnRAContinueChongzhiAddChongzhiGao(int chongzhi);
	void SendRAContinueChongzhiActivityInfoGao();
	void OnRAContinueChongzhiFetchRewardGao(int day_index);
	void OnRAContinueChongzhiGaoFetchExtraReward();

	//-------------------------------
	// ��ʱ����
	//-------------------------------
	void OnRAContinueChongzhiOperaReq2(int opera_type, int param1);							// ��ʱ����	
	void OnRAContinueChongzhiAddChongzhi2(int chongzhi);
	void SendRAContinueChongzhiActivityInfo2();
	void OnRAContinueChongzhiFetchReward2(int day_index);
	void OnRAContinueChongzhi2FetchExtraReward();

	//-------------------------------
	// ��������
	//-------------------------------
	void OnRAContinueConsumeOperaReq(int opera_type, int param1);							// �������ѻ����
	void OnRAContinueConsumeConsumeGold(int consume_gold);
	void SendRAContinueConsumeActivityInfo();
	void OnRAContinueConsumeFetchTodayReward();
	void OnRAContinueConsumeFetchExtraReward();

	//-------------------------------
	// ��������
	//-------------------------------
	void OnRaArmyDayOperaReq(int opera_type, int param1);
	void SendArmyDayInfo();
	void OnRaArmyDayExchangeFlag(int seq);
	void OnRaArmyDayExchangeItem(int army_side);
	void OnRaArmyDayCalBelongSide();

	//-------------------------------
	// ѭ����ֵ
	//-------------------------------
	void OnRACirculationChongzhiOperaReq(int opera_type, int param1);						// ѭ����ֵ�����
	void OnRACirculationChongzhiChongzhiGold(int chongzhi_gold);
	void SendRACirculationChongzhiActivityInfo();
	void OnRACirculationChongzhiFetchReward(int reward_seq);

	//-------------------------------
	// ��������
	//-------------------------------
	void OnRAExtremeLuckyOperaReq(int opera_type, int param1);
	void SendRAExtremeLuckyAllInfo();
	void SendRAExtremeLuckySingleInfo(int index);
	void OnRAExtremeLuckyFlush(bool is_free);				// Ԫ��ˢ�»�ʱ�䵽�Զ�ˢ��	
	void OnRAExtremeLuckyForceFlush();						
	void ExtremeLuckyCheckAndFlush();						// ����9���Զ�ˢ
	void OnRAExtremeLuckyDraw();
	void RAExtremeLuckyCheckReturnReward(int seq);

	//-------------------------------
	// �ؾ�Ѱ��2
	//-------------------------------
	void OnRaMiJingXunBao2OperReq(int opera, int param);										// �ؾ�Ѱ�������
	void OnRAMiJingXunBao2Tao(int type);
	void SendRAMiJingXunBao2Info();

	//-------------------------------
	// ҡǮ��2
	//-------------------------------
	void OnRAChongzhiMoneyTree2OperReq(int opera_type, int param);							// ҡǮ�������
	void SendRaMoneyTree2Info();
	void OnRaMoneyTree2Chou(int times);
	void OnRaMoneyTree2FetchReward(int seq);

	//-------------------------------
	//���ȹ���
	//-------------------------------
	void OnRaNationalDayOpeReq(int oprea, int param);
	void OnRaNationalDayExchange(int seq);													// ���ȹ���һ���Ʒ

	//-------------------------------
	// �䱦��2
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
	// ��������
	//-------------------------------
	void OnRAPromotingPositionOperaReq(int opera_type, int param1);							// �������������
	void SendRAPromotingPositionAllInfo();
	void OnRAPromotingPositionPlay(int times);
	void RAPromotingPositionGiveRewardHelper(int seq, int circle_type, RAPromotingPositionRewardInfo reward_info_list[RA_PROMOTION_POSITION_MULTIPLAY_TIMES * 2], int &reward_count);
	void OnRAPromotingPositionChongzhiGold(int chongzhi_gold);
	void OnRAPromotingPositionFetchReward(int seq);

	//-------------------------------
	// ���о���
	//-------------------------------
	void OnRABlackMarketOperaReq(int opera_type, int param1, int param2);					// �������������
	void SendRABlackMarketAllInfo();
	void OnRABlackMarketOfferItem(int index, int price);

	//-------------------------------
	// �䱦�̳�
	//-------------------------------
	void OnRaTreasuresMallOperaReq(int opera_type, int param1, int param2);
	void OnRaTreasuresMallBuy(int item_index);
	void OnRaTreasuresMallExchange(int exchange_index);
	void SendTreasuresMallAllInfo();

	//-------------------------------
	// �����
	//-------------------------------
	void OnRAOpenServerOperaReq(int ra_type, int opera_type, int param1, int param2);
	void SendRAOpenServerInfo(int ra_type, int reward_flag);
	void SendRAOpenServerSingleInfo(int ra_type);
	void SendRAOpenServerBossInfo();
	void SendOpenGameGiftShopBuyInfo();
	void SendOpenGameGiftShopBuy2Info();	//�޹����2
	void SendRAOpenServerBattleInfo();
	bool RACheckOpenServerFetchCond(int ra_type, const RandActivityOpenServerRewardItemConfig *cfg);
	void RACheckOpenServerGiveReweard(int ra_type);
	void RAActivityOpenNotification(int ra_type);							//������ʼ�֪ͨ
	void RACheckActivityOpenNotification();									//��鿪��������ʼ�֪ͨ
	void RACheckOpenServerBossKillerGiveReweard(int ra_type);

	bool RACheckOpenServerUpgradeCompleteFlag(const RandActivityOpenServerRewardItemConfig *cfg);
	bool RACheckOpenServerUpgradeGroupCompleteFlag(const RandActivityOpenServerRewardItemConfig *cfg);

	// ����
	void OnRAItemCollectionOperaReq(int opera_type, int param1);
	void OnRAItemCollectionExchange(int seq);

	// ��������
	void OnRaRefineOperaReq(int opera_type);
	void OnRARefineExpReq();
	void OnRAFetchRewardGold();
	void SendRAExpRefineInfo();

	// �������
	void OnRARedEnvelopeGiftConsumeGold(int consume_gold);
	void FetchRedEnvelopeGiftDiamond(int day_index);
	void SendRARedEnvelopeGiftInfo();

	//���ǽ���
	void OnRAMarryMeOperaReq(int opera_type);
	void SendRAMarryMeAllInfo();
	void OnMarry(Role *target);

	// Ԫ��ת��
	void AddTurntableScore(long long consume_gold);
	void SendGoldTurntableInfo();
	void GoldTurntableChouJiang();

	// �����ٻ�
	void OnRAGoldenPigOperateReq(int opera_type, int param);
	void SendRAGoldenPigSummonInfo();
	void RAGoldenPigSendBossState();
	void OnRASummonGoldenPigReq(int param);
	void OnKillGoldenPigBoss(int summon_type);

	// ����Ͷ��
	void OnRAOpenServerInvestOperateReq(int opera_type, int param1, int param2);
	void SendRAOpenServerInvestInfo();
	void OnRAInvest(int type);
	void OnRAOpenServerInvestFetchReward(int type, int seq);
	void AddOpenServerInvestFinishParam(int type, int param, bool err_notice = true);

	// ��������
	void OnRASingleChongZhiOperaReq(int opera_type, int param1);
	void OnRASingleChongZhiFetchReward(int seq);
	void OnRASingleChongZhiGiveReward();
	void SendSingleChongZhiAllInfo();
	void OnRASingleChongZhiAddChongZhi(int gold_num);

	// �ۻ���
	void OnRaXianyuanTreasOperReq(int opera, int param);
	void SendRaXianyuanTreasInfo();
	void OnRaXianyuanTreasBuy(int seq);
	void OnRaXianyuanTreasBuyAll();
	void OnRaXianyuanTreasFetchReward(int seq);
	void OnRaXianyuanTreasFetchAllBuyGift();

	// ��ʱ��ɱ
	void OnRARushBuyingOperaReq(int opera_type, int param1);
	void SendRARushBuyingAllInfo();
	void OnRARushBuyingBuyItem(int index);

	//------------------------------------------
	// ��ͼѰ��
	//------------------------------------------
	void OnRaMapHuntOperaReq(int opera_type, int param1, int param2);
	void SendRAMapHuntInfo(int extern_reward_route_id = 0);
	bool OnRaMapHuntFlush(bool is_free, bool &is_get, const int city_id = -1);
	bool OnRaMapHuntQuicklyFlush(const int city_id, bool &is_get);
	void OnRaMapHuntXunbao(const int city_id);												// Ѱ��
	void OnRaMapHuntFetchReturnReward(int index);

	//-------------------------------
	// ����Ѱ��
	//-------------------------------
	void OnRaLightTowerExploreOperaReq(int opera_type, int param1);
	void OnRALightTowerExploreDraw(int times);
	void OnRALightTowerExploreFetchExternReward(int layer);
	void OnRALightTowerExploreFetchReturnReward(int index);
	bool CheckCanFetchLightTowerExploreExternReward(int layer);
	void SendLightTowerExploreInfo();

	// ------------------------------
	// ���ۼƳ�ֵ
	// ------------------------------
	void OnRANewTotalChargeOperaReq(int opera_type, int param);
	void OnRANewTotalChargeCheckChongzhi(int chongzhi);
	void RACheckNewTotalChargeGiveReward();
	void SendRANewTotalChargeInfo();
	void OnRANewTotalChargeFetchReward(int seq);

	// ------------------------------
	// ���ۼƳ�ֵ1
	// ------------------------------
	void OnRANewTotalCharge1OperaReq(int opera_type, int param);
	void OnRANewTotalChargeCheckChongzhi1(int chongzhi);
	void RACheckNewTotalCharge1GiveReward();
	void SendRANewTotalCharge1Info();
	void OnRANewTotalCharge1FetchReward(int seq);

	// ------------------------------
	// ���ۼƳ�ֵ2
	// ------------------------------
	void OnRANewTotalCharge2OperaReq(int opera_type, int param);
	void OnRANewTotalChargeCheckChongzhi2(int chongzhi);
	void RACheckNewTotalCharge2GiveReward();
	void SendRANewTotalCharge2Info();
	void OnRANewTotalCharge2FetchReward(int seq);

	// ------------------------------
	// ���ۼƳ�ֵ3
	// ------------------------------
	void OnRANewTotalCharge3OperaReq(int opera_type, int param);
	void OnRANewTotalChargeCheckChongzhi3(int chongzhi);
	void RACheckNewTotalCharge3GiveReward();
	void SendRANewTotalCharge3Info();
	void OnRANewTotalCharge3FetchReward(int seq);

	// ------------------------------
	// ��װ�̵�
	// ------------------------------
	void OnRAMagicShopOperaReq(int opera_type, int param_1, int param_2);
	void OnRAMagicShopBuyOpera(int type, int index);
	void OnRAMagicShopBuyOne(int index);
	void OnRAMagicShopBuyAll();
	void SendRAMagicShopAllInfo();

	// ------------------------------
	// Ȥζ����
	// ------------------------------
	void OnRAFishingOperaReq(int opera_type, int param1);
	void OnFishingReq(int param1);			//�������
	void FishingAck();						//�������󷵻�
	void StealFishReq(int uid);				//����͵��
	void StealFishAck(int uid);
	void OnExchangeFishReward(int reward_type);
	void CancelAutoFishing();				//ȡ���Զ�����
	void ExitFishing();						//�˳�����
	void SendAllFishingInfo(short fish_type, short steal_fish_type);
	short GetFishingscore();
	short GetStealfishscore();
	unsigned int GetFishingFishingTimestamp();
	unsigned int GetFishingStealTimestamp();
	char GetFishingStation();

	// ս���Ҷ�
	void OnMessBattleEnter();

	// ------------------------------
	// ֲ��
	// ------------------------------
	int GetRAPlantingTreePlantingTimes() { return m_activity_info.ra_planting_tree_planting_times; }
	int GetRAPlantingTreeWateringTimes() { return m_activity_info.ra_planting_tree_watering_times; }
	void SendRAPlantingTreeRankInfo(int rank_type);
	void SendRAPlantingTreeTreeInfo(int scene_id, ObjID obj_id);
	void SendRAPlantingTreeMiniMapInfo(int scene_id);
	void OnRAPlantingTreeOperaReq(int opera_type, int param1, int param2);					// ֲ�������
	bool OnRAPlantingTreePlanting(ItemID item_id);
	void OnRAPlantingTreeWatering(int gather_id, ObjID obj_id);
	bool CanRAPlantingTreeWatering(int gather_id, int obj_id);
	void RACheckPlantingTreeCanyuReward();

	// ------------------------------
	// ����Ȼˮ��
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
	// ��Ԫ��
	//-----------------------------
	void OnRAChongzhiCrazyRebate(long long gold_num);
	void SendRaCrazyRebateChongzhiInfo();

	//------------------------------------------
	// ����             ����
	//------------------------------------------
	void OnFairyBuddhaCardActivateReq(int card_type);
	void SendFairyBuddhaCardInfo();
	void OnFairyBuddhaCardUpdate(unsigned int now_second);
	int GetFairyBuddhaCardExtraExpPer();
	int GetFairyBuddhaCardExtraMoJingPer();
	void OnFairyBuddhaCardGetBindGoldReq(int card_type);
	bool IsFairyBuddhaActive(int card_type);
	void FairyBuddhaCardOnDayChange();

	// ����
	// --------------------------------------------------------------------------------------------------------
	void SyncFishingGearNumList(int gear_num_list[FISHING_GEAR_MAX_COUNT], bool is_init = false);
	int GetFishingScore();
	void AddFishingScore(int score);
	bool DecFishingScore(int score);
	void SendFishingScoreInfo();

	// ------------------------------
	// �����콱
	// ------------------------------
	void OnRAConsumeGiftConsumeGold(int consume_gold);
	void OnRAConsumeGiftOpera(int param1, int param2);
	void SendRAConsumeGiftInfo();
	void OnRAConsumeGiftFetchReward(int param);
	void OnRAConsumeGiftRoll();       // ��ʼҡ��
	void OnRAConsumeGiftRollTen();       // ��ʼҡ��ʮ��
	void OnRAConsumeGiftRollReward(); // ��ȡ����
	void OnRAConsumeGiftRollRewardTen(); // ��ȡʮ�ν���

	// ------------------------------
	// ÿ���޹�
	// ------------------------------
	void OnRADailyLimitBuyOpera(int param1, int param2);
	void OnRADailyLimitBuyBuy(int seq);
	void SendRADailyLimitBuyInfo();

	// ------------------------------
	// �۱��裨�£�
	// ------------------------------
	void OnRACollectTreasureOperaReq(int param1, int param2);
	void OnRACollectTreasureChongZhi(int chongzhi);
	void SendRACollectTreasureInfo();
	void OnRACollectTreasureRoll();
	void OnRACollectTreasureReward();

	// ------------------------------
	// �����۳�
	// ------------------------------
	void OnRAHappyCumulChongzhiGold(int chongzhi_gold);
	void OnRAHappyCumulChongzhiOpera(int param1, int param2);
	void SendRAHappyCumulChongzhiInfo();
	void OnRAHappyCumulChongzhiReward(int param);

	// ------------------------------
	// ������
	// ------------------------------
	void OnRACriticalStrikeOperaReq();
	int GetCriticalStrikeDayMul(int type);       // ������  ��������  
	int GetRACriticalStrikeDayRate(int type);       // ������  �������ʣ���ֱ� 
	
	// ------------------------------
	// ���׷���
	// ------------------------------
	void OnRaJinJieOpera(int type, short grade,short old_grade);
	void OnRAJinJieReturnOpera(int param1, int param2);
	void SendRAJinJieReturnInfo();
	void OnRAJinJieReturnReward(int param);
	void CheckRAJinJieOldAllReward(int theme,int old_grade);
	int JinJieActGetGradeByTheme(int theme);
	static int JinJieActThemeGetByUpgrade(int upgrade_type);

	//------------------------------------------
	// ȫ�����
	//------------------------------------------
	void OnQuanMinJinJie(int opear_type, int seq);
	bool OnFetchUpgradeReward(int seq);
	void SendQuanMinJinJieInfo(int seq);
	int GetPersonalGrade();

	//------------------------------------------
	// ȫ���ܶ�Ա
	//------------------------------------------
	void OnUpgradeGroupe(int opear_type, int seq);
	bool OnFetchUpgradeGroupeReward(int seq);
	void SendUpgradeGroupeInfo(int type);

	//----------------------------------
	// ��ʱ����
	//----------------------------------
	void SendLimitTimeRebateInfo();
	void OnRALimitTimeRebateOperaReq(int opera_type, int param);
	void OnRaLimitTimeRebateFetchReward(int seq);
	void OnRaLimitTimeRebateAddchongzhi(int chongzhi_count);

	//-----------------------------
	// ��ʱ���
	//-----------------------------
	void OnRATimeLimitGiftCheckChongzhi(int chongzhi);
	void OnRATimeLimitGiftOperaReq(int opera_type, int param, int param2);
	void OnRATimeLimitGiftFetchReward(int seq, int fetsh_seq);
	void SendRATimeLimitGiftInfo();

	// ------------------------------
	//ѭ����ֵ2
	// ------------------------------
	void OnRACirculationChongzhiSecondOperaReq(int opera_type);
	void OnRACirculationChongzhiSecondAddchongzhi(int chongzhi_gold);
	void SendCirculationChongzhiSecondInfo();
	void OnCirculationChongzhiSecondFetchReward();

	// ------------------------------
	// ���ҡǮ��
	// ------------------------------
	void OnRAShakeMoneyOperaReq(int opera_type, int param);
	void OnRAShakeMoneyCheckChongzhi(int chongzhi);
	void OnRAShakeMoneyFetchGold();
	void SendRAShakeMoneyInfo();

	// ------------------------------
	// ��ʱ���񣨹���
	// ------------------------------
	void OnRATimeLimitLuxuryGiftBagOperaReq(int opera_type, int param);
	void OnRATimeLimitLuxuryGiftBagBuy(int seq);
	void SendRATimeLimitLuxuryGiftBagInfo();

	// ------------------------------
	//����ͬ��
	// ------------------------------
	void OnRADoubleChongzhiOpera(int opera_type);
	void OnRADoubleChongzhiChongzhi(int chongzhi);
	void SendRADoubleChongzhiInfo();

	// ------------------------------
	// RMB����
	// ------------------------------
	void OnRARmbBugChestShopReq();
	bool OnRARmbBugChestShopChongzhi(const int chongzhi_num);
	void SendRARmbBugChestShopInfo();

	// ------------------------------
	// ���ѷ���
	// ------------------------------
	void OnRAConsumeGoldRewardOpera(int opera_type);
	void OnRAConsumeGoldRewardConsumeGold(int consume_gold);
	void SendRAConsumeGoldRewardInfo();
	void OnConsumeGoldRewardFetchReward();

	// ------------------------------
	// ����2
	// ------------------------------
	void OnRAItemCollectionSecondOperaReq(int opera_type, int param1);
	void OnRAItemCollectionSecondExchange(int seq);
	void SendCollectSecondExchangeInfo();

	// ------------------------------
	// �汾�ۼƳ�ֵ
	// ------------------------------
	void OnRAVersionTotalChargeOperaReq(int opera_type, int param);
	void OnRAVersionTotalChargeCheckChongzhi(int chongzhi);
	void SendRAVersionTotalChargeInfo();
	void OnRAVersionTotalChargeFetchReward(int seq);

	// ------------------------------
	// �汾������ֵ
	// ------------------------------
	void OnRAVersionContinueGhargeOperaReq(int opera_type, int param1);
	void OnRAVersionContinueChargeAddChongzhi(int chongzhi);
	void SendRAVersionContinueChargeActivityInfo();
	void OnRAVersionContinueChargeFetchReward(int day_index);

	// ------------------------------
	// ����ҡ��2
	// ------------------------------
	void OnRaHuanLeYaoJiang2OperReq(int opera_type, int param);
	void OnRAHuanLeYaoJiang2Tao(int type);
	void SendRAHuanLeYaoJiang2Info();
	void OnRAHuanLeYaoJiang2FetchPersonalReward(int seq);

	// ------------------------------
	// ���µ��ʳ�ֵ0
	// ------------------------------
	void SendRAOfflineSingleChargeInfo();
	void OnRAOfflineSingleChargeChongZhi(int chongzhi_num);

	// --------------------------------------------------------------------------------------------------------
	// ��������
	// --------------------------------------------------------------------------------------------------------
	void OnRAConsumeForGiftOperaReq(int opera_type, int param1);
	void SendRAConsumeForGiftAllInfo();
	void OnRAConsumeForGiftExchangeItem(int index);
	void OnRAConsumeForGiftConsumeGold(int consume_gold);

	// �ؾ�Ѱ��3 
	void OnRaMiJingXunBao3OperReq(int opera_type, int param);
	void OnRAMiJingXunBao3Tao(int type);
	void SendRAMiJingXunBao3Info();
	void ReissueMiJingXunBao3FetchPersonalReward();
	void OnRAMiJingXunBao3FetchPersonalReward(int seq);

	// �����ҵ�
	void OnRaHuanLeZaDanOperReq(int opera_type, int param);
	void OnRAHuanLeZaDanTao(int type);
	void SendRAHuanLeZaDanInfo();
	void ReissueHuanLeZaDanFetchPersonalReward();
	void OnRAHuanLeZaDanFetchPersonalReward(int seq);

	// ����ҡ��
	void OnRaHuanLeYaoJiangOperReq(int opera_type, int param);
	void OnRAHuanLeYaoJiangTao(int type);
	void SendRAHuanLeYaoJiangInfo();
	void ReissueHuanLeYaoJiangFetchPersonalReward();
	void OnRAHuanLeYaoJiangFetchPersonalReward(int seq);

	//-----------------------------
	// ��һ��һ
	//-----------------------------
	void OnRABuyOneGetOneFreeOperaReq(int opera_type, int param);
	void SendRABuyOneGetOneFreeInfo();
	void RABuyOneGetOneFreeBuy(int item_index);
	void RABuyOneGetOneFreeFetchReward(int item_index);

	// �ۼƳ�ֵ5(�汾��������)
	void RACheckTotalCharge5RewardCheck();
	void OnRATotalCharge5CheckChongzhi(int chongzhi);
	void OnSendTotalCharge5Info();

	// ������ս
	void OnRAExtremeChallengeReq(int opera_type, int param1);
	void OnRAExtremeChallengeRefreshTask(int task_id);            // ��һ������
	void OnRAExtremeChallengeRefreshTask();                       
	void OnRAExtremeChallengeFetchReward(int task_id);
	void OnRAExtremeChallengeFetchUltimateReward();
	void OnRAExtremeChallengeAddPlan(int opera_type, int param1, int param2 = RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2);
	void SendRAExtremeChallengeTaskInfo();

	// �������
	void OnRAChongZhiGiftOperaReq(int opera_type, int param_1, int param_2);
	void OnRAChongZhiGiftFetchOpera(int index);
	void SendRAChongZhiGiftAllInfo();
	void RACheckChongZhiGiftGiveReward();
	void OnRAChongZhiGiftCheckChongzhi(int chongzhi);

	//���׷���2
	void OnRAJinJieReturn2Opera(int param1, int param2);
	void SendRAJinJieReturn2Info();
	void OnRAJinJieReturn2Reward(int param);
	void CheckRAJinJieOldAllReward2(int theme, int old_grade);
	// ������2
	void OnRACriticalStrike2OperaReq();

	// ��Ϲ���
	void OnRACombineBuyOpera(int opera_type, int param_0);
	void OnRACombineBuyAddInBucket(int seq);
	void OnRACombineBuyRemoveBucket(int seq);
	void OnRACombineBuyItem();
	void SendRACombineBuyBucketInfo();
	void SendRACombineBuyItemInfo();

	// �ۼ�������ֵ(���췵��)
	void OnDayChongzhiRewardAddChongzhi();
	void OnDayChongzhiRewardOpera(short opera_type, short param_0);
	void OnFetchDayChongzhiReward(int seq);
	void OnFetchDayChongzhiRareReward(int seq);
	void SendDayChongzhiRewardInfo();

	// ������Ը
	void OnRALuckyWishOpera(int opera_type, int param_1);			//�ͻ�������
	void SendRALuckyWishInfo(const Protocol::SCLuckyWishInfo &info);//��Э��
	void OnRALuckyWishResetLuckyValue();							//��������ֵ
	void OnRALuckyWishAddLuckyValue(int value);						//��������ֵ
	int GetRALuckyWishValue();										//��ȡ����ֵ
	bool OnRALuckyWishIsLucky();									//�Ƿ�ﵽ������ֵ
	bool OnRALuckyWishLottery(int repeat_times);					//��ʼ��Ը

	//------------------------------------------
	// ��ĩ�񻶻
	//------------------------------------------
	void OnLottery1OperaReq(int opera_type, int param1);
	void OnLottery1ChouReq(int chou_type);
	void OnFetchLottery1PersonRewardReq(int seq);
	void SendLottery1Info(const RALottery1ChouItem *chou_item_list, int size, int count);

	// ------------------------------
	// ��Ԫ��
	// ------------------------------
	void OnRAZeroBuyReturnOperaReq(int opera_type, int param1);
	void OnRAZeroBuyReturnOperaBuy(int buy_type);
	void OnRAZeroBuyReturnOperaFecth(int buy_type);
	void SendRAZeroBuyReturnInfo();

	// ------------------------------
	// �������
	// ------------------------------
	void SendKuangHaiInfo();
	void KuangHaiGetReward(int reward_list_seq);
	void OnKuangHaiCommonOpera(int opera_type, int param_1);
	void OnKuangHaiQingDianRecore(int task_type, int param);
	void CheckRAKuangHaiQingDianGiveReward();

	// ------------------------------
	// �����ո�
	// ------------------------------
	int GetRAGiftHarvestScore() { return m_activity_info.ra_gift_harvest_score; }
	int GetRAGiftHarvestGetScoreTimes() { return m_activity_info.ra_gift_harvest_get_score_times; }
	void RAGiftHarvestAddScore(int score) { m_activity_info.ra_gift_harvest_score += score;  m_activity_info.ra_gift_harvest_get_score_times++; }
	void OnEnterGiftHarvestScene();
	void OnGiftHarvestReq(int opera_type);
	void CheckRAGiftHarvestGiveReward();

	// --------------------------------------------------------------------------------------------------------
	// ��������ʱ��������
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
	void RACheckCirculationChongzhiSecondReward();	// ����ڿ����ʱ��ҲҪ�������������ﴦ��һ��
	void RACheckShakeMoneyGiveReward();
	void RACheckConsumeGoldRewardAutoFetch();
	void RACheckVersionTotalChargeGiveReward();
	void RACheckVersionContinueChongzhiGiveReward();
	void RACheckHuanLeYaoJiang2FetchPersonalReward();
	void CheckRAOfflineSingleChargeReward0();
	void RACheckBuyOneGetOneFreeReward();
	void RACheckGuaGuaAccReward();
	void OnRAExtremeChallengeGiveReward();					// ������ս ��������
	void RACheckChangeRepayment();

	// �����
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

	//����ʱװ��������������㼣
	void RACheckShizhuangUpgradeRankGiveReweard();
	void RACheckShenbinUpgradeRankGiveReweard();
	void RACheckFabaoUpgradeRankGiveReweard();
	void RACheckFootprintUpgradeRankGiveReweard();
	void RACheckFlypetUpgradeRankGiveReweard();
	void RACheckWeiyanUpgradeRankGiveReweard();

	void AddXingzuoYijiGatherBox(int type);

	// --------------------------------------------------------------------------------------------------------
	// ����
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

	// �����ػ�
	void OnHolidayGuardKillMonsterGiveReward();
	void OnHolidayGuardAddKillMonsterCount(int kill_count);

	// ��ɫ������Ҫ����������Ϣ���
	void SendRandActivityInfo();

	// �������
	void OnRAProfessAddScore(int score, bool is_initiative);			// ��¼����
	void SendRAProfessRankInfo();
	int GetRAProfessScore() { return m_activity_info.ra_profess_score; }
	int GetRAProfessToNum() { return m_activity_info.ra_profess_to_num; }
	int GetRAProfessFromNum() { return m_activity_info.ra_profess_from_num; }
	unsigned int GetRAProfessTime() { return m_activity_info.ra_profess_rank_last_profess_timestamp; }

	// add by wwd
	void CheckRandActivity(int rand_act_type, bool is_force = false);

	void OnCheckCrossConsumeGold(long long consume_gold);						// �������������
	void OnCheckCrossDailyConsumeGold(long long daily_consume_gold);			// ���������ÿ������

	void OnRALotteryPutChestStore(int activity_type, int list_num, ChestItemInfo item_list[MODE_MAX_COUNT], const char *reason);	// ���ڽ�Ѱ���ֿ�ĳ齱��

	void FetchTodayThemeReward(int seq);
	void SendFetchTodayThemeRewardInfo();

	// ÿ��һ��2
	void OnRADailyLove2Chongzhi(const int chongzhi_num);
	void OnSendDailyLove2ChongzhiInfo();

	// �����ƹ�
	void OnOpenLuckyCloudBuyPanel(unsigned int timestamp);
	void OnCloseLuckyCloudBuyPanel();
	void SendLuckyCloudBuyInfo();
	bool LuckyCloudBuy();
	void OnLuckyCloudBuyOperaReq(int opera_type, int param1);
	void CheckRALuckyCloudBuyReturn();	// ����Ƿ���δ������Ԫ��
	void ResetLuckyCloudBuy();			// �����ƹ���������
	//�񻶴��ֹ�
	void OnRACracyBuyOperaReq(int opera_type, int seq);
	void OnRAACracyBuySendItemLimitMessage();
	void OnRAACracyBuySendTakePartInMessage();
	void OnRAACracyBuyToBuy(int seq);
	void OnRACracyBuyChongZhi(int money);

private:
	void RandActivityOnRoleLogin();
	void CheckInitRandActivity(int rand_act_type, bool is_force = false);

	Role *m_role;
	CharIntAttrs m_attrs_add;																// �����Լӳ���ֵ
	int m_temp_zhanzhang_side;																// ս����һ�� �����û��ж����ҹ�ϵʹ�� ������
	RoleActivityParam m_activity_info;
	int m_xianmengzhan_defend_area;															// ����ս - �����ݵ�(������)
	bool m_is_first_update;																	// �Ƿ��һ��update
	int m_yizhandaodi_jisha_count;															// һվ���� ��ɱ���������棩

	bool m_is_auto_fishing;																	// �Ƿ��Զ�����
	bool m_is_fishing;																		// �Ƿ��ڵ���״̬
	ItemID m_csa_roll_item_id;																// �Ϸ�ת��roll������Ʒid
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


