#include "rmiglobal.hpp"

#include "servercommon/struct/global/campparam.hpp"
#include "servercommon/struct/global/guildparam.hpp"
#include "servercommon/struct/global/publicsaleparam.hpp"
#include "servercommon/struct/global/maillistparam.hpp"
#include "servercommon/struct/global/rankparam.hpp"
#include "servercommon/struct/global/tuodanparam.hpp"
#include "servercommon/struct/global/worldstatusparam.hpp"
#include "servercommon/struct/global/offlineregisterparam.hpp"
#include "servercommon/struct/global/crossrecordlistparam.hpp"
#include "servercommon/struct/global/challengeuserparam.hpp"
#include "servercommon/struct/global/fishpoolparam.hpp"
#include "servercommon/struct/global/spaceparam.hpp"
#include "servercommon/struct/global/globalroledataparam.hpp"
#include "servercommon/struct/global/worldstatus2param.hpp"

#include "servercommon/darmi/daglobal.hpp"
#include "servercommon/servercommon.h"
#include "engineadapter.h"

#include "gamelog.h"
#include "servercommon/struct/global/worldstatus3param.hpp"

USE_DATAACCESS_VAR();

static char TLV_BUFF[10 * 1024 * 1024];

enum RMI_INIT_ERROR_TYPE
{
	RMI_INIT_ERROR_REQUEST,
	RMI_INIT_ERROR_RESPONSE,
	RMI_INIT_ERROR_EXCEPTION,
	RMI_INIT_ERROR_TIMEOUT,
};

static void RMIInitError(int err, const char *msg, bool stop = true) 
{
	printf("RMI Init Error[type: %d, msg: %s]\n", err, NULL == msg ? "" : msg);

	if (stop)
	{
		EngineAdapter::Instance().StopGame();
	}
}

//static void RMISaveError(int err, const char *msg)
//{
//	printf("RMI Save Error[type: %d, msg: %s]\n", err, NULL == msg ? "" : msg);
//}

bool RMIGlobalClient::InitCampAsyn(rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_CAMP, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitCampAsyn");
		return false;
	}

	gamelog::g_log_world.printf(LL_MAINTANCE, "InitCampAsyn start!");

	return ret;
}

bool RMIInitCampBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitCampAsyn");
		return false;
	}

	static CampParam camp_param;
	camp_param.count = 0;

	if (ret == 0)
	{
		if (!camp_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitCampAsyn");
			return false;
		}
	}

	this->InitCampRet(ret, camp_param);

	return true;
}

void RMIInitCampBackObject::InitCampRet(int ret, const CampParam &camp_param)
{
	printf("InitCampRet:%d\n", ret);
}

void RMIInitCampBackObject::__exception(int error)
{
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitCampAsyn");
}

void RMIInitCampBackObject::__timeout()
{
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitCampAsyn");
}

void RMIInitCampBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::SaveCampAsyn(const CampParam &camp_param, rmi::RMIBackObject *backobj)
{	
	UNSTD_STATIC_CHECK(sizeof(CampParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = camp_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_CAMP, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISaveCampBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;

	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveCampRet(ret);

	return true;
}

void RMISaveCampBackObject::SaveCampRet(int ret)
{
	printf("SaveCampOfficialRet:%d\n", ret);
}

void RMISaveCampBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::CreateGuildAsyn(int create_db_index, const GuildBaseParam &base_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(GuildBaseParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(create_db_index);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = base_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::CREATE_GUILD, 
		s, backobj, true, 5000);

	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMICreateGuildBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	GuildID guild_id = 0;

	if (in_param.Pop(&ret) && in_param.Pop(&guild_id))
	{
		this->CreateGuildRet(ret, guild_id);
		return true;
	}

	return false;
}

void RMICreateGuildBackObject::CreateGuildRet(int ret, GuildID guild_id)
{
	printf("CreateGuildRet:%d guild_id:%d\n", ret, guild_id);
}

void RMICreateGuildBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::DismissGuildAsyn(GuildID guild_id, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(GuildID) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(guild_id);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::DESTROY_GUILD, 
		s, backobj, true, 5000);

	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMIDismissGuildBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;

	if (in_param.Pop(&ret))
	{
		this->DismissGuildRet(ret);
		return true;
	}
	
	return false;
}

void RMIDismissGuildBackObject::DismissGuildRet(int ret)
{
	printf("DismissGuildRet: %d\n", ret);
}

void RMIDismissGuildBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::InitGuildAsyn(GuildID guild_id_from, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(GuildID) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(guild_id_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitGuildAsyn");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_GUILD, 
		s, backobj, true ,5000);

	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitGuildAsyn");
		return false;
	}

	return ret;
}

bool RMIInitGuildBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitGuildAsyn");
		return false;
	}

	static AllGuildParam all_guild_list;
	all_guild_list.count = 0;

	if (ret == 0)
	{
		if (!all_guild_list.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitGuildAsyn");
			return false;
		}
	}

	this->InitGuildRet(ret, all_guild_list);

	return true;
}

void RMIInitGuildBackObject::InitGuildRet(int ret, const AllGuildParam &all_guild_list)
{
	printf("RMIInitGuildBackObject::InitGuildRet:%d\n", ret);
}

void RMIInitGuildBackObject::__exception(int error)
{
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitGuildAsyn");
}

void RMIInitGuildBackObject::__timeout()
{
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitGuildAsyn");
}

void RMIInitGuildBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::SaveGuildAsyn(GuildID guild_id, AllGuildParam::GuildParam &guild_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK((sizeof(GuildID) + sizeof(AllGuildParam)) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(guild_id);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = guild_param.base_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = guild_param.apply_for_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = guild_param.member_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = guild_param.event_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_GUILD, 
		s, backobj, true, 10000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISaveGuildBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveGuildRet(ret);

	return true;
}

void RMISaveGuildBackObject::SaveGuildRet(int ret)
{
	printf("SaveGuildRet:%d\n", ret);
}

void RMISaveGuildBackObject::__free()
{
	delete this;
}

void RMISaveGuildBackObject::__exception(int error)
{
	printf("SaveGuildRet __exception: %d !!!\n", error);
}

void RMISaveGuildBackObject::__timeout()
{
	printf("SaveGuildRet __timeout !!!\n");
}

bool RMIGlobalClient::InitPublicSaleAsyn(long long id_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(id_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitPublicSaleAsyn");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_PUBLIC_SALE, s, backobj, true, 5000); 
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitPublicSaleAsyn");
		return false;
	}

	return ret;
}

bool RMIInitPublicSaleBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitPublicSaleAsyn");
		return false;
	}

	static PublicSaleParam publicsale_param;
	publicsale_param.count = 0;

	if (ret == 0)
	{
		if (!publicsale_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitPublicSaleAsyn");
			return false;
		}
	}

	this->InitPublicSaleRet(ret, publicsale_param);

	return true;
}

void RMIInitPublicSaleBackObject::InitPublicSaleRet(int ret, const PublicSaleParam &publicsale_param)
{
	printf("RMIInitPublicSaleBackObject::InitPublicSaleRet:%d\n", ret);
}

void RMIInitPublicSaleBackObject::__exception(int error)
{
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitPublicSaleAsyn");
}

void RMIInitPublicSaleBackObject::__timeout()
{
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitPublicSaleAsyn");
}

void RMIInitPublicSaleBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::SavePublicSaleAsyn(const PublicSaleParam &publicsale_param, rmi::RMIBackObject *backobj)
{	
	UNSTD_STATIC_CHECK(sizeof(PublicSaleParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = publicsale_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_PUBLIC_SALE, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISavePublicSaleBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SavePublicSaleRet(ret);

	return true;
}

void RMISavePublicSaleBackObject::SavePublicSaleRet(int ret)
{
	printf("SavePublicSaleRet:%d\n", ret);
}

void RMISavePublicSaleBackObject::__free()
{
	delete this;
}

void RMISavePublicSaleBackObject::__exception(int error)
{
	printf("SavePublicSaleRet __exception: %d !!!\n", error);
}

void RMISavePublicSaleBackObject::__timeout()
{
	printf("SavePublicSaleRet __timeout !!!\n");
}

//bool RMIInitWantBuyBackObject::__response(TLVUnserializer &in_param)
//{
//	int ret = -1;
//	if (!in_param.Pop(&ret))
//	{
//		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitWantBuyAsyn");
//		return false;
//	}
//
//	static WantBuyParam wantbuy_param;
//	wantbuy_param.count = 0;
//
//	if (ret == 0)
//	{
//		if (!wantbuy_param.Unserialize(in_param))
//		{
//			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitWantBuyAsyn");
//			return false;
//		}
//	}
//
//	this->InitWantBuyRet(ret, wantbuy_param);
//
//	return true;
//}
//
//void RMIInitWantBuyBackObject::InitWantBuyRet(int ret, const WantBuyParam &wantbuy_param)
//{
//	printf("RMIInitWantBuyBackObject::InitWantBuyRet:%d\n", ret);
//}
//
//void RMIInitWantBuyBackObject::__exception(int error)
//{
//	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitWantBuyAsyn");
//}
//
//void RMIInitWantBuyBackObject::__timeout()
//{
//	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitWantBuyAsyn");
//}
//
//void RMIInitWantBuyBackObject::__free()
//{
//	delete this;
//}
//
//bool RMISaveWantBuyBackObject::__response(TLVUnserializer &in_param)
//{
//	int ret = -1;
//	if (!in_param.Pop(&ret))
//	{
//		return false;
//	}
//
//	this->SaveWantBuyRet(ret);
//
//	return true;
//}
//
//void RMISaveWantBuyBackObject::__free()
//{
//	delete this;
//}
//
//void RMISaveWantBuyBackObject::SaveWantBuyRet(int ret)
//{
//	printf("SavePublicSaleRet:%d\n", ret);
//}
//
//void RMISaveWantBuyBackObject::__exception(int error)
//{
//	printf("SavePublicSaleRet __exception: %d !!!\n", error);
//}
//
//void RMISaveWantBuyBackObject::__timeout()
//{
//	printf("SavePublicSaleRet __timeout !!!\n");
//}

bool RMIGlobalClient::InitMailsAsyn(int uid, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK((sizeof(int) + sizeof(int)) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(uid);
	if (!ret) 
	{
		delete backobj;
		return false;
	}
	
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_MAILS, s, backobj, true, 5000);
	}

	if (!ret) 
	{
		delete backobj;
	}

	return ret;
}

bool RMIInitMailsBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;

	if (!in_param.Pop(&ret))
	{
		return false;
	}

	static MailListParam mail_param;
	mail_param.count = 0;
	
	if (0 == ret)
	{
		if (!mail_param.Unserialize(in_param))
		{
			return false;
		}
	}
	
	this->InitMailsRet(ret, mail_param);

	return true;
}

void RMIInitMailsBackObject::InitMailsRet(int ret, const MailListParam &mail_param)
{
	printf("InitMailsRet:%d\n", ret);
}

void RMIInitMailsBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::SaveMailsAsyn(int uid, const MailListParam &mail, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK((sizeof(int) + sizeof(int) + sizeof(MailListParam)) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(uid);
	if (!ret) 
	{
		delete backobj;
		return false;
	}
	ret = mail.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_MAILS, 
		s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISaveMailBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;

	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveMailsRet(ret);

	return true;
}

void RMISaveMailBackObject::SaveMailsRet(int ret)
{
	printf("SaveMailsRet:%d\n", ret); 
}

void RMISaveMailBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::GetSystemMailsAsyn(int uid, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK((sizeof(int) + sizeof(int)) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(uid);
	if (!ret) 
	{
		delete backobj;
		return false;
	}

	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::GET_SYSTEM_MAILS, 
			s, backobj, false, 5000);
	}

	if (!ret) 
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMIGetSystemMailBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;

	if (!in_param.Pop(&ret))
	{
		return false;
	}

	static MailListParam mail_param;
	mail_param.count = 0;

	if (0 == ret)
	{
		if (!mail_param.Unserialize(in_param))
		{
			return false;
		}
	}

	this->GetSystemMailsRet(ret, mail_param);

	return true;
}

void RMIGetSystemMailBackObject::GetSystemMailsRet(int ret, const MailListParam &mail_param)
{
	printf("RMIGetSystemMailBackObject:%d\n", ret);
}

void RMIGetSystemMailBackObject::__free()
{
	delete this;
}

void RMIGetSystemMailBackObject::__timeout()
{
	printf("RMIGetSystemMailBackObject: __timeout\n");
}

bool RMIGlobalClient::RemoveSystemMailsAsyn(int uid, const MailListParam &mail_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK((sizeof(int) + sizeof(int) + sizeof(MailListParam)) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(mail_param.count);
	if (!ret) 
	{
		delete backobj;
		return false;
	}

	for (int i = 0; i < mail_param.count; ++i)
	{
		ret = s.Push(mail_param.mail_list[i].system_mail_id);
		if (!ret) 
		{
			delete backobj;
			return false;
		}
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::REMOVE_SYSTEM_MAILS, s, backobj, false, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMIRemoveSystemMailBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->RemoveSystemMailsRet(ret);

	return true;
}

void RMIRemoveSystemMailBackObject::RemoveSystemMailsRet(int ret)
{
	printf("RemoveSystemMailsRet:%d\n", ret); 
}

void RMIRemoveSystemMailBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::InitPersonRankAsyn(rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_PERSON_RANK, 
		s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitPersonRankAsyn");
		return false;
	}

	return ret;
}

bool RMIInitPersonRankBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitPersonRankAsyn");
		return false;
	}

	static PersonRankParam person_rank_param;
	person_rank_param.count = 0;

	if (0 == ret)
	{
		if (!person_rank_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitPersonRankAsyn");
			return false;
		}
	}

	this->InitPersonRankRet(ret, person_rank_param);

	return true;
}

void RMIInitPersonRankBackObject::InitPersonRankRet(int ret, const PersonRankParam &person_rank_param)
{
	printf("InitPersonRankRet:%d\n", ret);
}

void RMIInitPersonRankBackObject::__exception(int error)
{
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitPersonRankAsyn");
}

void RMIInitPersonRankBackObject::__timeout()
{
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitPersonRankAsyn");
}

void RMIInitPersonRankBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::SavePersonRankAsyn(const PersonRankParam &person_rank_param, rmi::RMIBackObject *backobj)
{	
	UNSTD_STATIC_CHECK(sizeof(PersonRankParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = person_rank_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_PERSON_RANK,
		s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}
bool RMISavePersonRankBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SavePersonRankRet(ret);

	return true;
}

void RMISavePersonRankBackObject::SavePersonRankRet(int ret)
{
	printf("SavePersonRankRet:%d\n", ret);
}

void RMISavePersonRankBackObject::__exception(int error)
{
	printf("RMISavePersonRankBackObject SaveError\n");
}

void RMISavePersonRankBackObject::__timeout()
{
}

void RMISavePersonRankBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::InitTuodanAsyn(int tuodanid_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(tuodanid_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitGuildAsyn");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_TUODAN, 
		s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitTuodanAsyn");
		return false;
	}

	return ret;
}

bool RMIInitTuodanBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitTuodanAsyn");
		return false;
	}

	static TuodanParam tuodan_param;
	tuodan_param.count = 0;

	if (0 == ret)
	{
		if (!tuodan_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitTuodanAsyn");
			return false;
		}
	}

	this->InitTuodanRet(ret, tuodan_param);

	return true;
}

void RMIInitTuodanBackObject::InitTuodanRet(int ret, const TuodanParam &tuodan_param)
{
	printf("InitTuodanRet:%d\n", ret);
}

void RMIInitTuodanBackObject::__exception(int error)
{
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitTuodanAsyn");
}

void RMIInitTuodanBackObject::__timeout()
{
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitTuodanAsyn");
}

void RMIInitTuodanBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::SaveTuodanAsyn(const TuodanParam &tuodan_param, rmi::RMIBackObject *backobj)
{	
	UNSTD_STATIC_CHECK(sizeof(TuodanParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = tuodan_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_TUODAN,
		s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISaveTuodanBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveTuodanRet(ret);

	return true;
}

void RMISaveTuodanBackObject::SaveTuodanRet(int ret)
{
	printf("SaveTuodanRet:%d\n", ret);
}

void RMISaveTuodanBackObject::__exception(int error)
{
	printf("RMISaveTuodanBackObject SaveError\n");
}

void RMISaveTuodanBackObject::__timeout()
{
}

void RMISaveTuodanBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::InitTeamRankAsyn(rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_TEAM_RANK, 
		s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitTeamRankAsyn");
		return false;
	}

	return ret;
}

bool RMIInitTeamRankBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitTeamRankAsyn");
		return false;
	}

	static TeamRankParam team_rank_param;
	team_rank_param.count = 0;

	if (0 == ret)
	{
		if (!team_rank_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitTeamRankAsyn");
			return false;
		}
	}

	this->InitTeamRankRet(ret, team_rank_param);

	return true;
}

