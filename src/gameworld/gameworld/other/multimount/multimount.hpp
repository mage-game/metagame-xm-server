#ifndef __MULTI_MOUNT_H__
#define __MULTI_MOUNT_H__

class Role;

#include "obj/character/attribute.hpp"
#include "servercommon/multimountdef.hpp"
#include "servercommon/newmultimountdef.hpp"
#include <map>

static const int MULTI_MOUNT_INVITE_MAX_DISTANCE = 20;						// 发出邀请最大允许距离

enum MULTI_MOUNT_RIDE_STATUS
{
	MULTI_MOUNT_RIDE_STATUS_NONE = 0,										// 无状态，未骑乘
	MULTI_MOUNT_RIDE_STATUS_OWNER_RIDE,										// 拥有者单人骑乘
	MULTI_MOUNT_RIDE_STATUS_SEND_REQUEST,									// 发出邀请未回复状态
	MULTI_MOUNT_RIDE_STATUS_TOW_RIDE,										// 双人骑乘状态
};

struct MultiMountBindItem													// 双人坐骑绑定对象
{
	MultiMountBindItem() : owner_uid(0), follower_uid(0), invite_uid(0), owner_obj_id(INVALID_OBJ_ID), follower_obj_id(INVALID_OBJ_ID), mount_id(-1), res_id(0), mount_star_level(0), status(MULTI_MOUNT_RIDE_STATUS_NONE) {}

	int owner_uid;															// 拥有者玩家ID
	int follower_uid;														// 搭乘者玩家ID
	int invite_uid;															// 邀请对方玩家ID
	ObjID owner_obj_id;
	ObjID follower_obj_id;

	int mount_id;															// 坐骑ID
	int res_id;																// 资源ID
	int mount_star_level;														// 坐骑阶数（用于计算坐骑速度）
	int status;																// 骑乘状态
};

class MultiMount
{
public:
	MultiMount();
	~MultiMount();

	void SetRole(Role *role) { m_role = role; } 
	void Init(Role *role, const MultiMountParam &param);
	void GetInitParam(MultiMountParam *param);
	void Init2(Role *role, const CoupleMountParam &param);
	void GetInitParam2(CoupleMountParam *param);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void Update(unsigned int now_second);
	void OnRoleLogin();
	void OnLogout();
	void OnEnterScene(int scene_id);

	int GetCurUseResID();

	void OnMountReq(int type, int param_1 = 0, int param_2 = 0, int param_3 = 0);

	void GMActiveAllImg();

	bool IsOnMount();
	void MountOn(bool is_ride);

	MultiMountBindItem * GetMountOnInfo(int uid = 0);

	void SendAllInfoToRole();
	void AreaBroadcastOwnerRideInfo(int follower_uid = 0, ObjID follower_obj_id = INVALID_OBJ_ID);

	bool OnUseMultiMountCard(int mount_id, int bless);

	bool ActiveSpecialImage(int imageid);                                   //双人坐骑特殊形象
	bool UseImage(int imageid);                                             //使用双人坐骑特殊形象
	bool IsSpecialImageActive(int imageid)  { return (0 != (m_couple_mount_param.special_img_active_flag & (1 << imageid))); }
	void UpgradeEquip(int equip_type);
	void MultiMountSpecialImgUpLevel(int special_img_id);
	short GetCurMountId() { return m_couple_mount_param.cur_select_mount_id; }
	//void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);

protected:
	void SelectMount(int mount_id);
	bool Upgrade(int mount_id, bool is_auto_buy);
	void RideMount();
	void UnRideMount();
	void InviteRide(int uid);
	void InviteRideAck(int uid, int agree_ride);

	void SendMountChangeNotify(short notify_type, int param_1 = 0, int param_2 = 0, int param_3 = 0);

private:
	Role *m_role;
	MultiMountParam m_param;
	CoupleMountParam m_couple_mount_param;

	CharIntAttrs m_attrs_add;												// 各属性加成数值

	static std::map<int, MultiMountBindItem> m_uid_to_binditem_map;			// 拥有者id对应共享坐骑对象
	static std::map<int, int> m_follower_uid_to_owner_uid_map;				// 搭乘者id对应拥有者id
};

#endif

