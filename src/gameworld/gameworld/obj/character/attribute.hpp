#ifndef __ATTRIBUTE_HPP__
#define __ATTRIBUTE_HPP__

#include <memory.h>
#include <map>

#include "gameworld/gameworld/gamedef.h"
#include "servercommon/servercommon.h"

#define ROLE_ATTR_PER 0.0001	// ��ɫ������صļӳɰٷֱȣ������������ֱȣ�
constexpr static float ROLE_ATTR_PER_F = 0.0001f;	// ��������double��˵��long long��float��˺���float���ͣ������ٴ�doubleת��float

class AttrConfig;

class CharIntAttrs
{
public:
	enum RECALC_REASON
	{
		RECALC_REASON_INVALID = 0,
		RECALC_REASON_SELF,														// ��ɫ����
		RECALC_REASON_MENTALITY,												// Ԫ��
		RECALC_REASON_MOUNT,													// ����
		RECALC_REASON_EQUIPMENT,												// װ��
		RECALC_REASON_TITLE,													// �ƺ�
		RECALC_REASON_SKILL,													// ����
		RECALC_REASON_GUILD_XIANSHU,											// ����
		RECALC_REASON_STONE,													// ��ʯ
		RECALC_REASON_XIANJIAN,													// �ɽ�
		RECALC_REASON_XIANNV,													// ��Ů
		RECALC_REASON_WING,														// ����
		RECALC_REASON_FOOTPRINT,												// �㼣
		RECALC_REASON_QINGYUAN,													// ��Ե
		RECALC_REASON_ZHANSHENDIAN,												// ս���
		RECALC_REASON_SHIZHUANG,												// ʱװ
		RECALC_REASON_CARD,														// ����
		RECALC_REASON_WUSHUANG_EQUIP,											// ��˫װ��
		RECALC_REASON_CARDZU,													// �������
		RECALC_REASON_BUFF,														// buff
		RECALC_REASON_LIEMING,													// ����
		RECALC_REASON_JINGLING,													// ����
		RECALC_REASON_VIPBUFF,													// vipbuff
		RECALC_REASON_SHENGWANG,												// ����
		RECALC_REASON_CHENGJIU,													// �ɾ�
		RECALC_REASON_WASH,														// ϴ��
		RECALC_REASON_IMG_FULING,												// ������												
		RECALC_REASON_CAMPEQUIP,												// ʦ��װ��
		RECALC_REASON_TUHAOJIN,													// ������
		RECALC_REASON_BIG_CHATFACE,												// �����
		RECALC_REASON_SHENZHOU_WEAPON,											// ��������
		RECALC_REASON_BABY,														// ����
		RECALC_REASON_PET,														// ����
		RECALC_REASON_RAND_ACTIVITY,											// ����
		RECALC_REASON_HUASHEN,													// ����
		RECALC_REASON_PASTURE_SPIRIT,											// ��������
		RECALC_REASON_MULTI_MOUNT,												// ˫������
		RECALC_REASON_PERSONALIZE_WINDOW,										// ���������
		RECALC_REASON_MAGIC_CARD,												// ħ��
		RECALC_REASON_WUSHANG_EQUIP,											// �������
		RECALC_REASON_MITAMA,													// ����
		RECALC_REASON_FAIRY_TREE,												// ����
		RECALC_REASON_MAGIC_EQUIPMENT,											// ħ��
		RECALC_REASON_XUNZHANG,													// ѫ��
		RECALC_REASON_ZHIBAO,													// ����
		RECALC_REASON_HALO,														// �⻷
		RECALC_REASON_SHENGONG,													// ��
		RECALC_REASON_SHENYI,													// ����
		RECALC_REASON_GUILD,													// ����
		RECALC_REASON_CHINESE_ZODIAC,											// ����ϵͳ	
		RECALC_REASON_XIANNV_SHOUHU,											// ��Ů�ػ�
		RECALC_REASON_JINGLING_GUANGHUAN,										// ����⻷
		RECALC_REASON_JINGLING_FAZHEN,											// ���鷨��
		RECALC_REASON_ZHUANSHENGEQUIP,											// ת��
		RECALC_REASON_LITTLE_PET,												// С����
		RECALC_REASON_FIGHT_MOUNT,												// ս������
		RECALC_REASON_MOJIE,													// ħ��
		RECALC_REASON_LOVE_TREE,												// ��˼��
		RECALC_REASON_EQUIP_SUIT,												// ������װ
		RECALC_REASON_SHEN_ZHUANG,												// ��װ
		RECALC_RESON_RUNE_SYSTEM,												// ����ϵͳ
		RECALC_REASON_SHENGE_SYSTEM,											// ���ϵͳ
		RECALC_REASON_SHENBING,													// ���ϵͳ
		RECALC_REASON_ROLE_GOAL,												// ��ɫĿ��
		RECALC_REASON_CLOAK,													// ����
		RECALC_REASON_SHENSHOU,													// ����
		RECALC_REASON_CSA_EQUIP,												// �Ϸ�װ��
		RECALC_REASON_SHEN_YIN,													// ��ӡ
		RECALC_REASON_ELEMENT_HEART,											// Ԫ��֮��
		RECALC_REASON_FABAO,											        // ����
		RECALC_REASON_FEIXIAN,													// ����
		RECALC_REASON_CHALLENGE_FIELD_BEST_RANK_BREAK,							// ��������ʷ���ͻ��
		RECALC_REASON_SHENGQI,													// ʥ��ϵͳ
		RECALC_REASON_JINGJIE,													// ����ϵͳ
		RECALC_REASON_TALENT,													// �츳ϵͳ
		RECALC_REASON_JINJIE_TALENT,											// �����츳ϵͳ
		RECALC_REASON_HESHEN_LUOSHU,											// ��������
		RECALC_REASON_IMP_GUARD,											    // С���ػ�
		RECALC_REASON_ZHUANZHI,                                                 // ����תְ
		RECALC_REASON_BOSS_CARD,                                                // bossͼ��
		RECALC_REASON_YAOSHI,													// ����
		RECALC_REASON_TOUSHI,													// ͷ��
		RECALC_REASON_QILINBI,													// �����
		RECALC_REASON_MASK,														// ���
		RECALC_REASON_ZHUANZHI_EQUIP,											// תְװ��
		RECALC_REASON_ROLE_CROSS,												// ������
		RECALC_REASON_SEAL,														// ʥӡϵͳ
		RECALC_REASON_DRESSING_ROOM,											// �¹�
		RECALC_REASON_SHENQI,													// ����
		RECALC_REASON_RED_EQUIP_COLLECT,                                        // ��װ�ռ�
		RECALC_REASON_ORANGE_EQUIP_COLLECT,										// ��װ�ռ�
		RECALC_REASON_MOLONG,                                                   // ħ��
		RECALC_REASON_SPOUSE_HOME,                                              // ���޼�԰
		RECALC_REASON_EQUIP_BAPTIZE,                                            // װ��ϴ��
		RECALC_REASON_MOJIE_SKILL,												// ħ�似��(class MojieSkill)
		RECALC_REASON_GREATE_SOLDIER,											// ����
		RECALC_REASON_TIANSHENHUTI,												// ��ĩװ��(������)
		RECALC_REASON_BAIZHAN_EQUIP,											// ��սװ��
		RECALC_REASON_XIANZUNKA,												// ����
		RECALC_REASON_MYTH,														// ��ϵͳ
		RECALC_REASON_ZODIAC,													// ʮ����Ф	
		RECALC_REASON_CROSSEQUIP,												// ����װ��
		RECALC_REASON_TIANSHUXUNZHU,											// ����Ѱ��

