#include "obj/character/role.h"

#include "internalcomm.h"
#include "engineadapter.h"

#include "item/itembase.h"
#include "item/itempool.h"

#include "servercommon/errornum.h"
#include "servercommon/servercommon.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/maildef.hpp"

#include "mailroute.hpp"
#include "gameworld/gamelog.h"
#include "global/mail/mailmanager.hpp"
#include "global/usercache/usercache.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "item/knapsack.h"
#include "friend/friendmanager.h"
#include "other/shop/roleshop.hpp"
#include "other/monitor/monitor.hpp"
#include "other/sublock/sublock.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/cross/rolecross.h"
#include "other/shenyin/shenyin.hpp"
#include "other/vip/vip.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "gameworld/config/logicconfigmanager.hpp"
#include "gameworld/config/agentadapterconfig.hpp"
#include "other/tianshenhuti/tianshenhuti.hpp"

MailRoute & MailRoute::Instance()
{
	static MailRoute mail_route;
	return mail_route;
}

// 发送邮件
void MailRoute::SendMail(Role *role, Protocol::CSMailSend *mailsend)
{
	if (NULL == role)
	{
		return;
	}

	// 好友才能发邮件了(防拉人, 防骂人）.
	if (!role->GetFriendManager()->IsFriend(IntToUid(mailsend->recver_uid)))
	{
		role->NoticeNum(errornum::EN_MAIL_FRIEND_LIMIT);
		return;
	}
	

	if (!role->IsInCanTradingTime())
	{
		role->NoticeNum(errornum::EN_MIAL_LOGIN_TIME_LIMIT);
		return;
	}

	if (mailsend->contenttxt_len >= static_cast<int>(sizeof(MailContentTxt)) || mailsend->contenttxt_len < 0) return;
	
	if (mailsend->item_count < 0) return;	
	if (mailsend->gold < 0) return;
	if (mailsend->coin < 0) return;
	
	if (UidToInt(role->GetUserId()) == mailsend->recver_uid) return;

	// 等级限制、vip等级限制
	{
		// 充值额度限制检查
//		if (!role->IsIgnoreChatOrMailLevelLimit() && role->GetLevel() < SHAREDCONFIG->GetOpenServerConfig().GetSendMailLevel()) return;

		if (!WorldStatus::Instance().CheckSatisfyLevel(role, OPENLEVEL_LIMIT_TYPE_SEND_MAIL))
		{
			return;
		}
	}

	// if ((mailsend->item_count > 0 || MAIL_POSTAGE > 0) && role->GetSublock()->CheckHasLock()) return;
	if ((mailsend->item_count > 0) && role->GetSublock()->CheckHasLock()) return;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	{
		if (!knapsack->GetMoney()->GoldMoreThan(mailsend->gold))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		if (mailsend->gold > 0)
		{
			role->NoticeNum(errornum::EN_FORBID_MAILGOLD);
			return;
		}
	}

	{
		if (!knapsack->GetMoney()->CoinMoreThan(mailsend->coin))
		{
			role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
			return;
		}
	}

	// 策划需求：去掉邮费
// 	int mail_postage = mailsend->item_count > 0 ? MAIL_POSTAGE_ATTACHMENT : MAIL_POSTAGE;
// 	if (!knapsack->GetMoney()->AllCoinMoreThan(mail_postage + mailsend->coin))
// 	{
// 		role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
// 		return;
// 	}

	MailSend gcms;
	gcms.recver_dbindex = IntToUid(mailsend->recver_uid).db_index;
	gcms.recver_roleid = IntToUid(mailsend->recver_uid).role_id;
	gcms.sender_dbindex = role->GetUserId().db_index;
	gcms.sender_roleid = role->GetUserId().role_id;
	role->GetName(gcms.sender_name);

	gcms.kind = MAIL_TYPE_PERSONAL;
	gcms.contentparam.coin = mailsend->coin;
	gcms.contentparam.coin_bind = 0;
	gcms.contentparam.gold = 0;
	gcms.contentparam.gold_bind = 0;	
	F_STRNCPY(gcms.contentparam.subject, mailsend->subject, sizeof(gcms.contentparam.subject));
	F_STRNCPY(gcms.contentparam.contenttxt, mailsend->contenttxt, sizeof(gcms.contentparam.contenttxt));

	gcms.contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GOLD, mailsend->gold);  // 元宝要以虚拟物品的方式发送下去

	ItemGridData igd;
	for (int i = 0; i < mailsend->item_count; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (mailsend->item_knapindex_list[i] == mailsend->item_knapindex_list[j]) return;
		}

		if (!knapsack->GetItemGridData(mailsend->item_knapindex_list[i], &igd))
		{
			role->NoticeNum(errornum::EN_ITEM_IS_NOT_EXIST);
			return;
		}

		if (!ItemBase::CanTrade(&igd))
		{
			role->NoticeNum(errornum::EN_ITEM_CANNOT_TRADE);
			return;
		}

		if (mailsend->item_comsume_num[i] < 0 || igd.num < mailsend->item_comsume_num[i])
		{
			role->NoticeNum(errornum::EN_ITEM_NUM_NOT_ENOUGH);
			return;
		}

		igd.num = mailsend->item_comsume_num[i];
		igd.WrapTo(&gcms.contentparam.item_list[i]);
		gcms.contentparam.item_list[i].gold_price = 0;
	}

	for (int i = 0; i < mailsend->item_count; i++)
	{
		if (!knapsack->ConsumeItemByIndex(mailsend->item_knapindex_list[i], gcms.contentparam.item_list[i].num, "SendMailConsume")) return;

		LOG_QUICK12(gamelog::g_log_quick,
			LOG_TYPE_MAIL_ITEM_OUT, UidToInt(role->GetUserId()), role->GetName(), NULL,
			gcms.contentparam.item_list[i].item_id, gcms.contentparam.item_list[i].num, gcms.contentparam.subject, gcms.contentparam.contenttxt,
			mailsend->recver_uid, 0, 0, 0);
	}

	if (mailsend->coin > 0 && !knapsack->GetMoney()->UseCoin(mailsend->coin, "MailSend")) return;
	// if (mail_postage > 0 && !knapsack->GetMoney()->UseAllCoin(mail_postage, "MailSend")) return;
	if (mailsend->gold > 0 && !knapsack->GetMoney()->UseGold(mailsend->gold, "SendMailConsume", false)) return;

	UserMailManager::Instance().OnSendMail(role, &gcms);

	role->GetMonitor()->AddMonitorNum(MONITOR_TYPE_SENDMAIL_NUM, 1);
	role->OnMailMonitor(mailsend->subject);

	UNSTD_STATIC_CHECK(3 == MAX_ATTACHMENT_ITEM_NUM);

	gamelog::g_log_mail.printf(LL_INFO, "mail_route -> add mail[ ret:succ, sender[ uid:%d,name:%s ], recver[ uid:%d ], "
		"detail[ type:%d,subject:%s ], "
		"attachment[ coin:%d,coin_bind:%d,gold:%d,gold_bind:%d, item1[ %s ], item2[ %s ], item3[ %s ] virtual_items[%s] ",
		UidToInt(UserID((int)gcms.sender_dbindex, (int)gcms.sender_roleid)), (const char*)gcms.sender_name, mailsend->recver_uid,
		(int)gcms.kind, (const char*)gcms.contentparam.subject,
		(int)gcms.contentparam.coin, (int)gcms.contentparam.coin_bind, (int)mailsend->gold, (int)gcms.contentparam.gold_bind, 
		ItemDataWrapperLog(&gcms.contentparam.item_list[0], 0), 
		ItemDataWrapperLog(&gcms.contentparam.item_list[1], 1), 
		ItemDataWrapperLog(&gcms.contentparam.item_list[2], 2),
		MailVirtualItemLog(&gcms.contentparam.virtual_item_list));

	if (mailsend->gold > 0)
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(mailsend->recver_uid);
		GameName recver_name = {0};
		if (NULL != user_node)
		{
			user_node->GetName(recver_name);
		}

		ROLE_LOG_QUICK6(LOG_TYPE_MAIL_SEND_GOLD, role, mailsend->recver_uid, mailsend->gold, recver_name, NULL);
	}
}

