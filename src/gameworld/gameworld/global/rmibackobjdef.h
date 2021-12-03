#ifndef _GL_RMIBACKOBJDEF_H_
#define _GL_RMIBACKOBJDEF_H_

#include "rmiclient/rmiglobal.hpp"
#include "servercommon/darmi/rmisystemclient.h"
#include "gamelog.h"

class WorldStatus;
class ChallengeField;
class FishPoolManager;
class GlobalRoleManager;

class RMISaveCampBackObjectImpl : public RMISaveCampBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveCampRet(int ret) 
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveCampBackObjectImpl SaveCampRet Failed ret:%d\n", ret);
		}
	}
};

class RMIInitGlobalRoleObjectImpl : public RMIInitGlobalRoleBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

	GlobalRoleManager *global_role_manager;

protected:
	virtual void InitGlobalRet(int ret, const GlobalRoleDatalListParam &global_role_list_param);
};

class RMIInitCampBackObjectImpl : public RMIInitCampBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitCampRet(int ret, const CampParam &camp_param);
};

class RMICreateGuildBackObjectImpl : public RMICreateGuildBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

	GuildBaseParam guild_base;

	UserID user_id;
	GameName role_name;
	PlatName plat_name;
	int level;
	char sex;
	char prof;
	char camp;
	char vip_level;
	int capability;
	char create_guild_type;
	ItemDataWrapper item_wrapper;

protected:
	virtual void CreateGuildRet(int ret, GuildID guild_id);
};

class RMIDismissGuildBackObjectImpl : public RMIDismissGuildBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

	int guild_id;
	UserID user_id;
	int dismiss_type;

protected:
	virtual void DismissGuildRet(int ret);
};

class RMIInitGuildBackObjectImpl : public RMIInitGuildBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitGuildRet(int ret, const AllGuildParam &all_guild_list);
};

class RMISaveGuildBackObjectImpl : public RMISaveGuildBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveGuildRet(int ret) 
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveGuildBackObjectImpl SaveGuildRet Failed ret:%d\n", ret);
		}
	}
};

class RMISavePublicSaleBackObjectImpl : public RMISavePublicSaleBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SavePublicSaleRet(int ret) 
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISavePublicSaleBackObjectImpl SavePublicSaleRet Failed ret:%d\n", ret);
		}
	}
};

class RMIInitPublicSaleBackObjectImpl : public RMIInitPublicSaleBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitPublicSaleRet(int ret, const PublicSaleParam &publicsale_param);
};

class RMISaveMailBackObjectImpl : public RMISaveMailBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveMailsRet(int ret) 
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveMailBackObjectImpl SaveMailsRet Failed ret:%d\n", ret);
		}
	}
};

class RMIInitMailsBackObjectImpl : public RMIInitMailsBackObject
{
public:
	UserID user_id;
	
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitMailsRet(int ret, const MailListParam &mail_param);
};

class RMIGetSystemMailsBackObjectImpl : public RMIGetSystemMailBackObject
{
public:
	UserID user_id;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void GetSystemMailsRet(int ret, const MailListParam &mail_param);
};

class RMIRemoveSystemMailsBackObjectImpl : public RMIRemoveSystemMailBackObject
{
public:
	UserID user_id;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void RemoveSystemMailsRet(int ret) 
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMIRemoveSystemMailsBackObjectImpl RemoveSystemMailsRet Failed ret:%d\n", ret);
		}
	}
};

class RMIGetPublicNoticeBackObjectImpl : public RMIGetPublicNoticeBackObject
{
public:

protected:
	virtual void GetPublicNoticeRet(int ret, const PublicNoticeList &notice_list);
};

class RMIInitPersonRankBackObjectImpl : public RMIInitPersonRankBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitPersonRankRet(int ret, const PersonRankParam &person_rank_param);
};

class RMISavePersonRankBackObjectImpl : public RMISavePersonRankBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SavePersonRankRet(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISavePersonRankBackObjectImpl SavePersonRankRet Failed ret:%d\n", ret);
		}
	}
};

class RMIInitTuodanBackObjectImpl : public RMIInitTuodanBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitTuodanRet(int ret, const TuodanParam &tuodan_param);
};

class RMISaveTuodanBackObjectImpl : public RMISaveTuodanBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveTuodanRet(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveTuodanBackObjectImpl SaveTuodanRet Failed ret:%d\n", ret);
		}
	}
};

class RMIInitTeamRankBackObjectImpl : public RMIInitTeamRankBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitTeamRankRet(int ret, const TeamRankParam &team_rank_param);
};

