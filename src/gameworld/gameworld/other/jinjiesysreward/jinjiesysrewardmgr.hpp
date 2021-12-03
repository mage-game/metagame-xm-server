#ifndef __JINJIE_SYS_REWARD_MGR_HPP__
#define __JINJIE_SYS_REWARD_MGR_HPP__

class Role;
class JinjieSysRewardParam;

class JinjieSysRewardMgr
{
public:
	static JinjieSysRewardMgr & Instance()
	{
		static JinjieSysRewardMgr _instance;
		return _instance;
	}

	void InitParam(Role *role, const JinjieSysRewardParam &param);

	void OnRoleLogin(Role *role);
	void OnRoleLevelChange(Role * role, int level);
	void OnDayChange(Role * role, unsigned int old_dayid, unsigned int now_dayid);

	void OnActiveJinjieSys(Role *role, int sys_type);
	int GetJinjieSpecialImgExtraAddPer(Role *role, int sys_type);
	void OnJinjieSysRewardUpgrade(Role *role, int sys_type, int to_grade);
	void CheckJinjieSysReward(Role *role, int sys_type, int special_img_id);

	void OnJinjiesysRewardOpera(Role *role, int opera_type, int param_0, int param_1);
	void SendJinjieSysRewardInfo(Role *role);
	void SendJinjieSysRewardTimestamp(Role *role);
	int GetJinjiesysGrade(Role *role, int sys_type);

private:
	JinjieSysRewardMgr() = default;
	~JinjieSysRewardMgr() = default;

	void SetCompleteTimestamp(Role *role);
	void CheckJinjiesysGrade(Role *role, int sys_type);
	void OnJinjiesysFetchUpgradeReward(Role *role, int sys_type, int reward_type);
	void OnJinjiesysBuySpecialImg(Role *role, int sys_type, int reward_type);
	void CheckBroadcast(Role *role, int sys_type, int special_img_id);
	void CheckGiveReward(Role *role, int sys_type, int special_img_id);
};

#endif // !__JINJIE_SYS_REWARD_MGR_HPP__
