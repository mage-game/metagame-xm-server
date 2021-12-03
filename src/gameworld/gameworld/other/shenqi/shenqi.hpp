#ifndef __SHENQI_HPP__
#define __SHENQI_HPP__
#include "gameworld/obj/character/attribute.hpp"
#include "servercommon/shenqidef.hpp"

namespace Protocol
{
	struct CSShenqiOperaReq;
}
class Role;
class Shenqi
{
public:
	Shenqi();

	void SetRole(Role* role) { m_role = role; }
	void InitParam(const ShenqiParam &shenqi_param);
	void GetInitParam(ShenqiParam* shenqi_param);
	void ReCalcAttr(CharIntAttrs &base_attr, bool is_recalc);
	void Update(unsigned long interval, time_t now_second);

	void OnDie(Role *killer);
	void OnRealive();

	void GetRoleAppearance(RoleAppearance *appearance);

	void OnShenqiOperaReq(Protocol::CSShenqiOperaReq* sqor);
	void SendShenqiInfo();

	bool IsShenbingImageActive(const int image_id);
	bool IsBaojiaImageActive(const int image_id);
	bool IsActiveQualityShenbingSuit(int id, int quality);		// 检查神兵套装是否达到某个品质
	bool IsActiveQualityBaojiaSuit(int id, int quality);		// 检查宝甲套装是否达到某个品质
	int GetActiveQualityShenbingSuitCount(int quality);			// 计算神兵套装超过某个品质的数量
	int GetActiveQualityBaojiaSuitCount(int quality);			// 计算宝甲套装超过某个品质的数量
	int GetActiveQualityAllSuitCount(int quality);				// 计算奇兵遁甲超过某个品质的数量
	bool IsAchieveShenbingSuitLevel(int id, int level);			// 检查神兵套装是否达到某个等级
	bool IsAchieveBaojiaSuitLevel(int id, int level);			// 检查宝甲套装是否达到某个等级
	short CheckBaojiaTeXiaoId(short cur_baojia_id);				// 检查宝甲特效id

	const int GetShenbingLevel(int index);
	const int GetBaojiaLevel(int index);
	const char GetShenbingCurImageId();
	const char GetBaojiaCurImageId();

	const short GetCurBaojiaTeXiaoId() { return m_cur_baojia_texiao_id; } 
	const short GetCurShenbingTeXiaoid() { return m_param.shenbing_cur_texiao_id; }		

	void SetKillerTeXiaoeId(short image_id);

	void UseShenbingImage(const int image_id);
	void UseBaojiaImage(const int image_id);

private:

	void Decompose(int item_id, int num);

	void ShenbingInlay(int id, int part_type, int quality);
	bool ShenbingUplevel(int id);
	void UseShenbingTexiao(const int texiao_id);

	void BaojiaInlay(int id, int part_type, int quality);
	bool BaojiaUplevel(int id);
	void UseBaojiaTexiao(const int texiao_id);

	void SendSingleShenqiInfo(const short type, const short index);
	void SendImageInfo(const short type);

	bool CanShenbingUplevel(int id);								// 限制神兵成套才能升级
	bool CanBaojiaUplevel(int id);									// 限制宝甲成套才能升级

	bool ActiveShenbingTeXiao(int shenqi_id, int quality);								// 激活神兵特效
	bool ActiveBaojiaTeXiao(int shenqi_id, int quality);								// 激活宝甲特效		
	bool IsActiveShenbingTeXiao(int id, int quality);
	bool IsActiveBaojiaTeXiao(int id, int quality);
	Role* m_role;
	CharIntAttrs m_attrs_add;

	ShenqiParam m_param;

	short m_cur_baojia_texiao_id;							// 当前宝甲特殊现象id
};

#endif