#ifndef __ATTRIBUTE_HPP__
#define __ATTRIBUTE_HPP__

#include <memory.h>
#include <map>

#include "gameworld/gameworld/gamedef.h"
#include "servercommon/servercommon.h"

#define ROLE_ATTR_PER 0.0001	// 角色属性相关的加成百分比，配置里配成万分比，
constexpr static float ROLE_ATTR_PER_F = 0.0001f;	// 相比上面的double来说，long long和float相乘后还是float类型，不用再从double转到float

class AttrConfig;

class CharIntAttrs
{
public:
	enum RECALC_REASON
	{
		RECALC_REASON_INVALID = 0,
		RECALC_REASON_SELF,														// 角色本身
		RECALC_REASON_MENTALITY,												// 元神
		RECALC_REASON_MOUNT,													// 坐骑
		RECALC_REASON_EQUIPMENT,												// 装备
		RECALC_REASON_TITLE,													// 称号
		RECALC_REASON_SKILL,													// 技能
		RECALC_REASON_GUILD_XIANSHU,											// 仙术
		RECALC_REASON_STONE,													// 宝石
		RECALC_REASON_XIANJIAN,													// 仙剑
		RECALC_REASON_XIANNV,													// 仙女
		RECALC_REASON_WING,														// 羽翼
		RECALC_REASON_FOOTPRINT,												// 足迹
		RECALC_REASON_QINGYUAN,													// 情缘
		RECALC_REASON_ZHANSHENDIAN,												// 战神殿
		RECALC_REASON_SHIZHUANG,												// 时装
		RECALC_REASON_CARD,														// 卡牌
		RECALC_REASON_WUSHUANG_EQUIP,											// 无双装备
		RECALC_REASON_CARDZU,													// 卡牌组合
		RECALC_REASON_BUFF,														// buff
		RECALC_REASON_LIEMING,													// 猎命
		RECALC_REASON_JINGLING,													// 精灵
		RECALC_REASON_VIPBUFF,													// vipbuff
		RECALC_REASON_SHENGWANG,												// 声望
		RECALC_REASON_CHENGJIU,													// 成就
		RECALC_REASON_WASH,														// 洗炼
		RECALC_REASON_IMG_FULING,												// 形象赋灵												
		RECALC_REASON_CAMPEQUIP,												// 师门装备
		RECALC_REASON_TUHAOJIN,													// 土豪金
		RECALC_REASON_BIG_CHATFACE,												// 大表情
		RECALC_REASON_SHENZHOU_WEAPON,											// 神州六器
		RECALC_REASON_BABY,														// 宝宝
		RECALC_REASON_PET,														// 宠物
		RECALC_REASON_RAND_ACTIVITY,											// 随机活动
		RECALC_REASON_HUASHEN,													// 化神
		RECALC_REASON_PASTURE_SPIRIT,											// 牧场精灵
		RECALC_REASON_MULTI_MOUNT,												// 双人坐骑
		RECALC_REASON_PERSONALIZE_WINDOW,										// 个性聊天框
		RECALC_REASON_MAGIC_CARD,												// 魔卡
		RECALC_REASON_WUSHANG_EQUIP,											// 无上神兵
		RECALC_REASON_MITAMA,													// 御魂
		RECALC_REASON_FAIRY_TREE,												// 仙树
		RECALC_REASON_MAGIC_EQUIPMENT,											// 魔器
		RECALC_REASON_XUNZHANG,													// 勋章
		RECALC_REASON_ZHIBAO,													// 至宝
		RECALC_REASON_HALO,														// 光环
		RECALC_REASON_SHENGONG,													// 神弓
		RECALC_REASON_SHENYI,													// 神翼
		RECALC_REASON_GUILD,													// 仙盟
		RECALC_REASON_CHINESE_ZODIAC,											// 星座系统	
		RECALC_REASON_XIANNV_SHOUHU,											// 仙女守护
		RECALC_REASON_JINGLING_GUANGHUAN,										// 精灵光环
		RECALC_REASON_JINGLING_FAZHEN,											// 精灵法阵
		RECALC_REASON_ZHUANSHENGEQUIP,											// 转生
		RECALC_REASON_LITTLE_PET,												// 小宠物
		RECALC_REASON_FIGHT_MOUNT,												// 战斗坐骑
		RECALC_REASON_MOJIE,													// 魔戒
		RECALC_REASON_LOVE_TREE,												// 相思树
		RECALC_REASON_EQUIP_SUIT,												// 锻造套装
		RECALC_REASON_SHEN_ZHUANG,												// 神装
		RECALC_RESON_RUNE_SYSTEM,												// 符文系统
		RECALC_REASON_SHENGE_SYSTEM,											// 神格系统
		RECALC_REASON_SHENBING,													// 神兵系统
		RECALC_REASON_ROLE_GOAL,												// 角色目标
		RECALC_REASON_CLOAK,													// 披风
		RECALC_REASON_SHENSHOU,													// 神兽
		RECALC_REASON_CSA_EQUIP,												// 合服装备
		RECALC_REASON_SHEN_YIN,													// 神印
		RECALC_REASON_ELEMENT_HEART,											// 元素之心
		RECALC_REASON_FABAO,											        // 法宝
		RECALC_REASON_FEIXIAN,													// 飞仙
		RECALC_REASON_CHALLENGE_FIELD_BEST_RANK_BREAK,							// 竞技场历史最高突破
		RECALC_REASON_SHENGQI,													// 圣器系统
		RECALC_REASON_JINGJIE,													// 境界系统
		RECALC_REASON_TALENT,													// 天赋系统
		RECALC_REASON_JINJIE_TALENT,											// 进阶天赋系统
		RECALC_REASON_HESHEN_LUOSHU,											// 河神洛书
		RECALC_REASON_IMP_GUARD,											    // 小鬼守护
		RECALC_REASON_ZHUANZHI,                                                 // 人物转职
		RECALC_REASON_BOSS_CARD,                                                // boss图鉴
		RECALC_REASON_YAOSHI,													// 腰饰
		RECALC_REASON_TOUSHI,													// 头饰
		RECALC_REASON_QILINBI,													// 麒麟臂
		RECALC_REASON_MASK,														// 面具
		RECALC_REASON_ZHUANZHI_EQUIP,											// 转职装备
		RECALC_REASON_ROLE_CROSS,												// 人物跨服
		RECALC_REASON_SEAL,														// 圣印系统
		RECALC_REASON_DRESSING_ROOM,											// 衣柜
		RECALC_REASON_SHENQI,													// 神器
		RECALC_REASON_RED_EQUIP_COLLECT,                                        // 红装收集
		RECALC_REASON_ORANGE_EQUIP_COLLECT,										// 橙装收集
		RECALC_REASON_MOLONG,                                                   // 魔龙
		RECALC_REASON_SPOUSE_HOME,                                              // 夫妻家园
		RECALC_REASON_EQUIP_BAPTIZE,                                            // 装备洗炼
		RECALC_REASON_MOJIE_SKILL,												// 魔戒技能(class MojieSkill)
		RECALC_REASON_GREATE_SOLDIER,											// 名将
		RECALC_REASON_TIANSHENHUTI,												// 周末装备(天神护体)
		RECALC_REASON_BAIZHAN_EQUIP,											// 百战装备
		RECALC_REASON_XIANZUNKA,												// 仙尊卡
		RECALC_REASON_MYTH,														// 神话系统
		RECALC_REASON_ZODIAC,													// 十二生肖	
		RECALC_REASON_CROSSEQUIP,												// 斗气装备
		RECALC_REASON_TIANSHUXUNZHU,											// 天书寻主