void RMIInitTeamRankBackObject::InitTeamRankRet(int ret, const TeamRankParam &team_rank_param)
{
	printf("InitTeamRankRet:%d\n", ret);
}

void RMIInitTeamRankBackObject::__exception(int error)
{
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitTeamRankAsyn");
}

void RMIInitTeamRankBackObject::__timeout()
{
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitTeamRankAsyn");
}

void RMIInitTeamRankBackObject::__free()
{
	delete this;
}

bool RMIGlobalClient::SaveTeamRankAsyn(const TeamRankParam &team_rank_param, rmi::RMIBackObject *backobj)
{	
	UNSTD_STATIC_CHECK(sizeof(TeamRankParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = team_rank_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_TEAM_RANK,
		s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}
bool RMISaveTeamRankBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveTeamRankRet(ret);

	return true;
}

void RMISaveTeamRankBackObject::SaveTeamRankRet(int ret)
{
	printf("SaveTeamRankRet:%d\n", ret);
}

void RMISaveTeamRankBackObject::__exception(int error)
{
	printf("RMISaveTeamRankBackObject SaveError\n");
}

void RMISaveTeamRankBackObject::__timeout()
{
}

void RMISaveTeamRankBackObject::__free()
{
	delete this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//bool RMIGlobalClient::InitGuildUnionAsyn(long long id_from, rmi::RMIBackObject *backobj)
//{
//	UNSTD_STATIC_CHECK(sizeof(long long) * 2 < sizeof(TLV_BUFF));
//
//	TLVSerializer s;
//	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));
//
//	bool ret = s.Push(id_from);
//	if (!ret)
//	{
//		delete backobj;
//		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitGuildUnionAsyn");
//		return false;
//	}
//
//	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_GUILD_UNION, 
//		s, backobj, true, 5000);
//	if (!ret)
//	{
//		delete backobj;
//		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitGuildUnionAsyn");
//		return false;
//	}
//
//	return ret;
//}
//
//bool RMIInitGuildUnionBackObject::__response(TLVUnserializer &in_param)
//{
//	int ret = -1;
//	if (!in_param.Pop(&ret))
//	{
//		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitGuildUnionAsyn");
//		return false;
//	}
//
//	static GuildUnionParam guild_union_param;
//	guild_union_param.count = 0;
//
//	if (0 == ret)
//	{
//		if (!guild_union_param.Unserialize(in_param))
//		{
//			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitGuildUnionAsyn");
//			return false;
//		}
//	}
//
//	this->InitGuildUnionRet(ret, guild_union_param);
//
//	return true;
//}
//
//void RMIInitGuildUnionBackObject::InitGuildUnionRet(int ret, const GuildUnionParam &guild_union_param)
//{
//	printf("InitGuildUnionAsyn:%d\n", ret);
//}
//
//void RMIInitGuildUnionBackObject::__exception(int error)
//{
//	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitGuildUnionAsyn");
//}
//
//void RMIInitGuildUnionBackObject::__timeout()
//{
//	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitGuildUnionAsyn");
//}
//
//void RMIInitGuildUnionBackObject::__free()
//{
//	delete this;
//}
//
//bool RMIGlobalClient::SaveGuildUnionAsyn(GuildUnionParam &guild_union_param, rmi::RMIBackObject *backobj)
//{
//	UNSTD_STATIC_CHECK(sizeof(GuildUnionParam) * 2 < sizeof(TLV_BUFF));
//
//	TLVSerializer s;
//	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));
//
//	bool ret = guild_union_param.Serialize(s);
//	if (!ret)
//	{
//		delete backobj;
//		return false;
//	}
//
//	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_GUILD_UNION, 
//		s, backobj, true, 10000);
//	if (!ret)
//	{
//		delete backobj;
//		return false;
//	}
//
//	return ret;
//}
//
//bool RMISaveGuildUnionBackObject::__response(TLVUnserializer &in_param)
//{
//	int ret = -1;
//	if (!in_param.Pop(&ret))
//	{
//		return false;
//	}
//
//	this->SaveGuildUnionRet(ret);
//
//	return true;
//}
//
//void RMISaveGuildUnionBackObject::SaveGuildUnionRet(int ret)
//{
//	printf("SaveGuildUnionRet:%d\n", ret);
//}
//
//void RMISaveGuildUnionBackObject::__exception(int error)
//{
//	printf("RMISaveGuildUnionBackObject SaveError\n");
//}
//
//void RMISaveGuildUnionBackObject::__timeout()
//{
//}
//
//void RMISaveGuildUnionBackObject::__free()
//{
//	delete this;
//}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bool RMIGlobalClient::InitGuildUnionApplyForAsyn(long long id_from, rmi::RMIBackObject *backobj)
//{
//	UNSTD_STATIC_CHECK(sizeof(long long) * 2 < sizeof(TLV_BUFF));
//
//	TLVSerializer s;
//	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));
//
//	bool ret = s.Push(id_from);
//	if (!ret)
//	{
//		delete backobj;
//		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitGuildUnionApplyForAsyn");
//		return false;
//	}
//
//	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_GUILD_UNION_APPLYFOR, 
//		s, backobj, true, 5000);
//	if (!ret)
//	{
//		delete backobj;
//		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitGuildUnionApplyForAsyn");
//		return false;
//	}
//
//	return ret;
//}
//
//bool RMIInitGuildUnionApplyForBackObject::__response(TLVUnserializer &in_param)
//{
//	int ret = -1;
//	if (!in_param.Pop(&ret))
//	{
//		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitGuildUnionAsyn");
//		return false;
//	}
//
//	static GuildUnionApplyForParam guild_union_applyfor_param;
//	guild_union_applyfor_param.count = 0;
//
//	if (0 == ret)
//	{
//		if (!guild_union_applyfor_param.Unserialize(in_param))
//		{
//			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitGuildUnionApplyForAsyn");
//			return false;
//		}
//	}
//
//	this->InitGuildUnionApplyForRet(ret, guild_union_applyfor_param);
//
//	return true;
//}
//
//void RMIInitGuildUnionApplyForBackObject::InitGuildUnionApplyForRet(int ret, const GuildUnionApplyForParam &guild_union_applyfor_param)
//{
//	printf("InitGuildUnionApplyForAsyn:%d\n", ret);
//}
//
//void RMIInitGuildUnionApplyForBackObject::__exception(int error)
//{
//	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitGuildUnionApplyForAsyn");
//}
//
//void RMIInitGuildUnionApplyForBackObject::__timeout()
//{
//	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitGuildUnionApplyForAsyn");
//}
//
//void RMIInitGuildUnionApplyForBackObject::__free()
//{
//	delete this;
//}
//
//bool RMIGlobalClient::SaveGuildUnionApplyForAsyn(GuildUnionApplyForParam &guild_union_applyfor_param, rmi::RMIBackObject *backobj)
//{
//	UNSTD_STATIC_CHECK(sizeof(GuildUnionApplyForParam) * 2 < sizeof(TLV_BUFF));
//
//	TLVSerializer s;
//	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));
//
//	bool ret = guild_union_applyfor_param.Serialize(s);
//	if (!ret)
//	{
//		delete backobj;
//		return false;
//	}
//
//	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_GUILD_UNION_APPLYFOR, 
//		s, backobj, true, 10000);
//	if (!ret)
//	{
//		delete backobj;
//		return false;
//	}
//
//	return ret;
//}
//
//bool RMISaveGuildUnionApplyForBackObject::__response(TLVUnserializer &in_param)
//{
//	int ret = -1;
//	if (!in_param.Pop(&ret))
//	{
//		return false;
//	}
//
//	this->SaveGuildUnionApplyForRet(ret);
//
//	return true;
//}
//
//void RMISaveGuildUnionApplyForBackObject::SaveGuildUnionApplyForRet(int ret)
//{
//	printf("SaveGuildUnionApplyForRet:%d\n", ret);
//}
//
//void RMISaveGuildUnionApplyForBackObject::__exception(int error)
//{
//	printf("RMISaveGuildUnionApplyForBackObject SaveError\n");
//}
//
//void RMISaveGuildUnionApplyForBackObject::__timeout()
//{
//}
//
//void RMISaveGuildUnionApplyForBackObject::__free()
//{
//	delete this;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool RMIGlobalClient::InitWorldStatusAsyn(long long id_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(id_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitWorldStatusAsyn");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_WORLDSTATUS, s, backobj, true, 10000); 
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitWorldStatusAsyn");
		return false;
	}

	return ret;
}

