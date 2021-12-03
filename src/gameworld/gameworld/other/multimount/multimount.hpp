#ifndef __MULTI_MOUNT_H__
#define __MULTI_MOUNT_H__

class Role;

#include "obj/character/attribute.hpp"
#include "servercommon/multimountdef.hpp"
#include "servercommon/newmultimountdef.hpp"
#include <map>

static const int MULTI_MOUNT_INVITE_MAX_DISTANCE = 20;						// ������������������

enum MULTI_MOUNT_RIDE_STATUS
{
	MULTI_MOUNT_RIDE_STATUS_NONE = 0,										// ��״̬��δ���
	MULTI_MOUNT_RIDE_STATUS_OWNER_RIDE,										// ӵ���ߵ������
	MULTI_MOUNT_RIDE_STATUS_SEND_REQUEST,									// ��������δ�ظ�״̬
	MULTI_MOUNT_RIDE_STATUS_TOW_RIDE,										// ˫�����״̬
};

struct MultiMountBindItem													// ˫������󶨶���
{
	MultiMountBindItem() : owner_uid(0), follower_uid(0), invite_uid(0), owner_obj_id(INVALID_OBJ_ID), follower_obj_id(INVALID_OBJ_ID), mount_id(-1), res_id(0), mount_star_level(0), status(MULTI_MOUNT_RIDE_STATUS_NONE) {}

	int owner_uid;															// ӵ�������ID
	int follower_uid;														// ��������ID
	int invite_uid;															// ����Է����ID
	ObjID owner_obj_id;
	ObjID follower_obj_id;

	int mount_id;															// ����ID
	int res_id;																// ��ԴID
	int mount_star_level;														// ������������ڼ��������ٶȣ�
	int status;																// ���״̬
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

	bool ActiveSpecialImage(int imageid);                                   //˫��������������
	bool UseImage(int imageid);                                             //ʹ��˫��������������
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

	CharIntAttrs m_attrs_add;												// �����Լӳ���ֵ

	static std::map<int, MultiMountBindItem> m_uid_to_binditem_map;			// ӵ����id��Ӧ�����������
	static std::map<int, int> m_follower_uid_to_owner_uid_map;				// �����id��Ӧӵ����id
};

#endif

