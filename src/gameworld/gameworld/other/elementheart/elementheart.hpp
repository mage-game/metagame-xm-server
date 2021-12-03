#ifndef __CHARM_GHOST_HPP__
#define __CHARM_GHOST_HPP__

#include "obj/character/attribute.hpp"
#include "servercommon/elementheartdef.hpp"
#include "gameworld/protocal/msgelementheart.h"

class Role;

class ElemnetHeart
{
	static const int CHARM_ELEMENT_UP_SPEED_NEED_ITEM_NUM = 1;	// 代替加速消耗物品数量
	enum WUXING_TYPE_RELACTION																											// 五行属性关系
	{
		WUXING_TYPE_RELACTION_NORMAL,																									// 正常
		WUXING_TYPE_RELACTION_SAME,																										// 相同
		WUXING_TYPE_RELACTION_PROMOTE,																									// 相生
		WUXING_TYPE_RELACTION_RESTRAINT,																								// 相克
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

	void ReqOpera(int info_type, int param1, int param2, int param3, int param4);														// 操作请求

	void GmOpera(int type, int p1, int p2, int p3, int p4, int p5);
	void SendAllInfo();																													// 发送全部信息

	void SendShopInfo();

	// 装备转职装备,激活附纹
	void ActiveElementTexture(short equip_index);

protected:
	void ActiveElement(int id);																											// 激活元素
	void FeedElement(int id, int v_itme_id, int num);																						// 喂养元素
	void FeedElementOnekey(int id);																										// 喂养一键
	bool UpElementGrade(int id, bool is_one_key, bool is_auto_buy);																		// 元素进阶
	void GetProduct(int id);																											// 产出采集
	void ProductUpSpeed(int id);																										// 产出加速
	void ChouJiang(int times, bool is_use_score, bool is_double = false);																// 抽奖
	void ChangeElementWuxing(int id);
	void SetElementWuxing(int id);

	static WUXING_TYPE_RELACTION GetTypeRelaction(int src_type, int target_type);														// 五行对应关系
	bool IsActiveElement(int id) const;																									// 是否激活元素
	static float GetFeedPercentByType(WUXING_TYPE_RELACTION type);																		// 根据五行关系获得加成百分比
	
	bool ElementTextureUpgrade(int e_index, int grid_index);																			// 元素之纹升级

	void SendElementSingleInfo(int id, int info_type = Protocol::SCElementHeartInfo::INFO_TYPE_SINGLE_ELEMENT) const;					// 发送元素单个信息
	void SendAllElementInfo() const;																									// 发送元素全部信息
	
	void SendCharmInfo() const;																											// 元素之纹列表信息
	void SendSingleCharmInfo(int e_index) const;																						// 单个元素之纹信息

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
