#ifndef __SPECIAL_HUNYAN_HPP__
#define __SPECIAL_HUNYAN_HPP__

#include <set>
#include <map>
#include <deque>
#include "scene/speciallogic/speciallogic.hpp"
#include "servercommon/marrydef.hpp"
#include "servercommon/maildef.hpp"

#include "protocal/msgother.h"

enum HUNYAN_STATE_TYPE
{
	HUNYAN_STATE_TYPE_INVALID = -1,

	HUNYAN_STATE_TYPE_STOP,							// 婚宴已结束
	HUNYAN_STATE_TYPE_STANDBY,						// 婚宴准备中
	HUNYAN_STATE_TYPE_START,						// 婚宴开始中
	HUNYAN_STATE_TYPE_XUNYOU,						// 婚宴巡游
	
	HUNYAN_STATE_TYPE_MAX,
};

// 正常顺序是准备-巡游-开始-结束，为方便客户端枚举顺序做了调整

enum HUANYAN_BAITANG_STATUS
{
	HUANYAN_BAITANG_STATUS_INVALID = 0,
	HUANYAN_BAITANG_STATUS_START,
	HUANYAN_BAITANG_STATUS_END,
};

enum HUNYAN_OPERA_TYPE
{
	HUNYAN_OPERA_TYPE_INVALID = 0,	

	HUNYAN_OPERA_TYPE_JOIN_HUNYAN,					// 参加进入婚宴 param1 fb_key
	HUNYAN_OPERA_TYPE_INVITE,						// 婚宴邀请
	HUNYAN_OPERA_TYPE_YANHUA,						// 婚宴燃放烟花
	HUNYAN_OPERA_TYPE_RED_BAG,						// 婚宴送红包 param1 目标uid param2 seq
	HUNYAN_OPERA_TYPE_FOLWER,						// 婚宴送花 param1 目标uid param2 seq
	HUNYAN_OPERA_TYPE_USE_YANHUA,					// 婚宴用烟花 param1 seq param2 是否购买
	HUNYAN_OPERA_TYPE_BAITANG_REQ,					// 请求拜堂
	HUNUAN_OPERA_TYPE_BAITANG_RET,					// 收到拜堂 param1 1:同意 0:拒绝
	HUNYAN_OPERA_TYPE_APPLY,						// 申请参加婚礼
	HUNYAN_OPERA_APPLICANT_OPERA,					// 处理申请参加婚礼者操作 param1 目标uid param2 1:同意 0:拒绝
	HUNYAN_GET_BLESS_RECORD_INFO,					// 获取祝福历史
	HUNYAN_GET_APPLICANT_INFO,						// 获取申请者信息
	HUNYAN_GET_WEDDING_INFO,						// 获取婚宴信息
	HUNYAN_GET_WEDDING_ROLE_INFO,					// 获取婚宴个人信息

	// 以下屏蔽
	HUNYAN_OPERA_TYPE_HONGBAO,						// 婚宴发红包 
	HUNYAN_OPERA_TYPE_HUAYU,						// 浪漫花雨
	HUNYAN_OPERA_TYPE_PAOHUAQIU,					// 婚宴抛花球
	HUNYAN_OPERA_TYPE_SAXIANHUA,					// 婚宴撒鲜花
	HUNYAN_OPERA_TYPE_INVITE_INFO,					// 所有开启的婚宴信息

	HUNYAN_OPERA_TYPE_MAX,	
};

struct RoleGatherRedBagNum
{
	RoleGatherRedBagNum()
	{
		memset(gather_num_list, 0, sizeof(gather_num_list));
	}

	int gather_num_list[WEDDING_MAX_LIVENESS_TYPE];
};

struct RoleNoticeErrorNumInfo
{
	RoleNoticeErrorNumInfo() : last_notice_gather_obj_id(INVALID_OBJ_ID), next_notice_gather_timestamp(0)
	{
	}

