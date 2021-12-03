#ifndef __ROLE_STORY_FB_HPP__
#define  __ROLE_STORY_FB_HPP__

class Role;

#include "servercommon/fbdef.hpp"
#include "storyfbconfig.hpp"

class RoleStoryFB
{
public:
	RoleStoryFB();
	~RoleStoryFB();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const StoryFBParam &param);
	void GetInitParam(StoryFBParam *param);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool CanEnter(int fb_index);
	void OnEnterFB(int fb_index);
	
	void AutoFBReqAll();
	void AutoFBReqOne(int fb_index);
	void SendInfo();

	int GetCurrFbIndex() { return m_param.curr_fb_index; }
	void OnPassLevel(int fb_index);
	bool IsPassLevel(int fb_index);

private:
	Role *m_role;

	StoryFBParam m_param;
};

#endif // __ROLE_STORY_FB_HPP__

