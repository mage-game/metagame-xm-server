-- 协议基类
BaseProtocolStruct = BaseProtocolStruct or BaseClass(BaseHotUpdate)
function BaseProtocolStruct:__init()
	self.msg_type = 0
end

function BaseProtocolStruct:GetMsgType()
	return self.msg_type
end

-- 编码
function BaseProtocolStruct:Encode(c_datastream)
end

-- 解码
function BaseProtocolStruct:Decode(c_datastream)
end

function BaseProtocolStruct:EncodeAndSend(c_netid)
	local c_datastream = C_LuaToCFun:OpenSendMsgStream(c_netid, self.msg_type)
	
	if nil == c_datastream then
		return
	end

	self:Encode(c_datastream)

	C_LuaToCFun:FlushMsgStream(c_datastream)
end

-- 协议池
ProtocolPool = ProtocolPool or BaseClass(BaseHotUpdate)
function ProtocolPool:__init()
	ProtocolPool.Instance = self
	self.protocol_list = {}
	self.protocol_list_by_type = {}
end

function ProtocolPool:__delete()
	self.protocol_list = {}
	self.protocol_list_by_type = {}
end

function ProtocolPool:Register(protocol)
	local reg_protocol = protocol.New()
	if nil ~= self.protocol_list_by_type[reg_protocol.msg_type] then
		ErrorLog("协议重复注册 msg_type:" .. reg_protocol.msg_type)
		reg_protocol:DeleteMe()
		return -1
	end

	self.protocol_list[protocol] = reg_protocol
	self.protocol_list_by_type[reg_protocol.msg_type] = reg_protocol
	return reg_protocol.msg_type
end

function ProtocolPool:GetProtocol(protocol)
	local reg_protocol = self.protocol_list[protocol]
	if nil ~= reg_protocol then
		return reg_protocol
	end
	
	self:Register(protocol)
	return self.protocol_list[protocol]
end