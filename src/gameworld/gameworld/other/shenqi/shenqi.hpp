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
	bool IsActiveQualityShenbingSuit(int id, int quality);		// ��������װ�Ƿ�ﵽĳ��Ʒ��
	bool IsActiveQualityBaojiaSuit(int id, int quality);		// ��鱦����װ�Ƿ�ﵽĳ��Ʒ��
	int GetActiveQualityShenbingSuitCount(int quality);			// ���������װ����ĳ��Ʒ�ʵ�����
	int GetActiveQualityBaojiaSuitCount(int quality);			// ���㱦����װ����ĳ��Ʒ�ʵ�����
	int GetActiveQualityAllSuitCount(int quality);				// ��������ݼ׳���ĳ��Ʒ�ʵ�����
	bool IsAchieveShenbingSuitLevel(int id, int level);			// ��������װ�Ƿ�ﵽĳ���ȼ�
	bool IsAchieveBaojiaSuitLevel(int id, int level);			// ��鱦����װ�Ƿ�ﵽĳ���ȼ�
	short CheckBaojiaTeXiaoId(short cur_baojia_id);				// ��鱦����Чid

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

	bool CanShenbingUplevel(int id);								// ����������ײ�������
	bool CanBaojiaUplevel(int id);									// ���Ʊ��׳��ײ�������

	bool ActiveShenbingTeXiao(int shenqi_id, int quality);								// ���������Ч
	bool ActiveBaojiaTeXiao(int shenqi_id, int quality);								// �������Ч		
	bool IsActiveShenbingTeXiao(int id, int quality);
	bool IsActiveBaojiaTeXiao(int id, int quality);
	Role* m_role;
	CharIntAttrs m_attrs_add;

	ShenqiParam m_param;

	short m_cur_baojia_texiao_id;							// ��ǰ������������id
};

#endif