bool RMIInitWorldStatusBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitWorldStatusAsyn");
		return false;
	}

	static WorldStatusParam worldstatus_param;

	if (ret == 0)
	{
		if (!worldstatus_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitWorldStatusAsyn");
			return false;
		}
	}

	this->InitWorldStatusRet(ret, worldstatus_param);

	return true;
}

void RMIInitWorldStatusBackObject::InitWorldStatusRet(int ret, const WorldStatusParam &worldstatus_param)
{
	printf("RMIInitWorldStatusBackObject::InitWorldStatusRet:%d\n", ret);
}

void RMIInitWorldStatusBackObject::__exception(int error)
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitWorldStatusAsyn");
}

void RMIInitWorldStatusBackObject::__timeout()
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitWorldStatusAsyn");
}

void RMIInitWorldStatusBackObject::__free()
{
	delete this;
}

void RMIInitWorldStatusBackObject::InitError()
{
	printf("RMIInitWorldStatusBackObject InitError\n");
}

bool RMIGlobalClient::SaveWorldStatusAsyn(const WorldStatusParam &worldstatus_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(WorldStatusParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = worldstatus_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_WORLDSTATUS, s, backobj, true, 10000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISaveWorldStatusBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveWorldStatusRet(ret);

	return true;
}

void RMISaveWorldStatusBackObject::SaveWorldStatusRet(int ret)
{
	printf("SaveWorldStatusRet:%d\n", ret);
}

void RMISaveWorldStatusBackObject::__free()
{
	delete this;
}

void RMISaveWorldStatusBackObject::__exception(int error)
{
	printf("RMISaveWorldStatusBackObject __exception: %d !!!\n", error);
}

void RMISaveWorldStatusBackObject::__timeout()
{
	printf("RMISaveWorldStatusBackObject __timeout !!!\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
bool RMIGlobalClient::InitOfflineRegisterAsyn(long long id_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(id_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitOfflineRegisterAsyn");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_OFFLINE_REGISTER, s, backobj, true, 100000); 
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitOfflineRegisterAsyn");
		return false;
	}

	return ret;
}

bool RMIGlobalClient::InitSpaceRecordAsyn(long long id_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(id_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitSpaceRecordAsyn");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_SPACE_RECORD, s, backobj, true, 100000); 
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitSpaceRecordAsyn");
		return false;
	}

	return ret;
}

bool RMIInitOfflineRegisterBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitOfflineRegisterAsyn");
		return false;
	}

	static OfflineRegisterParam offline_reg_param;
	offline_reg_param.count = 0;

	if (ret == 0)
	{
		if (!offline_reg_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitOfflineRegisterAsyn");
			return false;
		}
	}

	this->InitOfflineRegisterRet(ret, offline_reg_param);

	return true;
}

void RMIInitOfflineRegisterBackObject::InitOfflineRegisterRet(int ret, const OfflineRegisterParam &offline_reg_param)
{
	printf("RMIInitEscortBackObject::InitOfflineRegisterRet:%d\n", ret);
}

void RMIInitOfflineRegisterBackObject::__exception(int error)
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitOfflineRegisterAsyn");
}

