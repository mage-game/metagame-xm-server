#ifndef __HUGUOZHILI_H__
#define __HUGUOZHILI_H__

// 天神护体系统

#include "servercommon/huguozhilidef.hpp"

class Role;

class Huguozhili
{
public:
	Huguozhili() : m_role(nullptr), m_client_buff_key(0), m_next_check_client_buff_time(0) {}

	void SetRole(Role *role) { m_role = role; }
	void Init(const HuguozhiliParam &param) {  m_param = param; }
	void GetInitParam(HuguozhiliParam *param) { *param = m_param; }

	void Update(unsigned int now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnRoleDie();
	void OnAttackCalcHurt(long long &delta_hp, ObjID attacker);

	void SendHuguozhiliInfo();

	// client req
	void OnReq(int req_type);

private:
	void OnActiveReq();					// 请求激活（兑换）天神护体

	bool IsActived() const;				// 是否已激活天神护体
	void CheckMakeClientBuff();			// 检测发送一次buff信息给客户端

	Role *m_role;
	HuguozhiliParam m_param;

	//int m_is_first;
	unsigned int m_client_buff_key;		// 用于记录，通知客户端删除buff
	unsigned int m_next_check_client_buff_time;
};

#endif // __HUGUOZHILI_H__