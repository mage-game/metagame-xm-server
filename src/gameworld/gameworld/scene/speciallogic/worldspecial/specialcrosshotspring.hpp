#ifndef __SPECIAL_HOT_SPRING_HPP__
#define __SPECIAL_HOT_SPRING_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"

#include <vector>
#include <map>

class Role;

class SpecialCrossHotspring : public SpecialLogic
{
public:

	struct JoinerInfo
	{
		JoinerInfo()
		{
			this->Reset();
		}

		void Reset()
		{
			uuid = 0;
			unique_user_id = INVALID_UNIQUE_USER_ID;
			uid = 0;
			obj_id = INVALID_OBJ_ID;
			memset(name,0,sizeof(GameName));
			camp = 0;
			popularity = 0;
			partner_uuid = 0;
			partner_obj_id = INVALID_OBJ_ID;
			give_present_times = 0;
			popularity_change_time = 0;
			swinsuit_image = 0;
			add_exp_total = 0;

			question_score_change_time = 0;
			question_total_score = 0;
			question_shengwang = 0;
			question_honor = 0;
			question_gongxun = 0;
			question_weiwang = 0;
			question_right_count = 0;
			question_wrong_count = 0;
			question_curr_question_flag = 0;
			question_curr_issue_score = 0;
			question_curr_issue_answer_result = 0;

			skill_use_times_1 = 0;				
			skill_use_times_2 = 0;				
			skill_1_can_perform_time = 0;
			skill_2_can_perform_time = 0;

			gather_times = 0;
		}

		long long uuid;
		UniqueUserID unique_user_id;
		int uid;
		ObjID obj_id;
		GameName name;
		int camp;
		int popularity;
		unsigned int popularity_change_time;
		long long partner_uuid;
		ObjID partner_obj_id;
		short give_present_times;
		short swinsuit_image;
		//int add_exp_total;
		long long add_exp_total;

		unsigned int question_score_change_time;
		int question_total_score;									
		int question_shengwang;							// 答题获得总声望
		int question_honor;								// 答题获得总荣誉
		int question_gongxun;							// 答题获得总功勋
		int question_weiwang;							// 答题获得总威望
		char question_right_count;						// 正确答题数
		char question_wrong_count;						// 错误答题数
		short question_curr_question_flag;				// 参与者当前题目是否已结算奖励，0未获得，1已获得
		int question_curr_issue_score;					// 当前答题获得积分
		int question_curr_issue_answer_result;			// 当前回答题目结果

		short skill_use_times_1;						// 按摩技能使用次数
		short skill_use_times_2;						// 砸雪球技能使用次数
		unsigned int skill_1_can_perform_time;			// 按摩技能可使用时间戳
		unsigned int skill_2_can_perform_time;			// 砸雪球技能可使用时间戳

		int gather_times;								// 采集次数
	};

	struct RankItem
	{
		RankItem() : uuid(0), popularity(0),popularity_change_time(0)
		{
			memset(name, 0, sizeof(name));
		}

		RankItem(long long _uuid, int _uid,GameName _name, int _popularity,unsigned int _popularity_change_time):uuid(_uuid), uid(_uid),
			popularity(_popularity),popularity_change_time(_popularity_change_time)
		{
			F_STRNCPY(name, _name, sizeof(name));
		}

		long long uuid;
		int uid;
		GameName name;
		int popularity;
		unsigned int popularity_change_time;
	};

	struct ShuangxiuItem
	{
		ShuangxiuItem() : role_obj_id1(INVALID_OBJ_ID), role_obj_id2(INVALID_OBJ_ID) {}

		ObjID role_obj_id1;		// 发起双修者
		ObjID role_obj_id2; 	// 被邀请者
	};

	enum SPECIAL_IMAGE
	{
		INVAILD_APPEARANCE = 0,

		SWINSUIT_NORMAL_MAN,
		SWINSUIT_NORMAL_WOMAN,

		MAX_SWINSUIT,
	};

	struct QuestiongRankItem
	{
		QuestiongRankItem() : uuid(0), uid(0), score(0),score_change_time(0)
		{
			memset(name, 0, sizeof(name));
		}

