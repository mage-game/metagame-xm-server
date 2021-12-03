#ifndef __WELFARE_HPP__
#define __WELFARE_HPP__

#include "servercommon/struct/dailyfindlistparam.hpp"
#include "protocal/msgrole.h"

class Role;

class Welfare
{
public:
	Welfare();
	~Welfare();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const WelfareParam &p, const DailyFindListParam &daily_find_param);
	void GetInitParam(WelfareParam *p);

	void Update(unsigned int interval, unsigned int now_second);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnMonthChange();

	void OnRoleLogin(long long last_save_time);
	void OnRoleLogout();

	void OnAddChongZhi(long long chongzhi_gold);																					

	void SendWelfareInfo(int notify_reason = Protocol::SCWelfareInfo::WELFARE_INFO_DAFAULT, int durlogin_lottory_item_index = -1);
	void SendWelfareItemChange(char dailyfind_type);
	void GetOfflineExp(int type);												
	void GetDailyFind(int dailyfind_type, int get_type);

	void GetDailyFindList(DailyFindListParam *daily_find_param);
	void GetChangeDailyFindList(DailyFindListParam *daily_find_param);
	void ClearDirtyMark();
	void FetchSignInRewardHandle(int day = -1, int is_quick = 0);
	bool FetchSignInReward(int day = -1, int is_quick = 0);
	void FetchAccumulationSignInReward(int seq);
	void SignInFindBack();
	void ActivityFind(int find_type, bool is_free_find = false);
	void OnJoinActivity(unsigned int activity_find_type);
	int HasAutoActivity(int activity_type, int hold_times = 1);
	void OnAutoActivityReq(int activity_find_type);

	void FetchHappyTreeReward(int type);
	void FetchChongjihaoliReward(int level);

	int GetTotalLoginDays();
	void SendTotalLoginDays();

	void SignInByDay(int day);
	void FetchOnlineReward(int part);

	void AddHappytreeGrowthvalue(int type, int add_value = 0);
	void ExchangeReward(int part);

	void GMAddTotalGrouthVal(int value);

	void FetchSevenDayLoginReward(int fetch_day);
	void SendSevenDayLoginRewardInfo(int notify_reason = Protocol::SCSevenDayLoginRewardInfo::SDLRI_DAFAULT);

	// ����buff------------------------------------------
	void SetSaveExpBuff();		// ��鱣�汻��ͣ�ľ���buff
	int GetCurExpBuffLeftTime();
	void SendExpBuffInfo();

