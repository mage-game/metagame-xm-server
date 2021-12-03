
#ifndef MSGPRESENT_H
#define MSGPRESENT_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/chatserverdef.h"

#include "servercommon/maildef.hpp"
#include "servercommon/lovecontractdef.hpp"
#include "servercommon/yaojiangdef.hpp"
#include "servercommon/marrydef.hpp"
#include "servercommon/struct/discountbuyparam.hpp"
#include "other/turntable/turntableconfig.h"
#include "servercommon/roleshopdef.hpp"
#include "other/rollmoney/rollmoney.h"
#include "servercommon/struct/rolegoalparam.hpp"
#include "global/redpaper/redpapermanager.h"
#include "global/marriageseeking/marriageseekingmanager.h"
#include "servercommon/struct/shizhuangparam.hpp"
#include "servercommon/wingdef.hpp"
#include "servercommon/qingyuandef.hpp"
#include "servercommon/struct/cardparam.hpp"
#include "servercommon/struct/zhibaoparam.hpp"
#include "servercommon/guilddef.hpp"
#include "global/worldboss/worldbossmanager.hpp"
#include "servercommon/struct/rolegoalparam.hpp"
#include "servercommon/struct/zerogiftparam.hpp"
#include "gameworld/other/wabao/wabaoconfig.hpp"
#include "servercommon/shenyindef.hpp"
#include "other/upgraderush/upgraderushconfig.hpp"
#include "servercommon/talentdef.hpp"
#include "servercommon/droploglistdef.hpp"
#include "servercommon/bosscarddef.hpp"
#include "gameworld/global/luckylog/luckylogmanager.hpp"
#include "config/touzijihuaconfig.hpp"
#include "servercommon/struct/rolebigsmallgoalparam.hpp"
#include "servercommon/struct/global/worldstatus2param.hpp"
#include "servercommon/struct/orangeequipcollectdef.hpp"
#include "servercommon/struct/redequipcollectdef.hpp"
#include "servercommon/xianzunkadef.hpp"
#include "servercommon/equipbaptizedef.hpp"
#include "servercommon/commondata.hpp"
#include "servercommon/huguozhilidef.hpp"
#include "servercommon/crossgoaldef.hpp"
#include "servercommon/baizhanequipdef.hpp"
#include "global/guildrarelog/guildrarelogmanager.hpp"
#include "config/littlehelperconfig.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class CSSpeaker		// 2750
	{
	public:
		CSSpeaker();
		MessageHeader		header;

		enum SPEAKER_TYPE 
		{
			SPEAKER_TYPE_LOCAL,
			SPEAKER_TYPE_CROSS,

			SSPEAKER_TYPE_MAX,
		};

		void ResetSpeakMsg()
		{
			memset(speaker_msg, 0, sizeof(speaker_msg));
		}

		char				content_type;
		char				speaker_type;
		short				reserve_sh;
		int					is_auto_buy;
		unsigned int		speaker_msg_length;
		char				speaker_msg[MAX_SPEAKER_MSG_LENGTH];
	};

	class CSEvaluateRole
	{
	public:
		CSEvaluateRole();
		MessageHeader		header;

		int					uid;
		int					reserve_int;
	};


	class SCFunctionSuccNotice
	{
	public:
		SCFunctionSuccNotice();
		MessageHeader		header;

		enum FUNCTION_SUCC_TYPE
		{
			FST_EQUIP_STRENGTHEN = 0,
			FST_EQUIP_UPQUALITY,
			FST_EQUIP_UPLEVEL,
			FST_EQUIP_DECOMPOSE,
			FST_EQUIP_FLUSH,
			FST_GEMSTONE_COMPOSE,
			FST_EQUIP_STRENGTHEN_FAIL,
			FST_EQUIP_REFINE,

			FST_PET_LIFT_WUXING_CTRI = 20,
			FST_PET_LIFT_LINGXING,

			FST_MOUNT_STRENGTHEN = 40,
			FST_MOUNT_STRENGTHEN_FAIL,
		};
		short				succ_type;
		short				flag;
	};

	class SCFunctionFailNotice
	{
	public:
		SCFunctionFailNotice();
		MessageHeader		header;

		enum FUNCTION_SUCC_TYPE
		{
			FST_EQUIP_STRENGTHEN = 0,

			FST_MOUNT_STRENGTHEN = 40,
		};

		short				fail_type;
		short				flag;
	};


	class SCRoleSundryInfo
	{
	public:
		SCRoleSundryInfo();
		MessageHeader		header;

		int					has_favorited;
		int					can_fetch_weekend_reward;
	};

	class CSClientSpecialOperate
	{
	public:
		CSClientSpecialOperate();
		MessageHeader		header;

		enum
		{
			CLIENT_OPERATE_TYPE_INVALID = 0,
			CLIENT_OPERATE_TYPE_GUILD_GUIDE,		// ��������
			CLIENT_OPERATE_TYPE_MAX
		};

		int					client_operate_type;
	};

	class SCOperateSucc
	{
	public:
		SCOperateSucc();
		MessageHeader		header;

		enum SUCC_REASON
		{
			SUCC_REASON_EQUIP_STRENGTH = 1,										// װ��ǿ��
		};

		short				reason;												// ԭ��
		short				param;												// ����Ԥ��
	};

	class CSRoleTaskTransmit
	{
	public:
		CSRoleTaskTransmit();
		MessageHeader		header;

		int pos_x;
		int pos_y;
	};

	class CSTaskOpenPanel
	{
	public:
		CSTaskOpenPanel();
		MessageHeader		header;

		enum OPEN_TYPE
		{
			OPEN_TYPE_PET_SKILL_PANEL = 0,										// ���＼�����
			OPEN_TYPE_CAPABILITY_PANEL = 1,										// ս�������
		};

		int open_type;
	};

	/////////////////////////////////// Ͷ�ʼƻ� ///////////////////////////////////////
	class SCTouZiJiHuaInfo				// 2720 Ͷ�ʼƻ���Ϣ
	{
	public:
		SCTouZiJiHuaInfo();
		MessageHeader		header;

		char active_plan_0;				// �ȼ�Ͷ��1�����־
		char active_plan_1;				// �ȼ�Ͷ��2�����־
		char active_plan_2;				// �ȼ�Ͷ��3�����־
		char new_plan_first_reward_flag;	// �ܿ�Ͷ��������־

		short plan_0_reward_flag;		// �ȼ�Ͷ��1������־
		short plan_1_reward_flag;		// �ȼ�Ͷ��2������־
		short plan_2_reward_flag;		// �ȼ�Ͷ��3������־
		char active_highest_plan;		// ��ǰ������ߵȼ�Ͷ�ʼƻ�
		char reserve_sh;

		char reserve;
		char active_plan_3;				// �ȼ�Ͷ��4�����־
		short plan_3_reward_flag;		// �ȼ�Ͷ��4������־

		unsigned int buy_time;			// �ܿ�Ͷ�ʹ���ʱ��
		int reward_flag;
		int vip_reward_flag;
		int reward_gold_bind_flag;		// �ܿ�Ͷ�ʽ�����־

		int list_len;
		char foundation_next_reward_index[TouzijihuaConfig::MAX_FOUNDATION_TYPE];	//�ɳ���������ȡ����,0����û����

		UNSTD_STATIC_CHECK(TouzijihuaConfig::MAX_FOUNDATION_TYPE % 4 == 0)
	};

	class CSFetchTouZiJiHuaReward		// 2780 ��ȡͶ�ʼƻ���������
	{
	public:
		CSFetchTouZiJiHuaReward();
		MessageHeader		header;

		short plan_type;
		short seq;
	};

	class CSTouzijihuaActive			// 2786 ����Ͷ�ʼƻ�����
	{
	public:
		CSTouzijihuaActive();
		MessageHeader		header;

		short plan_type;
		short reserve_sh;
	};

	enum NEW_TOUZIJIHUA_OPERATE_TYPE
	{
		NEW_TOUZIJIHUA_OPERATE_BUY = 0,		// �����ܿ�Ͷ��
		NEW_TOUZIJIHUA_OPERATE_FETCH,		// ��ȡ�ܿ�����
		NEW_TOUZIJIHUA_OPERATE_FIRST,		// ��ȡ�ܿ�����
		NEW_TOUZIJIHUA_OPERATE_VIP_FETCH,
		NEW_TOUZIJIHUA_OPERATE_FOUNDATION_FETCH,	//��ȡ�ɳ�����
	};
	class CSNewTouzijihuaOperate		// 2787 ��Ͷ�ʼƻ���������
	{
	public:
		CSNewTouzijihuaOperate();
		MessageHeader		header;

		short operate_type;
		short param;
	};

	enum TOUZIJIHUA_FB_BOSS_OPERATE_TYPE
	{
		TOUZIJIHUA_FB_BOSS_OPERATE_FB_BUY = 0,			// ���򸱱�Ͷ�ʼƻ�
		TOUZIJIHUA_FB_BOSS_OPERATE_FB_REWARD =1,		// ��ȡ����Ͷ�ʼƻ�������param��index

		TOUZIJIHUA_FB_BOSS_OPERATE_BOSS_BUY =2,			// ����bossͶ�ʼƻ�
		TOUZIJIHUA_FB_BOSS_OPERATE_BOSS_REWARD =3,		// ��ȡbossͶ�ʼƻ���param��index

		TOUZIJIHUA_FB_BOSS_OPERATE_SHENYU_BOSS_BUY = 4,			// ��������bossͶ�ʼƻ�
		TOUZIJIHUA_FB_BOSS_OPERATE_SHENYU_BOSS_REWARD = 5,		// ��ȡ����bossͶ�ʼƻ���param��index
	};

	class CSTouzijihuaFbBossOperate			// 2731 ������bossͶ�ʼƻ�����
	{
	public:
		CSTouzijihuaFbBossOperate();
		MessageHeader	header;

		short operate_type;
		short param;
	};

	class SCTouzijihuaFbBossInfo		  // 2732 ������bossͶ�ʼƻ���Ϣ
	{
	public:
		SCTouzijihuaFbBossInfo();
		MessageHeader	header;

		short active_fb_plan;
		short fb_plan_reward_flag;
		short fb_pass_level;

		short active_boss_plan;
		short boss_plan_reward_flag;
		short kill_boss_num;

		short active_shenyu_boss_plan;
		short shenyu_boss_plan_reward_flag;
		short kill_shenyu_boss_num;
		short reserve_sh;
	};
	//////////////////////////////////////////////////////////////////////////
	class CSYaojiangReq			// 2807
	{
	public:
		CSYaojiangReq();
		MessageHeader		header;

		int yaojiang_type;
	};

	class SCYaojiangInfo		// 2745
	{
	public:
		SCYaojiangInfo();
		MessageHeader		header;

		int rand_index;
		ItemID item_id;
		short num;
	};

	///////////////////////////////// ���ϵͳ /////////////////////////////////////////

	//-------------------------------- ͨ�� -----------------------------------------
	class CSMarryXunyouOpera  // 2793 ���Ѳ��
	{
	public:
		CSMarryXunyouOpera();
		MessageHeader		header;

		int					opera_type;
	};

	class CSResetLoverName	// 2796 ������������
	{
	public:
		CSResetLoverName();
		MessageHeader		header;

		GameName			new_lover_name;
	};

	class SCMarryXunyouFirework	 // 6006 Ѳ���̻�(������)
	{
	public:
		SCMarryXunyouFirework();
		MessageHeader		header;

		ObjID				obj_id;
		short				reserved;
	};

	class SCMarryReqRoute	// 6004 �����Ϣת���Է�
	{
	public:
		SCMarryReqRoute();
		MessageHeader		header;

		int					hunli_type;
		int					req_uid;
		GameName			req_name;
	};

	class SCMarrySpecialEffect	// 6003�����Ч
	{
	public:
		SCMarrySpecialEffect();
		MessageHeader		header;

		int					marry_type;
	};

	enum MARRY_REQ_TYPE
	{
		MARRY_REQ_TYPE_PROPOSE = 0,		// ��� p1:�������� p2: �Է�uid
		MARRY_CHOSE_SHICI_REQ,			// ѡ�Ĵ� p1:�Ĵ�����
		MARRY_PRESS_FINGER_REQ,         // ����ָ
	};

	class CSMarryReq	// 2778 �������
	{
	public:
		CSMarryReq();
		MessageHeader		header;

		int				ope_type;
		int				param_1;
		int				param_2;
	};

	class CSMarryRet	// 2779 ���ظ�����
	{
	public:
		CSMarryRet();
		MessageHeader		header;

		int					marry_type;
		int					req_uid;
		int					is_accept;
	};

	class SCIsAcceptMarry	// 6616 �Ƿ�ͬ�����
	{
	public:
		SCIsAcceptMarry();
		MessageHeader		header;

		int is_accept;		// ͬ�⣺��ͬ��  1��0
	};

	enum MARRY_RET_TYPE
	{
		MARRY_AGGRE = 0,			// �����Ը��
		MARRY_CHOSE_SHICI,			// ѡ�Ĵ�
		MARRY_PRESS_FINGER,         // ����ָ
		MARRY_CANCEL,				// ȡ�����
	};

	class SCMarryRetInfo			// 2864 ����������  
	{
	public:
		SCMarryRetInfo();
		MessageHeader header;

		int ret_type;				// �������ͣ�MARRY_RET_TYPE��
		int ret_val;				// ����ֵ 0�������role_id��1���Ĵ����� 2��˫���Ƿ�����ָ��1�����Լ��Ѱ���2����˫���Ѱ���
	};

	class SCMarryInformation	// 6604 �����Ϣ
	{
	public:
		SCMarryInformation();
		MessageHeader		header;

		int				lover_uid;
		UserName		lover_name;
	};

	class CSQingyuanDivorceReq
	{
	public:
		CSQingyuanDivorceReq();
		MessageHeader header;

		int is_forced_divorce;  // 1��ǿ�����, 0����ǿ�����
	};

	class SCDivorceReqRoute	// 6005 �����Ϣת��
	{
	public:
		SCDivorceReqRoute();
		MessageHeader		header;

		int					req_uid;
		GameName			req_name;
	};

	class CSDivorceRet	// 6603 ���ظ�
	{
	public:
		CSDivorceRet();
		MessageHeader		header;

		int					req_uid;
		int					is_accept;
	};

	//-------------------------------- ��Ե�¼�Э�� ----------------------
	enum QINGYUAN_OPERA_TYPE
	{
		QINGYUAN_OPERA_TYPE_WEDDING_YUYUE = 0,		// ����ԤԼ param1 ԤԼ�±� param2 ��������
		QINGYUAN_OPERA_TYPE_WEDDING_INVITE_GUEST,	// ������� param1 ����uid
		QINGYUAN_OPERA_TYPE_WEDDING_REMOVE_GUEST,	// �Ƴ����� param1 ����uid
		QINGYUAN_OPERA_TYPE_WEDDING_BUY_GUEST_NUM,	// �����������
		QINGYUAN_OPERA_TYPE_WEDDING_GET_YUYUE_INFO,	// ��ȡԤԼ��Ϣ
		QINGYUAN_OPERA_TYPE_WEDDING_GET_ROLE_INFO,	// ��ȡ�����Ϣ
		QINGYUAN_OPERA_TYPE_WEDDING_YUYUE_FLAG,		// ��ȡ����ԤԼ���
		QINGYUAN_OPERA_TYPE_WEDDING_YUYUE_RESULT,	// �Ƿ�ͬ�����ԤԼʱ�� param1 seq param2 �Ƿ�ͬ��
		QINGYUAN_OPERA_TYPE_LOVER_INFO_REQ,			// ���������Ϣ
		QINGYUAN_OPERA_TYPE_XUNYOU_ROLE_INFO,		// ��ȡ���Ѳ����Ϣ
		QINGYUAN_OPERA_TYPE_XUNYOU_SA_HONGBAO,		// Ѳ������� param1:is_buy
		QINGYUAN_OPERA_TYPE_XUNYOU_GIVE_FLOWER,		// Ѳ�ι����ͻ�����  
		QINGYUAN_OPERA_TYPE_XUNYOU_OBJ_POS,			// ��ȡѲ�ζ���ǰ���� param1:seq
		QINGYUAN_OPERA_TYPE_LOVER_TITLE_INFO,		// ���³ƺ���Ϣ �������ã�
		QINGYUAN_OPERA_TYPE_FETCH_LOVER_TITLE,		// ��ȡ���³ƺ� param1 index �������ã�
		QINGYUAN_OPERA_TYPE_BUY_AND_PUTON_EQUIP,	// �����Ҵ���װ�� param1 index �������ã�

		QINGYUAN_OPERA_TYPE_MAX,
	};

	class CSQingYuanOperaReq		// 2795 ��Ե��������
	{
	public:
		CSQingYuanOperaReq();
		MessageHeader header;

		int opera_type;
		int param1;
		int param2;
	};

	enum QINGYUAN_INFO_TYPE
	{
		QINGYUAN_INFO_TYPE_WEDDING_YUYUE = 0,	// ����ԤԼ
		QINGYUAN_INFO_TYPE_WEDDING_STANDBY,		// ����׼��
		QINGYUAN_INFO_TYPE_GET_BLESSING,		// �յ�ף�� param_ch1 ף������ param2 ����
		QINGYUAN_INFO_TYPE_BAITANG_RET,			// ��������
		QINGYUAN_INFO_TYPE_BAITANG_EFFECT,		// ������Ч param_ch1 �Ƿ��Ѿ�����
		QINGYUAN_INFO_TYPE_LIVENESS_ADD,		// �����ȶ����� param2 ��ǰ�ȶ�
		QINGYUAN_INFO_TYPE_HAVE_APPLICANT,		// ���������� param2 ������uid 
		QINGYUAN_INFO_TYPE_APPLY_RESULT,		// ������ param2 1:ͬ�� 0:�ܾ�
		QINGYUAN_INFO_TYPE_ROLE_INFO,			// �����Ϣ param_ch1 �������� param_ch2 �Ƿ��л������ param_ch3 ��ǰ����״̬ param_ch4 ����ԤԼseq param_ch5 ��齱�����
		QINGYUAN_INFO_TYPE_WEDDING_YUYUE_FLAG,	// ����ԤԼ��� param6
		QINGYUAN_INFO_TYPE_YUYUE_RET,			// ����ԤԼ���� param_ch1 seq
		QINGYUAN_INFO_TYPE_YUYUE_POPUP,			// ����ԤԼ����
		QINGYUAN_OPERA_TYPE_BUY_QINGYUAN_FB_RET,// �յ������������
		QINGYUAN_INFO_TYPE_YUYUE_SUCC,			// ����ԤԼ�ɹ�
		QINGYUAN_INFO_TYPE_LOVER_INFO,			// ������Ϣ param5 ����uid param_ch1 ������Ӫ role_name ��������
		QINGYUAN_INFO_TYPE_LOVER_TITLE_INFO,	// ���³ƺ���Ϣ param2 ��ȡflag �������ã�
		QINGYUAN_INFO_TYPE_WEDDING_BEGIN_NOTICE,// ���翪��֪ͨ
		QINGYUAN_INFO_TYPE_XUNYOU_INFO,			// ����Ѳ����Ϣ p_c1:��������� p_c2:������������ p_c3:�ͻ����� p_c4:�ͻ�������� p_c5:�ɼ�Ѳ�κ������ p_ch6:��������
		QINGYUAN_INFO_TYPE_XUNYOU_OBJ_POS,		// ����Ѳ�ζ������� p_c1:�Ƿ���Ѳ������ p2:x p3:y p4:obj_id p5:scene_id
	};

	class SCQingYuanAllInfo  // 2784 ��Ե������Ϣ
	{
	public:
		SCQingYuanAllInfo();
		MessageHeader header;

		int info_type;
		char param_ch1;
		char param_ch2;
		char param_ch3;
		char param_ch4;
		char param_ch5;
		char param_ch6;
		short param2;
		short param3;
		short param4;
		int param5;
		long long param6;
		GameName role_name;    
	};

	struct WeddingGuestInfo
	{
		WeddingGuestInfo() : user_id(0)
		{
			memset(name, 0, sizeof(GameName));
		}

		int user_id;
		GameName name;
	};

	class SCQingYuanWeddingAllInfo	//  6007 ��Ե����ԤԼ��Ϣ����
	{
	public:
		SCQingYuanWeddingAllInfo();
		MessageHeader header;

		int role_id;						// ��������id
		GameName role_name;					// ������������
		int lover_role_id;					// ������id
		GameName lover_role_name;			// ����������
		char wedding_type;					// �������
		char has_invite_guests_num;			// �������������
		char can_invite_guest_num;			// �ܹ�����ı��������
		char wedding_yuyue_seq;				// ����ԤԼʱ���
		int wedding_index;					// ȫ���ڼ�������
		char role_prof;						// ��������ְҵ
		char lover_role_prof;				// ������ְҵ
		short reserve_sh;	
		int count;
		WeddingGuestInfo guests_list[WEDDING_GUESTS_MAX_NUM];
	};

	//-------------------------------- ���ǽ ----------------------

	enum PROFESS_WALL_REQ_TYPE
	{
		PROFESS_WALL_REQ_INFO = 0,			// ���ǽ��Ϣ��p1:0�Լ� 1�Է� 2������p2:ʱ����� ������Ϣ����ʱ���
		PROFESS_WALL_REQ_LEVEL_INFO = 1,	// ��׵ȼ���Ϣ
		PROFESS_WALL_REQ_DELETE = 2,		// ɾ����ף�p1:ǽ���ͣ�p2:ʱ�����p3:��ɫid�� �����1145���أ�operateΪ75,p1ʱ��� p2:role_id

		PROFESS_WALL_REQ_TYPE_MAX,
	};

	// ���ǽͨ������ 2865
	class CSProfessWallReq
	{
	public:
		CSProfessWallReq();
		MessageHeader header;

		int oper_type;
		unsigned int param_1;
		unsigned int param_2;
		unsigned int param_3;
	};

	// ������� 2866
	class CSProfessToReq
	{
	public:
		CSProfessToReq();
		MessageHeader header;

		int target_role_id;		// �Է�id
		short gift_id;			// ��������
		short is_auto_buy;		// �Ƿ��Զ�����
		ProfessContent	content;// �������
	};


	// �������ǽ��Ϣ 2867
	class SCGlobalProfessWallInfo
	{
	public:
		SCGlobalProfessWallInfo();
		MessageHeader header;

		struct ProfessItem
		{
			int				role_id_from;		// �����
			int				role_id_to;			// �������
			int				gife_type;			// ��������
			unsigned int	profess_time;		// ���ʱ��
			long long		avatar_timestamp;	// �����ͷ��
			long long		avatar_timestamp_to;// �������ͷ��
			GameName		role_name_from;		// ���������
			GameName		role_name_to;		// �����������
			ProfessContent	content;			// �������
		};

		int profess_count;						// �������
		unsigned int timestamp;					// ����ʱ��������ⷢ�ظ����ݣ�
		ProfessItem items[GLOBAL_PROFESS_MAX_NUM];	//���30��
	};

	enum PROFESS_INFO_NOTIFY_TYPE
	{
		PROFESS_INFO_NOTIFY_TYPE_NORMAL = 0,
		PROFESS_INFO_NOTIFY_TYPE_PROFESS_FROM,       // �����֪ͨ 
	};

	// ���˱��ǽ��Ϣ 2868
	class SCPersonProfessWallInfo
	{
	public:
		SCPersonProfessWallInfo();
		MessageHeader header;

		struct ProfessItem
		{
			int				other_role_id;		// �Է�id
			int				gife_type;			// ��������
			unsigned int	profess_time;		// ���ʱ��
			long long		avatar_timestamp;	// �Է�ͷ��
			GameName		other_name;			// �Է�����
			ProfessContent	content;			// �������
		};

		char notify_type;						// ֪ͨ����
		char profess_type;						// ���ǽ����
		short profess_count;					// �������
		unsigned int timestamp;					// ����ʱ��������ⷢ�ظ����ݣ�
		ProfessItem items[PERSON_PROFESS_MAX_NUM];	//���12��
	};

	// �����Ч	2869
	class SCProfessWallEffect
	{
	public:
		SCProfessWallEffect();
		MessageHeader header;

		int effect_type;
	};

	// ��׵ȼ���Ϣ	2870
	class SCProfessLevelInfo
	{
	public:
		SCProfessLevelInfo();
		MessageHeader header;

		unsigned short my_grade;		// �Լ��ı�׵ȼ�
		unsigned short other_grade;		// �Է��ı�׵ȼ�
		unsigned int my_exp;			// �Լ��ı�׾���
		unsigned int other_exp;			// �Է��ı�׾���
	};

	//-------------------------------- ���縱�� -----------------------------------------
	class CSMarryHunyanBless  // 2783 ����ף��
	{
	public:
		CSMarryHunyanBless();
		MessageHeader		header;

		void ResetMailContentTxt()
		{
			memset(contenttxt, 0, sizeof(contenttxt));
		}

		int					merry_uid;
		int					zhufu_type;

		int					contenttxt_len;
		MailContentTxt		contenttxt;
	};

	class CSMarryHunyanGetBlessNews  // 2794 ��ȡ����ף����Ϣ (��ʱ����)
	{
	public:
		CSMarryHunyanGetBlessNews();
		MessageHeader		header;
	};

	class CSMarryHunyanOpera   // 2782 �����������
	{
	public:
		CSMarryHunyanOpera();
		MessageHeader		header;

		int					opera_type;
		int					param_1;
		int					param_2;
	};

	enum SPECIAL_PARAM_CHANGE_TYPE
	{
		SPECIAL_PARAM_CHANGE_TYPE_VALID = 0,
		SPECIAL_PARAM_CHANGE_TYPE_XUNYOU,			// Ѳ��
	};

	class SCSpecialParamChange	// 2729 ����״̬�ı�
	{
	public:
		SCSpecialParamChange();
		MessageHeader		header;

		short change_type;
		ObjID obj_id;
		int param;
	};

	class SCMarryHunyanOpera	// 6008 �������������Ϣ
	{
	public:
		SCMarryHunyanOpera();
		MessageHeader		header;

		ObjID				obj_id;
		short				opera_type;
		short				opera_param;
		short				reserve;
	};

	class SCWeddingBlessingRecordInfo // 6010 ����ף����¼
	{
	public:
		SCWeddingBlessingRecordInfo();
		MessageHeader header;

		int count;
		WeddingBlessingRecord record_info_list[WEDDING_BLESSSING_MAX_RECORD_NUM];
	};

	class SCWeddingApplicantInfo  // 6016  ���������б���
	{
	public:
		SCWeddingApplicantInfo();
		MessageHeader header;

		int count;
		WeddingApplicantInfo applicant_list[WEDDING_APPLICANT_MAX_NUM];
	};

	class SCHunYanCurWeddingAllInfo  // 6000 ��ǰ���糡����Ϣ���󷵻�
	{
	public:
		SCHunYanCurWeddingAllInfo();
		MessageHeader header;

		int role_id;						// ��������id
		GameName role_name;					// ������������
		int lover_role_id;					// ������id
		GameName lover_role_name;			// ����������
		char role_prof;						// ��������ְҵ
		char lover_role_prof;				// ������ְҵ
		char reserve_ch;
		char cur_wedding_seq;				// ����ʱ������
		int wedding_index;					// ȫ���ڼ�������
		int count;
		int guests_uid[WEDDING_GUESTS_MAX_NUM]; // �����б�
	};

	class SCWeddingRoleInfo   // 5440 ���糡��������Ϣ
	{
	public:
		SCWeddingRoleInfo();
		MessageHeader header;

		void Reset()
		{
			wedding_liveness = 0;
			is_baitang = 0;
			is_in_red_bag_fulsh_time = 0;
			banquet_has_gather_num = 0;
			cur_turn_has_gather_red_bag = 0;
			total_exp = 0;
			hunyan_food_id_count = 0;

			memset(hunyan_food_id_list, 0, sizeof(hunyan_food_id_list));
		}

		static const int MAX_GATHR_FOOD_COUNT = 20;

		short wedding_liveness;
		char is_baitang;
		char is_in_red_bag_fulsh_time;
		short banquet_has_gather_num;
		short cur_turn_has_gather_red_bag;
		long long total_exp;

		short hunyan_food_id_count;							// �Ѳɻ����ϯ����
		ObjID hunyan_food_id_list[MAX_GATHR_FOOD_COUNT];	// �Ѳɻ����ϯobjid�б�
	};

	enum HUNYAN_NOTIFY_REASON
	{
		HUNYAN_NOTIFY_REASON_STATE_CHANGE = 0,			// ״̬�ı�
		HUNYAN_NOTIFY_REASON_ENTER_HUNYAN,				// �������
		HUNYAN_NOTIFY_REASON_LOGIN,						// ����
		HUNYAN_NOTIFY_REASON_INVITE_FRIEND,				// �������
		HUNYAN_NOTIFY_REASON_INVITE_GUILD,				// �������˺���
		HUNYAN_NOTIFY_REASON_GATHER,					// �ɼ�
		HUNYAN_NOTIFY_REASON_PAOHUAQIU,					// �׻���
		HUNYAN_NOTIFY_REASON_LEVEL_OR_ENTER,			// ���������䶯
		HUNYAN_NOTIFY_REASON_LEAVE,						// �뿪����
	};

	struct HunyanMarryUserInfo
	{
		HunyanMarryUserInfo()
			: marry_uid(0), sex(0), prof(0), camp(0), hongbao_count(0), avator_timestamp(0)
		{
			memset(marry_name, 0, sizeof(marry_name));
		}

		int marry_uid;
		GameName marry_name;
		char sex;
		char prof;
		char camp;
		char hongbao_count;
		long long avator_timestamp;
	};

	const int HUNYAN_MARRY_USER_COUNT = 2;

	class SCHunyanStateInfo	// 6009 ���糡��״̬��Ϣ
	{
	public:
		SCHunyanStateInfo();
		MessageHeader		header;

		int notify_reason;  

		int hunyan_state;												// ����״̬
		unsigned int next_state_timestmp;
		int fb_key;														// ����key
		int yanhui_type;												// ��������
		int remainder_eat_food_num;										// �����Բɼ�����ʳ��
		int roleuid_count;												// 
		int reserve_int;												// �Ƿ��һ����ʯ���
		int is_self_hunyan;												// �Ƿ����Լ����Ļ���
		unsigned int paohuoqiu_timestmp;                                // ���һ�ο�ʼ�׻����ʱ��
		int total_gather_times;											// �ɼ���ϯ����

		HunyanMarryUserInfo marryuser_list[HUNYAN_MARRY_USER_COUNT];	// ������
		int roleuid_list[MAX_ROLE_IN_YANHUI];							// ���͵�list
	};

	class SCQingyuanHunyanInviteInfo // 5439 ����������Ϣ�������ã�
	{
	public:
		SCQingyuanHunyanInviteInfo();
		MessageHeader header;

		struct InviteItem
		{
			InviteItem()
			{
				this->Reset();
			}

			void Reset()
			{
				memset(mem_name, 0, sizeof(mem_name));
				memset(women_name, 0, sizeof(women_name));
				yanhui_fb_key = -1;
				yanhui_type = -1;
				gather_num = 0;
			}

			GameName mem_name;
			GameName women_name;
			int yanhui_fb_key;
			int yanhui_type;
			int gather_num;
		};

		static const int INVITE_NUM = 10;
		int invite_count;
		InviteItem scq_invite_list[INVITE_NUM];
	};
	
	// 6013 ֪ͨȫ�����
	class SCMarryNotic
	{
	public:
		SCMarryNotic();
		MessageHeader		header;
		int uid1;
		GameName name1;
		long long avator_timestamp1;
		int prof1;

		int uid2;
		GameName name2;
		long long avator_timestamp2;
		int prof2;

		int server_marry_times;
	};

	enum MARRY_ZHUHE_TYPE
	{
		MARRY_ZHUHE_TYPE0 = 0,			// ף��
		MARRY_ZHUHE_TYPE1,				// �ͻ�
	};

	// 6014 ף������
	class CSMarryZhuheSend
	{
	public:
		CSMarryZhuheSend();
		MessageHeader		header;

		int uid;
		int type;
	};

	// 6015 �����յ�ף��
	class SCMarryZhuheShou
	{
	public:
		SCMarryZhuheShou();
		MessageHeader		header;
		
		int uid;
		GameName name;
		int type;
	};

	//-------------------------------- ��Եʥ�� -----------------------------------------

	enum QYSD_OPERA_TYPE
	{
		QYSD_OPERA_TYPE_FETCH_TASK_REWARD = 0,			// ��ȡ������
		QYSD_OPERA_TYPE_FETCH_OTHER_REWARD,				// ��ȡ���⽱��
	};

	// ��Եʥ�ز������� 6020
	class CSQingYuanShengDiOperaReq
	{
	public:
		CSQingYuanShengDiOperaReq();
		MessageHeader		header;

		int opera_type;
		int param;
	};

	// ��Եʥ��������Ϣ 6021
	class SCQingYuanShengDiTaskInfo
	{
	public:
		SCQingYuanShengDiTaskInfo();
		MessageHeader		header;

		char is_fetched_task_other_reward;
		char lover_is_all_task_complete;
		short task_count;
		QingYuanShengDiTask task_info_list[QING_YUAN_SHENG_DI_TASK_MAX_COUNT];
	};

	// ��Եʥ��boss��Ϣ 6022
	static const int QING_YUAN_SHENG_DI_BOSS_COUNT_MAX = 20;
	class SCQingYuanShengDiBossInfo
	{
	public:
		SCQingYuanShengDiBossInfo();
		MessageHeader		header;

		struct BossInfo
		{
			int boss_id;
			unsigned short pos_x;
			unsigned short pos_y;
			unsigned int next_refresh_time;
		};

		int boss_count;
		BossInfo boss_list[QING_YUAN_SHENG_DI_BOSS_COUNT_MAX];
	};

	//-------------------------------- ��Ե����-----------------------------------------

	enum QINGYUAN_FB_OPERA_TYPE
	{
		QINGYUAN_FB_OPERA_TYPE_BASE_INFO = 0,		  // ���󸱱�������Ϣ
		QINGYUAN_FB_OPERA_TYPE_BUY_TIMES,			  // ����������
		QINGYUAN_FB_OPERA_TYPE_BUY_BUFF,			  // ����buff
		QINGYUAN_FB_OPERA_TYPE_BUY_DOUBLE_REWARD,	  // ����˫������
	};

	class CSQingyuanFBOperaReq // 5469 ��Ե������������
	{
	public:
		CSQingyuanFBOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
	};

	class SCQingyuanBaseFBInfo		// 5425 ����������Ϣ����
	{
	public:
		SCQingyuanBaseFBInfo();
		MessageHeader header;

		char join_fb_times;
		char buy_fb_join_times;
		char is_hunyan_already_open;
		char reserve_ch;
		int qingyuan_value;
		int yanhui_fb_key;
	};

	class SCQingyuanFBInfo		// 5426 ����������Ϣ
	{
	public:
		SCQingyuanFBInfo();
		MessageHeader header;

		char curr_wave;					// ��ǰ����
		char max_wave_count;			// �����
		char is_pass;					// �Ƿ�ͨ��
		char is_finish;					// �Ƿ����
		int next_refresh_monster_time;	// �´�ˢ��ʱ��
		short add_qingyuan_value;
		short buy_buff_times;			// 
		int buff_failure_timestamp;
		int per_wave_remain_time;		// ÿ��ʣ��ʱ��
		int total_get_uplevel_stuffs;
		long long total_get_exps;
		int kick_out_timestamp;			// �߳�����ʱ��
		char male_is_buy_double_reward;	// �з��Ƿ���˫������
		char female_is_buy_double_reward; // Ů���Ƿ���˫������
		short reserve_sh;					
	};

	class SCQingyuanFBRewardRecordInfo				//	5442 ����������¼
	{
	public:
		SCQingyuanFBRewardRecordInfo();
		MessageHeader header;

		static const int MAX_ITEM_COUNT = 4;
		struct ItemInfo
		{
			ItemInfo() : reward_item_id(0), reward_item_num(0) {}
			ItemID reward_item_id;
			short reward_item_num;
		};

		int item_count;
		ItemInfo item_list[MAX_ITEM_COUNT];
	};

	//-------------------------------- ��Եװ��-----------------------------------------
	class CSHunjieUpLevel
	{
	public:
		CSHunjieUpLevel();
		MessageHeader header;

		ItemID stuff_id;
		short repeat_times;
		int is_auto_buy;
	};

	class CSHunjieReqEquipInfo
	{
	public:
		CSHunjieReqEquipInfo();
		MessageHeader header;
	};

	class CSQingyuanTakeOffEquip
	{
	public:
		CSQingyuanTakeOffEquip();
		MessageHeader header;
	};

	class SCHunjieEquipInfo
	{
	public:
		SCHunjieEquipInfo();
		MessageHeader header;

		int consume_num;
		int baoji_num;
		int exp;
		int star;

		int lover_level;
		ItemID lover_hunjie_item_id;
		ItemID hunjie_item_id;
		short lover_hunjie_star_level;
		short lover_prof;
	};

	class SCHunjieEquipUpLevelSendLover
	{
	public:
		SCHunjieEquipUpLevelSendLover();
		MessageHeader header;

		int level;
		ItemID hunjie_item_id;
		short hunjie_star_level;
	};

	class SCQingyuanBlessInfo
	{
	public:
		SCQingyuanBlessInfo();
		MessageHeader header;

		int is_fetch_bless_reward;
		int bless_days;
		int lover_bless_days;
	};


	class CSQingyuanQueryMateQingyuanValue
	{
	public:
		CSQingyuanQueryMateQingyuanValue();
		MessageHeader header;
	};

	class SCQingyuanMateQingyuanValue
	{
	public:
		SCQingyuanMateQingyuanValue();
		MessageHeader header;

		int mate_qingyuan_value;
	};

	class SCQingyuanCardLevelList											// ��Ե���Ƶȼ��б�  4724
	{
	public:
		SCQingyuanCardLevelList();
		MessageHeader header;

		char card_level_list[QINGYUAN_CARD_MAX_ID + 1];
	};

	class SCQingyuanCardUpdate												// ��Ե���Ƶȼ����֪ͨ 4725
	{
	public:
		SCQingyuanCardUpdate();
		MessageHeader header;

		char card_id;
		char card_level;
		short reserve_sh;
	};

	class CSQingyuanCardUpgradeReq											// ����������Ե���� 4780
	{
	public:
		CSQingyuanCardUpgradeReq();
		MessageHeader header;

		char card_id;
		char reserve_ch;
		short reserve_sh;
	};

	class CSQingyuanFetchBlessRewardReq										// ��ȡ��Եף������
	{
	public:
		CSQingyuanFetchBlessRewardReq();
		MessageHeader header;
	};

	class CSQingyuanAddBlessDaysReq										// ����ף������
	{
	public:
		CSQingyuanAddBlessDaysReq();
		MessageHeader header;
	};

	enum LOVE_CONTRACT_REQ_TYPE
	{
		LC_REQ_TYPE_INFO = 0,					// ��Ϣ����
		LC_REQ_TYPE_BUY_LOVE_CONTRACT,		    // ��������Լ
		LC_REQ_TYPE_NOTICE_LOVER_BUY_CONTRACT,   // ���ѶԷ���������Լ
	};

	class CSLoveContractOperaReq			// 4792 ��������Լ
	{
	public:
		CSLoveContractOperaReq();
		MessageHeader header;

		short opera_type;
		short param1;
	};

	enum LOVE_CONTRACT_NOTICE_TYPE
	{
		LC_NOTICE_TYPE_FETCH = 0,					// ��ȡ������Լ����
		LC_NOTICE_TYPE_NOTICE,						// ������Լ����
	};

	class CSQingyuanFetchLoveContract		// 4793 ��ȡ������Լ����
	{
	public:
		CSQingyuanFetchLoveContract();
		MessageHeader header;

		short opera_type;
		short day_num;
		LoveContractNotice love_contract_notice;
	};

	struct ContractNoticePack
	{
		unsigned int time;
		LoveContractNotice contract_notice;
	};

	enum LOVE_CONTRACT_INFO_TYPE
	{
		LOVE_CONTRACT_INFO_TYPE_NORMAL = 0,					// һ��
		LOVE_CONTRACT_INFO_TYPE_NOTICE_BUY_CONTRACT,	  // ���ѶԷ�������Լ
	};

	class SCQingyuanLoveContractInfo		// 4794 ������Լ��Ϣ
	{
	public:
		SCQingyuanLoveContractInfo();
		MessageHeader header;

		int info_type;									// ��Ϣ����
		int self_love_contract_reward_flag;
		int can_receive_day_num;						// -1 ��ʾ��������
		unsigned int lover_love_contract_timestamp;		// 0 ��ʾû�а���¹���

		int self_notice_list_count;
		ContractNoticePack self_notice_list[MAX_NOTICE_COUNT];

		int lover_notice_list_count;
		ContractNoticePack lover_notice_list[MAX_NOTICE_COUNT];
	};

	//-------------------------------- ��Ե�⻷-----------------------------------------

	enum QINGYUAN_COUPLE_HALO_REQ_TYPE
	{
		QINGYUAN_COUPLE_REQ_TYPE_INFO = 0,				// ������Ϣ
		QINGYUAN_COUPLE_REQ_TYPE_USE,					// װ���⻷ param_1�⻷����
		QINGYUAN_COUPLE_REQ_TYPE_UP_LEVEL,				// �⻷���� param_1�⻷���� param_2 �Ƿ��Զ�����
		QINGYUAN_COUPLE_REQ_TYPE_BUY_HALO,				// �⻷���� param_1 �⻷����

		QINGYUAN_COUPLE_REQ_TYPE_MAX,
	};

	class CSQingyuanCoupleHaloOperaReq								// 4785
	{
	public:
		CSQingyuanCoupleHaloOperaReq();
		MessageHeader header;

		unsigned short req_type;
		unsigned short param_1;
		unsigned short param_2;
		unsigned short param_3;
	};

	class SCQingyuanCoupleHaloInfo										// 4783
	{
	public:
		SCQingyuanCoupleHaloInfo();
		MessageHeader header;

		char equiped_couple_halo_type;														// ��װ���ķ��޹⻷
		char  cur_tejia_halo_type;															// ��ǰ�ؼ۹⻷����
		short couple_halo_max_type_count;													// ���޹⻷������
		unsigned int tejie_halo_invalid_time;												// ��ǰ�⻷�ؼ�ʧЧʱ��
		short couple_halo_level_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];						// ���޹⻷�ȼ�
		short couple_halo_exp_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];							// ���޹⻷����
	};

	class SCQingyuanCoupleHaloTrigger									// 4784
	{
	public:
		SCQingyuanCoupleHaloTrigger();
		MessageHeader header;

		int role1_uid;
		int role2_uid;

		int halo_type;
	};

	//-------------------------------- ��Եȫ��-----------------------------------------
	enum QINGYUAN_EQUIP_REQ_TYPE
	{
		QINGYUAN_EQUIP_REQ_SELF_EQUIP_INFO = 0,			// �����Լ�װ����Ϣ
		QINGYUAN_EQUIP_REQ_OTHER_EQUIP_INFO,			// �������װ����Ϣ
		QINGYUAN_EQUIP_REQ_ACTIVE_SUIT,					// ���󼤻���װ param_1��װ���ͣ�param_2 ��װ��, param_3��������
		QINGYUAN_EQUIP_REQ_TAKE_OFF,					// ������װ��, param_1 װ������

		QINGYUAN_EQUIP_REQ_ACTIVE_SPECIAL_BABY,			// ���󼤻����ⱦ�� param_1 ���� param_2 Ʒ��

		QINGYUAN_EQUIP_REQ_EQUIP2_UPLEVEL = 10,			// ����������Ƚ��װ�� param_1 װ����λ
		QINGYUAN_EQUIP_REQ_BUY_GIFT,					// ������� param_1 ������

		QINGYUAN_EQUIP_REQ_TYPE_MAX,
	};

	// ��Եȫ�ֲ��� 8392
	class CSQingyuanEquipOperate
	{
	public:
		CSQingyuanEquipOperate();
		MessageHeader header;

		int operate_type;
		int param_1;
		int param_2;
		int param_3;
	};

	// ��Եȫ����Ϣ 8394
	class SCQingyuanEquipInfo
	{
	public:
		SCQingyuanEquipInfo();
		MessageHeader header;

		int is_self;    // �Լ�1 ���� 0
		int marry_level;
		int marry_level_exp;
		unsigned char qingyuan_suit_flag[QINGYUAN_EQUIPSUIT_MAX_COUNT];		// ��Ե��װ���
		char buy_gift_flag;													// ���չ���������
		char reserve;
		ItemDataWrapper qingyuan_equip_list[QINGYUAN_EQUIP_TYPE_MAX];		// ��Եװ����Ϣ
		QingyuanEquip2Item qingyuan_equip2_list[QINGYUAN_EQUIP_TYPE_MAX];	// ��Ƚ��װ����Ϣ
		QingyuanSpecialBaby special_baby_list[SPECIAL_BABY_TYPE_MAX];		// ���ⱦ����Ϣ
	};

	//////////////////////////////// Լ�� //////////////////////////////////////////
	class CSDatingInviteReq
	{
	public:
		CSDatingInviteReq();
		MessageHeader		header;

		int					target_uid;
		int					is_specific_invite;
	};

	class CSDatingInviteAck
	{
	public:
		CSDatingInviteAck();
		MessageHeader	header;

		int				req_uid;
		int				is_agree;
	};

	//////////////////////////////////// ����BOSS /////////////////////////////////
	class CSGetWorldBossInfo					// 10151��������boss��Ϣ
	{
	public:
		CSGetWorldBossInfo();
		MessageHeader	header;

	};

	class SCWorldBossInfoToAll						// 10100 ��������boss�㲥��Ϣ
	{
	public:
		SCWorldBossInfoToAll();
		MessageHeader header;

		int boss_id;
		int scene_id;
		int status;
		unsigned int next_refresh_time;
		int killer_uid;
	};

	class SCWorldBossInfoList						// 10101 �·�����boss��Ϣ
	{
	public:
		SCWorldBossInfoList();
		MessageHeader	header;

		unsigned int next_refresh_time;
		static const int MAX_BOSS_COUNT = 100;
		int count;

		struct BossInfo
		{
			int boss_id;
			int status;

			WorldBossManager::KillerInfo killer_info_list[WORLD_KILLER_LIST_MAX_COUNT];
		};

		BossInfo boss_list[MAX_BOSS_COUNT];
	};

	class SCWorldBossBorn						// 10102����boss����
	{
	public:
		SCWorldBossBorn();
		MessageHeader	header;
	};

	class CSWorldBossPersonalHurtInfoReq				// 10152����boss�����˺���������
	{
	public:
		CSWorldBossPersonalHurtInfoReq();
		MessageHeader header;

		int boss_id;
	};

	class SCWorldBossSendPersonalHurtInfo				// 10103��������boss�����˺�������Ϣ
	{
	public:
		SCWorldBossSendPersonalHurtInfo();
		MessageHeader header;

		long long my_hurt;
		int self_rank;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				uid = 0;
				memset(name, 0, sizeof(name));
				hurt = 0;
			}


			int uid;
			GameName name;
			long long hurt;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class CSWorldBossGuildHurtInfoReq					// 10153����boss�����˺���������
	{
	public:
		CSWorldBossGuildHurtInfoReq();
		MessageHeader header;

		int boss_id;
	};

	class SCWorldBossSendGuildHurtInfo					// 10104��������boss�����˺�������Ϣ
	{
	public:
		SCWorldBossSendGuildHurtInfo();
		MessageHeader header;

		long long my_guild_hurt;
		int my_guild_rank;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				guild_id = 0;
				memset(guild_name, 0, sizeof(guild_name));
				guild_hurt = 0;
			}


			GuildID guild_id;
			GuildName guild_name;
			long long guild_hurt;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};

	class CSWorldBossWeekRankInfoReq			// 10154����boss��ɱ�����ܰ���������
	{
	public:
		CSWorldBossWeekRankInfoReq();
		MessageHeader header;

	};

	class SCWorldBossWeekRankInfo				// 10105��������boss��ɱ�����ܰ�������Ϣ
	{
	public:
		SCWorldBossWeekRankInfo();
		MessageHeader header;

		short my_guild_kill_count;
		short my_guild_rank;

		struct RankItem
		{
			RankItem()
			{
				this->Reset();
			}

			void Reset()
			{
				guild_id = 0;
				memset(guild_name, 0, sizeof(guild_name));
				guild_kill_count = 0;
			}


			GuildID guild_id;
			GuildName guild_name;
			int guild_kill_count;
		};

		static const int RANK_NUM = 100;
		int rank_count;
		RankItem rank_list[RANK_NUM];
	};
	
	class SCWorldBossCanRoll				// 10106����boss���ܱ����ƣ�֪ͨ��ҿ�ҡ��
	{
	public:
		SCWorldBossCanRoll();
		MessageHeader header;

		int boss_id;
		int hudun_index;					// ��һ�λ���1���ڶ��λ���2
	};

	class CSWorldBossKillerInfoReq				// 10150 ��������boss��ɱ�б�
	{
	public:
		CSWorldBossKillerInfoReq();
		MessageHeader header;

		int boss_id;
	};

	class SCWorldBossKillerList						// 10109 ����boss��ɱ�б�
	{
	public:
		SCWorldBossKillerList();
		MessageHeader header;

		WorldBossManager::KillerInfo killer_info_list[WORLD_KILLER_LIST_MAX_COUNT];
	};

	class CSWorldBossRollReq				// 10155�������ҡ��
	{
	public:
		CSWorldBossRollReq();
		MessageHeader header;

		int boss_id;
		int hudun_index;
	};

	class SCWorldBossRollInfo				// 10107�������ҡ�����
	{
	public:
		SCWorldBossRollInfo();
		MessageHeader header;

		int roll_point;
		int hudun_index;
	};

	class SCWorldBossRollTopPointInfo     // 10108������ߵ���Ϣ
	{
	public:
		SCWorldBossRollTopPointInfo();
		MessageHeader header;

		int boss_id;
		int hudun_index;
		int top_roll_point;
		GameName top_roll_name;
	};

	class SCWorldBossHPInfo				 // 10111����bossѪ����Ϣ
	{
	public:
		SCWorldBossHPInfo();
		MessageHeader header;

		int boss_id;
		long long cur_hp;
		long long max_hp;
		int shield_hp;
	};

	//////////////////////////////////////////////////////////////////////////
	class SCActivityFindInfo				// 6005 �����һ���Ϣ�·�
	{
	public:
		SCActivityFindInfo();
		MessageHeader header;

		enum  
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_REQ_INFO,
			NOTIFY_REASON_FIND_REQ,
			NOTIFY_REASON_AUTO_ACTIVITY,
			NOTIFY_REASON_JOIN_ACTIVITY,
		};

		int activity_open_flag;				// �ѿ������
		int activity_reward_flag;			// �ѽ������
		int activity_day_enter_flag;		// �ѽ�����
		int notify_reason;					// ֪ͨԭ��
	};

	//////////////////////////////////////////////////////////////////////////
	class CSSupplyBuyItem				// 2772��������
	{
	public:
		CSSupplyBuyItem();
		MessageHeader header;

		int supply_type;
		int index;

		char is_use_no_bind_gold;
		char reserver_1;
		short reserve_2;
	};

	class CSSupplySetRecoverRangePer	// 2773���ò����ٷֱ�
	{
	public:
		CSSupplySetRecoverRangePer();
		MessageHeader header;

		int supply_type;
		int recover_range_per;
	};

	class SCSupplyInfo					// 2726�·�������Ϣ
	{
	public:
		SCSupplyInfo();
		MessageHeader header;

		struct SupplyItem
		{
			long long supply_left_value;
			int supply_range_per;
		};

		SupplyItem supply_info_list[SUPPLY_TYPE_MAX];
	};

	class CSFindRoleByName
	{
	public:
		CSFindRoleByName();
		MessageHeader header;

		GameName name;
		int msg_identify;			// ��ǣ����ͻ����ã�
	};

	class SCFindRoleByNameRet
	{
	public:
		SCFindRoleByNameRet();
		MessageHeader header;

		long long uuserverid;
		long long uuid;
		int role_id;
		GameName role_name;
		char is_online;
		char sex;
		char prof;
		char camp;
		int level;
		int msg_identify;			// ��ǣ����ͻ����ã�
		long long avatar_timestamp;
		char at_cross;
		char reserve;
		short reserve_s;
		int capability;
	};

	//////////////////////////////////////////////////////////////////////////

