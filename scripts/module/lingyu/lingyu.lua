Lingyu = Lingyu or BaseClass(BaseHotUpdate)
Lingyu.MAX_PART = 12
Lingyu.MAX_COLOR = 6

function Lingyu:__init(role)
	self.role = role
	self:InitDataset()
end

function Lingyu:__delete()
	self.role = nil
end

function Lingyu:InitDataset()
	self.dataset.attrs_add = CFactory:CCharIntAttrs()
	self.dataset.s_starlevel_t = {}

	for i = 0, Lingyu.MAX_PART - 1 do
		self.dataset.s_starlevel_t[i] = {}
		for k = 0, Lingyu.MAX_COLOR - 1 do
			self.dataset.s_starlevel_t[i][k] = 0
		end
	end
end

function Lingyu:Serialize(c_roledb)
	for i = 0, Lingyu.MAX_PART - 1 do
		for k = 0, Lingyu.MAX_COLOR - 1 do
			c_roledb:WriteShort(self.dataset.s_starlevel_t[i][k])
		end
	end
end

function Lingyu:UnSerialize(c_roledb)
	for i = 0, Lingyu.MAX_PART - 1 do
		for k = 0, Lingyu.MAX_COLOR - 1 do
			self.dataset.s_starlevel_t[i][k] = c_roledb:ReadShort()
		end
	end
end

function Lingyu:ReCalcAttr(c_base_add, is_recalc)
	if is_recalc then
		self.dataset.attrs_add:Reset()

		for i = 0, Lingyu.MAX_PART - 1 do
			for k = 0, Lingyu.MAX_COLOR - 1 do
				local upstar_cfg = LingyuCfg.Instance:GetUpStarCfg(k, self.dataset.s_starlevel_t[i][k])
				if nil ~= upstar_cfg then
					self.dataset.attrs_add:AddValue(C_CharIntAttrs.BASE_CHARINTATTR_TYPE_GONGJI, upstar_cfg.gongji)
					self.dataset.attrs_add:AddValue(C_CharIntAttrs.BASE_CHARINTATTR_TYPE_FANGYU, upstar_cfg.fangyu)
					self.dataset.attrs_add:AddValue(C_CharIntAttrs.BASE_CHARINTATTR_TYPE_MAXHP, upstar_cfg.maxhp)
					self.dataset.attrs_add:AddValue(C_CharIntAttrs.BASE_CHARINTATTR_TYPE_MINGZHONG, upstar_cfg.mingzhong)
					self.dataset.attrs_add:AddValue(C_CharIntAttrs.BASE_CHARINTATTR_TYPE_SHANBI, upstar_cfg.shanbi)
				end
			end
		end
	end

	c_base_add:AddAttrs(self.dataset.attrs_add)
	self.role:ReCalcCap(CAPABILITY_TYPE.LINGYU, self.dataset.attrs_add)
end

function Lingyu:OnUpStar(part_index, color_index, is_autobuy)
	if part_index < 0 or part_index >= Lingyu.MAX_PART then
		return
	end

	if color_index < 0 or color_index >= Lingyu.MAX_COLOR then
		return
	end

	local old_level = self.dataset.s_starlevel_t[part_index][color_index]
	if nil == old_level then
		return
	end

	local upstar_cfg = LingyuCfg.Instance:GetUpStarCfg(color_index, old_level + 1)
	if nil == upstar_cfg then
		self.role:NoticeNumStr(LangTip.max_level)
		return
	end

	local c_stuff_list = CFactory:CConsumeStuffList()
	c_stuff_list:AddStuff(upstar_cfg.stuff_id, upstar_cfg.stuff_count, is_autobuy == 1)
	if not self.role:ConsumeItemList(c_stuff_list, "LingyuUpStar") then
		return
	end

 	self.dataset.s_starlevel_t[part_index][color_index] = self.dataset.s_starlevel_t[part_index][color_index] + 1 
	self.role:ReCalcAttr(RECALC_REASON_TYPE.LINGYU)

	self:SyncLingyuInfo()

	-- 传闻
	if 1 == upstar_cfg.is_broadcast then
		local msg = string.format(LangMsg.lingyu_upstar, 
			self.role:GetRoleId(), self.role:GetName(), self.role:GetCamp(), 
			self.dataset.s_starlevel_t[part_index][color_index])
		WorldManager.Instance:BroadcastMsg(msg)
	end

	local log = string.format("lingyu upstar, part_index %d, old_level %d, level %d", 
		part_index, old_level, self.dataset.s_starlevel_t[part_index][color_index])

	self.role:WriteLog(log) 
end

function Lingyu:SyncLingyuInfo()
	local protocol = ProtocolPool.Instance:GetProtocol(SCLingyuInfo)
	protocol.starlevel_t = self.dataset.s_starlevel_t
	self.role:SendProtocol(protocol)
end