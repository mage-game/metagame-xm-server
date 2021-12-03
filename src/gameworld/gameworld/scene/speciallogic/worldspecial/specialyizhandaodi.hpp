#ifndef __SPECIAL_YIZHANDAODI_HPP__
#define __SPECIAL_YIZHANDAODI_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"

class SpecialYiZhanDaoDi : public SpecialLogic
{
public:
	SpecialYiZhanDaoDi(Scene *scene);
	virtual ~SpecialYiZhanDaoDi();

	virtual bool CanDestroy() { return false; }
	virtual int GetSpecialParam(Role *role);
	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual void OnAddObj(Obj *obj);
	virtual void OnRemoveObj(Obj *obj);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool SpecialCanCampWeekBuffEffect(Role *role) { return false; }

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);

	void OnActivityStatusChange(int status);
	void YiZhanDaoDiGuWu(Role *role, Protocol::CSYiZhanDaoDiGuwu *yzddgw);

	void RefreshBuff(unsigned int now_sec);
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

protected:
	void OnActivityStart();
	void OnActivityClose();

	void CheckUserKillRank(YiZhanDaoDiUser *yizhandaodi_user,Role * role = NULL);
	void CheckUserKillNumReward(Role *role, YiZhanDaoDiUser *user);
	void GaveLuckyItemToUser();
	
	void SendInfoToUser(Role *role, YiZhanDaoDiUser *user, bool jiesuan);
	void SendLuckyInfoToUser(Role *role = NULL);
	void SendRankToUser(Role *role = NULL);
	void GetSelfRankInfo(Role *role, short &jisha_count, short &rank_pos);
	YiZhanDaoDiUser * GetYiZhanDaoDiUser(const UserID &user_id);

	void CalcReward();
	void RecalcGuwuBuff(Role *role);
	void SendRewardToUser(const UserID &user_id, int rank_idx); 

	void NotifySystemMsg(char *str_buff, int str_len);

	void Clear();

	void RecalDeadRewardBuff(Role *role);
	void CheckDeadRewardItem(Role *role);

	void CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist = false);

	void CheckRankTopMsg();

	bool m_isopen;

	int m_rankinfo_count;																	// 实时排行榜信息个数
	YiZhanDaoDiUserRankItem m_rankinfo_list[YIZHANDAODI_RANK_NUM];							// 实时排行榜信息

	Protocol::SCYiZhanDaoDiRankInfo m_yzddri;												// 实时排行榜协议

	YiZhanDaoDiUserMap m_yizhandaodi_usermap;												// 一站到底用户列表

	Protocol::SCYiZhanDaoDiLuckyInfo m_yzdli;												// 幸运玩家信息

	unsigned int m_next_refresh_gahter_time;
	unsigned int m_next_broast_total_num_time;
};


#endif // __SPECIAL_YIZHANDAODI_HPP__



