#ifndef CHARAINITPARAM_H
#define CHARAINITPARAM_H

#include <string>

#include "common/tlvprotocol.h"

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"
#include "gameworld/gameworld/gamedef.h"

#include "servercommon/struct/itemlistparam.h"

// 这个结构绝对不允许包含string这种东西 因为这个结构会memset的

class RoleInitParam 
{
public:
	static const int MYSTERIOUS_SHOP_INFO_NUM = 3;			// 神秘商店存储

	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	// DB中的Buff(存在最低版本到最高版本的数据) 内存中数据 (最高版本数据描述) 必须兼容
 	bool Serialize(TLVSerializer &outstream, bool ver_check) const;
	bool Unserialize(TLVUnserializer &instream, bool ver_check);  

	int				role_id;								// ID
	GameName		role_name;								// 名字
	char			avatar;									// 头像
	char			sex;									// 性别
	char			camp;									// 所属阵营
	char			camp_post;								// 阵营职位
	char			prof;									// 职业
	char			authority_type;							// 权限 
	int				level;									// 等级

	int				scene_id;								// 场景ID
	int				scene_x;								// X
	int				scene_y;								// Y
	int				last_scene_id;							// 上次普通场景ID
	int				last_scene_x;							// X
	int				last_scene_y;							// Y

	int				online_time;							// 在线时长
	int				today_online_time;						// 今日在线时长
	int				lastday_online_time;					// 昨日在线时长
	long long		create_time;							// 创建时间
	
	Int64			coin;									// 非绑铜币
	Int64			coin_bind;								// 绑定铜币
	Int64			gold;									// 元宝
	Int64			gold_bind;								// 绑定元宝

	int				knapsack_valid_num;						// 背包可用格子数
	int				storage_valid_num;						// 仓库可用格子数

	long long		forbid_talk_time;						// 禁言时间

	unsigned int	last_online_dayid;						// 上次更新的dayid

	Attribute		hp;										// 血量
	Attribute		max_hp;									// 最大血量
	Attribute		mp;										// 魔法
	Attribute		max_mp;									// 最大魔法
	Attribute		gongji;									// 攻击
	Attribute		fangyu;									// 防御
	Attribute		mingzhong;								// 命中
	Attribute		shanbi;									// 闪避
	Attribute		baoji;									// 暴击
	Attribute		jianren;								// 坚韧
	Attribute		move_speed;								// 移动速度
	Attribute		fujia_shanghai;							// 附加伤害
	Attribute		dikang_shanghai;						// 抵抗伤害
	Attribute		per_jingzhun;							// 精准
	Attribute		per_baoji;								// 暴击率
	Attribute		per_kangbao;							// 抗暴率
	Attribute		per_pofang;								// 破防率
	Attribute		per_mianshang;							// 免伤率
	Attribute		constant_zengshang;						// 固定增伤
	Attribute		constant_mianshang;						// 固定免伤
	Attribute		pvp_zengshang;							// pvp增伤
	Attribute		pvp_mianshang;							// pvp免伤
	Attribute		pve_zengshang;							// pve增伤
	Attribute		pve_mianshang;							// pve免伤
	Attribute		huixinyiji;								// 会心一击
	Attribute		huixinyiji_hurt_per;					// 会心一击伤害率
	Attribute		per_baoji_hurt;							// 暴击伤害万分比
	Attribute		per_kangbao_hurt;						// 暴击伤害抵抗万分比
	Attribute		zhufuyiji_per;							// 祝福一击率
	Attribute		gedang_per;								// 格挡率
	Attribute		gedang_dikang_per;						// 格挡抵抗率
	Attribute		gedang_jianshang;						// 格挡减伤
	Attribute		skill_zengshang;						// 技能增伤
	Attribute		skill_jianshang;						// 技能减伤
	Attribute		mingzhong_per;							// 命中率
	Attribute		shanbi_per;								// 闪避率
	Attribute		skill_cap_per;							// 技能战力万分比
	
	GuildID			guild_id;								// 军团ID
	GameName		guild_name;								// 军团名字
	char			guild_post;								// 军团职位
	char			reserve_ch;								// 保留1
	short			reserve_sh;								// 保留2
	int				guild_gongxian;							// 军团贡献
	int				guild_total_gongxian;					// 军团总贡献

