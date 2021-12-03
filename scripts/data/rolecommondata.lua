RoleCommonData = RoleCommonData or BaseClass(BaseHotUpdate)

function RoleCommonData:__init(c_role)
	self.c_role = c_role
	self:InitDataset()
end

function RoleCommonData:__delete()
	self.c_role = nil
end

function RoleCommonData:InitDataset()
	self.dataset.tianxiange_level = 0					--天仙阁已通关等级
end

function RoleCommonData:Serialize(c_roledb)
	c_roledb:WriteShort(self.dataset.tianxiange_level)
	c_roledb:WriteShort(0)
end

function RoleCommonData:UnSerialize(c_roledb)
	self.dataset.tianxiange_level = c_roledb:ReadShort()
	c_roledb:ReadShort()
end

function RoleCommonData:GetTianxiangeLevel()
	return self.dataset.tianxiange_level
end

function RoleCommonData:SetTianxiangeLevel(level)
	if level > self.dataset.tianxiange_level then
		self.dataset.tianxiange_level = level
	end
end
