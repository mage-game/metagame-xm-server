#ifndef __EFFECTLIST_HPP__
#define __EFFECTLIST_HPP__

#include "obj/character/attribute.hpp"
#include "effectbase.hpp"
#include "skill/skilldef.hpp"

class Character;

class EffectHpStore;
class EffectSpecialShield;
class EffectDoubleHurt;
class EffectBomb;
class EffectWudiProtect;
class EffectReboundHurt;
class EffectPose;
class EffectAbsorbHP;
class EffectOtherBuff;
class EffectAttrContainer;
class EffectAttrStore;
class EffectAbsorbHPBeenAttack;
class EffectForbidRecover;
class EffectBianshen;

class Role;

// 每种类型的effect最多几个 (个数和enum EFFECT_TYPE对应)
enum MAX_COUNT
{
	MAX_COUNT_ATTRBUFF = 8,					// 0
	MAX_COUNT_SPECIAL_STATE = 8,
	MAX_COUNT_INTERVAL_HP = 64,
	MAX_COUNT_COUNTER = 4,
	MAX_COUNT_INC_HURT = 4,
	MAX_COUNT_DEC_HURT = 4,
	MAX_COUNT_DOUBLE_HURT = 2,
	MAX_COUNT_MOVESPEED = 2,
	MAX_COUNT_HPSTORE = 2,
	MAX_COUNT_BOMB = 1,
	MAX_COUNT_WUDI_PROTECT = 1,				// 10
	MAX_COUNT_MODIRY_GLOBAL_CD = 1,
	MAX_COUNT_REBOUND_HURT = 1,
	MAX_COUNT_RECOVER_ON_ATKED = 1,
	MAX_COUNT_GRADUAL_HURT = 4,
	MAX_COUNT_JUHUAPIG = 1,
	MAX_COUNT_INVISIBLE = 1,
	MAX_COUNT_ATTR_RAND_INC_HURT = 1,
	MAX_COUNT_BIANSHEN = 1,
	MAX_COUNT_MP_NO_COST = 1,
	MAX_COUNT_SHENBING = 1,					// 20
	MAX_COUNT_ABSORB_HP = 1,
	MAX_COUNT_ATTRPERBUFF = 4,
	MAX_COUNT_OTHER = 32,
	MAX_COUNT_ATTR_CONTAINER = 8,
	MAX_COUNT_ATTR_STORE = 2,
	MAX_COUNT_FIRE = 1,
	MAX_COUNT_SPECIAL_SHIELD = 1,
	MAX_COUNT_DEADLY_ATTACK = 1,
	MAX_COUNT_ABSORB_HP_BEEN_ATTACK = 1,
	MAX_COUNT_TALENT_HPSTORE = 1,			// 30
	MAX_COUNT_FORBID_RECOVER = 1,
};

