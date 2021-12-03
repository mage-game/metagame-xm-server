#ifndef __SKILLEFFECT_HPP__
#define __SKILLEFFECT_HPP__

#include "common/tlvprotocol.h"
#include "skill/skilldef.hpp"
#include "effect/buffmanager.hpp"

class Character;
class Role;

class EffectBase
{
public:
	enum MERGE_RULE
	{
		MERGE_RULE_INVALID = -1,

		MERGE_RULE_MERGE,					// 角色相同 技能无关 效果类型相同：叠加
		MERGE_RULE_COUNTER,					// 角色相同 技能相同 计数类效果  ：叠加	

		MERGE_RULE_NOMERGE_1,				// 角色无关 技能无关 效果类型相同：取剩余时间最长的											  ------- 控制类

		MERGE_RULE_NOMERGE_2,				// 角色无关 技能相同 效果类型相同：等级高的替换低的，等级相同则取剩余时间最长的，否则忽略新的		
											// 角色无关 技能不同 效果类型相同：则共存。													  ------- 属性类

		MERGE_RULE_NOMERGE_3,				// 角色无关 技能无关 效果类型相同：数值高的替换低的，数值相同则取剩余时间最长的，否则忽略新的 ------- 速度类

		MERGE_RULE_NOMERGE_4,				// 角色相同 技能相同 效果类型相同：等级高的替换低的，等级相同则取剩余时间最长的，否则忽略新的
											// 角色和技能有一个不同			 ：则共存。                                                   ------- 持续伤害类
		MERGE_RULE_NOMERGE_5,				// 角色相同 技能相同 效果类型相同: 同等级下，时间累计, 新的等级大于旧的，替换，否则忽略新的。

		MERGE_RULE_NOMERGE_6,				// 角色相同 技能相同 效果类型相同: 同等级下，属性累计, 新的等级大于旧的，替换，否则忽略新的。

		MERGE_RULE_IGNORE,					// 忽略新的
	};

	enum EXIST_METHOD
	{
		EXIST_METHOD_MERGE = 0,									// 叠加
		EXIST_METHOD_REPLACE,									// 替换
		EXIST_METHOD_NOTHING,									// 共存
		EXIST_METHOD_IGNORE,									// 忽略
	};

	enum PRODUCT_METHOD
	{
		PRODUCT_METHOD_SKILL = 0,								// 技能释放
		PRODUCT_METHOD_SCENE_SKILL,								// 场景skill
		PRODUCT_METHOD_SYSTEM,									// 系统 用于BOSS加无敌状态等
		PRODUCT_METHOD_TRIGGER,									// 触发器
		PRODUCT_METHOD_FRIEND,									// 好友
		PRODUCT_METHOD_HUSONG,									// 护送
		PRODUCT_METHOD_REBOUNDHURT,								// 伤害反弹
		PRODUCT_METHOD_FAZHEN_HALO,								// 法阵和光环
		PRODUCT_METHOD_SHENSHI,									// 运送神石
		PRODUCT_METHOD_CROSS_XIULUO_TOWER_DUR_DEAD,				// 跨服修罗塔连败
		PRODUCT_METHOD_CROSS_XIULUO_TOWER_BUY_BUFF,				// 跨服修罗塔Buff
		PRODUCT_METHOD_CROSS_1V1,								// 跨服1V1
		PRODUCT_METHOD_NAME_COLOR,								// 红名
		PRODUCT_METHOD_GUILD_HALL,								// 仙盟建筑
		PRODUCT_METHOD_ITEM,									// 物品
		PRODUCT_METHOD_GONGCHENGZHAN,							// 攻城战
		PRODUCT_METHOD_SKILL_READDING,							// 技能读条释放
		PRODUCT_METHOD_JINGHUA_HUSONG,							// 精华护送
		PRODUCT_METHOD_CROSS_TEAM_FB_BUFF,						// 跨服组队本
		PRODUCT_METHOD_XIANNV_SHENGWU_SKILL,					// 仙女圣物技能
		PRODUCT_METHOD_IMG_FULING,								// 形象赋灵
		PRODUCT_METHOD_CROSS_PASTURE_ANIMAL_DEC_SPEED,			// 牧场动物减速
		PRODUCT_METHOD_BIANSHEN_TASK,							// 变身任务
		PRODUCT_METHOD_TALENT_SKILL,							// 天赋技能
		PRODUCT_METHOD_IMP_GURAD_EXP,                           // 守护小鬼 - 经验加成
		PRODUCT_METHOD_ZHUANZHI_SKILL,							// 转职技能
		PRODUCT_METHOD_LINGCHONG,								// 灵宠普攻
		PRODUCT_METHOD_GCZCZ_SKILL,								// 攻城战秒杀技能	
		PUODUCT_METHOD_GCZ_SKILL,								// 攻城战技能城主效果
		PUODUCT_METHOD_SHENZHOU_WEAPON_SKILL,					// 异火技能
		PRODUCT_METHOD_MOJIE_SKILL,								// 爬塔-魔戒技能
		PRODUCT_METHOD_PER_BAOJI,								// 暴击率
		PRODUCT_METHOD_PER_SHANBI,								// 闪避率
	};

