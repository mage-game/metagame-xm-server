/******************************************************************************************
FileName: guildroute.hpp
Author: shell
Description: 军团路由
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
shell	    2011/08/11/ 18:04:05     1.0           创建
*****************************************************************************************/

#ifndef __GUILDROUTE_HPP__
#define __GUILDROUTE_HPP__

#include <map>

#include "servercommon/serverdef.h"
#include "protocal/msgguild.hpp"

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/internalprotocal/ggprotocal.h"

class Guild;
class Role;

class GuildRoute
{
public:
	static GuildRoute & Instance();

	void CreateGuildCS(Role *role, Protocol::CSCreateGuild *cg);
	void DismissGuildCS(Role *role, Protocol::CSDismissGuild *dg);
	void ApplyForJoinGuildCS(Role *role, Protocol::CSApplyForJoinGuild *afjg);
	void ApplyForJoinGuildAckCS(Role *role, Protocol::CSApplyForJoinGuildAck *afjga);
	void QuitGuildCS(Role *role, Protocol::CSQuitGuild *qg);
	void InviteGuildCS(Role *role, Protocol::CSInviteGuild *ig);
	void InviteGuildAckCS(Role *role, Protocol::CSInviteGuildAck *iga);
	void KickoutGuildCS(Role *role, Protocol::CSKickoutGuild *kg);
	void AppointGuildCS(Role *role, Protocol::CSAppointGuild *ag);
	void LeaveGuildPostCS(Role *role, Protocol::CSLeaveGuildPost *ag);
	void GuildChangeNoticeCS(Role *role, Protocol::CSGuildChangeNotice *gsn);
	void GuildMailAllCS(Role *role, Protocol::CSGuildMailAll *gma);
	void GetGuildInfoCS(Role *role, Protocol::CSGetGuildInfo *agi);
	void GuildDelateCS(Role *role, Protocol::CSGuildDelate *gd);
	void GuildBackToStationCS(Role *role, Protocol::CSGuildBackToStation *gbts, bool notify = true);
	void GuildResetName(Role *role, Protocol::CSGuildResetName *grn);
	void GuildApplySetup(Role *role, Protocol::CSApplyforSetup *afs);
	void GuildAddGuildExp(Role *role, Protocol::CSAddGuildExp *age);
	void GuildUpLevel(Role *role, Protocol::CSGuildUpLevel *age);
	void GuildGetBuff(Role *role, Protocol::CSGuildGetBuff *age);
	void GuildExchange(Role *role, Protocol::CSGuildExchange *cmd);
	void GuildSetAutoKickoutSetup(Role *role, Protocol::CSGuildSetAutoKickoutSetup *gsaks);
	void GuildCallIn(Role *role, Protocol::CSGuildCallIn *gci);
	void OnCreateGuildFailRollback(Role *role, ggprotocal::CreateGuildFailRollback *cgfrb);

	// 仙盟试炼---------------------------------------------------------------------------
	void OnGuildFbEnterReq(Role *role);
	void OnGuildFbOpen(Guild *guild, Role *role);
	void OnGuildFbClose(Guild *guild);
	void OnGuildFbFinish(GuildID guild_id, bool is_pass);
	void OnGuildFbActivityOpen();

	// 仙盟篝火---------------------------------------------------------------------------
	void OnAddBonfireMucai(Role *role);
	void OnGotoBonfirePos(Role *role);
	void OnGuildBonfireOpen(Guild *guild, Role *role);
	void OnGuildBonfireClose(Guild *guild);

	void OnStorgePutItem(Role *role, int bag_index, int num);
	void OnStorgeTakeItem(Role *role, int storge_index, int num, ItemID item_id);
	void OnStorgeDiscardItem(Role *role, int storge_index, ItemID item_id);
	void OnReqStorgeInfo(Role *role);
	void OnStorgePutItemOneKey(Role *role, Protocol::CSGuildStorgeOneKeyOperate *cmd);
	void OnStorgeDiscardItemOneKey(Role *role, Protocol::CSGuildStorgeOneKeyOperate *cmd);
	void OnSetAutoClear(Role *role, short is_auto_clear);

private:
	void OnGetAllGuildBaseInfo(Role *role);
	void OnGetApplyForList(Role *role, GuildID guild_id);
	void OnGetApplyForJoinGuildList(Role *role);
	void OnGetMemberList(Role *role, GuildID guild_id);
	void OnGetGuildInfo(Role *role, GuildID guild_id);
	void OnGetGuildEventList(Role *role, GuildID guild_id);
	void OnGetGuildInviteList(Role *role, GuildID guild_id);
	void OnGetGuildGongziList(Role *role);

	GuildRoute() {}
	~GuildRoute() {}

	struct GuildBonfireInfo
	{
		GuildBonfireInfo() : bonfire_objid(INVALID_OBJ_ID), scene_id(0), scene_key(0), bonfire_pos(0, 0) {}

		ObjID bonfire_objid;
		int scene_id;
		int scene_key;
		Posi bonfire_pos;
	};

	typedef std::map<GuildID, GuildBonfireInfo> GuildBonfireInfoMap;
	typedef std::map<GuildID, GuildBonfireInfo>::iterator GuildBonfireInfoMapIt;
	GuildBonfireInfoMap m_bonfire_map_register;
};

#endif