// 每种类型的effect起始位置  (个数和enum EFFECT_TYPE对应)
enum START_INDEX
{
	START_INDEX_ATTRBUFF = 0,
	START_INDEX_SPECIAL_STATE = START_INDEX_ATTRBUFF + MAX_COUNT_ATTRBUFF,
	START_INDEX_INTERVAL_HP = START_INDEX_SPECIAL_STATE + MAX_COUNT_SPECIAL_STATE,
	START_INDEX_COUNTER = START_INDEX_INTERVAL_HP + MAX_COUNT_INTERVAL_HP,
	START_INDEX_INC_HURT = START_INDEX_COUNTER + MAX_COUNT_COUNTER,
	START_INDEX_DEC_HURT = START_INDEX_INC_HURT + MAX_COUNT_INC_HURT,
	START_INDEX_DOUBLE_HURT = START_INDEX_DEC_HURT + MAX_COUNT_DEC_HURT,
	START_INDEX_MOVESPEED = START_INDEX_DOUBLE_HURT + MAX_COUNT_DOUBLE_HURT,
	START_INDEX_HPSTORE = START_INDEX_MOVESPEED + MAX_COUNT_MOVESPEED,
	START_INDEX_BOMB = START_INDEX_HPSTORE + MAX_COUNT_HPSTORE,
	START_INDEX_WUDI_PROTECT = START_INDEX_BOMB + MAX_COUNT_BOMB,
	START_INDEX_MODIFY_GLOBAL_CD = START_INDEX_WUDI_PROTECT + MAX_COUNT_WUDI_PROTECT,
	START_INDEX_REBOUND_HURT = START_INDEX_MODIFY_GLOBAL_CD + MAX_COUNT_MODIRY_GLOBAL_CD,
	START_INDEX_RECOVER_ON_ATKED = START_INDEX_REBOUND_HURT + MAX_COUNT_REBOUND_HURT,
	START_INDEX_GRADUAL_HURT = START_INDEX_RECOVER_ON_ATKED + MAX_COUNT_RECOVER_ON_ATKED,
	START_INDEX_JUHUAPIG = START_INDEX_GRADUAL_HURT + MAX_COUNT_GRADUAL_HURT,
	START_INDEX_INVISIBLE = START_INDEX_JUHUAPIG + MAX_COUNT_JUHUAPIG,
	START_INDEX_ATTR_RAND_INC_HURT = START_INDEX_INVISIBLE + MAX_COUNT_INVISIBLE,
	START_INDEX_BIANSHEN = START_INDEX_ATTR_RAND_INC_HURT + MAX_COUNT_ATTR_RAND_INC_HURT,
	START_INDEX_MP_NO_COST = START_INDEX_BIANSHEN + MAX_COUNT_BIANSHEN,
	START_INDEX_SHENBING = START_INDEX_MP_NO_COST + MAX_COUNT_MP_NO_COST,
	START_INDEX_ABSORB_HP = START_INDEX_SHENBING + MAX_COUNT_SHENBING,
	START_INDEX_ATTRPERUBFF = START_INDEX_ABSORB_HP + MAX_COUNT_ABSORB_HP,
	START_INDEX_OTHER = START_INDEX_ATTRPERUBFF + MAX_COUNT_ATTRPERBUFF,
	START_INDEX_ATTR_CONTAINER = START_INDEX_OTHER + MAX_COUNT_OTHER,
	START_INDEX_ATTR_STORE = START_INDEX_ATTR_CONTAINER + MAX_COUNT_ATTR_CONTAINER,
	START_INDEX_FIRE = START_INDEX_ATTR_STORE + MAX_COUNT_ATTR_STORE,
	START_INDEX_SPECIAL_SHIELD = START_INDEX_FIRE + MAX_COUNT_FIRE,
	START_INDEX_DEADLY_ATTACK = START_INDEX_SPECIAL_SHIELD + MAX_COUNT_SPECIAL_SHIELD,
	START_INDEX_ABSORB_HP_BEEN_ATTACK = START_INDEX_DEADLY_ATTACK + MAX_COUNT_DEADLY_ATTACK,
	START_INDEX_TALENT_HPSTORE = START_INDEX_ABSORB_HP_BEEN_ATTACK + MAX_COUNT_ABSORB_HP_BEEN_ATTACK,
	START_INDEX_FORBID_RECOVER = START_INDEX_TALENT_HPSTORE + MAX_COUNT_TALENT_HPSTORE,
};

struct EffectTypeInfo
{
	short start_index;
	short max_count;
	short count;
};

class EffectList
{
public:
	EffectList();
	~EffectList();
	
	void Init(Character *cha, int effect_buff_len, const char *effect_buff);
	void GetInitParam(char *effect_buff, int *effect_buff_len);

	void Effect(unsigned long interval, Character *chara, bool *die);
	void ReEffect();

	void Recalc();
	void ReCalcPercentAttr(CharIntAttrs &base_add);
	void ReCalcBaseAttr(CharIntAttrs &base_add, bool is_recalc);

	void Add(EffectBase *new_effect);

	void ClearFightEffect(int clear_reason, bool clear_skill_effect = true, bool clear_scene_skill_effect = true);
	void ClearAllDeBuffEffect();

