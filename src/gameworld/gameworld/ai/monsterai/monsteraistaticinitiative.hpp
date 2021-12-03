#ifndef __MONSTERAISTATICINITIATIVE_HPP__
#define __MONSTERAISTATICINITIATIVE_HPP__

#include "monsteraibase.hpp"

// ��ֹ�����֣���ֹ����������
class MonsterAIStaticInitiative : public MonsterAIBase
{
public:
	MonsterAIStaticInitiative(Character *me, int aitype = MonsterInitParam::AITYPE_STAT_INITIAT);
	virtual ~MonsterAIStaticInitiative();

	virtual void Init(const MonsterAIParam &mp);

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);

	virtual void AI(unsigned long interval);

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	bool FindEnemy(unsigned long interval);

	unsigned int m_aoi_range;
	unsigned int m_aoi_handle;

	const static int MAX_ENEMY_NUM	= 5;	// �ɼ�¼��������
	ObjID	m_enemy_list[MAX_ENEMY_NUM];	// �����б�
	int		m_enemy_size;					// ��ǰ������Ŀ
};

#endif