		RECALC_REASON_UPGRADE_SYS_BEGIN = 200,									// ����ϵͳ0��200 + upgrade_type������
		RECALC_REASON_UPGRADE_SYS_1 = 201,										// ����ϵͳ1�ɱ�
		RECALC_REASON_UPGRADE_SYS_2 = 202,										// ����ϵͳ2��ͯ
		RECALC_REASON_UPGRADE_SYS_3 = 203,										// ����ϵͳ3�鹭
		RECALC_REASON_UPGRADE_SYS_4 = 204,										// ����ϵͳ4����
		RECALC_REASON_UPGRADE_SYS_5 = 205,										// ����ϵͳ5β��
		RECALC_REASON_UPGRADE_SYS_6 = 206,										// ����ϵͳ6�ֻ�
		RECALC_REASON_UPGRADE_SYS_7 = 207,										// ����ϵͳ7β��
		RECALC_REASON_UPGRADE_SYS_8 = 208,										// ����ϵͳ8�ɳ�
		RECALC_REASON_UPGRADE_SYS_END = 230,									// 

		RECALC_REASON_MAX,
	};

	struct AttrPair
	{
		AttrPair() : attr_type(0), attr_value(0) {}
		AttrPair(int _attr_type, long long _attr_value) : attr_type(_attr_type), attr_value(_attr_value) {}

		int			attr_type;
		long long	attr_value;
	};