	long long		exp;									// 经验
	long long		max_exp;								// 最大经验

	char			attack_mode;							// 攻击模式

	char			vip_level;								// VIP等级

	DayCountData	day_count_data;							// 每日计数

	int				capability;								// 战斗力
	int				max_capability;							// 历史最高战斗力

	int				evil;									// 罪恶值
	unsigned int	last_kill_me_time;						// 上次杀死我的时间
	int				yuanli;									// 元力
	int				m_yuanli;								// 元力
	int				nv_wa_shi;								// 女娲石
	int				lingjing;								// 灵晶
	int				chengjiu;								// 成就
	int				fenchen;								// 粉尘

	char			move_mode;								// 运动模式
	char			move_mode_param;						// 运动模式参数

	SublockString	sublock_pw;								// 二级锁密码
	char			sublock_has_lock;						// 二级锁是否锁上
	char			sublock_today_error_times;				// 二级锁今天已尝试失败次数

	int				all_give_flower_num;					// 总送花数
	int				day_give_flower_num;					// 今日送花数
	int				all_charm;								// 总魅力

	char			is_change_avatar;						// 是否改变过头像
	Attribute		gm_change_maxhp;						// GM命令改变的气血
	Attribute		gm_change_gongji;						// GM命令改变的攻击
	Attribute		gm_change_movespeed;					// GM命令改变移动速度		
	Attribute		gm_change_fangyu;						// GM命令改变防御
	Attribute		gm_chnage_mingzhong;					// GM命令改变命中
	Attribute		gm_change_capability_attr;				// GM命令改变战力，设置属性
// 	Attribute		gm_chnage_baoji;						// GM命令改变暴击
// 	Attribute		gm_chnage_jianren;						// GM命令改变坚韧
// 
// 	Attribute		gm_change_fujia_shanghai;				// GM命令改变的附加伤害
// 	Attribute		gm_change_dikang_shanghai;				// GM命令改变的抵抗伤害
// 	Attribute		gm_change_per_jingzhun;					// GM命令改变的精准
// 	Attribute		gm_change_per_baoji;					// GM命令改变的暴击
// 	Attribute		gm_change_per_kangbao;					// GM命令改变的抗暴
// 	Attribute		gm_change_per_pofang;					// GM命令改变的破防
// 	Attribute		gm_change_per_mianshang;				// GM命令改变的免伤
// 	Attribute		gm_change_constant_zengshang;			// GM命令固定增伤
// 	Attribute		gm_change_constant_mianshang;			// GM命令固定免伤

	long long 		avatar_timestamp;						// 头像更新时间戳

	// 下面的变量未存盘
	int				honour;									// 阵营荣誉
	
	char			is_mount_bianshen;						// 是否坐骑变身
	unsigned int	mount_bianshen_time;					// 坐骑变身时间

	int				skill_appearance;						// 技能外观
	unsigned int	skill_appearance_end_time;				// 技能外观结束时间
	int				item_appearance;						// 物品外观
	int				item_appearance_param;					// 物品外观参数
	unsigned int	item_appearance_end_time;				// 物品外观结束时间

	int				huashen_appearance;						// 化神外观
	int				huashen_appearance_param;				// 化神外观参数
	unsigned int	huashen_appearance_end_time;			// 化神外观结束时间

	int				greate_soldier_appearance;				// 名将外观
	int				greate_soldier_appearance_param;		// 名将外观参数
	unsigned int	greate_soldier_appearance_end_time;		// 名将外观结束时间

	int				team_index;								// 队伍索引
	int				team_seq;								// 队伍序列号
	char			has_send_all_info;						// 是否执行了OnAllInfoReq
	char			is_auto_join_team;						// 是否自动加入队伍

	unsigned int	login_timestamp;						// 登录时间戳

	//求婚缓存信息 -------------------------------------------------------------------------------------------
	int m_marry_type;						// 结婚类型
	int m_lover_id;							// 爱人id
	int m_shici_type;						// 誓词类型
	int m_marry_req_id;						// 结婚请求发起者的id（用来最后判断消耗的时候用）
	char m_is_sure;							// 是否有按手印
};

#endif

