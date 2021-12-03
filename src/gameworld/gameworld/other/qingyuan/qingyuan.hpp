#ifndef __QING_YUAN_HPP__
#define __QING_YUAN_HPP__

#include "servercommon/qingyuandef.hpp"
#include "obj/character/attribute.hpp"
#include "protocal/msgother.h"

struct RoleCrossGlobalData;
class Role;
class ItemGridData;

enum QINGYUAN_SHICI_TYPE
{
	SHICI_INVALID = 0,

	SHICI_ONE,
	SHICI_TWO,
	SHICI_THREE,

	SHICI_MAX,
};

enum QINGYUAN_MARRY_TYPE
{
	MARRY_INVALID = -1,

	MARRY_COMMON,
	MARRY_MIDDLE,
	MARRY_TOP_GRADE,

	MARRY_MAX,
};

enum QINGYUAN_FB_JOIN_TIMES
{
	QINGYUAN_FB_JOIN_TIMES_ONE = 0,
	QINGYUAN_FB_JOIN_TIMES_TWO,
	QINGYUAN_FB_JOIN_TIMES_THREE,
};

class Qingyuan
{
public:
	Qingyuan();
	~Qingyuan();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const QingyuanParam &qy_param, int marry_type, int lover_id, int shici_type, int marry_req_id, char is_sure);
	void GetInitParam(QingyuanParam *qy_param);
	void GetRoleInitParam(int *marry_type, int *lover_id, int *shici_type, int *marry_req_id, char *is_sure);

	//�������
	void Init(const PersonProfessParam &profess_param);
	void GetInitParam(PersonProfessParam *profess_param);

	void Update(unsigned int second);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnRoleLogin(unsigned int last_save_time);
	void OnEnterScene(int scene_id);

	void OnMarryOperate(int ope_type, int param1, int param2);			// ������

	void MarryReq(int marry_type, int target_uid);
	void MarryRet(int marry_type, int req_uid, bool is_accept);

	void MarryNotice(Role * marry_target);
	void SendMarryZhuhe(int uid, int type);
	
	void DivorceReq(int is_forced_divorce);
	void DivorceRet(int req_uid, bool is_accept);
	void OnDivorce(unsigned int divorce_time);

	int GetQingyuanValue() { return m_param.qingyuan_value; }
	void AddQingyuanValue(int add_value = 1);
	void QueryMateQingyuanValue();

	bool ChoseShici(int shici_type);										// ѡ�Ĵ�
	void OnPressFinger();													// ����ӡ(�˴�������������)
	void OnPressFingerRepeat(Role *req_role, Role *target_role);			// ����ӡ(�����ظ����)
	void SyncHaloType(int halo_type);									    // ͬ�����޹⻷����(��ʱ����)
	void SendMarryOperaRet(Role* role, int ret_type, int ret_val);			// ���ؽ��������
	void ResetAllData();

	void SetLoverId(int role_id) { m_lover_id = role_id; }
	void SetMarryReqId(int role_id) { m_marry_req_id = role_id; }
	void SetMarryType(int marry_type) { m_marry_type = marry_type; }		
	void SetShiciType(int shici_type) { m_shici_type = shici_type; }
	bool IsSureMarry() { return m_is_sure; }

	//----------------------------------- ��������߼� ----------------------------------
	void SetHunyanFBOpenStatus(int is_open);		//���û��翪��״̬
	bool IsHunyanFBAlreadyOpen() { return (0 != m_param.is_hunyan_fb_already_open); }
	bool CanJoinHunyanFB();

	void BuyFBTimesReq();
	void OnBuyDoubleReward();			// ������Ե�����⹺��˫������
	bool CanJoinFB(bool is_leader);
	void SendBaseFBInfo();

	void AddEnterTimes() { ++ m_param.join_fb_times; }

	bool IsSkipFBWaves() { return 0 != m_param.is_skip_fb_waves; }
	void SetSkipFBWaves(bool is_skip);

	void GMResetHunyan();

	//----------------------------------- �������߼� ----------------------------------

	int PutOn(const ItemDataWrapper &item_wrapper);
	int TakeOff();
	bool HunjieEquipUpLevel(ItemID stuff_id, short repeat_times, bool is_auto_buy);
	void AddUpLevelExp(int add_val);
	void SendHunjieEquipInfo();

	//----------------------------------- �⻷����߼� ----------------------------------
	void CoupleHaloUse(int halo_type);										// װ���⻷
	void UplevelCoupleHalo(int halo_type, bool is_auto_buy = false);		// �����⻷
	void OnBuyHalo(int halo_type);										    // ����⻷
	int GetCoupleHaloLevel(int halo_type);									// ��ȡ�⻷�ȼ�
	int GetActiveCoupleHaloCount(int color);								// ��ȡ�������

	void CheckTejiaHalo(unsigned int now_second);							// ����ؼ۹⻷
	void CheckTejiaHaloActivity(bool is_login, unsigned int last_save_time);	// ����ؼ۹⻷�

	void SendCoupleHaloInfo();												// ���͹⻷��Ϣ

	int GetCoupleHaloType() { return m_param.equiped_couple_halo_type; }	// ��ȡ��װ���⻷����
	bool IsInCoupleHaloEffect() { return m_is_in_couple_halo_effect; }
	
	//----------------------------------- ף������߼� ----------------------------------
	int GetBlessDays() { return m_param.bless_days; }
	void SetBlessDays(int days) { m_param.bless_days = days; }
	void FetchBlessReward();
	void AddBlessDays();													//����ף������
	void SendBlessInfo();

	//----------------------------------- ���� ------------------------------------------
	//bool IsHunyan() { return 0 == m_param.is_first_diamond_yanhui; } // ��ǰ�ǵ�һ�ν�����ʯ��������ѣ�����Ҫͬʱ�����½������ģ����ڲ���Ҫ�ˡ�
	bool IsFirstDiamondHunyan() { return false; }
	void SetHunyanFlag(int flag) { m_param.is_first_diamond_yanhui = (char)flag; }

	int GetMaxGetDiamonds() { return m_param.today_yanhui_get_diamonds; }
	int GetMaxGetExps() { return m_param.today_yanhui_get_exps; }
	int GetMaxGetHunjieStuffs() { return m_param.today_yanhui_get_hunjie_stuffs; }
	int GetMaxGetOtherStuffs() { return m_param.today_yanhui_get_other_stuffs; }
	int GetMaxGetGatherDayMaxStuffs() { return m_param.today_hunyan_gather_day_max_stuff_num; }				// ��ȡ��������Ѳɼ�����
	int GetTodayTuodanTimes() { return m_param.today_tuodan_times; }
	int GetTodaySaxianhuaTimes() { return m_param.today_saxianhua_times; }
	int GetMaxBeZhufuMaxStuffs() { return m_param.today_hunyan_be_zhufu_max_stuff_num; }
	int GetMaxZhufuMaxStuffs() { return m_param.today_hunyan_zhufu_max_stuff_num; }

	void AddMaxGetDiamonds(int number) { m_param.today_yanhui_get_diamonds += number; }
	void AddMaxGetExps(int number) { m_param.today_yanhui_get_exps += number; }
	void AddMaxGetHunjieStuffs(int number) { m_param.today_yanhui_get_hunjie_stuffs += number; }
	void AddMaxGetOtherStuffs(int number) { m_param.today_yanhui_get_other_stuffs += number; }
	void AddMaxGetGatherDayMaxStuffs(int number) { m_param.today_hunyan_gather_day_max_stuff_num += number; } // ���ӻ�������Ѳɼ�����
	void AddTodayTuodanTimes() { m_param.today_tuodan_times ++; }
	void AddTodaySaxianhuaTimes(){ m_param.today_saxianhua_times ++; }
	void AddMaxBeZhufuMaxStuffs(int number) { m_param.today_hunyan_be_zhufu_max_stuff_num += number; }
	void AddMaxZhufuMaxStuffs(int number) { m_param.today_hunyan_zhufu_max_stuff_num += number; }

	void SetOpenHunyanFB(int flag) { m_param.is_hunyan_fb_open = flag; }  // ������
	int GetOpenHunyanFB() {  return m_param.is_hunyan_fb_open; }		  // ������

	void SetYanHuiFBKey(int fb_key) { m_param.yanhui_fb_key = fb_key; }  // ���縱��key(��ʱ����)
	
	int GetMarryCount() {  return m_param.marry_count; }
	void AddMarryCount() { m_param.marry_count++; }

	int GetHunjieStarLevel();

	void SendYanhuiFBKey();

	QingyuanParam GetQingyuanParam(){return m_param;}

	void SetMarrReqUid(int uid){m_param.marry_req_uid = uid;}
	
	void GiveMarryReward(int hunli_type);

	char GetTodayHunyanTimes () { return m_param.today_hunyan_open_times; }
	void AddTodayHunyanTimes() { m_param.today_hunyan_open_times ++; }
	void SetCanHunyanOpen(char flag = 1) { m_param.can_open_hunyan = flag; }
	
	//----------------------------------- ��Եʥ������߼� ----------------------------------
	void SendQingYuanShengDiInfo();
	void ResetQingYuanShengDi(bool is_init = false);
	void OnQingYuanShengDiFecthTaskReward(int task_index);
	void OnQingYuanShengDiFecthOtherReward();
	void OnQingYuanShengDiKillMonster(int task_type, int monster_id);
	void OnQingYuanShengDiGether(int gather_id);
	void OnQingYuanShengDiSkipTask();

	//----------------------------------- ��������߼� ---------------------------------------
	void QingYuanOperaReq(int opera_type, int param1, int param2);	// �»����������
	void MakeAnAppointmantMarryTimeReq(int seq, int hunyan_type);	// �����ȷ��ԤԼʱ�估����
	void AppointmantMarryTimeRet(int seq, int is_agree);			// ����ԤԼʱ��ȷ�ϻ�Ӧ����Ӧ�����ģ�
	bool IsValidYuyueSeq(char seq);									// ԤԼ�Ƿ���Ч
	void WeddingInviteGuest(int uid);								// �������
	void WeddingRemoveGuest(int uid);								// �����ʹ������������Ƴ�
	void BuyWeddingInviteGuestNum();								// ���������������

	void SendWeddingYuYueInfo();									// ����ԤԼ��Ϣ				
	void SendWeddingInfo(int info_type, bool is_popup = false);		// ���������Ϣ

	void SetHunyanType(int type) { m_param.hunyan_type = type; }	// ԤԼ��������
	int GetHunyuanType() { return m_param.hunyan_type; }			// ��ȡ��������
	int GetOtherHunyuanType();										// ��ȡ���˻�������

	char GetHunyanMarryType() { return m_param.marry_type; }
	void SetHunyanMarryType(char marry_type) { m_param.marry_type = marry_type; }					// ���ý������(���翪��������Ч��)
	bool IsHadPropose(int marry_type);																// �Ƿ��Ѿ���ĳ���������

	void SetTodayMarryReqStauts(char is_succ) { m_param.is_today_marry_req_succ = is_succ; }        // ������
	void SetCurMarryStauts(int cur_stauts) { m_param.cur_marry_stauts = cur_stauts; }				// ���û���״̬
	void SetIsHaveHunYanTimes(char is_have_times) { m_param.is_have_hunyan_times = is_have_times; } // �����Ƿ��л������
	void SetWeddingYuYueSeq(char seq) { m_param.wedding_yuyue_seq = seq; }							// ���û���ԤԼ���
	void SetWeddingIndex(int index) { m_param.wedding_index = index; }								// ����ȫ�����ڼ���

	int GetTodayHunyanJoinTimes() { return m_param.today_hunyan_join_times; }						// ����μӻ������
	void AddTodayHunyanJoinTimes() { m_param.today_hunyan_join_times++; }							// ���ӽ���μӻ������

	//----------------------------------- ����-Ѳ�� ---------------------------------------
	bool IsInXunyouStatus();															// �ܷ�����Ѳ����ز���
	bool CheckCanGatherXunyouHongbao(int gather_id);									// �ܷ�ɼ�Ѳ�κ��
	void OnGatherXunyouHongbao(int gather_id);											// �ɼ�Ѳ�κ��

	bool CheckCanSaHongbao();															// ������������
	void OnBuySaHongBaoTimes();															// �������������
	void OnXunyouSaHongbao();															// �����
	bool CheckCanGiveFlower();															// ����ͻ�����
	void OnBuyGiveFlowerTimes();														// �����ͻ�����
	void OnXunyouGiveFlower(ItemID *item_id, int *flower_num);							// �ͻ�

	void CheckTransToXunyouPos();														// �����ת��Ѳ���е�����
	void CheckSyncAOIInXunyou(unsigned int now_second);									// �����Ѳ�ζ�������ͬ��

	void OnSendXunyouObjInfo(int seq);													// ����Ѳ�ζ���obj��Ϣ
	const Posi GetXunyouObjCurPos(int seq);												// ��ȡѲ�ζ���obj����

	//-----------------------------------���ǽ---------------------------------------
	void OnProfessWallReq(Protocol::CSProfessWallReq* msg);														// ���ǽͨ������
	void OnProfessTo(int other_role_id, int gift_type, bool is_auto_buy, ProfessContent& content);				// ���
	void OnProfessFrom(int other_role_id, PersonProfessItem& profess_item);										// �����
	void OnDeleteProfessItem(unsigned short profess_wall_type, unsigned int profess_time, int role_id);			// ɾ��ĳ�����
	void SendProfessWallInfo(unsigned short profess_wall_type, short notify_type);													// ���ͱ��ǽ��Ϣ
	void DeleteProfessItemFunc(std::deque<PersonProfessItem>& profess_deque, unsigned int profess_time, int role_id);
	void SendProfessWallInfoFunc(std::deque<PersonProfessItem>& profess_deque, unsigned short profess_wall_type, short notify_type);

	void AddProfessExp(int exp);																				// ���ӱ�׾���
	void SendProfessLevelInfo();																				// ���ͱ�׵ȼ���Ϣ

