
#ifndef MSGLOGIN_H
#define MSGLOGIN_H

#include "servercommon/serverdef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/logindef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	// ��½���
	class CSLogin
	{
	public:
		CSLogin();
		MessageHeader		header;
		int					rand_val1;		// ����� 100w-1000w
		unsigned int		login_time;
		LoginStr			login_str;
		PlatName			pname;
		int					rand_val2;		// ����� 100w-1000w
		short				reserved1;
		short				server;
	};

	enum __LoginAckResult
	{
		LOGIN_RESULT_SUC=0,
		LOGIN_NO_THREAD=-1,
		LOGIN_SERVER_ERROR=-2,
		LOGIN_RESULT_EXIST=-3,
		LOGIN_SCENE_NOT_EXIST=-4,
		LOGIN_RESULT_NO_GATEWAY=-5,
		LOGIN_RESULT_NO_ROLE=-6,
		LOGIN_THREAD_BUSY=-7,
		LOGIN_LOGIN_FORBID=-8,
		LOGIN_ANTI_WALLOW=-9,
	};

	struct RoleItem 
	{
		int					role_id;
		GameName			role_name;
		char				avatar;
		char				sex;
		char				professional;
		char				country;
		int					level;
		unsigned int		create_time;
		unsigned int		last_login_time;
		ItemID				wuqi_id;
		char				shizhuang_wuqi;
		char				shizhuang_wuqi_is_special;			// ʱװ��������������
		char				shizhuang_body;						// ʱװ����
		char				shizhuang_body_is_special;			// ʱװ��������������
		short				wing_used_imageid;
		short				halo_used_imageid;
		short				yaoshi_used_imageid;				// ����
		short				toushi_used_imageid;				// ͷ��
		short				qilinbi_used_imageid;				// �����
		short				mask_used_imageid;					// ���
		short				upgrade_used_imageid_0;			// ����ϵͳ����id
		short				upgrade_used_imageid_1;			// ����ϵͳ����id
		short				upgrade_used_imageid_2;			// ����ϵͳ����id
		short				upgrade_used_imageid_3;			// ����ϵͳ����id
		short				upgrade_used_imageid_4;			// ����ϵͳ����id
		short				upgrade_used_imageid_5;			// ����ϵͳ����id
		short				upgrade_used_imageid_6;			// ����ϵͳ����id
		short				upgrade_used_imageid_7;			// ����ϵͳ����id
		short				upgrade_used_imageid_8;			// ����ϵͳ����id
	};

	#define MAX_ACCOUNT_ROLE_NUM	3

	class SCRoleList	// 7001
	{
	public:
		SCRoleList();
		MessageHeader		header;
		short				reserved;
		short				result;
		int					count;
		RoleItem			role_item[MAX_ACCOUNT_ROLE_NUM];
	};

	class CSRoleReq
	{
	public:
		CSRoleReq();
		MessageHeader		header;

		int					rand_val1;		// ����� 100w-1000w
		unsigned int		login_time;
		LoginStr			login_str;
		PlatName			pname;
		short				server;
		char				reserved1;
		char				anti_wallow;

		int					rand_val2;		// ����� 100w-1000w
		int					role_id;
	};

	struct MergeRoleItem 
	{
		int					role_id;
		GameName			role_name;
		char				avatar;
		char				sex;
		char				professional;
		char				country;
		int					level;
		int					result;
		unsigned int		create_time;
		unsigned int		last_login_time;
		ItemID				wuqi_id;
		char				shizhuang_wuqi;
		char				shizhuang_wuqi_is_special;			// ʱװ��������������
		char				shizhuang_body;
		char				shizhuang_body_is_special;			// ʱװ��������������
		short				wing_used_imageid;
		short				halo_used_imageid;
		short				yaoshi_used_imageid;				// ����
		short				toushi_used_imageid;				// ͷ��
		short				qilinbi_used_imageid;				// �����
		short				mask_used_imageid;					// ���
		short				upgrade_used_imageid_0;			// ����ϵͳ����id
		short				upgrade_used_imageid_1;			// ����ϵͳ����id
		short				upgrade_used_imageid_2;			// ����ϵͳ����id
		short				upgrade_used_imageid_3;			// ����ϵͳ����id
		short				upgrade_used_imageid_4;			// ����ϵͳ����id
		short				upgrade_used_imageid_5;			// ����ϵͳ����id
		short				upgrade_used_imageid_6;			// ����ϵͳ����id
		short				upgrade_used_imageid_7;			// ����ϵͳ����id
		short				upgrade_used_imageid_8;			// ����ϵͳ����id
	};

	#define MAX_MERGE_ACCOUNT_ROLE_NUM	32

	class SCMergeRoleList			// 7005
	{
	public:
		SCMergeRoleList();
		MessageHeader		header;

		int					count;
		MergeRoleItem		role_item_list[MAX_MERGE_ACCOUNT_ROLE_NUM];
	};

	class SCLoginAck
	{
	public:
		SCLoginAck();
		MessageHeader		header;

		int					uid;
		short				result;
		char				reserve_ch;
		char				is_merged_server;
		int					scene_id;
		int					last_scene_id;
		SessionKey			key;
		unsigned int		time;
		char				gs_hostname[64];	// �޸ĸó��ȵ�ͬʱ�������޸�lwprotocal::WLRegisterGW::hostname�ĳ���
		unsigned short		gs_port;
		unsigned short		gs_index;
		unsigned int		server_time;
	};

	class CSLHeartBeat
	{
	public:
		CSLHeartBeat();
		MessageHeader		header;
	};

	class SCLHeartBeat
	{
	public:
		SCLHeartBeat();
		MessageHeader		header;
	};

	class CSGetThreadInfo
	{
	public:
		CSGetThreadInfo();
		MessageHeader		header;
		PlatName			pname;
		unsigned int		login_time;
		LoginStr			login_str;
		short				reserved1;
		short				server;
	};


	enum THREAD_STATE
	{
		THREAD_STATE_IDLE=0,
		THREAD_STATE_NORMAL,
		THREAD_STATE_BUSY,
		THREAD_STATE_FULL,
	};

	class SCGetThreadAck
	{
	public:
		SCGetThreadAck();
		MessageHeader		header;
		int					thread_state;
	};

	class CSPassAntiWallow
	{
	public:
		CSPassAntiWallow();
		MessageHeader		header;
		PlatName			pname;
		unsigned int		login_time;
		LoginStr			login_str;
		char				anti_wallow;
		char				reserved1;
		short				server;
	};

	class CSPassAntiWallowEx
	{
	public:
		CSPassAntiWallowEx();
		MessageHeader		header;
		PlatName			pname;
		unsigned int		login_time;
		LoginStr			login_str;
		char				anti_wallow;
		char				reserved1;
		short				server;

		char				name[20];
		char				id[20];
	};

	enum ANTIWALLOW_RET
	{
		ANTIWALLOW_SUC=0,
		ANTIWALLOW_ID_INVALID=-1,
		ANTIWALLOW_ID_REPEATE=-2,
		ANTIWALLOW_FAIL=-3,
		ANTIWALLOW_NOT_ENOUGH_18=-4,
	};

	class SCAntiWallowRet
	{
	public:
		SCAntiWallowRet();
		MessageHeader		header;
		int					ret;		// 0 �ɹ� -1���֤��Ч -2���֤�ظ� -3ʧ��
	};

	class SCAccountKeyError
	{
	public:
		SCAccountKeyError();
		MessageHeader		header;
	};

	class SCProfNumInfo
	{
	public:
		SCProfNumInfo();
		MessageHeader		header;

		int prof1_num;
		int prof2_num;
		int prof3_num;
		int prof4_num;
	};
}

#pragma pack(pop)

#endif

