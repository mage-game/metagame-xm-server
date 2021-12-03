#ifndef __HESHENLUOSHU_HPP__
#define __HESHENLUOSHU_HPP__

#include "servercommon/heshenluoshudef.hpp"
#include "gameworld/obj/character/attribute.hpp"
#include "obj/character/role.h"

class HeShengLuoShu
{
public:
	HeShengLuoShu();
	~HeShengLuoShu();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const HeShenLuoShuParam &heshen_luoshu_param);
	void GetInitParam(HeShenLuoShuParam * heshen_luoshu_param) const;

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnHandlerOpera(short opera_type, short param_1);
	void SendHeShenLuoShuAllInfo() const;
	void SendHeShenLuoShuChangeInfo(short param_1, short param_2, short param_3) const;

	int GetActiveLuoShuNum(int type, int color);					// 获取激活数量

	void PrintOperationLog();

private:
	void UseItemActivation(ItemID item_id);								//使用物品激活
	void UseItemUpgradeLevel(ItemID item_id);							//使用物品升星
	void ItemDecompose(ItemID item_id);									//物品分解

	Role *m_role;
	HeShenLuoShuParam m_heshen_luoshu_param;

	CharIntAttrs m_attrs_add;
};



#endif // __HESHENLUOSHU_HPP__