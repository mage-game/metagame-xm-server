#ifndef __SPOUSE_HOME__HPP__
#define __SPOUSE_HOME__HPP__

#include "global/virtualbag/virtualbag.hpp"
#include "obj/character/attribute.hpp"
#include "servercommon/spousehomedef.hpp"
#include "protocal/msgspousehome.hpp"
#include "global/globalrole/globalrole.hpp"

class SpouseHome : public IGlobalRoleItem
{
public:
	SpouseHome(GlobalRole *g_role);
	~SpouseHome();

	void Init(const SpouseHomeParam &param);
	void GetInit(SpouseHomeParam &param) const;
	void OnRoleLogin(Role *role);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc, Role *role);

	void GetTotalAttr(CharIntAttrs &tmp_attr, bool is_man_and_wife = false);
	void SetLoverId(int lover_id, bool is_calc = true);
	int GetLoverId() { return m_param.lover_id; }
	bool GetTargetSpouseHomeInfo(SpouseHomeRoomParam *target_room_info, int length);				// 获取房间所有信息

	void ReqOpera(int info_type, int param1, int param2, int param3, int param4);								// 操作请求
	void SpouseHomeEquip(int room_index, int furniture_type, int knapsack_index);					// 装备
	bool SpouseHomeTakeOff(int room_index, int slot_type, bool is_replace = false);					// 卸下
	void OnBuySpecialRoom(int buy_theme_index);														// 购买房间
	void SendAllInfo();																				// 发送所有信息
	void SendViewOtherPeopleRoom(int uid);															// 发送其他玩家房间信息
	void SendFriendCountInfo();																		// 发送好友和仙盟成员房间数量信息
	void SendGuildMemberInfo();
	void SendSpouseHomeSingleRoomInfo(int room_index);
	void SendSpouseHomeInfo() const;
	int GetTargetSpouseHomeRoomCount() { return m_param.open_room_count; }							// 获取目标房间数量

	void OnBuyFurniture(ItemID item_id, int item_num);

	void OnBuySpecialRoomForLover(int lover_id, int buy_theme_index);												// 为伴侣购买房间
	void SpouseHomeEquipForLover(int lover_id, int room_index, int furniture_type, int knapsack_index);			    // 为伴侣装备家具

private:
	void CheckCoupleRoomAddCapPer(bool is_need_recalc = false);

	CharIntAttrs m_attrs_add;
	SpouseHomeParam m_param;
};

#endif