		RECALC_REASON_UPGRADE_SYS_BEGIN = 200,									// 进阶系统0（200 + upgrade_type）灵珠
		RECALC_REASON_UPGRADE_SYS_1 = 201,										// 进阶系统1仙宝
		RECALC_REASON_UPGRADE_SYS_2 = 202,										// 进阶系统2灵童
		RECALC_REASON_UPGRADE_SYS_3 = 203,										// 进阶系统3灵弓
		RECALC_REASON_UPGRADE_SYS_4 = 204,										// 进阶系统4灵骑
		RECALC_REASON_UPGRADE_SYS_5 = 205,										// 进阶系统5尾焰
		RECALC_REASON_UPGRADE_SYS_6 = 206,										// 进阶系统6手环
		RECALC_REASON_UPGRADE_SYS_7 = 207,										// 进阶系统7尾巴
		RECALC_REASON_UPGRADE_SYS_8 = 208,										// 进阶系统8飞宠
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
		ATTR_NOTIFY_REASON_NORMAL = 0,											// 正常属性变动
		ATTR_NOTIFY_REASON_RESET_CHARACTER,										// 特殊切换 如上下镖车
		ATTR_NOTIFY_REASON_MOUNT,												// 上下坐骑
		ATTR_NOTIFY_REASON_LONGJUMP,											// 长跳
	};

