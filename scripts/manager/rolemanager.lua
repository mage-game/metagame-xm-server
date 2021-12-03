RoleManager = RoleManager or BaseClass(BaseHotUpdate)
function RoleManager:__init()
	if RoleManager.Instance then
		ErrorLog("[RoleManager] Attemp to create a singleton twice !")
	end
	
	RoleManager.Instance = self
end

function RoleManager:__delete()
	RoleManager.Instance = nil
end

function RoleManager:GetRoleByCRole(c_role)
	if nil == c_role then
		return nil
	end

	local role_id = c_role:GetUID()
	return self:GetObject(role_id)
end

function RoleManager:GetRoleById(role_id)
	return self:GetObject(role_id)
end

function RoleManager:OnAddRole(role_id, c_role)
	if role_id <= 0 or nil == c_role then
		return
	end

	self:DeleteRole(role_id)
	role = self:CreateDynamicObject(role_id, Role, c_role)
	Log(string.format("%d create role!", role_id))
end

function RoleManager:DeleteRole(role_id)
	if nil == self:GetRoleById(role_id) then
		return
	end

	self:DeleteObject(role_id)
	Log(string.format("%d delete role!", role_id))
end

function RoleManager:OnDestoryRole(role_id)
	local role = self:GetRoleById(role_id)
	if nil == role then
		return
	end

	role:DestoryC()
	self:DeleteRole(role_id)
	Log(string.format("%d destory role!", role_id))
end

function RoleManager:ReCalcRoleAttr(roleid, c_base_attr_add, recalc_reason, recalc_all)
	local role = self:GetRoleById(roleid)

	if nil == role then
		return
	end

	role:GetLingyu():ReCalcAttr(c_base_attr_add, RECALC_REASON_TYPE.LINGYU == recalc_reason or recalc_all)
	role:GetTianxiangeFb():ReCalcAttr(c_base_attr_add, RECALC_REASON_TYPE.TIANXIANGEFB == recalc_reason or recalc_all)
end

function RoleManager:OnRoleLogout(role_id)
	local role = self:GetRoleById(role_id)
	if nil ~= role then
		role:OnRoleLogout()
	end
end

function RoleManager:OnResetData(roleid, old_dayid, now_dayid)
	local role = self:GetRoleById(roleid)
	if nil ~= role then
		role:OnResetData(old_dayid, now_dayid)
	end
end

function RoleManager:OnRoleDieInStaticScene(role_id, killer_id)
	local role = self:GetRoleById(role_id)
	if nil ~= role then
		role:OnDieInStaticScene(killer_id)
	end
end