// 	class SCWingInfo		// 4703
// 	{
// 	public:
// 		SCWingInfo();
// 		MessageHeader header;
// 
// 		short select_jinhua_img;				// ѡ��������Ӧ�Ľ���ֵ
// 		short jinhua;							// ����ֵ
// 		int jinhua_bless;						// ����ף��ֵ	
// 		short special_img_flag;					// ����������
// 		char select_special_img;				// ѡ�е���������
// 		char reserve_ch;	
// 		short grade;							// �׼�
// 		short use_grade;						// ʹ�ý׼�
// 		int grade_bless_val;					// �׼�ף��ֵ
// 
// 		int shuxingdan_list[WingParam::SHUXINGDAN_MAX_TYPE];			// ���Ե�
// 		char special_img_grade_list[WingParam::MAX_SPECIAL_IMG_COUNT];	// �����������
// 		short fumo_level_list[WingParam::MAX_WING_FUMO_TYPE];
// 	};
// 
// 	class SCWingGrowExpType		// 4705
// 	{
// 	public:
// 		SCWingGrowExpType();
// 		MessageHeader header;
// 
// 		enum WGET
// 		{
// 			WGET_NORMAL = 0,
// 			WGET_BAOJI = 1,
// 		};
// 
// 		short grow_type;
// 		short reserve;
// 		int increase_point;
// 	};
// 
// 	class CSWingPromoteJinghua	// 4757
// 	{
// 	public:
// 		CSWingPromoteJinghua();
// 		MessageHeader header;
// 
// 		short is_auto_buy;						// 1 �Զ�����0 ��
// 		short repeat_times;
// 	};
// 
// 	class CSWingUseImage		// 4758
// 	{
// 	public:
// 		CSWingUseImage();
// 		MessageHeader header;
// 
// 		short img_id;						// ����id
// 		short is_special_img;				// �Ƿ���������
// 	};
// 
// 	class CSWingUpgrade
// 	{
// 	public:
// 		CSWingUpgrade();				
// 		MessageHeader header;
// 
// 		short repeat_times;
// 		short auto_buy;
// 	};
// 
// 	class CSWingUseGrade
// 	{
// 	public:
// 		CSWingUseGrade();				
// 		MessageHeader header;
// 
// 		int grade;
// 	};
// 
// 	class CSWingSpecialUpgrade
// 	{
// 	public:
// 		CSWingSpecialUpgrade();
// 		MessageHeader header;
// 
// 		int img_id;
// 	};
// 
// 	class CSWingFumoUplevel		// 4787
// 	{
// 	public:
// 		CSWingFumoUplevel();
// 		MessageHeader header;
// 
// 		int type;
// 		int is_auto_buy;
// 	};

	//////////////////////////////////////////////////////////////////////////
	class CSDiscountBuyGetInfo			// 4760 ���һ��������Ϣ
	{
	public:
		CSDiscountBuyGetInfo();
		MessageHeader header;
	};

	class CSDiscountBuyReqBuy			// 4761 һ������ ����
	{
	public:
		CSDiscountBuyReqBuy();
		MessageHeader header;

		short seq;
		short reserve_sh;
	};

	class SCDiscountBuyInfo				// 4706 һ��������Ϣ
	{
	public:
		SCDiscountBuyInfo();
		MessageHeader header;

		struct Phase
		{
			unsigned int close_timestamp;
			short buy_count_list[DISCOUNT_BUY_ITEM_PER_PHASE];
		};

		Phase phase_list[DISCOUNT_BUY_PHASE_MAX_COUNT];
	};

	//////////////////////////////////////////////////////////////////////////
	class CSZeroGiftOperate				// 4797��Ԫ�������
	{
	public:
		CSZeroGiftOperate();
		MessageHeader header;

		enum ZERO_GIFT_OPERATE_TYPE
		{
			ZERO_GIFT_GET_INFO = 0,
			ZERO_GIFT_BUY,
			ZERO_GIFT_FETCH_REWARD_ITEM,     //����1 seq   ����2 ����
			ZERO_GIFT_BUY_GOD_COSTUME,					//�����Ľ���װ
			ZERO_GIFT_FETCH_GOD_COSTUME_REWARD_ITEM,     //��ȡ�Ľ���װ����������1 seq   ����2 �ȼ��׶�
		};

		int operate_type;
		int param_1;
		int param_2;
	};

	class SCZeroGiftInfo				//  4796 ��Ԫ�����Ϣ
	{
	public:
		SCZeroGiftInfo();
		MessageHeader header;

		struct Phase
		{
			int state;
			unsigned int timestamp;
			int reward_flag;
		};

		Phase phase_list[ZERO_GIFT_PHASE_MAX];
	};

	//////////////////////////////////////////////////////////////////////////
	class CSAdvanceNoitceOperate				// 4798����Ԥ�����
	{
	public:
		CSAdvanceNoitceOperate();
		MessageHeader header;

		enum ADVANCE_NOTICE_OPERATE_TYPE
		{
			ADVANCE_NOTICE_GET_INFO = 0,
			ADVANCE_NOTICE_FETCH_REWARD,

			ADVANCE_NOTICE_DAY_GET_INFO = 2,		// ����Ԥ��
			ADVANCE_NOTICE_DAY_FETCH_REWARD,		// ��ȡ����
		};

		int operate_type;
		int param_1;
	};

	class SCAdvanceNoticeInfo				//  4799 ����Ԥ����Ϣ��Ϣ
	{
	public:
		SCAdvanceNoticeInfo();
		MessageHeader header;

		enum ADVANCE_NOTICE_TYPE
		{
			ADVANCE_NOTICE_TYPE_LEVEL,				// �ȼ�
			ADVANCE_NOTICE_TYPE_DAY,				// ����
		};

		int notice_type;							// ֪ͨ����
		int last_fecth_id;							// �ϴ���ȡ����id
		unsigned char notice_day_fectch_flag_list[CommonDataParam::NOTICE_DAY_MAX_NUM]; // ����Ԥ����ȡ���
	};

	//////////////////////////////////////////////////////////////////////////
	class CSGoldTurntableOperateReq				// 8425 Ԫ��ת�̲�������
	{
	public:
		CSGoldTurntableOperateReq();
		MessageHeader header;

		enum
		{
			INFO_REQ = 0,						// ������Ϣ����
			CHOU_JIANG = 1,						// �齱
			
		};

		int operate_type;
	};

	class SCGoldTurntableReward					// 8426 Ԫ��ת�� ��Ʒ��Ϣ
	{
	public:
		SCGoldTurntableReward();
		MessageHeader header;

		int index;
	};

	class SCGoldTurntableInfo					//8427 Ԫ��������
	{
	public:
		SCGoldTurntableInfo();
		MessageHeader header;

		long long gold_num;
		int chou_jiang_times;
	};

	//////////////////////////////////////////////////////////////////////////
	class SCMonthCardInfo
	{
	public:
		SCMonthCardInfo();
		MessageHeader header;

		unsigned int active_timestamp;
		int reserve_int;
		int next_reward_day_idx;
		int buy_times;
	};

	class CSMonthCardFetchDayReward 
	{
		CSMonthCardFetchDayReward();
		MessageHeader header;
	};

	static const int MAX_RANDOM_ROLE_LIST_NUM = 8;

	struct RoleInfo
	{
		int uid;
		GameName role_name;
		char avatar;
		char sex;
		char prof;
		char camp;
		int capability;
		int level;
		long long avatar_timestamp;
	};

	class SCRandomRoleListRet
	{
	public:
		SCRandomRoleListRet();
		MessageHeader header;

		int count;
		RoleInfo role_list[MAX_RANDOM_ROLE_LIST_NUM];
	};

	class CSGetRandomRoleList
	{
	public:
		CSGetRandomRoleList();
		MessageHeader header;
	};

	class CSTurnTableReq							// 5257����ת��ת��
	{
	public:
		CSTurnTableReq();
		MessageHeader header;

		char type;
		char is_roll;
		short reserve_sh;
	};

	class SCTurnTableInfo							// 5205ת�̻�����Ϣ
	{
	public:
		SCTurnTableInfo();
		MessageHeader header;

		short lucky_point[TURNTABLE_TYPE_MAX_COUNT];
	};

	class SCTurnTableReward							// 5204ת�̽����·�
	{
	public:
		SCTurnTableReward();
		MessageHeader header;

		char type;
		char rewards_index;
		short reserve_sh;
	};

	class SCTurnTableMillionaireView				// 5201�󸻺�����㲥
	{
	public:
		SCTurnTableMillionaireView();
		MessageHeader header;

		ObjID obj_id;
		short is_millionaire;

	};

	//////////////////////////////////////////////////////////////////////////
	class SCWabaoInfo
	{
	public:
		SCWabaoInfo();
		MessageHeader header;

		int baozang_scene_id;
		int baozang_pos_x;
		int baozang_pos_y;
		int baotu_count;				// ��ͼ��Ŀ
		int wabao_reward_type;			// ��������
		int wabao_reward_count;
		unsigned int shouhuzhe_time;
		short wabao_reward_list[WabaoBaozangRewardConfig::MAX_REWARD_LIMIT];
		int is_quick_complete;
	};

	class CSWabaoOperaReq
	{
	public:
		enum OPERA_TYPE
		{
			OPERA_TYPE_START = 0,
			OPERA_TYPE_DIG = 1,
			OPERA_TYPE_QUICK_COMPLETE = 2, 
			OPERA_TYPE_GET_INFO = 3,
			OPERA_TYPE_SHOUHUZHE_TIME = 4,

			OPERA_TYPE_MAX,
		};

		CSWabaoOperaReq();
		MessageHeader header;

		short opera_type;
		short is_killed;
	};

	//////////////////////////////////////////////////////////////////////////
	class CSJilianOpera
	{
	public:
		enum OPERA_TYPE
		{
			OPERA_TYPE_JILIAN,
			OPERA_TYPE_CONTINUE,

			OPERA_TYPE_MAX,
		};

		CSJilianOpera();
		MessageHeader header;

		short opera_type;
		short param;
	};

	class SCJilianInfo
	{
	public:
		SCJilianInfo();
		MessageHeader header;

		char jilian_type;
		char is_working;
		short progress;
	};

	class SCJianlianViewChange
	{
	public:
		SCJianlianViewChange();
		MessageHeader header;

		ObjID obj_id;
		char jilian_type;
		char reserve_ch;
	};

