#ifndef __CAPABILITY_HPP__
#define __CAPABILITY_HPP__

#include "obj/character/attribute.hpp"

class Role;

enum CAPABILITY_TYPE 
{
	CAPABILITY_TYPE_INVALID = 0,

	CAPABILITY_TYPE_BASE,				// ��������ս����
	CAPABILITY_TYPE_MENTALITY,			// Ԫ������ս����
	CAPABILITY_TYPE_EQUIPMENT,			// װ������ս����
	CAPABILITY_TYPE_WING,				// ��������ս����
	CAPABILITY_TYPE_MOUNT,				// ��������ս����
	CAPABILITY_TYPE_TITLE,				// �ƺ�����ս����
	CAPABILITY_TYPE_SKILL,				// ��������ս����
	CAPABILITY_TYPE_XIANJIAN,			// �ɽ�����ս����
	CAPABILITY_TYPE_XIANSHU,			// ������������ս����
	CAPABILITY_TYPE_GEM,				// ��ʯս����
	CAPABILITY_TYPE_XIANNV,				// ��Ů����ս����
	CAPABILITY_TYPE_FOOTPRINT,			// �㼣����ս����
	CAPABILITY_TYPE_QINGYUAN,			// ��Ե����ս����
	CAPABILITY_TYPE_ZHANSHENDIAN,		// ս�������ս����
	CAPABILITY_TYPE_SHIZHUANG,			// ʱװ����ս����
	CAPABILITY_TYPE_ATTR_PER,			// �������԰ٷֱȼӵ�ս����
	CAPABILITY_TYPE_LIEMING,			// ����װ��ս��
	CAPABILITY_TYPE_JINGLING,			// ����ս��
	CAPABILITY_TYPE_VIPBUFF,			// vipbuffս��
	CAPABILITY_TYPE_SHENGWANG,			// ����
	CAPABILITY_TYPE_CHENGJIU,			// �ɾ�
	CAPABILITY_TYPE_WASH,				// ϴ��
	CAPABILITY_TYPE_SHENZHUANG,			// ��װ
	CAPABILITY_TYPE_TUHAOJIN ,			// ������ս��
	CAPABILITY_TYPE_BIG_CHATFACE,		// �����ս��
	CAPABILITY_TYPE_SHENZHOU_WEAPON,	// ��������ս����
	CAPABILITY_TYPE_BABY,				// ��������ս����
	CAPABILITY_TYPE_PET,				// ����ս��
	CAPABILITY_TYPE_ACTIVITY,			// ����������ս��
	CAPABILITY_TYPE_HUASHEN,			// ����ս��
	CAPABILITY_TYPE_MULTIMOUNT,			// ˫������ս��
	CAPABILITY_TYPE_PERSONALIZE_WINDOW,	//���������ս��
	CAPABILITY_TYPE_MAGIC_CARD,			// ħ��ս����
	CAPABILITY_TYPE_MITAMA,				// ����ս��
	CAPABILITY_TYPE_XUNZHANG,			// ѫ��ս��
	CAPABILITY_TYPE_ZHIBAO,				// ����ս��
	CAPABILITY_TYPE_HALO,				// �⻷����ս����
	CAPABILITY_TYPE_SHENGONG,			// ������ս����
	CAPABILITY_TYPE_SHENYI,				// ��������ս����
	CAPABILITY_TYPE_GUILD,				// ����ս����
	CAPABILITY_TYPE_CHINESE_ZODIAC,		// ����ϵͳս����
	CAPABILITY_TYPE_XIANNV_SHOUHU,		// ��Ů�ػ�ս����
	CAPABILITY_TYPE_JINGLING_GUANGHUAN,	// ����⻷ս����
	CAPABILITY_TYPE_JINGLING_FAZHEN,	// ���鷨��ս����
	CAPABILITY_TYPE_CARDZU,				// �������ս��
	CAPABILITY_TYPE_ZHUANSHENGEQUIP,    // ת������ս����
	CAPABILITY_TYPE_LITTLE_PET,			// С����ս��
	CAPABILITY_TYPE_ZHUANSHEN_RAND_ATTR ,// ת��װ���������
	CAPABILITY_TYPE_FIGHT_MOUNT,		// ս������ս����
	CAPABILITY_TYPE_MOJIE,				// ħ��
	CAPABILITY_TYPE_LOVE_TREE,			// ��˼��
	CAPABILITY_TYPE_EQUIPSUIT,			// ������װս����
	CAPABILITY_TYPE_RUNE_SYSTEM,		// ����ϵͳ
	CAPABILITY_TYPE_SHENGE_SYSTEM,		// ���ϵͳ
	CAPABILITY_TYPE_SHENBING,			// ���ϵͳ
	CAPABILITY_TYPE_FABAO,			    // ��������ս����
	CAPABILITY_TYPE_ROLE_GOAL,			// ��ɫĿ��
	CAPABILITY_TYPE_CLOAK,				// ��������ս����
	CAPABILITY_TYPE_SHENSHOU,			// ����ս����
	CAPABILITY_TYPE_IMG_FULING,			// ������
	CAPABILITY_TYPE_CSA_EQUIP,			// �Ϸ�װ��
	CAPABILITY_TYPE_SHEN_YIN,			// ��ӡ
	CAPABILITY_TYPE_ELEMENT_HEART,		// Ԫ��֮��ս��
	CAPABILITY_TYPE_FEIXIAN,			// ����ս����
	CAPABILITY_TYPE_SHIZHUANG_WUQI,		// ʱװ���������������ս����
	CAPABILITY_TYPE_CF_BEST_RANK_BREAK,	// ��������ʷ�������ͻ��
	CAPABILITY_TYPE_SHENGQI,			// ʥ��ϵͳ
	CAPABILITY_TYPE_JINGJIE,            // ����ϵͳ
	CAPABILITY_TYPE_TALENT,				// �츳ϵͳ
	CAPABILITY_TYPE_JINJIE_TALENT,		// �����츳ϵͳ
	CAPABILITY_TYPE_HESHENLUOSHU,		// ��������
	CAPABILITY_TYPE_IMP_GUARD,          // С���ػ�
	CAPABILITY_TYPE_ZHUANZHI,           // ����תְ
	CAPABILITY_TYPE_BOSS_CARD,          // bossͼ��
	CAPABILITY_TYPE_YAOSHI,				// ����ս��
	CAPABILITY_TYPE_TOUSHI,				// ͷ��ս��
	CAPABILITY_TYPE_QILINBI,			// �����ս��
	CAPABILITY_TYPE_MASK,				// ���ս��
	CAPABILITY_TYPE_ZHUANZHI_EQUIP,		// תְװ��ս��
	CAPABILITY_TYPE_ROLE_CROSS,			// ������
	CAPABILITY_TYPE_SEAL,				// ʥӡϵͳ
	CAPABILITY_TYPE_DRESSING_ROOM,		// �¹�ս��
	CAPABILITY_TYPE_SHENQI,				// ����ϵͳ
	CAPABILITY_TYPE_RED_EQUIP_COLLECT,  // ��װ�ռ�
	CAPABILITY_TYPE_ORANGE_EQUIP_COLLET,// ��װ�ռ�
	CAPABILITY_TYPE_MOLONG,             // ħ��
	CAPABILITY_TYPE_SPOUSEHOME,         // ���޼�԰
	CAPABILITY_TYPE_EQUIP_BAPTIZE,      // װ��ϴ��
	CAPABILITY_TYPE_MOJIE_SKILL,		// ħ�似��(class MojieSkill)
	CAPABILITY_TYPE_GREATE_SOLDIER,		// ����
	CAPABILITY_TYPE_BAIZHAN_EQUIP,		// ��սװ��ս��
	CAPABILITY_TYPE_XIANZUNKA,			// ����
	CAPABILITY_TYPE_MYTH,				// ��ϵͳ
	CAPABILITY_TYPE_ZODIAC,				// ʮ����Ф
	CAPABILITY_TYPE_CROSSEQUIP,			// ����װ��
	CAPABILITY_TYPE_TIANSHENHUTI,		// ��ĩװ��(������)ս��
	CAPABILITY_TYPE_TIANSHUXUNZHU,		// ����Ѱ��

	CAPABILITY_TYPE_UPGRADE_SYS_BEGIN = 200,	// ����ϵͳս�� (200 + upgrade_type)
	CAPABILITY_TYPE_UPGRADE_SYS_END = 230,

	CAPABILITY_TYPE_TOTAL,				// ��ս������(ս�������㷽ʽ��Ϊ����������ú����׹�ʽ���㣬ȡ������ģ��ֱ�����ټ������ķ�ʽ��

	CAPABILITY_TYPE_MAX,
};

class Capability
{
public:
	Capability();
	~Capability();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, int max_cap);

	void OnRoleReCalcAttrBegin(bool recalc_all);
	void OnRoleReCalcAttrEnd(bool notify = true, bool is_time_limit = false);
	void OnGetRoleCapa();

	void ReCalcCap(int cap_type, const CharIntAttrs &diff_int_attrs);
	void AddCap(int cap_type, int cap);
	void SetCap(int cap_type, int cap);

	int GetRoleCapability(int cap_type);
	int GetTotalCapability();
	int GetMaxCapablity() { return m_max_cap; }
	int GetOtherCapablity() { return m_other_cap; }

	static int GetCapByAttr(const CharIntAttrs &diff_int_attrs, bool is_total = false);

	void GmSetCapability(int capability);
	void SendRoleCapability();

	static int GetHistoryType(int cap_type);

	void OnDayChange();
	void CheckClearAddCap();

private:
	Role *m_role;
	
	int m_total_cap;
	int m_gm_total_cap;
	int m_max_cap;
	int m_other_cap;
	int m_cap_list[CAPABILITY_TYPE_MAX]; 
};

#endif // __CAPABILITY_HPP__