	enum ATTR_NOTIFY_REASON
	{
		ATTR_NOTIFY_REASON_NORMAL = 0,											// �������Ա䶯
		ATTR_NOTIFY_REASON_RESET_CHARACTER,										// �����л� �������ڳ�
		ATTR_NOTIFY_REASON_MOUNT,												// ��������
		ATTR_NOTIFY_REASON_LONGJUMP,											// ����
	};

	enum CHARINTATTR_TYPE
	{	
		CHARINTATTR_TYPE_INVALID = 0,

		///////////////////////////////////////////////////////////////////////////////////////////////////
		FIGHT_CHARINTATTR_TYPE_BEGIN = CHARINTATTR_TYPE_INVALID,

		FIGHT_CHARINTATTR_TYPE_GLOBAL_COOLDOWN,									// ȫ��cooldownʱ��

		FIGHT_CHARINTATTR_TYPE_HP,												// Ѫ��
		FIGHT_CHARINTATTR_TYPE_MP,												// ħ��

		FIGHT_CHARINTATTR_TYPE_MAXHP,											// ���Ѫ��
		FIGHT_CHARINTATTR_TYPE_MAXMP,											// ���ħ��

		FIGHT_CHARINTATTR_TYPE_GONGJI,											// ����
		FIGHT_CHARINTATTR_TYPE_FANGYU,											// ����
		FIGHT_CHARINTATTR_TYPE_MINGZHONG,										// ����
		FIGHT_CHARINTATTR_TYPE_SHANBI,											// ����
		FIGHT_CHARINTATTR_TYPE_BAOJI,											// ����
		FIGHT_CHARINTATTR_TYPE_JIANREN,											// ���ͣ�������
		FIGHT_CHARINTATTR_TYPE_MOVE_SPEED,										// �ƶ��ٶ�
		FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI,									// �����˺���Ů�񹥻���
		FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI,									// �ֿ��˺�
		
		FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN,									// ��׼���Ƽ��ʣ�
		FIGHT_CHARINTATTR_TYPE_PER_BAOJI,										// ������
		FIGHT_CHARINTATTR_TYPE_PER_KANGBAO,										// ������
		FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT,									// �����˺���ֱ�
		FIGHT_CHARINTATTR_TYPE_PER_KANGBAO_HURT,								// �����˺��ֿ���ֱ�
		FIGHT_CHARINTATTR_TYPE_PER_POFANG,										// �Ʒ���ֱȣ������ʣ����ͻ��˵��˺��ӳɣ�
		FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG,									// ������ֱȣ������ʣ����ͻ��˵��˺����⣩

		FIGHT_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG,								// �̶�����
		FIGHT_CHARINTATTR_TYPE_CONSTANT_MIANSHANG,								// �̶�����

		FIGHT_CHARINTATTR_TYPE_HUIXINYIJI,										// ����һ��
		FIGHT_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER,								// ����һ���˺���

		FIGHT_CHARINTATTR_TYPE_ZHUFUYIJI_PER,									// ף��һ����

		FIGHT_CHARINTATTR_TYPE_GEDANG_PER,										// ����
		FIGHT_CHARINTATTR_TYPE_GEDANG_DIKANG_PER,								// �񵲵ֿ���
		FIGHT_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER,							// �񵲼�����ֱ�

		FIGHT_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER,								// ����������ֱ�
		FIGHT_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER,								// ���ܼ�����ֱ�

		FIGHT_CHARINTATTR_TYPE_MINGZHONG_PER,									// ������
		FIGHT_CHARINTATTR_TYPE_SHANBI_PER,										// ������

		FIGHT_CHARINTATTR_TYPE_MAX,												// 

		///////////////////////////////////////////////////////////////////////////////////////////////////
		BASE_CHARINTATTR_TYPE_BEGIN = 40,

		BASE_CHARINTATTR_TYPE_MAXHP,											// �������Ѫ��
		BASE_CHARINTATTR_TYPE_MAXMP,											// �������ħ��

		BASE_CHARINTATTR_TYPE_GONGJI,											// ��������
		BASE_CHARINTATTR_TYPE_FANGYU,											// ��������
		BASE_CHARINTATTR_TYPE_MINGZHONG,										// ��������
		BASE_CHARINTATTR_TYPE_SHANBI,											// ��������
		BASE_CHARINTATTR_TYPE_BAOJI,											// ��������
		BASE_CHARINTATTR_TYPE_JIANREN,											// �������ͣ�������
		BASE_CHARINTATTR_TYPE_MOVE_SPEED,										// �����ƶ��ٶ�
		BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI,									// �����˺���Ů�񹥻���
		BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI,									// �ֿ��˺�

