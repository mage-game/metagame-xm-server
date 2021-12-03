#include "huguozhili.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "huguozhiliconfig.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "servercommon/errornum.h"

void Huguozhili::Update(unsigned int now_second)
{
	//if (m_is_first)
	//{
	//	m_is_first = 0;

	//	this->CheckMakeClientBuff();
	//}

	if (m_client_buff_key > 0 && now_second >= m_next_check_client_buff_time)
	{
		m_next_check_client_buff_time = now_second + 1;

		if (!this->IsActived())
		{
			m_role->GetEffectList()->NotifyEffectRemove(m_client_buff_key);
			m_client_buff_key = 0;
		}
	}
}

void Huguozhili::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	m_param.today_die_times = 0;
	m_param.today_active_times = 0;
}

void Huguozhili::OnRoleDie()
{
	++m_param.today_die_times;

	this->SendHuguozhiliInfo();
}

void Huguozhili::OnAttackCalcHurt(long long &delta_hp, ObjID attacker)
{
	/*
	 天神护体将最大伤害限制在气血最大值的X%内
	*/

	if (!this->IsActived())
	{
		return;
	}

	{
		Obj *attacker_obj = m_role->GetScene()->GetObj(attacker);
		if (attacker_obj && attacker_obj->GetObjType() != Obj::OBJ_TYPE_ROLE)
		{
			return;
		}
	}

	auto other_cfg = LOGIC_CONFIG->GetHuguozhiliConfig().GetOtherConfig();
	if (other_cfg)
	{
		int max_hurt = static_cast<int>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * 0.01 * other_cfg->injure_maxhp_per);
		if (std::abs(delta_hp) > max_hurt)
		{
			delta_hp = -max_hurt;
		}
	}
}

void Huguozhili::SendHuguozhiliInfo()
{
	Protocol::SCHuguozhiliInfo hgzli;
	hgzli.info = m_param;
	
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&hgzli, sizeof(hgzli));

	// 顺便把buff发一下
	this->CheckMakeClientBuff();
}

void Huguozhili::OnReq(int req_type)
{
	switch (req_type)
	{
	case Protocol::CSHuguozhiliReq::REQ_INFO:
	{
		this->SendHuguozhiliInfo();
	}
	break;

	case Protocol::CSHuguozhiliReq::REQ_ACTIVE_HUGUOZHILI:
	{
		this->OnActiveReq();
	}
	break;
	}
}

void Huguozhili::OnActiveReq()
{
	//激活（兑换）天神护体

	// 检查：可兑换次数，激活中，激活CD中，死亡中
	int can_active_times = LOGIC_CONFIG->GetHuguozhiliConfig().GetCanActiveTimes(m_param.today_die_times);
	if (m_param.today_active_times >= can_active_times)
	{
		m_role->NoticeNum(errornum::EN_HUGUOZHILI_CANNOT_ACTIVE);
		return;
	}

	auto other_cfg = LOGIC_CONFIG->GetHuguozhiliConfig().GetOtherConfig();
	if (!other_cfg) return;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time()); 
	if (now_sec < m_param.active_huguozhili_timestamp + other_cfg->buff_interval_s)
	{
		m_role->NoticeNum(errornum::EN_HUGUOZHILI_ALREADY_ACTIVE);
		return;
	}

	if (now_sec < m_param.active_huguozhili_timestamp + other_cfg->buff_cd_s)
	{
		m_role->NoticeNum(errornum::EN_HUGUOZHILI_IN_CD);
		return;
	}

	if (m_role->IsAlive())
	{
		m_role->NoticeNum(errornum::EN_HUGUOZHILI_ALIVE);
		return;
	}

	// 消耗：增加激活次数
	++m_param.today_active_times;

	// 给予：激活护国之力
	m_param.active_huguozhili_timestamp = now_sec;

	// 发送 & Log
	this->SendHuguozhiliInfo();

}

bool Huguozhili::IsActived() const
{
	auto other_cfg = LOGIC_CONFIG->GetHuguozhiliConfig().GetOtherConfig();
	if (!other_cfg) return false;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	return now_sec < m_param.active_huguozhili_timestamp + other_cfg->buff_interval_s;
}

void Huguozhili::CheckMakeClientBuff()
{
	if (this->IsActived())
	{
		// 删了之前的，避免显示多条
		if (m_client_buff_key > 0)
		{
			m_role->GetEffectList()->NotifyEffectRemove(m_client_buff_key);
			m_client_buff_key = 0;
		}

		auto other_cfg = LOGIC_CONFIG->GetHuguozhiliConfig().GetOtherConfig();
		if (other_cfg)
		{
			unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			int interval_s = static_cast<int>(m_param.active_huguozhili_timestamp + other_cfg->buff_interval_s - now_sec);

			// 提醒一下客户端就好了
			EffectSpecialState special_state(m_role->GetId(), EffectBase::PRODUCT_ID_HUGUOZHILI, interval_s * 1000, EBT_HUGUOZHILI, 1);
			special_state.SetClientEffectType(EffectBase::ECT_HUGUOZHILI);
			m_role->GetEffectList()->SetEffectKey(&special_state);
			m_client_buff_key = special_state.GetUniqueKey();
			m_role->GetEffectList()->SendEffectInfo(&special_state, 0);
		}
	}
}
