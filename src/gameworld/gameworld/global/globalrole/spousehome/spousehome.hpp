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
	bool GetTargetSpouseHomeInfo(SpouseHomeRoomParam *target_room_info, int length);				// ��ȡ����������Ϣ

	void ReqOpera(int info_type, int param1, int param2, int param3, int param4);								// ��������
	void SpouseHomeEquip(int room_index, int furniture_type, int knapsack_index);					// װ��
	bool SpouseHomeTakeOff(int room_index, int slot_type, bool is_replace = false);					// ж��
	void OnBuySpecialRoom(int buy_theme_index);														// ���򷿼�
	void SendAllInfo();																				// ����������Ϣ
	void SendViewOtherPeopleRoom(int uid);															// ����������ҷ�����Ϣ
	void SendFriendCountInfo();																		// ���ͺ��Ѻ����˳�Ա����������Ϣ
	void SendGuildMemberInfo();
	void SendSpouseHomeSingleRoomInfo(int room_index);
	void SendSpouseHomeInfo() const;
	int GetTargetSpouseHomeRoomCount() { return m_param.open_room_count; }							// ��ȡĿ�귿������

	void OnBuyFurniture(ItemID item_id, int item_num);

	void OnBuySpecialRoomForLover(int lover_id, int buy_theme_index);												// Ϊ���¹��򷿼�
	void SpouseHomeEquipForLover(int lover_id, int room_index, int furniture_type, int knapsack_index);			    // Ϊ����װ���Ҿ�

private:
	void CheckCoupleRoomAddCapPer(bool is_need_recalc = false);

	CharIntAttrs m_attrs_add;
	SpouseHomeParam m_param;
};

#endif