void RMIInitOfflineRegisterBackObject::__timeout()
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitOfflineRegisterAsyn");
}

void RMIInitOfflineRegisterBackObject::__free()
{
	delete this;
}

void RMIInitOfflineRegisterBackObject::InitError()
{
	printf("RMIInitOfflineRegisterBackObject InitError\n");
}


bool RMIInitSpaceRecordBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitSpaceRecordAsyn");
		return false;
	}

	static SpaceParam space_param;
	space_param.count = 0;

	if (ret == 0)
	{
		if (!space_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitSpaceRecordAsyn");
			return false;
		}
	}

	this->InitSpaceRecordRet(ret, space_param);

	return true;
}

void RMIInitSpaceRecordBackObject::InitSpaceRecordRet(int ret, const SpaceParam &space_param)
{
	printf("RMIInitEscortBackObject::InitSpaceRecordRet:%d\n", ret);
}

void RMIInitSpaceRecordBackObject::__exception(int error)
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitSpaceRecordAsyn");
}

void RMIInitSpaceRecordBackObject::__timeout()
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitSpaceRecordAsyn");
}

void RMIInitSpaceRecordBackObject::__free()
{
	delete this;
}

void RMIInitSpaceRecordBackObject::InitError()
{
	printf("RMIInitSpaceRecordBackObject InitError\n");
}

bool RMISaveSpaceRecordBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveSpaceRecordRet(ret);

	return true;
}

void RMISaveSpaceRecordBackObject::SaveSpaceRecordRet(int ret)
{
	printf("SaveSpaceRecordRet:%d\n", ret);
}

void RMISaveSpaceRecordBackObject::__free()
{
	delete this;
}

void RMISaveSpaceRecordBackObject::__exception(int error)
{
	printf("RMISaveSpaceRecordBackObject __exception: %d !!!\n", error);
}

void RMISaveSpaceRecordBackObject::__timeout()
{
	printf("RMISaveSpaceRecordBackObject __timeout !!!\n");
}


bool RMIGlobalClient::SaveOfflineRegisterAsyn(const OfflineRegisterParam & offline_reg_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(OfflineRegisterParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = offline_reg_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_OFFLINE_REGISTER, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMIGlobalClient::SaveSpaceRecordAsyn(const SpaceParam & space_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(SpaceParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = space_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_SPACE_RECORD, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISaveOfflineRegisterBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveOfflineRegisterRet(ret);

	return true;
}

void RMISaveOfflineRegisterBackObject::SaveOfflineRegisterRet(int ret)
{
	printf("RMISaveOfflineRegisterBackObject:%d\n", ret);
}

void RMISaveOfflineRegisterBackObject::__free()
{
	delete this;
}

void RMISaveOfflineRegisterBackObject::__exception(int error)
{
	printf("RMISaveOfflineRegisterBackObject __exception: %d !!!\n", error);
}

void RMISaveOfflineRegisterBackObject::__timeout()
{
	printf("RMISaveOfflineRegisterBackObject __timeout !!!\n");
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool RMIGlobalClient::InitChallengeUserAsyn(int rank_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(rank_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitChallengeUserAsyn");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_CHALLENGE_USER, s, backobj, true, 10000); 
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitChallengeUserAsyn");
		return false;
	}

	return ret;
}

bool RMIInitChallengeUserBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "RMIInitChallengeUserBackObject_1");
		return false;
	}

	static ChallengeUserListParam challenge_user_param;
	challenge_user_param.count = 0;

	if (ret == 0)
	{
		if (!challenge_user_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "RMIInitChallengeUserBackObject_2");
			return false;
		}
	}

	this->InitChallengeUserRet(ret, challenge_user_param);

	return true;
}

void RMIInitChallengeUserBackObject::InitChallengeUserRet(int ret, const ChallengeUserListParam &challenge_user_param)
{
	printf("RMIInitChallengeUserBackObject::InitChallengeUserRet:%d\n", ret);
}

void RMIInitChallengeUserBackObject::__exception(int error)
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "RMIInitChallengeUserBackObject::__exception");
}

void RMIInitChallengeUserBackObject::__timeout()
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "RMIInitChallengeUserBackObject::__timeout");
}

void RMIInitChallengeUserBackObject::__free()
{
	delete this;
}

void RMIInitChallengeUserBackObject::InitError()
{
	printf("RMIInitChallengeUserBackObject InitError\n");
}

