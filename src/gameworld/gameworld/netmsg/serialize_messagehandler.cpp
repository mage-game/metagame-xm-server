#include <assert.h>

#include "gamelog.h"
#include "static_byte_array.hpp"
#include "serialize_messagehandler.h"
#include "engineadapter.h"
#include "obj/character/role.h"

#include "protocol/protocol_110.hpp"
#include "other/shenyin/shenyin.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "protocal/msgrole.h"
#include "gameworld/gameworld/other/shizhuang/shizhuang.h"

#define SERIALIZE_REGISTER_MSG(CLASS_NAME, FUNCTION_PTR) \
m_msg_handler_list[CLASS_NAME::MSG_TYPE - Protocol::AUTO_PROTOCOL_RESERVE_MIN] = FUNCTION_PTR;\
UNSTD_STATIC_CHECK(CLASS_NAME::MSG_TYPE >= Protocol::AUTO_PROTOCOL_RESERVE_MIN && CLASS_NAME::MSG_TYPE <= Protocol::AUTO_PROTOCOL_RESERVE_MAX)

bool SerializeMessageHandler::is_print_msg_type_from_client = false;

SerializeMessageHandler::SerializeMessageHandler() : m_scene_manager(NULL)
{
	memset(m_msg_handler_list, 0, sizeof(m_msg_handler_list));

	SERIALIZE_REGISTER_MSG(Protocol::CSTianxiangOperaReq, &SerializeMessageHandler::OnShenYinSystemReq);
	SERIALIZE_REGISTER_MSG(Protocol::CSFabaoOperateReq, &SerializeMessageHandler::OnFabaoReq);
	SERIALIZE_REGISTER_MSG(Protocol::CSShizhuangSkillUplevelReq,&SerializeMessageHandler::OnShizhuangUpLevelReq);
	SERIALIZE_REGISTER_MSG(Protocol::CSShizhuangUplevelEquip,&SerializeMessageHandler::OnShizhuangUpEquipLevelReq);
}

SerializeMessageHandler::~SerializeMessageHandler()
{
}

void SerializeMessageHandler::Init(SceneManager * scene_manager)
{
	assert(NULL != scene_manager);
	m_scene_manager = scene_manager;
}

void SerializeMessageHandler::Release()
{
	m_scene_manager = NULL;
}

void SerializeMessageHandler::HandleMessage(const GSNetID &netid, Scene *scene, Role *role, unsigned short msg_type, const char *msg, int length)
{
	if (length < 0) // 允许内容长度为0的空请求，上层已经解析过协议类型了
	{
		return;
	}

	int msg_index = msg_type - Protocol::AUTO_PROTOCOL_RESERVE_MIN;

 	if (msg_index < 0 || msg_index >= Protocol::AUTO_PTOROCOL_MAX_MSG_COUNT || NULL == m_msg_handler_list[msg_index])
	{
		printf("SerializeMessageHandler::HandleMessage m_msg_handler_list[%u] Error \n", msg_type);
		gamelog::g_log_disconnect.printf(LL_CRITICAL, "SerializeMessageHandler::HandleMessage m_msg_handler_list[%u] Error", msg_type);
		return;
	}
	
	Protocol::MsgSerializeBase *serialize_msg = MsgSerializeManager::GetInstance().GetSerializeObj(msg_type);
	if (NULL == serialize_msg)
	{
		printf("SerializeMessageHandler::HandleMessage - MsgSerializeManager GetSerializeObj(%u) == NULL\n", msg_type);
		gamelog::g_log_disconnect.printf(LL_CRITICAL, "SerializeMessageHandler::HandleMessage no this msg object[%u].", msg_type);
		return;
	}

	static StaticByteArray s_bytearray;
	s_bytearray.Clear();
	s_bytearray.WriteWriteObject(msg, length);

	serialize_msg->Unserialize(s_bytearray);
	(this->*m_msg_handler_list[msg_index])(netid, scene, role, serialize_msg);

#ifdef _DEBUG
	// 打印协议号，用于调试
	if (is_print_msg_type_from_client)
	{
		static unsigned msg_cnt = 0;
		printf("[DEBUG] New user msg[%u], user[%d], msg_type[%d], msg_length[%u]\n",
			++msg_cnt, role->GetUID(), (int)msg_type, length);
	}
#endif
}

