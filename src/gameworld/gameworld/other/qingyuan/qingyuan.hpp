#ifndef __QING_YUAN_HPP__
#define __QING_YUAN_HPP__

#include "servercommon/qingyuandef.hpp"
#include "obj/character/attribute.hpp"
#include "protocal/msgother.h"

struct RoleCrossGlobalData;
class Role;
class ItemGridData;

enum QINGYUAN_SHICI_TYPE
{
	SHICI_INVALID = 0,

	SHICI_ONE,
	SHICI_TWO,
	SHICI_THREE,

	SHICI_MAX,
};

enum QINGYUAN_MARRY_TYPE
{
	MARRY_INVALID = -1,

	MARRY_COMMON,
	MARRY_MIDDLE,
	MARRY_TOP_GRADE,

	MARRY_MAX,
};

enum QINGYUAN_FB_JOIN_TIMES
{
	QINGYUAN_FB_JOIN_TIMES_ONE = 0,
	QINGYUAN_FB_JOIN_TIMES_TWO,
	QINGYUAN_FB_JOIN_TIMES_THREE,
};

class Qingyuan
{
public:
	Qingyuan();
	~Qingyuan();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const QingyuanParam &qy_param, int marry_type, int lover_id, int shici_type, int marry_req_id, char is_sure);
	void GetInitParam(QingyuanParam *qy_param);
	void GetRoleInitParam(int *marry_type, int *lover_id, int *shici_type, int *marry_req_id, char *is_sure);

	//表白数据
	void Init(const PersonProfessParam &profess_param);
	void GetInitParam(PersonProfessParam *profess_param);

	void Update(unsigned int second);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void OnRoleLogin(unsigned int last_save_time);
	void OnEnterScene(int scene_id);

	void OnMarryOperate(int ope_type, int param1, int param2);			// 结婚操作

	void MarryReq(int marry_type, int target_uid);
	void MarryRet(int marry_type, int req_uid, bool is_accept);

	void MarryNotice(Role * marry_target);
	void SendMarryZhuhe(int uid, int type);
	
	void DivorceReq(int is_forced_divorce);
	void DivorceRet(int req_uid, bool is_accept);
	void OnDivorce(unsigned int divorce_time);

	int GetQingyuanValue() { return m_param.qingyuan_value; }
	void AddQingyuanValue(int add_value = 1);
	void QueryMateQingyuanValue();

	bool ChoseShici(int shici_type);										// 选誓词
	void OnPressFinger();													// 按手印(此处是真正的消耗)
	void OnPressFingerRepeat(Role *req_role, Role *target_role);			// 按手印(用于重复求婚)
	void SyncHaloType(int halo_type);									    // 同步夫妻光环数据(暂时屏蔽)
	void SendMarryOperaRet(Role* role, int ret_type, int ret_val);			// 返回结婚操作结果
	void ResetAllData();

	void SetLoverId(int role_id) { m_lover_id = role_id; }
	void SetMarryReqId(int role_id) { m_marry_req_id = role_id; }
	void SetMarryType(int marry_type) { m_marry_type = marry_type; }		
	void SetShiciType(int shici_type) { m_shici_type = shici_type; }
	bool IsSureMarry() { return m_is_sure; }

	//----------------------------------- 副本相关逻辑 ----------------------------------
	void SetHunyanFBOpenStatus(int is_open);		//设置婚宴开启状态
	bool IsHunyanFBAlreadyOpen() { return (0 != m_param.is_hunyan_fb_already_open); }
	bool CanJoinHunyanFB();

	void BuyFBTimesReq();
	void OnBuyDoubleReward();			// 用于情缘副本外购买双倍奖励
	bool CanJoinFB(bool is_leader);
	void SendBaseFBInfo();

	void AddEnterTimes() { ++ m_param.join_fb_times; }

	bool IsSkipFBWaves() { return 0 != m_param.is_skip_fb_waves; }
	void SetSkipFBWaves(bool is_skip);

	void GMResetHunyan();

	//----------------------------------- 婚戒相关逻辑 ----------------------------------

	int PutOn(const ItemDataWrapper &item_wrapper);
	int TakeOff();
	bool HunjieEquipUpLevel(ItemID stuff_id, short repeat_times, bool is_auto_buy);
	void AddUpLevelExp(int add_val);
	void SendHunjieEquipInfo();

	//----------------------------------- 光环相关逻辑 ----------------------------------
	void CoupleHaloUse(int halo_type);										// 装备光环
	void UplevelCoupleHalo(int halo_type, bool is_auto_buy = false);		// 升级光环
	void OnBuyHalo(int halo_type);										    // 购买光环
	int GetCoupleHaloLevel(int halo_type);									// 获取光环等级
	int GetActiveCoupleHaloCount(int color);								// 获取激活个数

	void CheckTejiaHalo(unsigned int now_second);							// 检查特价光环
	void CheckTejiaHaloActivity(bool is_login, unsigned int last_save_time);	// 检查特价光环活动

	void SendCoupleHaloInfo();												// 发送光环信息

	int GetCoupleHaloType() { return m_param.equiped_couple_halo_type; }	// 获取已装备光环类型
	bool IsInCoupleHaloEffect() { return m_is_in_couple_halo_effect; }
	
	//----------------------------------- 祝福相关逻辑 ----------------------------------
	int GetBlessDays() { return m_param.bless_days; }
	void SetBlessDays(int days) { m_param.bless_days = days; }
	void FetchBlessReward();
	void AddBlessDays();													//购买祝福天数
	void SendBlessInfo();

	//----------------------------------- 杂项 ------------------------------------------
	//bool IsHunyan() { return 0 == m_param.is_first_diamond_yanhui; } // 以前是第一次进入钻石婚宴是免费，并且要同时拉伴侣进副本的，现在不需要了。
	bool IsFirstDiamondHunyan() { return false; }
	void SetHunyanFlag(int flag) { m_param.is_first_diamond_yanhui = (char)flag; }

	int GetMaxGetDiamonds() { return m_param.today_yanhui_get_diamonds; }
	int GetMaxGetExps() { return m_param.today_yanhui_get_exps; }
	int GetMaxGetHunjieStuffs() { return m_param.today_yanhui_get_hunjie_stuffs; }
	int GetMaxGetOtherStuffs() { return m_param.today_yanhui_get_other_stuffs; }
	int GetMaxGetGatherDayMaxStuffs() { return m_param.today_hunyan_gather_day_max_stuff_num; }				// 获取婚宴今天已采集次数
	int GetTodayTuodanTimes() { return m_param.today_tuodan_times; }
	int GetTodaySaxianhuaTimes() { return m_param.today_saxianhua_times; }
	int GetMaxBeZhufuMaxStuffs() { return m_param.today_hunyan_be_zhufu_max_stuff_num; }
	int GetMaxZhufuMaxStuffs() { return m_param.today_hunyan_zhufu_max_stuff_num; }

	void AddMaxGetDiamonds(int number) { m_param.today_yanhui_get_diamonds += number; }
	void AddMaxGetExps(int number) { m_param.today_yanhui_get_exps += number; }
	void AddMaxGetHunjieStuffs(int number) { m_param.today_yanhui_get_hunjie_stuffs += number; }
	void AddMaxGetOtherStuffs(int number) { m_param.today_yanhui_get_other_stuffs += number; }
	void AddMaxGetGatherDayMaxStuffs(int number) { m_param.today_hunyan_gather_day_max_stuff_num += number; } // 增加婚宴今天已采集次数
	void AddTodayTuodanTimes() { m_param.today_tuodan_times ++; }
	void AddTodaySaxianhuaTimes(){ m_param.today_saxianhua_times ++; }
	void AddMaxBeZhufuMaxStuffs(int number) { m_param.today_hunyan_be_zhufu_max_stuff_num += number; }
	void AddMaxZhufuMaxStuffs(int number) { m_param.today_hunyan_zhufu_max_stuff_num += number; }

	void SetOpenHunyanFB(int flag) { m_param.is_hunyan_fb_open = flag; }  // 暂无用
	int GetOpenHunyanFB() {  return m_param.is_hunyan_fb_open; }		  // 暂无用

	void SetYanHuiFBKey(int fb_key) { m_param.yanhui_fb_key = fb_key; }  // 婚宴副本key(暂时无用)
	
	int GetMarryCount() {  return m_param.marry_count; }
	void AddMarryCount() { m_param.marry_count++; }

	int GetHunjieStarLevel();

	void SendYanhuiFBKey();

	QingyuanParam GetQingyuanParam(){return m_param;}

	void SetMarrReqUid(int uid){m_param.marry_req_uid = uid;}
	
	void GiveMarryReward(int hunli_type);

	char GetTodayHunyanTimes () { return m_param.today_hunyan_open_times; }
	void AddTodayHunyanTimes() { m_param.today_hunyan_open_times ++; }
	void SetCanHunyanOpen(char flag = 1) { m_param.can_open_hunyan = flag; }
	
	//----------------------------------- 情缘圣地相关逻辑 ----------------------------------
	void SendQingYuanShengDiInfo();
	void ResetQingYuanShengDi(bool is_init = false);
	void OnQingYuanShengDiFecthTaskReward(int task_index);
	void OnQingYuanShengDiFecthOtherReward();
	void OnQingYuanShengDiKillMonster(int task_type, int monster_id);
	void OnQingYuanShengDiGether(int gather_id);
	void OnQingYuanShengDiSkipTask();

	//----------------------------------- 婚宴相关逻辑 ---------------------------------------
	void QingYuanOperaReq(int opera_type, int param1, int param2);	// 新婚宴操作请求
	void MakeAnAppointmantMarryTimeReq(int seq, int hunyan_type);	// 向伴侣确认预约时间及类型
	void AppointmantMarryTimeRet(int seq, int is_agree);			// 伴侣预约时间确认回应（回应后消耗）
	bool IsValidYuyueSeq(char seq);									// 预约是否有效
	void WeddingInviteGuest(int uid);								// 邀请宾客
	void WeddingRemoveGuest(int uid);								// 将宾客从邀请名单中移除
	void BuyWeddingInviteGuestNum();								// 购买邀请宾客上限

	void SendWeddingYuYueInfo();									// 婚宴预约信息				
	void SendWeddingInfo(int info_type, bool is_popup = false);		// 婚宴相关信息

	void SetHunyanType(int type) { m_param.hunyan_type = type; }	// 预约婚宴类型
	int GetHunyuanType() { return m_param.hunyan_type; }			// 获取婚宴类型
	int GetOtherHunyuanType();										// 获取他人婚宴类型

	char GetHunyanMarryType() { return m_param.marry_type; }
	void SetHunyanMarryType(char marry_type) { m_param.marry_type = marry_type; }					// 设置结婚类型(婚宴开启播放特效用)
	bool IsHadPropose(int marry_type);																// 是否已经求某个结婚类型

	void SetTodayMarryReqStauts(char is_succ) { m_param.is_today_marry_req_succ = is_succ; }        // 暂无用
	void SetCurMarryStauts(int cur_stauts) { m_param.cur_marry_stauts = cur_stauts; }				// 设置婚宴状态
	void SetIsHaveHunYanTimes(char is_have_times) { m_param.is_have_hunyan_times = is_have_times; } // 设置是否有婚宴次数
	void SetWeddingYuYueSeq(char seq) { m_param.wedding_yuyue_seq = seq; }							// 设置婚宴预约序号
	void SetWeddingIndex(int index) { m_param.wedding_index = index; }								// 设置全服结婚第几场

	int GetTodayHunyanJoinTimes() { return m_param.today_hunyan_join_times; }						// 今天参加婚宴次数
	void AddTodayHunyanJoinTimes() { m_param.today_hunyan_join_times++; }							// 增加今天参加婚宴次数

	//----------------------------------- 婚宴-巡游 ---------------------------------------
	bool IsInXunyouStatus();															// 能否请求巡游相关操作
	bool CheckCanGatherXunyouHongbao(int gather_id);									// 能否采集巡游红包
	void OnGatherXunyouHongbao(int gather_id);											// 采集巡游红包

	bool CheckCanSaHongbao();															// 检查撒红包条件
	void OnBuySaHongBaoTimes();															// 购买撒红包次数
	void OnXunyouSaHongbao();															// 撒红包
	bool CheckCanGiveFlower();															// 检查送花条件
	void OnBuyGiveFlowerTimes();														// 购买送花次数
	void OnXunyouGiveFlower(ItemID *item_id, int *flower_num);							// 送花

	void CheckTransToXunyouPos();														// 检查跳转至巡游中的坐标
	void CheckSyncAOIInXunyou(unsigned int now_second);									// 检查与巡游对象坐标同步

	void OnSendXunyouObjInfo(int seq);													// 发送巡游对象obj信息
	const Posi GetXunyouObjCurPos(int seq);												// 获取巡游对象obj坐标

	//-----------------------------------表白墙---------------------------------------
	void OnProfessWallReq(Protocol::CSProfessWallReq* msg);														// 表白墙通用请求
	void OnProfessTo(int other_role_id, int gift_type, bool is_auto_buy, ProfessContent& content);				// 表白
	void OnProfessFrom(int other_role_id, PersonProfessItem& profess_item);										// 被表白
	void OnDeleteProfessItem(unsigned short profess_wall_type, unsigned int profess_time, int role_id);			// 删除某条表白
	void SendProfessWallInfo(unsigned short profess_wall_type, short notify_type);													// 发送表白墙信息
	void DeleteProfessItemFunc(std::deque<PersonProfessItem>& profess_deque, unsigned int profess_time, int role_id);
	void SendProfessWallInfoFunc(std::deque<PersonProfessItem>& profess_deque, unsigned short profess_wall_type, short notify_type);

	void AddProfessExp(int exp);																				// 增加表白经验
	void SendProfessLevelInfo();																				// 发送表白等级信息

