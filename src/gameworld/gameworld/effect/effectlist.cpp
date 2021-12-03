#include "effectlist.hpp"
#include "effectbase.hpp"

#include "effect/skilleffect/effectotherbuff.hpp"
#include "effect/skilleffect/effectattrcontainer.hpp"
#include "effect/skilleffect/effectattrstore.hpp"
#include "effect/skilleffect/effecthspecialshield.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effecthurtinc.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "effect/skilleffect/effectdoublehurt.hpp"
#include "effect/skilleffect/effectbomb.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effectmodifyglobalcd.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "effect/skilleffect/effectrecoveronatked.hpp"
#include "effect/skilleffect/effectgradualhurt.hpp"
#include "effect/skilleffect/effectattrhurtrandinc.hpp"
#include "effect/skilleffect/effectbianshen.hpp"
#include "effect/skilleffect/effectcounter.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effectabsorbhp.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectpercentbuff.hpp"
#include "effect/skilleffect/effectotherbuff.hpp"
#include "effect/skilleffect/effectattrcontainer.hpp"
#include "effect/skilleffect/effectdeadlyattack.hpp"
#include "effect/skilleffect/effectforbidrecover.hpp"

#include "obj/character/character.h"
#include "obj/character/role.h"

#include "servercommon/serverclmempool.h"
#include "servercommon/struct/roleotherinitparam.hpp"
#include "globalconfig/globalconfig.h"
#include "protocal/msgfight.h"

#include "engineadapter.h"
#include "scene/scene.h"
#include "other/specialappearance/specialappearance.hpp"
#include "effect/skilleffect/effectabsorbhpbeenattack.hpp"


EffectList::EffectList() 
	: m_character(NULL), m_update_interval(0), /*m_special_buff_mark(0),*/ m_bianshen_special_appearence(APPEARANCE_NORMAL), m_max_unique_key(0), m_first(true),
	m_xuanyun_effect_reduce_end_time(0), m_xuanyun_effect_reduce_count(0)
{
	memset(m_all_effect_list, 0, sizeof(m_all_effect_list));
	memset(m_special_buff_mark, 0, sizeof(m_special_buff_mark));
}

EffectList::~EffectList()
{
	for (int effect_type = EffectBase::EFFECT_TYPE_ATTRBUFF; effect_type <= EffectBase::EFFECT_TYPE_RESERVE; ++effect_type)
	{
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
		if (NULL == effect_typeinfo)
		{
			continue;
		}

		for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
		{
			int index = effect_typeinfo->start_index + i;

			if (NULL != m_all_effect_list[index])
			{
				delete m_all_effect_list[index];
			}
		}
	}
}

// 清理战斗效果
void EffectList::ClearFightEffect(int clear_reason, bool clear_skill_effect, bool clear_scene_skill_effect)
{
	for (int effect_type = EffectBase::EFFECT_TYPE_ATTRBUFF; effect_type <= EffectBase::EFFECT_TYPE_RESERVE; ++effect_type)
	{
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
		if (NULL == effect_typeinfo)
		{
			continue;
		}

		int protect_counter = 0;

		for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count && protect_counter < 100; ++protect_counter)
		{
			int index = effect_typeinfo->start_index + i;

			if (NULL == m_all_effect_list[index]) break;

			if (EffectBase::CLEAR_REASON_DIE == clear_reason && !m_all_effect_list[index]->IsClearOnDie())
			{
				++i;
				continue;
			}

			int product_method = m_all_effect_list[index]->GetProductMethod();
			
			if ((clear_skill_effect && (EffectBase::PRODUCT_METHOD_SKILL == product_method || 
										EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL == product_method)) ||

				(clear_scene_skill_effect && (EffectBase::PRODUCT_METHOD_SCENE_SKILL == product_method || 
											  EffectBase::PRODUCT_METHOD_FAZHEN_HALO == product_method ||
											  EffectBase::PRODUCT_METHOD_SHENSHI == product_method || 
											  EffectBase::PRODUCT_METHOD_GONGCHENGZHAN == product_method ||
											  (EffectBase::CLEAR_REASON_LEAVE_SCENE == clear_reason && EffectBase::PRODUCT_METHOD_FRIEND == product_method))
				))
			{
				if (!m_first) m_all_effect_list[index]->ClearEffect(m_character);
				this->RemoveEffect(effect_type, i);
			}
			else
			{
				++i;
			}
		}
	}
}

// 净化负面的效果
void EffectList::ClearAllDeBuffEffect()
{
	for (int effect_type = EffectBase::EFFECT_TYPE_ATTRBUFF; effect_type <= EffectBase::EFFECT_TYPE_RESERVE; ++effect_type)
	{
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
		if (NULL == effect_typeinfo)
		{
			continue;
		}

		int protect_counter = 0;

		for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count && protect_counter < 100; ++ protect_counter)
		{
			int index = effect_typeinfo->start_index + i;

			if (NULL == m_all_effect_list[index]) break;

			if (!m_all_effect_list[index]->IsPositiveEffect())
			{
				if (CanClearByFuncClearAllDeBuffEffect(m_all_effect_list[index]))
				{
					if (!m_first) m_all_effect_list[index]->ClearEffect(m_character);
					this->RemoveEffect(effect_type, i);
				}
			}
			else
			{
				++i;
			}
		}
	}
}

void EffectList::Init(Character *cha, int effect_buff_len, const char *effect_buff)
{
	UNSTD_STATIC_CHECK(RoleOtherInitParam::EFFECT_BUFFER_LENGTH > 0);

	m_character = cha;
	
	if (NULL != effect_buff && effect_buff_len >= static_cast<int>(sizeof(unsigned int)))
	{
		m_max_unique_key = *(unsigned int *)effect_buff;
		effect_buff += sizeof(unsigned int);
		effect_buff_len -= sizeof(unsigned int);

		if (effect_buff_len > 0 && 0 == effect_buff_len % RoleOtherInitParam::EFFECT_BUFFER_LENGTH)
		{
			int count = effect_buff_len / RoleOtherInitParam::EFFECT_BUFFER_LENGTH;
			if (count > RoleOtherInitParam::MAX_EFFECT_NUM) count = RoleOtherInitParam::MAX_EFFECT_NUM;

			for (int i = 0; i < count; i++)
			{
				TLVUnserializer effect_data;
				effect_data.Reset(effect_buff, RoleOtherInitParam::EFFECT_BUFFER_LENGTH);

				char effect_type = 0;
				if (effect_data.Pop(&effect_type))
				{
					EffectBase *effect = EffectBase::UnserializeCreateEffect(effect_type);
					if (NULL != effect)
					{
						if (effect->Unserialize(effect_data))
						{
							this->Add(effect);
						}
						else 
						{
							delete effect;
						}
					}
				}

				effect_buff += RoleOtherInitParam::EFFECT_BUFFER_LENGTH;
			}
		}
	}
}