	ObjID last_notice_gather_obj_id;									// 上次提醒了的采集物的objid
	unsigned int next_notice_gather_timestamp;							// 下次提醒采集物是否能被采集的时间戳
};

struct XunyouInfo
{
	XunyouInfo() { this->Reset(); }

	void Reset()
	{
		marryobjpos_x = 0;
		marryobjpos_y = 0;
	}

	short marryobjpos_x;										// 结婚对象坐标X
	short marryobjpos_y;										// 结婚对象坐标Y
};

const static int MAX_YANHUI_INVITE_NUMBER = 150;

class SpecialHunyan : public SpecialLogic
{
public:
	SpecialHunyan(Scene *scene);
	virtual ~SpecialHunyan();

	virtual bool CanDestroy() { return this->HunyanIsStop(); }

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRemoveObj(Obj *obj);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool SpecialCanMountOn(Role *role) { return false; }

	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual int GetSpecialParam(Role *role);

	bool IsCanEnterHunYanSence(Role *role);											// 能否参加婚宴

	void OnHunyanStandBy();
	void OnHunyanXunyou();															// 巡游开始
	void OnHunyanStart();
	void OnHunyanStop();

	void ZhuFuHelper(Role *role, Protocol::CSMarryHunyanBless *mhb);				// 祝福(暂时无用)
	void OnHunyanOpera(Role *role, int opera_type, int param_1 ,int param_2);		// 婚宴场景内操作
	
	int GetHunyanState() { return m_state; }
	bool HunyanIsStop() { return m_state <= HUNYAN_STATE_TYPE_STOP; }
	void SendHunyanInfo(Role *role, int notify_reason, Role *leave_role = NULL);

	void GMForceStopHunyan() { this->OnHunyanStop(); }								// GM结束婚宴

	bool IsMarryUser(int uid, int *user_index);										// 是否是婚宴男女主

	int GetYanHuiType() { return m_hunyan_type; }
	void GetMenName(GameName name) { F_STRNCPY(name, m_mem_name, sizeof(GameName)); }
	void GetWomenNmae(GameName name) { F_STRNCPY(name, m_women_name, sizeof(GameName)); }

	int GetGatherByUid(int uid);

	bool UseYanHua(Role *role, int item_id, int param1, int param2);				// 使用烟花(主动消耗)

	bool IsInApplicantList(int target_uid);											// 是否在宾客列表中
	bool AddApplicantToGuestList(Role *role, int target_uid, int is_agree);			// 处理加入宾客列表

	void BrocastLiveness(int liveness_val);											// 热度达成传闻

	// 巡游--------------------------------------------------------------
	void CreateXunyouObj();															// 创建拜天地对象
	void CheckUpdateXunyou(unsigned long inteval, time_t now);						// 检查巡游移动
	void OnCharacterMoveStop(Character *cha);										// 巡游对象移动停止

	void BrocastXunyouBegin();														// 巡游开始传闻
	void CheckReportXunyou();														// 定时巡游传闻
	void BrocastXunyouEnd();														// 巡游结束传闻

	void BrocastSpecialParamChange(Role *role, int change_type, int param);			// 特殊状态广播

	void OnDeleteXunyouObj();														// 删除巡游对象
	Scene *GetXunyouScene();														// 获取巡游场景

	const Posi GetXunyouObjCurPos(int seq, ObjID &obj_id);							// 获取巡游对象当前坐标
	int GetCurXunyouSeq();															// 获取当前巡游点序号

protected:
	int GetCurWeddingSeq();															// 获取当前的婚宴序号

	//操作请求
	void GiveRedBag(Role *role, int target_uid, int seq);							// 发红包
	void GiveFolwer(Role *role, int target_uid, int seq);							// 送花
	bool OnUseYanHua(Role *role, int seq, bool is_auto_buy);						// 使用烟花（被动消耗）
	void OnBaiTangReq(Role *role);
	void OnBaiTangRet(Role *role, int is_accept);
	void SendWeddingLivenessToAll();
	void SendBlessRecordInfo(Role *role);
	void HunyanStopResetData();
	void ApplyJoinWedding(Role *role);
	void SendWeddingApplicantInfo(Role *role);
	void SendCurWeddingAllInfo(Role *role);
	void SendWeddingRoleInfo(Role *role);
	
