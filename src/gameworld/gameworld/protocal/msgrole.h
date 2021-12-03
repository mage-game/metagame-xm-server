#ifndef MSGROLE_H
#define MSGROLE_H

#include "gamedef.h"
#include "servercommon/struct/roleinitparam.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/mountparam.hpp"
#include "servercommon/struct/fightmountparam.hpp"
#include "servercommon/struct/xiannvparam.hpp"
#include "servercommon/stonedef.hpp"
#include "servercommon/qingyuandef.hpp"
#include "servercommon/struct/cardparam.hpp"

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/mentalitydef.hpp"
#include "other/capability/capability.hpp"
#include "servercommon/vipdef.hpp"
#include "servercommon/titledef.hpp"
#include "servercommon/welfaredef.hpp"
#include "servercommon/struct/shizhuangparam.hpp"
#include "servercommon/wingdef.hpp"
#include "servercommon/halodef.hpp"
#include "servercommon/shengongdef.hpp"
#include "servercommon/shenyidef.hpp"
#include "servercommon/footprintdef.hpp"
#include "servercommon/lifeskilldef.hpp"
#include "servercommon/campdef.hpp"
#include "task/taskpool.h"
#include "servercommon/struct/xiannvparam.hpp"
#include "servercommon/jinglingdef.hpp"
#include "servercommon/liemingdef.hpp"
#include "servercommon/goldvipdef.hpp"
#include "servercommon/shenzhuangdef.hpp"
#include "servercommon/combineserverequipdef.hpp"

