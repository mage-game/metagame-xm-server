#ifndef _TITLEMANAGER_HPP_
#define _TITLEMANAGER_HPP_

#include "obj/character/attribute.hpp"
#include "servercommon/titledef.hpp"
#include "servercommon/struct/roleglobalmoduleinfo.hpp"

class Role;

enum TITLE_ACTION_TYPE
{
	TITLE_ACTION_TYPE_INVALID = 0,

	TITLE_ACTION_TYPE_GET = 1,							// 获得称号
	TITLE_ACTION_TYPE_LOST = 2,							// 失去称号

	TITLE_ACTION_TYPE_MAX,
};

class TitleManager
{
public:
	TitleManager();
	~TitleManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const TitleParam &title_param);
	void GetOtherInitParam(TitleParam *param);
	
	void OnRoleLogin();
	void Update(time_t now_second);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	bool AddTitle(unsigned short title_id, bool syn_title_info, bool need_recal, bool notify_client, bool auto_use);
	bool RemoveTitle(unsigned short title_id, bool syn_title_info, bool need_recal, bool notify_client);
	bool HasOtherTitle(unsigned short title_id);
	void UseTitleList(unsigned short used_title_list[MAX_USE_TITLE_COUNT]);
	int GetUseTitleList(unsigned short used_title_list[MAX_USE_TITLE_COUNT]);
	unsigned short GetUsetJingLingTitle() { return m_use_jingling_titleid; }

	short GetAllTitleList(unsigned short title_id_list[MAX_OWN_TITLE_COUNT]);

	void SendUsedTitle();
	void SendAllTitle();

	void OnCrossTopRankInfo(const CrossPersonRankInfo &cross_personrank_info, int reason);
	void OnTopRankInfo(const PersonRankInfo &personrank_info, int reason);
	void OnActivityTitleInfo(const ActivityTitleInfo &activity_info, int reason);
	void OnChangeCharmRankTitle(int reason, bool is_init = false);
	void OnChangeWorldQuestionRankTitle(int reason, bool is_init = false);
	void OnOtherTitleInfo(const OtherTitleInfo &other_info, int reason);
	void OnRoomActivityTitleInfo(const RoomActivityInfo & info,int reason);
	void OnCrossActivityTitleInfo(const CrossActivityTitleInfo & info, int reason);
	void RecalPataFbTitle();
	void RecalJinglingCardTitle();
	void RecalJinglingTitle();

	void CheckAddCrossGuildKillRankTitle(int reason);

	bool OnActive(unsigned short title_id, bool syn_title_info, bool need_recal, bool notify_client, bool auto_use);
	void OnUpGradeReq(int title_id);

	void CheckAllTitleCondition(int type);
	void OnChallengeFieldRankInfo(int rank_pos, int reason); // 通过邮件发放，已无用
	void CheckShowChallengeFieldRank();

	void OnChangeLieKunTitle(int title_id, int reason);

private:
	bool IsUsed(unsigned short title_id);

	void BroadCastUsedTitle();

	void AttachAttr(CharIntAttrs &base_add, unsigned short title_id, char title_grade);

	void OnOneTitle(unsigned short title_id, bool is_add, int reason, bool *title_chged);

	void OnTitleOwnStatusChange(int action_type, unsigned short title_id);

	void MailOnTileStatusChange(int activity_type, bool is_add, int title_id, int param1 = 0);
	
	int GetTitleIndex(int title_id);

	Role *m_role;

	CharIntAttrs m_attrs_add;

	int m_other_title_count;									// 其他称号数量
	Title m_other_title_list[MAX_OTHER_TITLE_COUNT];			// 其他称号ID列表

	int m_use_title_count;
	unsigned short m_use_title_list[MAX_USE_TITLE_COUNT];		// 顶在头上的称号
	unsigned short m_use_jingling_titleid;

	unsigned int m_last_update_time;

	int m_active_title_grade_count;
};

#endif