	// 以下productid为自定义的id.为确保绝对安全。不应该与技能id重复
	enum PRODUCT_ID
	{
		PRODUCT_ID_GATHER_SPEED = 10000,		// 采集速度上升
		PRODUCT_ID_GATHER_NOT_STOP,				// 采集不打断
		PRODUCT_ID_TO_BUILDINF_INC_HURT,		// 对建筑物提升伤害
		PRODUCT_ID_HP_CONTINUE,					// 气血持续回复
		PRODUCT_ID_MP_CONTINUE,					// mp持续回复
		PRODUCT_ID_INC_PUTON_LEVEL,				// 增加穿戴等级
		PRODUCT_ID_INC_EXTRAL_EXP_PER,			// 经验额外加成百分比
		PRODUCT_ID_INC_INHURT_ON_ATK,			// 攻击时提高别人伤害
		PRODUCT_ID_HP_DAN,						// 气血丹回复气血
		PRODUCT_ID_SZ_PROTECT,					// 神装保护
		PRODUCT_ID_JINGHUAHUSONG,				// 精华护送
		PRODUCT_ID_NAMECOLOR,					// 红名
		PRODUCT_ID_GREATE_SOLDIER_PASSIVE_SKILL,// 名将被动技能
		PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL,// 名将特殊技能
		PRODUCT_ID_HUGUOZHILI,					// 天神护体

		PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL1,// 周末装备主动技能1
		PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL2,// 周末装备主动技能2
		PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL3,// 周末装备主动技能3

		PRODUCT_ID_GCZ_ZHIMINGYIJI = 10100,		// 攻城战：致命一击
		PRODUCT_ID_GCZ_YOUDIANYIJI,				// 攻城战：弱点一击
		PRODUCT_ID_GCZ_POZHENGYIJI,				// 攻城战：破阵一击
		PRODUCT_ID_GCZ_NEXT_CHENGMO,			// 攻城战：下一次攻击沉默
		PRODUCT_ID_GCZ_NEXT_DINGSHENG,			// 攻城战：下一次攻击定身
		PRODUCT_ID_GCZ_NEXT_XUANYUN,			// 攻城战：下一次攻击眩晕

		PRODUCT_ID_BASE_ATTR_BEGIN = 10200,
		PRODUCT_ID_BASE_ATTR_ADD_JINGZHUN,		// 增加精准固定值
		PRODUCT_ID_BASE_ATTR_ADD_BAOJI,			// 增加暴击固定值
		PRODUCT_ID_BASE_ATTR_END,

		PRODUCT_ID_IMP_GURAD_EXP,              // 守护小鬼 - 经验

		PRODUCT_ID_MASK_FANQIAO,			   // 面具

		PRODUCT_ID_MOJIE_SKILL,					// 魔戒技能(是MojieSkill，不是MojieSkillImpl)

		PRODUCT_ID_CONTINUE_KILL,				// 连斩

		PRODUCT_ID_DAILY_TASK_ACCEPT,			// 日常任务
	};

	enum CMP_RET
	{
		CMP_RET_BIGGER,											// 大于
		CMP_RET_EQUAL,											// 相等
		CMP_RET_SMALLER,										// 小于
		CMP_RET_NOTHING,										// 无关
	};

