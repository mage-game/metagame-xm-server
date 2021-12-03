TianxiangeFb = TianxiangeFb or BaseClass(BaseHotUpdate)

function TianxiangeFb:__init(role)
	self.role = role
	self:InitDataset()
end

function TianxiangeFb:__delete()
	self.role = nil
end

function TianxiangeFb:__delete()
	self.role = nil
end

function TianxiangeFb:InitDataset()
	self.dataset.attrs_add = CFactory:CCharIntAttrs()
end

function TianxiangeFb:ReCalcAttr(c_base_add, is_recalc)
	if is_recalc then
		self.dataset.attrs_add:Reset()
		local level = self.role:GetCommonData():GetTianxiangeLevel()
		local cfg = TianxiangeFbCfg.Instance:GetLevelCfg(level)
		if nil ~= cfg then
			self.dataset.attrs_add:AddValue(C_CharIntAttrs.BASE_CHARINTATTR_TYPE_GONGJI, cfg.gongji)
			self.dataset.attrs_add:AddValue(C_CharIntAttrs.BASE_CHARINTATTR_TYPE_FANGYU, cfg.fangyu)
			self.dataset.attrs_add:AddValue(C_CharIntAttrs.BASE_CHARINTATTR_TYPE_MAXHP, cfg.maxhp)
			self.dataset.attrs_add:AddValue(C_CharIntAttrs.BASE_CHARINTATTR_TYPE_MINGZHONG, cfg.mingzhong)
			self.dataset.attrs_add:AddValue(C_CharIntAttrs.BASE_CHARINTATTR_TYPE_SHANBI, cfg.shanbi)
		end
	end

	c_base_add:AddAttrs(self.dataset.attrs_add)
	self.role:ReCalcCap(CAPABILITY_TYPE.TIANXIANGEFB, self.dataset.attrs_add)
end

function TianxiangeFb:OnFinish(level)
	self.role:GetCommonData():SetTianxiangeLevel(level)
	self.role:ReCalcAttr(RECALC_REASON_TYPE.TIANXIANGEFB)

	self:SyncTianxiangeInfo()
end

function TianxiangeFb:SyncTianxiangeInfo()
	local protocol = ProtocolPool.Instance:GetProtocol(SCTianxiangeInfo)
	protocol.level = self.role:GetCommonData():GetTianxiangeLevel()
	self.role:SendProtocol(protocol)
end