#ifndef __BUFFMANAGER_HPP__
#define __BUFFMANAGER_HPP__

#include "servercommon/servercommon.h"

static const int SPECIAL_BUFF_MAX_BYTE = 16;

enum EFFECT_BUFF_TYPE
{
	EBT_INVALID = 0,

	EBT_XUANYUN = 1,										// 眩晕
	EBT_DINGSHEN = 2,										// 定身
	EBT_CHENMO = 3,											// 沉默
	EBT_CHIHUAN = 4,										// 迟缓

	EBT_MIANYI_XUANYUN = 5,									// 免疫眩晕
	EBT_MIANYI_DINGSHEN = 6,								// 免疫定身
	EBT_MIANYI_CHENMO = 7,									// 免疫沉默
	EBT_MIANYI_CHIHUAN = 8,									// 免疫迟缓

	EBT_WUDI = 9,											// 无敌
	EBT_JIASU = 10,											// 加速
	EBT_INVISIBLE = 11,										// 隐身
	EBT_NUQI_BAOFA = 12,									// 怒气爆发
	EBT_FIER = 13,											// 火

	EBT_REDUCE_GONGJI = 14,									// 降低攻击
	EBT_REDUCE_FANGYU = 15,									// 降低防御
	EBT_XIANNV_ADD_GONGJI = 16,								// 增加攻击-女神身上特效
	EBT_XIANNV_ADD_FANGYU = 18,								// 增加防御-女神身上特效
	EBT_XIANNV_ADD_BAOJI = 19,								// 女神暴击-女神身上特效
	EBT_WING_FENGDUN = 20,									// 羽翼风盾技能特效
	EBT_WING_JIEJIE = 21,									// 羽翼结界技能特效
	EBT_WING_SHOUHU = 22,									// 羽翼守护技能特效
	EBT_SHENYI_JIUSHU = 23,									// 神翼技能救赎
	EBT_SHENYI_SHENPAN = 24,								// 神翼技能审判
	EBT_SHENGGONG_ZHUMO = 25,								// 神弓技能诛魔
	EBT_REBOUNDHURT = 26,									// 反伤BUFF
	EBT_ADD_FANGYU = 27,									// 增加防御
	EBT_PINK_EQUIP_NARROW = 28,								// 粉色装备-缩小
	EBT_ADD_GONGJI = 29,									// 增加攻击

	EBT_SPECIAL_EFFECT_MAX = 30,							// 最大特殊技能buff

	EBT_SPECIAL_OTHER_BEGIN = 40,
	EBT_SUPER_MIANYI = EBT_SPECIAL_OTHER_BEGIN,				// 免疫定身，昏迷，沉默，击退 
	EBT_HPSTORE = 41,										// 护盾
	EBT_WUDI_PROTECT = 42,									// 无敌保护
	EBT_BIAN_XING = 43,										// 变形 ---- 可攻击
	EBT_BIANXING_FOOL = 44,									// 变形 ---- 不可攻击
	EBT_DEC_FANGYU = 45,									// 减少防御
	EBT_DEC_HURT = 46,										// 减免伤害
	EBT_BE_HURT_ADD = 47,									// 受到伤害加深
	EBT_MOJIESTORE = 48,									// 魔戒技能护盾特效
	EBT_TERRITORYWAR_TOWER_WUDI = 49,						// 领土战防御塔无敌
	EBT_DROP_BLOOD = 50,									// 流血
	EBT_ADD_BLOOD = 51,										// 加血
	EBT_FROZEN = 52,										// 冰冻
	EBT_DEADLY_ATTACK = 53,									// 致命一击
	EBT_MOHUA = 54,											// 形象赋灵-魔化
	EBT_TALENT_MOUNT_SKILL = 55,							// 天赋技能-坐骑
	EBT_TALENT_WING_SKILL = 56,								// 天赋技能-羽翼
	EBT_ADD_MULTI_ATTR = 57,								// 属性增益
	EBT_FORBID_RECOVER_HP = 58,								// 禁止回血
	EBT_DEC_CONTROL_BUFF_TIME = 59,							// 减少控制debuff的控制时间
	EBT_ZHUANZHI_ADD_SHANBI = 60,							// 转职技能加闪避
	EBT_ZHUANZHI_ADD_SHANGHAI = 61,							// 转职技能加伤害
	EBT_ZHUANZHI_ABSORB_HP = 62,							// 转职技能吸血
	EBT_DROP_BLOOD_POISON = 63,								// 仙女技能中毒失血
	EBT_XIANNV_RECOVER_HP = 64,								// 仙女技能恢复血量
	EBT_MOJIE_WUDI = 65,									// 魔戒技能无敌
	EBT_MOJIE_RESTORE_HP = 66,								// 魔戒技能回血
	EBT_MOJIESKILL_THUNDER = 67,							// 传世名剑雷电
	EBT_MOJIESKILL_RESTORE_HP = 68,							// 传世名剑回血
	EBT_MOJIESKILL_SLOW = 69,								// 传世名剑减速
	EBT_HUGUOZHILI = 70,									// 天神护体
	EBT_INVISIBLE_HURT_ABLE = 71,							// 隐身[可攻击]

	EBT_SPECIAL_OTHER_END = 72,

	// 以上段落用于带外形显示的 bu，不需要显示的加在下边

	EBT_ACTIVITY_BUFF_MIN = 160,						// 活动buff最小值

	EBT_ACTIVITY_BUFF_MAX,								// 活动buff最大值
	
	EBT_MAX,
};

UNSTD_STATIC_CHECK(EBT_SPECIAL_EFFECT_MAX <= 34);


#endif