	enum FIGHT_TYPE
	{
		FIGHT_TYPE_SHANBI = 0,									// 闪避
		FIGHT_TYPE_NORMAL,										// 正常攻击
		FIGHT_TYPE_GEDANG,										// 格挡
		FIGHT_TYPE_BAOJI,										// 暴击
		FIGHT_TYPE_MIANYI,										// 免疫
		FIGHT_TYPE_DIKANG,										// 抵抗
		FIGHT_TYPE_POJI,										// 破击
		FIGHT_TYPE_IGNORE_DEFENSE,								// 攻击-无视防御
		FIGHT_TYPE_SHEHUN_DUOPO,								// 摄魂夺魄
		FIGHT_TYPE_TALENT_ZHIXIN,								// 天赋炙心
		FIGHT_TYPE_TALENT_XIXING,								// 天赋吸星
		FIGHT_TYPE_TALENT_ZHENDANGFAJIAN,						// 天赋震荡法箭
		FIGHT_TYPE_TALENT_QIAODUOTIANGONG,						// 天赋巧夺天工
		FIGHT_TYPE_TALENT_ZHUIHUNDUOHUN,						// 天赋追魂夺魄
		FIGHT_TYPE_HUIXINYIJI,									// 会心一击
		FIGHT_TYPE_LINGCHONG,									// 灵宠伤害
		FIGHT_TYPE_GREATE_SOLDIER,								// 名将变身
	};

	enum PASSIVE_FLAG
	{
		PASSIVE_FLAG_ZHIBAO_TIANLEI,							// 至宝天雷
		PASSIVE_FLAG_ZHIBAO_NULEI,								// 至宝怒雷
		PASSIVE_FLAG_MOUNT_JIANTA,								// 坐骑践踏
		PASSIVE_FLAG_MOUNT_NUTA,								// 坐骑怒踏
		PASSIVE_FLAG_SHENGONG_SHENGA,							// 神弓神罚
		PASSIVE_FLAG_SHENGONG_GUANGJIAN,						// 神弓光箭
		PASSIVE_FLAG_SHENGONG_ZHUMO,							// 神弓诛魔
		PASSIVE_FLAG_SHENYI_HUANYING,							// 神翼幻影
		PASSIVE_FLAG_XIANNV_1,									// 仙女1
		PASSIVE_FLAG_XIANNV_2,									// 仙女2
		PASSIVE_FLAG_XIANNV_3,									// 仙女3
		PASSIVE_FLAG_XIANNV_4,									// 仙女4
		PASSIVE_FLAG_XIANNV_5,									// 仙女5
		PASSIVE_FLAG_XIANNV_6,									// 仙女6
		PASSIVE_FLAG_XIANNV_7,									// 仙女7
		PASSIVE_FLAG_JING_LING_LEI_TING,						// 精灵 雷霆一怒
		PASSIVE_FLAG_JING_LING_XI_XUE,							// 精灵 血之盛宴
	};

	enum EFFECT_TYPE
	{
		EFFECT_TYPE_INVALID = -1,

