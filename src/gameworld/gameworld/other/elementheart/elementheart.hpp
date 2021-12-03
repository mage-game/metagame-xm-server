#ifndef __CHARM_GHOST_HPP__
#define __CHARM_GHOST_HPP__

#include "obj/character/attribute.hpp"
#include "servercommon/elementheartdef.hpp"
#include "gameworld/protocal/msgelementheart.h"

class Role;

class ElemnetHeart
{
	static const int CHARM_ELEMENT_UP_SPEED_NEED_ITEM_NUM = 1;	// �������������Ʒ����
	enum WUXING_TYPE_RELACTION																											// �������Թ�ϵ
	{
		WUXING_TYPE_RELACTION_NORMAL,																									// ����
		WUXING_TYPE_RELACTION_SAME,																										// ��ͬ
		WUXING_TYPE_RELACTION_PROMOTE,																									// ����
		WUXING_TYPE_RELACTION_RESTRAINT,																								// ���
	};

	enum GM_TYPE
	{
		GM_TYPE_TEST,
		GM_TYPE_ADD_ITEN,
	};

public:
	ElemnetHeart();
	~ElemnetHeart();

	void InitParam(Role *role, const ElementHeartParam &param);
	void GetInitParam(ElementHeartParam *param) const;
	void SetRole(Role *role) { m_role = role; }

	void Update(unsigned int now_second);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnDayChange(unsigned old_dayid, unsigned now_dayid);

	void ReqOpera(int info_type, int param1, int param2, int param3, int param4);														// ��������

	void GmOpera(int type, int p1, int p2, int p3, int p4, int p5);
	void SendAllInfo();																													// ����ȫ����Ϣ

	void SendShopInfo();

	// װ��תְװ��,�����
	void ActiveElementTexture(short equip_index);

protected:
	void ActiveElement(int id);																											// ����Ԫ��
	void FeedElement(int id, int v_itme_id, int num);																						// ι��Ԫ��
	void FeedElementOnekey(int id);																										// ι��һ��
	bool UpElementGrade(int id, bool is_one_key, bool is_auto_buy);																		// Ԫ�ؽ���
	void GetProduct(int id);																											// �����ɼ�
	void ProductUpSpeed(int id);																										// ��������
	void ChouJiang(int times, bool is_use_score, bool is_double = false);																// �齱
	void ChangeElementWuxing(int id);
	void SetElementWuxing(int id);

	static WUXING_TYPE_RELACTION GetTypeRelaction(int src_type, int target_type);														// ���ж�Ӧ��ϵ
	bool IsActiveElement(int id) const;																									// �Ƿ񼤻�Ԫ��
	static float GetFeedPercentByType(WUXING_TYPE_RELACTION type);																		// �������й�ϵ��üӳɰٷֱ�
	
	bool ElementTextureUpgrade(int e_index, int grid_index);																			// Ԫ��֮������

	void SendElementSingleInfo(int id, int info_type = Protocol::SCElementHeartInfo::INFO_TYPE_SINGLE_ELEMENT) const;					// ����Ԫ�ص�����Ϣ
	void SendAllElementInfo() const;																									// ����Ԫ��ȫ����Ϣ
	
	void SendCharmInfo() const;																											// Ԫ��֮���б���Ϣ
	void SendSingleCharmInfo(int e_index) const;																						// ����Ԫ��֮����Ϣ

	void ShopBuy(int shop_item_index);
	void ShopRefresh(bool is_use_score, bool is_auto = true);

	void PutEquip(int element_heart_type, int slot_index);
	bool UpgradeEquip(int element_heart_type, bool is_auto_buy);

	void XilianReq(int elemengt_id, int lock_flag, int xilian_comsume_color, bool is_auto_buy);
	void CheckOpenXiLianSlot(int elementid);
	void SendXiLianSingleInfo(int element_id);
	void SendXilianAllInfo();

	void OnOpenXilianSlot(int elementid, int slot_id);
	void OnRecycleEquip(int knapsack_index,  int count);

private:
	Role *m_role;

	int TransformXilianAttrType(int type);
	double GetXilianExtraPercent(int element_id);
	void OnChangeWuXing(int element_id, int from_type, int to_type);

	ElementHeartParam m_param;
	CharIntAttrs m_attrs_add;
};

#endif
