#ifndef __SPECIAL_CROSS_FISHING_HPP__
#define __SPECIAL_CROSS_FISHING_HPP__
#include <map>
#include "scene/speciallogic/speciallogic.hpp"
#include "servercommon/serverconfig/crossfishingconfig.hpp"

struct CrossFishingUserInfo
{
	enum FISHING_STATUS
	{
		FISHING_STATUS_IDLE,				// 未钓鱼，即不在钓鱼界面
		FISHING_STATUS_WAITING,				// 在钓鱼界面等待抛竿
		FISHING_STATUS_CAST,				// 已经抛竿，等待触发事件
		FISHING_STATUS_HOOKED,				// 已经触发事件，等待拉杆
		FISHING_STATUS_PULLED,				// 已经拉杆，等待玩家做选择
	};

	enum SPECIAL_STATUS
	{
		SPECIAL_STATUS_OIL,					// 使用香油中
		SPECIAL_STATUS_AUTO_FISHING,		// 自动钓鱼
		SPECIAL_STATUS_QUICK_FISHING,		// 快速钓鱼
	};

	CrossFishingUserInfo()
	{
		uuid = 0;
		prof = 0;
		special_status_flag = 0;
		least_count_cfg_index = 0;
		is_consumed_auto_fishing = 0;
		is_init = 0;
		cur_score_stage = 0;
		score_stage_fetch_flag = 0;
		total_score = 0;
		special_status_oil_end_timestamp = 0;
		steal_fish_count = 0;
		be_stealed_fish_count = 0;
		buy_steal_count = 0;
		news_count = 0;

		memset(user_name, 0, sizeof(user_name));
		memset(fish_num_list, 0, sizeof(fish_num_list));
		memset(gear_num_list, 0, sizeof(gear_num_list));

		this->Reset();
	}

	// 重置到刚开始钓鱼时的状态
	void Reset()
	{
		fishing_status = FISHING_STATUS_WAITING;
		use_bait_type = -1;
		get_fish_type = -1;
		event_type = -1;
		event_trigger_timestamp = 0;
		auto_pull_timestamp = 0;

		memset(helper_uuid_list, 0, sizeof(helper_uuid_list));
	}

	bool IsSpecialStatusSet(int status) const
	{
		return (special_status_flag & (1 << status)) != 0;
	}

	void SetSpecialStatus(int status)
	{
		special_status_flag |= (1 << status);
	}

	void ClearSpecialStatus(int status)
	{
		special_status_flag &= ~(1 << status);
	}

	long long 		uuid;											// 玩家uid
	GameName		user_name;										// 玩家名字
	char			prof;											// 职业
	char			fishing_status;									// 当前钓鱼状态
	unsigned char	special_status_flag;							// 特殊状态标记
	char			use_bait_type;									// 当前使用的鱼饵类型

	char			get_fish_type;									// 当前获得的鱼类型
	char			event_type;										// 当前触发的事件类型
	char			least_count_cfg_index;							// 最低钓鱼数量配置索引
	char			is_consumed_auto_fishing;						// 是否消费过自动钓鱼

	char			is_init;										// 是否已初始化
	char			cur_score_stage;								// 当前积分阶段
	short			score_stage_fetch_flag;							// 积分阶段奖励领取标记
	int				total_score;									// 总积分

	time_t			event_trigger_timestamp;						// 事件触发时间戳
	time_t			auto_pull_timestamp;							// 自动拉杆时间戳，没有触发事件则为0
	time_t			special_status_oil_end_timestamp;				// 特殊状态香油结束时间戳

	long long		helper_uuid_list[MAX_TEAM_MEMBER_NUM];			// 帮忙拉钩的同伴的uid
	int				fish_num_list[FISHING_FISH_TYPE_MAX_COUNT];		// 当前钓上的各类鱼的数量
	int				gear_num_list[FISHING_GEAR_MAX_COUNT];			// 当前拥有的法宝数量

	int				steal_fish_count;								// 偷鱼次数
	int				be_stealed_fish_count;							// 被偷鱼次数
	int				buy_steal_count;								// 购买偷鱼次数

	int				news_count;										// 日志数量
	FishingNews		news_list[FISHING_NEWS_MAX];					// 日志列表

	std::vector<ItemConfigData> get_reward_item_vec;				// 获得物品奖励
};

class Role;

class SpecialCrossFishing : public SpecialLogic
{
public:

	struct ScoreRankItem
	{
		ScoreRankItem() : uuid(0), total_score(0) {}

		inline bool operator < (ScoreRankItem& rhs) const
		{
			return total_score > rhs.total_score;
		}

		long long  uuid;
		int total_score;
	};

	SpecialCrossFishing(Scene *scene);
	virtual ~SpecialCrossFishing();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool CanDestroy();
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj) { return false; }
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual int GetSpecialParam(Role *role);

	void OnActivityStatusChange(int status);

	void GmAddGear(Role *role);