	void PackHunYanInfo(Protocol::SCHunyanStateInfo *hi, Role *role = NULL);				// 获取婚宴信息

	// 以下屏蔽
	bool GiveHongbaoHelper(Role *role);
	bool HuaYuHelper(Role *role);
	bool SaXianHuaHelper(Role *role);	
	bool PaoHuaQiuHelper(Role *role);
	void SendRoleYanhuaInfo(Role *role);
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	int m_state;							// 当前婚宴状态
	int m_hunyan_type;						// 婚宴类型
	GameName m_mem_name;					// 婚宴男主名
	GameName m_women_name;					// 婚宴女主名

	// 时间戳
	unsigned int m_next_state_time;			// 下一个状态时间戳
	unsigned int m_next_add_exp_time;		// 下一次自动加经验时间戳
	unsigned int m_baitang_end_time;		// 拜堂结束时间戳

	// 热度
	int m_wedding_liveness;					// 当前热度值
	int m_wedding_liveness_reward_flag;		// 热度红包刷新标记
	int m_cur_wedding_seq;					// 当前婚宴时间段序号
	int m_red_bag_num;						// 当前红包数量
	int m_cur_red_bag_turn;					// 当前为第几轮红包

	bool m_is_in_red_bag_flush_time;
	bool m_wedding_liveness_is_change;

	char m_bai_tang_status;					// 拜堂状态

	char m_is_first_role_enter;

	//抛花球
// 	unsigned int m_paohuoqiu_timestmp;
// 	int m_paohuoqiu_times;

	Protocol::HunyanMarryUserInfo m_marryuser_list[Protocol::HUNYAN_MARRY_USER_COUNT];			// 婚宴男女主信息

	std::map<int, int> m_gathar_num_map;														// uid - gather_num 采集数量表
	std::map<int, RoleGatherRedBagNum> m_gathar_red_bag_num_map;								// 热度采集红包表
	std::map<int, long long> m_wedding_role_exp_map;											// uid - exp 累计获得经验记录
	std::map<int, RoleNoticeErrorNumInfo> m_role_notice_errornum_info_map;						// uid - 采集提醒信息 
	std::set<int> m_invited_role_set;															// 邀请过的玩家列表
	std::set<int> m_enter_role_set;																// 场景内玩家uid
	std::map<int, std::set<ObjID> > m_role_gather_record;										// uid - 酒席id_list 只适用于整场婚宴只刷一次酒席

	WeddingBlessingRecordInfo m_bless_record_info;												// 祝福记录
	WeddingApplicantInfo m_applicant_list[WEDDING_APPLICANT_MAX_NUM];							// 婚宴申请者记录

	// 巡游--------------------------------------------------------------------------
	XunyouInfo m_xunyou_info;																	// 巡游信息

	int m_cur_xunyou_point_index;																// 当前巡游的点
	unsigned long m_xunyou_obj_stop_time;														// 巡游对象当前停留时间（毫秒）
	time_t m_next_report_xunyou_time;															// 下一次传闻巡游时间
	time_t m_next_sync_xunyou_pos_time;															// 下一次同步巡游坐标时间
	ObjID m_xunyou_obj_id_list[HUNYAN_XUNYOU_MARRYOBJ_MAX_NUM];									// 巡游对象obj列表  

//	std::map<int, int> m_yanhua_num_map;
//	std::map<int, int> m_paohuaqiu_num_map;	
//	std::map<int, int> m_zhufu_num_map;
//	std::map<int, int> m_saxianhua_num_map;
};

#endif // __SPECIAL_HUNYAN_HPP__




