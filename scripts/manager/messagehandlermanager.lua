MessageHandleManagr = MessageHandleManagr or BaseClass(BaseHotUpdate)
function MessageHandleManagr:__init()
	if MessageHandleManagr.Instance then
		ErrorLog("[MessageHandleManagr] Attemp to create a singleton twice !")
	end
	
	MessageHandleManagr.Instance = self

	self.msg_handle_fun_t = {}
	self:RegisterProtocols()
end

function MessageHandleManagr:__delete()
	MessageHandleManagr.Instance = nil
	self.msg_handle_fun_t = {}
end

function MessageHandleManagr:HandleMessage(c_scene, c_role, c_obj_id, c_datastream)
	if nil == c_datastream then
		return
	end
	
	local msg_type = c_datastream:ReadUShort()
	local server_id = c_datastream:ReadUShort()

	local protocol_obj = self:GetObject(msg_type)
	if nil ~= protocol_obj and protocol_obj.Decode then
		protocol_obj:Decode(c_datastream)

		local handle_fun = self.msg_handle_fun_t[msg_type]
		local role = RoleManager.Instance:GetRoleByCRole(c_role)
		if nil ~= role and nil ~= handle_fun then
			handle_fun(self, role, protocol_obj)	
		end
	end
end

function MessageHandleManagr:RegisterProtocols()
	local msg_handle_t = self:GetMessageHandleList()

	for k, v in pairs(msg_handle_t) do
		local msg = v[1].New()
		self:AddObject(msg:GetMsgType(), msg)

		self.msg_handle_fun_t[msg:GetMsgType()] = v[2]
	end
end

function MessageHandleManagr:GetMessageHandleList()
	return {
		{CSLingyuOperate, self.LingyuOperate},
		{CSChallengeFbOperate, self.ChallengeFbOperate},
	}
end

-- 角色上线时，服务器会主动推送一些需要的相关消息给客户端
function MessageHandleManagr:OnRoleSendAllInfo(roleid)
	local role = RoleManager.Instance:GetRoleById(roleid)
	if nil == role then
		return
	end

	role:GetTianxiangeFb():SyncTianxiangeInfo()
end

-- 灵玉操作处理
function MessageHandleManagr:LingyuOperate(role, protocol)
	if MSG_OPERATE_TYPE['LINGYU_REQINFO'] == protocol.operate then
		role:GetLingyu():SyncLingyuInfo()
	end

	if MSG_OPERATE_TYPE['LINGYU_UPSTAR'] == protocol.operate then
		role:GetLingyu():OnUpStar(protocol.param1, protocol.param2, protocol.param3)
	end
end

-- 挑战副本操作处理
function MessageHandleManagr:ChallengeFbOperate(role, protocol)
	if MSG_OPERATE_TYPE['CHALLENGEFB_REQINFO'] == protocol.operate then
		role:GetChallengeFb():SyncChallengeFbInfo()
	end

	if MSG_OPERATE_TYPE['CHALLENGEFB_BUYJOINTIMES'] == protocol.operate then
		role:GetChallengeFb():BuyJoinTimes()
	end

	if MSG_OPERATE_TYPE['CHALLGNGEFB_AUTO'] == protocol.operate then
		role:GetChallengeFb():AutoFBReq(protocol.param1, protocol.param2)
	end

	if MSG_OPERATE_TYPE['CHALLENGEFB_RESETLEVEL'] == protocol.operate then
		role:GetChallengeFb():ResetLevel(protocol.param1)
	end
end