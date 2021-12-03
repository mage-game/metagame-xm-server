#ifndef __SERIALIZE_MESSAGEHANDLER_H__
#define __SERIALIZE_MESSAGEHANDLER_H__

#include "gamedef.h"
#include "serialize_base.hpp"

class SceneManager;
class Scene;
class Role;

class SerializeMessageHandler 
{
public:
	SerializeMessageHandler();
	~SerializeMessageHandler();

	void Init(SceneManager *scene_manager);
	void Release();

	void HandleMessage(const GSNetID &netid, Scene *scene, Role *role, unsigned short msg_type, const char *msg, int length);

	static bool is_print_msg_type_from_client;

protected:
	void OnShenYinSystemReq(const GSNetID &netid, Scene *scene, Role *role, Protocol::MsgSerializeBase *data);
	void OnFabaoReq(const GSNetID &netid, Scene *scene, Role *role, Protocol::MsgSerializeBase *data);
	void OnShizhuangUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, Protocol::MsgSerializeBase *data);
	void OnShizhuangUpEquipLevelReq(const GSNetID &netid, Scene *scene, Role *role, Protocol::MsgSerializeBase *data);
private:
	typedef void (SerializeMessageHandler::*HandleFunc)(const GSNetID &netid, Scene *scene, Role *role, Protocol::MsgSerializeBase *msg);
	HandleFunc m_msg_handler_list[Protocol::AUTO_PTOROCOL_MAX_MSG_COUNT];

	SceneManager	*m_scene_manager;
}; 

#endif

