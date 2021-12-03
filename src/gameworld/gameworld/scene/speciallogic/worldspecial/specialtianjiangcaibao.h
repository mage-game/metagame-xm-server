#ifndef __SPECIAL_TIANJIAN_CAIBAO_HPP__
#define __SPECIAL_TIANJIAN_CAIBAO_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <vector>

class SpecialTianJiangCaiBao : public SpecialLogic
{
public:
	SpecialTianJiangCaiBao(Scene *scene);
	virtual ~SpecialTianJiangCaiBao();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual bool CanDestroy() { return m_is_finish; }

	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid);
	virtual void OnHurtOther(Character *cha, Character *target);

	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnRoleEnterScene(Role *role);

	virtual bool SpecialCanMountOn(Role *role){return false;}

	void OnActivityStatusChange(int status);

protected:
	TianJiangCaiBaoUser * GetTianJiangCaiBaoUser(const UserID &user_id);

	void CheckBornQianduoduo(time_t now_second);
	void CheckBornBigQianduoduo(time_t now_second);

	void OnKillQianduoduo(Role *role);
	void OnKillBigQianduoduo(Role *role);

	void CheckTaskProgress(Role *role, int task_type, int param_id);

	void SendTianJiangCaiBaoInfoToAllUser();
	void SendTianJiangCaiBaoUserInfo(Role *role);

	void OnActivityStart();
	void OnActivityClose();

	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	void CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist = false);

	bool m_is_finish;

	bool m_is_qianduoduo_notice;
	time_t m_reflush_qianduoduo_time;
	int m_cur_qianduoduo_num;

	bool m_is_bigqianduoduo_notice;
	time_t m_reflush_bigqiandoudou_time;
	int m_cur_bigqianduoduo_num;

	TianJiangCaiBaoUserMap m_tianjiangcaibao_usermap;											// 天降财宝用户列表
};

#endif // __SPECIAL_TIANJIAN_CAIBAO_HPP__


