ChallengeFb = ChallengeFb or BaseClass(BaseHotUpdate)
ChallengeFb.MAX_LEVEL = 6
ChallengeFb.MAX_LAYER = 10
ChallengeFb.FREE_JOIN_MAX_TIMES = 2

function ChallengeFb:__init(role)
	self.role = role

	self:InitDataset()
end

function ChallengeFb:__delete()
	self.role = nil
end

function ChallengeFb:InitDataset()
	self.dataset.level_t = {}
	self.dataset.join_times = 0
	self.dataset.buy_join_times = 0
	self.dataset.item_buy_join_times = 0
	self.dataset.free_autofb_times = 0

	for i = 0, ChallengeFb.MAX_LEVEL - 1 do
		local item = {}
		item.is_pass = 0
		item.fight_layer = -1

		self.dataset.level_t[i] = item
	end
end

function ChallengeFb:Serialize(c_roledb)
	c_roledb:WriteChar(self.dataset.join_times)
	c_roledb:WriteChar(self.dataset.buy_join_times)
	c_roledb:WriteChar(self.dataset.item_buy_join_times)
	c_roledb:WriteChar(self.dataset.free_autofb_times)

	for i = 0, ChallengeFb.MAX_LEVEL - 1 do
		local item = self.dataset.level_t[i]
		c_roledb:WriteChar(item.is_pass)
		c_roledb:WriteChar(item.fight_layer)
		c_roledb:WriteShort(0)
	end
end

function ChallengeFb:UnSerialize(c_roledb)
	self.dataset.join_times = c_roledb:ReadChar()
	self.dataset.buy_join_times = c_roledb:ReadChar()
	self.dataset.item_buy_join_times = c_roledb:ReadChar()
	self.dataset.free_autofb_times = c_roledb:ReadChar()

	for i = 0, ChallengeFb.MAX_LEVEL - 1 do
		local item = self.dataset.level_t[i]
		item.is_pass = c_roledb:ReadChar()
		item.fight_layer = c_roledb:ReadChar(-1)
		c_roledb:ReadShort()
	end
end

function ChallengeFb:OnResetData(old_dayid, now_dayid)
	self.dataset.join_times = 0
	self.dataset.buy_join_times = 0
	self.dataset.item_buy_join_times = 0
	self.dataset.free_autofb_times = 0
	self:SyncChallengeFbInfo()
end

function ChallengeFb:CanEnter(level, auto_buy_join_times)
	if level < 0 or level >= ChallengeFb.MAX_LEVEL then
		return false
	end

	local chapter_cfg = ChallengeFbCfg.Instance:GetChappterCfg(level)
	if nil == chapter_cfg then
		return false
	end

	if self.role:GetLevel() < chapter_cfg.role_level then
		self.role:NoticeNumStr(LangTip.role_level_limit)
		return false
	end

	if level > 0 and 0 == self.dataset.level_t[level - 1].is_pass then -- 必须先通关前边的关卡，第一关除外
		return false
	end

	if self.dataset.level_t[level].fight_layer < 0 or self.dataset.level_t[level].fight_layer >= ChallengeFb.MAX_LAYER then
		if self.dataset.join_times >= ChallengeFb.FREE_JOIN_MAX_TIMES + self.dataset.buy_join_times + self.dataset.item_buy_join_times then
			if not auto_buy_join_times then
				self.role:NoticeNumStr(LangTip.times_limit)
				return false
			else
				if not self:BuyJoinTimes() or self.dataset.join_times >= ChallengeFb.FREE_JOIN_MAX_TIMES + self.dataset.buy_join_times + self.dataset.item_buy_join_times then
					return false
				end
			end
		end
	end

	return true
end

function ChallengeFb:OnEnterFB(level)
	if level < 0 or level >= ChallengeFb.MAX_LEVEL then
		return
	end

	if self.dataset.level_t[level].fight_layer < 0 then
		self.dataset.level_t[level].fight_layer = 0
		self.dataset.join_times = self.dataset.join_times + 1
	end

	self:SyncChallengeFbInfo()
end

function ChallengeFb:OnFBFinish(is_pass, scene_id, is_active_leave_fb)
	local level_cfg = ChallengeFbCfg.Instance:GetLevelCfgBySceneID(scene_id)
	if nil == level_cfg then
		return
	end

	local level = level_cfg.level
	if level < 0 or level >= ChallengeFb.MAX_LEVEL then
		return
	end

	local level_item = self.dataset.level_t[level]
	level_item.fight_layer = level_cfg.layer

	if is_pass then
		level_item.fight_layer = level_item.fight_layer + 1
		if level_item.fight_layer >= ChallengeFb.MAX_LAYER then
			if 0 == level_item.is_pass then
				local msg = string.format(LangMsg.challenge_pass, 
					self.role:GetRoleId(), self.role:GetName(), self.role:GetCamp(), level + 1)
				WorldManager.Instance:BroadcastMsg(msg)
			end
			level_item.fight_layer = -1		--下一次又从第一层开始打
			level_item.is_pass = 1
		end

		self.role:Recover()
	end

	self:SyncChallengeFbInfo()
end

function ChallengeFb:GetFightLayer(level)
	if level < 0 or level >= ChallengeFb.MAX_LEVEL then
		return -1
	end

	local layer = self.dataset.level_t[level].fight_layer
	if layer < 0 or layer >= ChallengeFb.MAX_LAYER then
		return 0
	end

	return layer