	EffectHpStore * GetEffectHpStore();
	EffectSpecialShield * GetEffectSpecialShield();
	EffectDoubleHurt * GetDoubleHurtEffect();
	EffectAbsorbHP * GetAbsortHPEffect();
	EffectAbsorbHPBeenAttack * GetAbsortHPBeenAttackEffect();
	EffectBianshen* GetBianshenEffect();

	bool IsInWudiProtect();
	int GetReboundHurtPercent();
	int GetRecoverPercentOnAtked();
	bool IsInvisible();
	bool IsBianshen(short *bianshen_img = NULL);
	bool IsForbidRecover();
	
	void GetSpecialBuffMark(unsigned char buff_mark[SPECIAL_BUFF_MAX_BYTE]);
	bool HaveSpecialBuff(unsigned char special_effect_buff); 
	bool AddSpecialBuff(unsigned char special_effect_buff);
	bool RemoveSpecialBuff(unsigned char special_effect_buff);

	void SetBianshenAppearance(int bianshen_appearance) {m_bianshen_special_appearence = bianshen_appearance;}
	int GetBianshenAppearance(){return m_bianshen_special_appearence;}

	void ModifyGlobalCD(Attribute *global_cd);

	float GetHurtIncPercent(Character *target);
	float GetHurtDecPercent(Character *target);
	float GetHurtDecFix(Character *target);
	float GetFixHurtPercent(Character *target);

	void OnEffectInfoReq(char effect_type, unsigned int effect_key, Role *role);

	void SendEffectList(Role *role = NULL);

	EffectBomb * FindBomb(unsigned char buff_type);
	EffectOtherBuff * FindOtherEffectBuff(int product_id);
	EffectAttrContainer * FindAttrContainerBuff(int product_id);
	EffectAttrStore * FindAttrStoreBuff(int product_id);
	EffectBase * FindEffectByType(char effect_type, unsigned char buff_type);
	EffectBase * FindEffect(char effect_type, unsigned char buff_type, ObjID deliverer, char product_method = EffectBase::PRODUCT_METHOD_SKILL, UInt16 product_id = 0);

	bool FindAndRemoveEffect(char effect_type, unsigned char buff_type, ObjID deliverer = INVALID_OBJ_ID, char product_method = EffectBase::PRODUCT_METHOD_SKILL, UInt16 product_id = 0);
	bool RemoveByKey(char effect_type, unsigned int effect_key);
	bool RandRemoveFightEffect();

	void OnHurtOther(Character *target, int skill_id, long long injure);

	bool IsControlBuffType(unsigned char buff_type);
	bool CanClearByFuncClearAllDeBuffEffect(EffectBase *effect);

	void SetEffectKey(EffectBase *effect);
	void SendEffectInfo(EffectBase *effect, short notify_reason, Role *role = NULL);
	void NotifyEffectRemove(unsigned int effect_key);

private:
	void AddSkillEffect(EffectBase *new_effect, bool is_init = false);
	void AddEffectToTypeList(EffectBase *new_effect, char effect_type, bool is_init = false);
	void Replace(int exist_index, EffectBase *new_effect, bool is_init = false);
	void Merge(int exist_index, EffectBase *new_effect, bool is_init = false);

	void RemoveEffect(int effect_type, int i);

	unsigned int GenerateKey();
	int CalcExistMethod(EffectBase *curr_effect, EffectBase* new_effect);
	EffectTypeInfo *GetTypeInfo(char effect_type);

	void SendEffectInfo(char effect_type, unsigned int effect_key, short notify_reason, Role *role = NULL);

	int SerializeEffectInfo(char *buffer, EffectInfo *effect_info);

	void CheckCtrlEffectReduceTime(EffectBase *effect, bool *remove);

	bool IsValidSpecialBuffType(unsigned char buff_type);

	Character *m_character;
	
	static const int MAX_EFFECT_COUNT = 180;

	EffectBase* m_all_effect_list[MAX_EFFECT_COUNT];

	unsigned long m_update_interval;
	
	//long long m_special_buff_mark;
	unsigned char m_special_buff_mark[SPECIAL_BUFF_MAX_BYTE];

