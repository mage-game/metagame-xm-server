#ifndef __CARD_ZU_H__
#define __CARD_ZU_H__

class Role;

#include "obj/character/attribute.hpp"
#include "servercommon/cardzudef.hpp"
#include <set>

class Cardzu
{
	enum BUY_LINGLI_TYPE
	{
		BUY_LINGLI_TYPE_COIN = 0,											// 铜币单抽
		BUY_LINGLI_TYPE_GOLD,												// 元宝单抽
		BUY_LINGLI_TYPE_GOLD10,												// 元宝十连抽
		BUY_LINGLI_TYPE_GOLD_BIND,											// 绑钻单抽
	};

public:
	Cardzu();
	~Cardzu();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const CardzuParam &param);
	void GetInitParam(CardzuParam *param);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SendCardzuAllInfo();												// 发送卡组所有信息
	void OnChouCardReq(int cardzu_id, int type);							// 抽卡
	void OnHualingReq(int card_id, int count);								// 请求化灵
	void OnLingzhuReq(int card_id);											// 请求灵铸
	void OnActiveZuheReq(int zuhe_id);										// 激活某个组合
	void OnUpgradeZuheReq(int zuhe_id, int target_level);					// 升级某个组合

protected:
	typedef std::set<int> ChangeCardSet;
	typedef std::set<int>::const_iterator ChangeCardSetIt;
	void OnNotifyChange(int zuhe_id = -1, ChangeCardSet *change_card_list = NULL); // 发送卡组改变信息
	void CheckMaxActiveCardzu(bool is_init_check);
	void OnCoinChouCard(int cardzu_id);										// 铜币单抽
	void OnGoldChouCard(int cardzu_id);										// 元宝单抽
	void OnGoldBatchChouCard(int cardzu_id);								// 元宝十连抽
	void OnGoldBindChouCard(int cardzu_id);									// 绑钻单抽

private:
	Role *m_role;
	CardzuParam m_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值
	int m_active_max_cardzu_id;												// 当前激活的最大卡组id
};

#endif

