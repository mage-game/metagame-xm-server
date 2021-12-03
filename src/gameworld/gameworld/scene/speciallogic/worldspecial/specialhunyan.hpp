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

	HUNYAN_STATE_TYPE_STOP,							// �����ѽ���
	HUNYAN_STATE_TYPE_STANDBY,						// ����׼����
	HUNYAN_STATE_TYPE_START,						// ���翪ʼ��
	HUNYAN_STATE_TYPE_XUNYOU,						// ����Ѳ��
	
	HUNYAN_STATE_TYPE_MAX,
};

// ����˳����׼��-Ѳ��-��ʼ-������Ϊ����ͻ���ö��˳�����˵���

enum HUANYAN_BAITANG_STATUS
{
	HUANYAN_BAITANG_STATUS_INVALID = 0,
	HUANYAN_BAITANG_STATUS_START,
	HUANYAN_BAITANG_STATUS_END,
};

enum HUNYAN_OPERA_TYPE
{
	HUNYAN_OPERA_TYPE_INVALID = 0,	

	HUNYAN_OPERA_TYPE_JOIN_HUNYAN,					// �μӽ������ param1 fb_key
	HUNYAN_OPERA_TYPE_INVITE,						// ��������
	HUNYAN_OPERA_TYPE_YANHUA,						// ����ȼ���̻�
	HUNYAN_OPERA_TYPE_RED_BAG,						// �����ͺ�� param1 Ŀ��uid param2 seq
	HUNYAN_OPERA_TYPE_FOLWER,						// �����ͻ� param1 Ŀ��uid param2 seq
	HUNYAN_OPERA_TYPE_USE_YANHUA,					// �������̻� param1 seq param2 �Ƿ���
	HUNYAN_OPERA_TYPE_BAITANG_REQ,					// �������
	HUNUAN_OPERA_TYPE_BAITANG_RET,					// �յ����� param1 1:ͬ�� 0:�ܾ�
	HUNYAN_OPERA_TYPE_APPLY,						// ����μӻ���
	HUNYAN_OPERA_APPLICANT_OPERA,					// ��������μӻ����߲��� param1 Ŀ��uid param2 1:ͬ�� 0:�ܾ�
	HUNYAN_GET_BLESS_RECORD_INFO,					// ��ȡף����ʷ
	HUNYAN_GET_APPLICANT_INFO,						// ��ȡ��������Ϣ
	HUNYAN_GET_WEDDING_INFO,						// ��ȡ������Ϣ
	HUNYAN_GET_WEDDING_ROLE_INFO,					// ��ȡ���������Ϣ

	// ��������
	HUNYAN_OPERA_TYPE_HONGBAO,						// ���緢��� 
	HUNYAN_OPERA_TYPE_HUAYU,						// ��������
	HUNYAN_OPERA_TYPE_PAOHUAQIU,					// �����׻���
	HUNYAN_OPERA_TYPE_SAXIANHUA,					// �������ʻ�
	HUNYAN_OPERA_TYPE_INVITE_INFO,					// ���п����Ļ�����Ϣ

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

	ObjID last_notice_gather_obj_id;									// �ϴ������˵Ĳɼ����objid
	unsigned int next_notice_gather_timestamp;							// �´����Ѳɼ����Ƿ��ܱ��ɼ���ʱ���
};

struct XunyouInfo
{
	XunyouInfo() { this->Reset(); }

	void Reset()
	{
		marryobjpos_x = 0;
		marryobjpos_y = 0;
	}

	short marryobjpos_x;										// ����������X
	short marryobjpos_y;										// ����������Y
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

	bool IsCanEnterHunYanSence(Role *role);											// �ܷ�μӻ���

	void OnHunyanStandBy();
	void OnHunyanXunyou();															// Ѳ�ο�ʼ
	void OnHunyanStart();
	void OnHunyanStop();

	void ZhuFuHelper(Role *role, Protocol::CSMarryHunyanBless *mhb);				// ף��(��ʱ����)
	void OnHunyanOpera(Role *role, int opera_type, int param_1 ,int param_2);		// ���糡���ڲ���
	
	int GetHunyanState() { return m_state; }
	bool HunyanIsStop() { return m_state <= HUNYAN_STATE_TYPE_STOP; }
	void SendHunyanInfo(Role *role, int notify_reason, Role *leave_role = NULL);

	void GMForceStopHunyan() { this->OnHunyanStop(); }								// GM��������

	bool IsMarryUser(int uid, int *user_index);										// �Ƿ��ǻ�����Ů��

	int GetYanHuiType() { return m_hunyan_type; }
	void GetMenName(GameName name) { F_STRNCPY(name, m_mem_name, sizeof(GameName)); }
	void GetWomenNmae(GameName name) { F_STRNCPY(name, m_women_name, sizeof(GameName)); }

	int GetGatherByUid(int uid);

	bool UseYanHua(Role *role, int item_id, int param1, int param2);				// ʹ���̻�(��������)

	bool IsInApplicantList(int target_uid);											// �Ƿ��ڱ����б���
	bool AddApplicantToGuestList(Role *role, int target_uid, int is_agree);			// �����������б�

	void BrocastLiveness(int liveness_val);											// �ȶȴ�ɴ���

	// Ѳ��--------------------------------------------------------------
	void CreateXunyouObj();															// ��������ض���
	void CheckUpdateXunyou(unsigned long inteval, time_t now);						// ���Ѳ���ƶ�
	void OnCharacterMoveStop(Character *cha);										// Ѳ�ζ����ƶ�ֹͣ