private:
	void CheckCoupleHaloEffect(unsigned int now_second);									// ��鴥���⻷��Ч
	bool IsCoupleHaloTrigger();																// �Ƿ񴥷��⻷
	void BroadcastCoupleHaloTrigger(int halo_type);											// �㲥�⻷����
	bool CheckTeamInformationCorrect(int uid);												// ������

	Role *m_role;

	QingyuanParam m_param;
	CharIntAttrs m_attrs_add;

	bool m_is_in_couple_halo_effect;
	unsigned int m_next_check_couple_halo_effect_time;

	int m_halo_level_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];

	//-------------------------------------------------------------------------------------------
	int m_marry_type;						// �������
	int m_lover_id;							// ����id
	int m_shici_type;						// �Ĵ�����
	int m_marry_req_id;						// ����������ߵ�id����������ж����ĵ�ʱ���ã�
	bool m_is_sure;							// �Ƿ��а���ӡ

	//-------------------------------------------------------------------------------------------
	int m_yuyue_hunyan_type;				// ԤԼ�Ļ�������
	bool m_is_trans_to_xunyou_scene;		// �Ƿ��Ѵ��͵�Ѳ�γ���
	UInt32 m_next_sync_aoi_timestamp;		// �´���Ѳ�ζ���ͬ��ʱ���

	//-------------------------------------------------------------------------------------------
	unsigned int m_last_profess_time;				// ��һ�α��ʱ��
	std::deque<PersonProfessItem> m_profess_to;		// ��׳�ȥ��
	std::deque<PersonProfessItem> m_profess_from;	// �յ��ı��

};
#endif

