#ifndef __FRIEND_EXP_BOTTLE_HPP__
#define __FRIEND_EXP_BOTTLE_HPP__

#include "servercommon/struct/friendexpbottleparam.hpp"
#include "other/friendexpbottle/friendexpbottleconfig.hpp"

class Role;

class FriendExpBottle
{
public:
	FriendExpBottle();
	~FriendExpBottle();

	static const unsigned int ADD_BOTTLE_EXP_INTERVAL = 60;

	enum FRIEND_EXP_BOTTLE_OPERATE_TYPE
	{
		FRIEND_EXP_BOTTLE_OPERATE_TYPE_ADD_EXP = 0,			//	��ȡ����
		FRIEND_EXP_BOTTLE_OPERATE_TYPE_BROADCAST,			//  ������
		FRIEND_EXP_BOTTLE_OPERATE_TYPE_REQ_INFO,			//  ������Ϣ
	};

	enum FRIEND_EXP_BOTTLE_STATE
	{
		FRIEND_EXP_BOTTLE_STATE_NOT_ACTIVE = 0,				//  δ����
		FRIEND_EXP_BOTTLE_STATE_NOT_FULL,					//  δ��
		FRIEND_EXP_BOTTLE_STATE_FRIEND_NOT_ENOUGH,			//  �������޲���
		FRIEND_EXP_BOTTLE_STATE_FULL,						//  ����ȡ
	};

	enum FRIEND_EXP_BOTTLE_SEND_TYPE
	{
		FRIEND_EXP_BOTTLE_SEND_TYPE_ADD_EXP = 0,			//  �Ӿ���
		FRIEND_EXP_BOTTLE_SEND_TYPE_BROADCAST,				//  ����
	};

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const FriendExpBottleParam &param);
	void GetInitParam(FriendExpBottleParam *param);

	void Update(unsigned int now_second);

	void OnRoleLogin();
	void OnRoleLogout();
	void OnRoleUpLevel(int level);

	void OnFriendExpBottleOperateReq(int type);

private:
	void AddRoleExp();
	void Broadcast();

	void AddBottleExp();

	void SendFriendExpBottleInfo();

	FriendExpBottleParam m_param;
	Role *m_role;

	unsigned int m_next_add_bottle_exp_time;
	unsigned int m_next_broadcast_time;
	unsigned int m_next_auto_add_friend_num_time;
	bool m_is_notice;
};

#endif // __FRIEND_EXP_BOTTLE_HPP__