void EffectList::GetInitParam(char *effect_buff, int *effect_buff_len)
{
	if (NULL == effect_buff || NULL == effect_buff_len) return;

	*effect_buff_len = 0; 

	*(unsigned int *)effect_buff = m_max_unique_key;
	effect_buff += sizeof(unsigned int);
	*effect_buff_len += sizeof(unsigned int);

	int count = 0;  // 能够存储的BUFF个数

	for (int effect_type = EffectBase::EFFECT_TYPE_ATTRBUFF; effect_type <= EffectBase::EFFECT_TYPE_RESERVE; ++effect_type)
	{
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
		if (NULL == effect_typeinfo)
		{
			continue;
		}

		for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
		{
			int index = effect_typeinfo->start_index + i;

			EffectBase *effect = m_all_effect_list[index];
			if (NULL != effect && effect->IsSave())
			{
				// 超出数据存储上限的部分舍弃
				if (count >= RoleOtherInitParam::MAX_EFFECT_NUM)
				{
					break;
				}

				TLVSerializer effect_data;
				effect_data.Reset(effect_buff, RoleOtherInitParam::EFFECT_BUFFER_LENGTH);

				if (effect_data.Push(effect->GetEffectType()) && effect->Serialize(effect_data))
				{
					effect_buff += RoleOtherInitParam::EFFECT_BUFFER_LENGTH; 
					*effect_buff_len += RoleOtherInitParam::EFFECT_BUFFER_LENGTH;

					count++;
				}
			}
		}
	}
}

void EffectList::Effect(unsigned long interval, Character *chara, bool *die)
{
	if (m_first)
	{
		m_first = false;

		this->ReEffect();
		return;
	}

	m_update_interval += interval;
	if (m_update_interval < 500) return;

	bool is_die = false; ObjID killer_objid = INVALID_OBJ_ID;

	for (int effect_type = EffectBase::EFFECT_TYPE_ATTRBUFF; !(*die) && effect_type <= EffectBase::EFFECT_TYPE_RESERVE; ++effect_type)
	{
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
		if (NULL == effect_typeinfo)
		{
			continue;
		}

		int protect_counter = 0;

		int i = 0;
		while (!(*die) && i < effect_typeinfo->count && i < effect_typeinfo->max_count && protect_counter < 100)
		{
			++ protect_counter;

			int index = effect_typeinfo->start_index + i;

			if (NULL == m_all_effect_list[index]) break;

			ObjID deliverer = m_all_effect_list[index]->GetDeliverer();
			if (m_all_effect_list[index]->CanEffect(chara))
			{
				if (m_all_effect_list[index]->Update(m_update_interval, chara, die))
				{
					++i;
				}
				else
				{
					this->RemoveEffect(effect_type, i);
				}

				if (*die)
				{
					is_die = true; killer_objid = deliverer;
					break;
				}
			}
			else
			{
				++i;
			}
		}
	}
	
	m_update_interval = 0;

	if (*die && is_die) chara->OnDie(killer_objid);			// OnDie函数会对m_effect_num操作 不要在while循环里面发起 以免迭代器失效
}

void EffectList::ReEffect()
{
	for (int effect_type = EffectBase::EFFECT_TYPE_ATTRBUFF; effect_type <= EffectBase::EFFECT_TYPE_RESERVE; ++effect_type)
	{
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
		if (NULL == effect_typeinfo)
		{
			continue;
		}

		for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
		{
			int index = effect_typeinfo->start_index + i;

			if (NULL != m_all_effect_list[index])
			{
				bool die = false;
				m_all_effect_list[index]->ReEffect(m_character, &die);
			}
		}
	}
}

bool EffectList::IsValidSpecialBuffType(unsigned char buff_type)
{
	if ((buff_type > EBT_INVALID && buff_type < EBT_SPECIAL_EFFECT_MAX) ||
		(buff_type >= EBT_SPECIAL_OTHER_BEGIN && buff_type < EBT_SPECIAL_OTHER_END))
	{
		return true;
	}

	return false;
}

void EffectList::GetSpecialBuffMark(unsigned char buff_mark[SPECIAL_BUFF_MAX_BYTE])
{
	UNSTD_STATIC_CHECK(SPECIAL_BUFF_MAX_BYTE == 16);
	UNSTD_STATIC_CHECK((sizeof(unsigned char) * SPECIAL_BUFF_MAX_BYTE) == sizeof(m_special_buff_mark));

	memcpy(buff_mark, m_special_buff_mark, sizeof(m_special_buff_mark));
}

bool EffectList::HaveSpecialBuff(unsigned char special_effect_buff)
{
	if (!this->IsValidSpecialBuffType(special_effect_buff)) return false;

	//return ((m_special_buff_mark >> special_effect_buff) & LL_ONE_BIT);
	return BitmapFunction::IsBitSet(m_special_buff_mark, SPECIAL_BUFF_MAX_BYTE, special_effect_buff);
}

bool EffectList::AddSpecialBuff(unsigned char special_effect_buff)
{
	if (!this->IsValidSpecialBuffType(special_effect_buff)) return false;

	//if (!((m_special_buff_mark >> special_effect_buff) & LL_ONE_BIT))
	if (!BitmapFunction::IsBitSet(m_special_buff_mark, SPECIAL_BUFF_MAX_BYTE, special_effect_buff))
	{
		//m_special_buff_mark |= (LL_ONE_BIT << special_effect_buff);
		BitmapFunction::SetBit(m_special_buff_mark, SPECIAL_BUFF_MAX_BYTE, special_effect_buff);

		static Protocol::SCBuffAdd ba;
		ba.obj_id = m_character->GetId();
		ba.buff_type = (short)special_effect_buff;
		if (NULL != m_character->GetScene()) m_character->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_character->GetObserHandle(), (void *)&ba, sizeof(ba));
	}

	return true;
}

bool EffectList::RemoveSpecialBuff(unsigned char special_effect_buff)
{
	if (!this->IsValidSpecialBuffType(special_effect_buff)) return false;

	//if ((m_special_buff_mark >> special_effect_buff) & LL_ONE_BIT)
	if (BitmapFunction::IsBitSet(m_special_buff_mark, SPECIAL_BUFF_MAX_BYTE, special_effect_buff))
	{
		//m_special_buff_mark &= ~(LL_ONE_BIT << special_effect_buff);
		BitmapFunction::ClearBit(m_special_buff_mark, SPECIAL_BUFF_MAX_BYTE, special_effect_buff);

		static Protocol::SCBuffRemove br;
		br.obj_id = m_character->GetId();
		br.buff_type = (short)special_effect_buff;
		if (NULL != m_character->GetScene()) m_character->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_character->GetObserHandle(), (void *)&br, sizeof(br));
	}

	return true;
}