#include "servercommon/struct/global/spaceparam.hpp"
#include "servercommon/struct/global/worldstatusparam.hpp"
#include "servercommon/feixianparam.hpp"
#include "servercommon/struct/fabaoparam.hpp"
#include "servercommon/struct/yaoshiparam.hpp"
#include "servercommon/struct/toushiparam.hpp"
#include "servercommon/struct/qilinbiparam.hpp"
#include "servercommon/struct/maskparam.hpp"
#include "servercommon/struct/mojieparam.hpp"
#include "servercommon/struct/upgradeparam.hpp"
#include "servercommon/cloakdef.hpp"
#include "servercommon/zhuanzhiequipdef.hpp"
#include "config/shuxingdanconfig.hpp"
#include "other/impguard/impguard.hpp"
#include "servercommon/struct/shenbingparam.hpp"
#include "servercommon/equipbaptizedef.hpp"
#include "servercommon/tianshenhutidef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/*
		�����ɫ��Ϣ
	*/
	class CSRoleInfoReq		// 1450
	{
	public:
		CSRoleInfoReq();
		MessageHeader		header;
	};

	class SCRoleInfoAck
	{
	public:
		SCRoleInfoAck();		// 1400
		MessageHeader		header;
		
		char				sex;
		char				prof;
		char				camp;
		char				authority_type;							// ������ͣ�����
		short				level;
		short				energy;
		
		Attribute			hp;										// ��ǰѪ��
		Attribute			base_max_hp;							// �������Ѫ��

		Attribute			mp;										// ��ǰ����ֵ
		Attribute			base_max_mp;							// ���������ֵ

		Attribute			base_gongji;							// ��������
		Attribute			base_fangyu;							// ��������
		Attribute			base_mingzhong;							// ��������
		Attribute			base_shanbi;							// ��������
		Attribute			base_baoji;								// ��������
		Attribute			base_jianren;							// ��������
		Attribute			base_move_speed;						// �����ƶ��ٶ�
		Attribute			base_fujia_shanghai;					// �����˺�
		Attribute			base_dikang_shanghai;					// �ֿ��˺�
		Attribute			base_per_jingzhun;						// ��׼�ٷֱ�
		Attribute			base_per_baoji;							// �����ٷֱ�
		Attribute			base_per_kangbao;						// �����ٷֱ�
		Attribute			base_per_pofang;						// �Ʒ��ٷֱ�
		Attribute			base_per_mianshang;						// ���˰ٷֱ�
		Attribute			base_constant_zengshang;				// �̶�����
		Attribute			base_constant_mianshang;				// �̶�����
		Attribute			base_huixinyiji;						// ����һ��
		Attribute			base_huixinyiji_hurt_per;				// ����һ���˺���
		Attribute			base_per_baoji_hurt;					// �����˺���ֱ�
		Attribute			base_per_kangbao_hurt;					// �����˺��ֿ���ֱ�
		Attribute			base_zhufuyiji_per;						// ף��һ����
		Attribute			base_gedang_per;						// ����
		Attribute			base_gedang_dikang_per;					// �񵲵ֿ���
		Attribute			base_gedang_jianshang;					// �񵲼���
		Attribute			base_skill_zengshang;					// ��������
		Attribute			base_skill_jianshang;					// ���ܼ���
		Attribute			base_mingzhong_per;						// ����������
		Attribute			base_shanbi_per;						// ����������


		long long			exp;									// ����
		long long			max_exp;								// �����

		char				attack_mode;							// ����ģʽ
		char				name_color;								// ������ɫ
		char				move_mode;								// �˶�ģʽ
		char				move_mode_param;						// �˶�ģʽ����
		short				xiannv_huanhua_id;						// ��Ů�û�id
		short				jump_remain_times;						// ��Ծʣ�����
		unsigned int		jump_last_recover_time;					// ���ָ���Ծʱ��
		unsigned int		create_role_time;						// ������ɫʱ��
		
		int					capability;								// ս����

		//unsigned long long	buff_mark;							// buffЧ�����
		unsigned char		buff_mark[SPECIAL_BUFF_MAX_BYTE];
		int 				evil;									// ���ֵ
		int					xianhun;								// �ɻ�
		int					yuanli;									// Ԫ��
		int					nv_wa_shi;								// Ů�ʯ
		int					lingjing;								// �龧
		int					chengjiu;								// �ɾ�
		int					fenchen;								// �۳�
		int					guanghui;								// ���
	
		GuildID				guild_id;								// ����ID			int
		GuildName			guild_name;								// ��������		char[32]
		unsigned int		last_leave_guild_time;					// ����뿪����ʱ��
		char				guild_post;								// ����ְλ
		char				is_team_leader;							// ��Ӷӳ���־
		short				mount_appeid;							// �������

		char				husong_color;							// ����������ɫ
		char				is_change_avatar;						// �Ƿ�ı��ͷ��
		unsigned short		husong_taskid;							// ��������ID

		int					nuqi;									// ŭ��
		int					honour;									// ����

		int					guild_gongxian;							// ���Ź���
		int					guild_total_gongxian;					// �����ܹ���

		Attribute			fight_max_hp;							// ս�����Ѫ��
		Attribute			fight_max_mp;							// ս�������
		Attribute			fight_gongji;							// ս������
		Attribute			fight_fangyu;							// ս������
		Attribute			fight_mingzhong;						// ս������
		Attribute			fight_shanbi;							// ս������
		Attribute			fight_baoji;							// ս������
		Attribute			fight_jianren;							// ս������
		Attribute			fight_move_speed;						// ս���ƶ��ٶ�
		Attribute			fight_fujia_shanghai;					// �����˺�
		Attribute			fight_dikang_shanghai;					// �ֿ��˺�
		Attribute			fight_per_jingzhun;						// ��׼�ٷֱ�
		Attribute			fight_per_baoji;						// �����ٷֱ�
		Attribute			fight_per_kangbao;						// �����ٷֱ�
		Attribute			fight_per_pofang;						// �Ʒ��ٷֱ�
		Attribute			fight_per_mianshang;					// ���˰ٷֱ�
		Attribute			fight_constant_zengshang;				// �̶�����
		Attribute			fight_constant_mianshang;				// �̶�����
		Attribute			pvp_zengshang;							// pvp����
		Attribute			pvp_mianshang;							// pvp����
		Attribute			pve_zengshang;							// pve����
		Attribute			pve_mianshang;							// pve����
		Attribute			fight_huixinyiji;						// ����һ��
		Attribute			fight_huixinyiji_hurt_per;				// ����һ���˺���
		Attribute			fight_per_baoji_hurt;					// �����˺���ֱ�
		Attribute			fight_per_kangbao_hurt;					// �����˺��ֿ���ֱ�
		Attribute			fight_zhufuyiji_per;					// ף��һ����
		Attribute			fight_gedang_per;						// ����
		Attribute			fight_gedang_dikang_per;				// �񵲵ֿ���
		Attribute			fight_gedang_jianshang;					// �񵲼���
		Attribute			fight_skill_zengshang;					// ��������
		Attribute			fight_skill_jianshang;					// ���ܼ���
		Attribute			fight_mingzhong_per;					// ս��������
		Attribute			fight_shanbi_per;						// ս��������
		Attribute			skill_cap_per;							// ����ս����ֱ�

		RoleAppearance		appearance;								// ��ɫ���

		char				used_xiannv_grade;						// ��Ů�񽻵ȼ�
		char				used_xiannv_quality;					// ��ŮƷ�ʵȼ�
		short				mount_flyup_use_img;					// �����������
		short				chengjiu_title_level;					// �ɾͳƺŵȼ�
		ItemID				used_jingling_id;						// ʹ�õľ���id
		GameName			jingling_name;							// ��������
		int					shengwang;								// ����
		long long			avatar_timestamp;						// ͷ���޸�ʱ���
		int					lover_uid;								// ����uid
		GameName			lover_name;								// ��������
		unsigned int		m_last_marry_time;						// ���һ�ν��ʱ��
		short				use_xiannv_halo_img;					// ��Ů�⻷����
		short				used_jingling_level;					// ʹ�þ���ĵȼ�
		short				xianjie_level;							// �ɽ׵ȼ�
		unsigned short		day_revival_times;						// ���ո������
		int					cross_honor;							// �������
		long long			role_uuid;								// ��ɫΨһID
		long long			uuserverid;								// ���һ�ε�½�ķ�����ID
		char				jinghua_husong_status;					// ��������״̬
		char				jingling_use_imageid;					// ����ʹ������
		short				jingling_phantom_img;					// ����û�
		int					gongxun;								// ��ѫֵ

		int					pet_id;									// ����ID
		short				pet_level;								// ����ȼ�	
		short				pet_grade;								// ����׼�		
		GameName			pet_name;								// ��������
		short				user_pet_special_img;                   // ������������û�
		short				jingling_halo_img;						// ����⻷

		int					multi_mount_res_id;						// ˫��������Դid
		int					multi_mount_is_owner;					// �Ƿ�ǰ˫�����������
		int					multi_mount_partner_uid;				// һ����˵����objID

		GameName			little_pet_name;						// С��������
		short				little_pet_using_id;					// С����ʹ������id
		
		short				fight_mount_appeid;						// ս���������

		int					wuqi_color;								// ������ɫ

		short				imp_appe_id[ImpGuard::IMP_GUARD_GRID_INDEX_MAX];	// �ػ�С�����
		int					lover_qingyuan_capablity;				// ������Ես��
		int					lover_baby_capablity;					// ���±���ս��
		int					lover_little_pet_capablity;				// ����С����ս��
	};

	class CSSetAvatarTimeStamp
	{
	public:
		CSSetAvatarTimeStamp();
		MessageHeader		header;

		long long avatar_timestamp;
	};

	class SCAvatarTimeStampInfo
	{
	public:
		SCAvatarTimeStampInfo();
		MessageHeader header;

		char is_change_avatar;
		char reserve_1;
		short reserve_2;
	};

	class CSGetExpExtraPer
	{
	public:
		CSGetExpExtraPer();
		MessageHeader		header;
	};

	class CSQueryRoleInfo
	{
	public:
		CSQueryRoleInfo();
		MessageHeader		header;

		int target_uid;
	};

	class CSCrossQueryRoleInfo
	{
	public:
		CSCrossQueryRoleInfo();
		MessageHeader		header;

		int plat_type;
		int target_uid;
	};

	class CSCrossFindRoleByUUID
	{
	public:
		CSCrossFindRoleByUUID();
		MessageHeader		header;

		int plat_type;
		int target_uid;
		int msg_identify;
	};

	class CSSkillInfoReq
	{
	public:
		CSSkillInfoReq();
		MessageHeader		header;
	};

	struct RoleSkillInfoItem 
	{
		short				index;
		UInt16				skill_id;
		int					level;
		int					exp;
		unsigned int		last_perform;
	};

	static const int SC_MAX_SKILL_ITEM_NUM = 32;
	
	class SCSkillListInfoAck
	{
	public:
		SCSkillListInfoAck();
		MessageHeader			header;
		
		short					default_skill_index;
		short					is_init;
		int						count;
		RoleSkillInfoItem		skill[SC_MAX_SKILL_ITEM_NUM];
	};

	class SCSkillInfoAck
	{
	public:
		SCSkillInfoAck();
		MessageHeader			header;

		RoleSkillInfoItem		skill_info;
	};

	class SCSkillOtherSkillInfo
	{
	public:
		SCSkillOtherSkillInfo();
		MessageHeader			header;

		short skill124_effect_star;
		char skill124_effect_baoji;
		char reserve_ch;
	};

	static const int SC_ATTR_CHANGE_COUNT = 80;

	class SCRoleAttributeValue			// 1402 ��ɫ����ֵ֪ͨ
	{
	public:
		SCRoleAttributeValue();
		MessageHeader header;

		unsigned short obj_id;

		struct AttrPair
		{
			int attr_type;
			Attribute attr_value;
		};

		unsigned char count;
		unsigned char attr_notify_reason;
		AttrPair attr_list[SC_ATTR_CHANGE_COUNT];
	};

	class SCRoleXianhun
	{
	public:
		SCRoleXianhun();
		MessageHeader header;

		int xianhun;
	};

	class SCRoleYuanli
	{
	public:
		SCRoleYuanli();
		MessageHeader header;

		int yuanli;
	};

	class SCRoleNvWaShi
	{
	public:
		SCRoleNvWaShi();
		MessageHeader header;

		int nv_wa_shi;
	};

	class SCRoleHunli
	{
	public:
		SCRoleHunli();
		MessageHeader header;

		int hunli;
	};

	class SCRoleLingJing
	{
	public:
		SCRoleLingJing();
		MessageHeader header;

		int lingjing;
	};

	class SCRoleChengJiu
	{
	public:
		SCRoleChengJiu();
		MessageHeader header;

		int chengjiu;
	};

	class SCRoleDayRevivalTimes
	{
	public:
		SCRoleDayRevivalTimes();
		MessageHeader header;

		unsigned short day_revival_times;
		short reserve;
	};

	class SCRoleNuqi
	{
	public:
		SCRoleNuqi();
		MessageHeader header;

		int nuqi;
	};

	class SCRoleEnergy
	{
	public:
		SCRoleEnergy();
		MessageHeader header;

		short energy;
		short reserve;
	};

	class SCRoleShengwang
	{
	public:
		SCRoleShengwang();
		MessageHeader header;

		int shengwang;
	};

 	class SCRoleGongxun
 	{
 	public:
 		SCRoleGongxun();
		MessageHeader header;

		int gongxun;
 	};

	class SCRoleExpExtraPer
	{
	public:
		SCRoleExpExtraPer();
		MessageHeader header;

		int exp_extra_per;  //��ֱ�
	};

	class SCRoleCampHonour
	{
	public:
		enum
		{
			REASON_NORMAL,
			REASON_WEEK_CHANGE,
		};

		SCRoleCampHonour();
		MessageHeader header;

		int honour;
		int reason;
	};

	class CSRoleSkillLearnReq
	{
	public:
		enum UPLEVEL_SKILL_TYPE
		{
			UPLEVEL_SKILL,
			UPLEVEL_ACTIVE_SKILL_ALL,//��������ȫ������
			UPLEVEL_PASSIVE_SKILL_ALL,//��������ȫ������
		};
		CSRoleSkillLearnReq();
		MessageHeader header;
		
		short req_type;
		short skill_id;
	};

	class SCChaExpChange
	{
	public:
		SCChaExpChange();
		MessageHeader	header;

		short			reason;
		short			reserve;
		long long		exp;
		long long		delta;
		int				add_percent;
		long long		reserve_ll1;
		long long		reserve_ll2;
	};


	class SCChaLevelUpgrade				// 1404 ���������㲥
	{
	public:
		SCChaLevelUpgrade();
		MessageHeader	header;

		unsigned short	obj_id;
		short			level;
		long long	exp;
		long long	max_exp;
	};

	class SCRoleChangeProf				// 1405 תְ�㲥
	{
	public:
		SCRoleChangeProf();
		MessageHeader	header;
		unsigned short	obj_id;
		short			prof;
		Attribute		maxhp;
	};

	class SCRoleChangeSex
	{
	public:
		SCRoleChangeSex();
		MessageHeader	header;

		unsigned short	obj_id;
		short			sex;
	};


	class SCRoleAddCamp
	{
	public:
		SCRoleAddCamp();
		MessageHeader	header;
		ObjID			obj_id;
		char			camp;
		char			reserved;
	};

	class SCRoleZhuanzhi
	{
	public:
		SCRoleZhuanzhi();
		MessageHeader	header;
		ObjID			obj_id;
		short			reserved;
	};

	class CSSetAttackMode
	{
	public:
		CSSetAttackMode();
		MessageHeader	header;
		int				mode;
		int				is_fanji;
	};

	enum SET_ATTACK_MODE_RESULT					// ���ù���ģʽ���
	{
		SET_ATTACK_MODE_SUCC,					// �ɹ�
		SET_ATTACK_MODE_PROTECT_LEVEL,			// ���ֱ�����
		SET_ATTACK_MODE_NO_CAMP,				// û�м�����Ӫ
		SET_ATTACK_MODE_NO_GUILD,				// û�м������
		SET_ATTACK_MODE_NO_TEAM,				// û�����
		SET_ATTACK_MODE_PEACE_INTERVAL,			// С�ں�ƽģʽ�л�ʱ����
		SET_ATTACK_MODE_NO_GUILD_UNION,			// û�о�������
		SET_ATTACK_MODE_STATUS_LIMIT,			// ��ǰ״̬�²������л�����ģʽ
		SET_ATTACK_MODE_MAX
	};

	class SCSetAttackMode
	{
	public:
		SCSetAttackMode();
		MessageHeader	header;

		ObjID			obj_id;
		short			result;
		int				attack_mode;
		unsigned int	last_peace_mode_time;
	};

	class SCRoleGuildInfoChange
	{
	public:
		SCRoleGuildInfoChange();
		MessageHeader	header;

		ObjID			obj_id;
		char			reserved;
		char			guild_post;
		GuildID			guild_id;
		GuildName		guild_name;

		int				guild_gongxian;
		int				guild_total_gongxian;
		unsigned int	last_leave_guild_time;
	};

	class SCRoleNameColorChange
	{
	public:
		SCRoleNameColorChange();
		MessageHeader	header;

		ObjID			obj_id;
		char			name_color;
		char			reserved;
	};

	class SCRoleEvilChange
	{
	public:
		SCRoleEvilChange();
		MessageHeader	header;
		Attribute		evil;
	};
	
	// �鿴������Ϣ����  1412
	class SCGetRoleBaseInfoAck
	{
	public:
		SCGetRoleBaseInfoAck();
		MessageHeader	header;

		struct MountInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// ���Ե�����
			short star_level;										// �Ǽ�
			unsigned char active_special_image_flag[MountParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

			short equip_level_list[MountParam::EQUIP_COUNT];		// װ����Ϣ
			short skill_level_list[MountParam::SKILL_COUNT];		// ���ܵȼ�
		};

		struct FightMountInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			short shuxingdan_count;									// ���Ե�����
			short chengzhangdan_count;								// �ɳ�������
			short star_level;										// �Ǽ�
			unsigned char active_special_image_flag[FightMountParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

			short equip_level_list[FightMountParam::EQUIP_COUNT];	// װ����Ϣ
			short skill_level_list[FightMountParam::SKILL_COUNT];	// ���ܵȼ�
		};


		struct WingInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// ���Ե�����
			short star_level;										// �Ǽ�
			unsigned char active_special_image_flag[WingParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

			short equip_level_list[WingParam::EQUIP_COUNT];			// װ����Ϣ
			short skill_level_list[WingParam::SKILL_COUNT];			// ���ܵȼ�
		};

		struct HaloInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			short shuxingdan_count;									// ���Ե�����
			short chengzhangdan_count;								// �ɳ�������
			short star_level;										// �Ǽ�
			unsigned char active_special_image_flag[HaloParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

			short equip_level_list[HaloParam::EQUIP_COUNT];			// װ����Ϣ
			short skill_level_list[HaloParam::SKILL_COUNT];			// ���ܵȼ�
		};

		struct ShengongInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			short shuxingdan_count;									// ���Ե�����
			short chengzhangdan_count;								// �ɳ�������
			short star_level;										// �Ǽ�
			unsigned char active_special_image_flag[ShengongParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

			short equip_level_list[ShengongParam::EQUIP_COUNT];		// װ����Ϣ
			short skill_level_list[ShengongParam::SKILL_COUNT];		// ���ܵȼ�
		};

		struct ShenyiInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			short shuxingdan_count;									// ���Ե�����
			short chengzhangdan_count;								// �ɳ�������
			short star_level;										// �Ǽ�
			unsigned char active_special_image_flag[ShenyiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

			short equip_level_list[ShenyiParam::EQUIP_COUNT];		// װ����Ϣ
			short skill_level_list[ShenyiParam::SKILL_COUNT];		// ���ܵȼ�
		};

		struct FootprintInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			short shuxingdan_count;									// ���Ե�����
			short chengzhangdan_count;								// �ɳ�������
			short star_level;										// �Ǽ�
			unsigned char active_special_image_flag[FootprintParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

			short equip_level_list[FootprintParam::EQUIP_COUNT];	// װ����Ϣ
			short skill_level_list[FootprintParam::SKILL_COUNT];	// ���ܵȼ�
		};

		struct CloakInfo
		{
			int level;												// �ȼ�
			int used_imageid;										// ʹ�õ�����
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// ���Ե�����
			unsigned long long  active_special_image_flag;			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

			short equip_level_list[CloakParam::EQUIP_COUNT];	// װ����Ϣ
			short skill_level_list[CloakParam::SKILL_COUNT];	// ���ܵȼ�
		};

		struct XiannvShouhuInfo
		{
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
		};

		struct JinglingFazhenInfo
		{
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
		};

		struct JinglingGuanghuanInfo
		{
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
		};

		struct ZhibaoInfo
		{
			short level;											// �ȼ�
			short used_imageid;										// ʹ�õ�����
		};

		struct EquipmentInfo
		{
			ItemID equip_id;
			short has_lucky;

			short quality;											// Ʒ��
			short strengthen_level;									// ǿ���ȼ�
			short shen_level;										// �����ȼ�
			short fuling_level;										// ����ȼ�
			
			short grid_strengthen_level;							// ����ǿ���ȼ�
			short grid_shen_level;									// ���������ȼ�
			short grid_star_level;									// �������ǵȼ�
			short star_level;
			short eternity_level;
			short reserved;

			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // ��������
		};

		struct ShizhuangPart
		{
			char use_img;					                       // ��ǰʹ�õ�����ID
			char use_special_img;			                       // ��ǰʹ�õ���������ID��������ȼ��ߣ�
			char grade;						                       // ����
			char reserve_ch;
			short shuxingdan_count;                                // ���Ե�����
			short res_sh;                                          
			int capability;                                        // ս����
			int equip_skill_level;                                 // װ�����ܵȼ�
			short skill_level_list[ShizhuangItem::SKILL_COUNT];    // ���ܵȼ�,SKILL_COUNT=4
			short equip_level_list[ShizhuangItem::EQUIP_COUNT];    // װ���ȼ�,EQUIP_COUNT=4
		};

		struct XiannvInfo
		{
			XiannvInfo() : active_xiannv_flag(0), huanhua_id(0), reserved(0)
			{
				memset(active_huanhua_flag, 0, sizeof(active_huanhua_flag));
				memset(xiannv_huanhua_level, 0, sizeof(xiannv_huanhua_level));
				memset(pos_list, 0, sizeof(pos_list));
				memset(xiannv_name, 0, sizeof(xiannv_name));
			}

			struct BaseInfo
			{
				short xn_level;
				short xn_zizhi;
			};

			int active_xiannv_flag;								// �������Ů�б�
			unsigned char active_huanhua_flag[MAX_XIANNV_ACTIVE_SPECIAL_IMAGE_BYTE];								// ����Ļû��б�
			short huanhua_id;										// �û�id
			short reserved;
			int xiannv_huanhua_level[MAX_XIANNV_HUANHUA_COUNT];		// ��Ů�û��ȼ�

			BaseInfo xiannv_item_list[MAX_XIANNV_COUNT];			// ������Ů��Ϣ
			char pos_list[MAX_XIANNV_POS];							// ��Ů����λ�� 0Ϊ��սλ
			GameName xiannv_name;									// ��ս��Ů����
			int capability;											// ս����
		};

		struct JinglingInfo
		{
			struct BaseInfo
			{
				short jingling_id;									// ����id
				short jingling_level;								// ����ȼ�
				short is_bind;										// �Ƿ��
				short reserve_sh;
				short talent_list[ItemNamespace::MAX_XIANPIN_NUM];	// �츳�б�
			};

			BaseInfo jinling_item_list[JINGLING_MAX_TAKEON_NUM];	// ������Ϣ
			short phantom_level_list[JINGLING_PTHANTOM_MAX_TYPE];	// �û��ȼ�
			short use_jingling_id;									// ��ս����id
			short reserve_sh2;
			unsigned char phantom_image_flag[JINGLING_PTHANTOM_MAX_BYTE];	// ����Ļû�
			int phantom_imgageid;									// ʹ�õĻû�
		};

		struct LieMingInfo
		{
			short hunshou_id;
			short level;
			long long curr_exp;
		};

		struct FeixianInfo
		{
			ItemID item_id;
			short is_bind;
			UInt32 invalid_time;
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // ��������
		};

		struct FaBaoInfo
		{
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			short shuxingdan_count;									// ���Ե�����
			short used_special_imageid;                             // ʹ�õ���������
			unsigned char active_special_image_flag[FabaoParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;

			short equip_level_list[FabaoParam::EQUIP_COUNT];		// װ����Ϣ,EQUIP_COUNT=4
			short skill_level_list[FabaoParam::SKILL_COUNT];		// ���ܵȼ�,SKILL_COUNT=4
		};

		struct YaoShiInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// ���Ե�����
			short star_level;										// �Ǽ�
			unsigned char active_special_image_flag[YaoShiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

 			short equip_level_list[YaoShiParam::EQUIP_COUNT];		// װ����Ϣ
 			short skill_level_list[YaoShiParam::SKILL_COUNT];		// ���ܵȼ�
		};

		struct TouShiInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// ���Ե�����
			short star_level;										// �Ǽ�
			unsigned char active_special_image_flag[TouShiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

 			short equip_level_list[TouShiParam::EQUIP_COUNT];		// װ����Ϣ
			short skill_level_list[TouShiParam::SKILL_COUNT];		// ���ܵȼ�
		};

		struct QiLinBiInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// ���Ե�����
			short star_level;										// �Ǽ�
			unsigned char active_special_image_flag[QiLinBiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

 			short equip_level_list[QiLinBiParam::EQUIP_COUNT];		// װ����Ϣ
 			short skill_level_list[QiLinBiParam::SKILL_COUNT];		// ���ܵȼ�
		};

		struct MaskInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// ���Ե�����
			short star_level;										// �Ǽ�
			unsigned char active_special_image_flag[MaskParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// ��������
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

 			short equip_level_list[MaskParam::EQUIP_COUNT];		// װ����Ϣ
 			short skill_level_list[MaskParam::SKILL_COUNT];		// ���ܵȼ�
		};

		struct ImpGuardItem
		{
			short impguard_use_type[ImpGuard::IMP_GUARD_GRID_INDEX_MAX];
			unsigned int impguard_invaild_time[ImpGuard::IMP_GUARD_GRID_INDEX_MAX];
		};

		struct Mojie
		{
			struct MojieItem
			{
				char mojie_type;
				char mojie_level;
			};

			MojieItem mojie_level_list[MojieParam::MOJIE_MAX_TYPE];
		};

		struct ZhuanzhiEquipInfo
		{
			struct ZhuanzhiEquipItem
			{
				UInt16 star_level;												// ���ǵȼ�
				ItemID equip_id;												// װ��id
				UInt32 star_exp;												// ���Ǿ���
				UInt16 fuling_count_list[MAX_ZHUANZHI_FULING_ATTR_COUNT];		// �����������
				UInt16 xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];		// ��Ʒ����
			};

			UInt32 capability;													// ս����

			ZhuanzhiEquipItem zhuanzhi_equip_list[E_INDEX_ZHUANZHI::E_INDEX_MAX]; // װ������
			ZhuanzhiStonesPartData stone_list[E_INDEX_ZHUANZHI::E_INDEX_MAX]; 	  // ��ʯ����
			char part_suit_type_list[E_INDEX_ZHUANZHI::E_INDEX_MAX];			  // ��װ�����б�
			char part_order_list[E_INDEX_ZHUANZHI::E_INDEX_MAX];				  // �����б�
			unsigned char zhuanzhi_equip_awakening_lock_flag[MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE];	//�����
			ZhuanzhiEquipAwakening zhuanzhi_equip_awakening_list[E_INDEX_MAX];		// ������Ϣ
		};

		struct UpgradeSysInfo
		{
			short level;											// �ȼ�
			short grade;											// ��
			short used_imageid;										// ʹ�õ�����
			short star_level;										// �Ǽ�
			int equip_skill_level;									// װ�����ܵȼ�
			int capability;											// ս����

			UInt16 shuxingdan_list[SHUXINGDAN_MAX_TYPE];			// ���Ե�����
			UInt8 active_img_flag[UPGRADE_MAX_IMAGE_BYTE];			// ���󼤻���
			short equip_level_list[UPGRADE_EQUIP_COUNT];			// װ����Ϣ
			short skill_level_list[UPGRADE_SKILL_COUNT];			// ���ܵȼ�
			
		};

		struct ShenBingInfo
		{
			short level;
			short use_image;
			int capability;
			unsigned short shuxingdan_list[ShenBingParam::SHUXINGDAN_MAX_TYPE];	// ���Ե������б�
		};

		struct EquipBaptizeInfo
		{
			int baptize_list[EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM];       // ����ֵ
			short attr_seq_list[EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM];      // ����seq
		};

		struct ImageItemInfo
		{
			short img_id = 0;
			short level = 0;
		};

		struct BaizhanEquipInfo
		{ 
			BaizhanEquipInfo()
			{
				this->Reset();
			}

			void Reset()
			{
				capability = 0;
				memset(baizhan_equip_list, 0, sizeof(baizhan_equip_list));
				memset(part_order_list, 0, sizeof(part_order_list));
				reserve_sh = 0;
			}

			int capability;														// ս����
			ItemID baizhan_equip_list[E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX];	// װ������
			UInt8 part_order_list[E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX];			// �����б�
			short reserve_sh;
		};

		int				role_id;									// ��ɫID
		GameName		role_name;									// ��ɫ����
		long long		role_uuid;									// ��ɫUUID

		short			level;										// �ȼ�
		char			is_online;									// �Ƿ�����(1,0)
		char			sex;										// �Ա�

		char			camp_id;									// ��Ӫ
		char			prof;										// ְҵ
		char			vip_level;									// VIP�ȼ�
		char			guild_post;									// ����ְλ

		int				plat_type;									// ƽ̨����
		PlatName		plat_name;									// ƽ̨��(64)

		GuildID			guild_id;									// ����ID
		GuildName		guild_name;									// ��������

		long long		avatar_timestamp;							// ͷ�����ʱ���

		int				lover_uid;									// ����uid
		GameName		lover_name;									// ��������
		int				qingyuan_value;								// ��Եֵ
		ItemID			qingyuan_equip_id;							// ��Եװ��id
		short			qingyuan_equip_star_level;					// ��Եװ���ȼ�
		char			lover_prof;									// ����ְҵ
		char			lover_sex;									// �����Ա�
		char			lover_camp;									// ������Ӫ
		char			lover_vip_level;							// ����VIP�ȼ�
		long long		lover_avatar_timestamp;						// ����ͷ�����ʱ���
		short			lover_level;								// ���µȼ�

		short			name_color;									// ������ɫ
		int				evil_val;									// ���ֵ
		int				all_charm;									// ����
		int				capability;									// ս����

		Attribute		hp;											// Ѫ��
		Attribute		max_hp;										// ���Ѫ��
		Attribute		gongji;										// ����
		Attribute		fangyu;										// ����
		Attribute		mingzhong;									// ����
		Attribute		shanbi;										// ����
		Attribute		baoji;										// ����
		Attribute		jianren;									// ����
		Attribute		fujia_shanghai;								// �����˺�
		Attribute		dikang_shanghai;							// �ֿ��˺�
		Attribute		per_jingzhun;								// ��׼
		Attribute		per_baoji;									// ����
		Attribute		per_kangbao;								// ����
		Attribute		per_pofang;									// �Ʒ��ٷֱ�
		Attribute		per_mianshang;								// ���˰ٷֱ�
		Attribute		constant_zengshang;							// �̶�����
		Attribute		constant_mianshang;							// �̶�����
		Attribute		pvp_zengshang;								// pvp����
		Attribute		pvp_mianshang;								// pvp����
		Attribute		pve_zengshang;								// pve����
		Attribute		pve_mianshang;								// pve����
		Attribute		huixinyiji;									// ����һ��
		Attribute		huixinyiji_hurt_per;						// ����һ���˺���
		Attribute		per_baoji_hurt;								// �����˺���ֱ�
		Attribute		per_kangbao_hurt;							// �����˺��ֿ���ֱ�
		Attribute		zhufuyiji_per;								// ף��һ����
		Attribute		gedang_per;									// ����
		Attribute		gedang_dikang_per;							// �񵲵ֿ���
		Attribute		gedang_jianshang;							// �񵲼���
		Attribute		skill_zengshang;							// ��������
		Attribute		skill_jianshang;							// ���ܼ���
		Attribute		mingzhong_per;								// ������
		Attribute		shanbi_per;									// ������
		Attribute		skill_cap_per;								// ����ս����ֱ�

		MountInfo		mount_info;									// ������Ϣ
		WingInfo		wing_info;									// ������Ϣ
		HaloInfo		halo_info;									// �⻷��Ϣ
		ShengongInfo	shengong_info;								// ����Ϣ
		ShenyiInfo		shenyi_info;								// ������Ϣ

		EquipmentInfo	equipment_info[ItemNamespace::MAX_EQUIPMENT_GRID_NUM]; // װ����Ϣ
		ShizhuangPart	shizhuang_part_list[SHIZHUANG_TYPE_MAX];	// ʱװ

		XiannvShouhuInfo xiannv_shouhu_info;						// ��Ů�ػ�
		JinglingFazhenInfo jingling_fazhen_info;					// ���鷨��
		JinglingGuanghuanInfo jingling_guanghuan_info;				// ����⻷
		ZhibaoInfo		zhibao_info;								// ������Ϣ

		FightMountInfo	fight_mount_info;							// ս��������Ϣ

		XiannvInfo xiannv_info;										// ��ս��Ů��Ϣ
		JinglingInfo jingling_info;									// ������Ϣ
		LieMingInfo slot_list[LIEMING_FUHUN_SLOT_COUNT];			// ������Ϣ

		StoneParam stone_param;										// װ�����ӱ�ʯ

		FootprintInfo	footprint_info;								// �㼣��Ϣ
		FaBaoInfo fabao_info;                                       // ������Ϣ
		CloakInfo	cloak_info;										// ������Ϣ
		short szlevel_list[SHENZHUANG_MAX_PART];					// ��װ��Ϣ
		FeixianInfo	feixian_info[FeiXianParam::E_INDEX_MAX];		// ����װ����Ϣ

		YaoShiInfo		yaoshi_info;								// ������Ϣ
		TouShiInfo		toushi_info;								// ͷ����Ϣ
		QiLinBiInfo		qilinbi_info;								// �������Ϣ
		MaskInfo		mask_info;									// �����Ϣ

		ImpGuardItem    impguard_info;							    // �ػ�С����Ϣ
		Mojie           mojie_info;                                 // ħ����Ϣ
		ZhuanzhiEquipInfo zhuanzhi_equip_info;						// תְװ����Ϣ

		UpgradeSysInfo upgrade_sys_info[UPGRADE_TYPE_MAX];			// ������ϵͳ

		char shenbing_use_image_id;									// ���ʹ��id
		char baojia_use_image_id;									// ����ʹ��id
		short reserve_sh;											// �ֽڶ���

		ShenBingInfo shenbing_info;                                 // ������Ϣ

		int total_jingjie;											// ����ȼ�

		EquipBaptizeInfo equip_baptize_info[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];   // װ��ϴ��
		
		ImageItemInfo greate_soldier_info;							// ������Ϣ
		BaizhanEquipInfo baizhan_equip_info;						// ��սװ��
		TianshenEquipID tianshenhuti_inuse_equip_list[TIANSHENHUTI_EQUIP_MAX_COUNT];	// �����壬ÿ����λ��Ӧװ��ID
	};

	class CSAllInfoReq
	{
	public:
		CSAllInfoReq();
		MessageHeader	header;

		char is_send_chat_board;			// �Ƿ��·������¼
		char reserve_ch;
		short reserve_sh;
	};

	class SCVipInfo
	{
	public:
		SCVipInfo();
		MessageHeader	header;
		ObjID			obj_id;
		char			vip_level;
		char			fetch_qifu_buycoin_reward_flag;
		short			gold_buycoin_times;
		short			gold_buyyuanli_times;
		short			gold_buyxianhun_times;
		char			fetch_qifu_buyxianhun_reward_flag;
		char			fetch_qifu_buyyuanli_reward_flag;
		int				vip_exp;
		int				fetch_level_reward_flag;
		short			free_buycoin_times;
		short			free_buyyuanli_times;
		short			free_buyxianhun_times;
		short			reserve_sh;
		unsigned int	last_free_buycoin_timestamp;
		unsigned int	last_free_buyyuanli_timestamp;
		unsigned int	last_free_buyxianhun_timestamp;
		int				vip_week_gift_times;
		unsigned int	time_limit_vip_time;
	};

	class CSFetchVipLevelReward
	{
	public:
		CSFetchVipLevelReward();
		MessageHeader	header;

		short			seq;
		short			reserve_sh;
	};

	class CSFetchVipWeekReward
	{
	public:
		CSFetchVipWeekReward();
		MessageHeader	header;
	};

	class CSFetchTimeLimitVip	// 6631
	{
	public:
		CSFetchTimeLimitVip();
		MessageHeader	header;
	};

	class CSGetVipWelfare
	{
	public:
		CSGetVipWelfare();
		MessageHeader	header;

		int				type;								// ��������
		int				param;								// �������
	};

	class CSGetRoleCapability
	{
	public:
		CSGetRoleCapability();
		MessageHeader	header;
	};

	class SCRoleCapability				// 1414 ��ɫս����
	{
	public:
		SCRoleCapability();
		MessageHeader	header;

		int total_capability;
		int other_capability;
		int capability_list[CAPABILITY_TYPE_MAX];
	};

	class CSQiFuReq
	{
	public:
		CSQiFuReq();
		MessageHeader  header;

		short type;
		char can_use_gold;
		char qifu_times;
	};

	class CSFetchQiFuReward
	{
	public:
		CSFetchQiFuReward();
		MessageHeader  header;

		short type;
		short seq;
	};

	//////////////////////////////////////  �ճ��������    ////////////////////////////////////
	class SCWelfareInfo										// ���͸�����Ϣ
	{
	public:
		enum NOTIFY_REASON									// ֪ͨԭ��
		{
			WELFARE_INFO_DAFAULT = 0,

			ACCOUNT_LOGIN_INFO,								// �ۼƵ�½��Ϣ
			OFFLINE_EXP_INFO,								// ���߾�����Ϣ

			ONLINE_GIFT_INFO,								// ����������Ϣ

			OFFLINE_EXP_NOTICE,								// ���߾��������Ϣ������ʾ
		};
		const static int ACTIVITY_FIND_TYPE_EXP_MAX = 32;
		SCWelfareInfo();
		MessageHeader header;

		int notify_reson;

		int offline_second;													// ����ʱ��
		//int offline_exp;													// ���߾���
		long long offline_exp;												// ���߾���
		int offline_mojing;													// ����ħ��
		int exp_buff_effect_second;											// ����BUFF��Чʱ��
		int add_double_exp;													// ׷��˫���ɻ�þ���
		int exp_buff_effect_rate;											// ����BUFF����
		int role_level_after_fetch;											// ��ȡ���߾���������ĵȼ�

		int green_item_count;												// ���߽�����ɫ��Ʒ����
		int green_item_resolve_count;										// ���߽�����ɫ��Ʒ�ֽ�����
		int blue_item_count;												// ���߽�����ɫ��Ʒ����
		int blue_item_resolve_count;										// ���߽�����ɫ��Ʒ�ֽ�����
		int purple_item_count;												// ���߽�����ɫ��Ʒ����
		int purple_item_resolve_count;										// ���߽�����ɫ��Ʒ�ֽ�����
		int orange_item_count;												// ���߽�����ɫ��Ʒ����
		int orange_item_resolve_count;										// ���߽�����ɫ��Ʒ�ֽ�����
		int red_item_count;													// ���߽�����ɫ��Ʒ����
		int red_item_resolve_count;											// ���߽�����ɫ��Ʒ�ֽ�����
		int pink_item_count;												// ���߽�����ɫ��Ʒ����
		int pink_item_resolve_count;										// ���߽�����ɫ��Ʒ�ֽ�����
		int collect_item_count;												// ������Ʒ����

		unsigned int sign_in_days;											// ǩ������
		unsigned int sign_in_reward_mark;									// ǩ���������
		unsigned int activity_find_flag;									// ��һر��
		unsigned int activity_join_flag;									// �������
		unsigned int auto_activity_flag;									// �ί�б��
		unsigned int today_online_time;										// ��������ʱ��
		unsigned int online_reward_mark;									// ���߽�����ȡ���
		char is_chongzhi_today;												// �����Ƿ��ֵ
		char continuity_signin_days;										// ����ǩ������
		char sign_in_today_times;											// ����ǩ������
		char max_growth_value_get_type;										// happy_tree_grouth_val����Ĵ�С
		int happy_tree_level;												// ���ֹ����ȼ�
		short happy_tree_grouth_val[MAX_GROWTH_VALUE_GET_TYPE + 1];			// ���ֹ�������ֵ
		short happy_tree_reward;											// ���ֹ���������ȡ���
		short chongjihaoli_reward_mark;										// �弶������ȡ���
		int total_happy_tree_growth_val;									// ���ֹ����ܳɳ�ֵ
		int accumulation_signin_days;										// �ۼ�ǩ������

		int total_chongjihaoli_record_list[MAX_CHONGJIHAOLI_RECORD_COUNT];  // �弶��������ȼ�ȫ������ȡ��¼

		int role_login_level;												// ��ҵ�¼�ȼ�
		long long exp_activityfind[ACTIVITY_FIND_TYPE_EXP_MAX];				// ��һؾ���
		short is_first_send;
		short reserve_sh;

		int daily_find_count;
		DailyFindRewardItem daily_find_list[DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS];	// �ճ��һ�
		
	};

	class CSGetOfflineExp									// ��ȡ���߾���
	{
	public:
		CSGetOfflineExp();
		MessageHeader header;

		int type;											// ��ȡ����
	};

	class CSGetDailyLoginWelfareInfo						// ����ÿ�յ�½������Ϣ
	{
	public:
		CSGetDailyLoginWelfareInfo();
		MessageHeader header;
	};

	class CSGetDailyFindWelfare								// ��ȡ�ճ��һ�
	{
	public:
		CSGetDailyFindWelfare();
		MessageHeader header;

		char dailyfind_type;
		char get_type;
		short reserved;
	};

	class SCDailyFindItemChange								// �ճ��һظı�
	{
	public:
		SCDailyFindItemChange();
		MessageHeader header;

		char dailyfind_type;
		char result;
		short reserved;
	};

	class SCTotalLoginDays									// �ܵ�¼����
	{
	public:
		SCTotalLoginDays();
		MessageHeader header;

		int total_login_day;
	};

	class CSFetchSevenDayLoginReward						// ��ȡ�����¼�����
	{
	public:
		CSFetchSevenDayLoginReward();
		MessageHeader header;

		int fetch_day;										
	};

	class SCSevenDayLoginRewardInfo							// �����¼������Ϣ
	{
	public:
		enum NOTIFY_REASON									// ֪ͨԭ��
		{
			SDLRI_DAFAULT = 0,

			SDLRI_FETCH_REWARD,								// ��ȡ�������¼����
		};

		SCSevenDayLoginRewardInfo();
		MessageHeader header;

		int notify_reason;									// ֪ͨԭ��
		int account_total_login_daycount;					// һ���ۼƵ�¼����
		int seven_day_login_fetch_reward_mark;				// �����ۼƵ�¼������ȡ���
	};

	class CSWelfareSignInReward								// ��ȡǩ������
	{
	public:
		enum REQUEST_TYPE
		{
			DEFAULT_REQUEST = 0,

			PER_DAY_SIGNIN_REWARD,						// ��ȡÿ��ǩ������

			ACCUMULATION_REWARD,						// ��ȡ�ۼƺ�����ǩ������
		};			
		CSWelfareSignInReward();
		MessageHeader header;

		short operate_type;
		short param;
		int is_quick;									// 1��һ����ǩ���㲹ǩ����0��ָ��ǩ��һ��
	};

	class CSWelfareOnlineReward							// ��ȡ���߽���
	{
	public:
		CSWelfareOnlineReward();
		MessageHeader header;

		int part;										
	};

	class CSWelfareSignInFindBack						// ǩ���һ�
	{
	public:
		CSWelfareSignInFindBack();
		MessageHeader header;

		short day;										
		short reserve_sh;
	};

	class CSWelfareActivityFind						// ��һ�
	{
	public:
		CSWelfareActivityFind();
		MessageHeader header;

		short find_type;										
		char is_free_find;							// �Ƿ�����һأ�1 �ǣ�0 ��
		char reserve_ch;
	};

	class CSWelfareAutoActivity					// �ί��
	{
	public:
		CSWelfareAutoActivity();
		MessageHeader header;

		short find_type;										
		short reserve_sh;
	};

	class CSWelfareFetchHappytreeReward			// ���ֹ�������
	{
	public:
		CSWelfareFetchHappytreeReward();
		MessageHeader header;

		int type;
	};

	class CSWelfareFetchChongjihaoliReward				// ��ȡ�弶����
	{
	public:
		CSWelfareFetchChongjihaoliReward();
		MessageHeader header;

		int level;
	};

	class SCExpBuffInfo							// ����buff��Ϣ 8938
	{
	public:
		SCExpBuffInfo();
		MessageHeader header;

		ExpBuffInfo::ExpBuffItem exp_buff_list[EXP_BUFF_TYPE_MAX];
	};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class SCDailyBuyCoinInfo
	{
	public:
		SCDailyBuyCoinInfo();
		MessageHeader header;

		int buy_count;
	};

	class CSGetDailyBuyCoin
	{
	public:
		CSGetDailyBuyCoin();
		MessageHeader header;

		int buy_count;
	};

	class CSSetMoveMode
	{
	public:
		CSSetMoveMode();
		MessageHeader header;

		short move_mode;
		short move_mode_param;
		short longjump_dest_x;
		short longjump_dest_y;
	};

	class SCRoleAuthority
	{
	public:
		SCRoleAuthority();
		MessageHeader header;

		ObjID	obj_id;
		short	authority_type;
	};

	class SCRoleTeamLeaderInfo
	{
	public:
		SCRoleTeamLeaderInfo();
		MessageHeader	header;

		ObjID	obj_id;
		short	is_team_leader;
	};

	// �����޸�
	class SCRoleResetName
	{
	public:
		SCRoleResetName();
		MessageHeader	header;

		ObjID obj_id;
		short reserved;
		GameName name;
	};

	class CSRoleResetName
	{
	public:
		CSRoleResetName();
		MessageHeader	header;

		int				is_item_reset;
		GameName		new_name;
	};

	class SCRoleDayCount
	{
	public:
		SCRoleDayCount();
		MessageHeader		header;

		int					role_id;
		short				daycount_id;
		short				day_count;
	};


	class SCInvestCountInfo
	{
	public:
		SCInvestCountInfo();
		MessageHeader		header;

		int					invest_count;
	};

	class SCRoleMarryInfoChange
	{
	public:
		SCRoleMarryInfoChange();
		MessageHeader header;

		int lover_uid;
		GameName lover_name;
		ObjID obj_id;
		short reserved;
		unsigned int last_marry_time;
	};

	class SCRoleSpecialAppearanceChange
	{
	public:
		SCRoleSpecialAppearanceChange();
		MessageHeader	header;

		unsigned short	obj_id;
		short			special_appearance;
		int				appearance_param;
	};

	class CSRoleSetZombieReq
	{
	public:
		CSRoleSetZombieReq();
		MessageHeader	header;

		int				is_zombie;
	};

	class SCNewhandEndTime
	{
	public:
		SCNewhandEndTime();
		MessageHeader	header;

		unsigned int newhand_end_time;
	};

	class CSMountBianshenOpera
	{
	public:
		CSMountBianshenOpera();
		MessageHeader	header;

		int is_bianshen;
	};

	class SCMountBianshenInfo
	{
	public:
		SCMountBianshenInfo();
		MessageHeader	header;

		int uid;
		unsigned short obj_id;
		short is_mount_bianshen;
		unsigned int mount_bianshen_time;
	};

	class CSSpeedUpHello		//  ���ټ������
	{
	public:
		CSSpeedUpHello();
		MessageHeader header;
	};

	class SCOperateResult			// 1145
	{
	public:
		SCOperateResult();
		MessageHeader header;

		enum
		{
			OP_MOUNT_UPGRADE = 1,						// �������
			OP_QIBING_UPGRADE = 2,						// �������
			OP_XIANNV_PROMOTE_LEVEL = 3,				// ��Ů�ȼ�
			OP_XIANNV_PROMOTE_YUANSH = 4,				// ��Ů����Ԫ��
			OP_XIANNV_PROMOTE_ZIZHI = 5,				// ��Ů��������
			OP_WING_PROMOTE = 6,						// �������
			OP_EQUIP_STRENGTHEN = 7,					// װ��ǿ��
			OP_STONE_UPLEVEL = 8,						// ��ʯ����
			OP_FISH_POOL_EXTEND_CAPACITY = 9,			// �����չ�ɹ�
			OP_WING_UPGRADE = 10,						// ��������
			OP_MOUNT_FLYUP = 11,						// �������
			OP_XIANNV_HALO_UPGRADE = 12,				// ��Ů�ػ�����
			OP_WUSHAUNG_EQUIP_DAZAO = 13,				// ��˫װ������
			OP_XIANNV_JIE_UPGRADE = 14,					// ��Ů����
			OP_XIANJIAN_JIE_UPGRADE = 15,				// �ɽ�����
			OP_EQUIP_UP_STAR_SUCC = 16,					// װ�����ǳɹ�
			OP_JINGLING_UPGRADE = 17,					// ��������
			OP_SHENZHUANG_JINJIE = 18,					// ��װ���� 
			OP_BABY_JIE_UPGRADE = 19,					// ��������  
			OP_PET_JIE_UPGRADE = 20,					// �������(����)
			OP_QINGYUAN_EQUIP_UPLEVEL = 21,				// �������
			OP_HUASHEN_UPLEVEL = 22,					// ��������
			OP_PET_QINMI_PROMOTE = 23,					// �������ܶ�
			OP_MULTI_MOUNT_UPGRADE = 24,				// ˫����������
			OP_WUSHANG_EQUIP_UPSTAR = 25,				// �����������
			OP_JINGLING_HALO_UPSTAR = 26,				// ����⻷����
			OP_FAIRY_TREE_UPGRADE = 27,					// ��������
			OP_MAGIC_EQUIPMENT_STRENGTHEN = 28,			// ħ��ǿ��
			OP_HALO_UPGRADE = 29,						// �⻷����
			OP_SHENGONG_UPGRADE = 30,					// ������
			OP_SHENYI_UPGRADE = 31,						// ��������
			OP_XIANNV_PROMOTE_HUANHUA_LEVEL = 32,		// ��Ů�û��ȼ�
			OP_JINGLING_FAZHEN_UPGRADE = 33,			// ���鷨������
			OP_SHENGONG_UPSTAR = 34,					// ������
			OP_SHENYI_UPSTAR = 35,						// ��������
			OP_HUASHEN_UPGRADE_SPIRIT = 36,				// ���������
			OP_FIGHT_MOUNT_UPGRADE = 37,				// ս���������
			OP_LIEMING_CHOUHUN = 38,					// ��������
			OP_MOUNT_UPSTAR = 39,						// ��������
			OP_WING_UPSTAR = 40,						// ��������
			OP_HALO_UPSTAR = 41,						// �⻷����
			OP_FIGHT_MOUNT_UPSTAR = 42,					// ս������		
			OP_SHENBING_UPLEVEL = 43,					// �������	
			OP_SHENZHOU_WEAPON = 44,					// ��������
			OP_UP_ETERNITY = 45,						// ����װ��
			OP_RA_MAPHUNT_AUTO_FLUSH = 46,				// ��ͼѰ���Զ�ˢ��
			OP_FOOTPRINT_UPGRADE = 47,					// �㼣����
			OP_FOOTPRINT_UPSTAR = 48,					// �㼣����
			OP_CLOAK_UP_LEVEL = 49,						// ��������
			OP_SHEN_YIN_LIEHUN = 50,					// ��ӡϵͳ�Ի�
			OP_FISHING_REQ = 50,						// ��������
			OP_STEAL_FISH_RESULT = 51,					// ͵����
			OP_ELEMENT_HEART_UPGRADE = 52,				// Ԫ�ؽ���
			OP_ELEMENT_TEXTURE_UPGRADE = 53,			// Ԫ��֮�ƽ���
			OP_ELEMENT_EQUIP_UPGRADE = 54,				// Ԫ��װ������
			OP_SHIZHUANG_UPGRADE = 55,					// ʱװ���·�������
			OP_FABAO_UPGRADE = 56,					    // ��������
			OP_FABAO_SPECIAL_IMG_UPGRADE = 57,		    // ���������������
			OP_SHIZHUANG_WEAPON_UPGRADE = 58,			// ʱװ������������
			OP_FABAO_UPLEVELSKILL = 59,                 // ������������
			OP_FABAO_UPLEVELEQUIP = 60,                 // ����װ������
			OP_FEIXIAN_COMPOSE = 61,                    // ����װ���ϳ�
			OP_FEIXIAN_UPLEVEL = 62,                    // ����װ������
			OP_BABY_JL_UPGRADE = 63,					// �����������
			OP_ONEKEY_LIEMING_GAIMING = 64,				// һ����������
			OP_ONEKEY_LIEMING_CHOUHUN = 65,				// һ���������
			OP_YAOSHI_UPGRADE = 66,						// ���ν���
			OP_TOUSHI_UPGRADE = 67,						// ͷ�ν���
			OP_QILINBI_UPGRADE = 68,					// ����۽���
			OP_MASK_UPGRADE = 69,						// ��߽���
			OP_ZHUANZHI_STONE_REFINE = 70,				// ��ʯ����
			OP_ZHUANZHI_FULING = 71,					// תְװ������
			OP_RA_FANFAN_REFRESH = 72,					// ����������ˢ��
			OP_SHENQI_SHENGBING_UPLEVEL = 73,			// �������
			OP_SHENQI_BAOJIA_UPLEVEL = 74,				// ��������
			OP_DELETE_PROFESS = 75,						// ɾ�����
			OP_UPLEVEL_RUNE = 76,						// ��������
			OP_JINGLING_SKILL_REFRESH = 77,				// ���鼼��ˢ��
			OP_GREATE_SOLDIER_SLOT_UPLEVEL = 78,		// ������λ����
			OP_JINGLING_LINGHUN_UPLEVEL = 79,			// �����������

			OP_UPGRADE_SYS_UPGRADE = 200,				// ����ϵͳ���׽�� (200 + upgrade_type)
		};

		short operate;
		short result;			// 1 succ, 0 fail
		int param1;
		int param2;
	};

	//////////////////////////////////////////////////////////////////////////
	class SCOtherUserOnlineStatus
	{
	public:
		SCOtherUserOnlineStatus();
		MessageHeader header;

		int user_id;
		char is_online;
		char reserve_ch;
		short reserve_sh;
		unsigned int last_logout_timestamp;
	};

	enum JIANXIN_OPERA_TYPE
	{
		JIANXIN_OPERA_TYPE_INVALID = 0,
		JIANXIN_OPERA_TYPE_UPLEVEL,
		JIANXIN_OPERA_TYPE_XILIAN,
		JIANXIN_OPERA_TYPE_LOCK,
		JIANXIN_OPERA_TYPE_REPLACE,

		JIANXIN_OPERA_TYPE_MAX,
	};

	class CSRoleChangeCamp
	{
	public:
		CSRoleChangeCamp();
		MessageHeader	header;

		char camp;
		char reserve_ch;
		short reserve_sh;
	};

	class SCRoleChangeCamp
	{
	public:
		SCRoleChangeCamp();
		MessageHeader	header;

		ObjID obj_id;
		char camp;
		char reserve_ch;
	};

	class CSChangeProfReq
	{
	public:
		CSChangeProfReq();
		MessageHeader	header;

		char prof;
		char sex;
		short reserve_sh;
	};

	class SCChangeProfResult
	{
	public:
		SCChangeProfResult();
		MessageHeader	header;

		ObjID obj_id;
		char prof;
		char sex;
	};

	class CSRoleZhuanSheng	// 1776
	{
	public:
		CSRoleZhuanSheng();
		MessageHeader	header;
	};

	class SCRoleNorexItemUseTimes
	{
	public:
		SCRoleNorexItemUseTimes();
		MessageHeader	header;

		int count;
		NorextItemUseDayTimes use_norex_day_times[ItemNamespace::MAX_USE_NOREXITEM_RECORD_NUM];
	};

	class CSTransformExpToBottle
	{
	public:
		CSTransformExpToBottle();
		MessageHeader	header;
		
		int is_query;
		int seq;
		int item_num;
	};

	class SCExpBottleInfo
	{
	public:
		SCExpBottleInfo();
		MessageHeader	header;

		int expbottle_use_count;
		int expbottle_transform_count;
	};

	class SCMoveChessInfo											// 1720��������Ϣ
	{
	public:
		SCMoveChessInfo();
		MessageHeader	header;

		short move_chess_free_times;
		short move_chess_reset_times;
		int move_chess_cur_step;
		unsigned int move_chess_next_free_time;
	};

	class SCMoveChessStepRewardInfo									// 1723������ÿ��������Ϣ
	{
	public:
		SCMoveChessStepRewardInfo();
		MessageHeader	header;

		static const int MAX_REWARD_ITEM_COUNT = 20;
		struct StepRewardInfo
		{
			int step;
			ItemID item_id;
			short item_num;
		};

		int item_count;
		StepRewardInfo reward_list[MAX_REWARD_ITEM_COUNT];
	};

	enum SPACE_MSG_TYPE
	{
		SPACE_MSG_TYPE_INVALID = 0,

		SPACE_MSG_TYPE_LIUYAN,										// ����
		SPACE_MSG_TYPE_CAI,											// ��һ�� ��¼
		SPACE_MSG_TYPE_GIFT,										// ���� ��¼
		
		SPACE_MSG_TYPE_XINQING,										// ���� ��¼

		SPACE_MSG_TYPE_MAX,
	};

	class CSMillionaireInfoReq										// 1721����󸻺��ɼ���Ϣ
	{
	public:
		CSMillionaireInfoReq();
		MessageHeader	header;
	};

	class SCMillionaireInfo											// 1722�󸻺��ɼ���Ϣ
	{
	public:
		SCMillionaireInfo();
		MessageHeader	header;

		short gather_total_times;
		char is_millionaire;										// 1��ʾ�鵽�󸻺���0û�г鵽
		char is_turn;												// 0û��������ת�̣�1��������ת��
		short reward_index;											// �󸻺���������
		short millionaire_use_skill_times;							// ��������ʹ�ô���
		unsigned int millionaire_valid_perform_skill_time;			// �������ܿ���ʹ��ʱ��
	};

	class CSGetRoleSpaceMsgInfo
	{
	public:
		CSGetRoleSpaceMsgInfo();
		MessageHeader	header;

		int msg_type;
		int target_uid;
		int is_seek_byhand;
	};

	class SCRoleSpaceMsgInfo
	{
	public:
		SCRoleSpaceMsgInfo();
		MessageHeader	header;

		int type;
		int target_uid;
		unsigned int last_seek_gift_timestamp;
		unsigned int last_seek_msg_timestamp;

		int is_new_notice;

		struct RecordMsgInfo
		{
			long long record_unq_id;
			int owner_uid;
			GameName owner_name;

			int from_uid;
			GameName from_name;
			int to_uid;
			GameName to_name;

			unsigned int happen_timestamp;
			int happen_space_uid;
			GameName happen_space_ower_name;
			short type;
			short is_huifu;

			char from_prof;
			char from_sex;
			short reserve_sh;
			
			long long from_avatar_timestamp;

			SpaceMessage msg;
		};
		
		int msg_count;
		RecordMsgInfo msg_list[SPACE_MESSAGE_MAX];
	};

	class CSSpaceOper
	{
	public:
		CSSpaceOper();
		MessageHeader	header;

		int who_space_uid;

		int from_uid;
		int to_uid;

		int msg_type;
		int is_huifu;

		SpaceMessage space_msg;
	};

	class CSSpaceRemoveRecord
	{
	public:
		CSSpaceRemoveRecord();
		MessageHeader	header;

		int msg_type;
		long long unq_id;
	};

	class SCSpaceSelfInfo
	{
	public:
		SCSpaceSelfInfo();
		MessageHeader	header;

		int space_renqi;							// �ռ�����
		int space_getgift_count;					// �ռ����������
		int space_day_cai_addhuoli;					// �ռ�ÿ�ղȻ�õĻ���
		int space_huoli;							// �ռ����
		SpaceMessage space_xinqing;					// �ռ�����
	};

	enum LIFE_SKILL_OPERAT_TYPE
	{
		LIFE_SKILL_OPERAT_TYPE_REQ_INFO = 0,
		LIFE_SKILL_OPERAT_TYPE_UPLEVEL,
		LIFE_SKILL_OPERAT_TYPE_MAKE,
	};

	class CSLifeSkillOpera
	{
	public:
		CSLifeSkillOpera();
	
		MessageHeader header;
		int operat_type;
		int param1;
	};

	class SCLifeSkillInfo
	{
	public:
		SCLifeSkillInfo();

		MessageHeader header;

		LifeSkillParam skill_param;
	};

	class SCZhuaGuiRoleInfo
	{
	public:
		SCZhuaGuiRoleInfo();
		MessageHeader		header;

		int zhuagui_day_gethunli;
		int zhuagui_day_catch_count;
		int zhuagui_hunli;
	};

	class SCPaoHuanRollPool
	{
	public:
		SCPaoHuanRollPool();
		MessageHeader header;

		struct RollItem
		{
			unsigned short item_id;
			char is_bind;
			char num;
		};
		RollItem roll_item_list[PAOHUAN_ROLLPOOL_TOTAL_COUNT];
	};

	class SCPaoHuanRollInfo
	{
	public:
		SCPaoHuanRollInfo();
		MessageHeader header;

		enum RIREASON
		{
			RIREASON_START = 0,							// ��ʼ����
			RIREASON_ROLL = 1,							// ���ƽ��
			RIREASON_END = 2,							// ���ƽ���
		};
		short reason;
		char phase;
		char hit_seq;									// ������Ʒ��ţ��������±�

		short clint_click_index;
		short reserve;
	};

	class CSPaoHuanRollReq
	{
	public:
		CSPaoHuanRollReq();
		MessageHeader header;

		short client_click_index;
		short reserve_sh;
	};

	class SCJinghuaHusongViewChange
	{
	public:
		SCJinghuaHusongViewChange();
		MessageHeader header;

		ObjID obj_id;
		short jinghua_husong_status;
	};

	enum SHENZHUANG_OPERATE_TYPE
	{
		SHENZHUANG_OPERATE_NONE = 0,
		SHENZHUANG_OPERATE_UPLEVEL,
	};

	class CSShenzhaungOper
	{
	public:
		CSShenzhaungOper();
		MessageHeader header;

		int operate;
		int param;
	};

	class SCShenzhaungInfo
	{
	public:
		SCShenzhaungInfo();
		MessageHeader header;

		short level_list[SHENZHUANG_MAX_PART];
	};

	enum CSA_OPERATE_TYPE
	{
		CSA_OPERATE_TYPE_NONE = 0,						
		CSA_OPERATE_TYPE_UP_COMMON_LEVEL = 1,			//��ͨװ������
		CSA_OPERATE_TYPE_UP_GREAT_LEVEL = 2,			//����װ������
	};

	class CSCSAEquipOpera								//1881
	{
	public:
		CSCSAEquipOpera();
		MessageHeader header;

		int operate;									//��������
		int param;										//װ����λ
	};

	class SCCSAEquipInfo								//1880
	{
	public:
		SCCSAEquipInfo();
		MessageHeader header;

		short common_level_list[CSA_EQUIP_MAX_PART];	//��ͨװ���ȼ�
		short great_level_list[CSA_EQUIP_MAX_PART];		//����װ���ȼ�
		short active_special;							//�Ƿ񼤻���Ȩ,0����1��
		char is_active_sys;								// �Ƿ�Ϸ�����
		char reserve_ch;								// 
	};

	class SCCSAActivePower								//1882
	{
	public:
		SCCSAActivePower();
		MessageHeader header;

		unsigned short obj_id;
		short csa_active_special;						//�Ƿ񼤻���Ȩ,0����1��
	};

	class CSSeekRoleWhere
	{
	public:
		CSSeekRoleWhere();
		MessageHeader header;

		GameName name;
	};

	class SCRoleSeekWhereInfo
	{
	public:
		SCRoleSeekWhereInfo();
		MessageHeader header;

		int scene_id;
		int posx;
		int posy;
	};

	class SCHuangChengHuiZhanInfo
	{
	public:
		SCHuangChengHuiZhanInfo();
		MessageHeader header;

		int monster_num;
		unsigned int next_refrestime;
	};

	enum CAMPEQUIP_OPERATE_TYPE
	{
		CAMPEQUIP_OPERATE_TYPE_REQ_INFO = 0,	//������Ϣ
		CAMPEQUIP_OPERATE_TYPE_TAKEOFF,			//����
		CAMPEQUIP_OPERATE_TYPE_HUNLIAN,			//����
	};

	class CSCampEquipOperate
	{
	public:
		CSCampEquipOperate();
		MessageHeader header;

		int operate;
		int param1;
		int param2;
		int param3;
	};

	class SCCampEquipInfo
	{
	public:
		SCCampEquipInfo();
		MessageHeader header;
		
		struct CampEquipItem
		{
			int index;
			ItemDataWrapper itemdata;
			int equip_hunlian_level;
		};

		short beast_level;
		short reserver_sh;
		int beast_exp;

		int count;
		CampEquipItem equip_list[MAX_CAMP_EQUIP_INDEX];

	};

	enum CAMP_NORMALDUOBAO_OPERATE_TYPE	
	{
		CAMP_NORMALDUOBAO_OPERATE_ENTER = 0,	//�������ʦ����ͨ�ᱦ
		CAMP_NORMALDUOBAO_OPERATE_EXIT,			//�����˳�ʦ����ͨ�ᱦ
	};

	class CSCampNormalDuobaoOperate
	{
	public:
		CSCampNormalDuobaoOperate();
		MessageHeader header;

		int operate;
		int param1;
	};

	class SCCampDefendInfo
	{
	public:
		SCCampDefendInfo();
		MessageHeader header;

		static const int MAX_RANK_NUM = 5;

		struct RankItem
		{
			RankItem() : roleid(0), hurt(0) { memset(name, 0, sizeof(GameName)); }

			int roleid;
			GameName name;
			long long hurt;
		};

		long long self_hurt;
		long long diaoxiang1_hp;
		long long diaoxiang1_maxhp;
		long long diaoxiang2_hp;
		long long diaoxiang2_maxhp;
		long long diaoxiang3_hp;
		long long diaoxiang3_maxhp;

		RankItem rank_list[MAX_RANK_NUM];
	};

	enum GOLD_VIP_OPERA_TYPE
	{
		OPERA_TYPE_ACTIVE = 0,               // ����
		OPERA_TYPE_FETCH_RETURN_REWARD,		 // ��ȡ��������
		OPERA_TYPE_CONVERT_SHOP,             // �һ��̵�

		OPERA_TYPE_MAX,
	};

	class CSGoldVipOperaReq    //  �ƽ��Ա��������
	{
	public:
		CSGoldVipOperaReq();
		MessageHeader	header;

		short			type;								// ��������
		short			reserve_sh;
		short			param1;								// �������
		short			param2;								// �������2
	};

	class SCGoldVipInfo     //  �ƽ��Ա��Ϣ����
	{
	public:
		SCGoldVipInfo();
		MessageHeader	header;

		char gold_vip_shop_counts_list[GOLD_VIP_CONVERT_SHOP_MAX_COUNT];
		unsigned int gold_vip_active_timestamp;		// ����ʱ���
		int day_score;								// ÿ�ջ���
		char shop_active_grade_flag;				// �̵꼤��α��
		char can_fetch_return_reward;				// �ܷ���ȡ��������
		char is_not_first_fetch_return_reward;		// �Ƿ��ǵ�һ����ȡ��������
		char reserver_1;
	};

	class SCWorldBossWearyInfo
	{
	public:
		SCWorldBossWearyInfo();
		MessageHeader	header;

		int worldboss_weary;
		unsigned int worldboss_weary_last_die_time;          // ���һ������ʱ��
		unsigned int worldboss_weary_last_relive_time;       // ���һ����Ѹ���ʱ��
	};
	
	enum JINJIESYS_REWARD_OPEAR_TYPE
	{
		JINJIESYS_REWARD_OPEAR_TYPE_INFO = 0,		// ��ȡ��Ϣ
		JINJIESYS_REWARD_OPEAR_TYPE_BUY = 1,		// ����param_0 = ����ϵͳ����, raram_1 = ��������
		JINJIESYS_REWARD_OPEAR_TYPE_FETCH = 2,		// ��ȡ���׽�����param_0 = ����ϵͳ����, raram_1 = ��������
	};

	class CSJinjiesysRewardOpera		//1824
	{
	public:
		CSJinjiesysRewardOpera();
		MessageHeader header;

		short opera_type;				//��������
		short param_0;
		short param_1;
		short param_2;
	};

	class SCJinjiesysRewardInfo			//1825
	{
	public:
		SCJinjiesysRewardInfo();
		MessageHeader header;

		unsigned int reward_flag;		//��ȡ(����)�������
		unsigned int can_reward_flag;	//������ȡ���
		unsigned int reward_flag_1;		//��ȡ(����)�������
		unsigned int can_reward_flag_1;	//������ȡ���
	};

	class SCJinjiesysRewardTimestamp	//1826
	{
	public:
		SCJinjiesysRewardTimestamp();
		MessageHeader header;

		static const int JINJIE_SYS_COUNT = 64;
		struct SysTimestamp
		{
			short reward_type;				//��������
			short sys_type;					//ϵͳ����
			unsigned int end_timestamp;		//����ʱ���
		};

		int use_sys_count;
		SysTimestamp timestamp_list[JINJIE_SYS_COUNT];		//ȫ��ϵͳ��ʱ���
	};

	//===============================================================
	//����򵥵�Э�����������ͨ�õġ�����Э��д������!!!!!
	//===============================================================
	// �ͻ��˲�����������
	enum COMMON_OPERATE_TYPE
	{
		COT_JINGHUA_HUSONG_COMMIT = 1,				// ���������ύ
		COT_JINGHUA_HUSONG_INFO_REQ = 2,			// ��������������Ϣ
		COT_KEY_ADD_FRIEND = 3,						// һ���Ⱥ��ѿռ�
		COT_REQ_RED_EQUIP_COLLECT_TAKEON = 4,				// ��װ�ռ��������ϣ�param1�Ǻ�װseq��param2�Ǻ�װ��index�� param3�Ǳ���index
		COT_REQ_RED_EQUIP_COLLECT_FETCH_ATC_REWARD = 5,		// ��װ�ռ�����ȡ�����������param1�ǽ���seq
		COT_REQ_RED_EQUIP_COLLECT_FETCH_TITEL_REWARD = 6,	// ��װ�ռ���ȡ�ƺŽ���, param1�ǽ���seq
		COT_REQ_ORANGE_EQUIP_COLLECT_TAKEON = 7,			// ��װ�ռ�, �����ϣ�param1�Ǻ�װseq��param2�Ǻ�װ��index�� param3�Ǳ���index
		COT_REQ_PERSONAL_BOSS_BUY_TIMES = 8,                // ����boss�������

		COT_ACT_BUY_EQUIPMENT_GIFT = 1000,			// � ����װ�����
	};

	// ������֪ͨ�ͻ�����Ϣ����
	enum SC_COMMON_INFO_TYPE
	{
		SCIT_JINGHUA_HUSONG_INFO = 1,				// ����������Ϣ		
		SCIT_RAND_ACT_ZHUANFU_INFO = 2,				// ����ר����Ϣ
		SCIT_TODAY_FREE_RELIVE_NUM = 3,				// ÿ���Ѿ�ʹ�õ���Ѹ������
	};

	class CSReqCommonOpreate
	{
	public:
		CSReqCommonOpreate();
		MessageHeader header;

		int operate_type;

		int param1;
		int param2;
		int param3;
	};

	class SCCommonInfo	// 1814
	{
	public:
		SCCommonInfo();
		MessageHeader header;

		int info_type;

		int param1;
		int param2;
		int param3;
		int param4;
	};


	///////////////////////////////////// ��ɫ���� ////////////////////////////
	enum CS_ROLE_JINGJIE_OPERA
	{
		CS_ROLE_JINGJIE_OPERA_PROMOTE_LEVEL = 0,	// ��������
		CS_ROLE_JINGJIE_OPERA_GET_INFO,

		CS_ROLE_JINGJIE_OPERA_MAX,
	};

	class CSRoleJingJieReq				// 1472
	{
	public:
		CSRoleJingJieReq();
		MessageHeader header;

		short opera_type;
		short is_auto_buy;
	};

	class SCRoleJingJie					// 1442
	{
	public:
		SCRoleJingJie();
		MessageHeader header;

		int jingjie_level;
	};

	class SCRoleMsgReply					// 4500
	{
	public:
		SCRoleMsgReply();
		MessageHeader header;

		int typ;
		int value;
	};

	enum ROLE_APPE_CHANGE_TYPE
	{
		ROLE_APPE_CHANGE_TYPE_WUQICOLOR = 0,       //תְװ��������ɫ���׳�������أ�
	};

	class SCRoleAppeChange                // 8841
	{
	public:
		SCRoleAppeChange();
		MessageHeader header;

		int appe_type;
		int obj_id;
		int param;
	};

	class CSRolePersonAreaMsgInfo			//8888
	{
	public:
		CSRolePersonAreaMsgInfo();
		MessageHeader header;
		char msg[32];
	};
	class SCRolePersonAreaMsgInfo			//8889
	{
	public:
		SCRolePersonAreaMsgInfo();
		MessageHeader header;
		ObjID obj_id;
		short count;
		char msg[32];
	};

	class SCFightBackRoleList				// 8615 �·������б����uid
	{
	public:
		SCFightBackRoleList();
		MessageHeader header;

		enum 
		{
			NOTIFY_LIST_ADD = 1,	// ���
			NOTIFY_LIST_DEL = 2,	// ɾ��
		};
		
		static const int MAX_ROLE_LIST_NUM = 50;

		short notify;		// enum ֪ͨ����ö��
		short count;
		int role_uid_list[MAX_ROLE_LIST_NUM];
	};
}

#pragma pack(pop)

#endif 

