/******************************************************************************************
FileName: rmiglobal.hpp
Author: dreamwang
Description: GLOBAL库的处理  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __RMI_GLOBAL_HPP__
#define __RMI_GLOBAL_HPP__

#include "rmi/rmibase.h"

#include "servercommon/struct/global/campparam.hpp"
#include "servercommon/struct/global/guildparam.hpp"
#include "servercommon/struct/global/tuodanparam.hpp"
#include "servercommon/struct/global/publicsaleparam.hpp"
#include "servercommon/struct/global/wantbuyparam.hpp"
#include "servercommon/struct/global/maillistparam.hpp"
#include "servercommon/struct/global/rankparam.hpp"
#include "servercommon/struct/global/worldstatusparam.hpp"
#include "servercommon/struct/global/offlineregisterparam.hpp"
#include "servercommon/struct/global/challengeuserparam.hpp"
#include "servercommon/struct/global/fishpoolparam.hpp"
#include "servercommon/struct/global/crossrecordlistparam.hpp"
#include "servercommon/struct/global/spaceparam.hpp"
#include "servercommon/struct/global/globalroledataparam.hpp"
#include "servercommon/struct/global/worldstatus2param.hpp"
#include "servercommon/struct/global/worldstatus3param.hpp"

struct FightingSailingDataParam;
struct FightingMiningDataParam;
class IStatement;
class DataNode;

// 加表记得在删除军团的时候也要删掉 请注意！！！！ 

class RMIGlobalObject : public rmi::RMIObject
{
public:
	RMIGlobalObject();
	virtual ~RMIGlobalObject();

	virtual void __free();
	virtual const char * __get_obj_id();
	
	static int __InitCamp(TLVUnserializer &in_param, TLVSerializer *out_param); 
	static int InitCamp(CampParam *camp_param);

	static int __SaveCamp(TLVUnserializer &in_param, TLVSerializer *out_param); 
	static int SaveCamp(const CampParam &param);

	static int __CreateGuild(TLVUnserializer &in_param, TLVSerializer *out_param); 
	static int CreateGuild(int create_db_index, const GuildBaseParam &base_param, GuildID *guild_id);

	static int __DestroyGuild(TLVUnserializer &in_param, TLVSerializer *out_param); 
	static int DestoryGuild(GuildID guild_id);

	static int __InitGuild(TLVUnserializer &in_param, TLVSerializer *out_param); 
	static int InitGuild(GuildID guild_id_from, AllGuildParam *guild_list);

	static int __SaveGuild(TLVUnserializer &in_param, TLVSerializer *out_param); 
	static int SaveGuild(GuildID guild_id, const GuildBaseParam &base_param, const GuildApplyForParam &apply_for_param,
		const GuildMemberParam &member_param, const GuildEventParam &event_param); 

	static int __SavePublicSale(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SavePublicSale(const PublicSaleParam & publicsale_param);

	static int __InitPublicSale(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitPublicSale(long long id_from, PublicSaleParam *publicsale_param);

	static int __InitRoleMails(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitRoleMails(int uid,  MailListParam *mail_list);

	static int __SaveRoleMails(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveRoleMails(int uid, const MailListParam &mail_list);

	static int __GetRoleSystemMails(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int GetRoleSystemMails(int uid,  MailListParam *mail_list);

	static int __RemoveRoleSystemMails(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int RemoveRoleSystemMail(long long systemmail_id);


	static int __InitPersonRank(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitPersonRank(PersonRankParam *person_rank_param);

	static int __SavePersonRank(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SavePersonRank(const PersonRankParam &person_rank_param);

	static int __InitTuodan(TLVUnserializer &in_param, TLVSerializer *out_param); 
	static int InitTuodan(int tuodan_id_from, TuodanParam *tuodan_param);

	static int __SaveTuodan(TLVUnserializer &in_param, TLVSerializer *out_param); 
	static int SaveTuodan(const TuodanParam &tuodan_param); 

	static int __InitTeamRank(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitTeamRank(TeamRankParam *team_rank_param);

	static int __SaveTeamRank(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveTeamRank(const TeamRankParam &team_rank_param);
	
	static int __InitWorldStatus(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitWorldStatus(long long id_from, WorldStatusParam *worldstatus_param);

	static int __SaveWorldStatus(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveWorldStatus(const WorldStatusParam &worldstatus_param);

	static int __InitOfflineRegister(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitOfflineRegister(long long id_from, OfflineRegisterParam *offline_reg_param);

	static int __SaveOfflineRegister(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveOfflineRegister(const OfflineRegisterParam & offline_reg_param);

	static int __InitSpaceRecord(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitSpaceRecord(long long id_from, SpaceParam *space_param);

	static int __SaveSpaceRecord(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveSpaceRecord(const SpaceParam & space_param);

	static int __InitChallengeUser(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitChallengeUser(int rank_from, ChallengeUserListParam *challenge_user_param);

	static int __SaveChallengeUser(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveChallengeUser(const ChallengeUserListParam &challenge_user_param);

	static int __InitFishPool(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitFishPool(long long from_id, FishPoolListParam *challenge_user_param);

	static int __SaveFishPool(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveFishPool(const FishPoolListParam &challenge_user_param);

	static int __InitCrossRecord(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitCrossRecord(long long id_from, CrossRecordListParam *crossrecord_param);

	static int __SaveCrossRecord(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveCrossRecord(const CrossRecordListParam &crossrecord_param);

	static int __InitGlobalRole(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitGlobalRole(long long id_from, GlobalRoleDatalListParam *global_role_list_param);

	static int __SaveGlobalRole(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveGlobalRole(const GlobalRoleDatalListParam &global_role_list_param);

	static int __InitWorldStatus2(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitWorldStatus2(long long id_from, WorldStatus2Param *worldstatus2_param);

	static int __SaveWorldStatus2(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveWorldStatus2(const WorldStatus2Param &worldstatus2_param);

	static int __InitWorldStatus3(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitWorldStatus3(long long id_from, WorldStatus3Param *worldstatus3_param);

	static int __SaveWorldStatus3(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveWorldStatus3(const WorldStatus3Param &worldstatus3_param);

private:
	static bool ItemParamInit(ItemParamDataStruct *param, const char *vcharp, int length);
	static void ItemParamUpdate(DataNode *data_node, bool has_param, const ItemParamDataStruct *param);
	static void ItemParamInsert(char *vcharp, int length, const ItemParamDataStruct *param);
};

#endif  // __RMI_GLOBAL_HPP__

