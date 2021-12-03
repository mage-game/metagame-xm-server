-- 灵玉操作
CSLingyuOperate = CSLingyuOperate or BaseClass(BaseProtocolStruct)
function CSLingyuOperate:__init()
	self.msg_type = 15100
end

function CSLingyuOperate:Decode(c_datastream)
	self.operate = c_datastream:ReadInt()
	self.param1 = c_datastream:ReadInt()
	self.param2 = c_datastream:ReadInt()
	self.param3 = c_datastream:ReadInt()
end

-- 灵玉信息
SCLingyuInfo = SCLingyuInfo or BaseClass(BaseProtocolStruct)
function SCLingyuInfo:__init()
	self.msg_type = 15101
	self.starlevel_t = {}
end

function SCLingyuInfo:Encode(c_datastream)
	for i = 0, Lingyu.MAX_PART - 1 do
		for k=0,Lingyu.MAX_COLOR - 1 do
			c_datastream:WriteShort(self.starlevel_t[i][k] or 0)
		end
	end
end

-- 天仙阁信息
SCTianxiangeInfo = SCTianxiangeInfo or BaseClass(BaseProtocolStruct)
function SCTianxiangeInfo:__init()
	self.msg_type = 15102
end

function SCTianxiangeInfo:Encode(c_datastream)
	c_datastream:WriteInt(self.level)
end

-- 天仙阁场景信息
SCTianxiangeSceneInfo = SCTianxiangeSceneInfo or BaseClass(BaseProtocolStruct)
function SCTianxiangeSceneInfo:__init()
	self.msg_type = 15103
end

function SCTianxiangeSceneInfo:Encode(c_datastream)
	c_datastream:WriteInt(self.level)
	c_datastream:WriteUInt(self.time_out_stamp)
	c_datastream:WriteChar(self.is_finish)
	c_datastream:WriteChar(self.is_pass)
	c_datastream:WriteUShort(self.pass_time_s)
end

-- 挑战副本操作
CSChallengeFbOperate = CSChallengeFbOperate or BaseClass(BaseProtocolStruct)
function CSChallengeFbOperate:__init()
	self.msg_type = 15104
end

function CSChallengeFbOperate:Decode(c_datastream)
	self.operate = c_datastream:ReadInt()
	self.param1 = c_datastream:ReadInt()
	self.param2 = c_datastream:ReadInt()
	self.param3 = c_datastream:ReadInt()
end

-- 挑战副本信息
SCChallengeFbInfo = SCChallengeFbInfo or BaseClass(BaseProtocolStruct)
function SCChallengeFbInfo:__init()
	self.msg_type = 15105
	self.level_t = {}
end

function SCChallengeFbInfo:Encode(c_datastream)
	c_datastream:WriteChar(self.join_times)
	c_datastream:WriteChar(self.buy_join_times)
	c_datastream:WriteChar(self.item_buy_join_times)
	c_datastream:WriteChar(self.free_autofb_times)

	for i = 0, ChallengeFb.MAX_LEVEL - 1 do
		c_datastream:WriteChar(self.level_t[i].is_pass)
		c_datastream:WriteChar(self.level_t[i].fight_layer)
		c_datastream:WriteShort(0)
	end
end

-- 挑战副本 扫荡结果
SCChallengeAutoResult = SCChallengeAutoResult or BaseClass(BaseProtocolStruct)
function SCChallengeAutoResult:__init()
	self.msg_type = 15106
	self.reward_item_list = {} 
end

function SCChallengeAutoResult:Encode(c_datastream)
	c_datastream:WriteInt(self.reward_coin)
	c_datastream:WriteInt(self.reward_exp)
	c_datastream:WriteInt(#self.reward_item_list)
	for i, v in ipairs(self.reward_item_list) do
		c_datastream:WriteUShort(v.item_id)
		c_datastream:WriteShort(v.num)
		c_datastream:WriteInt(v.is_bind)
	end
end

-- 挑战副本（灵玉副本）场景信息
SCChallengeSceneInfo = SCChallengeSceneInfo or BaseClass(BaseProtocolStruct)
function SCChallengeSceneInfo:__init()
	self.msg_type = 15107
end

function SCChallengeSceneInfo:Encode(c_datastream)
	c_datastream:WriteShort(self.level)
	c_datastream:WriteShort(self.layer)
	c_datastream:WriteUInt(self.time_out_stamp)
	c_datastream:WriteChar(self.is_finish)
	c_datastream:WriteChar(self.is_pass)
	c_datastream:WriteUShort(self.pass_time_s)
end