		EFFECT_TYPE_ATTRBUFF,									// 修改属性类，可同时最多改3种
		EFFECT_TYPE_SPECIAL_STATE,								// 控制类，4职业晕，定身，沉默，各种免疫，暴走，无敌等
		EFFECT_TYPE_INTERVAL_HP,								// 定时改HP
		EFFECT_TYPE_COUNTER,									// 计数器
		EFFECT_TYPE_INC_HURT,									// 伤害加深
		EFFECT_TYPE_DEC_HURT,									// 伤害减弱
		EFFECT_TYPE_DOUBLE_HURT,								// 二次伤害
		EFFECT_TYPE_MOVESPEED,									// 速度类，加或减
		EFFECT_TYPE_HPSTORE,									// 护盾
		EFFECT_TYPE_BOMB,										// 炸弹
		EFFECT_TYPE_WUDI_PROTECT,								// 无敌保护	受到的所有伤害都为1
		EFFECT_TYPE_MODIFY_GLOBAL_CD,							// 修改全局CD
		EFFECT_TYPE_REBOUND_HURT,								// 伤害反弹
		EFFECT_TYPE_RECOVER_ON_ATKED,							// 被击回血
		EFFECT_TYPE_GRADUAL_HURT,								// 渐增伤害
		EFFECT_TYPE_JUHUAPIG,									// 菊花猪
		EFFECT_TYPE_INVISIBLE,									// 隐身
		EFFECT_TYPE_ATTR_RAND_INC_HURT,							// 加属性并随机伤害加深
		EFFECT_TYPE_BIANSHEN,									// 变身
		EFFECT_TYPE_MP_NO_COST,									// 内力零消耗
		EFFECT_TYPE_SHENBING,									// 神兵
		EFFECT_TYPE_ABSORB_HP,									// 吸血
		EFFECT_TYPE_ATTR_PER,									// 修改属性百分比
		EFEECT_TYPE_OTHER,										// 其他buff
		EFFECT_TYPE_ATTR_CONTAINER,								// 容器类
		EFFECT_TYPE_ATTR_STORE,									// 存储类
		EFFECT_TYPE_FIRE,										// 火，烧伤自己或周围的人
		EFFECT_TYPE_SPECIAL_SHIELD,								// 护盾，受到伤害掉固定血
		EFFECT_TYPE_DEADLY_ATTACK,								// 致命一击，被打者掉固定百分比血量
		EFFECT_TYPE_ABSORB_HP_BEEN_ATTACK,						// 被击中时回血
		EFFECT_TYPE_TALENT_HPSTORE,								// 吸收所有伤害, 护盾消失时恢复吸收伤害的%
		EFFECT_TYPE_FORBID_RECOVER,								// 禁止恢复

		EFFECT_TYPE_RESERVE,									// 这里不能超过32个，不然会影响下面的
		
		EFFECT_TYPE_ONCE = 33,									// 一次性效果
	};

	// 对应客户端的Buff表
	enum EFFECT_CLIENT_TYPE
	{
		ECT_SKILL_DROP_BLOOD = 1001,						// 流血
		ECT_SKILL_DINGSHEN = 1002,							// 定身
		ECT_SKILL_BIANSHEN = 1003,							// 变身
		ECT_SKILL_DEC_ON_HURT = 1004,						// 受到伤害减少(比例)
		ECT_SKILL_XUANYUN = 1005,							// 眩晕
		ECT_SKILL_DEC_HURT = 1006,							// 攻击力减少
		ECT_SKILL_DEC_FANGYU = 1007,						// 防御下降
		ECT_SKILL_ADD_GONGJI = 1008,						// 攻击力提升
		ECT_SKILL_ADD_FANGYU = 1009,						// 防御上升
		ECT_SKILL_ADD_BAOJI = 1010,							// 暴击提升
		ECT_SKILL_DEC_SPEED = 1011,							// 速度下降
		ECT_SKILL_REBOUNDHURT = 1012,						// 反伤BUFF,反弹
		ECT_SKILL_POISON = 1013,							// 中毒
		ECT_SKILL_HPSTORE = 1014,							// 护盾
		ECT_SKILL_ADD_SHANBI_PER = 1015,					// 提升闪避率
		ECT_SKILL_ADD_GEDANG_PER = 1016,					// 提升格挡率
		ECT_SKILL_ADD_BAOJI_PER = 1017,						// 提升暴击率
		ECT_SKILL_WUDI = 1018,								// 无敌(通用)
		ECT_SKILL_CHENMO = 1019,							// 沉默

		ECT_JL_SKILL_CJJS = 1101,							// 精灵-采集加速
		ECT_JL_SKILL_CJKDD = 1102,							// 精灵-采集抗打断
		ECT_JL_SKILL_JF = 1103,								// 精灵-减防
		ECT_JL_SKILL_JG = 1104,								// 精灵-减攻
		ECT_JL_SKILL_JSHAN = 1105,							// 精灵-减伤
		ECT_JL_SKILL_JSHU = 1106,							// 精灵-减速
		ECT_JL_SKILL_JZGS = 1107,							// 精灵-建筑高伤
		ECT_JL_SKILL_WD = 1108,								// 精灵-无敌
		ECT_JL_SKILL_XY = 1109,								// 精灵-眩晕

		ECT_UPGRADE_WING_SHOUHU = 1501,						// 进阶-守护
		ECT_UPGRADE_WING_FENGDUN = 1502,					// 进阶-风盾
		ECT_UPGRADE_WING_JIEJIE = 1503,						// 进阶-结界