	enum CHARINTATTR_TYPE
	{	
		CHARINTATTR_TYPE_INVALID = 0,

		///////////////////////////////////////////////////////////////////////////////////////////////////
		FIGHT_CHARINTATTR_TYPE_BEGIN = CHARINTATTR_TYPE_INVALID,

		FIGHT_CHARINTATTR_TYPE_GLOBAL_COOLDOWN,									// 全局cooldown时间

		FIGHT_CHARINTATTR_TYPE_HP,												// 血量
		FIGHT_CHARINTATTR_TYPE_MP,												// 魔法

		FIGHT_CHARINTATTR_TYPE_MAXHP,											// 最大血量
		FIGHT_CHARINTATTR_TYPE_MAXMP,											// 最大魔法

		FIGHT_CHARINTATTR_TYPE_GONGJI,											// 攻击
		FIGHT_CHARINTATTR_TYPE_FANGYU,											// 防御
		FIGHT_CHARINTATTR_TYPE_MINGZHONG,										// 命中
		FIGHT_CHARINTATTR_TYPE_SHANBI,											// 闪避
		FIGHT_CHARINTATTR_TYPE_BAOJI,											// 暴击
		FIGHT_CHARINTATTR_TYPE_JIANREN,											// 坚韧（抗暴）
		FIGHT_CHARINTATTR_TYPE_MOVE_SPEED,										// 移动速度
		FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI,									// 附加伤害（女神攻击）
		FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI,									// 抵抗伤害
		
		FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN,									// 精准（破甲率）
		FIGHT_CHARINTATTR_TYPE_PER_BAOJI,										// 暴击率
		FIGHT_CHARINTATTR_TYPE_PER_KANGBAO,										// 抗暴率
		FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT,									// 暴击伤害万分比
		FIGHT_CHARINTATTR_TYPE_PER_KANGBAO_HURT,								// 暴击伤害抵抗万分比
		FIGHT_CHARINTATTR_TYPE_PER_POFANG,										// 破防万分比（增伤率）（客户端的伤害加成）
		FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG,									// 免伤万分比（免伤率）（客户端的伤害减免）

		FIGHT_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG,								// 固定增伤
		FIGHT_CHARINTATTR_TYPE_CONSTANT_MIANSHANG,								// 固定免伤

		FIGHT_CHARINTATTR_TYPE_HUIXINYIJI,										// 会心一击
		FIGHT_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER,								// 会心一击伤害率

		FIGHT_CHARINTATTR_TYPE_ZHUFUYIJI_PER,									// 祝福一击率

		FIGHT_CHARINTATTR_TYPE_GEDANG_PER,										// 格挡率
		FIGHT_CHARINTATTR_TYPE_GEDANG_DIKANG_PER,								// 格挡抵抗率
		FIGHT_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER,							// 格挡减伤万分比

		FIGHT_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER,								// 技能增伤万分比
		FIGHT_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER,								// 技能减伤万分比

		FIGHT_CHARINTATTR_TYPE_MINGZHONG_PER,									// 命中率
		FIGHT_CHARINTATTR_TYPE_SHANBI_PER,										// 闪避率

		FIGHT_CHARINTATTR_TYPE_MAX,												// 

		///////////////////////////////////////////////////////////////////////////////////////////////////
		BASE_CHARINTATTR_TYPE_BEGIN = 40,

		BASE_CHARINTATTR_TYPE_MAXHP,											// 基础最大血量
		BASE_CHARINTATTR_TYPE_MAXMP,											// 基础最大魔法