		BASE_CHARINTATTR_TYPE_PER_JINGZHUN,										// ��׼���Ƽ��ʣ�

		BASE_CHARINTATTR_TYPE_PER_BAOJI,										// ������
		BASE_CHARINTATTR_TYPE_PER_KANGBAO,										// ������
		BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT,									// �����˺���ֱ�
		BASE_CHARINTATTR_TYPE_PER_KANGBAO_HURT,									// �����˺��ֿ���ֱ�

		BASE_CHARINTATTR_TYPE_PER_POFANG,										// �Ʒ���ֱȣ������ʣ����ͻ��˵��˺��ӳɣ�
		BASE_CHARINTATTR_TYPE_PER_MIANSHANG,									// ������ֱȣ������ʣ����ͻ��˵��˺����⣩

		BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG,								// �̶�����
		BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG,								// �̶�����

		BASE_CHARINTATTR_TYPE_HUIXINYIJI,										// ����һ����
		BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER,								// ����һ���˺���

		BASE_CHARINTATTR_TYPE_ZHUFUYIJI_PER,									// ף��һ����

		BASE_CHARINTATTR_TYPE_GEDANG_PER,										// ����
		BASE_CHARINTATTR_TYPE_GEDANG_DIKANG_PER,								// �񵲵ֿ���
		BASE_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER,								// �񵲼�����ֱ�

		BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER,								// ����������ֱ�
		BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER,								// ���ܼ�����ֱ�

		BASE_CHARINTATTR_TYPE_MINGZHONG_PER,									// ����������
		BASE_CHARINTATTR_TYPE_SHANBI_PER,										// ����������

		SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER,										// pvp������ֱ�
		SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER,                                     // pvp������ֱ�
		SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER,                                     // pve������ֱ�
		SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER,                                     // pve������ֱ�
		SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER,											// ����ս����ֱ�

		BASE_CHARINTATTR_TYPE_MAX,												// 

		///////////////////////////////////////////////////////////////////////////////////////////////////
		FIRST_ATTR_TYPE_BEGIN = 80,

		FIRST_ATTR_TYPE_MAX,													// 

		///////////////////////////////////////////////////////////////////////////////////////////////////

		SPEICAL_CHARINTATTR_TYPE_BEGIN = 145,

		//SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER = SPEICAL_CHARINTATTR_TYPE_BEGIN,	// pvp������ֱ�
		//SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER,                                     // pvp������ֱ�
		//SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER,                                     // pve������ֱ�
		//SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER,                                     // pve������ֱ�
		//SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER,											// ����ս����ֱ�

		SPEICAL_CHARINTATTR_TYPE_END,

		CHARINTATTR_TYPE_MAX,
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	static inline bool IsFightAttr(int attr_type, bool include_hpmp_attr)		// �Ƿ�Ϊս������
	{
		if ((FIGHT_CHARINTATTR_TYPE_BEGIN < attr_type && attr_type < FIGHT_CHARINTATTR_TYPE_MAX) ||
			(SPEICAL_CHARINTATTR_TYPE_BEGIN <= attr_type && attr_type < SPEICAL_CHARINTATTR_TYPE_END)
			)
		{
			if (!include_hpmp_attr && IsHpMpAttr(attr_type)) return false;

			return true;
		}

		return false;
	}

	static inline bool IsRoleBaseAttr(int attr_type)							// �Ƿ�Ϊ��ɫ��������
	{
		if (BASE_CHARINTATTR_TYPE_BEGIN < attr_type && attr_type < BASE_CHARINTATTR_TYPE_MAX)
		{
			return true;
		}

		return false;
	}

