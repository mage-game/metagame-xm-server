#ifndef __SPECIAL_AI_LINGSHANXIANQI_HPP__
#define __SPECIAL_AI_LINGSHANXIANQI_HPP__

#include "ai/monsterai/monsteraibase.hpp"

class MonsterAILingShanXianQi : public MonsterAIBase
{
public:
	MonsterAILingShanXianQi(Character *me);
	virtual ~MonsterAILingShanXianQi();

	virtual void Init(const MonsterAIParam &mp);

	void SetOwnerInfo(const UserID &user_id, ObjID user_objid);
	void ClearOwnerInfo();
	const UserID & GetOwnerUserID() { return m_owner_user_id; }
	bool HasOwner() { return INVALID_USER_ID != m_owner_user_id; }

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	virtual bool IsEnemy(Obj *obj) { return false; }
	virtual bool IsFriend(Obj *obj, bool check_alive = true) { return false; }

	Role * GetFollowRole();
	void FollowRole(unsigned long interval);

	static const int JUMP_TO_ROLE_DISTANCE = 10;	// �����˾���ֱ�ӷɵ���ɫ���
	static const int FOLLOW_ROLE_INTERVAL = 600;	// ����ʱ���� ����
	static const int BESIDE_ROLE_LENGTH = 5;		// �����ɫ��ߵ�ƫ�ƾ���

	UserID m_owner_user_id;							// �����û�ID
	ObjID m_owner_user_objid;						// ���˶���ID
};

#endif // __SPECIAL_AI_LINGSHANXIANQI_HPP__


