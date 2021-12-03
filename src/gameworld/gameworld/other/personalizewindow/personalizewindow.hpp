#ifndef __PERSONALIZE_WINDOW_H__
#define __PERSONALIZE_WINDOW_H__

#include "servercommon/personalizewindowdef.hpp"
#include "obj/character/attribute.hpp"
#include <set>

class Role;

class PersonalizeWindow
{
public:
	PersonalizeWindow();
	~PersonalizeWindow();

	void SetRole(Role *role) { m_role = role; }
	void InitParam(Role *role, const PersonalizeWindowParam &param);
	void GetInitParam(PersonalizeWindowParam *param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SendBubbleWindowInfo();

	bool OnBubbleWindowUpLevel(short bubble_type);
	void OnUseBubbleWindwo(short buble_type);

	int GetCurUseBubbleWindow();

	void SendAvatarWindowInfo();

	bool OnAvatarWindowUpLevel(short avatar_type);
	void OnUseAvatarWindow(short avatar_type);

	int GetCurUseAvatarWindowForClient() { return m_param.cur_use_avatar_type; }	

private:

	void SetAvatarTypeWindow(short avatar_type);
	void SetBubbleTypeWindow(short bubble_type);

	Role *m_role;
	PersonalizeWindowParam m_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

};

#endif

