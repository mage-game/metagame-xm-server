
#ifndef __ROLE_QUERY_PARAM_HPP__
#define __ROLE_QUERY_PARAM_HPP__

#include <string>

#include "common/tlvprotocol.h"
#include "servercommon/basedef.h"
#include "servercommon/mentalitydef.hpp"
#include "servercommon/frienddef.hpp"

#include "servercommon/vipdef.hpp"
#include "servercommon/roleshopdef.hpp"
#include "servercommon/activedegreedef.hpp"
#include "servercommon/roleactivitydef.hpp"
#include "servercommon/welfaredef.hpp"
#include "servercommon/rolehangupdef.hpp"
#include "servercommon/itemcolddowndef.hpp"
#include "servercommon/struct/shizhuangparam.hpp"

#include "servercommon/evaluatedef.hpp"

#include "servercommon/monitordef.hpp"
#include "servercommon/titledef.hpp"
#include "servercommon/taskdef.hpp"
#include "servercommon/mysteriousshopdef.hpp"			

#include "servercommon/fbdef.hpp"
#include "servercommon/guilddef.hpp"
#include "servercommon/struct/itemlistparam.h"

#include "servercommon/struct/mountparam.hpp"
#include "servercommon/stonedef.hpp"
#include "servercommon/commondata.hpp"
#include "servercommon/taskdef.hpp"
#include "servercommon/xianjiandef.hpp"
#include "servercommon/wingdef.hpp"
#include "servercommon/halodef.hpp"
#include "servercommon/shengongdef.hpp"
#include "servercommon/shenyidef.hpp"
#include "servercommon/footprintdef.hpp"
#include "servercommon/cloakdef.hpp"
#include "servercommon/struct/roleinitparam.h"
#include "servercommon/qingyuandef.hpp"
#include "servercommon/petdef.hpp"
#include "servercommon/xiannvshouhudef.hpp"
#include "servercommon/jinglingfazhendef.hpp"
#include "servercommon/jinglingguanghuandef.hpp"
#include "servercommon/struct/fightmountparam.hpp"
#include "servercommon/struct/zhibaoparam.hpp"
#include "servercommon/struct/xiannvparam.hpp"
#include "servercommon/jinglingdef.hpp"
#include "servercommon/liemingdef.hpp"
#include "servercommon/shenzhuangdef.hpp"
#include "servercommon/feixianparam.hpp"
#include "fabaoparam.hpp"
#include "servercommon/struct/yaoshiparam.hpp"
#include "servercommon/struct/toushiparam.hpp"
#include "servercommon/struct/qilinbiparam.hpp"
#include "servercommon/struct/maskparam.hpp"
#include "servercommon/zhuanzhiequipdef.hpp"
#include "mojieparam.hpp"
#include "servercommon/struct/upgradeparam.hpp"
#include "servercommon/struct/jinjiesysrewardparam.hpp"
#include "shenbingparam.hpp"
#include "servercommon/equipbaptizedef.hpp"
#include "servercommon/greatesoldierdef.hpp"
#include "servercommon/baizhanequipdef.hpp"
#include "servercommon/tianshenhutidef.hpp"

class RoleQueryParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	int				role_id;								// ID
	GameName		role_name;								// 名字
	char			sex;									// 性别
	char			camp;									// 所属阵营
	char			prof;									// 职业
	char			vip_level;
	short			level;
	GuildID			guild_id;
	long long		avatar_timestamp;

	TitleParam title_param;
	CommonDataParam common_data_param;
	WingParam wing_param;
	RoleInitParam role_init_param;

	QingyuanParam qingyuan_param;
	ShizhuangParam shizhuang_param;

	MountParam mount_param;
	StoneParam stone_param;

	PetParam pet_param;
	EquipmentGridParam equipment_param;

	HaloParam halo_param;
	ShengongParam shengong_param;
	ShenyiParam shenyi_param;
	XiannvShouhuParam xiannv_shouhu_param;
	JinglingFazhenParam jingling_fazhen_param;
	JinglingGuanghuanParam jingling_guanghuan_param;

	FightMountParam fight_mount_param;

	XiannvParam xiannv_param;
	JingLingParam jingling_param;
	LieMingParam lieming_param;

	FootprintParam footprint_param;
	FabaoParam fabao_param;
	CloakParam cloak_param;
	ShenZhuangParam shenzhuang_param;
	FeiXianParam feixian_param;

	YaoShiParam	yaoshi_param;
	TouShiParam	toushi_param;
	QiLinBiParam qilinbi_param;
	MaskParam mask_param;
	MojieParam mojie_param;
	ZhuanzhiEquipParam zhuanzhi_equip_param;

	UpgradeManagerParam upgrade_manager_param;
	JinjieSysRewardParam jinjiesys_reward_param;

	ShenBingParam shenbing_param;
	LieMingParam2 lieming_param2;

	EquipBaptizeParam equipbaptize_param;
	GreateSoldierParam greate_soldier_param;

	BaizhanEquipParam baizhan_equip_param;

	mutable DataPageParam data_page_10;
};

#endif // __ROLE_OTHER_INIT_PARAM_HPP__

