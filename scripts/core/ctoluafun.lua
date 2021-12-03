function CL_OnHotUpdate()
	__hotupdate()
end

function CL_OnHandleMessage(c_scene, c_role, obj_id, c_datastream)
	MessageHandleManagr.Instance:HandleMessage(c_scene, c_role, obj_id, c_datastream)
end

function CL_OnLuaGM(c_role, cmd)
	GmManager.Instance:OnGm(c_role, cmd)
end

function CL_InitRoleDBProfile(c_datastream)
	DBManager.Instance:InitRoleDBProfile(c_datastream)
end

function CL_GetInitRoleDBProfile(c_datastream)
	DBManager.Instance:GetInitRoleDBProfile(c_datastream)
end

function CL_InitWorldDBProfile(c_datastream)
	DBManager.Instance:InitWorldDBProfile(c_datastream)
end

function CL_GetInitWorldDBProfile(c_datastream)
	DBManager.Instance:GetInitWorldDBProfile(c_datastream)
end