void EffectList::ModifyGlobalCD(Attribute *global_cd)
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_MODIFY_GLOBAL_CD);
	if (NULL == effect_typeinfo)
	{
		return;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_MODIFY_GLOBAL_CD == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		EffectModifyGlobalCD *effect_cd = (EffectModifyGlobalCD *)m_all_effect_list[effect_typeinfo->start_index];
		effect_cd->ModifyGlobalCD(global_cd);
	}
}

void EffectList::RemoveEffect(int effect_type, int i)
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
	if (NULL == effect_typeinfo)
	{
		return;
	}
	
	if (i < 0 || i >= effect_typeinfo->count || NULL == m_all_effect_list[effect_typeinfo->start_index + i]) return;

	if(EffectBase::EFFECT_TYPE_ATTRBUFF == m_all_effect_list[effect_typeinfo->start_index + i]->GetEffectType())
	{
		EffectBuff *effect_buff = (EffectBuff *)(m_all_effect_list[effect_typeinfo->start_index + i]);
		
		if (effect_buff->GetAttrValue(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU) < 0)  // 减防御buff去掉对应的效果
		{
			this->RemoveSpecialBuff(EBT_DEC_FANGYU);
		}
	}

	if (EffectBase::EFFECT_TYPE_BIANSHEN == m_all_effect_list[effect_typeinfo->start_index + i]->GetEffectType())
	{
		this->SetBianshenAppearance(APPEARANCE_NORMAL);

		// 广播形象
		static Protocol::SCBianShenView cmd;
		cmd.obj_id = m_character->GetId();
		cmd.bianshen_image = APPEARANCE_NORMAL;
		m_character->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_character->GetObserHandle(), (void*)&cmd, sizeof(cmd));
	}

	if (m_all_effect_list[effect_typeinfo->start_index + i]->GetBuffType() > 0)
	{
		this->RemoveSpecialBuff(m_all_effect_list[effect_typeinfo->start_index + i]->GetBuffType());
	}
	
	int role_recalc_reason = static_cast<int>(m_all_effect_list[effect_typeinfo->start_index + i]->GetRecalcReasonOnAddOrDel());
	
	this->NotifyEffectRemove(m_all_effect_list[effect_typeinfo->start_index + i]->GetUniqueKey());

	delete m_all_effect_list[effect_typeinfo->start_index + i];
	m_all_effect_list[effect_typeinfo->start_index + i] = NULL;

	if (i != effect_typeinfo->count - 1)
	{
		m_all_effect_list[effect_typeinfo->start_index + i] = m_all_effect_list[effect_typeinfo->start_index + effect_typeinfo->count - 1];
		m_all_effect_list[effect_typeinfo->start_index + effect_typeinfo->count - 1] = NULL;
	}
	--effect_typeinfo->count;
	
	if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType() &&
		role_recalc_reason > CharIntAttrs::RECALC_REASON_INVALID && role_recalc_reason < CharIntAttrs::RECALC_REASON_MAX)
	{
		Role *role = (Role *)m_character;
		role->ReCalcAttr(role_recalc_reason);
	}
}

EffectBomb * EffectList::FindBomb(unsigned char buff_type)
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_BOMB);
	if (NULL == effect_typeinfo)
	{
		return NULL;
	}

	for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
	{
		int index = effect_typeinfo->start_index + i;

		if (NULL != m_all_effect_list[index] && EffectBase::EFFECT_TYPE_BOMB == m_all_effect_list[index]->GetEffectType() &&
			buff_type == m_all_effect_list[index]->GetBuffType())
		{
			return (EffectBomb *)m_all_effect_list[index];
		}
	}

	return NULL;
}

EffectOtherBuff * EffectList::FindOtherEffectBuff(int product_id)
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFEECT_TYPE_OTHER);
	if (NULL == effect_typeinfo)
	{
		return NULL;
	}

	for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
	{
		int index = effect_typeinfo->start_index + i;

		if (NULL != m_all_effect_list[index] && EffectBase::EFEECT_TYPE_OTHER == m_all_effect_list[index]->GetEffectType())
		{
			EffectOtherBuff *effect_buff = (EffectOtherBuff *)m_all_effect_list[index];
			if (product_id == effect_buff->GetProductId())
			{
				return effect_buff;
			}
		}
	}

	return NULL;
}

// 查找属性容器buff
EffectAttrContainer * EffectList::FindAttrContainerBuff(int product_id)
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_ATTR_CONTAINER);
	if (NULL == effect_typeinfo)
	{
		return NULL;
	}

	for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
	{
		int index = effect_typeinfo->start_index + i;

		if (NULL != m_all_effect_list[index] && EffectBase::EFFECT_TYPE_ATTR_CONTAINER == m_all_effect_list[index]->GetEffectType())
		{
			EffectAttrContainer *effect = (EffectAttrContainer *)m_all_effect_list[index];
			if (product_id == effect->GetProductId())
			{
				return effect;
			}
		}
	}

	return NULL;
}

// 查找属性存储buff
EffectAttrStore * EffectList::FindAttrStoreBuff(int product_id)
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_ATTR_STORE);
	if (NULL == effect_typeinfo)
	{
		return NULL;
	}

	for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
	{
		int index = effect_typeinfo->start_index + i;

		if (NULL != m_all_effect_list[index] && EffectBase::EFFECT_TYPE_ATTR_STORE == m_all_effect_list[index]->GetEffectType())
		{
			EffectAttrStore *effect = (EffectAttrStore *) m_all_effect_list[index];
			if (product_id == effect->GetProductId())
			{
				return effect;
			}
		}
	}

	return NULL;
}

EffectBase * EffectList::FindEffectByType(char effect_type, unsigned char buff_type)
{
	if (effect_type > EffectBase::EFFECT_TYPE_RESERVE) effect_type = EffectBase::EFFECT_TYPE_RESERVE;

	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
	if (NULL == effect_typeinfo)
	{
		return NULL;
	}

	for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
	{
		int index = effect_typeinfo->start_index + i;

		if (NULL != m_all_effect_list[index] && 
			effect_type == m_all_effect_list[index]->GetEffectType() && 
			buff_type == m_all_effect_list[index]->GetBuffType())
		{
			return m_all_effect_list[index];
		}
	}

	return NULL;
}

EffectBase * EffectList::FindEffect(char effect_type, unsigned char buff_type, ObjID deliverer, char product_method, UInt16 product_id)
{
	if (effect_type > EffectBase::EFFECT_TYPE_RESERVE) effect_type = EffectBase::EFFECT_TYPE_RESERVE;

	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
	if (NULL == effect_typeinfo)
	{
		return NULL;
	}

	for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
	{
		int index = effect_typeinfo->start_index + i;

		if (NULL != m_all_effect_list[index] && 
			effect_type == m_all_effect_list[index]->GetEffectType() &&
			buff_type == m_all_effect_list[index]->GetBuffType() &&
			deliverer == m_all_effect_list[index]->GetDeliverer())
		{
			if (0 == product_id ||
				(product_method == m_all_effect_list[index]->GetProductMethod() && product_id == m_all_effect_list[index]->GetProductId()))
			{
				return m_all_effect_list[index];
			}
		}
	}

	return NULL;
}

