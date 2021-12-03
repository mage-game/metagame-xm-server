#ifndef __GUILD_AUTHORITY_HPP__
#define __GUILD_AUTHORITY_HPP__

#include "servercommon/guilddef.hpp"

class GuildMember;

class GuildAuthority
{
public:
	enum AUTHORITY
	{
		AUTHORITY_INVALID = 0,
		AUTHORITY_VERIFY,							// 审批加入
		AUTHORITY_APPOINT,							// 任免职位
		AUTHORITY_KICKOUT,							// 踢人
		AUTHORITY_LEAVEPOST,						// 卸任
		AUTHORITY_QUIT,								// 退出
		AUTHORITY_DISSMISS,							// 解散
		AUTHORITY_NOTICE,							// 修改军团公告
		AUTHORITY_MAILALL,							// 群发邮件
		AUTHORITY_SETUP,							// 设置相关 权限
		AUTHORITY_BACKHOME,							// 快速回营
		AUTHORITY_UNION,							// 联盟相关
		AUTHORITY_DELATE,							// 弹劾
		AUTHORITY_INVITE,							// 邀请
		AUTHORITY_CALLBOSS,							// 召唤BOSS
		AUTHORITY_TOWER_DEFEND,						// 仙盟塔防
		AUTHORITY_GUILD_UPLEVEL,					// 仙盟升级
		AUTHORITY_STORGE,							// 仙盟仓库销毁物品
		AUTHORITY_EXTEND_MEMBER,					// 扩展仙盟成员上限
		AUTHORITY_SET_AUTO_CLEAR,					// 设置自动清理公会成员
		AUTHORITY_CHANGE_AVATAR,					// 改变公会头像
		AUTHORITY_BIAOCHE_START,					// 开始镖车
		AUTHORITY_TIANCITONGBI,						// 天赐铜币

		AUTHORITY_MAX,
	};

	static bool CanVerify(GuildMember *member);
	static bool CanAppoint(GuildMember *appoint_member, GuildMember *beappoint_member, int beappoint_pos);
	static bool CanKickMember(GuildMember *kicker_member, GuildMember *bekicker_member);
	static bool CanLeavePost(GuildMember *member);
	static bool CanQuit(GuildMember *member);
	static bool CanDismiss(GuildMember *member);	
	static bool CanChangeNotice(GuildMember *member);
	static bool CanMailAll(GuildMember *member);
	static bool CanSetup(GuildMember *member);
	static bool CanBackHome(GuildMember *member);
	static bool CanUnion(GuildMember *member);
	static bool CanDelate(GuildMember *member);
	static bool CanInvite(GuildMember *member);
	static bool CanCallBoss(GuildMember *member);
	static bool CanOpenTowerDefend(GuildMember *member);
	static bool CanGuildUpLevel(GuildMember *member);
	static bool CanDiscardStorgeItem(GuildMember *member);
	static bool CanExtendMember(GuildMember *member);
	static bool CanUseStorage(GuildMember *member);
	static bool CanSetAutoClear(GuildMember *member);
	static bool CanChangeAvatar(GuildMember *member);
	static bool CanBiaoCheStart(GuildMember *member);
	static bool CanOpenTianCiTongBi(GuildMember * member);

private:
	GuildAuthority();
	~GuildAuthority();

	static bool ValidPost(char post);
	static int ComparePost(char post1, char post2);

	static bool m_autority[GUILD_POST_MAX][AUTHORITY_MAX];
};

#endif // __GUILD_AUTHORITY_HPP__

