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
		请求角色信息
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
		char				authority_type;							// 身份类型，待定
		short				level;
		short				energy;
		
		Attribute			hp;										// 当前血量
		Attribute			base_max_hp;							// 基础最大血量

		Attribute			mp;										// 当前法力值
		Attribute			base_max_mp;							// 基础最大法力值

		Attribute			base_gongji;							// 基础攻击
		Attribute			base_fangyu;							// 基础防御
		Attribute			base_mingzhong;							// 基础命中
		Attribute			base_shanbi;							// 基础闪避
		Attribute			base_baoji;								// 基础暴击
		Attribute			base_jianren;							// 基础抗暴
		Attribute			base_move_speed;						// 基础移动速度
		Attribute			base_fujia_shanghai;					// 附加伤害
		Attribute			base_dikang_shanghai;					// 抵抗伤害
		Attribute			base_per_jingzhun;						// 精准百分比
		Attribute			base_per_baoji;							// 暴击百分比
		Attribute			base_per_kangbao;						// 抗暴百分比
		Attribute			base_per_pofang;						// 破防百分比
		Attribute			base_per_mianshang;						// 免伤百分比
		Attribute			base_constant_zengshang;				// 固定增伤
		Attribute			base_constant_mianshang;				// 固定免伤
		Attribute			base_huixinyiji;						// 会心一击
		Attribute			base_huixinyiji_hurt_per;				// 会心一击伤害率
		Attribute			base_per_baoji_hurt;					// 暴击伤害万分比
		Attribute			base_per_kangbao_hurt;					// 暴击伤害抵抗万分比
		Attribute			base_zhufuyiji_per;						// 祝福一击率
		Attribute			base_gedang_per;						// 格挡率
		Attribute			base_gedang_dikang_per;					// 格挡抵抗率
		Attribute			base_gedang_jianshang;					// 格挡减伤
		Attribute			base_skill_zengshang;					// 技能增伤
		Attribute			base_skill_jianshang;					// 技能减伤
		Attribute			base_mingzhong_per;						// 基础命中率
		Attribute			base_shanbi_per;						// 基础闪避率


		long long			exp;									// 经验
		long long			max_exp;								// 最大经验

		char				attack_mode;							// 攻击模式
		char				name_color;								// 名字颜色
		char				move_mode;								// 运动模式
		char				move_mode_param;						// 运动模式参数
		short				xiannv_huanhua_id;						// 仙女幻化id
		short				jump_remain_times;						// 跳跃剩余次数
		unsigned int		jump_last_recover_time;					// 最后恢复跳跃时间
		unsigned int		create_role_time;						// 创建角色时间
		
		int					capability;								// 战斗力

		//unsigned long long	buff_mark;							// buff效果标记
		unsigned char		buff_mark[SPECIAL_BUFF_MAX_BYTE];
		int 				evil;									// 罪恶值
		int					xianhun;								// 仙魂
		int					yuanli;									// 元力
		int					nv_wa_shi;								// 女娲石
		int					lingjing;								// 灵晶
		int					chengjiu;								// 成就
		int					fenchen;								// 粉尘
		int					guanghui;								// 光辉
	
		GuildID				guild_id;								// 军团ID			int
		GuildName			guild_name;								// 军团名字		char[32]
		unsigned int		last_leave_guild_time;					// 最后离开军团时间
		char				guild_post;								// 军团职位
		char				is_team_leader;							// 组队队长标志
		short				mount_appeid;							// 坐骑外观

		char				husong_color;							// 护送任务颜色
		char				is_change_avatar;						// 是否改变过头像
		unsigned short		husong_taskid;							// 护送任务ID

		int					nuqi;									// 怒气
		int					honour;									// 荣誉

		int					guild_gongxian;							// 军团贡献
		int					guild_total_gongxian;					// 军团总贡献

		Attribute			fight_max_hp;							// 战斗最大血量
		Attribute			fight_max_mp;							// 战斗最大法力
		Attribute			fight_gongji;							// 战斗攻击
		Attribute			fight_fangyu;							// 战斗防御
		Attribute			fight_mingzhong;						// 战斗命中
		Attribute			fight_shanbi;							// 战斗闪避
		Attribute			fight_baoji;							// 战斗暴击
		Attribute			fight_jianren;							// 战斗抗暴
		Attribute			fight_move_speed;						// 战斗移动速度
		Attribute			fight_fujia_shanghai;					// 附加伤害
		Attribute			fight_dikang_shanghai;					// 抵抗伤害
		Attribute			fight_per_jingzhun;						// 精准百分比
		Attribute			fight_per_baoji;						// 暴击百分比
		Attribute			fight_per_kangbao;						// 抗暴百分比
		Attribute			fight_per_pofang;						// 破防百分比
		Attribute			fight_per_mianshang;					// 免伤百分比
		Attribute			fight_constant_zengshang;				// 固定增伤
		Attribute			fight_constant_mianshang;				// 固定免伤
		Attribute			pvp_zengshang;							// pvp增伤
		Attribute			pvp_mianshang;							// pvp免伤
		Attribute			pve_zengshang;							// pve增伤
		Attribute			pve_mianshang;							// pve免伤
		Attribute			fight_huixinyiji;						// 会心一击
		Attribute			fight_huixinyiji_hurt_per;				// 会心一击伤害率
		Attribute			fight_per_baoji_hurt;					// 暴击伤害万分比
		Attribute			fight_per_kangbao_hurt;					// 暴击伤害抵抗万分比
		Attribute			fight_zhufuyiji_per;					// 祝福一击率
		Attribute			fight_gedang_per;						// 格挡率
		Attribute			fight_gedang_dikang_per;				// 格挡抵抗率
		Attribute			fight_gedang_jianshang;					// 格挡减伤
		Attribute			fight_skill_zengshang;					// 技能增伤
		Attribute			fight_skill_jianshang;					// 技能减伤
		Attribute			fight_mingzhong_per;					// 战斗命中率
		Attribute			fight_shanbi_per;						// 战斗闪避率
		Attribute			skill_cap_per;							// 技能战力万分比

		RoleAppearance		appearance;								// 角色外观

		char				used_xiannv_grade;						// 仙女神交等级
		char				used_xiannv_quality;					// 仙女品质等级
		short				mount_flyup_use_img;					// 坐骑飞升形象
		short				chengjiu_title_level;					// 成就称号等级
		ItemID				used_jingling_id;						// 使用的精灵id
		GameName			jingling_name;							// 精灵名字
		int					shengwang;								// 声望
		long long			avatar_timestamp;						// 头像修改时间戳
		int					lover_uid;								// 伴侣uid
		GameName			lover_name;								// 伴侣名字
		unsigned int		m_last_marry_time;						// 最近一次结婚时间
		short				use_xiannv_halo_img;					// 仙女光环形象
		short				used_jingling_level;					// 使用精灵的等级
		short				xianjie_level;							// 仙阶等级
		unsigned short		day_revival_times;						// 当日复活次数
		int					cross_honor;							// 跨服荣誉
		long long			role_uuid;								// 角色唯一ID
		long long			uuserverid;								// 最后一次登陆的服务器ID
		char				jinghua_husong_status;					// 精华护送状态
		char				jingling_use_imageid;					// 精灵使用形象
		short				jingling_phantom_img;					// 精灵幻化
		int					gongxun;								// 功勋值

		int					pet_id;									// 宠物ID
		short				pet_level;								// 宠物等级	
		short				pet_grade;								// 宠物阶级		
		GameName			pet_name;								// 宠物名字
		short				user_pet_special_img;                   // 宠物特殊形象幻化
		short				jingling_halo_img;						// 精灵光环

		int					multi_mount_res_id;						// 双人坐骑资源id
		int					multi_mount_is_owner;					// 是否当前双人坐骑的主人
		int					multi_mount_partner_uid;				// 一起骑乘的玩家objID

		GameName			little_pet_name;						// 小宠物名字
		short				little_pet_using_id;					// 小宠物使用形象id
		
		short				fight_mount_appeid;						// 战斗坐骑外观

		int					wuqi_color;								// 武器颜色

		short				imp_appe_id[ImpGuard::IMP_GUARD_GRID_INDEX_MAX];	// 守护小鬼外观
		int					lover_qingyuan_capablity;				// 伴侣情缘战力
		int					lover_baby_capablity;					// 伴侣宝宝战力
		int					lover_little_pet_capablity;				// 伴侣小宠物战力
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

	class SCRoleAttributeValue			// 1402 角色属性值通知
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

		int exp_extra_per;  //万分比
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
			UPLEVEL_ACTIVE_SKILL_ALL,//主动技能全部升级
			UPLEVEL_PASSIVE_SKILL_ALL,//被动技能全部升级
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


	class SCChaLevelUpgrade				// 1404 经验升级广播
	{
	public:
		SCChaLevelUpgrade();
		MessageHeader	header;

		unsigned short	obj_id;
		short			level;
		long long	exp;
		long long	max_exp;
	};

	class SCRoleChangeProf				// 1405 转职广播
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

	enum SET_ATTACK_MODE_RESULT					// 设置攻击模式结果
	{
		SET_ATTACK_MODE_SUCC,					// 成功
		SET_ATTACK_MODE_PROTECT_LEVEL,			// 新手保护期
		SET_ATTACK_MODE_NO_CAMP,				// 没有加入阵营
		SET_ATTACK_MODE_NO_GUILD,				// 没有加入军团
		SET_ATTACK_MODE_NO_TEAM,				// 没有组队
		SET_ATTACK_MODE_PEACE_INTERVAL,			// 小于和平模式切换时间间隔
		SET_ATTACK_MODE_NO_GUILD_UNION,			// 没有军团联盟
		SET_ATTACK_MODE_STATUS_LIMIT,			// 当前状态下不允许切换攻击模式
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
	
	// 查看个人信息返回  1412
	class SCGetRoleBaseInfoAck
	{
	public:
		SCGetRoleBaseInfoAck();
		MessageHeader	header;

		struct MountInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// 属性丹数量
			short star_level;										// 星级
			unsigned char active_special_image_flag[MountParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊坐骑
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

			short equip_level_list[MountParam::EQUIP_COUNT];		// 装备信息
			short skill_level_list[MountParam::SKILL_COUNT];		// 技能等级
		};

		struct FightMountInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			short shuxingdan_count;									// 属性丹数量
			short chengzhangdan_count;								// 成长丹数量
			short star_level;										// 星级
			unsigned char active_special_image_flag[FightMountParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊坐骑
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

			short equip_level_list[FightMountParam::EQUIP_COUNT];	// 装备信息
			short skill_level_list[FightMountParam::SKILL_COUNT];	// 技能等级
		};


		struct WingInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// 属性丹数量
			short star_level;										// 星级
			unsigned char active_special_image_flag[WingParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];// 特殊形象
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

			short equip_level_list[WingParam::EQUIP_COUNT];			// 装备信息
			short skill_level_list[WingParam::SKILL_COUNT];			// 技能等级
		};

		struct HaloInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			short shuxingdan_count;									// 属性丹数量
			short chengzhangdan_count;								// 成长丹数量
			short star_level;										// 星级
			unsigned char active_special_image_flag[HaloParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊形象
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

			short equip_level_list[HaloParam::EQUIP_COUNT];			// 装备信息
			short skill_level_list[HaloParam::SKILL_COUNT];			// 技能等级
		};

		struct ShengongInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			short shuxingdan_count;									// 属性丹数量
			short chengzhangdan_count;								// 成长丹数量
			short star_level;										// 星级
			unsigned char active_special_image_flag[ShengongParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊形象
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

			short equip_level_list[ShengongParam::EQUIP_COUNT];		// 装备信息
			short skill_level_list[ShengongParam::SKILL_COUNT];		// 技能等级
		};

		struct ShenyiInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			short shuxingdan_count;									// 属性丹数量
			short chengzhangdan_count;								// 成长丹数量
			short star_level;										// 星级
			unsigned char active_special_image_flag[ShenyiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊形象
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

			short equip_level_list[ShenyiParam::EQUIP_COUNT];		// 装备信息
			short skill_level_list[ShenyiParam::SKILL_COUNT];		// 技能等级
		};

		struct FootprintInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			short shuxingdan_count;									// 属性丹数量
			short chengzhangdan_count;								// 成长丹数量
			short star_level;										// 星级
			unsigned char active_special_image_flag[FootprintParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊形象
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

			short equip_level_list[FootprintParam::EQUIP_COUNT];	// 装备信息
			short skill_level_list[FootprintParam::SKILL_COUNT];	// 技能等级
		};

		struct CloakInfo
		{
			int level;												// 等级
			int used_imageid;										// 使用的形象
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// 属性丹数量
			unsigned long long  active_special_image_flag;			// 特殊形象
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

			short equip_level_list[CloakParam::EQUIP_COUNT];	// 装备信息
			short skill_level_list[CloakParam::SKILL_COUNT];	// 技能等级
		};

		struct XiannvShouhuInfo
		{
			short grade;											// 阶
			short used_imageid;										// 使用的形象
		};

		struct JinglingFazhenInfo
		{
			short grade;											// 阶
			short used_imageid;										// 使用的形象
		};

		struct JinglingGuanghuanInfo
		{
			short grade;											// 阶
			short used_imageid;										// 使用的形象
		};

		struct ZhibaoInfo
		{
			short level;											// 等级
			short used_imageid;										// 使用的形象
		};

		struct EquipmentInfo
		{
			ItemID equip_id;
			short has_lucky;

			short quality;											// 品质
			short strengthen_level;									// 强化等级
			short shen_level;										// 神铸等级
			short fuling_level;										// 附灵等级
			
			short grid_strengthen_level;							// 格子强化等级
			short grid_shen_level;									// 格子神铸等级
			short grid_star_level;									// 格子星星等级
			short star_level;
			short eternity_level;
			short reserved;

			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // 传奇属性
		};

		struct ShizhuangPart
		{
			char use_img;					                       // 当前使用的形象ID
			char use_special_img;			                       // 当前使用的特殊形象ID（这个优先级高）
			char grade;						                       // 阶数
			char reserve_ch;
			short shuxingdan_count;                                // 属性丹数量
			short res_sh;                                          
			int capability;                                        // 战斗力
			int equip_skill_level;                                 // 装备技能等级
			short skill_level_list[ShizhuangItem::SKILL_COUNT];    // 技能等级,SKILL_COUNT=4
			short equip_level_list[ShizhuangItem::EQUIP_COUNT];    // 装备等级,EQUIP_COUNT=4
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

			int active_xiannv_flag;								// 激活的仙女列表
			unsigned char active_huanhua_flag[MAX_XIANNV_ACTIVE_SPECIAL_IMAGE_BYTE];								// 激活的幻化列表
			short huanhua_id;										// 幻化id
			short reserved;
			int xiannv_huanhua_level[MAX_XIANNV_HUANHUA_COUNT];		// 仙女幻化等级

			BaseInfo xiannv_item_list[MAX_XIANNV_COUNT];			// 所有仙女信息
			char pos_list[MAX_XIANNV_POS];							// 仙女阵形位置 0为主战位
			GameName xiannv_name;									// 出战仙女名字
			int capability;											// 战斗力
		};

		struct JinglingInfo
		{
			struct BaseInfo
			{
				short jingling_id;									// 精灵id
				short jingling_level;								// 精灵等级
				short is_bind;										// 是否绑定
				short reserve_sh;
				short talent_list[ItemNamespace::MAX_XIANPIN_NUM];	// 天赋列表
			};

			BaseInfo jinling_item_list[JINGLING_MAX_TAKEON_NUM];	// 精灵信息
			short phantom_level_list[JINGLING_PTHANTOM_MAX_TYPE];	// 幻化等级
			short use_jingling_id;									// 出战精灵id
			short reserve_sh2;
			unsigned char phantom_image_flag[JINGLING_PTHANTOM_MAX_BYTE];	// 激活的幻化
			int phantom_imgageid;									// 使用的幻化
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
			unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM]; // 传奇属性
		};

		struct FaBaoInfo
		{
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			short shuxingdan_count;									// 属性丹数量
			short used_special_imageid;                             // 使用的特殊形象
			unsigned char active_special_image_flag[FabaoParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊形象
			int equip_skill_level;									// 装备技能等级
			int capability;

			short equip_level_list[FabaoParam::EQUIP_COUNT];		// 装备信息,EQUIP_COUNT=4
			short skill_level_list[FabaoParam::SKILL_COUNT];		// 技能等级,SKILL_COUNT=4
		};

		struct YaoShiInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// 属性丹数量
			short star_level;										// 星级
			unsigned char active_special_image_flag[YaoShiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊坐骑
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

 			short equip_level_list[YaoShiParam::EQUIP_COUNT];		// 装备信息
 			short skill_level_list[YaoShiParam::SKILL_COUNT];		// 技能等级
		};

		struct TouShiInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// 属性丹数量
			short star_level;										// 星级
			unsigned char active_special_image_flag[TouShiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊坐骑
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

 			short equip_level_list[TouShiParam::EQUIP_COUNT];		// 装备信息
			short skill_level_list[TouShiParam::SKILL_COUNT];		// 技能等级
		};

		struct QiLinBiInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// 属性丹数量
			short star_level;										// 星级
			unsigned char active_special_image_flag[QiLinBiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊坐骑
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

 			short equip_level_list[QiLinBiParam::EQUIP_COUNT];		// 装备信息
 			short skill_level_list[QiLinBiParam::SKILL_COUNT];		// 技能等级
		};

		struct MaskInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];	// 属性丹数量
			short star_level;										// 星级
			unsigned char active_special_image_flag[MaskParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 特殊坐骑
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

 			short equip_level_list[MaskParam::EQUIP_COUNT];		// 装备信息
 			short skill_level_list[MaskParam::SKILL_COUNT];		// 技能等级
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
				UInt16 star_level;												// 星星等级
				ItemID equip_id;												// 装备id
				UInt32 star_exp;												// 星星经验
				UInt16 fuling_count_list[MAX_ZHUANZHI_FULING_ATTR_COUNT];		// 附灵次数数据
				UInt16 xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];		// 仙品属性
			};

			UInt32 capability;													// 战斗力

			ZhuanzhiEquipItem zhuanzhi_equip_list[E_INDEX_ZHUANZHI::E_INDEX_MAX]; // 装备数据
			ZhuanzhiStonesPartData stone_list[E_INDEX_ZHUANZHI::E_INDEX_MAX]; 	  // 玉石数据
			char part_suit_type_list[E_INDEX_ZHUANZHI::E_INDEX_MAX];			  // 套装类型列表
			char part_order_list[E_INDEX_ZHUANZHI::E_INDEX_MAX];				  // 阶数列表
			unsigned char zhuanzhi_equip_awakening_lock_flag[MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE];	//锁标记
			ZhuanzhiEquipAwakening zhuanzhi_equip_awakening_list[E_INDEX_MAX];		// 觉醒信息
		};

		struct UpgradeSysInfo
		{
			short level;											// 等级
			short grade;											// 阶
			short used_imageid;										// 使用的形象
			short star_level;										// 星级
			int equip_skill_level;									// 装备技能等级
			int capability;											// 战斗力

			UInt16 shuxingdan_list[SHUXINGDAN_MAX_TYPE];			// 属性丹数量
			UInt8 active_img_flag[UPGRADE_MAX_IMAGE_BYTE];			// 形象激活标记
			short equip_level_list[UPGRADE_EQUIP_COUNT];			// 装备信息
			short skill_level_list[UPGRADE_SKILL_COUNT];			// 技能等级
			
		};

		struct ShenBingInfo
		{
			short level;
			short use_image;
			int capability;
			unsigned short shuxingdan_list[ShenBingParam::SHUXINGDAN_MAX_TYPE];	// 属性丹数量列表
		};

		struct EquipBaptizeInfo
		{
			int baptize_list[EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM];       // 属性值
			short attr_seq_list[EQUIP_BAPTIZE_ONE_PART_MAX_BAPTIZE_NUM];      // 属性seq
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

			int capability;														// 战斗力
			ItemID baizhan_equip_list[E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX];	// 装备数据
			UInt8 part_order_list[E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX];			// 阶数列表
			short reserve_sh;
		};

		int				role_id;									// 角色ID
		GameName		role_name;									// 角色名字
		long long		role_uuid;									// 角色UUID

		short			level;										// 等级
		char			is_online;									// 是否在线(1,0)
		char			sex;										// 性别

		char			camp_id;									// 阵营
		char			prof;										// 职业
		char			vip_level;									// VIP等级
		char			guild_post;									// 军团职位

		int				plat_type;									// 平台类型
		PlatName		plat_name;									// 平台名(64)

		GuildID			guild_id;									// 军团ID
		GuildName		guild_name;									// 军团名字

		long long		avatar_timestamp;							// 头像更新时间戳

		int				lover_uid;									// 伴侣uid
		GameName		lover_name;									// 伴侣名字
		int				qingyuan_value;								// 情缘值
		ItemID			qingyuan_equip_id;							// 情缘装备id
		short			qingyuan_equip_star_level;					// 情缘装备等级
		char			lover_prof;									// 伴侣职业
		char			lover_sex;									// 伴侣性别
		char			lover_camp;									// 伴侣阵营
		char			lover_vip_level;							// 伴侣VIP等级
		long long		lover_avatar_timestamp;						// 伴侣头像更新时间戳
		short			lover_level;								// 伴侣等级

		short			name_color;									// 名字颜色
		int				evil_val;									// 罪恶值
		int				all_charm;									// 魅力
		int				capability;									// 战斗力

		Attribute		hp;											// 血量
		Attribute		max_hp;										// 最大血量
		Attribute		gongji;										// 攻击
		Attribute		fangyu;										// 防御
		Attribute		mingzhong;									// 命中
		Attribute		shanbi;										// 闪避
		Attribute		baoji;										// 暴击
		Attribute		jianren;									// 抗暴
		Attribute		fujia_shanghai;								// 附加伤害
		Attribute		dikang_shanghai;							// 抵抗伤害
		Attribute		per_jingzhun;								// 精准
		Attribute		per_baoji;									// 暴击
		Attribute		per_kangbao;								// 抗暴
		Attribute		per_pofang;									// 破防百分比
		Attribute		per_mianshang;								// 免伤百分比
		Attribute		constant_zengshang;							// 固定增伤
		Attribute		constant_mianshang;							// 固定免伤
		Attribute		pvp_zengshang;								// pvp增伤
		Attribute		pvp_mianshang;								// pvp免伤
		Attribute		pve_zengshang;								// pve增伤
		Attribute		pve_mianshang;								// pve免伤
		Attribute		huixinyiji;									// 会心一击
		Attribute		huixinyiji_hurt_per;						// 会心一击伤害率
		Attribute		per_baoji_hurt;								// 暴击伤害万分比
		Attribute		per_kangbao_hurt;							// 暴击伤害抵抗万分比
		Attribute		zhufuyiji_per;								// 祝福一击率
		Attribute		gedang_per;									// 格挡率
		Attribute		gedang_dikang_per;							// 格挡抵抗率
		Attribute		gedang_jianshang;							// 格挡减伤
		Attribute		skill_zengshang;							// 技能增伤
		Attribute		skill_jianshang;							// 技能减伤
		Attribute		mingzhong_per;								// 命中率
		Attribute		shanbi_per;									// 闪避率
		Attribute		skill_cap_per;								// 技能战力万分比

		MountInfo		mount_info;									// 坐骑信息
		WingInfo		wing_info;									// 羽翼信息
		HaloInfo		halo_info;									// 光环信息
		ShengongInfo	shengong_info;								// 神弓信息
		ShenyiInfo		shenyi_info;								// 神翼信息

		EquipmentInfo	equipment_info[ItemNamespace::MAX_EQUIPMENT_GRID_NUM]; // 装备信息
		ShizhuangPart	shizhuang_part_list[SHIZHUANG_TYPE_MAX];	// 时装

		XiannvShouhuInfo xiannv_shouhu_info;						// 仙女守护
		JinglingFazhenInfo jingling_fazhen_info;					// 精灵法阵
		JinglingGuanghuanInfo jingling_guanghuan_info;				// 精灵光环
		ZhibaoInfo		zhibao_info;								// 至宝信息

		FightMountInfo	fight_mount_info;							// 战斗坐骑信息

		XiannvInfo xiannv_info;										// 出战仙女信息
		JinglingInfo jingling_info;									// 精灵信息
		LieMingInfo slot_list[LIEMING_FUHUN_SLOT_COUNT];			// 命魂信息

		StoneParam stone_param;										// 装备格子宝石

		FootprintInfo	footprint_info;								// 足迹信息
		FaBaoInfo fabao_info;                                       // 法宝信息
		CloakInfo	cloak_info;										// 披风信息
		short szlevel_list[SHENZHUANG_MAX_PART];					// 神装信息
		FeixianInfo	feixian_info[FeiXianParam::E_INDEX_MAX];		// 飞仙装备信息

		YaoShiInfo		yaoshi_info;								// 腰饰信息
		TouShiInfo		toushi_info;								// 头饰信息
		QiLinBiInfo		qilinbi_info;								// 麒麟臂信息
		MaskInfo		mask_info;									// 面具信息

		ImpGuardItem    impguard_info;							    // 守护小鬼信息
		Mojie           mojie_info;                                 // 魔戒信息
		ZhuanzhiEquipInfo zhuanzhi_equip_info;						// 转职装备信息

		UpgradeSysInfo upgrade_sys_info[UPGRADE_TYPE_MAX];			// 进阶类系统

		char shenbing_use_image_id;									// 神兵使用id
		char baojia_use_image_id;									// 宝甲使用id
		short reserve_sh;											// 字节对齐

		ShenBingInfo shenbing_info;                                 // 灵刃信息

		int total_jingjie;											// 境界等级

		EquipBaptizeInfo equip_baptize_info[EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM];   // 装备洗炼
		
		ImageItemInfo greate_soldier_info;							// 名将信息
		BaizhanEquipInfo baizhan_equip_info;						// 百战装备
		TianshenEquipID tianshenhuti_inuse_equip_list[TIANSHENHUTI_EQUIP_MAX_COUNT];	// 天神护体，每个部位对应装备ID
	};

	class CSAllInfoReq
	{
	public:
		CSAllInfoReq();
		MessageHeader	header;

		char is_send_chat_board;			// 是否下发聊天记录
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

		int				type;								// 福利类型
		int				param;								// 命令参数
	};

	class CSGetRoleCapability
	{
	public:
		CSGetRoleCapability();
		MessageHeader	header;
	};

	class SCRoleCapability				// 1414 角色战斗力
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

	//////////////////////////////////////  日常福利相关    ////////////////////////////////////
	class SCWelfareInfo										// 发送福利信息
	{
	public:
		enum NOTIFY_REASON									// 通知原因
		{
			WELFARE_INFO_DAFAULT = 0,

			ACCOUNT_LOGIN_INFO,								// 累计登陆信息
			OFFLINE_EXP_INFO,								// 离线经验信息

			ONLINE_GIFT_INFO,								// 在线礼物信息

			OFFLINE_EXP_NOTICE,								// 离线经验结算信息弹框提示
		};
		const static int ACTIVITY_FIND_TYPE_EXP_MAX = 32;
		SCWelfareInfo();
		MessageHeader header;

		int notify_reson;

		int offline_second;													// 离线时间
		//int offline_exp;													// 离线经验
		long long offline_exp;												// 离线经验
		int offline_mojing;													// 离线魔晶
		int exp_buff_effect_second;											// 经验BUFF生效时间
		int add_double_exp;													// 追加双倍可获得经验
		int exp_buff_effect_rate;											// 经验BUFF倍数
		int role_level_after_fetch;											// 领取离线经验升级后的等级

		int green_item_count;												// 离线奖励绿色物品数量
		int green_item_resolve_count;										// 离线奖励绿色物品分解数量
		int blue_item_count;												// 离线奖励蓝色物品数量
		int blue_item_resolve_count;										// 离线奖励蓝色物品分解数量
		int purple_item_count;												// 离线奖励紫色物品数量
		int purple_item_resolve_count;										// 离线奖励紫色物品分解数量
		int orange_item_count;												// 离线奖励橙色物品数量
		int orange_item_resolve_count;										// 离线奖励橙色物品分解数量
		int red_item_count;													// 离线奖励红色物品数量
		int red_item_resolve_count;											// 离线奖励红色物品分解数量
		int pink_item_count;												// 离线奖励粉色物品数量
		int pink_item_resolve_count;										// 离线奖励粉色物品分解数量
		int collect_item_count;												// 集字物品数量

		unsigned int sign_in_days;											// 签到天数
		unsigned int sign_in_reward_mark;									// 签到奖励标记
		unsigned int activity_find_flag;									// 活动找回标记
		unsigned int activity_join_flag;									// 活动参与标记
		unsigned int auto_activity_flag;									// 活动委托标记
		unsigned int today_online_time;										// 今天在线时长
		unsigned int online_reward_mark;									// 在线奖励领取标记
		char is_chongzhi_today;												// 今天是否充值
		char continuity_signin_days;										// 连续签到天数
		char sign_in_today_times;											// 今日签到次数
		char max_growth_value_get_type;										// happy_tree_grouth_val数组的大小
		int happy_tree_level;												// 欢乐果树等级
		short happy_tree_grouth_val[MAX_GROWTH_VALUE_GET_TYPE + 1];			// 欢乐果树长成值
		short happy_tree_reward;											// 欢乐果树奖励领取标记
		short chongjihaoli_reward_mark;										// 冲级豪礼领取标记
		int total_happy_tree_growth_val;									// 欢乐果树总成长值
		int accumulation_signin_days;										// 累计签到天数

		int total_chongjihaoli_record_list[MAX_CHONGJIHAOLI_RECORD_COUNT];  // 冲级豪礼各个等级全服的领取记录

		int role_login_level;												// 玩家登录等级
		long long exp_activityfind[ACTIVITY_FIND_TYPE_EXP_MAX];				// 活动找回经验
		short is_first_send;
		short reserve_sh;

		int daily_find_count;
		DailyFindRewardItem daily_find_list[DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS];	// 日常找回
		
	};

	class CSGetOfflineExp									// 获取离线经验
	{
	public:
		CSGetOfflineExp();
		MessageHeader header;

		int type;											// 获取类型
	};

	class CSGetDailyLoginWelfareInfo						// 请求每日登陆福利信息
	{
	public:
		CSGetDailyLoginWelfareInfo();
		MessageHeader header;
	};

	class CSGetDailyFindWelfare								// 获取日常找回
	{
	public:
		CSGetDailyFindWelfare();
		MessageHeader header;

		char dailyfind_type;
		char get_type;
		short reserved;
	};

	class SCDailyFindItemChange								// 日常找回改变
	{
	public:
		SCDailyFindItemChange();
		MessageHeader header;

		char dailyfind_type;
		char result;
		short reserved;
	};

	class SCTotalLoginDays									// 总登录天数
	{
	public:
		SCTotalLoginDays();
		MessageHeader header;

		int total_login_day;
	};

	class CSFetchSevenDayLoginReward						// 领取七天登录活动奖励
	{
	public:
		CSFetchSevenDayLoginReward();
		MessageHeader header;

		int fetch_day;										
	};

	class SCSevenDayLoginRewardInfo							// 七天登录奖励信息
	{
	public:
		enum NOTIFY_REASON									// 通知原因
		{
			SDLRI_DAFAULT = 0,

			SDLRI_FETCH_REWARD,								// 领取了七天登录奖励
		};

		SCSevenDayLoginRewardInfo();
		MessageHeader header;

		int notify_reason;									// 通知原因
		int account_total_login_daycount;					// 一生累计登录天数
		int seven_day_login_fetch_reward_mark;				// 七天累计登录奖励领取标记
	};

	class CSWelfareSignInReward								// 领取签到奖励
	{
	public:
		enum REQUEST_TYPE
		{
			DEFAULT_REQUEST = 0,

			PER_DAY_SIGNIN_REWARD,						// 领取每日签到奖励

			ACCUMULATION_REWARD,						// 领取累计和连续签到奖励
		};			
		CSWelfareSignInReward();
		MessageHeader header;

		short operate_type;
		short param;
		int is_quick;									// 1：一键补签（算补签），0：指定签到一天
	};

	class CSWelfareOnlineReward							// 领取在线奖励
	{
	public:
		CSWelfareOnlineReward();
		MessageHeader header;

		int part;										
	};

	class CSWelfareSignInFindBack						// 签到找回
	{
	public:
		CSWelfareSignInFindBack();
		MessageHeader header;

		short day;										
		short reserve_sh;
	};

	class CSWelfareActivityFind						// 活动找回
	{
	public:
		CSWelfareActivityFind();
		MessageHeader header;

		short find_type;										
		char is_free_find;							// 是否免费找回，1 是，0 否
		char reserve_ch;
	};

	class CSWelfareAutoActivity					// 活动委托
	{
	public:
		CSWelfareAutoActivity();
		MessageHeader header;

		short find_type;										
		short reserve_sh;
	};

	class CSWelfareFetchHappytreeReward			// 欢乐果树奖励
	{
	public:
		CSWelfareFetchHappytreeReward();
		MessageHeader header;

		int type;
	};

	class CSWelfareFetchChongjihaoliReward				// 领取冲级豪礼
	{
	public:
		CSWelfareFetchChongjihaoliReward();
		MessageHeader header;

		int level;
	};

	class SCExpBuffInfo							// 经验buff信息 8938
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

	// 名字修改
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

	class CSSpeedUpHello		//  加速检测心跳
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
			OP_MOUNT_UPGRADE = 1,						// 坐骑进阶
			OP_QIBING_UPGRADE = 2,						// 骑兵打造
			OP_XIANNV_PROMOTE_LEVEL = 3,				// 仙女等级
			OP_XIANNV_PROMOTE_YUANSH = 4,				// 仙女升级元神
			OP_XIANNV_PROMOTE_ZIZHI = 5,				// 仙女提升资质
			OP_WING_PROMOTE = 6,						// 羽翼进化
			OP_EQUIP_STRENGTHEN = 7,					// 装备强化
			OP_STONE_UPLEVEL = 8,						// 宝石升级
			OP_FISH_POOL_EXTEND_CAPACITY = 9,			// 鱼池扩展成功
			OP_WING_UPGRADE = 10,						// 羽翼升阶
			OP_MOUNT_FLYUP = 11,						// 坐骑飞升
			OP_XIANNV_HALO_UPGRADE = 12,				// 仙女守护升星
			OP_WUSHAUNG_EQUIP_DAZAO = 13,				// 无双装备打造
			OP_XIANNV_JIE_UPGRADE = 14,					// 仙女升阶
			OP_XIANJIAN_JIE_UPGRADE = 15,				// 仙剑升阶
			OP_EQUIP_UP_STAR_SUCC = 16,					// 装备升星成功
			OP_JINGLING_UPGRADE = 17,					// 精灵升阶
			OP_SHENZHUANG_JINJIE = 18,					// 神装进阶 
			OP_BABY_JIE_UPGRADE = 19,					// 宝宝进阶  
			OP_PET_JIE_UPGRADE = 20,					// 宠物进阶(灵器)
			OP_QINGYUAN_EQUIP_UPLEVEL = 21,				// 婚戒升级
			OP_HUASHEN_UPLEVEL = 22,					// 化神升级
			OP_PET_QINMI_PROMOTE = 23,					// 宠物亲密度
			OP_MULTI_MOUNT_UPGRADE = 24,				// 双人坐骑升级
			OP_WUSHANG_EQUIP_UPSTAR = 25,				// 无上神兵升星
			OP_JINGLING_HALO_UPSTAR = 26,				// 精灵光环升阶
			OP_FAIRY_TREE_UPGRADE = 27,					// 仙树升级
			OP_MAGIC_EQUIPMENT_STRENGTHEN = 28,			// 魔器强化
			OP_HALO_UPGRADE = 29,						// 光环升阶
			OP_SHENGONG_UPGRADE = 30,					// 神弓升阶
			OP_SHENYI_UPGRADE = 31,						// 神翼升阶
			OP_XIANNV_PROMOTE_HUANHUA_LEVEL = 32,		// 仙女幻化等级
			OP_JINGLING_FAZHEN_UPGRADE = 33,			// 精灵法阵升阶
			OP_SHENGONG_UPSTAR = 34,					// 神弓升星
			OP_SHENYI_UPSTAR = 35,						// 神翼升星
			OP_HUASHEN_UPGRADE_SPIRIT = 36,				// 化神精灵进阶
			OP_FIGHT_MOUNT_UPGRADE = 37,				// 战斗坐骑进阶
			OP_LIEMING_CHOUHUN = 38,					// 命魂猎命
			OP_MOUNT_UPSTAR = 39,						// 坐骑升星
			OP_WING_UPSTAR = 40,						// 羽翼升星
			OP_HALO_UPSTAR = 41,						// 光环升星
			OP_FIGHT_MOUNT_UPSTAR = 42,					// 战骑升星		
			OP_SHENBING_UPLEVEL = 43,					// 神兵升级	
			OP_SHENZHOU_WEAPON = 44,					// 神州六器
			OP_UP_ETERNITY = 45,						// 永恒装备
			OP_RA_MAPHUNT_AUTO_FLUSH = 46,				// 地图寻宝自动刷新
			OP_FOOTPRINT_UPGRADE = 47,					// 足迹升阶
			OP_FOOTPRINT_UPSTAR = 48,					// 足迹升星
			OP_CLOAK_UP_LEVEL = 49,						// 披风升级
			OP_SHEN_YIN_LIEHUN = 50,					// 神印系统猎魂
			OP_FISHING_REQ = 50,						// 钓鱼请求
			OP_STEAL_FISH_RESULT = 51,					// 偷鱼结果
			OP_ELEMENT_HEART_UPGRADE = 52,				// 元素进阶
			OP_ELEMENT_TEXTURE_UPGRADE = 53,			// 元素之纹进阶
			OP_ELEMENT_EQUIP_UPGRADE = 54,				// 元素装备进阶
			OP_SHIZHUANG_UPGRADE = 55,					// 时装（衣服）进阶
			OP_FABAO_UPGRADE = 56,					    // 法宝进阶
			OP_FABAO_SPECIAL_IMG_UPGRADE = 57,		    // 法宝特殊形象进阶
			OP_SHIZHUANG_WEAPON_UPGRADE = 58,			// 时装（武器）进阶
			OP_FABAO_UPLEVELSKILL = 59,                 // 法宝技能升级
			OP_FABAO_UPLEVELEQUIP = 60,                 // 法宝装备升级
			OP_FEIXIAN_COMPOSE = 61,                    // 飞仙装备合成
			OP_FEIXIAN_UPLEVEL = 62,                    // 飞仙装备升级
			OP_BABY_JL_UPGRADE = 63,					// 宝宝精灵进阶
			OP_ONEKEY_LIEMING_GAIMING = 64,				// 一键猎命改命
			OP_ONEKEY_LIEMING_CHOUHUN = 65,				// 一键猎命抽魂
			OP_YAOSHI_UPGRADE = 66,						// 腰饰进阶
			OP_TOUSHI_UPGRADE = 67,						// 头饰进阶
			OP_QILINBI_UPGRADE = 68,					// 麒麟臂进阶
			OP_MASK_UPGRADE = 69,						// 面具进阶
			OP_ZHUANZHI_STONE_REFINE = 70,				// 玉石精炼
			OP_ZHUANZHI_FULING = 71,					// 转职装备附灵
			OP_RA_FANFAN_REFRESH = 72,					// 随机活动翻翻乐刷新
			OP_SHENQI_SHENGBING_UPLEVEL = 73,			// 神兵升级
			OP_SHENQI_BAOJIA_UPLEVEL = 74,				// 宝甲升级
			OP_DELETE_PROFESS = 75,						// 删除表白
			OP_UPLEVEL_RUNE = 76,						// 符文升级
			OP_JINGLING_SKILL_REFRESH = 77,				// 精灵技能刷新
			OP_GREATE_SOLDIER_SLOT_UPLEVEL = 78,		// 名将槽位升级
			OP_JINGLING_LINGHUN_UPLEVEL = 79,			// 精灵灵魂升级

			OP_UPGRADE_SYS_UPGRADE = 200,				// 进阶系统进阶结果 (200 + upgrade_type)
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

	class SCMoveChessInfo											// 1720走棋子信息
	{
	public:
		SCMoveChessInfo();
		MessageHeader	header;

		short move_chess_free_times;
		short move_chess_reset_times;
		int move_chess_cur_step;
		unsigned int move_chess_next_free_time;
	};

	class SCMoveChessStepRewardInfo									// 1723走棋子每步奖励信息
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

		SPACE_MSG_TYPE_LIUYAN,										// 留言
		SPACE_MSG_TYPE_CAI,											// 踩一踩 记录
		SPACE_MSG_TYPE_GIFT,										// 礼物 记录
		
		SPACE_MSG_TYPE_XINQING,										// 心晴 记录

		SPACE_MSG_TYPE_MAX,
	};

	class CSMillionaireInfoReq										// 1721请求大富豪采集信息
	{
	public:
		CSMillionaireInfoReq();
		MessageHeader	header;
	};

	class SCMillionaireInfo											// 1722大富豪采集信息
	{
	public:
		SCMillionaireInfo();
		MessageHeader	header;

		short gather_total_times;
		char is_millionaire;										// 1表示抽到大富豪，0没有抽到
		char is_turn;												// 0没有启动过转盘，1已启动过转盘
		short reward_index;											// 大富豪奖励索引
		short millionaire_use_skill_times;							// 冰冻技能使用次数
		unsigned int millionaire_valid_perform_skill_time;			// 冰冻技能可以使用时间
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

		int space_renqi;							// 空间人气
		int space_getgift_count;					// 空间获得礼物次数
		int space_day_cai_addhuoli;					// 空间每日踩获得的活力
		int space_huoli;							// 空间活力
		SpaceMessage space_xinqing;					// 空间心情
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
			RIREASON_START = 0,							// 开始翻牌
			RIREASON_ROLL = 1,							// 翻牌结果
			RIREASON_END = 2,							// 翻牌结束
		};
		short reason;
		char phase;
		char hit_seq;									// 命中物品序号，即数组下标

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
		CSA_OPERATE_TYPE_UP_COMMON_LEVEL = 1,			//普通装备升级
		CSA_OPERATE_TYPE_UP_GREAT_LEVEL = 2,			//传世装备升级
	};

	class CSCSAEquipOpera								//1881
	{
	public:
		CSCSAEquipOpera();
		MessageHeader header;

		int operate;									//操作类型
		int param;										//装备部位
	};

	class SCCSAEquipInfo								//1880
	{
	public:
		SCCSAEquipInfo();
		MessageHeader header;

		short common_level_list[CSA_EQUIP_MAX_PART];	//普通装备等级
		short great_level_list[CSA_EQUIP_MAX_PART];		//传世装备等级
		short active_special;							//是否激活特权,0不是1是
		char is_active_sys;								// 是否合服激活
		char reserve_ch;								// 
	};

	class SCCSAActivePower								//1882
	{
	public:
		SCCSAActivePower();
		MessageHeader header;

		unsigned short obj_id;
		short csa_active_special;						//是否激活特权,0不是1是
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
		CAMPEQUIP_OPERATE_TYPE_REQ_INFO = 0,	//请求信息
		CAMPEQUIP_OPERATE_TYPE_TAKEOFF,			//脱下
		CAMPEQUIP_OPERATE_TYPE_HUNLIAN,			//魂炼
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
		CAMP_NORMALDUOBAO_OPERATE_ENTER = 0,	//请求进入师门普通夺宝
		CAMP_NORMALDUOBAO_OPERATE_EXIT,			//请求退出师门普通夺宝
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
		OPERA_TYPE_ACTIVE = 0,               // 激活
		OPERA_TYPE_FETCH_RETURN_REWARD,		 // 领取返还奖励
		OPERA_TYPE_CONVERT_SHOP,             // 兑换商店

		OPERA_TYPE_MAX,
	};

	class CSGoldVipOperaReq    //  黄金会员操作请求
	{
	public:
		CSGoldVipOperaReq();
		MessageHeader	header;

		short			type;								// 操作类型
		short			reserve_sh;
		short			param1;								// 命令参数
		short			param2;								// 命令参数2
	};

	class SCGoldVipInfo     //  黄金会员信息返回
	{
	public:
		SCGoldVipInfo();
		MessageHeader	header;

		char gold_vip_shop_counts_list[GOLD_VIP_CONVERT_SHOP_MAX_COUNT];
		unsigned int gold_vip_active_timestamp;		// 激活时间戳
		int day_score;								// 每日积分
		char shop_active_grade_flag;				// 商店激活档次标记
		char can_fetch_return_reward;				// 能否领取返还奖励
		char is_not_first_fetch_return_reward;		// 是否不是第一次领取返还奖励
		char reserver_1;
	};

	class SCWorldBossWearyInfo
	{
	public:
		SCWorldBossWearyInfo();
		MessageHeader	header;

		int worldboss_weary;
		unsigned int worldboss_weary_last_die_time;          // 最近一次死亡时间
		unsigned int worldboss_weary_last_relive_time;       // 最近一次免费复活时间
	};
	
	enum JINJIESYS_REWARD_OPEAR_TYPE
	{
		JINJIESYS_REWARD_OPEAR_TYPE_INFO = 0,		// 获取信息
		JINJIESYS_REWARD_OPEAR_TYPE_BUY = 1,		// 购买，param_0 = 进阶系统类型, raram_1 = 奖励类型
		JINJIESYS_REWARD_OPEAR_TYPE_FETCH = 2,		// 领取进阶奖励，param_0 = 进阶系统类型, raram_1 = 奖励类型
	};

	class CSJinjiesysRewardOpera		//1824
	{
	public:
		CSJinjiesysRewardOpera();
		MessageHeader header;

		short opera_type;				//操作类型
		short param_0;
		short param_1;
		short param_2;
	};

	class SCJinjiesysRewardInfo			//1825
	{
	public:
		SCJinjiesysRewardInfo();
		MessageHeader header;

		unsigned int reward_flag;		//获取(购买)奖励标记
		unsigned int can_reward_flag;	//可以领取标记
		unsigned int reward_flag_1;		//获取(购买)奖励标记
		unsigned int can_reward_flag_1;	//可以领取标记
	};

	class SCJinjiesysRewardTimestamp	//1826
	{
	public:
		SCJinjiesysRewardTimestamp();
		MessageHeader header;

		static const int JINJIE_SYS_COUNT = 64;
		struct SysTimestamp
		{
			short reward_type;				//奖励类型
			short sys_type;					//系统类型
			unsigned int end_timestamp;		//结束时间戳
		};

		int use_sys_count;
		SysTimestamp timestamp_list[JINJIE_SYS_COUNT];		//全部系统的时间戳
	};

	//===============================================================
	//杂碎简单的协议可以用下面通用的。其他协议写在上面!!!!!
	//===============================================================
	// 客户端操作请求类型
	enum COMMON_OPERATE_TYPE
	{
		COT_JINGHUA_HUSONG_COMMIT = 1,				// 精华护送提交
		COT_JINGHUA_HUSONG_INFO_REQ = 2,			// 精华护送请求信息
		COT_KEY_ADD_FRIEND = 3,						// 一键踩好友空间
		COT_REQ_RED_EQUIP_COLLECT_TAKEON = 4,				// 红装收集，请求穿上，param1是红装seq，param2是红装槽index， param3是背包index
		COT_REQ_RED_EQUIP_COLLECT_FETCH_ATC_REWARD = 5,		// 红装收集，领取开服活动奖励，param1是奖励seq
		COT_REQ_RED_EQUIP_COLLECT_FETCH_TITEL_REWARD = 6,	// 红装收集领取称号奖励, param1是奖励seq
		COT_REQ_ORANGE_EQUIP_COLLECT_TAKEON = 7,			// 橙装收集, 请求穿上，param1是红装seq，param2是红装槽index， param3是背包index
		COT_REQ_PERSONAL_BOSS_BUY_TIMES = 8,                // 个人boss购买次数

		COT_ACT_BUY_EQUIPMENT_GIFT = 1000,			// 活动 购买装备礼包
	};

	// 服务器通知客户端信息类型
	enum SC_COMMON_INFO_TYPE
	{
		SCIT_JINGHUA_HUSONG_INFO = 1,				// 精华护送信息		
		SCIT_RAND_ACT_ZHUANFU_INFO = 2,				// 随机活动专服信息
		SCIT_TODAY_FREE_RELIVE_NUM = 3,				// 每天已经使用的免费复活次数
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


	///////////////////////////////////// 角色境界 ////////////////////////////
	enum CS_ROLE_JINGJIE_OPERA
	{
		CS_ROLE_JINGJIE_OPERA_PROMOTE_LEVEL = 0,	// 提升境界
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
		ROLE_APPE_CHANGE_TYPE_WUQICOLOR = 0,       //转职装备武器颜色（首充武器相关）
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

	class SCFightBackRoleList				// 8615 下发反击列表玩家uid
	{
	public:
		SCFightBackRoleList();
		MessageHeader header;

		enum 
		{
			NOTIFY_LIST_ADD = 1,	// 添加
			NOTIFY_LIST_DEL = 2,	// 删除
		};
		
		static const int MAX_ROLE_LIST_NUM = 50;

		short notify;		// enum 通知类型枚举
		short count;
		int role_uid_list[MAX_ROLE_LIST_NUM];
	};
}

#pragma pack(pop)

#endif 

