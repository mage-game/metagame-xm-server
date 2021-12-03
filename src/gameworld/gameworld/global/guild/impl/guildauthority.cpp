#include "guildauthority.hpp"
#include "guildmember.hpp"
#include "global/guild/guild.hpp"

bool GuildAuthority::m_autority[GUILD_POST_MAX][AUTHORITY_MAX] = 
{						/*无效* *审批*	*任命*	*踢人*	*卸任*	*退出*  *解散* *修改公告*  *群发邮件*	 *设置*  *回城*  *联盟*   *弹劾*  *邀请*  *召唤BOSS*	*仙盟塔防*  *仙盟升级*  *仙盟仓库* *扩展成员*  *自动清理 *更改头像 *开启运镖 *天赐铜币  */
	/* 无效	   */ {	false,	false,	false,	false,	false,	false,	false,	false,		false,	  false,  false,  false,  false,  false,   false,	 false, 	false,		false,		false,		false,		false,    false,    false },
	/* 士卒     */  {	false,	false,	false,	false,	false,	true,	false,	false,		false,	  false,  true,	  false,  true,	  false,   false,	 false, 	false,		false,		false,		false,	false,    false,    false },
	/* 长老     */  {	false,	true,	false,	false,	true,	true,	false,	true,		true,	  false,  true,	  false,  true,	  true,	   false,	 false,	    false,		false,		false,		false,	false,	  false,    false },
	/* 副团长   */  { false,  true,   true,   true,   true,   true,   false,  true,	   true,	 true,	 true,	 true,	 true,	 true,	  true,		true,	   true,	   true,	   true,	   false,		true,    true,      true },
	/* 团长	   */ {	false,	true,	true,	true,	false,	false,	true,	true,		true,	  true,	  true,	  true,	  false,  true,	   true,	 true,		true,		true,		true,		true,		true,    true,      true },
	/* 精英     */  {	false,	false,	false,	false,	false,	true,	false,	false,		false,	  false,  true,	  false,  true,	  false,   false,	 false,		false,		false,		false,		false,	false,    false,    false },
	/* 护法     */  {	false,	false,	false,	false,	false,	true,	false,	false,		false,	  false,  true,	  false,  true,	  false,   false,	 false,		false,		false,		false,		false,	false,    false,    false },
};

bool GuildAuthority::CanVerify(GuildMember *member)
{
	if (NULL == member)
	{
		return false;
	}

	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_VERIFY];
}

bool GuildAuthority::CanAppoint(GuildMember *appoint_member, GuildMember *beappoint_member, int beappoint_pos)
{
	if (!ValidPost(appoint_member->GetPost()) || !ValidPost(beappoint_member->GetPost()) || !ValidPost(beappoint_pos)) return false;

	if (GUILD_POST_TUANGZHANG == appoint_member->GetPost()) return (appoint_member != beappoint_member);	// 任免者是团长

	if (ComparePost(appoint_member->GetPost(), beappoint_member->GetPost()) > 0 && 
		ComparePost(appoint_member->GetPost(), beappoint_pos) > 0) 
	{
		return true;
	}

	return false;
}

bool GuildAuthority::CanKickMember(GuildMember *kicker_member, GuildMember *bekicker_member)
{
	if (kicker_member == bekicker_member || !ValidPost(kicker_member->GetPost()) || !ValidPost(bekicker_member->GetPost())) return false;

	if (GUILD_POST_TUANGZHANG == kicker_member->GetPost()) return true;

	if (ComparePost(kicker_member->GetPost(), bekicker_member->GetPost()) > 0) return true;

	return false;
}

bool GuildAuthority::CanLeavePost(GuildMember *member)
{
	if (GUILD_POST_TUANGZHANG == member->GetPost()) return false;

	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_LEAVEPOST];
}

bool GuildAuthority::CanQuit(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_QUIT];
}

bool GuildAuthority::CanDismiss(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_DISSMISS];
}

bool GuildAuthority::CanChangeNotice(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_NOTICE];
}

bool GuildAuthority::CanMailAll(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_MAILALL];
}

bool GuildAuthority::CanSetup(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_SETUP];
}

bool GuildAuthority::CanUnion(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_UNION];
}

bool GuildAuthority::CanBackHome(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_BACKHOME];
}

bool GuildAuthority::CanDelate(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_DELATE];
}

bool GuildAuthority::CanInvite(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_INVITE];
}

bool GuildAuthority::CanUseStorage(GuildMember *member)
{
	return ValidPost(member->GetPost());
}

bool GuildAuthority::CanSetAutoClear(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_SET_AUTO_CLEAR];
}

bool GuildAuthority::CanCallBoss(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_CALLBOSS];
}

bool GuildAuthority::CanOpenTowerDefend(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_TOWER_DEFEND];
}

bool GuildAuthority::CanGuildUpLevel(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_GUILD_UPLEVEL];
}

bool GuildAuthority::CanDiscardStorgeItem(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_STORGE];
}

bool GuildAuthority::CanExtendMember(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_EXTEND_MEMBER];
}

bool GuildAuthority::CanChangeAvatar(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_CHANGE_AVATAR];
}

bool GuildAuthority::CanBiaoCheStart(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_BIAOCHE_START];
}

bool GuildAuthority::CanOpenTianCiTongBi(GuildMember *member)
{
	return ValidPost(member->GetPost()) && m_autority[(int)member->GetPost()][AUTHORITY_TIANCITONGBI];
}

bool GuildAuthority::ValidPost(char post)
{
	return post > GUILD_POST_INVALID && post < GUILD_POST_MAX;
}

int GuildAuthority::ComparePost(char post1, char post2)
{
	int compare_value1 = 0;
	if (GUILD_POST_CHENG_YUAN == post1) compare_value1 = 1;
	else if (GUILD_POST_JINGYING == post1) compare_value1 = 2;
	else if (GUILD_POST_ZHANG_LAO == post1) compare_value1 = 3;
	else if (GUILD_POST_FU_TUANGZHANG == post1) compare_value1 = 4;
	else if (GUILD_POST_TUANGZHANG == post1) compare_value1 = 5;

	int compare_value2 = 0;
	if (GUILD_POST_CHENG_YUAN == post2) compare_value2 = 1;
	else if (GUILD_POST_JINGYING == post2) compare_value2 = 2;
	else if (GUILD_POST_ZHANG_LAO == post2) compare_value2 = 3;
	else if (GUILD_POST_FU_TUANGZHANG == post2) compare_value2 = 4;
	else if (GUILD_POST_TUANGZHANG == post2) compare_value2 = 5;

	return (compare_value1 == compare_value2 ? 0 : (compare_value1 > compare_value2 ? 1 : -1));
}