end

function ChallengeFb:BuyJoinTimes()
	local buy_times = VipManager.Instance:GetAuthParam(self.role:GetVipLevel(), VIP_AUTH_TYPE.CHALLENGEFB_BUYTIMES)
	if self.dataset.buy_join_times >= buy_times then
		self.role:NoticeNumStr(LangTip.times_limit)
		return false
	end

	local need_gold = ChallengeFbCfg.Instance:GetBuyJoinTimes(self.dataset.buy_join_times + 1)
	if need_gold <= 0 then
		return false
	end

	if not self.role:UseGold(need_gold, "BuyChanllengeJoinTimes") then
		return false
	end

	self.dataset.buy_join_times = self.dataset.buy_join_times + 1
	self:SyncChallengeFbInfo()

	return true
end

function ChallengeFb:ResetLevel(level)
	if level < 0 or level >= ChallengeFb.MAX_LEVEL then
		return
	end

	self.dataset.level_t[level].fight_layer = -1
	self:SyncChallengeFbInfo()
end

function ChallengeFb:AutoFBReq(level, is_autobuy)
	if level < 0 or level >= ChallengeFb.MAX_LEVEL then
		return
	end

	local level_item = self.dataset.level_t[level]

	if level_item.fight_layer >= 0 and level_item.fight_layer < ChallengeFb.MAX_LAYER then
		self.role:NoticeNumStr(LangTip.is_challenging)
		return
	end

	if 0 == self.dataset.is_pass then
		return
	end

	if self.dataset.join_times >= ChallengeFb.FREE_JOIN_MAX_TIMES + self.dataset.buy_join_times + self.dataset.item_buy_join_times then
		self.role:NoticeNumStr(LangTip.times_limit)
		return
	end

	local chapter_cfg = ChallengeFbCfg.Instance:GetChappterCfg(level)
	if nil == chapter_cfg then
		return
	end

	if self.role:GetLevel() < chapter_cfg.role_level then
		self.role:NoticeNumStr(LangTip.role_level_limit)
		return
	end

	local is_free = false
	local free_times = VipManager.Instance:GetAuthParam(self.role:GetVipLevel(), VIP_AUTH_TYPE.CHALLENGEFB_FREE_AUTOTIMES)
	if free_times > 0 and self.dataset.free_autofb_times < free_times then
		is_free = true
	end

	-- 获得怪物掉落
	local c_droplist = CFactory:CMonsterDropList()
	for i = level_item.fight_layer, ChallengeFb.MAX_LAYER - 1 do
		local layer_cfg = ChallengeFbCfg.Instance:GetLayerCfg(level, i)
		if nil ~= layer_cfg then
			c_droplist:AddMonsterDrop(layer_cfg.boss_id)
		end
	end

	if not self.role:CheckEmptyGridNoLessThan(c_droplist:GetIndexCount()) then
		self.role:NoticeNumStr(LangTip.knapsack_space_limit)
		return
	end

	if is_free then
		self.dataset.free_autofb_times  = self.dataset.free_autofb_times + 1
	else
		local other_cfg = ChallengeFbCfg.Instance:GetOtherCfg()
		local c_stuff_list = CFactory:CConsumeStuffList()
		c_stuff_list:AddStuff(other_cfg.auto_item_id, other_cfg.auto_item_num, is_autobuy == 1)
		if not self.role:ConsumeItemList(c_stuff_list, "ChallengeFbAuto") then
			return
		end
	end
	
	if level_item.fight_layer < 0 or level_item.fight_layer >= ChallengeFb.MAX_LAYER then
		self.dataset.join_times = self.dataset.join_times + 1
	end 

	level_item.fight_layer = -1
	self.role:PutDropList(c_droplist, PUT_REASON_TYPE.CHALLENGEFB_AUTO)

	local protocol = ProtocolPool.Instance:GetProtocol(SCChallengeAutoResult)
	protocol.reward_coin = c_droplist:GetRewardCoin()
	protocol.reward_exp = c_droplist:GetRewardExp()
	protocol.reward_item_list = {}
	
	local reward_item_count = c_droplist:GetIndexCount()
	for i = 0, reward_item_count - 1 do
		local c_item_config_data = c_droplist:GetItemCfgData(i)
		if nil ~= c_item_config_data then
			local item = {}
			item.item_id = c_item_config_data.item_id
			item.num = c_item_config_data.num
			item.is_bind = c_item_config_data.is_bind and 1 or 0
			table.insert(protocol.reward_item_list, item)
		end
	end

	self.role:SendProtocol(protocol)
	self:SyncChallengeFbInfo()
	self.role:NoticeNumStr(LangTip.autofb_success)
end

function ChallengeFb:SyncChallengeFbInfo()
	local protocol = ProtocolPool.Instance:GetProtocol(SCChallengeFbInfo)
	protocol.join_times = self.dataset.join_times
	protocol.buy_join_times = self.dataset.buy_join_times
	protocol.item_buy_join_times = self.dataset.item_buy_join_times
	protocol.free_autofb_times = self.dataset.free_autofb_times
	protocol.level_t = self.dataset.level_t
	self.role:SendProtocol(protocol)
end