		ECT_UPGRADE_SHENGONG_ZHUMO = 1511,					// 进阶-诛魔

		ECT_UPGRADE_SHENYI_SHENPAN = 1521,					// 进阶-审判

		ECT_UPGRADE_TOUSHI_SUYI = 1525,						// 头饰进阶-速移
		ECT_UPGRADE_YAOSHI_FANQIAO = 1526,					// 腰饰进阶-返巧
		ECT_UPGRADE_MASK_SHUNSHANG = 1527,					// 面具进阶-瞬闪
		ECT_UPGRADE_QILINBI_BAONU = 1528,					// 麒麟臂进阶-暴怒

		ECT_ITEM_HP1 = 2001,								// 药水-回血药1
		ECT_ITEM_HP2 = 2002,								// 药水-回血药2
		ECT_ITEM_HP3 = 2003,								// 药水-回血药3
		ECT_ITEM_HP4 = 2004,								// 药水-回血药4
		
		ECT_ITEM_MP1 = 2101,								// 药水-回蓝药1
		ECT_ITEM_MP2 = 2102,								// 药水-回蓝药2
		ECT_ITEM_MP3 = 2103,								// 药水-回蓝药3
		ECT_ITEM_MP4 = 2104,								// 药水-回蓝药4
		
		ECT_ITEM_EXP1 = 2201,								// 药水-经验加成1
		ECT_ITEM_EXP2 = 2202,								// 药水-经验加成2
		ECT_ITEM_EXP3 = 2203,								// 药水-经验加成3
		ECT_ITEM_EXP4 = 2204,								// 药水-经验加成4

		ECT_ITEM_BJ1 = 2211,								// 药水-会心一击1
		ECT_ITEM_BJ2 = 2212,								// 药水-会心一击2
		ECT_ITEM_BJ3 = 2213,								// 药水-会心一击3
		ECT_ITEM_BJ4 = 2214,								// 药水-会心一击4

		ECT_ITEM_JZ1 = 2221,								// 药水-加精准1
		ECT_ITEM_JZ2 = 2222,								// 药水-加精准2
		ECT_ITEM_JZ3 = 2223,								// 药水-加精准3
		ECT_ITEM_JZ4 = 2224,								// 药水-加精准4

		ECT_ITEM_YJ1 = 2231,								// 药水-越级1
		ECT_ITEM_YJ2 = 2232,								// 药水-越级2

		ECT_TW_EDGE_MEDICINE = 3000,						// 领土战-利刃药剂
		ECT_TW_LANDMINE_FROZEN = 3001,						// 领土战-被冰霜地雷冻结

		ECT_QY_FB_GONGJI = 3010,							// 情缘副本攻击buff

		ECT_XIULUOTOWER_BUFF = 3030,						// 修罗塔buff

		ECT_YZDD_ADD_SPEED = 3040,							// 一站到底-加速
		ECT_YZDD_DEC_SPEED = 3041,							// 一站到底-减速
		ECT_YZDD_ADD_GONGJI = 3042,							// 一站到底-加攻
		ECT_YZDD_DEC_GONGJI = 3043,							// 一站到底-减攻
		ECT_YZDD_ADD_FANGYU = 3044,							// 一站到底-加防
		ECT_YZDD_DEC_FANGYU = 3045,							// 一站到底-减防
		ECT_YZDD_RECOVER = 3046,							// 一站到底-回复
		ECT_YZDD_WUDI = 3047,								// 一站到底-无敌
		ECT_YZDD_BIANSHEN = 3048,							// 一站到底-变身
		ECT_YZDD_DEADLY_ATTACK = 3049,						// 一站到底-致命一击
		ECT_YZDD_DEAD_ADD = 3050,							// 一站到底-死亡加血
		ECT_YZDD_GUWU_GONGJI = 3051,						// 一站到底-鼓舞攻击
		ECT_YZDD_GUWU_MAXHP = 3052,							// 一站到底-鼓舞气血

		ECT_NSTF_ADD_GONGJI = 3060,							// 女神塔防-加攻

		ECT_TDT_ADD_GONGJI = 3065,							// 组队守护-加攻
		ECT_TDT_ADD_FANGYU = 3066,							// 组队守护-加防

