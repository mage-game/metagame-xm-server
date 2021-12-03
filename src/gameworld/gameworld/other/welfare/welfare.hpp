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

	// 经验buff------------------------------------------
	void SetSaveExpBuff();		// 检查保存被暂停的经验buff
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
	//计算离线物品奖励
	/*
	上线时同离线经验一起计算离线物品, 缓存下来.
	领取时使用GiveOfflineItemReward()
	拿缓存的物品信息发放,缓存信息不入库. 每次上线重新计算
	*/
	void CalcOfflineItemReward(int second);
	//发放离线物品奖励
	void GiveOfflineItemReward();

	// 计算离线BUFF追加奖励
	long long CalcExpBuffAddOfflineExp(long long last_save_time, long long base_offline_exp);
	void CheckMergeExpBuff(unsigned int now_s);				// 检查添加被暂停的buff
	void CalcOfflineExpBuffTime(unsigned int now_second, long long last_save_time, int &offline_exp_buff_time, int &offline_exp_buff_rate);
	int  CalcOfflineExpBuffLeftCost();


	Role *m_role;

	int m_accumulation_signin_days;																			// 累计签到天数
	unsigned int m_online_reward_mark;																		// 每日在线奖励领取标记

	int m_offline_second;																					// 离线时间
	//int m_offline_exp;																					// 离线经验
	long long m_offline_exp;																				// 离线经验
	int m_offline_mojing;																					// 离线魔晶

	int m_green_item_count;																					// 离线奖励绿色物品数量
	int m_green_item_resolve_count;																			// 离线奖励绿色物品数量
	int m_blue_item_count;																					// 离线奖励蓝色物品数量
	int m_blue_item_resolve_count;																			// 离线奖励蓝色物品数量
	int m_purple_item_count;																				// 离线奖励紫色物品数量
	int m_purple_item_resolve_count;																		// 离线奖励紫色物品数量
	int m_orange_item_count;																				// 离线奖励橙色物品数量
	int m_orange_item_resolve_count;																		// 离线奖励橙色物品数量
	int m_red_item_count;																					// 离线奖励红色物品数量
	int m_red_item_resolve_count;																			// 离线奖励红色物品数量
	int m_pink_item_count;																					// 离线奖励粉色物品数量
	int m_pink_item_resolve_count;																			// 离线奖励粉色物品数量

	int m_chongzhi_count;																					// 充值次数

	int m_account_total_login_daycount;																		// 一生累计登录天数

	unsigned int m_last_signin_time;																		// 最后一次签到时间
	unsigned int m_sign_in_days;																			// 按位标记签到天数
	unsigned int m_sign_in_reward_mark;																		// 按位标记累计签到奖励是否领取
	char m_is_chongzhi_today;																				// 今天是否充值
	char m_continuity_signin_days;																			// 连续签到次数

	unsigned int m_activity_find_flag;																		// 活动能找回标记
	unsigned int m_activity_join_flag;																		// 活动参与标记
	unsigned int m_activity_auto_flag;																		// 活动委托标记
	int m_offline_pass_days;																				// 离线天数 不保存仅用于活动找回
	
	int m_offline_days;																						// 离线天数 不保存仅用于日常找回
	DailyFindItem m_daily_find_list[DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS];						// 日常找回类型*最大找回天数
	bool m_dirty_mark[DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS];										// dirty标志位
	bool m_old_state[DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS];										// 旧数据标志，用于在保存数据时决定何种操作
	bool m_change_flag;																						// 数据是否有改变

	short m_happy_tree_level;																				// 欢乐果树等级
	short m_happy_tree_grouth_val[MAX_GROWTH_VALUE_GET_TYPE];												// 欢乐果树长成值
	short m_happy_tree_reward;																				// 欢乐果树奖励领取标记
	short m_chongjihaoli_reward_mark;																		// 冲级豪礼领取标记
	int m_total_happy_tree_growth_val;																		// 总的欢乐果树长成值
	unsigned int m_happy_tree_online_time;																	// 欢乐果树在线时长
	long long m_today_chongzhi_gold_num;																	// 今天充值钻石可领取欢乐果树长成值

	int m_seven_day_login_fetch_reward_mark;																// 七天累计登录奖励领取标记

	char m_sign_in_today_times;																				// 每日签到的次数
	std::map<ItemID, int> item_list;																		// 离线物品奖励
	std::map<ItemID, int> collect_item_list;																// 离线集字掉落
	
	int m_role_level_after_fetch;																			// 领取离线经验升级后的等级
	int m_role_login_level;																					// 玩家登录等级

	int m_collect_left_time_s;																				// 集字活动剩余时间
	int m_collect_offline_time_s;																			// 集字活动离线掉落时间
	int m_collect_item_count;																				// 集字物品掉落数量

	char m_is_not_first_send;																				// 是否非首次领取离线奖励

	ExpBuffInfo m_exp_buff_info;																			// 经验buff 信息
	unsigned int  m_nex_check_buff_time = 0;																// 下一次检查buff时间
	int m_offline_buff_time = 0;																			// 离线buff时间
};

#endif // __WELFARE_DEF_HPP__