bool EffectList::FindAndRemoveEffect(char effect_type, unsigned char buff_type, ObjID deliverer, char product_method, UInt16 product_id)
{
	if (effect_type > EffectBase::EFFECT_TYPE_RESERVE) effect_type = EffectBase::EFFECT_TYPE_RESERVE;

	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
	if (NULL == effect_typeinfo)
	{
		return false;
	}

	for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
	{
		int index = effect_typeinfo->start_index + i;

		if (NULL != m_all_effect_list[index] && 
			effect_type == m_all_effect_list[index]->GetEffectType() &&
			buff_type == m_all_effect_list[index]->GetBuffType())
		{
			if (INVALID_OBJ_ID != deliverer)
			{
				if (deliverer == m_all_effect_list[index]->GetDeliverer())
				{
					if (0 == product_id ||
						(product_method == m_all_effect_list[index]->GetProductMethod() && product_id == m_all_effect_list[index]->GetProductId()))
					{
						if (!m_first) m_all_effect_list[index]->ClearEffect(m_character);
						this->RemoveEffect(effect_type, i);
						return true;
					}
				}
			}
			else
			{
				if (0 == product_id ||
					(product_method == m_all_effect_list[index]->GetProductMethod() && product_id == m_all_effect_list[index]->GetProductId()))
				{
					if (!m_first) m_all_effect_list[index]->ClearEffect(m_character);
					this->RemoveEffect(effect_type, i);
					return true;
				}
			}
		}
	}

	return false;
}

bool EffectList::RemoveByKey(char effect_type, unsigned int effect_key)
{
	if (effect_type > EffectBase::EFFECT_TYPE_RESERVE) effect_type = EffectBase::EFFECT_TYPE_RESERVE;

	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
	if (NULL == effect_typeinfo)
	{
		return false;
	}

	for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
	{
		int index = effect_typeinfo->start_index + i;

		if (NULL != m_all_effect_list[index] && effect_key == m_all_effect_list[index]->GetUniqueKey())
		{
			if (!m_first) m_all_effect_list[index]->ClearEffect(m_character);
			this->RemoveEffect(effect_type, i);
			return true;
		}
	}

	return false;
}

bool EffectList::RandRemoveFightEffect()
{
	UNSTD_STATIC_CHECK(MAX_EFFECT_COUNT >= START_INDEX_OTHER + MAX_COUNT_OTHER);

	int info_count = 0;
	static struct  
	{
		int effect_type;
		int i;
	} tmp_effect_list[MAX_EFFECT_COUNT];

	for (int effect_type = EffectBase::EFFECT_TYPE_ATTRBUFF; effect_type <= EffectBase::EFFECT_TYPE_RESERVE; ++effect_type)
	{
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
		if (NULL == effect_typeinfo)
		{
			continue;
		}

		for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count && info_count < MAX_EFFECT_COUNT; ++i)
		{
			int index = effect_typeinfo->start_index + i;

			if (NULL == m_all_effect_list[index]) break;

			if (EffectBase::PRODUCT_METHOD_SKILL == m_all_effect_list[index]->GetProductMethod() ||
				EffectBase::PRODUCT_METHOD_SCENE_SKILL == m_all_effect_list[index]->GetProductMethod())
			{
				tmp_effect_list[info_count].effect_type = effect_type;
				tmp_effect_list[info_count].i = i;
				++ info_count;
			}
		}
	}

	if (info_count > 0 && info_count < MAX_EFFECT_COUNT)
	{
		int rand_val = RandomNum(info_count);

		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(tmp_effect_list[rand_val].effect_type);
		if (NULL == effect_typeinfo)
		{
			return false;
		}

		if (tmp_effect_list[rand_val].i >= 0 && tmp_effect_list[rand_val].i < effect_typeinfo->count)
		{
			if (!m_first) 
			{
				int index = effect_typeinfo->start_index + tmp_effect_list[rand_val].i;
				if (index >= 0 && index < MAX_EFFECT_COUNT && NULL != m_all_effect_list[index])
				{
					m_all_effect_list[index]->ClearEffect(m_character);
				}
			}

			this->RemoveEffect(tmp_effect_list[rand_val].effect_type, tmp_effect_list[rand_val].i);
			return true;
		}
	}

	return false;
}

void EffectList::SendEffectList(Role *role)
{
	if (NULL == role && Obj::OBJ_TYPE_ROLE == m_character->GetObjType()) role = (Role *)m_character;
	if (NULL == role) return;

	static Protocol::SCEffectList eli; 
	eli.obj_id = m_character->GetId();
	eli.count = 0;

	{
		EffectTypeInfo effect_typeinfo;
		memset(&effect_typeinfo, 0, sizeof(effect_typeinfo));
		for (int effect_type = EffectBase::EFFECT_TYPE_ATTRBUFF; effect_type <= EffectBase::EFFECT_TYPE_RESERVE; ++effect_type)
		{
			EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
			if (NULL == effect_typeinfo)
			{
				continue;
			}

			for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count && eli.count < static_cast<int>(sizeof(eli.effect_info_list) / sizeof(eli.effect_info_list[0])); ++i)
			{
				int index = effect_typeinfo->start_index + i;

				if (NULL != m_all_effect_list[index])
				{
					m_all_effect_list[index]->GetEffectInfo(&eli.effect_info_list[eli.count]);
					++ eli.count;
				}
			}
		}
	}

	int send_len = 0;
	static char msg_buff[sizeof(Protocol::SCEffectList)] = {0};

	{
		char *pt = msg_buff;

		int offset = static_cast<int>(UNSTD_OFFSETOFF(Protocol::SCEffectList, effect_info_list));

		memcpy(pt, (char*)&eli, offset);
		pt += offset; send_len += offset; 

		for (int i = 0; i < eli.count; ++i)
		{
			int tmp_size = this->SerializeEffectInfo(pt, &eli.effect_info_list[i]);
			pt += tmp_size; send_len += tmp_size;
		}
	}
	
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg_buff, send_len);
}

void EffectList::SendEffectInfo(EffectBase *effect, short notify_reason, Role *role)
{
	if (NULL == role && Obj::OBJ_TYPE_ROLE == m_character->GetObjType()) role = (Role *)m_character;
	if (NULL == role) return;

	if (NULL != effect)
	{
		static Protocol::SCEffectInfo ei;
		ei.notify_reason = notify_reason;
		ei.obj_id = m_character->GetId();
		effect->GetEffectInfo(&ei.effect_info);

		int send_len = 0;
		static char msg_buff[sizeof(Protocol::SCEffectInfo)] = {0};

		{
			char *pt = msg_buff;

			int offset = static_cast<int>(UNSTD_OFFSETOFF(Protocol::SCEffectInfo, effect_info));

			memcpy(pt, (char*)&ei, offset);
			pt += offset; send_len += offset; 

			int tmp_size = this->SerializeEffectInfo(pt, &ei.effect_info);
			pt += tmp_size; send_len += tmp_size;
		}

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg_buff, send_len);
	}
}