private:
	void CheckCoupleHaloEffect(unsigned int now_second);									// 检查触发光环特效
	bool IsCoupleHaloTrigger();																// 是否触发光环
	void BroadcastCoupleHaloTrigger(int halo_type);											// 广播光环触发
	bool CheckTeamInformationCorrect(int uid);												// 暂无用

	Role *m_role;

	QingyuanParam m_param;
	CharIntAttrs m_attrs_add;

	bool m_is_in_couple_halo_effect;
	unsigned int m_next_check_couple_halo_effect_time;

	int m_halo_level_list[QINGYUAN_COUPLE_HALO_MAX_TYPE];

	//-------------------------------------------------------------------------------------------
	int m_marry_type;						// 结婚类型
	int m_lover_id;							// 爱人id
	int m_shici_type;						// 誓词类型
	int m_marry_req_id;						// 结婚请求发起者的id（用来最后判断消耗的时候用）
	bool m_is_sure;							// 是否有按手印

	//-------------------------------------------------------------------------------------------
	int m_yuyue_hunyan_type;				// 预约的婚宴类型
	bool m_is_trans_to_xunyou_scene;		// 是否已传送到巡游场景
	UInt32 m_next_sync_aoi_timestamp;		// 下次与巡游对象同步时间戳

	//-------------------------------------------------------------------------------------------
	unsigned int m_last_profess_time;				// 上一次表白时间
	std::deque<PersonProfessItem> m_profess_to;		// 表白出去的
	std::deque<PersonProfessItem> m_profess_from;	// 收到的表白

};
#endif

