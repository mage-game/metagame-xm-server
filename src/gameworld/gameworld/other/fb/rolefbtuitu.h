#pragma once
#ifndef __ROLE_FB_TUITU_H__
#define __ROLE_FB_TUITU_H__
#include "servercommon/fbdef.hpp"
class Role;
namespace Protocol 
{
	class CSTuituFbOperaReq;
}

const static int TUITU_HARD_FB_PASS_STAR = 3;				// 推图副本精英本通关星数

class RoleTuituFb
{
public:
	RoleTuituFb();
	~RoleTuituFb();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const TuituFbParam &param);
	void GetInitParam(TuituFbParam *param);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool CanEnterFb(int fb_type, int chapter, int level);
	void OnEnterFb(int fb_type, int chapter, int level);
	void OnFbFinish(int fb_type, int chapter, int level, int star);
	void GetEnterFbParam(int &fb_type, int &chapter, int &level);
	bool GetNextLayerParam(int &fb_type, int &chapter, int &level);
	bool IsPassFb(int fb_type, int chapter, int level);

	void OnTuituOperaReq(Protocol::CSTuituFbOperaReq *tfor);
	void OnBuyTimes(int fb_type, int buy_times);
	void OnFetchStarReward(int chapter, int seq);
	void OnSaodangFb(int fb_type, int chapter, int level);

	void SendFbInfo();
	void SendSingleLayerInfo(int fb_type, int chapter, int level);

private:

	Role *m_role;
	TuituFbParam m_param;
};

#endif