void EffectList::SendEffectInfo(char effect_type, unsigned int effect_key, short notify_reason, Role *role)
{
	if (NULL == role && Obj::OBJ_TYPE_ROLE == m_character->GetObjType()) role = (Role *)m_character;
	if (NULL == role) return;

	if (effect_type > EffectBase::EFFECT_TYPE_RESERVE) effect_type = EffectBase::EFFECT_TYPE_RESERVE;

	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
	if (NULL == effect_typeinfo)
	{
		return;
	}

	for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
	{
		int index = effect_typeinfo->start_index + i;

		if (NULL != m_all_effect_list[index] && effect_key == m_all_effect_list[index]->GetUniqueKey())
		{
			this->SendEffectInfo(m_all_effect_list[index], notify_reason, role);
			return;
		}
	}
}

void EffectList::OnEffectInfoReq(char effect_type, unsigned int effect_key, Role *role)
{
	this->SendEffectInfo(effect_type, effect_key, Protocol::SCEffectInfo::NOTIFY_REASON_DEFAULT, role);
}

void EffectList::NotifyEffectRemove(unsigned int effect_key)
{
	if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType())
	{
		Role *role = (Role*)m_character;

		static Protocol::SCEffectRemove er;
		er.effect_key = effect_key;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&er, sizeof(er));
	}
}

EffectHpStore * EffectList::GetEffectHpStore()
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_HPSTORE);
	if (NULL == effect_typeinfo)
	{
		return NULL;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_HPSTORE == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		return (EffectHpStore*)m_all_effect_list[effect_typeinfo->start_index];
	}
	
	return NULL;
}

EffectSpecialShield * EffectList::GetEffectSpecialShield()
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_SPECIAL_SHIELD);
	if (NULL == effect_typeinfo)
	{
		return NULL;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_SPECIAL_SHIELD == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		return (EffectSpecialShield*)m_all_effect_list[effect_typeinfo->start_index];
	}

	return NULL;
}

bool EffectList::IsInWudiProtect()
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_WUDI_PROTECT);
	if (NULL == effect_typeinfo)
	{
		return false;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_WUDI_PROTECT == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		return true;
	}

	return false;
}

bool EffectList::IsInvisible()
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_INVISIBLE);
	if (NULL == effect_typeinfo)
	{
		return false;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_INVISIBLE == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		return true;
	}

	return false;
}

bool EffectList::IsBianshen(short *bianshen_img)
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_BIANSHEN);
	if (NULL == effect_typeinfo)
	{
		return false;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_BIANSHEN == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		if (NULL != bianshen_img) 
		{
			EffectBianshen *effect = (EffectBianshen *)m_all_effect_list[effect_typeinfo->start_index];
			*bianshen_img = effect->GetImage();
		}

		return true;
	}

	return false;
}

bool EffectList::IsForbidRecover()
{
	if (Obj::OBJ_TYPE_ROLE != m_character->GetObjType())
	{
		return false;
	}

	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_FORBID_RECOVER);
	if (NULL == effect_typeinfo)
	{
		return false;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_FORBID_RECOVER == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		return true;
	}

	return false;
}

float EffectList::GetHurtIncPercent(Character *target)
{
	float sum = 0;

	{
		// 固定加伤
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_INC_HURT);
		if (NULL != effect_typeinfo)
		{
			for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
			{
				int index = i + effect_typeinfo->start_index;
				if (NULL != m_all_effect_list[index] && EffectBase::EFFECT_TYPE_INC_HURT == m_all_effect_list[index]->GetEffectType())
				{
					EffectIncHurt *effect = (EffectIncHurt *)m_all_effect_list[index];
					sum += static_cast<float>(effect->GetIncPercent(target));
				}
			}
		}		
	}

	{
		// 随机加伤
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_ATTR_RAND_INC_HURT);
		if (NULL != effect_typeinfo)
		{
			if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
				EffectBase::EFFECT_TYPE_ATTR_RAND_INC_HURT == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
			{
				EffectAttrRandIncHurt *effect = (EffectAttrRandIncHurt *)m_all_effect_list[effect_typeinfo->start_index];
				sum += static_cast<float>(effect->GetIncPercent());
			}
		}		
	}

	{
		// 计数点加伤
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_COUNTER);
		if (NULL != effect_typeinfo)
		{
			for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
			{
				int index = i + effect_typeinfo->start_index;
				if (NULL != m_all_effect_list[index] && EffectBase::EFFECT_TYPE_COUNTER == m_all_effect_list[index]->GetEffectType())
				{
					EffectCounter *effect = (EffectCounter *)m_all_effect_list[index];
					sum += static_cast<float>(effect->GetIncHurtPercent());
				}
			}
		}		
	}

	{
		EffectOtherBuff *other_buff = this->FindOtherEffectBuff(EffectBase::PRODUCT_ID_INC_INHURT_ON_ATK);
		if (NULL != other_buff)
		{
			sum += other_buff->GetIncHurtOnAtk();
		}
	}

	return sum;
}

// 获取免伤百分比
float EffectList::GetHurtDecPercent(Character *target)
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_DEC_HURT);
	if (NULL != effect_typeinfo)
	{
		float sum = 0;
		for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
		{
			int index = i + effect_typeinfo->start_index;

			if (NULL != m_all_effect_list[index] && EffectBase::EFFECT_TYPE_DEC_HURT == m_all_effect_list[index]->GetEffectType())
			{
				EffectDecHurt *effect = (EffectDecHurt *)m_all_effect_list[index];
				sum += static_cast<float>(effect->GetDecPercent(target));
			}
		}

		return sum;
	}

	return 0;
}

float EffectList::GetHurtDecFix(Character *target)
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_DEC_HURT);
	if (NULL != effect_typeinfo)
	{
		float sum = 0;
		for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
		{
			int index = i + effect_typeinfo->start_index;

			if (NULL != m_all_effect_list[index] && EffectBase::EFFECT_TYPE_DEC_HURT == m_all_effect_list[index]->GetEffectType())
			{
				EffectDecHurt *effect = (EffectDecHurt *)m_all_effect_list[index];
				sum += static_cast<float>(effect->GetDecFix(target));
			}
		}

		return sum;
	}

	return 0;
}