void SerializeMessageHandler::OnShenYinSystemReq(const GSNetID &netid, Scene *scene, Role *role, Protocol::MsgSerializeBase *data)
{
	Protocol::CSTianxiangOperaReq *txr = dynamic_cast<Protocol::CSTianxiangOperaReq*>(data);

	role->GetShenYin()->ReqOpera(txr->info_type, txr->param1, txr->param2, txr->param3, txr->param4);
}

void SerializeMessageHandler::OnFabaoReq(const GSNetID& netid, Scene* scene, Role* role, Protocol::MsgSerializeBase* data)
{

	Protocol::CSFabaoOperateReq *t_data = dynamic_cast<Protocol::CSFabaoOperateReq*>(data);
	switch (t_data->req_type)
	{
		// 请求进阶
	case Protocol::CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UPGRADE: 
		{
			//bool need_continue_upgrade = true;
			bool ret = role->GetFabaoManager()->OnUpGrade(t_data->param1 > 0, t_data->param2);
			role->SendOperateResult(Protocol::SCOperateResult::OP_FABAO_UPGRADE, ret);
		}
		break;

		// 请求激活特殊形象
	case Protocol::CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_ACTIVESPECIALIMG: 
		{
			bool ret = role->GetFabaoManager()->ActiveSpecialImg(t_data->param1, t_data->param2 > 0);
			role->SendOperateResult(Protocol::SCOperateResult::OP_FABAO_UPGRADE, ret);
		}
		break;

		// 请求撤消特殊形象
	case Protocol::CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UNACTIVESPECIALIMG: 
		{
			role->GetFabaoManager()->UnActiveSpecialImg(t_data->param1);
		}
		break;

		// 请求使用特殊形象
	case Protocol::CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_USESPECIALIMG: 
		{
			role->GetFabaoManager()->UseSpecialImg(t_data->param1);
		}
		break;

		// 请求卸下特殊形象
	case Protocol::CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UNUSESPECIALIMG: 
		{
			role->GetFabaoManager()->UnUseSpecialImg(t_data->param1);
		}
		break;

		// 特殊形象进阶
	case Protocol::CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_FABAOSPECIALIMGUPGRADE: 
		{
			bool ret = role->GetFabaoManager()->FabaoSpecialImgUpgrade(t_data->param1);
			role->SendOperateResult(Protocol::SCOperateResult::OP_FABAO_SPECIAL_IMG_UPGRADE, ret);
		}
		break;

		// 使用形象,param1 = 形象id
	case Protocol::CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_USEIMA:
		{
			role->GetFabaoManager()->UseImg(t_data->param1);
		}
		break;

	case Protocol::CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UPLEVELSKILL:
		{
			bool ret = role->GetFabaoManager()->UplevelSkill(t_data->param1, t_data->param2 > 0);
			role->SendOperateResult(Protocol::SCOperateResult::OP_FABAO_UPLEVELSKILL, ret);
		}
		break;

	case Protocol::CS_FABAO_REQ_TYPE_CS_FABAO_REQ_TYPE_UPLEVELEQUIP:
		{
			bool ret = role->GetFabaoManager()->UplevelEquip(t_data->param1);
			role->SendOperateResult(Protocol::SCOperateResult::OP_FABAO_UPLEVELEQUIP, ret);
		}
		break;

	default:
		break;
		
	}
}

void SerializeMessageHandler::OnShizhuangUpEquipLevelReq(const GSNetID& netid, Scene* scene, Role* role, Protocol::MsgSerializeBase* data)
{
	Protocol::CSShizhuangUplevelEquip *sse =dynamic_cast<Protocol::CSShizhuangUplevelEquip*>(data);
	role->GetShizhuang()->UplevelEquip(role, sse->shizhuang_type, sse->equip_idx);
}

void SerializeMessageHandler::OnShizhuangUpLevelReq(const GSNetID& netid, Scene* scene, Role* role, Protocol::MsgSerializeBase* data)
{
	Protocol::CSShizhuangSkillUplevelReq *ssu = dynamic_cast<Protocol::CSShizhuangSkillUplevelReq*>(data);
	role->GetShizhuang()->UpSkillLevel(role, ssu->shizhuang_type, ssu->skill_idx, 0 != ssu->auto_buy);
}