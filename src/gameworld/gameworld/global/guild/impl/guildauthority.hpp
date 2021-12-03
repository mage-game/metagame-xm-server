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
		AUTHORITY_VERIFY,							// ��������
		AUTHORITY_APPOINT,							// ����ְλ
		AUTHORITY_KICKOUT,							// ����
		AUTHORITY_LEAVEPOST,						// ж��
		AUTHORITY_QUIT,								// �˳�
		AUTHORITY_DISSMISS,							// ��ɢ
		AUTHORITY_NOTICE,							// �޸ľ��Ź���
		AUTHORITY_MAILALL,							// Ⱥ���ʼ�
		AUTHORITY_SETUP,							// ������� Ȩ��
		AUTHORITY_BACKHOME,							// ���ٻ�Ӫ
		AUTHORITY_UNION,							// �������
		AUTHORITY_DELATE,							// ����
		AUTHORITY_INVITE,							// ����
		AUTHORITY_CALLBOSS,							// �ٻ�BOSS
		AUTHORITY_TOWER_DEFEND,						// ��������
		AUTHORITY_GUILD_UPLEVEL,					// ��������
		AUTHORITY_STORGE,							// ���˲ֿ�������Ʒ
		AUTHORITY_EXTEND_MEMBER,					// ��չ���˳�Ա����
		AUTHORITY_SET_AUTO_CLEAR,					// �����Զ��������Ա
		AUTHORITY_CHANGE_AVATAR,					// �ı乫��ͷ��
		AUTHORITY_BIAOCHE_START,					// ��ʼ�ڳ�
		AUTHORITY_TIANCITONGBI,						// ���ͭ��

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

