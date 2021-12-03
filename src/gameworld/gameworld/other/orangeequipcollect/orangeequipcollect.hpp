#ifndef __ORAGNE_EQUIP_COLLECT_HPP__
#define __ORAGNE_EQUIP_COLLECT_HPP__
#include <vector>
#include "obj/character/attribute.hpp"
#include "orangeequipcollectreqhandler.hpp"
#include "servercommon/struct/orangeequipcollectdef.hpp"

class Role;

class OrangeEquipCollect
{
	friend class reqhandler_orange_equip_collect::TakeOn;
	friend class reqhandler_orange_equip_collect::FetchActReward;

public:
	OrangeEquipCollect();
	
	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const OrangeEquipCollectParam &param);
	void GetInitParam(OrangeEquipCollectParam *param) { *param = m_param; }

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnRoleLogin();
	void OnEnterScene(int scene_id);
	void OnCheckActive(int to_level);
	
	void SendAllInfo();

	int GetCollectCount() const;	// 获取已经集齐的套装数量

private:
	void RefreshCollectInfo();

	void CheckGiveActReward();		// 检测给予开服活动奖励

	ItemDataWrapper * GetItem(int seq, int slot);

	void SendSeqInfo(int seq);
	void SendOtherInfo();

private:
	Role *m_role;
	CharIntAttrs m_attrs_add;
	OrangeEquipCollectParam m_param;

	OrangeEquipCollectInfo m_collect_info_list[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT];		// 套装收集个数
};

#endif // __REQ_EQUIP_COLLECT_HPP__