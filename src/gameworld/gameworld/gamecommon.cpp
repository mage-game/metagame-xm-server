#include <stdlib.h>
#include "gamecommon.h"
#include "gstr.h"
#include "globalconfig/globalconfig.h"
#include "servercommon/servercommon.h"
#include "servercommon/msgcommonstruct.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "item/itempool.h"
#include "scene/scene.h"
#include "scene/map.h"

#include <algorithm>

namespace gamecommon
{

	Posf g_move_dir_vec[Dir_Count_Reserved];

	void GlobalInit()
	{
		// g_move_dir_vec[Dir_S] =	 Posf( 0,	 0  );
		g_move_dir_vec[Dir_L] =  Posf(-1.0,	 0  );
		g_move_dir_vec[Dir_R] =  Posf( 1.0,	 0  );
		g_move_dir_vec[Dir_U] =  Posf( 0,	 1.0);
		g_move_dir_vec[Dir_D] =  Posf( 0,   -1.0);
		g_move_dir_vec[Dir_LU] = Posf(-1.0,	 1.0);
		g_move_dir_vec[Dir_LD] = Posf(-1.0, -1.0);
		g_move_dir_vec[Dir_RU] = Posf( 1.0,	 1.0);
		g_move_dir_vec[Dir_RD] = Posf( 1.0, -1.0);
		g_move_dir_vec[Dir_RESERVED] = Posf( 0, 0);

		for (int i = 1; i < Dir_Count_Reserved; ++i)
		{
			g_move_dir_vec[i].Normalize();
		}
	}

	GameStat g_game_stat;

	// 是否是在边界(合法区域，且周围八个方位是非法区域)
	bool IsBorder(const Posi &pos, Scene *scene)
	{
		if (NULL == scene)
		{
			return false;
		}

		if (!scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos.x, pos.y))
		{
			return false;
		}