	void BrocastXunyouBegin();														// Ѳ�ο�ʼ����
	void CheckReportXunyou();														// ��ʱѲ�δ���
	void BrocastXunyouEnd();														// Ѳ�ν�������

	void BrocastSpecialParamChange(Role *role, int change_type, int param);			// ����״̬�㲥

	void OnDeleteXunyouObj();														// ɾ��Ѳ�ζ���
	Scene *GetXunyouScene();														// ��ȡѲ�γ���

	const Posi GetXunyouObjCurPos(int seq, ObjID &obj_id);							// ��ȡѲ�ζ���ǰ����
	int GetCurXunyouSeq();															// ��ȡ��ǰѲ�ε����

protected:
	int GetCurWeddingSeq();															// ��ȡ��ǰ�Ļ������

	//��������
	void GiveRedBag(Role *role, int target_uid, int seq);							// �����
	void GiveFolwer(Role *role, int target_uid, int seq);							// �ͻ�
	bool OnUseYanHua(Role *role, int seq, bool is_auto_buy);						// ʹ���̻����������ģ�
	void OnBaiTangReq(Role *role);
	void OnBaiTangRet(Role *role, int is_accept);
	void SendWeddingLivenessToAll();
	void SendBlessRecordInfo(Role *role);
	void HunyanStopResetData();
	void ApplyJoinWedding(Role *role);
	void SendWeddingApplicantInfo(Role *role);
	void SendCurWeddingAllInfo(Role *role);
	void SendWeddingRoleInfo(Role *role);
	
	void PackHunYanInfo(Protocol::SCHunyanStateInfo *hi, Role *role = NULL);				// ��ȡ������Ϣ

	// ��������
	bool GiveHongbaoHelper(Role *role);
	bool HuaYuHelper(Role *role);
	bool SaXianHuaHelper(Role *role);	
	bool PaoHuaQiuHelper(Role *role);
	void SendRoleYanhuaInfo(Role *role);
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	int m_state;							// ��ǰ����״̬
	int m_hunyan_type;						// ��������
	GameName m_mem_name;					// ����������
	GameName m_women_name;					// ����Ů����

	// ʱ���
	unsigned int m_next_state_time;			// ��һ��״̬ʱ���
	unsigned int m_next_add_exp_time;		// ��һ���Զ��Ӿ���ʱ���
	unsigned int m_baitang_end_time;		// ���ý���ʱ���

	// �ȶ�
	int m_wedding_liveness;					// ��ǰ�ȶ�ֵ
	int m_wedding_liveness_reward_flag;		// �ȶȺ��ˢ�±��
	int m_cur_wedding_seq;					// ��ǰ����ʱ������
	int m_red_bag_num;						// ��ǰ�������
	int m_cur_red_bag_turn;					// ��ǰΪ�ڼ��ֺ��

	bool m_is_in_red_bag_flush_time;
	bool m_wedding_liveness_is_change;

	char m_bai_tang_status;					// ����״̬

	char m_is_first_role_enter;

	//�׻���
// 	unsigned int m_paohuoqiu_timestmp;
// 	int m_paohuoqiu_times;

	Protocol::HunyanMarryUserInfo m_marryuser_list[Protocol::HUNYAN_MARRY_USER_COUNT];			// ������Ů����Ϣ

	std::map<int, int> m_gathar_num_map;														// uid - gather_num �ɼ�������
	std::map<int, RoleGatherRedBagNum> m_gathar_red_bag_num_map;								// �ȶȲɼ������
	std::map<int, long long> m_wedding_role_exp_map;											// uid - exp �ۼƻ�þ����¼
	std::map<int, RoleNoticeErrorNumInfo> m_role_notice_errornum_info_map;						// uid - �ɼ�������Ϣ 
	std::set<int> m_invited_role_set;															// �����������б�
	std::set<int> m_enter_role_set;																// ���������uid
	std::map<int, std::set<ObjID> > m_role_gather_record;										// uid - ��ϯid_list ֻ��������������ֻˢһ�ξ�ϯ

	WeddingBlessingRecordInfo m_bless_record_info;												// ף����¼
	WeddingApplicantInfo m_applicant_list[WEDDING_APPLICANT_MAX_NUM];							// ���������߼�¼

	// Ѳ��--------------------------------------------------------------------------
	XunyouInfo m_xunyou_info;																	// Ѳ����Ϣ

	int m_cur_xunyou_point_index;																// ��ǰѲ�εĵ�
	unsigned long m_xunyou_obj_stop_time;														// Ѳ�ζ���ǰͣ��ʱ�䣨���룩
	time_t m_next_report_xunyou_time;															// ��һ�δ���Ѳ��ʱ��
	time_t m_next_sync_xunyou_pos_time;															// ��һ��ͬ��Ѳ������ʱ��
	ObjID m_xunyou_obj_id_list[HUNYAN_XUNYOU_MARRYOBJ_MAX_NUM];									// Ѳ�ζ���obj�б�  

//	std::map<int, int> m_yanhua_num_map;
//	std::map<int, int> m_paohuaqiu_num_map;	
//	std::map<int, int> m_zhufu_num_map;
//	std::map<int, int> m_saxianhua_num_map;
};

#endif // __SPECIAL_HUNYAN_HPP__