void MailRoute::RemoveMail(Role *role, int mail_index)
{
	if (NULL == role)
	{
		return;
	}

	if (mail_index < 0 || mail_index >= MAX_MAIL_NUM) return;

	UserMailManager::Instance().OnDeleteMail(role, mail_index);
}

void MailRoute::LockMail(Role *role, int mail_index)
{
	if (NULL == role)
	{
		return;
	}

	if (mail_index < 0 || mail_index >= MAX_MAIL_NUM) return;

	UserMailManager::Instance().OnLockMail(role, mail_index);
}

void MailRoute::UnlockMail(Role *role, int mail_index)
{
	if (NULL == role)
	{
		return;
	}

	if (mail_index < 0 || mail_index >= MAX_MAIL_NUM) return;

	UserMailManager::Instance().OnUnlockMail(role, mail_index);
}

void MailRoute::GetMailList(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	UserMailManager::Instance().OnFetchMailList(role);
}

void MailRoute::ReadMail(Role *role, int mail_index)
{
	if (NULL == role)
	{
		return;
	}

	if (mail_index < 0 || mail_index >= MAX_MAIL_NUM) return;

	UserMailManager::Instance().OnFetchMailDetail(role, mail_index);
}

void MailRoute::FetchMailAttachment(Role *role, Protocol::CSMailFetchAttachment *attachment)
{
	if (NULL == role)
	{
		return;
	}

	if (attachment->mail_index < 0 || attachment->mail_index >= MAX_MAIL_NUM) return;
	if (attachment->item_index >= MAX_ATTACHMENT_ITEM_NUM) return;
	if (attachment->item_index < 0 && (MAIL_FETCH_VIRTUAL != attachment->item_index && MAIL_FETCH_ALL != attachment->item_index)) return;

// 	if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(attachment->item_num))
// 	{
// 		if (0 != attachment->is_last) role->NoticeNum(errornum::EN_HAS_NO_SPACING);
// 		return;
// 	}

	FetchAttachment fam;
	fam.db_index = role->GetUserId().db_index;
	fam.role_id = role->GetUserId().role_id;
	fam.mail_index = attachment->mail_index;
	fam.item_index = attachment->item_index;
	fam.is_last = attachment->is_last;

	gamelog::g_log_mail.printf(LL_INFO, "mail_route -> attachment request[ ret:succ, uid:%d, role_name:%s, mail_index:%d ]", 
		UidToInt(UserID((int)fam.db_index, (int)fam.role_id)), (const char*)role->GetName(), (int)fam.mail_index);

	UserMailManager::Instance().OnFetchMailAttachment(role, &fam);
}

