#ifndef _QUERYHANDLER_HPP_
#define _QUERYHANDLER_HPP_

#include "servercommon/struct/rolequeryparam.hpp"
#include <map>
#include "servercommon/struct/fabaoparam.hpp"
#include "servercommon/struct/shenbingparam.hpp"

class Role;

struct OfflineQueryCacheNode
{
	struct EquipmentInfo
	{
		EquipmentInfo() : equip_id(0), star_level(0), has_lucky(0), quality(0), strengthen_level(0), shen_level(0), fuling_level(0), grid_strengthen_level(0), grid_shen_level(0)
		{
			memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
		}

		ItemID equip_id;
		short star_level;
		short has_lucky;

		short quality;										// 品质
		short strengthen_level;								// 强化等级
		short shen_level;									// 神铸等级
		short fuling_level;									// 附灵等级

		short grid_strengthen_level;						// 格子强化等级
		short grid_shen_level;								// 格子神铸等级
		short grid_star_level;								// 格子星星等级
		short eternity_level;								// 格子永恒等级

		unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];
	};

	OfflineQueryCacheNode() : role_id(0), role_uuid(0), sex(0), camp(0), prof(0), vip_level(0), level(0), guild_id(0)
	{
		memset(role_name, 0, sizeof(role_name));
	}

	int				role_id;								// ID
	long long		role_uuid;								// ID
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

	HaloParam halo_param;
	ShengongParam shengong_param;
	ShenyiParam shenyi_param;
	FootprintParam footprint_param;
	CloakParam cloak_param;

	XiannvShouhuParam xiannv_shouhu;
	JinglingFazhenParam jingling_fazhen;
	JinglingGuanghuanParam jingling_guanghuan;
	FightMountParam fight_mount_param;
	ZhiBaoParam zhibao_param;

	EquipmentInfo equipment_info[ItemNamespace::MAX_EQUIPMENT_GRID_NUM];

	XiannvParam xiannv_param;
	JingLingParam jingling_param;
	LieMingParam lieming_param;
	ShenZhuangParam shenzhuang_param;
	FeiXianParam feixian_param;
	FabaoParam fabao_param;

	YaoShiParam	yaoshi_param;
	TouShiParam	toushi_param;
	QiLinBiParam qilinbi_param;
	MaskParam mask_param;
	MojieParam mojie_param;
	ZhuanzhiEquipParam zhuanzhi_equip_param;

	UpgradeManagerParam upgrade_manager_param;

	ShenBingParam shenbing_param;

	EquipBaptizeParam equipbaptize_param;
	GreateSoldierParam greate_soldier_param;

	BaizhanEquipParam baizhan_equip_param;
	TianshenhutiParam tianshenhuti_param;
};

enum QUERY_FROM
{
	QUERY_FROM_CLIENT = 0,			// 客户端请求
	QUERY_FROM_CROSS,				// 跨服
	QUERY_FROM_CAMPDIAOXIANG,		// 雕象守卫	
};

class QueryHandler
{
public:
	static QueryHandler & Instance();

	void OnQuery(int target_uid, int query_from, int viewer_uid, int plat_type);

	void OnQueryBack(int query_from, UserID viewer_userid, int plat_type, int target_userid, int ret, const RoleQueryParam &p, const ItemListParam &item_list_param);

	void OnBaseInfoChange(Role *role);
	void OnUserLogout(Role *role);

private:
	QueryHandler();
	~QueryHandler();

	void OnQueryOnline(int query_from, int viewer_uid, int plat_type, Role *target_role);

	void SendInfoToViewer(int query_from, int viewer_uid, int plat_type, OfflineQueryCacheNode *cache_node);

	void GetInfoToCacheNode(const RoleQueryParam &p, const ItemListParam &item_list_param, OfflineQueryCacheNode *cache_node);
	void GetInfoToCacheNode(Role *target_role, OfflineQueryCacheNode *cache_node);

	bool NeedToCache(UserID user_id);

	std::map<int, OfflineQueryCacheNode> m_cache_map;
};

#endif


