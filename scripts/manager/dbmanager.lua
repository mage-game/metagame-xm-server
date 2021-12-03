DBManager = DBManager or BaseClass(BaseHotUpdate)
function DBManager:__init()
	if DBManager.Instance then
		ErrorLog("[DBManager] Attemp to create a singleton twice !")
	end
	
	DBManager.Instance = self
end

function DBManager:__delete()
	DBManager.Instance = nil
end

function DBManager:CreateObjects()
	self:AddObject("roledb", RoleDB.New(self))
	self:AddObject("worlddb", WorldDB.New(self))
end

function DBManager:InitRoleDBProfile(c_datastream)
	if nil == c_datastream then
		return
	end

	local ret = self:GetRoleDB():UnSerializeProfile(c_datastream)
	if ret < 0 then
		ErrorLog("[RoleDB] UnSerializeProfile Fail", ret)
	end
end

function DBManager:GetInitRoleDBProfile(c_datastream)
	if nil == c_datastream then
		return
	end

	self:GetRoleDB():SerializeProfile(c_datastream)
end

function DBManager:InitWorldDBProfile(c_datastream)
	if nil == c_datastream then
		return
	end

	local ret = self:GetWorldDB():UnSerializeProfile(c_datastream)
	if ret < 0 then
		ErrorLog("[WorldDB] UnSerializeProfile Fail", ret)
	end
end

function DBManager:GetInitWorldDBProfile(c_datastream)
	if nil == c_datastream then
		return
	end

	self:GetWorldDB():SerializeProfile(c_datastream)
end

function DBManager:OnRoleInitParam(role_id, c_param_p, c_roledb)
	local role = RoleManager.Instance:GetRoleById(role_id)
	if nil == role then
		return
	end

	self:GetRoleDB():OnInitParam(c_param_p, c_roledb, role)
end

function DBManager:OnRoleGetInitParam(role_id, c_param_p, c_roledb)
	local role = RoleManager.Instance:GetRoleById(role_id)
	if nil == role then
		return
	end

	self:GetRoleDB():OnGetInitParam(c_param_p, c_roledb, role)
end

function DBManager:OnWorldInitParam(c_param_p, c_worlddb)
	self:GetWorldDB():OnInitParam(c_param_p, c_worlddb)
end

function DBManager:OnWorldGetInitParam(c_param_p, c_worlddb)
	self:GetWorldDB():OnGetInitParam(c_param_p, c_worlddb)
end

function DBManager:GetRoleDB()
	return self:GetObject("roledb")
end

function DBManager:GetWorldDB()
	return self:GetObject("worlddb")
end