public:
	void ResetUserInfoOnFail(Role *role, const char *reason);			// 钓鱼流程出错时，重置玩家状态到刚开始钓鱼时的状态

																		// 下面放处理玩家相关请求的接口
	bool StartFishing(Role *role);										// 开始钓鱼（进入钓鱼界面）
	bool CastingRods(Role *role, int bait_type);						// 抛竿，参数1是鱼饵类型 参数2:是否完美钓鱼
	bool PullRods(Role *role, bool is_perfect);							// 拉杆，检查钓鱼事件
	bool ConfirmEvent(Role *role, int param1);							// 确认本次钓鱼事件
	void UseFishingGear(Role *role, int type);							// 使用法宝
	void HelpBigFish(Role *helper_role, int be_helped_uid);				// 帮助拉大鱼
	void StopFishing(Role *role);										// 停止钓鱼（离开钓鱼界面）
	void AutoFishingReq(Role *role, int is_set, int special_status);	// 请求自动钓鱼，is_set为0代表取消
	void OnRandUserReq(Role *role);										// 请求随机玩家信息
	bool IsCanStealFish(Role *role, long long target_uuid);				// 能否偷鱼(客户端显示)
	void OnStealFishReq(Role *role, long long target_uuid);				// 请求偷鱼
	void OnBuyStealCountReq(Role *role);								// 请求购买偷鱼次数
	void OnExChange(Role* role, int combination_index);					// 请求兑换
	void OnBuyBait(Role* role, int bait_type, int num);					// 购买鱼饵
	void OnFetchScoreReward(Role* Role);								// 拿取积分奖励

	void SendFishingRankListToRole(Role * role = nullptr);
	void SetEndTime(unsigned int end_time) { m_activity_end_time = end_time; }

private:
	inline void OnActivityStart() { m_isopen = true; }
	void OnActivityClose();

	void OnEventTrigger(Role *role);														// 触发事件

	void OnCheckEventGetFish(Role *role, CrossFishingUserInfo *user_info);					// 检查事件鱼类上钩
	void OnCheckEventTreasure(Role *role, CrossFishingUserInfo *user_info);					// 检查事件破旧宝箱
	void OnCheckEventGotYuwang(Role *role, CrossFishingUserInfo *user_info);				// 检查事件渔网
	void OnCheckEventGotYucha(Role *role, CrossFishingUserInfo *user_info);					// 检查事件鱼叉
	void OnCheckEventGotOil(Role *role, CrossFishingUserInfo *user_info);					// 检查事件香油
	void OnCheckEventRubber(Role *role, CrossFishingUserInfo *user_info);					// 检查事件盗贼
	void OnCheckEventBigFish(Role *role, CrossFishingUserInfo *user_info);					// 检查事件传说中的大鱼

	void OnConfirmEventGetFish(Role *role, CrossFishingUserInfo *user_info, int give_uid);	// 确认事件鱼类上钩，参数3是要给的玩家，给自己传0
	void OnConfirmEventTreasure(Role *role, CrossFishingUserInfo *user_info, bool is_open);	// 确认事件破旧宝箱
	void OnConfirmEventFisher(Role *role, CrossFishingUserInfo *user_info);					// 确认事件垂钓的渔翁
	void OnConfirmEventRubber(Role *role, CrossFishingUserInfo *user_info);					// 确认事件盗贼
	void OnConfirmEventBigFish(Role *role, CrossFishingUserInfo *user_info, int give_uid);	// 确认事件传说中的大鱼，参数3是要给的玩家，给自己传0

	bool AddUserInfo(Role *role);														// 添加玩家信息
	CrossFishingUserInfo* GetUserInfo(long long uid);									// 获取玩家信息
	void SendUserInfo(Role *role);														// 发送玩家信息
	void SendTeamMemberInfo(Role *role);												// 发送玩家队友的fish_num_list给自己
	void SendFishInfoToTeam(Role *role);												// 发送玩家的fish_num_list给队友（包括自己）
	void SendCheckEventResult(Role *role, int param1, int param2, int param3);			// 发送检查事件结果

	void UpdateScoreRank(Role *role);
	void SendStageScoreInfo(Role* role);												// 钓鱼阶段积分信息
	void NotifyStatuChange(Role* role);													// 钓鱼状态改变

	void ChangeFishNum(Role *role, int fish_type, int num, bool is_chg_score = true,bool is_steal = false);	// 鱼的数量改变时
	void NoticeActStatus();																// 公告活动状态
	void CheckRewardOnClose();
	void AddRewardItem(long long uuid, const ItemConfigData *reward_item_list, int put_count);// 添加活动奖励物品（展示用）

private:
	bool m_isopen;
	bool m_has_notice_act_close;														// 是否公告过活动快要结束

	typedef std::map<long long, CrossFishingUserInfo> FishingUserList;
	typedef std::map<long long, CrossFishingUserInfo>::iterator FishingUserIt;
	FishingUserList m_user_list;

	std::vector<ScoreRankItem> m_score_rank_vec;										// 积分排名玩家列表

	typedef std::map<long long, unsigned short> GatherIdMap;							// 玩家钓鱼的采集物
	GatherIdMap m_gather_id_map;

	unsigned int m_activity_end_time;
	int m_broadcast_times;
	unsigned int m_next_broadcast_time;													
};

#endif