//////////////////////////////////////////////////////////////////////////////////

	class SCZhanShenDianFBInfo
	{
	public:
		SCZhanShenDianFBInfo();
		MessageHeader header;

		short pass_level;
		short yesterday_pass_level;
		unsigned int fetch_day_reward_timestamp;
	};

	class SCZhanShenDianFBResult
	{
	public:
		SCZhanShenDianFBResult();
		MessageHeader header;

		char is_pass;
		char reseve_ch;
		unsigned short time_cost;
	};

	class SCTiangongShilianFBResult
	{
	public:
		SCTiangongShilianFBResult();
		MessageHeader header;

		char is_pass;
		char reseve_ch;
		unsigned short time_cost;
	};

	class SCTiangongShilianFBInfo
	{
	public:
		SCTiangongShilianFBInfo();
		MessageHeader header;

		int pass_level;
	};

	class CSFBReqNextLevel
	{
	public:
		CSFBReqNextLevel();
		MessageHeader header;
	};

	class SCRollMoneyInfo
	{
	public:
		SCRollMoneyInfo();
		MessageHeader header;

		short fetch_gold_reward_times;
		short fetch_coin_reward_times;

		char day_roll_one_gold_times;
		char reserve_ch;
		short reserve_sh;

		char gold_roll_times;
		char gold_roll_num_list[ROLL_MONEY_GOLD_ROLL_NUM_MAX_COUNT];
		

		char coin_roll_times;
		char coin_roll_num_list[ROLL_MONEY_COIN_ROLL_NUM_MAX_COUNT];
	};

	enum ROLL_MONEY_OPERA_TYPE
	{
		ROLL_MONEY_OPERA_TYPE_QUERY = 0,
		ROLL_MONEY_OPERA_TYPE_ROLL_GOLD = 1,
		ROLL_MONEY_OPERA_TYPE_ROLL_COIN = 2,
		ROLL_MONEY_OPERA_TYPE_FETCH_GOLD_REWARD = 3,
		ROLL_MONEY_OPERA_TYPE_FETCH_COIN_REWARD = 4,	

		ROLL_MONEY_OPERA_TYPE_MAX, 
	};

	class CSRollMoneyOperaReq
	{
	public:
		CSRollMoneyOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
	};

	class CSZhanshendianFetchDayReward
	{
	public:
		CSZhanshendianFetchDayReward();
		MessageHeader header;
	};

	class CSRoleGoalOperaReq
	{
	public:

		enum OPERA_TYPE
		{
			PERSONAL_GOAL_INFO_REQ = 0,
			FETCH_PERSONAL_GOAL_REWARD_REQ = 1,
			FETCH_BATTLE_FIELD_GOAL_REWARD_REQ = 2,
			FINISH_GOLE_REQ = 3,
			UPLEVEL_SKILL = 4,
		};

		CSRoleGoalOperaReq();
		MessageHeader header;

		int opera_type;
		int param;
	};

	class SCRoleGoalInfo
	{
	public:
		SCRoleGoalInfo();
		MessageHeader header;

		int cur_chapter;
		int old_chapter;
		int data_list[PERSONAL_GOAL_COND_MAX];

		int field_goal_can_fetch_flag;
		int field_goal_fetch_flag;
		short skill_level_list[FIELD_GOAL_SKILL_TYPE_MAX];
	};

	class SCRoleGoalChange
	{
	public:
		SCRoleGoalChange();
		MessageHeader header;

		int gold_id;
		int param;
	};

	class CSRedPaperCreateReq
	{
	public:
		CSRedPaperCreateReq();
		MessageHeader header;

		int type;
		int gold_num;
		int can_fetch_times;
	};

	class CSRedPaperFetchReq
	{
	public:
		CSRedPaperFetchReq();
		MessageHeader header;

		int red_paper_id;
	};

	class CSRedPaperQueryDetailReq
	{
	public:
		CSRedPaperQueryDetailReq();
		MessageHeader header;

		int red_paper_id;
	};

	class SCRedPaperRoleInfo
	{
	public:
		SCRedPaperRoleInfo();
		MessageHeader header;

		int day_guild_red_paper_consume_gold_num;
	};

	class SCRedPaperDetailInfo
	{
	public:
		SCRedPaperDetailInfo();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_DEFAULT = 0,
			NOTIFY_REASON_CREATE = 1,

			NOTIFY_REASON_MAX,
		};

		struct FetchUserInfo
		{
			int uid;
			int gold_num;
			long long avator_timestamp;
			GameName fetch_name;
		};

		int notify_reason;

		int id;
		int type;
		int total_gold_num;
		int fetch_gold_num;
		int can_fetch_times;
		unsigned int timeount_timestamp;

		int creater_uid;
		GameName creater_name;
		int creater_guild_id;
		long long creater_avator_timestamp;
		char creater_sex;
		char creater_prof;
		short reserve;
		int boss_id;

		int fetch_user_count;
		FetchUserInfo fetch_user_info_list[RED_PAPER_MAX_CAN_FETCH_TIMES];
	};

	class SCRedPaperFetchResult
	{
	public:
		SCRedPaperFetchResult();
		MessageHeader header;

		enum NOTIFY_REASON
		{
			NOTIFY_REASON_FETCH_SUCC = 0,
			NOTIFY_REASON_HAS_FETCH,
			NOTIFY_REASON_NO_FETCH_TIMES,
			NOTIFY_REASON_FAIL,

			NOTIFY_REASON_MAX,
		};

		int notify_reason;
		int fetch_gold;
		GameName creater_name;
		int red_paper_type;
		int red_paper_id;
	};

	class CSCreateCommandRedPaper				//4727
	{
	public:
		CSCreateCommandRedPaper();
		MessageHeader header;

		int msg_length;							//��Ϣ����
		//���������Ϣ����
	};

	class CSFetchCommandRedPaper				//4728
	{
	public:
		CSFetchCommandRedPaper();
		MessageHeader header;

		int red_paper_id;						//���id
	};

	class CSCommandRedPaperCheckInfo			//4729
	{
	public:
		CSCommandRedPaperCheckInfo();
		MessageHeader header;

		int red_paper_id;						//���id
	};

	class SCCommandRedPaperSendInfo				//4730
	{
	public:
		SCCommandRedPaperSendInfo();
		MessageHeader header;

		int creater_uid;
		GameName creater_name;
		long long creater_avator_timestamp;
		int msg_len;							//��Ϣ����
		char creater_sex;
		char creater_prof;
		short reserve;
		int red_paper_id;
		//���������Ϣ
	};

	struct MarriageSeekingInfo
	{
		GameName			gamename;
		int					user_id;
		char				camp;
		char				sex;
		char				prof;
		char				is_online;
		short				level;
		short				reserve_sh;
		unsigned int		timestamp;
		MarriageSeekingNotice marriage_seeking_notice;
	};

	class SCMarriageSeekingInfo
	{
	public:
		SCMarriageSeekingInfo();							// 4715
		MessageHeader header;
		
		int count;
		MarriageSeekingInfo	marriage_seeking_list[MARRIAGE_SEEKING_MAX_COUNT];
	};

	class CSMarriageSeekingOpera
	{
	public:
		CSMarriageSeekingOpera();							// 4768
		MessageHeader header;

		short type;
		short reserve_sh;
		MarriageSeekingNotice marriage_seeking_notice;
	};

	class SCFakePrivateChat
	{
	public:
		SCFakePrivateChat();
		MessageHeader header;

		int	from_uid;
		UserName username;		
		
		char sex;
		char camp;
		char vip_level;
		char prof;
		short level;
		short reserve_ch;
		long long avatar_timestamp;
	};

	enum LINGYUSHUISHOU_OPERA_TYPE
	{
		LINGYUSHUISHOU_OPERA_TYPE_INVALID, 
		LINGYUSHUISHOU_OPERA_TYPE_FETCH_XMZ_REWARD,
		LINGYUSHUISHOU_OPERA_TYPE_FETCH_GCZ_REWARD,
		LINGYUSHUISHOU_OPERA_TYPE_MOBAI,

		LINGYUSHUISHOU_OPERA_TYPE_MAX,
	};

	class CSShizhuangUseReq
	{
	public:
		CSShizhuangUseReq();
		MessageHeader header;

		char shizhuang_type;	// 0-������1-ʱװ
		char img_type;			// 0-��ͨ����1-��������
		short img_id;			// ����ID
	};

	class CSShizhuangUpgradeReq
	{
	public:
		CSShizhuangUpgradeReq();
		MessageHeader header;
	
		short shizhuang_type;
		char is_auto_buy;								// �Ƿ��Զ����������Ʒ
		char repeat_times;								// ��ǰ�������������״���
	};

	class CSShizhuangSpecialImgUpgradeReq				// ��������ʱװ��������  4809
	{
	public:
		CSShizhuangSpecialImgUpgradeReq();
		MessageHeader header;

		short shizhuang_type;
		char special_img_id;							// ��������ID
		char reserve_ch;
	};

	class SCShizhuangInfo
	{
	public:
		SCShizhuangInfo();
		MessageHeader header;
		
		struct ShizhuangItemMsg 
		{
			char use_img;					// ��ǰʹ�õ�����ID
			char use_special_img;			// ��ǰʹ�õ���������ID��������ȼ��ߣ�
			char grade;						// ����
			char is_use_special_img;        // �Ƿ�ʹ����������(�û�����),0��ʹ��,1ʹ��
			int grade_bless;				// ����ף��ֵ
			unsigned char active_flag[SHIZHUANG_MAX_ACTIVE_IMAGE_BYTE];
			unsigned char special_img_active_flag[SHIZHUANG_MAX_ACTIVE_SPECIAL_IMAGE_BYTE]; // �������󼤻���
			short special_img_grade_list[SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT]; // ��������ȼ��б�
			unsigned int valid_timestamp_list[SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT];
			short shuxingdan_count;         // ʹ�����Ե�����(���ʵ�)
			short reserve;
			short skill_level_list[ShizhuangItem::SKILL_COUNT];              //���ܵȼ�
			short equip_level_list[ShizhuangItem::EQUIP_COUNT];              //װ���ȼ�
			int equip_skill_level;                                           //װ�����ܵȼ�
			unsigned int clear_bless_value_time;                             //���ף��ֵ��ʱ��
		};

		ShizhuangItemMsg item_list[SHIZHUANG_TYPE_MAX];
	};

	enum CARD_OPERATE_TYPE
	{
		CARD_OPERATE_TYPE_REQ = 0,
		CARD_OPERATE_TYPE_INLAY,
		CARD_OPERATE_TYPE_UPLEVEL,
		CARD_OPERATE_TYPE_KEY_UPLEVEL,
	};

	class CSCardOperate
	{
	public:
		CSCardOperate();
		MessageHeader header;
			
		int operate_type;

		int param1;
		int param2;
		int param3;
		int param4;
	};

	class SCCardInfo
	{
	public:
		SCCardInfo();
		MessageHeader header;

		struct CardItem 
		{
			char slot_color[SLOT_PER_CARD];
			short level;
			short reserve;
			int exp;
		};

		CardItem card_item_list[CARD_MAX_COUNT];
	};

	class SCCardUplevelSucc
	{
	public:
		SCCardUplevelSucc();
		MessageHeader header;
	};

	class CSDisconnectReq
	{
	public:
		CSDisconnectReq();
		MessageHeader header;
	};

	class CSZhuanzhiReq
	{
	public:
		CSZhuanzhiReq();
		MessageHeader header;
	};

	class SCMolongInfo      // 6104
	{
	public:
		SCMolongInfo();
		MessageHeader header;

		int accumulate_consume_gold;
		int today_consume_gold;
		short today_move_step;
		short total_move_step;
		int curr_loop;


		int rank_grade;
		int rank_cumulate_gold;
	};

	class SCOtherCapabilityInfo		// 2427
	{
	public:
		SCOtherCapabilityInfo();
		MessageHeader header;

		int active_flag;
	};

	class SCAllZhiBaoInfo								// ����������Ϣ
	{
	public:
		SCAllZhiBaoInfo();
		MessageHeader header;

		int exp;
		short level;
		short use_image;
		short reserve;
		short grade_max;
		short grade[MAX_ZHIBAO_HUANHUA_TPYE_COUNT];
		short revese;
	};

	class CSFriendExpBottleOP							// ���Ѿ���ƿ�������� 1790
	{
	public:
		CSFriendExpBottleOP();
		MessageHeader header;

		int type;										// 0��ȡ����,1���ʹ���
	};

	class SCFriendExpBottleAddExp						// ���Ѿ���ƿ�����ۼƾ��� 1791
	{
	public:
		SCFriendExpBottleAddExp();
		MessageHeader header;

		int next_broadcast_time;						// �´ι㲥ʱ��
		int get_exp_count;								// ��ȡ����ƿ��
		long long exp;									// ��ǰ�ۼƾ���
		int auto_add_friend_count;						// �Զ����ӵĺ�������
	};

	class CSSkipReq
	{
	public:
		CSSkipReq();
		MessageHeader header;

		enum SKIP_TYPE
		{
			SKIP_TYPE_CHALLENGE = 0,					//����������������
			SKIP_TYPE_SAILING,							//������������
			SKIP_TYPE_MINE,								//���������ڿ�
			SKIP_TYPE_FISH,								//����
			SKIP_TYPE_JINGLING_ADVANTAGE,				//��������
			SKIP_TYPE_SHENZHOU_WEAPON,					//�Ϲ��ż�
			SKIP_TYPE_XINGZUOYIJI,						//�����ż�
			SKIP_TYPE_QYSD,								//��Եʥ��
			SKIP_TYPE_PRECIOUS_BOSS,					//�ز�boss
			SKIP_TYPE_PAOHUAN_TASK,						//�ܻ�����
			SKIP_TYPE_CROSS_GUIDE_BATTLE,						//�������
		};

		int type;										//����
		int param;										//Ʒ��,��0��ʼ(û��Ʒ������ľ������)
	};

	//��ӡЭ��
	class SCShenYinScoreInfo // 8444
	{
	public:
		SCShenYinScoreInfo();
		MessageHeader header;

		int notify_type;
		int add_score;

		int score;				// ����ӡ�Ǳһ���	
		int chouhun_score;		//��ӡ������
	};

	struct ShenYinGridParamMsg
	{
		int param1; // type 0 Ϊ��������  type 1 Ϊӡλ
		short v_item_id;
		short item_num;
		short is_bind;
		short level;
		int grade;

		ShenYinPastureSpiritGridParam::AttrItem attr_param;
		ShenYinPastureSpiritGridParam::AttrItem new_attr_param;
	};


	class SCShenYinBagInfo			// ���ⱳ����Ϣ8445
	{
	public:
		enum TYPE
		{
			BAG,			// param1  Ϊ��������
			IMPRINT_SLOT,	// param1  Ϊӡλ
		};

		SCShenYinBagInfo();
		MessageHeader header;


		char type;
		char count;
		short imprint_grade;
		ShenYinGridParamMsg grid_list[SHEN_YIN_PASTURE_SPIRIT_MAX_GRID_COUNT];
	};

	class SCShenYinShopInfo	// 8446
	{
	public:
		struct Shop
		{
			char index;
			char buy_count;
			short reserve_sh;
			unsigned int timestamp;
		};

		SCShenYinShopInfo();
		MessageHeader header;

		int count;
		Shop shop_list[SHEN_YIN_PASTURE_SPIRIT_MAX_SHOP_ITEM_COUNT];
	};

	class SCShenYinLieMingBagInfo		// 8447
	{
	public:
		SCShenYinLieMingBagInfo();
		MessageHeader header;

		int liehun_color;
		short liehun_pool[SHEN_YIN_LIEHUN_POOL_MAX_COUNT];
	};

	struct VirtualBagData
	{
		VirtualBagData() { this->Reset(); }

		void Reset()
		{
			virtual_bag_index = -1;
			num = 0;
		}

		int virtual_bag_index;
		int num;
	};

	class CSShenYinOneKeyRecyleReq
	{
	public:
		CSShenYinOneKeyRecyleReq();
		MessageHeader header;

		int count;
		short bag_index_list[SHEN_YIN_PASTURE_SPIRIT_MAX_GRID_COUNT];
	};

	class SCUpgradeRushInfo
	{
	public:
		SCUpgradeRushInfo();
		MessageHeader header;

		unsigned int upgrade_return_end_time_list[UPGRADE_RUSH_MAX_TYPE_COUNT];
	};

	enum DROP_LOG_TYPE
	{
		DOPE_LOG_TYPE_BOSS =0,
		DOPE_LOG_TYPE_FB = 1,
	};
	class CSGetDropLog                 // 8521
	{
	public:
		CSGetDropLog();
		MessageHeader header;

		short type;
		short param;
	};

	class SCDropLogRet                 // 8522
	{
	public:
		SCDropLogRet();
		MessageHeader header;

		short type;
		short count;
		DropLogItem log_item[DROP_LOG_NUM_MAX];
	};


	enum ROLE_TALENT_OPERATE_TYPE
	{
		ROLE_TALENT_OPERATE_TYPE_INFO = 0,		// �����츳��Ϣ
		ROLE_TALENT_OPERATE_TYPE_UPLEVEL,		// �����츳
		ROLE_TALENT_OPERATE_TYPE_RESET,			// �����츳��
	};

	class CSRoleTelentOperate
	{
	public:
		CSRoleTelentOperate();
		MessageHeader header;

		int opera_type;
		int param_1;
	};

	class SCRoleTelentInfo
	{
	public:
		SCRoleTelentInfo();
		MessageHeader header;

		char talent_level_list[MAX_TELENT_TYPE_COUT][MAX_TELENT_INDEX_COUT];
		int talent_point;
	};


	static const int TALENT_SKILL_TARGET_MAX = 5;

	class SCTalentSkillTargetList						// 7252
	{
	public:
		SCTalentSkillTargetList();
		MessageHeader header;

		long long injure;
		int target_count;
		unsigned short deliver_obj_id;
		unsigned short target_objid_list[TALENT_SKILL_TARGET_MAX];
	};


	//////////////////////////////////////////////////////////////////////////////////
	struct SCWorldBossItemInfo
	{
		static const int HISTORY_KILLER_MAX_COUNT = 5;      

		int boss_id;
		unsigned int next_refresh_time;

		char is_concern;	// �Ƿ��ע�˸�BOSS
		char reserve_ch;
		short reserve_sh;
	};

	///////////////////////////////// �Ϲ�boss ///////////////////////////////////////
	static const int SC_SHANGGU_BOSS_LAYER_MAX_COUNT = 10;	// �Ϲ�BOSS - ÿһ�����boss����
	static const int SC_SHANGGU_BOSS_MAX_LAYER = 6;			// �Ϲ�BOSS - ������
	enum SHANGGU_BOSS_OPERA_TYPE
	{
		SHANGGU_BOSS_OPERA_TYPE_ENTER = 0,				// ��������param1 ������param2 bossID
		SHANGGU_BOSS_OPERA_TYPE_ALL_BOSS_INFO = 1,		// ������Ϣ
		SHANGGU_BOSS_OPERA_TYPE_LAYER_BOSS_INFO = 2,	// ������Ϣ��param1 ����
		SHANGGU_BOSS_OPERA_TYPE_SET_CONCERN = 3,		// ���ù�ע, param1 ���� param2 BOSS id  param3 �Ƿ��ע��1��0��
	};

	class CSShangGuBossReq    // �������� 8545
	{
	public:
		CSShangGuBossReq();
		MessageHeader header;

		short opera_type;
		short param1;
		short param2;
		short param3;
	};

	class SCShangGuBossAllInfo        // ������Ϣ 8546
	{
	public:
		SCShangGuBossAllInfo();
		MessageHeader header;

		struct LayerBossInfo
		{
			short reserve_sh;
			short boss_count;	// ��ǰ��boss��
			SCWorldBossItemInfo boss_info_list[SC_SHANGGU_BOSS_LAYER_MAX_COUNT];
		};

		int tire_value;		// ƣ��ֵ
		short enter_times;	// �ѽ������
		short layer_count;	// ����
		LayerBossInfo layer_list[SC_SHANGGU_BOSS_MAX_LAYER];
	};

	class SCShangGuBossLayerInfo   // ������Ϣ 8547
	{
	public:
		SCShangGuBossLayerInfo();
		MessageHeader header;

		short cur_layer;
		short boss_count;	// ��ǰ��boss��
		SCWorldBossItemInfo boss_info_list[SC_SHANGGU_BOSS_LAYER_MAX_COUNT];
	};

	class SCShangGuBossSceneInfo				// BOSS��������Ϣ 8548
	{
	public:
		SCShangGuBossSceneInfo();
		MessageHeader	header;

		short angry_value;				// ŭ��ֵ
		short reserve_sh;
		unsigned int kick_out_time;		// �߳�������ʱ���
	};

	class SCShangGuBossSceneOtherInfo	// ��������Ϣ�ƽ�֡�����boss�����䡢С���� 8549
	{
	public:
		SCShangGuBossSceneOtherInfo();
		MessageHeader	header;

		struct LayerInfo
		{
			LayerInfo() : gold_monster_num(0), hide_boss_num(0), max_boss_num(0), min_boss_num(0) {}

			short gold_monster_num;
			short hide_boss_num;
			short max_boss_num;
			short min_boss_num;
		};

		int layer_count;	// ����
		LayerInfo layer_info_list[SC_SHANGGU_BOSS_MAX_LAYER];
	};

	/////////////////////////////// BOSSͼ�� //////////////////////////////////////
	enum BOSS_CARD_OPERA_TYPE
	{
		BOSS_CARD_OPERA_TYPE_ALL_INFO = 0,       //������Ϣ
		BOSS_CARD_OPERA_TYPE_ACTIVE,			 // ���� param1 ���seq
		BOSS_CARD_OPERA_TYPE_FETCH,              // ��ȡ������� param1 ����type

		BOSS_CARD_OPERA_TYPE_MAX,
	};

	class CSBossCardReq			// 8775
	{
	public:
		CSBossCardReq();
		MessageHeader header;

		short req_type;
		short param1;
		short param2;
	};

	class SCBossCardAllInfo			// 8776 
	{
	public:
		SCBossCardAllInfo();
		MessageHeader header;

		unsigned long long card_group_reward_fetch_flag;                              // ���뽱����ȡ���
		unsigned char card_can_active_flag[BOSS_CARD_STORE_MAX_BYTE];			// �ɼ�����
		unsigned char card_has_active_flag[BOSS_CARD_STORE_MAX_BYTE];			// �Ѽ�����
	};

	class SCReliveTire				          //  ����ƣ��   5740
	{
	public:
		SCReliveTire();
		MessageHeader	header;

		short reserve_sh;
		short relive_tire_value;				// �������
		unsigned int tire_buff_end_time;		// ����ƣ��BUFF����ʱ��
		unsigned int tire_can_relive_time;		// �´�������ʱ��
	};

	class CSGetLuckyLog                 // 8620
	{
	public:
		CSGetLuckyLog();
		MessageHeader header;

		int activity_type;
	};

	class SCLuckyLogRet                 // 8621
	{
	public:
		SCLuckyLogRet();
		MessageHeader header;

		int activity_type;
		int count;
		LuckyLogItem log_item[LOG_NUM_MAX];
	};

	////////////////////////// ��ɫ��СĿ�� //////////////////////////
	enum ROLE_BIG_SMALL_GOAL_OPERA_TYPE
	{
		ROLE_BIG_SMALL_GOAL_OPERA_INFO = 0,      // ������Ϣ   param_1 ϵͳ����
		ROLE_BIG_SMALL_GOAL_OPERA_FETCH,         // ��ȡ       param_1 ϵͳ����  param_2 �Ƿ�ʹ��Ԫ��
	};

	class CSRoleBigSmallGoalOper            // 8821
	{
	public:
		CSRoleBigSmallGoalOper();
		MessageHeader header;

		int oper_type;                    // ��������
		int param_1;
		int param_2;
	};

	class SCRoleBigSmallGoalInfo           // 8822
	{
	public:
		SCRoleBigSmallGoalInfo();
		MessageHeader header;

		struct GoalInfoItem
		{
			GoalInfoItem() : active_flag(0), fetch_flag(0){}
			char active_flag;                                               // ������
			char fetch_flag;                                                // ��ȡ���
		};

		short system_type;                                                   // ϵͳ����
		char res_ch;
		char active_special_attr_flag;                                       // �����������Ա��
		unsigned int open_system_timestamp;                                  // ϵͳ������ʱ���
		GoalInfoItem goal_item_info[ROLE_BIG_SMALL_GOAL_REWARD_TYPE_MAX];    // 2   0 СĿ��  1 ��Ŀ��
	};

	//-------------------------------------------------------------------------------------------

	// ��װ�ռ�           8766
	class SCRedEquipCollect
	{
	public:
		SCRedEquipCollect();
		MessageHeader header;

		int seq;
		ItemDataWrapper equip_slot[RED_EQUIP_COLLECT_SLOT_COUNT];
	};

	// ��װ�ռ�-������Ϣ        8767
	class SCRedEquipCollectOther
	{
	public:
		SCRedEquipCollectOther();
		MessageHeader header;

		UInt32 seq_active_flag;					// ��װ�����ǣ��Ѽ���ſɴ�����
		int collect_count;						// �Ѽ������װ��
		UInt32 act_reward_can_fetch_flag;		// ���������ȡ���
		UInt32 active_reward_fetch_flag;		// �������ȡ��ǣ��ƺ���ȡ��ǣ�
		RedEquipCollectInfo collect_info_list[RED_EQUIP_COLLECT_SEQ_MAX_COUNT];		// ��װ�ռ�����
	};

	//-------------------------------------------------------------------------------------------

	// ��װ�ռ�               8768
	class SCOrangeEquipCollect
	{
	public:
		SCOrangeEquipCollect();
		MessageHeader header;

		int seq;
		ItemDataWrapper equip_slot[ORANGE_EQUIP_COLLECT_SLOT_COUNT];
	};

	// ��װ�ռ�-������Ϣ            8769
	class SCOrangeEquipCollectOther
	{
	public:
		SCOrangeEquipCollectOther();
		MessageHeader header;

		UInt32 seq_active_flag;					// ��װ�����ǣ��Ѽ���ſɴ�����
		int collect_count;						// �Ѽ������װ��
		UInt32 act_reward_can_fetch_flag;		// ���������ȡ���
		UInt32 active_reward_fetch_flag;		// �������ȡ��ǣ��ƺ���ȡ��ǣ�
		OrangeEquipCollectInfo collect_info_list[ORANGE_EQUIP_COLLECT_SEQ_MAX_COUNT];		// ��װ�ռ�����
	};

	//---------------------------����--------------------------------------------------------
	enum XIANZUNKA_OPERA_REQ_TYPE
	{
		XIANZUNKA_OPERA_REQ_TYPE_ALL_INFO = 0,				// ����������Ϣ
		XIANZUNKA_OPERA_REQ_TYPE_BUY_CARD,					// �������� param_1 : ��������
		XIANZUNKA_OPERA_REQ_TYPE_FETCH_DAILY_REWARD,		// ��ȡÿ�ս��� param_1 :��������
		XIANZUNKA_OPERA_REQ_TYPE_ACTIVE,					// ������������ param_1:��������

		XIANZUNKA_OPERA_REQ_TYPE_MAX
	};

	struct CSXianZunKaOperaReq			// 8839
	{
		CSXianZunKaOperaReq();
		MessageHeader header;

		short opera_req_type;
		short param_1;
		int param_2;
	};

	struct SCXianZunKaAllInfo		//8840
	{
		SCXianZunKaAllInfo();
		MessageHeader header;

		short forever_active_flag;						// ���ü���λ���	
		char first_active_reward_flag;					// �״μ�������
		char daily_reward_fetch_flag;					// ÿ�ս�����ȡ���

		unsigned int temporary_valid_end_timestamp_list[XIANZUNKA_TYPE_MAX];		// ��ʱ�������ʱ���
	};

	///////////////////// װ��ϴ�� ////////////////////////////////
	enum EQUIP_BAPTIZE_OPERA_TYPE
	{
		EQUIP_BAPTIZE_OPERA_TYPE_ALL_INFO = 0,		// ������Ϣ
		EQUIP_BAPTIZE_OPERA_TYPE_OPEN_SLOT,			// ������          param_1 װ����λ   param_2  ϴ��������
		EQUIP_BAPTIZE_OPERA_TYPE_LOCK_OR_UNLOCK,	// ���� or ȥ��    param_1 װ����λ   param_2  ϴ��������
		EQUIP_BAPTIZE_OPERA_TYPE_BEGIN_BAPTIZE,		// ��ʼϴ��        param_1 ϴ��λ��   param_2  �Ƿ�ʹ��Ԫ��   param_3  ����ϴ��ʯ

		EQUIP_BAPTIZE_OPERA_TYPE_MAX,
	};

	class CSEquipBaptizeOperaReq            // 8847
	{
	public:
		CSEquipBaptizeOperaReq();
		MessageHeader header;

		int opera_type;
		int param_1;
		int param_2;
		int param_3;
	};

	class SCEquipBaptizeAllInfo             // 8848
	{
	public:
		SCEquipBaptizeAllInfo();
		MessageHeader header;

		EquipBaptizePartInfo part_info_list[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];

		unsigned char open_flag[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];
		unsigned char lock_flag[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];
		unsigned char used_free_times;
		unsigned char reserve_ch;
		unsigned short reserve_sh;
	};

	class SCMiZangBossReliveTire				          //  �زظ���ƣ��   8860
	{
	public:
		SCMiZangBossReliveTire();
		MessageHeader	header;

		short reserve_sh;
		short relive_tire_value;				// �������
		unsigned int tire_buff_end_time;		// ����ƣ��BUFF����ʱ��
		unsigned int tire_can_relive_time;		// �´�������ʱ��
	};

	class SCYouMingBossReliveTire				          //  ��ڤ����ƣ�� 8855
	{
	public:
		SCYouMingBossReliveTire();
		MessageHeader	header;

		short reserve_sh;
		short relive_tire_value;				// �������
		unsigned int tire_buff_end_time;		// ����ƣ��BUFF����ʱ��
		unsigned int tire_can_relive_time;		// �´�������ʱ��
	};

	class SCUpgradeCardBuyInfo										// 8878 ������Ϣ
	{
	public:
		SCUpgradeCardBuyInfo();
		MessageHeader header;

		short activity_id;
		short grade;												// ��ǰ����
		short is_already_buy;	                                    // 0 û�й���  1�Ѿ�����
		short reserve_sh;
	};

	class CSUpgradeCardBuyReq										// 8879 ��������
	{
	public:
		CSUpgradeCardBuyReq();
		MessageHeader header;

		short activity_id;
		ItemID item_id;												// unsigned short
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	class SCHuguozhiliInfo											// 8610  ������ - ��Ϣ
	{
	public:
		SCHuguozhiliInfo();
		MessageHeader header;

		HuguozhiliParam info;
	};

	class CSHuguozhiliReq											// 8611  ������ - ����
	{
	public:
		enum
		{
			REQ_INFO,				// ������Ϣ
			REQ_ACTIVE_HUGUOZHILI,	// ���󼤻��֮��
		};

		CSHuguozhiliReq();
		MessageHeader header;

		int req_type;
	};

	class CSTodayThemeRewardReq			//8916
	{
	public:
		CSTodayThemeRewardReq();
		MessageHeader header;

		int seq;				//��ȡ�ĸ������������
	};

	class SCTodayThemeRewardFlagInfo	//8917
	{
	public:
		SCTodayThemeRewardFlagInfo();
		MessageHeader header;

		unsigned char fetch_flag[TODAY_THEME_REWARD_MAX_BYTE];	//��ȡ���
	};

	//////////////////////////////////////////////////////////////////////////
	class SCWorldAcquisitionLog										// 8612  �·�ȫ���չ���¼
	{
	public:
		SCWorldAcquisitionLog();
		MessageHeader header;

		enum NOTICE_TYPE
		{
			NOTICE_TYPE_ADD = 0, // ���һ��
			NOTICE_TYPE_ALL = 1, // �·�����
		};

		struct LogItem
		{
			LogItem() { this->Reset(); }

			void Reset()
			{
				log_type = 0;
				log_str_id = 0;
				item_id = 0;
				memset(xianpin_type_list, 0, sizeof(xianpin_type_list));

				timestamp = 0;
				memset(role_name, 0, sizeof(role_name));
				uuid = 0;
			}

			char log_type;			// ����
			char log_str_id;		// ��־�ַ���ID
			ItemID item_id;			// ��ƷID
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // ��Ʒ��������

			unsigned int timestamp;	// ��¼ʱ���
			GameName	role_name;	// ��ɫ
			long long uuid;
		};

		short notice_type;		// ֪ͨ���ͣ�ö��NOTICE_TYPE
		short count;
		LogItem log_List[MAX_SERVER_ACQUISITION_RECORD_NUM];  // ���100��
	};

	class CSWorldAcquisitionLogReq									// 8613  ��������չ���¼
	{
	public:
		CSWorldAcquisitionLogReq();
		MessageHeader header;

		char log_type;			// ����
		char log_str_id;		// ��־�ַ���ID
		ItemID item_id;			// ��ƷID
		unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // ��Ʒ��������
	};

	class CSCrossGoalOperaReq	 //8625
	{
	public:
		enum OPERA_TYPE
		{
			CROSS_GOAL_INFO_REQ = 0,		//��ȡ��Ϣ
			FETCH_CROSS_GOAL_REWARD_REQ = 1,//��ȡ����
			FETCH_GUILD_GOAL_REWAED_REQ = 2,//��ȡ����Ŀ�꽱��
		};

		CSCrossGoalOperaReq();
		MessageHeader header;

		int opera_type;
		int param;
	};

	class SCCrossGoalInfo //8626
	{
	public:
		SCCrossGoalInfo();
		MessageHeader header;

		char fetch_reward_flag[CROSS_GOAL_TASK_COUNT_MAX]; // ����״̬���
		unsigned short kill_cross_boss_num;		//��ɱ����boss
		unsigned short cross_boss_role_killer;	//����boss����ɱ����
		unsigned short kill_baizhan_boss_num;		//��ɱ��սboss��
		unsigned short finish_baizhan_task_num;		//��ɰ�սboss��
		char guild_fetch_reward_flag[CROSS_GOAL_TASK_COUNT_MAX]; // ��������״̬��� 0 δ��� 1 ����ȡ 2 ����ȡ
		unsigned short guild_kill_cross_boss;
		unsigned short guild_kill_baizhan_boss;
		int reserve;
	};

	class SCCrossGoalGuildNotify //8627
	{
	public:
		SCCrossGoalGuildNotify();
		MessageHeader header;

		int flag;
		unsigned short guild_kill_cross_boss;
		unsigned short guild_kill_baizhan_boss;
	};

	class SCZeroGiftGodCostumeInfo				//  8922 ��Ԫ����Ľ���װ��Ϣ
	{
	public:
		SCZeroGiftGodCostumeInfo();
		MessageHeader header;

		ZeroGiftGodCostumeInfo zero_gift_god_costume_info[ZERO_GIFT_GOD_COSTUME_MAX];
	};

	class SCSceneActivityRewardInfo				//  8925 �����չʾ
	{
	public:

		enum REWARD_TYPE
		{
			REWARD_TYPE_SHIZHUANG = 0,		//ʱװ
			REWARD_TYPE_WEAPON = 1,			//����
			REWARD_TYPE_MOUNT = 2,			//����
			REWARD_TYPE_TITLE = 3,		    //�ƺ�
			REWARD_TYPE_WING = 4,		    //����
		};

		enum REWARD_SOURCE_ID				//�����ڿͻ���ʶ��ǻ�Ž�����Դ
		{
			REWARD_SOURCE_ID_CROSS_ADD_CAP = 10000,					//�����ս��
			REWARD_SOURCE_ID_CROSS_ADD_CHARM = 10001,				//������Ȱ�
			REWARD_SOURCE_ID_CROSS_QINGYUAN_CAP = 10002,			//�����Ե��
			REWARD_SOURCE_ID_CROSS_GUILD_KILL_BOSSP = 10003,		//��������ɱ��
			REWARD_SOURCE_ID_CHALLENGEFIELD = 10004,				//����������
		};

		SCSceneActivityRewardInfo();
		MessageHeader header;

		int activity_id;
		int reward_type;
		int reward_id;
		int param;
	};

	class SCEquipUplevelSuitInfo				//  8924 װ���ȼ���װ
	{
	public:

		SCEquipUplevelSuitInfo();
		MessageHeader header;

		int suit_level;
		int suit_active_flag;
	};

	class CSEquipUplevelSuitActive				//  8937 װ���ȼ���װ
	{
	public:

		CSEquipUplevelSuitActive();
		MessageHeader header;

		int active_suit_level;
	};

	/////////////////////////////////////// ��սװ�� ///////////////////////////////////////////////////
	// ��սװ����������
	enum BAIZHAN_EQUIP_OPERATE_TYPE
	{
		BAIZHAN_EQUIP_OPERATE_TYPE_ALL_INFO = 0,		// ����װ��������Ϣ

		BAIZHAN_EQUIP_OPERATE_TYPE_TAKE_OFF,			// ���� p1: part_index

		BAIZHAN_EQUIP_OPERATE_TYPE_UP_LEVEL,			// ����װ��	p1: part_index p2:item_id
	};

	class CSBaizhanEquipOpera		// 8926
	{
	public:
		CSBaizhanEquipOpera();
		MessageHeader header;

		short operate;                                  // �������ͣ�0 = ������Ϣ��1 = ����...��
		short param1;
		int param2;
	};

	class SCBaizhanEquipAllInfo			// 8927 ��սװ����Ϣ
	{
	public:
		SCBaizhanEquipAllInfo();
		MessageHeader header;

		ItemDataWrapper baizhan_equip_list[E_INDEX_BAIZHAN_MAX];

		char part_order_list[E_INDEX_BAIZHAN_MAX];			// �����б�
		short reserve_sh;
	};


	enum LITTLE_HELPER_COMPLETE_TYPE	//CSLittleHelperOpera::type
	{
		LITTLE_HELPER_COMPLETE_TYPE_ALL = -1,
		LITTLE_HELPER_COMPLETE_TYPE_EASY_BOSS = 0,					//��boss
		LITTLE_HELPER_COMPLETE_TYPE_DIFFICULT_BOSS = 1,				//����boss
		LITTLE_HELPER_COMPLETE_TYPE_BABY_BOSS = 2,					//����boss
		LITTLE_HELPER_COMPLETE_TYPE_SUIT_BOSS = 3,					//��װboss
		LITTLE_HELPER_COMPLETE_TYPE_DEMON_BOSS = 4,					//��ħboss
		LITTLE_HELPER_COMPLETE_TYPE_PET_ADVENTURE = 5,				//��������
		LITTLE_HELPER_COMPLETE_TYPE_ESCORT_FAIRY = 6,				//������Ů
		LITTLE_HELPER_COMPLETE_TYPE_EXP_FB = 7,						//���鸱�� p0: �ظ����� p1: ���鸱��������
		LITTLE_HELPER_COMPLETE_TYPE_TOWER_DEFENSE_FB = 8,			//�������� p0: �ظ����� p1: �ٻ���������
		LITTLE_HELPER_COMPLETE_TYPE_CYCLE_TASK = 9,					//�ܻ�����
		LITTLE_HELPER_COMPLETE_TYPE_EXP_TASK = 10,					//��������
		LITTLE_HELPER_COMPLETE_TYPE_GUILD_TASK = 11,				//��������

		LITTLE_HELPER_COMPLETE_TYPE_MAX
	};

	class CSLittleHelperOpera		// 8932
	{
	public:
		CSLittleHelperOpera();
		MessageHeader header;

		struct Req
		{
			Req() : type(0), param_0(0), param_1(0), param_2(0) {}

			short type;									 // �������
			short param_0;
			short param_1;
			short param_2;
		};
		Req req;
	};

	class CSLittleHelperRepeatOpera		// 8958
	{
	public:
		CSLittleHelperRepeatOpera();
		MessageHeader header;

		int count;										// �������
		CSLittleHelperOpera::Req req_list[Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX];		// ����
	};

	class SCLittleHelperInfo
	{
	public:
		SCLittleHelperInfo();
		MessageHeader header;

		short type;									// ֪ͨ�ͻ������������Ҫˢ��
		short reverse_sh;
	};

	class CSGetGuildRareLog                 // 8930  ����ͬ����־
	{
	public:
		CSGetGuildRareLog();
		MessageHeader header;
	};

	class SCGuildRareLogRet                 // 8931  ��ϡ��־��Ϣ
	{
	public:
		SCGuildRareLogRet();
		MessageHeader header;

		int count;
		GuildRareLogItem log_item[GUILD_RARE_LOG_MAX_NUM];
	};

	class SCTianShuXZInfo	// 8935
	{
	public:
		SCTianShuXZInfo();
		MessageHeader header;

		static const int TIANSHU_XUNZHU_MAX_TYPE_COUNT = 8;

		short equip_level_50;
		short equip_level_100;
		long long zhuanzhi_equip_fangyu;									//תְװ����
		unsigned int fetch_flag_list[TIANSHU_XUNZHU_MAX_TYPE_COUNT];		// Ŀ����ȡ����б�
		unsigned int act_flag_list[TIANSHU_XUNZHU_MAX_TYPE_COUNT];			// Ŀ�꼤�����б�
		short baizhan_equip_num;
		short reserve_sh;
	};

	enum CS_TIANSHUXZ_SEQ_TYPE
	{
		CS_TIANSHUXZ_SEQ_TYPE_INFO = 0,	//��ȡ��Ϣ
		CS_TIANSHUXZ_SEQ_TYPE_FETCH = 1,//��ȡ����
	};

	class CSTianShuXZFetchReward	// 8936
	{
	public:
		CSTianShuXZFetchReward();
		MessageHeader header;

		int type;				// ��ȡ����
		short tianshu_type;		// ��������
		short seq;				// ������ȡ����
	};

	class SCLittleHelperItemInfo
	{
	public:
		SCLittleHelperItemInfo();
		MessageHeader		header;


		struct RewardItem
		{
			RewardItem() : item_id(0), is_bind(0), num(0)
			{
				memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
			}

			ItemID item_id;
			short is_bind;
			int num;
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];	// ��Ʒ��������
		};

		int count;
		RewardItem	item_list[lhns::MAX_DROP_ITEM_COUNT];
	};

}

#pragma pack(pop)

#endif

