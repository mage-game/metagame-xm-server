#ifndef GATHEROBJ_H
#define GATHEROBJ_H

#include "obj/obj.h"
#include "servercommon/struct/itemlistparam.h"

#include <set>

class Role;
class GatherGenerater;
struct LingMaiItem;

// ����ɼ������ͣ������ڷ�Э��ʱ���Ĵ����в�ͬ�ĺ���
enum SPECIAL_GATHER_TYPE
{
	SPECIAL_GATHER_TYPE_JINGHUA = 1,		// �����ɼ�
	SPECIAL_GATHER_TYPE_GUILDBATTLE = 2,	// �������Բɼ���
	SPECIAL_GATHER_TYPE_BIGCOIN = 3,		// ���ͭ�Ҵ�ͭ��
	SPECIAL_GATHER_TYPE_RESERVE_2 = 4,		// 
	SPECIAL_GATHER_TYPE_GUILD_BONFIRE = 5,	// ��������
	SPEICAL_GATHER_TYPE_CROSS_FISHING = 6,	// ���������¨
	SPEICAL_GATHER_TYPE_CROSS_GUILD_BATTLE_BOSS= 7, //������ɳ�ħ
	SPECIAL_GATHER_TYPE_HUSONG_SHUIJING = 8,// �������ˮ��
};

class GatherObj : public Obj
{
public:
	GatherObj();
	virtual ~GatherObj();

	void Init(GatherGenerater *gather_generater, int gather_id, int gather_time, int evil_add, bool disappear_after_gather, 
		int disappear_s = 0, int disappear_gather_max = 0);
	
	int GatherId() { return m_gather_id; }
	int GatherTime(const UserID &user_id) { return m_gather_time; }
	bool HasGather() { return m_has_gather; }

	void Gather(Role *gathar_role);
	bool CanGather(Role *gather_role);
													
	virtual void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	bool HasGenerater() { return NULL != m_gather_generater; }
	GatherGenerater *GetGatherGenerater() { return m_gather_generater; }
	void SetGenerater(GatherGenerater *g) { m_gather_generater = g; }

	void *			operator new(size_t c);
	void			operator delete(void *m);

	void InitBonfire(GuildID guild_id, GuildName guild_name);
	bool CanAddMuCai(Role * role);
	void AddMuCai(Role *role);
	
	GuildID GetBelongGuildID() { return m_belong_guildid; }
	void GetBelongGuildName(GuildName guild_name) { F_STRNCPY(guild_name, m_belong_guildname, sizeof(GuildName)); }
	bool IsBonfire();
	bool IsMillionaireBox();
	bool IsShenzhouGather();

	void SetSpecialGatherType(int special_gather_type) { m_special_gather_type = special_gather_type; }
	int GetSpecialGatherType() { return m_special_gather_type; }

	void OnGatherChange();

	long long GetData() { return m_data; }
	void SetData(int data) { m_data = data; }

	void InitCrossFishingGather(Int64 unique_user_id, const GameName name);
	void GetBelongName(GameName own_name) { F_STRNCPY(own_name, m_own_name, sizeof(GameName)); }

	void SetGatherIngObjId(ObjID obj_id) { m_gather_ing_obj_id = obj_id; }
	ObjID GetGatherIngObjId() { return m_gather_ing_obj_id; }
	const int GetLeftGatherTimes() { return m_disappear_gather_max - m_cur_gather_count; }
	void SetGatherTimesMax(int gather_time_max) { m_disappear_gather_max = gather_time_max;}

	void InitCrossGuildBattleBoss(GuildName guild_name);
private:
	void RemoveOnGather(Role *gathar_role);
	bool CanGatherBonfire(Role *gathar_role, bool notice = true);
	void OnGatharBonfire(Role *gathar_role);
	void UpdateBonfire(unsigned long interval, time_t now_second, unsigned int now_dayid);
	
	int m_gather_id;															// �ɼ�ID
	int m_gather_time;															// �ɼ�ʱ��
	GatherGenerater *m_gather_generater;										// �ɼ������� ����ΪNULL
	int m_evil_add;																// ���ֵ
	
	bool m_disappear_after_gather;												// �ɼ����Ƿ���ʧ
	bool m_has_gather;															// �Ƿ��Ѿ����ɼ�

	unsigned int m_disappear_timestamp;											// ��ʧʱ���
	int m_disappear_gather_max;													// ���ɼ�����
	int m_cur_gather_count;														// ��ǰ�ɼ�����
	long long m_data;

	unsigned int m_bonfire_next_reward_time;									// �����´�ʱ�佱��
	GuildID m_belong_guildid;													// ��������ID
	GuildName m_belong_guildname;												// ������������

	int m_special_gather_type;													// ����ɼ�������

	GameName m_own_name;														// �ɼ������������

	ObjID m_gather_ing_obj_id;													// �Ƿ����˲ɼ���
};

#endif