	static inline bool IsHpMpAttr(int attr_type)								// �Ƿ���HpMp
	{
		if (FIGHT_CHARINTATTR_TYPE_HP == attr_type || FIGHT_CHARINTATTR_TYPE_MP == attr_type)
		{
			return true;
		}
		
		return false;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CharIntAttrs()
	{
		Reset();
	}
	~CharIntAttrs() {}

	void Reset()
	{
		memset(m_attrs, 0, sizeof(m_attrs));
		memset(m_percent_attrs, 0, sizeof(m_percent_attrs));
		memset(m_special_percent_attrs, 0, sizeof(m_special_percent_attrs));
	}

	Attribute GetAttr(int index) const 
	{
		if (index < 0 || index >= CHARINTATTR_TYPE_MAX) return 0;
		return m_attrs[index];
	}
	void SetAttr(int index, Attribute value)
	{
		if (index < 0 || index >= CHARINTATTR_TYPE_MAX) return;
		m_attrs[index] = value;
	}

	// ���ӵ�ϵͳ������ʱ������is_total = true
	void Add(Attribute attrs[CHARINTATTR_TYPE_MAX], bool is_total = false)
	{
		for (int i = 0; i < CHARINTATTR_TYPE_MAX; i++)
		{
			if (attrs[i] <= 0)
			{
				continue;
			}
			// 2019.03.15 �߻�Ҫ���ض���ֱ������ó˷�
			if (is_total)
			{
				switch (i)
				{
				case BASE_CHARINTATTR_TYPE_PER_POFANG:
				case SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER:
				case SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER:
				case BASE_CHARINTATTR_TYPE_PER_MIANSHANG:
				case SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER:
				case SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER:
					{
						double attr_value = static_cast<double>(attrs[i] * ROLE_ATTR_PER);
						if (attr_value > 0)
						{
							if (m_special_percent_attrs[i] > 0)
							{
								m_special_percent_attrs[i] *= attr_value;
							}
							else
							{
								m_special_percent_attrs[i] = attr_value;
							}
						}
					}
					break;

				default:
					{
						m_attrs[i] += attrs[i];
					}
					break;
				}
			}
			else
			{
				switch (i)
				{
				case BASE_CHARINTATTR_TYPE_PER_POFANG:
				case SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER:
				case SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER:
				case BASE_CHARINTATTR_TYPE_PER_MIANSHANG:
				case SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER:
				case SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER:
					{
						if (m_attrs[i] > 0)
						{
							m_attrs[i] *= attrs[i];
						}
						else
						{
							m_attrs[i] = attrs[i];
						}
					}
					break;

				default:
					{
						m_attrs[i] += attrs[i];
					}
					break;
				}
			}
		}
	}

	void AddAttrs(CharIntAttrs *char_int_attrs)
	{
		if (NULL != char_int_attrs)
		{
			this->Add(char_int_attrs->m_attrs);
		}
	}

	// ��������ֵ�뾡��ʹ�øýӿ�
	void AddValue(int index, Attribute value)
	{
		if (index < 0 || index >= CHARINTATTR_TYPE_MAX || value <= 0) return;

		switch (index)
		{
		case BASE_CHARINTATTR_TYPE_PER_POFANG:
		case SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER:
		case SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER:
			{
				double attr_value = static_cast<double>(1.0 + value * ROLE_ATTR_PER);
				if (attr_value > 0)
				{
					if (m_attrs[index] > 0)
					{
						double real_add_value = m_attrs[index] * attr_value;
						m_attrs[index] = static_cast<Attribute>(real_add_value);
					}
					else
					{
						m_attrs[index] = static_cast<Attribute>(attr_value * 10000.0f);
					}
				}
			}
			break;

		case BASE_CHARINTATTR_TYPE_PER_MIANSHANG:
		case SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER:
		case SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER:
			{
				double attr_value = static_cast<double>(1.0 - value * ROLE_ATTR_PER);
				if (attr_value > 0)
				{
					if (m_attrs[index] > 0)
					{
						double real_add_value = m_attrs[index] * attr_value;
						m_attrs[index] = static_cast<Attribute>(real_add_value);
					}
					else
					{
						m_attrs[index] = static_cast<Attribute>(attr_value * 10000.0f);
					}
				}
			}
			break;

		default:
			{
				m_attrs[index] += value;
			}
			break;
		}
	}

	bool AddPercentValue(int index, float percent_attrs)
	{
		if (index < 0 || index >= CHARINTATTR_TYPE_MAX || percent_attrs <= 0.0f)
		{
			return false;
		}
		m_percent_attrs[index] += percent_attrs;
		return true;
	}

	void AddPercent(float percent_attrs[CHARINTATTR_TYPE_MAX])
	{
		for (int i = 0; i < CHARINTATTR_TYPE_MAX; i++)
		{
			m_percent_attrs[i] += percent_attrs[i];
		}
	}

	void Set(Attribute attrs[CHARINTATTR_TYPE_MAX])
	{
		for (int i = 0; i < CHARINTATTR_TYPE_MAX; i++)
		{
			m_attrs[i] = attrs[i];
		}
	}

	static bool GetAttrTypeFromAttrString(std::string &attr_str, short *attr_type);

	void AddAttrFromConfig(const AttrConfig &cfg, double factor = 1.0);

	Attribute m_attrs[CHARINTATTR_TYPE_MAX];
	float m_percent_attrs[CHARINTATTR_TYPE_MAX];
	double m_special_percent_attrs[CHARINTATTR_TYPE_MAX];
};

#endif