		ECT_HUGUOZHILI = 3070,								// 天神护体

		ECT_MOJIE_SKILL_9 = 3080,							// 魔戒技能9
		ECT_MOJIE_SKILL_3 = 3081,							// 魔戒技能3 经验加成
		ECT_MOJIE_SKILL_WUDI = 3082,						// 魔戒技能 无敌
		ECT_MOJIE_SKILL_RESTORE_HP = 3083,					// 魔戒技能 回血

		ECT_IMG_FULING_MOHUA = 4000,						// 形象赋灵-魔化
		ECT_IMG_FULING_ABSORB = 4001,						// 形象赋灵-吸血
		ECT_IMG_FULING_WUDI = 4002,							// 形象赋灵-无敌

		ECT_DARK_NIGHT_WUDI = 4010,							// 跨服珍宝秘境-无敌

		ECT_IMG_SHENZHOU_WEAPON_ZHUOSHAO = 4100,			// 异火-灼烧
			
		ECT_TALENT_INC_HURT = 4110,							// 角色天赋-攻击增伤 杀意波动
		ECT_TALENT_DEC_ON_HURT = 4111,						// 角色天赋-受击减伤 诸邪辟易

		ECT_SKILL_WEEKEND_EQUIP_MODEL_SIZE_CHANGE = 4220,	// 周末装备 模型大小改变
		ECT_SKILL_WEEKEND_EQUIP_INC_HURT = 4221,			// 周末装备 增伤
		ECT_SKILL_WEEKEND_EQUIP_DEC_HURT = 4222,			// 周末装备 减伤
		ECT_SKILL_WEEKEND_EQUIP_INC_HURT_INVISIABLE = 4223,	// 周末装备 隐身并增伤

		ECT_BUFF_GATHER_NO_STOP = 4300,						// buff采集不打断

		ECT_OTHER_VIP = 9001,								// vip加成
		ECT_OTHER_FBGF = 9002,								// 副本鼓舞
		ECT_OTHER_HMCF = 9003,								// 红名惩罚
		ECT_OTHER_SJJC = 9004,								// 世界加成
		ECT_HP_DAN = 9005,									// 气血丹
		ECT_SZ_PROTECT = 9006,								// 神装保护符
		ECT_OTHER_CALL_MONSTER_DEC_HURT = 9007,				// 召唤怪物替身，减伤

		ECT_IMP_GURAD_EXP1 = 9031,                          // 守护小鬼 - 经验加成
		ECT_IMP_GURAD_EXP2 = 9032,

		ECT_SKILL_CONTINUE_KILL = 9050,						// 连斩BUFF
	};

	enum CLEAR_REASON
	{
		CLEAR_REASON_DIE = 1,									// 死亡清理
		CLEAR_REASON_LEAVE_SCENE,								// 离开场景
		CLEAR_REASON_SKILL,										// 技能
		CLEAR_REASON_ENTER_SCENE,								// 进入场景
	};

	EffectBase() : m_deliverer(0), m_product_id(0), m_product_method(0), m_product_level(0), m_merge_rule(0),
					m_is_save(true), m_clear_on_die(true), m_merge_layer(1), m_max_merge_layer(1), m_is_main_target(false), m_client_effect_type(0), m_unique_key(0), m_recalc_reason_on_add_or_del(0) {}

	EffectBase(ObjID deliverer, UInt16 product_id, char product_method, char product_level)
		: m_deliverer(deliverer), m_product_id(product_id), m_product_method(product_method), m_product_level(product_level),
		m_merge_rule(0), m_is_save(true), m_clear_on_die(true), m_merge_layer(1), m_max_merge_layer(1), m_is_main_target(false), m_client_effect_type(0), m_unique_key(0), m_recalc_reason_on_add_or_del(0) {}

	virtual ~EffectBase() {}

	void SetSave(bool is_save) { m_is_save = is_save; }
	void SetClearOnDie(bool is_clear) { m_clear_on_die = is_clear; }

	void SetMergeRule(char merge_rule) { m_merge_rule = merge_rule; }
	char GetMergeRule() const { return m_merge_rule; }