void MailRoute::OnMailAttachment(Role *role, ggprotocal::MailAttachment *ma)
{
	if (ma->item_count < 0 || ma->item_count > MAX_ATTACHMENT_ITEM_NUM) return;
	
	if (NULL == role)
	{
		return;
	}

	FetchAttachmentAffirm faa;
	faa.db_index = role->GetUserId().db_index;
	faa.role_id = role->GetUserId().role_id;
	faa.mail_index = ma->mail_index;
	faa.item_index = ma->item_index;
	faa.is_last = ma->is_last;

	Knapsack *knapsack = role->GetKnapsack();
	if (!knapsack->CheckEmptyGridNoLessThan(ma->item_count))
	{
		if (0 != ma->is_last) role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);

		faa.result = -1;
		//InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char*)&faa, sizeof(faa));
		UserMailManager::Instance().OnFetchMailAttachmentAffirm(role, &faa);
		return;
	}
	
	long long log_get_mail_gold = 0;

	if (ma->item_index >= 0)
	{
		if (ma->item_index >= MAX_ATTACHMENT_ITEM_NUM)
		{
			faa.result = -1;
			//InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char*)&faa, sizeof(faa));
			UserMailManager::Instance().OnFetchMailAttachmentAffirm(role, &faa);
			return;
		}

		if (ma->item_list[0].item_id != ItemBase::INVALID_ITEM_ID)
		{
			LOG_QUICK12(gamelog::g_log_quick, 
				LOG_TYPE_MAIL_ITEM_IN, UidToInt(role->GetUserId()), role->GetName(), NULL,
				ma->item_list[0].item_id, ma->item_list[0].num, ma->subject, ma->contenttxt,
				ma->sender_uid, 0, 0, 0);

			const ItemBase *itembase = ITEMPOOL->GetItem(ma->item_list[0].item_id);
			if (NULL != itembase) 
			{
				role->GetKnapsack()->Put(ma->item_list[0], PUT_REASON_MAIL_RECEIVE);
			}
			else
			{
				role->NoticeNum(errornum::EN_ITEM_IS_NOT_EXIST);
			}
		}
	}
	else
	{
		if (MAIL_FETCH_ALL == ma->item_index)
		{
			log_get_mail_gold = ma->gold;

			for (int i = 0; i < ma->item_count; i++)
			{
				if (ma->item_list[i].item_id != ItemBase::INVALID_ITEM_ID)
				{
					LOG_QUICK12(gamelog::g_log_quick, 
						LOG_TYPE_MAIL_ITEM_IN, UidToInt(role->GetUserId()), role->GetName(), NULL,
						ma->item_list[i].item_id, ma->item_list[i].num, ma->subject, ma->contenttxt,
						ma->sender_uid, 0, 0, 0);

					const ItemBase *itembase = ITEMPOOL->GetItem(ma->item_list[i].item_id);
					if (NULL != itembase) 
					{
						role->GetKnapsack()->Put(ma->item_list[i], PUT_REASON_MAIL_RECEIVE);
					}
					else
					{
						role->NoticeNum(errornum::EN_ITEM_IS_NOT_EXIST);
					}
				}
			}

			knapsack->GetMoney()->AddCoinBind(ma->coin_bind, "MailReceive");
			knapsack->GetMoney()->AddCoin(ma->coin, "MailReceive");
			knapsack->GetMoney()->AddGoldBind(ma->gold_bind, "MailReceive");
			knapsack->GetMoney()->AddGold(ma->gold, "MailReceive");
		}

		if (MAIL_FETCH_ALL == ma->item_index || MAIL_FETCH_VIRTUAL == ma->item_index)
		{
			for (int i = 0; i < MAX_MAIL_VIRTUAL_ITEM_NUM; ++i)
			{
				switch (ma->virtual_item_list.virtual_item_type[i])
				{
					case MAIL_VIRTUAL_ITEM_BATTLEFIELDHONOR:
					{
						role->GetRoleActivity()->AddBattleFieldHonor((int)ma->virtual_item_list.virtual_item[i], "MailAttachment", true);
					}
					break;

					case MAIL_VIRTUAL_ITEM_EXP:
					{
						role->AddExp(ma->virtual_item_list.virtual_item[i], "MailAttachment", Role::EXP_ADD_REASON_DEFAULT);
					}
					break;

					case MAIL_VIRTUAL_ITEM_GUILDGONGXIAN:
					{
						role->AddGuildGongxian((int)ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_SHENGWANG:
					{
						role->AddShengwang((int)ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_BIND_GOLD:
					{
						role->GetKnapsack()->GetMoney()->AddGoldBind(ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_GOLD:
					{
						log_get_mail_gold += ma->virtual_item_list.virtual_item[i];
						role->GetKnapsack()->GetMoney()->AddGold(ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_BIND_COIN:
					{
						role->GetKnapsack()->GetMoney()->AddCoinBind(ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_CROSS_HONOR:
					{
						role->AddCrossHonor((int)ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_GONGXUN:
					{
						role->AddGongxun((int)ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_CONVERTSHOP_MOJING:
					{
						role->GetRoleShop()->AddChestShopMojing((int)ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG:
					{
						role->GetRoleShop()->AddChestShopShengwang((int)ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_CONVERTSHOP_GONGXUN:
					{
						role->GetRoleShop()->AddChestShopGongXun((int)ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_CONVERTSHOP_WEIWANG:
					{
						role->GetRoleShop()->AddChestShopWeiWang((int)ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_SHENMOZHILI:
					{
						role->GetShenYin()->AddShenYinChouHunScore((int)ma->virtual_item_list.virtual_item[i]);
					}
					break;

					case MAIL_VIRTUAL_ITEM_CONVERTSHOP_HUNJING:
					{
						role->GetRoleShop()->AddChestShopHunjing((int)ma->virtual_item_list.virtual_item[i], "MailAttachment");
					}
					break;

					case MAIL_VIRTUAL_ITEM_TIANSHENHUTI_SCORE:
					{
						TianshenHuti* tianshenhuti = role->GetOtherModule<TianshenHuti>(ROLE_MODULE_TYPE_TIANSHENHUTI);
						if (nullptr != tianshenhuti)
						{
							tianshenhuti->AddRollScore((int)ma->virtual_item_list.virtual_item[i], "MailAttachment");
						}
					}
					break;
				}
			}
		}
	}

	faa.result = 0;
	//InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char*)&faa, sizeof(faa));

	UNSTD_STATIC_CHECK(3 == MAX_ATTACHMENT_ITEM_NUM);

	gamelog::g_log_mail.printf(LL_INFO, "mail_route attachment receive-> role[ uid:%d,name:%s ], "
		"attachment[ coin:%d,coin_bind:%d,gold:%d,gold_bind:%d, item_count:%d, %s, %s, %s] virtual_items[%s]]",
		UidToInt(role->GetUserId()), (const char*)role->GetName(),
		(int)ma->coin, (int)ma->coin_bind, (int)ma->gold, (int)ma->gold_bind, (int)ma->item_count,
		ItemDataWrapperLog(&ma->item_list[0], 0),
		ItemDataWrapperLog(&ma->item_list[1], 1),
		ItemDataWrapperLog(&ma->item_list[2], 2),
		MailVirtualItemLog(&ma->virtual_item_list));

	UserMailManager::Instance().OnFetchMailAttachmentAffirm(role, &faa);

	if (log_get_mail_gold > 0)
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(ma->sender_uid);
		GameName sender_name = {0};
		if (NULL != user_node)
		{
			user_node->GetName(sender_name);
		}
		ROLE_LOG_QUICK6(LOG_TYPE_MAIL_GET_GOLD, role, ma->sender_uid, log_get_mail_gold, sender_name, NULL);
	}

	role->GetMonitor()->AddMonitorNum(MONITOR_TYPE_FETCH_ATTACHMENT_NUM, 1);
}

void MailRoute::CleanMail(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	UserMailManager::Instance().OnCleanMail(role);
}

void MailRoute::MailToUser(const UserID &user_id, int reason, const MailContentParam &contentparam, int mail_kind)
{
	if (mail_kind <= MAIL_TYPE_INVALID || mail_kind >= MAIL_TYPE_MAX) return;

	UserID user_list[1];
	user_list[0] = user_id;
	MailToMultUser(user_list, 1, reason, contentparam, mail_kind);
}

void MailRoute::MailToUser(const UserID &user_id, MailSubject subject, MailContentTxt contenttxt, int mail_kind)
{
	if (mail_kind <= MAIL_TYPE_INVALID || mail_kind >= MAIL_TYPE_MAX) return;

	UserID user_list[1];
	user_list[0] = user_id;
	MailToMultUser(user_list, 1, subject, contenttxt, mail_kind);
}

void MailRoute::MailToMultUser(UserID *user_list, int user_num , MailSubject subject, MailContentTxt contenttxt, int mail_kind)
{
	if (mail_kind <= MAIL_TYPE_INVALID || mail_kind >= MAIL_TYPE_MAX) return;

	static MailContentParam contentparam; contentparam.Reset();
	F_STRNCPY(contentparam.subject, subject, sizeof(contentparam.subject));
	F_STRNCPY(contentparam.contenttxt, contenttxt, sizeof(contentparam.contenttxt));
	MailToMultUser(user_list, user_num, 0, contentparam, mail_kind);
}

void MailRoute::MailToMultUser(UserID *user_list, int user_num , int reason, const MailContentParam &contentparam, int mail_kind)
{
	if (NULL == user_list || user_num <= 0 || user_num > MAX_GAMEWORLD_SEND_MAIL_USER) return;
	if (mail_kind <= MAIL_TYPE_INVALID || mail_kind >= MAIL_TYPE_MAX) return;

	UserMailManager::Instance().MailToMultUser(mail_kind, user_list, user_num, reason, contentparam);
}

void MailRoute::MailToAllUser(int reason, const MailContentParam &contentparam, UserCacheFilterFunc filter_func, void **func_param)
{
	const static int USERID_LIST_MAX_COUNT = 256;

	int userid_count = 0, from = 0;
	static UserID userid_list[USERID_LIST_MAX_COUNT];

	int try_count = 0;
	do 
	{
		userid_count = UserCacheManager::Instance().GetUserIDList(from, USERID_LIST_MAX_COUNT, userid_list, filter_func, func_param);
		if (userid_count > 0)
		{
			MailRoute::Instance().MailToMultUser(userid_list, userid_count, reason, contentparam, MAIL_TYPE_SYSTEM);
		}

	} while (0 != userid_count && try_count ++ < 512);
}

void MailRoute::MailToAllUser(int reason, MailSubject subject, MailContentTxt contenttxt, UserCacheFilterFunc filter_func, void **func_param)
{
	static MailContentParam contentparam; contentparam.Reset();
	F_STRNCPY(contentparam.subject, subject, sizeof(contentparam.subject));
	F_STRNCPY(contentparam.contenttxt, contenttxt, sizeof(contentparam.contenttxt));
	MailRoute::Instance().MailToAllUser(reason, contentparam, filter_func, func_param);
}

void MailRoute::MailToCrossUser(const UniqueUserID &unique_user_id, int reason, const MailContentParam &contentparam)
{
	static crossgameprotocal::GameCrossSendMailToUser gcsmtu;
	gcsmtu.unique_user_id = UniqueUserIDToLongLong(unique_user_id);
	gcsmtu.contentparam = contentparam;
	InternalComm::Instance().SendToCross((const char *)&gcsmtu, sizeof(gcsmtu));
}

void MailRoute::MailToRole(Role *role, int reason, const MailContentParam &contentparam)
{
	if (NULL != role)
	{
		if (CrossConfig::Instance().IsHiddenServer())
		{
			MailRoute::MailToCrossUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID(), reason, contentparam);
		}
		else
		{
			MailRoute::MailToUser(role->GetUserId(), reason, contentparam);
		}
	}
}

bool MailRoute::FastMail(int uid, const ItemConfigData &item_configdata, const char *content_str, const char *subject_str)
{
	if (uid <= 0)
	{
		return false;
	}

	const ItemBase *itembase = ITEMPOOL->GetItem(item_configdata.item_id);
	if (NULL == itembase) return false;

	static MailContentParam contentparam; contentparam.Reset();
	contentparam.item_list[0].item_id = item_configdata.item_id;
	contentparam.item_list[0].is_bind = (short)item_configdata.is_bind;
	contentparam.item_list[0].num = item_configdata.num;
	contentparam.item_list[0].invalid_time = itembase->CalInvalidTime();

	if (NULL != subject_str) // 有传主题字符串才赋值，不是必须的
	{
		SNPRINTF(contentparam.subject, sizeof(contentparam.subject), subject_str);
	}

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), content_str);
	if (length > 0)
	{

		MailRoute::MailToUser(IntToUid(uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
		return true;
	}

	return false;
}

bool MailRoute::FastMailList(int uid, const ItemConfigData item_configdata_list[MAX_ATTACHMENT_ITEM_NUM], const char *content_str, const char *subject_str)
{
	static MailContentParam contentparam; contentparam.Reset();

	int attach_num = 0;
	for (int i = 0; attach_num < MAX_ATTACHMENT_ITEM_NUM && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		if (0 != item_configdata_list[i].item_id && item_configdata_list[i].num > 0)
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(item_configdata_list[i].item_id);
			if (NULL == itembase) return false;

			contentparam.item_list[attach_num].item_id = item_configdata_list[i].item_id;
			contentparam.item_list[attach_num].is_bind = (short)item_configdata_list[i].is_bind;
			contentparam.item_list[attach_num].num = item_configdata_list[i].num;
			contentparam.item_list[attach_num].invalid_time = itembase->CalInvalidTime();

			++attach_num;
		}
	}

	if (NULL != subject_str) // 有传主题字符串才赋值，不是必须的
	{
		SNPRINTF(contentparam.subject, sizeof(contentparam.subject), subject_str);
	}

	int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), content_str);
	if (length2 > 0)
	{
		MailRoute::MailToUser(IntToUid(uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
		return true;
	}

	return false;
}

bool UserCacheLevelFilter(const UserCacheNode &user_node, void **func_param)
{
	if (NULL == func_param) return false;

	int min_level_limit = *((int*)func_param);

	return user_node.level >= min_level_limit; 
}

bool UserCacheCampFilter(const UserCacheNode &user_node, void **func_param)
{
	if (NULL == func_param) return false;

	int limit_camp = *((int*)func_param);

	return user_node.camp == limit_camp; 
}