float EffectList::GetFixHurtPercent(Character *target)
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_DEADLY_ATTACK);
	if (NULL != effect_typeinfo)
	{
		float sum = 0;
		for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
		{
			int index = i + effect_typeinfo->start_index;

			if (NULL != m_all_effect_list[index] && EffectBase::EFFECT_TYPE_DEADLY_ATTACK == m_all_effect_list[index]->GetEffectType())
			{
				EffectDeadlyAttack *effect = (EffectDeadlyAttack *)m_all_effect_list[index];
				sum += static_cast<float>(effect->GetFixHurtPer(target));
			}
		}

		return sum;
	}

	return 0;
}

EffectDoubleHurt * EffectList::GetDoubleHurtEffect()
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_DOUBLE_HURT);
	if (NULL == effect_typeinfo)
	{
		return NULL;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_DOUBLE_HURT == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		return (EffectDoubleHurt *)m_all_effect_list[effect_typeinfo->start_index];
	}

	return NULL;
}

EffectAbsorbHP * EffectList::GetAbsortHPEffect()
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_ABSORB_HP);
	if (NULL == effect_typeinfo)
	{
		return NULL;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_ABSORB_HP == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		return (EffectAbsorbHP *)m_all_effect_list[effect_typeinfo->start_index];
	}

	return NULL;
}

EffectAbsorbHPBeenAttack * EffectList::GetAbsortHPBeenAttackEffect()
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_ABSORB_HP_BEEN_ATTACK);
	if (NULL == effect_typeinfo)
	{
		return NULL;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_ABSORB_HP_BEEN_ATTACK == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		return (EffectAbsorbHPBeenAttack *)m_all_effect_list[effect_typeinfo->start_index];
	}

	return NULL;
}

EffectBianshen* EffectList::GetBianshenEffect()
{
	EffectTypeInfo *type_info = this->GetTypeInfo(EffectBase::EFFECT_TYPE_BIANSHEN);
	if (NULL == type_info)
	{
		return NULL;
	}

	if (type_info->count > 0 && NULL != m_all_effect_list[type_info->start_index] &&
		EffectBase::EFFECT_TYPE_BIANSHEN == m_all_effect_list[type_info->start_index]->GetEffectType())
	{
		return (EffectBianshen *)m_all_effect_list[type_info->start_index];
	}

	return NULL;
}

int EffectList::GetReboundHurtPercent()
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_REBOUND_HURT);
	if (NULL == effect_typeinfo)
	{
		return 0;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_REBOUND_HURT == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		EffectReboundHurt *effect_rebound = (EffectReboundHurt *)m_all_effect_list[effect_typeinfo->start_index];
		return effect_rebound->GetReboundPercent();
	}

	return 0;
}

int EffectList::GetRecoverPercentOnAtked()
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_RECOVER_ON_ATKED);
	if (NULL == effect_typeinfo)
	{
		return 0;
	}

	if (effect_typeinfo->count > 0 && NULL != m_all_effect_list[effect_typeinfo->start_index] &&
		EffectBase::EFFECT_TYPE_RECOVER_ON_ATKED == m_all_effect_list[effect_typeinfo->start_index]->GetEffectType())
	{
		EffectRecoverOnAtked *effect_recover = (EffectRecoverOnAtked *)m_all_effect_list[effect_typeinfo->start_index];
		return effect_recover->GetRecoverPercent();
	}

	return 0;
}

// 计算合并规则
int EffectList::CalcExistMethod(EffectBase *curr_effect, EffectBase* new_effect)
{
	if (curr_effect->GetEffectType() != new_effect->GetEffectType() ||
		curr_effect->GetBuffType() != new_effect->GetBuffType() ||
		curr_effect->GetMergeRule() != new_effect->GetMergeRule())
	{
		return EffectBase::EXIST_METHOD_NOTHING;
	}

	switch (new_effect->GetMergeRule())
	{
	case EffectBase::MERGE_RULE_MERGE:
		{
			if (curr_effect->GetDeliverer() == new_effect->GetDeliverer())
			{
				return EffectBase::EXIST_METHOD_MERGE;
			}
		}
		break;

	case EffectBase::MERGE_RULE_COUNTER:
		{
			if (curr_effect->GetDeliverer() == new_effect->GetDeliverer() && 
				curr_effect->GetProductMethod() == new_effect->GetProductMethod() &&
				curr_effect->GetProductId() == new_effect->GetProductId())
			{
				return EffectBase::EXIST_METHOD_MERGE;
			}
		}
		break;

	case EffectBase::MERGE_RULE_NOMERGE_1:
		{
			if (new_effect->GetLeftTimeMS() > curr_effect->GetLeftTimeMS())
			{
				return EffectBase::EXIST_METHOD_REPLACE;
			}

			return EffectBase::EXIST_METHOD_IGNORE;
		}
		break;

	case EffectBase::MERGE_RULE_NOMERGE_2:
		{
			if (curr_effect->GetProductMethod() == new_effect->GetProductMethod() &&
				curr_effect->GetProductId() == new_effect->GetProductId())
			{
				if (new_effect->GetProductLevel() > curr_effect->GetProductLevel() ||
					(new_effect->GetProductLevel() == curr_effect->GetProductLevel() && new_effect->GetLeftTimeMS() > curr_effect->GetLeftTimeMS()))
				{
					return EffectBase::EXIST_METHOD_REPLACE;
				}

				return EffectBase::EXIST_METHOD_IGNORE;
			}
		}
		break;

	case EffectBase::MERGE_RULE_NOMERGE_3:
		{
			int cmp_ret = new_effect->CompareWith(curr_effect);
			if (EffectBase::CMP_RET_BIGGER == cmp_ret)
			{
				return EffectBase::EXIST_METHOD_REPLACE;
			}
			
			if (EffectBase::CMP_RET_EQUAL == cmp_ret || EffectBase::CMP_RET_SMALLER == cmp_ret)
			{
				return EffectBase::EXIST_METHOD_IGNORE;
			}			
		}
		break;

	case EffectBase::MERGE_RULE_NOMERGE_4:
		{
			if (curr_effect->GetDeliverer() == new_effect->GetDeliverer() && 
				curr_effect->GetProductMethod() == new_effect->GetProductMethod() &&
				curr_effect->GetProductId() == new_effect->GetProductId())
			{
				if (new_effect->GetProductLevel() > curr_effect->GetProductLevel() ||
					(new_effect->GetProductLevel() == curr_effect->GetProductLevel() && new_effect->GetLeftTimeMS() > curr_effect->GetLeftTimeMS()))
				{
					return EffectBase::EXIST_METHOD_REPLACE;
				}

				return EffectBase::EXIST_METHOD_IGNORE;
			}
		}
		break;

	case EffectBase::MERGE_RULE_NOMERGE_5:
		{
			if (curr_effect->GetDeliverer() == new_effect->GetDeliverer() && 
				curr_effect->GetProductMethod() == new_effect->GetProductMethod() &&
				curr_effect->GetProductId() == new_effect->GetProductId())
			{
				if (new_effect->GetProductLevel() == curr_effect->GetProductLevel())
				{
					new_effect->SetLeftTimeMS(new_effect->GetLeftTimeMS() + curr_effect->GetLeftTimeMS());
					return EffectBase::EXIST_METHOD_REPLACE;
				}

				else if (new_effect->GetProductLevel() > curr_effect->GetProductLevel())
				{
					return EffectBase::EXIST_METHOD_REPLACE;
				}

				return EffectBase::EXIST_METHOD_IGNORE;
			}
		}

		break;

	case EffectBase::MERGE_RULE_NOMERGE_6:
		{
			if (curr_effect->GetDeliverer() == new_effect->GetDeliverer() &&
				curr_effect->GetProductMethod() == new_effect->GetProductMethod() &&
				curr_effect->GetProductId() == new_effect->GetProductId())
			{
				if (new_effect->GetProductLevel() == curr_effect->GetProductLevel())
				{
					return EffectBase::EXIST_METHOD_MERGE;
				}

				else if (new_effect->GetProductLevel() > curr_effect->GetProductLevel())
				{
					return EffectBase::EXIST_METHOD_REPLACE;
				}

				return EffectBase::EXIST_METHOD_IGNORE;
			}
		}
		break;

	case EffectBase::MERGE_RULE_IGNORE:
		{
			return EffectBase::EXIST_METHOD_IGNORE;
		}
		break;
	}

	return EffectBase::EXIST_METHOD_NOTHING;
}