	void SetMergeLayer(char merge_layer) { m_merge_layer = merge_layer; }
	char GetMergeLayer() const { return m_merge_layer; }
	void SetMaxMergeLayer(char max_layer) { m_max_merge_layer = max_layer; }
	int GetClientEffectType() { return m_client_effect_type; }
	void SetClientEffectType(int effect_type) { m_client_effect_type = effect_type; }

	UInt16 GetProductId() const { return m_product_id; }
	ObjID GetDeliverer() const { return m_deliverer; }
	int GetProductMethod() const { return m_product_method; }
	char GetProductLevel() const { return m_product_level; }

	virtual int CompareWith(EffectBase *effect) { return CMP_RET_NOTHING; }

	void SetUniqueKey(unsigned int unique_key) { m_unique_key = unique_key; }
	unsigned int GetUniqueKey() { return m_unique_key; }

	virtual void Merge(EffectBase *new_effect, Character *cha) {}
	
	virtual void SetLeftTimeMS(int effect_time) {}
	virtual int GetLeftTimeMS() = 0;
	virtual char GetEffectType() = 0;
	virtual int GetImage(){return 0;}
	virtual void Effect(Character *cha, bool *die) = 0;
	virtual void ReEffect(Character *cha, bool *die) = 0;				// 角色init的时候调用，各effect根据自己情况实现，基本原则是不要改HP
	virtual bool Update(unsigned long interval, Character *cha, bool *die) = 0;
	virtual void ClearEffect(Character *cha) {}

	virtual bool IsPositiveEffect() const { return true; }
	virtual bool IsDingshenEffect() const { return false; }
	virtual bool IsXuanyunEffect() const { return false; }
	virtual bool IsChemoEffect() const { return false; }
	virtual bool IsChihuanEffect() const { return false; }

	virtual unsigned char GetBuffType() const { return EBT_INVALID; }

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]) {}
	void GetEffectInfo(EffectInfo *effect_info);
	// virtual void MergeParam(EffectBase *effect) {}; // 合并参数

	virtual bool Serialize(TLVSerializer &s) const { return false; }
	virtual bool Unserialize(TLVUnserializer &s) { return false; }

	bool IsSave() { return m_is_save; }
	bool IsClearOnDie() { return m_clear_on_die; }

	void SetMainTarget(bool is_main_target) { m_is_main_target = is_main_target; }
	bool IsMainTarget() { return m_is_main_target; }

	virtual bool CanEffect(Character *cha);

	virtual void Recalc(Character *chara) {}

	void SetRecalcReasonOnAddOrDel(char reason) { m_recalc_reason_on_add_or_del = reason; }
	char GetRecalcReasonOnAddOrDel() { return m_recalc_reason_on_add_or_del; }

	static EffectBase * UnserializeCreateEffect(char type);
	
protected:
	bool SerializeBase(TLVSerializer &s) const;
	bool UnserializeBase(TLVUnserializer &s);

	ObjID	m_deliverer;
	UInt16	m_product_id;
	char	m_product_method;

	char	m_product_level;				// 一般是技能等级
	char	m_merge_rule;					// 叠加或者替换的6种规则
	bool	m_is_save;						// 是否存盘
	bool	m_clear_on_die;					// 死亡是否清掉
	char	m_merge_layer;					// 叠加层数
	char	m_max_merge_layer;				// 可叠加最多层数
	bool	m_is_main_target;				// 是否为主目标
	int		m_client_effect_type;			// 客户端效果类型
	unsigned int m_unique_key;

	char	m_recalc_reason_on_add_or_del;	// 在添加或删除时的重算人物属性原因

	// 在buff里对自己造成伤害要用这个函数
	void EffectHP(Attribute *real_delta_hp, Attribute *view_delta_hp, Character *cha, bool *die, bool is_from_skill, int fight_type = FIGHT_TYPE_NORMAL);

	// 在buff里对别人造成伤害要用这个函数
	void HurtOthers(Character *cha, short target_count, ObjID *target_objid_list, long long injure, int fight_type);

	void SendAttackBlock(Character *cha, Attribute real_delta_hp, Attribute delta_hp, unsigned short fight_type, unsigned char blood_seq, bool is_send_product_id = true, int passive_flag = 0);
	void SendMPChangeMsg(Character *cha, Attribute delta_hp);
};

#endif