private:
	int GetDailyFindDayCount(int dailyfind_type);
	void OnDailyFindChange(int dailyfind_type, unsigned int time, int find_count);
	bool FindMinPos(int dailyfind_type, int *min_pos, int real_find_days);
	void CalcReward(int pos, DailyFindRewardItem *item,int * xiannv_shengwu = NULL);
	bool IsChongZhiUser() { return m_chongzhi_count > 0; }
	void CheckSignIn();
	void CheckActvityCanFind(int check_day, int activity_join_flag = 0, int auto_activity_flag = 0);
	void CheckRewardActiveDegree(int activity_find_type);
	bool IsCanAutoActivityType(int activity_find_type);
	void CalcAccumulationSignInDays();
	//����������Ʒ����
	/*
	����ʱͬ���߾���һ�����������Ʒ, ��������.
	��ȡʱʹ��GiveOfflineItemReward()
	�û������Ʒ��Ϣ����,������Ϣ�����. ÿ���������¼���
	*/
	void CalcOfflineItemReward(int second);
	//����������Ʒ����
	void GiveOfflineItemReward();

	// ��������BUFF׷�ӽ���
	long long CalcExpBuffAddOfflineExp(long long last_save_time, long long base_offline_exp);
	void CheckMergeExpBuff(unsigned int now_s);				// �����ӱ���ͣ��buff
	void CalcOfflineExpBuffTime(unsigned int now_second, long long last_save_time, int &offline_exp_buff_time, int &offline_exp_buff_rate);
	int  CalcOfflineExpBuffLeftCost();


	Role *m_role;

	int m_accumulation_signin_days;																			// �ۼ�ǩ������
	unsigned int m_online_reward_mark;																		// ÿ�����߽�����ȡ���

	int m_offline_second;																					// ����ʱ��
	//int m_offline_exp;																					// ���߾���
	long long m_offline_exp;																				// ���߾���
	int m_offline_mojing;																					// ����ħ��

	int m_green_item_count;																					// ���߽�����ɫ��Ʒ����
	int m_green_item_resolve_count;																			// ���߽�����ɫ��Ʒ����
	int m_blue_item_count;																					// ���߽�����ɫ��Ʒ����
	int m_blue_item_resolve_count;																			// ���߽�����ɫ��Ʒ����
	int m_purple_item_count;																				// ���߽�����ɫ��Ʒ����
	int m_purple_item_resolve_count;																		// ���߽�����ɫ��Ʒ����
	int m_orange_item_count;																				// ���߽�����ɫ��Ʒ����
	int m_orange_item_resolve_count;																		// ���߽�����ɫ��Ʒ����
	int m_red_item_count;																					// ���߽�����ɫ��Ʒ����
	int m_red_item_resolve_count;																			// ���߽�����ɫ��Ʒ����
	int m_pink_item_count;																					// ���߽�����ɫ��Ʒ����
	int m_pink_item_resolve_count;																			// ���߽�����ɫ��Ʒ����

	int m_chongzhi_count;																					// ��ֵ����

	int m_account_total_login_daycount;																		// һ���ۼƵ�¼����

	unsigned int m_last_signin_time;																		// ���һ��ǩ��ʱ��
	unsigned int m_sign_in_days;																			// ��λ���ǩ������
	unsigned int m_sign_in_reward_mark;																		// ��λ����ۼ�ǩ�������Ƿ���ȡ
	char m_is_chongzhi_today;																				// �����Ƿ��ֵ
	char m_continuity_signin_days;																			// ����ǩ������

	unsigned int m_activity_find_flag;																		// ����һر��
	unsigned int m_activity_join_flag;																		// �������
	unsigned int m_activity_auto_flag;																		// �ί�б��
	int m_offline_pass_days;																				// �������� ����������ڻ�һ�
	
	int m_offline_days;																						// �������� ������������ճ��һ�
	DailyFindItem m_daily_find_list[DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS];						// �ճ��һ�����*����һ�����
	bool m_dirty_mark[DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS];										// dirty��־λ
	bool m_old_state[DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS];										// �����ݱ�־�������ڱ�������ʱ�������ֲ���
	bool m_change_flag;																						// �����Ƿ��иı�

	short m_happy_tree_level;																				// ���ֹ����ȼ�
	short m_happy_tree_grouth_val[MAX_GROWTH_VALUE_GET_TYPE];												// ���ֹ�������ֵ
	short m_happy_tree_reward;																				// ���ֹ���������ȡ���
	short m_chongjihaoli_reward_mark;																		// �弶������ȡ���
	int m_total_happy_tree_growth_val;																		// �ܵĻ��ֹ�������ֵ
	unsigned int m_happy_tree_online_time;																	// ���ֹ�������ʱ��
	long long m_today_chongzhi_gold_num;																	// �����ֵ��ʯ����ȡ���ֹ�������ֵ

	int m_seven_day_login_fetch_reward_mark;																// �����ۼƵ�¼������ȡ���

	char m_sign_in_today_times;																				// ÿ��ǩ���Ĵ���
	std::map<ItemID, int> item_list;																		// ������Ʒ����
	std::map<ItemID, int> collect_item_list;																// ���߼��ֵ���
	
	int m_role_level_after_fetch;																			// ��ȡ���߾���������ĵȼ�
	int m_role_login_level;																					// ��ҵ�¼�ȼ�

	int m_collect_left_time_s;																				// ���ֻʣ��ʱ��
	int m_collect_offline_time_s;																			// ���ֻ���ߵ���ʱ��
	int m_collect_item_count;																				// ������Ʒ��������

	char m_is_not_first_send;																				// �Ƿ���״���ȡ���߽���

	ExpBuffInfo m_exp_buff_info;																			// ����buff ��Ϣ
	unsigned int  m_nex_check_buff_time = 0;																// ��һ�μ��buffʱ��
	int m_offline_buff_time = 0;																			// ����buffʱ��
};

#endif // __WELFARE_DEF_HPP__