unsigned int EffectList::GenerateKey()
{
	return ++ m_max_unique_key;
}

void EffectList::Add(EffectBase *new_effect)
{
	if (NULL == new_effect) return;

	bool is_init = m_first;

	int effect_type = new_effect->GetEffectType();

	if (effect_type > EffectBase::EFFECT_TYPE_INVALID && effect_type < EffectBase::EFFECT_TYPE_RESERVE)
	{
		this->AddSkillEffect(new_effect, is_init);

		return;
	}

	if (EffectBase::EFFECT_TYPE_ONCE == effect_type)
	{
		if (NULL == new_effect || new_effect->GetEffectType() < EffectBase::EFFECT_TYPE_RESERVE) return;

		this->AddEffectToTypeList(new_effect, EffectBase::EFFECT_TYPE_RESERVE, is_init);

		return;
	}

	delete new_effect;
}

void EffectList::AddSkillEffect(EffectBase *new_effect, bool is_init)
{
	int effect_type = new_effect->GetEffectType();

	if (effect_type <= EffectBase::EFFECT_TYPE_INVALID || effect_type >= EffectBase::EFFECT_TYPE_RESERVE) return;

	{
		// 检查控制效果衰减
		if (!is_init)
		{
			bool remove = false;
			this->CheckCtrlEffectReduceTime(new_effect, &remove);

			if (remove)
			{
				delete new_effect;
				return;
			}
		}
	}

	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
	if (NULL == effect_typeinfo)
	{
		return;
	}

	int i = 0;
	for (; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
	{
		int index = effect_typeinfo->start_index + i;

		if (NULL == m_all_effect_list[index]) continue;

		int exist_method = this->CalcExistMethod(m_all_effect_list[index], new_effect);
		switch (exist_method)
		{
		case EffectBase::EXIST_METHOD_MERGE: // 合并
			{
				this->Merge(index, new_effect, is_init);
				return;
			}
			break;

		case EffectBase::EXIST_METHOD_REPLACE: // 替换
			{
				this->Replace(index, new_effect, is_init);
				return;
			}
			break;

		case EffectBase::EXIST_METHOD_IGNORE: // 忽略
			{
				delete new_effect;
				return;
			}
			break;
		}
	}

	this->AddEffectToTypeList(new_effect, effect_type, is_init);
}

void EffectList::AddEffectToTypeList(EffectBase *new_effect, char effect_type, bool is_init)
{
	if (NULL == new_effect || effect_type <= EffectBase::EFFECT_TYPE_INVALID || effect_type > EffectBase::EFFECT_TYPE_RESERVE) return;

	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(effect_type);
	if (NULL == effect_typeinfo)
	{
		return;
	}

	if (effect_typeinfo->count >= 0 && effect_typeinfo->count < effect_typeinfo->max_count)
	{
		this->SetEffectKey(new_effect);

		m_all_effect_list[effect_typeinfo->start_index + effect_typeinfo->count] = new_effect;
		++effect_typeinfo->count;

		if (!is_init) 
		{
			bool die = false;
			new_effect->Effect(m_character, &die);

			this->SendEffectInfo(new_effect, Protocol::SCEffectInfo::NOTIFY_REASON_ADD);
		}

		if (new_effect->GetBuffType() > 0)
		{
			this->AddSpecialBuff(new_effect->GetBuffType());
		}

		if (EffectBase::EFFECT_TYPE_BIANSHEN == new_effect->GetEffectType())
		{
			this->SetBianshenAppearance(new_effect->GetImage());
		}
	}
	else
	{
		delete new_effect;
	}
}

// 替换效果
void EffectList::Replace(int exist_index, EffectBase *new_effect, bool is_init)
{
	if (exist_index < 0 || exist_index >= static_cast<int>(sizeof(m_all_effect_list) / sizeof(m_all_effect_list[0]))) return;

	if (NULL != m_all_effect_list[exist_index] && NULL != new_effect)
	{
		if (!is_init) m_all_effect_list[exist_index]->ClearEffect(m_character);

		int del_effect_key = m_all_effect_list[exist_index]->GetUniqueKey();

		delete m_all_effect_list[exist_index];

		this->SetEffectKey(new_effect);

		m_all_effect_list[exist_index] = new_effect;

		if (!is_init) 
		{
			bool die = false;
			new_effect->Effect(m_character, &die);

			this->NotifyEffectRemove(del_effect_key);
			this->SendEffectInfo(new_effect, Protocol::SCEffectInfo::NOTIFY_REASON_ADD);
		}
	}
}

// 合并效果
void EffectList::Merge(int exist_index, EffectBase *new_effect, bool is_init)
{
	if (exist_index < 0 || exist_index >= static_cast<int>(sizeof(m_all_effect_list) / sizeof(m_all_effect_list[0]))) return;

	if (NULL != m_all_effect_list[exist_index] && NULL != new_effect)
	{
		m_all_effect_list[exist_index]->Merge(new_effect, m_character);
		delete new_effect;
		
		if (!is_init)
		{
			this->SendEffectInfo(m_all_effect_list[exist_index], Protocol::SCEffectInfo::NOTIFY_REASON_CHANGE);
		}		
	}	
}

EffectTypeInfo *EffectList::GetTypeInfo(char effect_type)
{
	EffectTypeInfo *effect_typeinfo = NULL;

	UNSTD_STATIC_CHECK(MAX_EFFECT_COUNT >= START_INDEX_FORBID_RECOVER + MAX_COUNT_FORBID_RECOVER);

	if (effect_type > EffectBase::EFFECT_TYPE_INVALID && effect_type <= EffectBase::EFFECT_TYPE_RESERVE)
	{
		effect_typeinfo = &m_effect_typeinfo_list[static_cast<int>(effect_type)];
	}

	return effect_typeinfo;
}

int EffectList::SerializeEffectInfo(char *buffer, EffectInfo *effect_info)
{
	int size = 0;
	char *pt = buffer;

	int offset = static_cast<int>(UNSTD_OFFSETOFF(EffectInfo, param_list));

	memcpy(pt, (char*)effect_info, offset);	
	pt += offset; size += offset;

	for (int i = 0; i < effect_info->param_count; ++i)
	{
		*(long long *)pt = effect_info->param_list[i]; pt += sizeof(long long); size += sizeof(long long);
	}

	return size;
}

void EffectList::SetEffectKey(EffectBase *effect)
{
	if (NULL != effect && 0 == effect->GetUniqueKey())
	{
		unsigned int effect_key = this->GenerateKey();
		effect->SetUniqueKey(effect_key);
	}
}

void EffectList::CheckCtrlEffectReduceTime(EffectBase *effect, bool *remove)
{
	if (NULL == effect || EffectBase::EFFECT_TYPE_SPECIAL_STATE != effect->GetEffectType() || 
		EffectBase::PRODUCT_METHOD_SKILL != effect->GetProductMethod()) return;

	if (Obj::OBJ_TYPE_ROLE != m_character->GetObjType() || NULL == m_character->GetScene()) return;

	Obj *deliverer_obj = m_character->GetScene()->GetObj(effect->GetDeliverer());
	if (NULL == deliverer_obj || Obj::OBJ_TYPE_ROLE != deliverer_obj->GetObjType()) return;
	
	if (EBT_XUANYUN == effect->GetBuffType())
	{
		unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		if (now_second <= m_xuanyun_effect_reduce_end_time)
		{
			++ m_xuanyun_effect_reduce_count;
		}
		else
		{
			m_xuanyun_effect_reduce_end_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + GLOBALCONFIG->GetCtrlEffectReduceCircleS();
			m_xuanyun_effect_reduce_count = 1;
		}

		int reduce_percent = GLOBALCONFIG->GetCtrlEffectReducePercent(m_xuanyun_effect_reduce_count);
		int real_effect_time_ms = static_cast<int>(effect->GetLeftTimeMS() * (reduce_percent / 100.0f));

		if (real_effect_time_ms != effect->GetLeftTimeMS())
		{
			EffectSpecialState *effect_special = (EffectSpecialState *)effect;
			effect_special->SetLeftTimeMS(real_effect_time_ms);

			if (real_effect_time_ms <= 0)
			{
				if (NULL != remove) *remove = true;
			}
		}
	}
}

// 重算属性效果
void EffectList::Recalc()
{
	{
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_ATTRBUFF);
		if (NULL == effect_typeinfo)
		{
			return;
		}

		for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
		{
			int index = i + effect_typeinfo->start_index;
			if (NULL != m_all_effect_list[index] && EffectBase::EFFECT_TYPE_ATTRBUFF == m_all_effect_list[index]->GetEffectType())
			{
				EffectBuff *effect = (EffectBuff *)m_all_effect_list[index];
				effect->Recalc(m_character);
			}
		}
	}

	{
		EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_MOVESPEED);
		if (NULL == effect_typeinfo)
		{
			return;
		}

		for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
		{
			int index = i + effect_typeinfo->start_index;
			if (NULL != m_all_effect_list[index] && EffectBase::EFFECT_TYPE_MOVESPEED == m_all_effect_list[index]->GetEffectType())
			{
				EffectMoveSpeed *effect = (EffectMoveSpeed *)m_all_effect_list[index];
				effect->Recalc(m_character);
			}
		}
	}
}

