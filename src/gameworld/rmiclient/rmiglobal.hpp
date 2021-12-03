/******************************************************************************************
FileName: rmiglobal.hpp
Author: dreamwang
Description: 线级 数据库 （比如军团 阵营） 回调相关定义 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __RMI_GLOBAL_HPP__
#define __RMI_GLOBAL_HPP__

#include "rmi/rmibase.h"
#include "servercommon/serverdef.h"
#include "servercommon/struct/global/guildparam.hpp"

class GlobalRoleDatalListParam;
class CrossRecordListParam;
class FishPoolListParam;
class ChallengeUserListParam;
class SpaceParam;
class OfflineRegisterParam;
class WorldStatusParam;
class TeamRankParam;
class TuodanParam;
class PersonRankParam;
class MailListParam;
class PublicSaleParam;
class GuildBaseParam;
class CampParam;
struct FightingSailingDataParam;
struct FightingMiningDataParam;
class Fighting;
class WorldStatus2Param;
class WorldStatus3Param;

class RMIGlobalClient : public rmi::RMIProxyObject
{
	friend class RMIInitWantBuyBackObjectImpl;

public:
	RMIGlobalClient() {}
	virtual ~RMIGlobalClient() {}
	
	bool InitCampAsyn(rmi::RMIBackObject *backobj);
	bool SaveCampAsyn(const CampParam &camp_param, rmi::RMIBackObject *backobj);

	bool CreateGuildAsyn(int create_db_index, const GuildBaseParam &guild_base, rmi::RMIBackObject *backobj);
	bool DismissGuildAsyn(GuildID guild_id, rmi::RMIBackObject *backobj);
	bool InitGuildAsyn(GuildID guild_id_from, rmi::RMIBackObject *backobj);
	bool SaveGuildAsyn(GuildID guild_id, AllGuildParam::GuildParam &guild_param, rmi::RMIBackObject *backobj);

	bool InitPublicSaleAsyn(long long id_from, rmi::RMIBackObject *backobj);
	bool SavePublicSaleAsyn(const PublicSaleParam &publicsale_param, rmi::RMIBackObject *backobj);

	bool InitMailsAsyn(int uid, rmi::RMIBackObject *backobj);
	bool SaveMailsAsyn(int uid, const MailListParam &mail, rmi::RMIBackObject *backobj);
	bool GetSystemMailsAsyn(int uid, rmi::RMIBackObject *backobj);
	bool RemoveSystemMailsAsyn(int uid, const MailListParam &mail_param, rmi::RMIBackObject *backobj);
	
	bool InitPersonRankAsyn(rmi::RMIBackObject *backobj);
	bool SavePersonRankAsyn(const PersonRankParam &person_rank_param, rmi::RMIBackObject *backobj);

	bool InitTuodanAsyn(int tuodanid_from, rmi::RMIBackObject *backobj);
	bool SaveTuodanAsyn(const TuodanParam &tuodan_param, rmi::RMIBackObject *backobj);

	bool InitTeamRankAsyn(rmi::RMIBackObject *backobj);
	bool SaveTeamRankAsyn(const TeamRankParam &team_rank_param, rmi::RMIBackObject *backobj);

	bool InitWorldStatusAsyn(long long id_from, rmi::RMIBackObject *backobj);
	bool SaveWorldStatusAsyn(const WorldStatusParam &worldstatus_param, rmi::RMIBackObject *backobj);

	bool InitOfflineRegisterAsyn(long long id_from, rmi::RMIBackObject *backobj);
	bool SaveOfflineRegisterAsyn(const OfflineRegisterParam &offline_reg_param, rmi::RMIBackObject *backobj);

	bool InitSpaceRecordAsyn(long long id_from, rmi::RMIBackObject *backobj);
	bool SaveSpaceRecordAsyn(const SpaceParam &space_param, rmi::RMIBackObject *backobj);

	bool InitChallengeUserAsyn(int rank_from, rmi::RMIBackObject *backobj);
	bool SaveChallengeUserAsyn(const ChallengeUserListParam &challenge_user_param, rmi::RMIBackObject *backobj);

	bool InitFishPoolAsyn(long long id_from,  rmi::RMIBackObject *backobj);
	bool SaveFishPoolAsyn(const FishPoolListParam &fish_pool_list_param, rmi::RMIBackObject *backobj);

	bool InitCrossRecordAsyn(long long id_from, rmi::RMIBackObject *backobj);
	bool SaveCrossRecordAsyn(const CrossRecordListParam &cross_record_param, rmi::RMIBackObject *backobj);

	bool InitGlobalRoleAsyn(long long id_from, rmi::RMIBackObject *backobj);
	bool SaveGlobalRoleAsyn(const GlobalRoleDatalListParam &global_role_list_param, rmi::RMIBackObject *backobj);

	bool InitWorldStatus2Asyn(long long id_from, rmi::RMIBackObject *backobj);
	bool SaveWorldStatus2Asyn(const WorldStatus2Param &worldstatus2_param, rmi::RMIBackObject *backobj);

	bool InitWorldStatus3Asyn(long long id_from, rmi::RMIBackObject *backobj);
	bool SaveWorldStatus3Asyn(const WorldStatus3Param &worldstatus3_param, rmi::RMIBackObject *backobj);
};

class RMIInitCampBackObject : public rmi::RMIBackObject
{
public:
	RMIInitCampBackObject(){};
	virtual ~RMIInitCampBackObject(){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitCampRet(int ret, const CampParam &camp_param);
};

class RMISaveCampBackObject : public rmi::RMIBackObject
{
public:
	RMISaveCampBackObject(){};
	virtual ~RMISaveCampBackObject(){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();

protected:
	virtual void SaveCampRet(int ret);
};

class RMICreateGuildBackObject : public rmi::RMIBackObject
{
public:
	RMICreateGuildBackObject(){};
	virtual ~RMICreateGuildBackObject(){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();

protected:
	virtual void CreateGuildRet(int ret, GuildID guild_id);
};

class RMIDismissGuildBackObject : public rmi::RMIBackObject
{
public:
	RMIDismissGuildBackObject(){};
	virtual ~RMIDismissGuildBackObject(){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();

protected:
	virtual void DismissGuildRet(int ret);
};

class RMIInitGuildBackObject : public rmi::RMIBackObject
{
public:
	RMIInitGuildBackObject(){};
	virtual ~RMIInitGuildBackObject(){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitGuildRet(int ret, const AllGuildParam &all_guild_list);
};

class RMISaveGuildBackObject : public rmi::RMIBackObject
{
public:
	RMISaveGuildBackObject() {}
	virtual ~RMISaveGuildBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveGuildRet(int ret);
};

class RMIInitPublicSaleBackObject : public rmi::RMIBackObject
{
public:
	RMIInitPublicSaleBackObject(){};
	virtual ~RMIInitPublicSaleBackObject(){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitPublicSaleRet(int ret, const PublicSaleParam &publicsale_param);
};

class RMISavePublicSaleBackObject : public rmi::RMIBackObject
{
public:
	RMISavePublicSaleBackObject(){};
	virtual ~RMISavePublicSaleBackObject(){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SavePublicSaleRet(int ret);
};

//class RMIInitWantBuyBackObject : public rmi::RMIBackObject
//{
//public:
//	RMIInitWantBuyBackObject(){};
//	virtual ~RMIInitWantBuyBackObject(){};
//
//	virtual bool __response(TLVUnserializer &in_param);
//	virtual void __exception(int error);
//	virtual void __timeout();
//	virtual void __free();
//
//protected:
//	virtual void InitWantBuyRet(int ret, const WantBuyParam &wantbuy_param);
//};
//
//class RMISaveWantBuyBackObject : public rmi::RMIBackObject
//{
//public:
//	RMISaveWantBuyBackObject(){};
//	virtual ~RMISaveWantBuyBackObject(){};
//
//	virtual bool __response(TLVUnserializer &in_param);
//	virtual void __exception(int error);
//	virtual void __timeout();
//	virtual void __free();
//
//protected:
//	virtual void SaveWantBuyRet(int ret);
//};
//
class RMISaveMailBackObject : public rmi::RMIBackObject
{
public:
	RMISaveMailBackObject() {};
	virtual ~RMISaveMailBackObject() {};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error) {};
	virtual void __timeout() {};
	virtual void __free();

protected:
	virtual void SaveMailsRet(int ret);
};

class RMIInitMailsBackObject : public rmi::RMIBackObject
{
public:
	RMIInitMailsBackObject() {};
	virtual ~RMIInitMailsBackObject() {};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error) {};
	virtual void __timeout() {};
	virtual void __free();

protected:
	virtual void InitMailsRet(int ret, const MailListParam &mail_param);
};

class RMIGetSystemMailBackObject : public rmi::RMIBackObject
{
public:
	RMIGetSystemMailBackObject() {};
	virtual ~RMIGetSystemMailBackObject() {};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error) {};
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void GetSystemMailsRet(int ret, const MailListParam &mail_param);
};

class RMIRemoveSystemMailBackObject : public rmi::RMIBackObject
{
public:
	RMIRemoveSystemMailBackObject() {};
	virtual ~RMIRemoveSystemMailBackObject() {};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error) {};
	virtual void __timeout() {};
	virtual void __free();

protected:
	virtual void RemoveSystemMailsRet(int ret);
};

class RMIInitPersonRankBackObject : public rmi::RMIBackObject
{
public:
	RMIInitPersonRankBackObject (){};
	virtual ~RMIInitPersonRankBackObject (){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitPersonRankRet(int ret, const PersonRankParam &person_rank_param);
};

class RMISavePersonRankBackObject : public rmi::RMIBackObject
{
public:
	RMISavePersonRankBackObject (){};
	virtual ~RMISavePersonRankBackObject (){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SavePersonRankRet(int ret);
};

class RMIInitTuodanBackObject : public rmi::RMIBackObject
{
public:
	RMIInitTuodanBackObject (){};
	virtual ~RMIInitTuodanBackObject (){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitTuodanRet(int ret, const TuodanParam &tuodan_param);
};

class RMISaveTuodanBackObject : public rmi::RMIBackObject
{
public:
	RMISaveTuodanBackObject (){};
	virtual ~RMISaveTuodanBackObject (){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveTuodanRet(int ret);
};

class RMIInitTeamRankBackObject : public rmi::RMIBackObject
{
public:
	RMIInitTeamRankBackObject (){};
	virtual ~RMIInitTeamRankBackObject (){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitTeamRankRet(int ret, const TeamRankParam &team_rank_param);
};

class RMISaveTeamRankBackObject : public rmi::RMIBackObject
{
public:
	RMISaveTeamRankBackObject (){};
	virtual ~RMISaveTeamRankBackObject (){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveTeamRankRet(int ret);
};

class RMIInitWorldStatusBackObject : public rmi::RMIBackObject
{
public:
	RMIInitWorldStatusBackObject() {}
	virtual ~RMIInitWorldStatusBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitWorldStatusRet(int ret, const WorldStatusParam &status_param);
	void InitError();
};

class RMISaveWorldStatusBackObject : public rmi::RMIBackObject
{
public:
	RMISaveWorldStatusBackObject() {}
	virtual ~RMISaveWorldStatusBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveWorldStatusRet(int ret);
};

class RMIInitOfflineRegisterBackObject : public rmi::RMIBackObject
{
public:
	RMIInitOfflineRegisterBackObject() {}
	virtual ~RMIInitOfflineRegisterBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitOfflineRegisterRet(int ret, const OfflineRegisterParam &offline_reg_param);
	void InitError();
};

class RMISaveOfflineRegisterBackObject : public rmi::RMIBackObject
{
public:
	RMISaveOfflineRegisterBackObject() {}
	virtual ~RMISaveOfflineRegisterBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveOfflineRegisterRet(int ret);
};

class RMIInitSpaceRecordBackObject : public rmi::RMIBackObject
{
public:
	RMIInitSpaceRecordBackObject() {}
	virtual ~RMIInitSpaceRecordBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitSpaceRecordRet(int ret, const SpaceParam &space_param);
	void InitError();
};

class RMISaveSpaceRecordBackObject : public rmi::RMIBackObject
{
public:
	RMISaveSpaceRecordBackObject() {}
	virtual ~RMISaveSpaceRecordBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveSpaceRecordRet(int ret);
};

class RMIInitChallengeUserBackObject : public rmi::RMIBackObject
{
public:
	RMIInitChallengeUserBackObject() {}
	virtual ~RMIInitChallengeUserBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitChallengeUserRet(int ret, const ChallengeUserListParam &challenge_user_param);
	void InitError();
};

class RMISaveChallengeUserBackObject : public rmi::RMIBackObject
{
public:
	RMISaveChallengeUserBackObject() {}
	virtual ~RMISaveChallengeUserBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveChallengeUserRet(int ret);
};


class RMIInitFishPoolBackObject : public rmi::RMIBackObject
{
public:
	RMIInitFishPoolBackObject() {}
	virtual ~RMIInitFishPoolBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitFishPoolRet(int ret, const FishPoolListParam &fish_pool_list_param);
	void InitError();
};

class RMISaveFishPoolBackObject : public rmi::RMIBackObject
{
public:
	RMISaveFishPoolBackObject() {}
	virtual ~RMISaveFishPoolBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveFishPoolRet(int ret);
};

class RMIInitCrossRecordBackObject : public rmi::RMIBackObject
{
public:
	RMIInitCrossRecordBackObject() {}
	virtual ~RMIInitCrossRecordBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitCrossRecordRet(int ret, const CrossRecordListParam &crossrecord_list_param);
	void InitError();
};

class RMISaveCrossRecordBackObject : public rmi::RMIBackObject
{
public:
	RMISaveCrossRecordBackObject() {}
	virtual ~RMISaveCrossRecordBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveCrossRecordRet(int ret);
};

class RMISaveGlobalRoleBackObject : public rmi::RMIBackObject
{
public:
	RMISaveGlobalRoleBackObject() {}
	virtual ~RMISaveGlobalRoleBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveGlobalRoleRet(int ret);
};

class RMIInitGlobalRoleBackObject : public rmi::RMIBackObject
{
public:
	RMIInitGlobalRoleBackObject() {}
	virtual ~RMIInitGlobalRoleBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitGlobalRet(int ret, const GlobalRoleDatalListParam &fish_pool_list_param);
};

class RMIInitWorldStatus2BackObject : public rmi::RMIBackObject
{
public:
	RMIInitWorldStatus2BackObject() {}
	virtual ~RMIInitWorldStatus2BackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitWorldStatus2Ret(int ret, const WorldStatus2Param &status2_param);
	void InitError();
};

class RMISaveWorldStatus2BackObject : public rmi::RMIBackObject
{
public:
	RMISaveWorldStatus2BackObject() {}
	virtual ~RMISaveWorldStatus2BackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveWorldStatus2Ret(int ret);
};

class RMIInitWorldStatus3BackObject : public rmi::RMIBackObject
{
public:
	RMIInitWorldStatus3BackObject() {}
	virtual ~RMIInitWorldStatus3BackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitWorldStatus3Ret(int ret, const WorldStatus3Param &status2_param);
	void InitError();
};

class RMISaveWorldStatus3BackObject : public rmi::RMIBackObject
{
public:
	RMISaveWorldStatus3BackObject() {}
	virtual ~RMISaveWorldStatus3BackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveWorldStatus3Ret(int ret);
};
#endif  // __RMI_GLOBAL_HPP__