bool RMIGlobalClient::SaveChallengeUserAsyn(const ChallengeUserListParam &challenge_user_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(ChallengeUserListParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = challenge_user_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_CHALLENGE_USER, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISaveChallengeUserBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveChallengeUserRet(ret);

	return true;
}

void RMISaveChallengeUserBackObject::SaveChallengeUserRet(int ret)
{
	printf("RMISaveChallengeUserBackObject:%d\n", ret);
}

void RMISaveChallengeUserBackObject::__free()
{
	delete this;
}

void RMISaveChallengeUserBackObject::__exception(int error)
{
	printf("RMISaveChallengeUserBackObject __exception: %d !!!\n", error);
}

void RMISaveChallengeUserBackObject::__timeout()
{
	printf("RMISaveChallengeUserBackObject __timeout !!!\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool RMIGlobalClient::InitFishPoolAsyn(long long id_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(id_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitFishPoolAsyn 1");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_FISH_POOL, s, backobj, true, 10000); 
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitFishPoolAsyn 2");
		return false;
	}

	return ret;
}

bool RMIInitFishPoolBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "RMIInitFishPoolBackObject 1");
		return false;
	}

	static FishPoolListParam fish_pool_list_param;
	fish_pool_list_param.count = 0;

	if (ret == 0)
	{
		if (!fish_pool_list_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "RMIInitFishPoolBackObject 2");
			return false;
		}
	}

	this->InitFishPoolRet(ret, fish_pool_list_param);

	return true;
}

void RMIInitFishPoolBackObject::InitFishPoolRet(int ret, const FishPoolListParam &fish_pool_list_param)
{
	printf("RMIInitFishPoolBackObject::InitFishPoolRet:%d \n", ret);
}

void RMIInitFishPoolBackObject::__exception(int error)
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "RMIInitChallengeUserBackObject::__exception");
}

void RMIInitFishPoolBackObject::__timeout()
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "RMIInitFishPoolBackObject::__timeout");
}

void RMIInitFishPoolBackObject::__free()
{
	delete this;
}

void RMIInitFishPoolBackObject::InitError()
{
	printf("RMIInitFishPoolBackObject InitError\n");
}

bool RMIGlobalClient::SaveFishPoolAsyn(const FishPoolListParam &fish_pool_list_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(FishPoolListParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = fish_pool_list_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_FISH_POOL, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISaveFishPoolBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveFishPoolRet(ret);

	return true;
}

void RMISaveFishPoolBackObject::SaveFishPoolRet(int ret)
{
	printf("RMISaveFishPoolBackObject :%d\n", ret);
}

void RMISaveFishPoolBackObject::__free()
{
	delete this;
}

void RMISaveFishPoolBackObject::__exception(int error)
{
	printf("RMISaveFishPoolBackObject __exception: %d !!!\n", error);
}

void RMISaveFishPoolBackObject::__timeout()
{
	printf("RMISaveFishPoolBackObject __timeout !!!\n");
}


bool RMIInitCrossRecordBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "RMIInitCrossRecordBackObject_1");
		return false;
	}

	static CrossRecordListParam crossrecord_list_param;
	crossrecord_list_param.count = 0;

	if (ret == 0)
	{
		if (!crossrecord_list_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "RMIInitCrossRecordBackObject_2");
			return false;
		}
	}

	this->InitCrossRecordRet(ret, crossrecord_list_param);

	return true;
}

void RMIInitCrossRecordBackObject::InitCrossRecordRet(int ret, const CrossRecordListParam &crossrecord_list_param)
{
	printf("RMIInitCrossRecordBackObject::InitCrossRecordRet:%d\n", ret);
}

void RMIInitCrossRecordBackObject::__exception(int error)
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "RMIInitCrossRecordBackObject::__exception");
}

void RMIInitCrossRecordBackObject::__timeout()
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "RMIInitCrossRecordBackObject::__timeout");
}

void RMIInitCrossRecordBackObject::__free()
{
	delete this;
}

void RMIInitCrossRecordBackObject::InitError()
{
	printf("RMIInitCrossRecordBackObject InitError\n");
}


bool RMISaveCrossRecordBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveCrossRecordRet(ret);

	return true;
}

void RMISaveCrossRecordBackObject::SaveCrossRecordRet(int ret)
{
	printf("RMISaveCrossRecordBackObject:%d\n", ret);
}

void RMISaveCrossRecordBackObject::__free()
{
	delete this;
}

void RMISaveCrossRecordBackObject::__exception(int error)
{
	printf("RMISaveCrossRecordBackObject __exception: %d !!!\n", error);
}

void RMISaveCrossRecordBackObject::__timeout()
{
	printf("RMISaveCrossRecordBackObject __timeout !!!\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool RMIGlobalClient::InitCrossRecordAsyn(long long id_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(id_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitCrossRecordAsyn");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_CROSSRECORD, s, backobj, true, 10000); 
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitCrossRecordAsyn");
		return false;
	}

	return ret;
}

bool RMIGlobalClient::SaveCrossRecordAsyn(const CrossRecordListParam &crossrecord_list_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(CrossRecordListParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = crossrecord_list_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_CROSSRECORD, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}


bool RMIGlobalClient::InitGlobalRoleAsyn(long long id_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(id_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitGlobalRoleAsyn 1");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_GLOBAL_ROLE, s, backobj, true, 10000);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitGlobalRoleAsyn 2");
		return false;
	}

	return ret;
}

bool RMIGlobalClient::SaveGlobalRoleAsyn(const GlobalRoleDatalListParam &global_role_list_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(GlobalRoleDatalListParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = global_role_list_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_GLOBAL_ROLE, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMIInitGlobalRoleBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "RMIInitGlobalRoleBackObject 1");
		return false;
	}

	static GlobalRoleDatalListParam global_role_list_param;
	global_role_list_param.count = 0;

	if (ret == 0)
	{
		if (!global_role_list_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "RMIInitGlobalRoleBackObject 2");
			return false;
		}
	}

	this->InitGlobalRet(ret, global_role_list_param);

	return true;
}