// 重算百分比属性
void EffectList::ReCalcPercentAttr(CharIntAttrs &base_add)
{
	EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFFECT_TYPE_ATTR_PER);
	if (NULL == effect_typeinfo)
	{
		return;
	}

	for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
	{
		int index = i + effect_typeinfo->start_index;
		if (NULL != m_all_effect_list[index] && EffectBase::EFFECT_TYPE_ATTR_PER == m_all_effect_list[index]->GetEffectType())
		{
			EffectPercentBuff *effect = (EffectPercentBuff *)m_all_effect_list[index];
			effect->ReCalcPercent(base_add);
		}
	}
}

// 重算基础属性
void EffectList::ReCalcBaseAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		{
			EffectTypeInfo *effect_typeinfo = this->GetTypeInfo(EffectBase::EFEECT_TYPE_OTHER);
			if (NULL == effect_typeinfo)
			{
				return;
			}

			for (int i = 0; i < effect_typeinfo->count && i < effect_typeinfo->max_count; ++i)
			{
				int index = i + effect_typeinfo->start_index;
				if (NULL != m_all_effect_list[index] && EffectBase::EFEECT_TYPE_OTHER == m_all_effect_list[index]->GetEffectType())
				{
					EffectOtherBuff *effect = (EffectOtherBuff *)m_all_effect_list[index];
					effect->ReCalcBaseAttr(m_attrs_add);
				}
			}
		}
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

// 造成的其他伤害
void EffectList::OnHurtOther(Character *target, int skill_id, long long injure)
{
	if (NULL == target || injure <= 0)
	{
		return;
	}

	EffectAbsorbHP *absorb_hp = this->GetAbsortHPEffect();
	if (NULL != absorb_hp)
	{
		int absorb_percent = absorb_hp->GetAbsorbPercent();

		if (absorb_percent > 0)
		{
			long long add_hp = (long long)(injure * (absorb_percent / 100.0f));

			EffectOnce *addhp_effect = new EffectOnce(INVALID_OBJ_ID, 0, add_hp, EffectBase::FIGHT_TYPE_NORMAL);
			m_character->AddEffect(addhp_effect);
		}
	}
}

bool EffectList::IsControlBuffType(unsigned char buff_type)
{
	return buff_type == EBT_XUANYUN || buff_type == EBT_FROZEN || buff_type == EBT_DINGSHEN ||
		buff_type == EBT_CHENMO || buff_type == EBT_CHIHUAN || buff_type == EBT_BIANXING_FOOL;
}

bool EffectList::CanClearByFuncClearAllDeBuffEffect(EffectBase *effect)
{
	if (nullptr == effect)	//空指针肯定不能清除
	{
		return false;
	}

	int product_method = effect->GetProductMethod();
	if (EffectBase::PRODUCT_METHOD_SKILL == product_method || EffectBase::PRODUCT_METHOD_IMG_FULING == product_method ||
		EffectBase::PRODUCT_METHOD_XIANNV_SHENGWU_SKILL == product_method || EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL == product_method)
	{
		return true;
	}

	return false;
}