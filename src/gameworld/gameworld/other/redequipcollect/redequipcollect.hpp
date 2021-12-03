#ifndef __REQ_EQUIP_COLLECT_HPP__
#define __REQ_EQUIP_COLLECT_HPP__

#include "obj/character/attribute.hpp"
#include "redequipcollectreqhandler.hpp"
#include "servercommon/struct/redequipcollectdef.hpp"

class Role;

class RedEquipCollect
{
	friend class reqhandler_red_equip_collect::TakeOn;
	friend class reqhandler_red_equip_collect::FetchActReward;
	friend class reqhandler_red_equip_collect::RedEquipCollectFetchRewardHandler;

public:
	RedEquipCollect();
	
	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const RedEquipCollectParam &param);
	void GetInitParam(RedEquipCollectParam *param) { *param = m_param; }

	bool GetActiveTitleList(std::vector<unsigned short> &title_list);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnFirstUpdate(time_t now_second) {}
	void OnDayChange();
	void OnPutonEquip();
	bool IsSuitActive(int seq);

	void OnRoleLogin();
	void OnEnterScene(int scene_id);
	void OnRoleLevelUp(int to_level);
	void SendAllInfo();

	int GetCollectCount() const;	// 获取已经集齐的套装数量

	int GetRedCollectStarAttrPer(int title_id);        // 红装星星加成加到称号id上

private:
	void RefreshCollectInfo();

	void CheckGiveActReward();		// 检测给予开服活动奖励

	ItemDataWrapper * GetItem(int seq, int slot);

	void SendSeqInfo(int seq);
	void SendOtherInfo();

private:
	Role *m_role;
	CharIntAttrs m_attrs_add;
	RedEquipCollectParam m_param;

	RedEquipCollectInfo m_collect_info_list[RED_EQUIP_COLLECT_SEQ_MAX_COUNT];		// 套装收集个数
};

#endif // __REQ_EQUIP_COLLECT_HPP__