	int m_bianshen_special_appearence;

	unsigned int m_max_unique_key;

	bool m_first;

	unsigned int m_xuanyun_effect_reduce_end_time;				// 各种眩晕时长衰减的结束时间
	short m_xuanyun_effect_reduce_count;						// 各种眩晕衰减次数

	CharIntAttrs	m_attrs_add;								// 各属性加成数值

	EffectTypeInfo m_effect_typeinfo_list[EffectBase::EFFECT_TYPE_RESERVE + 1] =
	{
		{ START_INDEX_ATTRBUFF, MAX_COUNT_ATTRBUFF, 0 },
		{ START_INDEX_SPECIAL_STATE, MAX_COUNT_SPECIAL_STATE, 0 },
		{ START_INDEX_INTERVAL_HP, MAX_COUNT_INTERVAL_HP, 0 },
		{ START_INDEX_COUNTER, MAX_COUNT_COUNTER, 0 },
		{ START_INDEX_INC_HURT, MAX_COUNT_INC_HURT, 0 },
		{ START_INDEX_DEC_HURT, MAX_COUNT_DEC_HURT, 0 },
		{ START_INDEX_DOUBLE_HURT, MAX_COUNT_DOUBLE_HURT, 0 },
		{ START_INDEX_MOVESPEED, MAX_COUNT_MOVESPEED, 0 },
		{ START_INDEX_HPSTORE, MAX_COUNT_HPSTORE, 0 },
		{ START_INDEX_BOMB, MAX_COUNT_BOMB, 0 },
		{ START_INDEX_WUDI_PROTECT, MAX_COUNT_WUDI_PROTECT, 0 },
		{ START_INDEX_MODIFY_GLOBAL_CD, MAX_COUNT_MODIRY_GLOBAL_CD, 0 },
		{ START_INDEX_REBOUND_HURT, MAX_COUNT_REBOUND_HURT, 0 },
		{ START_INDEX_RECOVER_ON_ATKED, MAX_COUNT_RECOVER_ON_ATKED, 0 },
		{ START_INDEX_GRADUAL_HURT, MAX_COUNT_GRADUAL_HURT, 0 },
		{ START_INDEX_JUHUAPIG, MAX_COUNT_JUHUAPIG, 0 },
		{ START_INDEX_INVISIBLE, MAX_COUNT_INVISIBLE, 0 },
		{ START_INDEX_ATTR_RAND_INC_HURT, MAX_COUNT_ATTR_RAND_INC_HURT, 0 },
		{ START_INDEX_BIANSHEN, MAX_COUNT_BIANSHEN, 0 },
		{ START_INDEX_MP_NO_COST, MAX_COUNT_MP_NO_COST, 0 },
		{ START_INDEX_SHENBING, MAX_COUNT_SHENBING, 0 },
		{ START_INDEX_ABSORB_HP, MAX_COUNT_ABSORB_HP, 0 },
		{ START_INDEX_ATTRPERUBFF, MAX_COUNT_ATTRPERBUFF, 0 },
		{ START_INDEX_OTHER, MAX_COUNT_OTHER, 0 },
		{ START_INDEX_ATTR_CONTAINER, MAX_COUNT_ATTR_CONTAINER, 0 },
		{ START_INDEX_ATTR_STORE, MAX_COUNT_ATTR_STORE, 0 },
		{ START_INDEX_FIRE, MAX_COUNT_FIRE, 0 },
		{ START_INDEX_SPECIAL_SHIELD, MAX_COUNT_SPECIAL_SHIELD, 0 },
		{ START_INDEX_DEADLY_ATTACK, MAX_COUNT_DEADLY_ATTACK, 0 },
		{ START_INDEX_ABSORB_HP_BEEN_ATTACK, MAX_COUNT_ABSORB_HP_BEEN_ATTACK, 0 },
		{ START_INDEX_TALENT_HPSTORE, MAX_COUNT_TALENT_HPSTORE, 0 },
		{ START_INDEX_FORBID_RECOVER, MAX_COUNT_FORBID_RECOVER, 0 },
	};
};

#endif