		// 检测八个方位
		if (scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos.x, pos.y + 1)
			&& scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos.x, pos.y - 1)
			&& scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos.x - 1, pos.y)
			&& scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos.x + 1, pos.y)
			&& scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos.x - 1, pos.y - 1)
			&& scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos.x - 1, pos.y + 1)
			&& scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos.x + 1, pos.y + 1)
			&& scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos.x + 1, pos.y - 1))
		{
			return false;
		}

		return true;
	}

	Dir GetMoveDir(const Posi &deltaPos, Scalar *distance)
	{
		if (deltaPos.x == 0 && deltaPos.y == 0)
		{
			if (NULL != distance) *distance = 0;
			return Dir_S;
		}

		if (NULL != distance) 
		{
			*distance = static_cast<Scalar>(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y);
			*distance = sqrt(*distance);
		}

		return atan2((float)deltaPos.y, (float)deltaPos.x);

		//Posi absP((deltaPos.x >= 0 ? deltaPos.x : -deltaPos.x), (deltaPos.y >= 0 ? deltaPos.y : -deltaPos.y));
		//Axis s, l;
		//bool x_s = true;
		//if (absP.x < absP.y)
		//{
		//	s = absP.x;
		//	l = absP.y;
		//}
		//else
		//{
		//	s = absP.y;
		//	l = absP.x;
		//	x_s = false;
		//}
		//const float sqrt2 = 1.41421356f;

		//Dir dir;
		//Axis deltaSL = l - s;
		//float slashD = sqrt2 * s;
		//if (deltaSL > slashD)
		//{
		//	*distance = (Scalar)deltaSL;
		//	dir = (x_s ? (deltaPos.y > 0 ? Dir_U : Dir_D) : (deltaPos.x > 0 ? Dir_R : Dir_L));
		//}
		//else
		//{
		//	*distance = (Scalar)slashD;
		//	dir = ((deltaPos.x > 0) ? (deltaPos.y > 0 ? Dir_RU : Dir_RD) : (deltaPos.y > 0 ? Dir_LU : Dir_LD));
		//}
		// return dir;
	}

	//const Posi GetDisRandPos(const Posi &curr_pos, int rand_dis)
	//{
	//	if (rand_dis <= 0) return curr_pos;

	//	int x = curr_pos.x + RandomNum(-rand_dis, rand_dis);
	//	int y = curr_pos.y + RandomNum(-rand_dis, rand_dis);

	//	return Posi(x, y);
	//}

	const Posi GetValidRandPos(const Posi &curr_pos, int rand_dis, Scene *scene)
	{
		if (NULL == scene || rand_dis <= 0) return curr_pos;

		Posi pos = curr_pos;

		int protect_count = 0;
		while (protect_count++ < 100)
		{
			Posi tmp_pos = gamecommon::GetDisRandPos(curr_pos, rand_dis);
			if (scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, tmp_pos.x, tmp_pos.y) && !gamecommon::IsBorder(tmp_pos, scene))
			{
				pos = tmp_pos;
				break;
			}
		}

		return pos;
	}

	void GetValidRandPosList(const Posi &center_pos, int range, Scene *scene, int min_dis, std::queue<Posi> &pos_list)
	{
		if (NULL == scene || range <= 0 || min_dis <= 0) return;

		Posi lu_pos = Posi(center_pos.x - range, center_pos.y + range);
		Posi rd_pos = Posi(center_pos.x + range, center_pos.y - range);

		std::vector<Posi> pos_vec;
		for (int x = lu_pos.x; x <= rd_pos.x; x += min_dis)
		{
			for (int y = rd_pos.y; y <= lu_pos.y; y += min_dis)
			{
				Posi tmp_pos(x, y);
				if (scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, tmp_pos.x, tmp_pos.y) && !gamecommon::IsBorder(tmp_pos, scene))
				{
					pos_vec.push_back(tmp_pos);
					continue;
				}
			}
		}
		std::random_shuffle(pos_vec.begin(), pos_vec.end());

		for (std::vector<Posi>::iterator it = pos_vec.begin(); it != pos_vec.end(); ++it)
		{
			pos_list.push(*it);
		}
	}

	void SendMultiMailToRole(const UserID &user_id, const ItemDataWrapper item_list[], int item_count, bool is_gamestring, const char *str_content, ...)
	{
		if (user_id == INVALID_USER_ID || (item_count <= 0) || strlen(str_content) <= 0) return;

		// 获取字符串
		const char *fmt = NULL;
		if (is_gamestring)
		{
			fmt = str_content;
		}
		else
		{
			fmt = gstr::GetStr(ContainerType_MailContent, str_content).c_str();
		}

		MailContentTxt content_str;
		{
			// 格式化字符串
			const int content_len = sizeof(content_str);

			va_list va;
			va_start(va, str_content);
			int str_len = vsnprintf(content_str, content_len, fmt, va);
			if (str_len >= content_len) str_len = -1;
			va_end(va);
			content_str[content_len - 1] = '\0';

			if (str_len <= 0)
			{
				return;
			}
		}
		static MailContentParam contentparam;

		// 计算需要发送邮件个数
		int mail_count = (item_count % MAX_ATTACHMENT_ITEM_NUM > 0) ? (item_count / MAX_ATTACHMENT_ITEM_NUM + 1) : item_count / MAX_ATTACHMENT_ITEM_NUM;
		int had_send_reward_count = 0;
		for (int k = 0; k < mail_count; k++)
		{
			contentparam.Reset();

			for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
			{
				const ItemBase *item_base = ITEMPOOL->GetItem(item_list[had_send_reward_count].item_id);
				if (NULL == item_base)
				{
					return;
				}

				contentparam.item_list[i].item_id = item_list[had_send_reward_count].item_id;
				contentparam.item_list[i].num = item_list[had_send_reward_count].num;
				contentparam.item_list[i].is_bind = item_list[had_send_reward_count].is_bind;
				contentparam.item_list[i].invalid_time = item_list[had_send_reward_count].invalid_time;
				contentparam.item_list[i].has_param = item_list[had_send_reward_count].has_param;
				contentparam.item_list[i].param_data = item_list[had_send_reward_count].param_data;

				had_send_reward_count++;
				if (had_send_reward_count >= item_count)
				{
					break;
				}
			}

			SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), content_str);
			MailRoute::MailToUser(user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}

	void SendMultiMailToRole(const UserID &user_id, const ItemConfigData item_list[], int item_count, int virtual_item_value[MAIL_VIRTUAL_ITEM_MAX], bool is_gamestring, const char *str_content, ...)
	{
		bool has_virault_item = false;
		if (NULL != virtual_item_value)
		{
			for (int v_item_type = 0; v_item_type < MAIL_VIRTUAL_ITEM_MAX; ++v_item_type)
			{
				if (virtual_item_value[v_item_type] > 0)
				{
					has_virault_item = true;
					break;
				}
			}
		}

		if (user_id == INVALID_USER_ID || (item_count <= 0 && !has_virault_item) || strlen(str_content) <= 0) return;

		// 获取字符串
		const char *fmt = NULL;
		if (is_gamestring)
		{
			fmt = str_content;
		}
		else
		{
			fmt = gstr::GetStr(ContainerType_MailContent, str_content).c_str();
		}

		MailContentTxt content_str;
		{
			// 格式化字符串
			const int content_len = sizeof(content_str);

			va_list va;
			va_start(va, str_content);
			int str_len = vsnprintf(content_str, content_len, fmt, va);
			if (str_len >= content_len) str_len = -1;
			va_end(va);
			content_str[content_len - 1] = '\0';

			if (str_len <= 0)
			{
				return;
			}
		}

		static MailContentParam contentparam;
		if (item_count <= 0) // 没有物品，则只发虚拟物品
		{
			if (has_virault_item)
			{
				contentparam.Reset();
				int v_item_count = 0;
				for (int v_item_type = 0; v_item_type < MAIL_VIRTUAL_ITEM_MAX; ++v_item_type)
				{
					if (virtual_item_value[v_item_type] > 0)
					{
						contentparam.AddVirtualItem(v_item_count, v_item_type, virtual_item_value[v_item_type]);
						++v_item_count;
					}
				}
				SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), content_str);
				MailRoute::MailToUser(user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
		else
		{
			// 计算需要发送邮件个数
			int mail_count = (item_count % MAX_ATTACHMENT_ITEM_NUM > 0) ? (item_count / MAX_ATTACHMENT_ITEM_NUM + 1) : item_count / MAX_ATTACHMENT_ITEM_NUM;
			int had_send_reward_count = 0;
			for (int k = 0; k < mail_count; k++)
			{
				contentparam.Reset();
				if (has_virault_item && 0 == k) // 有虚拟物品，则放进去，并且只在第一封邮件里附带
				{
					int v_item_count = 0;
					for (int v_item_type = 0; v_item_type < MAIL_VIRTUAL_ITEM_MAX; ++v_item_type)
					{
						if (virtual_item_value[v_item_type] > 0)
						{
							contentparam.AddVirtualItem(v_item_count, v_item_type, virtual_item_value[v_item_type]);
							++v_item_count;
						}
					}
				}

				for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
				{
					const ItemBase *item_base = ITEMPOOL->GetItem(item_list[had_send_reward_count].item_id);
					if (NULL == item_base)
					{
						return;
					}
					contentparam.item_list[i].item_id = item_list[had_send_reward_count].item_id;
					contentparam.item_list[i].num = item_list[had_send_reward_count].num;
					contentparam.item_list[i].is_bind = item_list[had_send_reward_count].is_bind;
					contentparam.item_list[i].invalid_time = item_base->CalInvalidTime();

					had_send_reward_count++;
					if (had_send_reward_count >= item_count)
					{
						break;
					}
				}

				SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), content_str);
				MailRoute::MailToUser(user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}

	void SendMultiMailToCrossRole(const UniqueUserID &unique_user_id, const ItemConfigData item_list[], int item_count, int virtual_item_value[MAIL_VIRTUAL_ITEM_MAX], bool is_gamestring, const char *str_content, ...)
	{
		bool has_virault_item = false;
		if (NULL != virtual_item_value)
		{
			for (int v_item_type = 0; v_item_type < MAIL_VIRTUAL_ITEM_MAX; ++v_item_type)
			{
				if (virtual_item_value[v_item_type] > 0)
				{
					has_virault_item = true;
					break;
				}
			}
		}

		if (unique_user_id == INVALID_UNIQUE_USER_ID || (item_count <= 0 && !has_virault_item) || strlen(str_content) <= 0) return;

		// 获取字符串
		const char *fmt = NULL;
		if (is_gamestring)
		{
			fmt = str_content;
		}
		else
		{
			fmt = gstr::GetStr(ContainerType_MailContent, str_content).c_str();
		}

		MailContentTxt content_str;
		{
			// 格式化字符串
			const int content_len = sizeof(content_str);

			va_list va;
			va_start(va, str_content);
			int str_len = vsnprintf(content_str, content_len, fmt, va);
			if (str_len >= content_len) str_len = -1;
			va_end(va);
			content_str[content_len - 1] = '\0';

			if (str_len <= 0)
			{
				return;
			}
		}

		static MailContentParam contentparam;
		if (item_count <= 0) // 没有物品，则只发虚拟物品
		{
			if (has_virault_item)
			{
				contentparam.Reset();
				int v_item_count = 0;
				for (int v_item_type = 0; v_item_type < MAIL_VIRTUAL_ITEM_MAX; ++v_item_type)
				{
					if (virtual_item_value[v_item_type] > 0)
					{
						contentparam.AddVirtualItem(v_item_count, v_item_type, virtual_item_value[v_item_type]);
						++v_item_count;
					}
				}
				SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), content_str);
				MailRoute::MailToCrossUser(unique_user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
		else
		{
			// 计算需要发送邮件个数
			int mail_count = (item_count % MAX_ATTACHMENT_ITEM_NUM > 0) ? (item_count / MAX_ATTACHMENT_ITEM_NUM + 1) : item_count / MAX_ATTACHMENT_ITEM_NUM;
			int had_send_reward_count = 0;
			for (int k = 0; k < mail_count; k++)
			{
				contentparam.Reset();
				if (has_virault_item && 0 == k) // 有虚拟物品，则放进去，并且只在第一封邮件里附带
				{
					int v_item_count = 0;
					for (int v_item_type = 0; v_item_type < MAIL_VIRTUAL_ITEM_MAX; ++v_item_type)
					{
						if (virtual_item_value[v_item_type] > 0)
						{
							contentparam.AddVirtualItem(v_item_count, v_item_type, virtual_item_value[v_item_type]);
							++v_item_count;
						}
					}
				}

				for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
				{
					const ItemBase *item_base = ITEMPOOL->GetItem(item_list[had_send_reward_count].item_id);
					if (NULL == item_base)
					{
						return;
					}
					contentparam.item_list[i].item_id = item_list[had_send_reward_count].item_id;
					contentparam.item_list[i].num = item_list[had_send_reward_count].num;
					contentparam.item_list[i].is_bind = item_list[had_send_reward_count].is_bind;
					contentparam.item_list[i].invalid_time = item_base->CalInvalidTime();

					had_send_reward_count++;
					if (had_send_reward_count >= item_count)
					{
						break;
					}
				}

				SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), content_str);
				MailRoute::MailToCrossUser(unique_user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}

	void SendDrawResult(Role *role, const std::vector<ItemConfigData> &item_list, int draw_reason, int draw_count)
	{
		static Protocol::SCDrawResult msg;
		msg.reward_item_list.Copy1(item_list);
		msg.draw_reason = draw_reason;
		msg.draw_count = draw_count;

		SEND_TO_ROLE(role, msg);
	}
}

