#ifndef CHARAINITPARAM_H
#define CHARAINITPARAM_H

#include <string>

#include "common/tlvprotocol.h"

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"
#include "gameworld/gameworld/gamedef.h"

#include "servercommon/struct/itemlistparam.h"

// ����ṹ���Բ��������string���ֶ��� ��Ϊ����ṹ��memset��

class RoleInitParam 
{
public:
	static const int MYSTERIOUS_SHOP_INFO_NUM = 3;			// �����̵�洢

	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	// DB�е�Buff(������Ͱ汾����߰汾������) �ڴ������� (��߰汾��������) �������
 	bool Serialize(TLVSerializer &outstream, bool ver_check) const;
	bool Unserialize(TLVUnserializer &instream, bool ver_check);  

	int				role_id;								// ID
	GameName		role_name;								// ����
	char			avatar;									// ͷ��
	char			sex;									// �Ա�
	char			camp;									// ������Ӫ
	char			camp_post;								// ��Ӫְλ
	char			prof;									// ְҵ
	char			authority_type;							// Ȩ�� 
	int				level;									// �ȼ�

	int				scene_id;								// ����ID
	int				scene_x;								// X
	int				scene_y;								// Y
	int				last_scene_id;							// �ϴ���ͨ����ID
	int				last_scene_x;							// X
	int				last_scene_y;							// Y

	int				online_time;							// ����ʱ��
	int				today_online_time;						// ��������ʱ��
	int				lastday_online_time;					// ��������ʱ��
	long long		create_time;							// ����ʱ��
	
	Int64			coin;									// �ǰ�ͭ��
	Int64			coin_bind;								// ��ͭ��
	Int64			gold;									// Ԫ��
	Int64			gold_bind;								// ��Ԫ��

	int				knapsack_valid_num;						// �������ø�����
	int				storage_valid_num;						// �ֿ���ø�����

	long long		forbid_talk_time;						// ����ʱ��

	unsigned int	last_online_dayid;						// �ϴθ��µ�dayid

	Attribute		hp;										// Ѫ��
	Attribute		max_hp;									// ���Ѫ��
	Attribute		mp;										// ħ��
	Attribute		max_mp;									// ���ħ��
	Attribute		gongji;									// ����
	Attribute		fangyu;									// ����
	Attribute		mingzhong;								// ����
	Attribute		shanbi;									// ����
	Attribute		baoji;									// ����
	Attribute		jianren;								// ����
	Attribute		move_speed;								// �ƶ��ٶ�
	Attribute		fujia_shanghai;							// �����˺�
	Attribute		dikang_shanghai;						// �ֿ��˺�
	Attribute		per_jingzhun;							// ��׼
	Attribute		per_baoji;								// ������
	Attribute		per_kangbao;							// ������
	Attribute		per_pofang;								// �Ʒ���
	Attribute		per_mianshang;							// ������
	Attribute		constant_zengshang;						// �̶�����
	Attribute		constant_mianshang;						// �̶�����
	Attribute		pvp_zengshang;							// pvp����
	Attribute		pvp_mianshang;							// pvp����
	Attribute		pve_zengshang;							// pve����
	Attribute		pve_mianshang;							// pve����
	Attribute		huixinyiji;								// ����һ��
	Attribute		huixinyiji_hurt_per;					// ����һ���˺���
	Attribute		per_baoji_hurt;							// �����˺���ֱ�
	Attribute		per_kangbao_hurt;						// �����˺��ֿ���ֱ�
	Attribute		zhufuyiji_per;							// ף��һ����
	Attribute		gedang_per;								// ����
	Attribute		gedang_dikang_per;						// �񵲵ֿ���
	Attribute		gedang_jianshang;						// �񵲼���
	Attribute		skill_zengshang;						// ��������
	Attribute		skill_jianshang;						// ���ܼ���
	Attribute		mingzhong_per;							// ������
	Attribute		shanbi_per;								// ������
	Attribute		skill_cap_per;							// ����ս����ֱ�
	
	GuildID			guild_id;								// ����ID
	GameName		guild_name;								// ��������
	char			guild_post;								// ����ְλ
	char			reserve_ch;								// ����1
	short			reserve_sh;								// ����2
	int				guild_gongxian;							// ���Ź���
	int				guild_total_gongxian;					// �����ܹ���

	long long		exp;									// ����
	long long		max_exp;								// �����

	char			attack_mode;							// ����ģʽ

	char			vip_level;								// VIP�ȼ�

	DayCountData	day_count_data;							// ÿ�ռ���

	int				capability;								// ս����
	int				max_capability;							// ��ʷ���ս����

	int				evil;									// ���ֵ
	unsigned int	last_kill_me_time;						// �ϴ�ɱ���ҵ�ʱ��
	int				yuanli;									// Ԫ��
	int				m_yuanli;								// Ԫ��
	int				nv_wa_shi;								// Ů�ʯ
	int				lingjing;								// �龧
	int				chengjiu;								// �ɾ�
	int				fenchen;								// �۳�

	char			move_mode;								// �˶�ģʽ
	char			move_mode_param;						// �˶�ģʽ����

	SublockString	sublock_pw;								// ����������
	char			sublock_has_lock;						// �������Ƿ�����
	char			sublock_today_error_times;				// �����������ѳ���ʧ�ܴ���

	int				all_give_flower_num;					// ���ͻ���
	int				day_give_flower_num;					// �����ͻ���
	int				all_charm;								// ������

	char			is_change_avatar;						// �Ƿ�ı��ͷ��
	Attribute		gm_change_maxhp;						// GM����ı����Ѫ
	Attribute		gm_change_gongji;						// GM����ı�Ĺ���
	Attribute		gm_change_movespeed;					// GM����ı��ƶ��ٶ�		
	Attribute		gm_change_fangyu;						// GM����ı����
	Attribute		gm_chnage_mingzhong;					// GM����ı�����
	Attribute		gm_change_capability_attr;				// GM����ı�ս������������
// 	Attribute		gm_chnage_baoji;						// GM����ı䱩��
// 	Attribute		gm_chnage_jianren;						// GM����ı����
// 
// 	Attribute		gm_change_fujia_shanghai;				// GM����ı�ĸ����˺�
// 	Attribute		gm_change_dikang_shanghai;				// GM����ı�ĵֿ��˺�
// 	Attribute		gm_change_per_jingzhun;					// GM����ı�ľ�׼
// 	Attribute		gm_change_per_baoji;					// GM����ı�ı���
// 	Attribute		gm_change_per_kangbao;					// GM����ı�Ŀ���
// 	Attribute		gm_change_per_pofang;					// GM����ı���Ʒ�
// 	Attribute		gm_change_per_mianshang;				// GM����ı������
// 	Attribute		gm_change_constant_zengshang;			// GM����̶�����
// 	Attribute		gm_change_constant_mianshang;			// GM����̶�����

	long long 		avatar_timestamp;						// ͷ�����ʱ���

	// ����ı���δ����
	int				honour;									// ��Ӫ����
	
	char			is_mount_bianshen;						// �Ƿ��������
	unsigned int	mount_bianshen_time;					// �������ʱ��

	int				skill_appearance;						// �������
	unsigned int	skill_appearance_end_time;				// ������۽���ʱ��
	int				item_appearance;						// ��Ʒ���
	int				item_appearance_param;					// ��Ʒ��۲���
	unsigned int	item_appearance_end_time;				// ��Ʒ��۽���ʱ��

	int				huashen_appearance;						// �������
	int				huashen_appearance_param;				// ������۲���
	unsigned int	huashen_appearance_end_time;			// ������۽���ʱ��

	int				greate_soldier_appearance;				// �������
	int				greate_soldier_appearance_param;		// ������۲���
	unsigned int	greate_soldier_appearance_end_time;		// ������۽���ʱ��

	int				team_index;								// ��������
	int				team_seq;								// �������к�
	char			has_send_all_info;						// �Ƿ�ִ����OnAllInfoReq
	char			is_auto_join_team;						// �Ƿ��Զ��������

	unsigned int	login_timestamp;						// ��¼ʱ���

	//��黺����Ϣ -------------------------------------------------------------------------------------------
	int m_marry_type;						// �������
	int m_lover_id;							// ����id
	int m_shici_type;						// �Ĵ�����
	int m_marry_req_id;						// ����������ߵ�id����������ж����ĵ�ʱ���ã�
	char m_is_sure;							// �Ƿ��а���ӡ
};

#endif