class RMISaveTeamRankBackObjectImpl : public RMISaveTeamRankBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveTeamRankRet(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveTeamRankBackObjectImpl SaveTeamRankRet Failed ret:%d\n", ret);
		}
	}
};


class RMIInitWorldStatusBackObjectImpl : public RMIInitWorldStatusBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitWorldStatusRet(int ret, const WorldStatusParam &worldstatus_param);
};

class RMISaveWorldStatusBackObjectImpl : public RMISaveWorldStatusBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveWorldStatusRet(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveWorldStateBackObjectImpl SaveWorldStateRet ret:%d \n", ret);
		}
	}
};

class RMIInitOfflineRegisterBackObjectImpl : public RMIInitOfflineRegisterBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitOfflineRegisterRet(int ret, const OfflineRegisterParam &offline_reg_param);
};

class RMISaveOfflineRegisterBackObjectImpl : public RMISaveOfflineRegisterBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveOfflineRegisterRet(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveOfflineRegisterBackObjectImpl SaveOfflineRegisterRet ret:%d \n", ret);
		}
	}
};


class RMIInitSpaceRecordBackObjectImpl : public RMIInitSpaceRecordBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitSpaceRecordRet(int ret, const SpaceParam &space_param);
};

class RMISaveSpaceRecordBackObjectImpl : public RMISaveSpaceRecordBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveSpaceRecordRet(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveSpaceRecordBackObjectImpl SaveSpaceRecordRet ret:%d \n", ret);
		}
	}
};

class RMIGetCmdBackObjectImpl : public RMIGetCmdBackObject
{
public:

protected:
	virtual void GetCmdRet(int ret, const CmdList &cmd_list);
};

class RMIInitChallengeUserObjectImpl : public RMIInitChallengeUserBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

	ChallengeField *challenge_field; 

protected:
	virtual void InitChallengeUserRet(int ret, const ChallengeUserListParam &challenge_user_param);
};

class RMISaveChallengeUserObjectImpl : public RMISaveChallengeUserBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveChallengeUserRet(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveChallengeUserObjectImpl SaveChallengeUserRet ret:%d \n", ret);
		}
	}
};

class RMIInitFishPoolObjectImpl : public RMIInitFishPoolBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

	FishPoolManager *fish_pool_manager;

protected:
	virtual void InitFishPoolRet(int ret, const FishPoolListParam &fish_pool_list_param);
};

class RMISaveFishPoolObjectImpl : public RMISaveFishPoolBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveFishPoolRet(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveFishPoolObjectImpl SaveFishPoolRet ret:%d \n", ret);
		}
	}
};

class RMIInitCrossRecordObjectImpl : public RMIInitCrossRecordBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitCrossRecordRet(int ret, const CrossRecordListParam &crossrecord_list_param);
};

class RMISaveCrossRecordObjectImpl : public RMISaveCrossRecordBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveCrossRecordRet(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveCrossRecordObjectImpl SaveCrossRecordRet ret:%d \n", ret);
		}
	}
};

class RMISaveGlobalRoleObjectImpl : public RMISaveGlobalRoleBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveGlobalRoleRet(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveGlobalRoleObjectImpl SaveGlobalRoleRet ret:%d \n", ret);
		}
	}
};

class RMIInitWorldStatus2BackObjectImpl : public RMIInitWorldStatus2BackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitWorldStatus2Ret(int ret, const WorldStatus2Param &worldstatus2_param);
};

class RMISaveWorldStatus2BackObjectImpl : public RMISaveWorldStatus2BackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveWorldStatus2Ret(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveWorldState2BackObjectImpl SaveWorldState2Ret ret:%d \n", ret);
		}
	}
};

class RMIInitWorldStatus3BackObjectImpl : public RMIInitWorldStatus3BackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitWorldStatus3Ret(int ret, const WorldStatus3Param &worldstatus3_param);
};

class RMISaveWorldStatus3BackObjectImpl : public RMISaveWorldStatus3BackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveWorldStatus3Ret(int ret)
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISaveWorldState3BackObjectImpl SaveWorldState3Ret ret:%d \n", ret);
		}
	}
};

class RMISwapRoleBackObjectImpl : public RMISwapRoleBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

	int role_id1 = 0;
	int role_id2 = 0;
protected:
	virtual void SwapRoleRet(int ret) override
	{
		if (0 != ret)
		{
			gamelog::g_log_world.printf(LL_ERROR, "RMISwapRoleBackObjectImpl SwapRoleRet role_id1: %d role_id2 %d ret:%d \n",role_id1, role_id2, ret);
		}
	}
};
#endif  // _GL_RMIBACKOBJDEF_H_



