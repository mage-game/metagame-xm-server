
#ifndef NPC_H
#define NPC_H

#include "gamedef.h"

class Role;
struct ConfigSceneNpc;
struct ConfigSceneNpcFuntion;
typedef std::vector<ConfigSceneNpcFuntion> ConfigSceneNpcFuntionList;

class NPC
{
public:
	static const int MAX_FUNCTION_NUM = 16;

	NPC();
	~NPC();

	void Init(const ConfigSceneNpc &npcconfig);

	UInt16 GetNPCId() const { return m_npc_id; }
	const Posi & GetPos() const { return m_pos; }
	
	bool CanTouch(Role *role, bool notify = true);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:

	UInt16			m_npc_id;							// NPC ID
	Posi			m_pos;								// λ��
	int				m_default_dialog_id;				// Ĭ�϶Ի�ID
};

#endif
