
#ifndef GAMECOMMON_H
#define GAMECOMMON_H

#include "obj/character/role.h"
#include "gamedef.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "scene/scene.h"

#include <queue>

// ��Э����ͻ���
#define SEND_TO_ROLE(role, msg)\
do\
{\
	EngineAdapter::Instance().NetSend((role)->GetNetId(), (const char*)&(msg), sizeof(msg));\
} while(0)

#define SEND_TO_ALLGATEWAY(msg)\
do\
{\
	World::GetInstWorld()->SendToAllGateway((const char *)&(msg), sizeof(msg));\
} while(0)


namespace gamecommon
{

	extern Posf g_move_dir_vec[Dir_Count_Reserved];
	void GlobalInit();

	struct GameStat 
	{
		GameStat():user_packet_count(0), sync_pos_count(0), sync_pos_packege_count(0),
			enter_count(0), leave_count(0){}

		unsigned int user_packet_count;
		unsigned int sync_pos_count;
		unsigned int sync_pos_packege_count;
		unsigned int enter_count;
		unsigned int leave_count;

		void Clear()
		{
			user_packet_count = 0;
			sync_pos_count = 0;
		}
	};
	extern GameStat g_game_stat;

	// ͨ��λ��ƫ��������ȡ��һ���˶��ķ���
	Dir GetMoveDir(const Posi &deltaPos, Scalar *distance);

	// ��ȡ��Χ�ڵ�һ�������Pos
	const Posi GetDisRandPos(const Posi &curr_pos, int rand_dis);

	// ��ȡһ������Ŀ��õ�Pos�����ʧ�ܣ�����ԭpos
	const Posi GetValidRandPos(const Posi &curr_pos, int rand_dis, Scene *scene);

	// ��ȡһ������Ŀ��õ�Pos�б�ÿ��Pos���min_dis
	void GetValidRandPosList(const Posi &center_pos, int range, Scene *scene, int min_dis, std::queue<Posi> &pos_list);

	// ���ʼ��������� MAX_ATTACHMENT_ITEM_NUM ʱ֧�ַ�����ʼ�
	void SendMultiMailToRole(const UserID &user_id, const ItemDataWrapper item_list[], int item_count, bool is_gamestring, const char *str_content, ...);
	void SendMultiMailToRole(const UserID &user_id, const ItemConfigData item_list[], int item_count, int virtual_item_value[MAIL_VIRTUAL_ITEM_MAX], bool is_gamestring, const char *str_content, ...);
	void SendMultiMailToCrossRole(const UniqueUserID &unique_user_id, const ItemConfigData item_list[], int item_count, int virtual_item_value[MAIL_VIRTUAL_ITEM_MAX], bool is_gamestring, const char *str_content, ...);

	// ����Ȩ�����һ�������T��vector�е�Ԫ�����ͣ��ұ�������Ϊweight�ĳ�Ա
	template <class T>
	inline const T* RandListItem(const std::vector<T> &list)
	{
		int total_weight = 0;
		for (typename std::vector<T>::const_iterator it = list.begin(); it != list.end(); ++it)
		{
			total_weight += (*it).weight;
		}

		if (total_weight <= 0)
		{
			return NULL;
		}

		int rand_val = RandomNum(total_weight);

		for (typename std::vector<T>::const_iterator it = list.begin(); it != list.end(); ++it)
		{
			const T &cfg_item = *it;
			if (rand_val < cfg_item.weight)
			{
				return &cfg_item;
			}
			rand_val -= cfg_item.weight;
		}

		return NULL;
	}

	// ��ͨ��Э��
	void SendDrawResult(Role *role, const std::vector<ItemConfigData> &item_list, int draw_reason, int draw_count);
}

// ���á��жϡ�������λ��bitȡֵ��Χ��[0, 64) ����籣֤��Խ��
#define SET_BIT(flag, bit) ((flag) |= (UInt64)1 << (bit))

#define IS_BIT_SET(flag, bit) (((flag) & ((UInt64)1 << (bit))) != 0)

#define CLEAR_BIT(flag, bit) ((flag) &= ~((UInt64)1 << (bit)))

//�ж�ĳλ�Ƿ���
//15.14....3.2.1.0 
#define ISSET0(x) ((x)&0x1)
#define ISSET1(x) ((x)&0x2)
#define ISSET2(x) ((x)&0x4)
#define ISSET3(x) ((x)&0x8)
#define ISSET4(x) ((x)&0x10)
#define ISSET5(x) ((x)&0x20)
#define ISSET6(x) ((x)&0x40)
#define ISSET7(x) ((x)&0x80)
#define ISSET8(x) ((x)&0x100)
#define ISSET9(x) ((x)&0x200)
#define ISSET10(x) ((x)&0x400)
#define ISSET11(x) ((x)&0x800)
#define ISSET12(x) ((x)&0x1000)
#define ISSET13(x) ((x)&0x2000)
#define ISSET14(x) ((x)&0x4000)
#define ISSET15(x) ((x)&0x8000)

// ����ĳλ
//15.14....3.2.1.0 
#define SET0(x) (x |= 0x1)
#define SET1(x) (x |= 0x2)
#define SET2(x) (x |= 0x4)
#define SET3(x) (x |= 0x8)
#define SET4(x) (x |= 0x10)
#define SET5(x) (x |= 0x20)
#define SET6(x) (x |= 0x40)
#define SET7(x) (x |= 0x80)
#define SET8(x) (x |= 0x100)
#define SET9(x) (x |= 0x200)
#define SET10(x) (x |= 0x400)
#define SET11(x) (x |= 0x800)
#define SET12(x) (x |= 0x1000)
#define SET13(x) (x |= 0x2000)
#define SET14(x) (x |= 0x4000)
#define SET15(x) (x |= 0x8000)

#endif