		QuestiongRankItem(long long _uuid, int _uid, GameName _name, int _score,unsigned int _score_change_time):uuid(_uuid), uid(_uid), 
			score(_score),score_change_time(_score_change_time)
		{
			F_STRNCPY(name, _name, sizeof(name));
		}

		long long uuid;
		int uid;
		GameName name;
		int score;
		unsigned int score_change_time;
	};

	enum QASyncGlobalInfoType
	{
		QA_BROADCAST_INVALID = 0,
		QA_BROADCAST_QUSETION,
		QA_BROADCAST_RANK,

		QA_BROADCAST_MAX,
	};

	SpecialCrossHotspring(Scene *scene);
	virtual ~SpecialCrossHotspring();

	void OnActivityStatusChange(int status);
	virtual bool CanDestroy();

	JoinerInfo *GetJoinerItem(long long uuid);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj) { return false; }
	virtual int GetSpecialParam(Role *role);
 
 	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnAOIRoleEnter(Role *role, Role *target_role);

	void OnGivePresentReq(Role *role, Protocol::CSHotspringGivePresent *cmd);
	void OnAddPartner(Role *role,Protocol::CSHSAddPartnerReq *cmd);
	void OnAddPartnerRet(Role *role,Protocol::CSHSAddPartnerRet *cmd);
	void OnDeletePartnerReq(Role *role);

	void SendRoleInfo(Role *role);
	void SendRankInfo(Role *role);
	void SyncRankInfoToAll();
	void SendPartnerInfo(Role *role);
	void NotifyShuangxiuInfo(Role *role_1, Role *role_2);

	void QASyncGlobalInfoToAll(SpecialCrossHotspring::QASyncGlobalInfoType type);
	void QASyncGlobalQuestionInfo(Role *role,int side);
	void QASyncGlobalRankInfo(Role *role);

	void QARoleAnswerReq(Role *role,int choose, bool is_use_item);
	void QAFllowRankFirst(Role *role);
	void OnUseBianshenCard(Role *user, Role *target);

	void OnUseSkillReq(Role *role, ObjID obj_id, short skill_type);
	void NotifySkillInfo(Role* use_role, Role* be_use_role, short skill_type);

protected:
	void OnActivityStandy();
	void OnActivityStart();
	void OnActivityClose();

	void SyncRoleInfoToCross(Role *role);
	void AddExp(Role *role);

	void CalcPopularityRank();

	void CalcScoreRank();
	void CalcRewardOnQAActClose();

	void RefreshGather();

	void ClearAllJoninerAnswerState();
	void CalcAllJoinerAnswerResult();
	int FindQuestionRank(long long uuid);
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

 	bool m_isopen;
	bool m_is_hidden_server;
	bool m_is_change;
	time_t m_next_give_exp_timestamp;
	time_t m_give_exp_interval;
	time_t m_next_calc_rank_timestamp;
	time_t m_popularity_rank_interval_s;
	time_t m_next_give_item_timestamp;
	time_t m_give_item_interval;
 
 	std::map<long long, JoinerInfo*> m_joiner_map;
	std::vector<RankItem> m_popularity_rank_vec;

	bool m_is_finish_question_activity;
	bool m_is_can_answer;
	unsigned int m_cur_question_begin_time;			// 答题开始时间
	unsigned int m_curr_question_end_time;			// 答题结束时间
	unsigned int m_next_question_begin_time;		// 下一题开始时间
	short m_total_question_num;						// 答题配置总题目数
	short m_broadcast_question_total;				// 已广播题目总数
	short m_curr_question_id;						// 当前问题id
	int m_side;

	unsigned int m_answer_continue_time;
	unsigned int m_answer_prepare_time;
	unsigned int m_last_first_broadcast_time;
	unsigned int m_last_lover_broadcast_time;

	unsigned int m_start_refresh_gather_time;
	unsigned int m_next_refresh_gather_time;

	std::vector<QuestiongRankItem> m_question_rank_vec;

};

#endif // __SPECIAL_WUXINGGUAJI_HPP__