void RMIInitGlobalRoleBackObject::InitGlobalRet(int ret, const GlobalRoleDatalListParam &challenge_user_param)
{
	printf("RMIInitGlobalRoleBackObject::InitFishPoolRet:%d \n", ret);
}

void RMIInitGlobalRoleBackObject::__exception(int error)
{
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "RMIInitGlobalRoleBackObject::__exception");
}

void RMIInitGlobalRoleBackObject::__timeout()
{
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "RMIInitGlobalRoleBackObject::__timeout");
}

void RMIInitGlobalRoleBackObject::__free()
{
	delete this;
}

bool RMISaveGlobalRoleBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveGlobalRoleRet(ret);

	return true;
}

void RMISaveGlobalRoleBackObject::SaveGlobalRoleRet(int ret)
{
	printf("RMISaveGlobalRoleBackObject :%d\n", ret);
}

void RMISaveGlobalRoleBackObject::__free()
{
	delete this;
}

void RMISaveGlobalRoleBackObject::__exception(int error)
{
	printf("RMISaveGlobalRoleBackObject __exception: %d !!!\n", error);
}

void RMISaveGlobalRoleBackObject::__timeout()
{
	printf("RMISaveGlobalRoleBackObject __timeout !!!\n");
}
bool RMIGlobalClient::InitWorldStatus2Asyn(long long id_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(id_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitWorldStatus2Asyn");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_WORLDSTATUS2, s, backobj, true, 10000);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitWorldStatus2Asyn");
		return false;
	}

	return ret;
}

bool RMIInitWorldStatus2BackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitWorldStatus2Asyn");
		return false;
	}

	static WorldStatus2Param worldstatus2_param;

	if (ret == 0)
	{
		if (!worldstatus2_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitWorldStatus2Asyn");
			return false;
		}
	}

	this->InitWorldStatus2Ret(ret, worldstatus2_param);

	return true;
}

void RMIInitWorldStatus2BackObject::InitWorldStatus2Ret(int ret, const WorldStatus2Param &worldstatus2_param)
{
	printf("RMIInitWorldStatus2BackObject::InitWorldStatus2Ret:%d\n", ret);
}

void RMIInitWorldStatus2BackObject::__exception(int error)
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitWorldStatus2Asyn");
}

void RMIInitWorldStatus2BackObject::__timeout()
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitWorldStatus2Asyn");
}

void RMIInitWorldStatus2BackObject::__free()
{
	delete this;
}

void RMIInitWorldStatus2BackObject::InitError()
{
	printf("RMIInitWorldStatus2BackObject InitError\n");
}

bool RMIGlobalClient::SaveWorldStatus2Asyn(const WorldStatus2Param &worldstatus2_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(WorldStatus2Param) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = worldstatus2_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_WORLDSTATUS2, s, backobj, true, 10000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISaveWorldStatus2BackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveWorldStatus2Ret(ret);

	return true;
}

void RMISaveWorldStatus2BackObject::SaveWorldStatus2Ret(int ret)
{
	printf("SaveWorldStatus2Ret:%d\n", ret);
}

void RMISaveWorldStatus2BackObject::__free()
{
	delete this;
}

void RMISaveWorldStatus2BackObject::__exception(int error)
{
	printf("RMISaveWorldStatus2BackObject __exception: %d !!!\n", error);
}

void RMISaveWorldStatus2BackObject::__timeout()
{
	printf("RMISaveWorldStatus2BackObject __timeout !!!\n");
}

bool RMIGlobalClient::InitWorldStatus3Asyn(long long id_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(id_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitWorldStatus3Asyn");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::INIT_WORLDSTATUS3, s, backobj, true, 10000);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitWorldStatus3Asyn");
		return false;
	}

	return ret;
}

bool RMIInitWorldStatus3BackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitWorldStatus3Asyn");
		return false;
	}

	static WorldStatus3Param worldstatus3_param;

	if (ret == 0)
	{
		if (!worldstatus3_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitWorldStatus3Asyn");
			return false;
		}
	}

	this->InitWorldStatus3Ret(ret, worldstatus3_param);

	return true;
}

void RMIInitWorldStatus3BackObject::InitWorldStatus3Ret(int ret, const WorldStatus3Param &worldstatus3_param)
{
	printf("RMIInitWorldStatus3BackObject::InitWorldStatus3Ret:%d\n", ret);
}

void RMIInitWorldStatus3BackObject::__exception(int error)
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitWorldStatus3Asyn");
}

void RMIInitWorldStatus3BackObject::__timeout()
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitWorldStatus3Asyn");
}

void RMIInitWorldStatus3BackObject::__free()
{
	delete this;
}

void RMIInitWorldStatus3BackObject::InitError()
{
	printf("RMIInitWorldStatus3BackObject InitError\n");
}

bool RMIGlobalClient::SaveWorldStatus3Asyn(const WorldStatus3Param &worldstatus3_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(WorldStatus3Param) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = worldstatus3_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::GLOBAL, dataaccessrmi::global::SAVE_WORLDSTATUS3, s, backobj, true, 10000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISaveWorldStatus3BackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveWorldStatus3Ret(ret);

	return true;
}

void RMISaveWorldStatus3BackObject::SaveWorldStatus3Ret(int ret)
{
	printf("SaveWorldStatus3Ret:%d\n", ret);
}

void RMISaveWorldStatus3BackObject::__free()
{
	delete this;
}

void RMISaveWorldStatus3BackObject::__exception(int error)
{
	printf("RMISaveWorldStatus3BackObject __exception: %d !!!\n", error);
}

void RMISaveWorldStatus3BackObject::__timeout()
{
	printf("RMISaveWorldStatus3BackObject __timeout !!!\n");
}