		BASE_CHARINTATTR_TYPE_GONGJI,											// 基础攻击
		BASE_CHARINTATTR_TYPE_FANGYU,											// 基础防御
		BASE_CHARINTATTR_TYPE_MINGZHONG,										// 基础命中
		BASE_CHARINTATTR_TYPE_SHANBI,											// 基础闪避
		BASE_CHARINTATTR_TYPE_BAOJI,											// 基础暴击
		BASE_CHARINTATTR_TYPE_JIANREN,											// 基础坚韧（抗暴）
		BASE_CHARINTATTR_TYPE_MOVE_SPEED,										// 基础移动速度
		BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI,									// 附加伤害（女神攻击）
		BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI,									// 抵抗伤害

		BASE_CHARINTATTR_TYPE_PER_JINGZHUN,										// 精准（破甲率）

		BASE_CHARINTATTR_TYPE_PER_BAOJI,										// 暴击率
		BASE_CHARINTATTR_TYPE_PER_KANGBAO,										// 抗暴率
		BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT,									// 暴击伤害万分比
		BASE_CHARINTATTR_TYPE_PER_KANGBAO_HURT,									// 暴击伤害抵抗万分比

		BASE_CHARINTATTR_TYPE_PER_POFANG,										// 破防万分比（增伤率）（客户端的伤害加成）
		BASE_CHARINTATTR_TYPE_PER_MIANSHANG,									// 免伤万分比（免伤率）（客户端的伤害减免）

		BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG,								// 固定增伤
		BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG,								// 固定免伤

		BASE_CHARINTATTR_TYPE_HUIXINYIJI,										// 会心一击率
		BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER,								// 会心一击伤害率

		BASE_CHARINTATTR_TYPE_ZHUFUYIJI_PER,									// 祝福一击率

		BASE_CHARINTATTR_TYPE_GEDANG_PER,										// 格挡率
		BASE_CHARINTATTR_TYPE_GEDANG_DIKANG_PER,								// 格挡抵抗率
		BASE_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER,								// 格挡减伤万分比

		BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER,								// 技能增伤万分比
		BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER,								// 技能减伤万分比

		BASE_CHARINTATTR_TYPE_MINGZHONG_PER,									// 基础命中率
		BASE_CHARINTATTR_TYPE_SHANBI_PER,										// 基础闪避率

		SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER,										// pvp减伤万分比
		SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER,                                     // pvp增伤万分比
		SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER,                                     // pve减伤万分比
		SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER,                                     // pve增伤万分比
		SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER,											// 技能战力万分比

		BASE_CHARINTATTR_TYPE_MAX,												// 

		///////////////////////////////////////////////////////////////////////////////////////////////////
		FIRST_ATTR_TYPE_BEGIN = 80,

		FIRST_ATTR_TYPE_MAX,													// 

		///////////////////////////////////////////////////////////////////////////////////////////////////

		SPEICAL_CHARINTATTR_TYPE_BEGIN = 145,

		//SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER = SPEICAL_CHARINTATTR_TYPE_BEGIN,	// pvp减伤万分比
		//SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER,                                     // pvp增伤万分比
		//SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER,                                     // pve减伤万分比
		//SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER,                                     // pve增伤万分比
		//SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER,											// 技能战力万分比

		SPEICAL_CHARINTATTR_TYPE_END,

		CHARINTATTR_TYPE_MAX,
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	static inline bool IsFightAttr(int attr_type, bool include_hpmp_attr)		// 是否为战斗属性
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

	static inline bool IsRoleBaseAttr(int attr_type)							// 是否为角色基础属性
	{
		if (BASE_CHARINTATTR_TYPE_BEGIN < attr_type && attr_type < BASE_CHARINTATTR_TYPE_MAX)
		{
			return true;
		}

		return false;
	}

	static inline bool IsHpMpAttr(int attr_type)								// 是否是HpMp
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

	// 增加单系统总属性时，需设is_total = true
	void Add(Attribute attrs[CHARINTATTR_TYPE_MAX], bool is_total = false)
	{
		for (int i = 0; i < CHARINTATTR_TYPE_MAX; i++)
		{
			if (attrs[i] <= 0)
			{
				continue;
			}
			// 2019.03.15 策划要求，特定万分比属性用乘法
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

	// 增加属性值请尽量使用该接口
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

