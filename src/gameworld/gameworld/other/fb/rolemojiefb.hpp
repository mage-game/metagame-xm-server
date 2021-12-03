#ifndef __ROLE_MOJIE_FB_HPP__
#define  __ROLE_MOJIE_FB_HPP__

class Role;

#include "servercommon/fbdef.hpp"
#include "mojiefbconfig.hpp"
#include "obj/character/attribute.hpp"

class Character;
class Role;

class RoleMojieFB
{
public:
	RoleMojieFB();
	~RoleMojieFB();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const MojieFBParam &param);
	void GetInitParam(MojieFBParam *param);
	void OnPassRenLeiFb(int fb_index);
	void OnPassJingLingFb(int chapter, int param1 = -1, int param2 = -1, int param3 = -1);
	void OnChangeFangyu();
	void OnPutonEquip(short equip_type, short quality, short color);
	void OnKillMonster(int monster_id);
	bool IsUseSkill(int skill_type);
	double GetJingLingPianExpPer(int exp_per);

	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag);


	bool CanEnter(int fb_index);
	void OnEnterFB(int fb_index);

	const MojieFBParam * GetMojieFbParam();
	
	void SendInfo();


private:
	Role *m_role;

	MojieFBParam m_param;
};

#endif // __ROLE_MOJIE_FB_HPP__

