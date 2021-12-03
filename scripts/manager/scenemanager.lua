SceneManager = SceneManager or BaseClass(BaseHotUpdate)
function SceneManager:__init()
	if SceneManager.Instance then
		ErrorLog("[SceneManager] Attemp to create a singleton twice !")
	end
	
	SceneManager.Instance = self
end

function SceneManager:__delete()
	SceneManager.Instance = nil
end

function SceneManager:GetCSceneMgr()
	return WorldManager.Instance:GetCWorld():GetSceneManager()
end

-- 是否可以在场景中重置位置
function SceneManager:IsCanResetScene(scene_type)
	if SCENE_TYPE.TIANXIANGE == scene_type then
		return true
	end

	return false
end

-- 是否是延迟删除的场景（意思是玩家下线后不马上删除，在一定时间内玩家再上线仍在该场景）
function SceneManager:IsDelayDeleteScene(scene_type)
	if SCENE_TYPE.TIANXIANGE == scene_type or SCENE_TYPE.CHALLENGE == scene_type then
		return true
	end 

	return false
end

function SceneManager:IsStaticSceneType(scene_type)
	return C_Scene:IsStaticScene(scene_type)
end

function SceneManager:IsStaticScene(c_scene)
	if nil == c_scene then
		return false
	end
	
	return self:IsStaticSceneType(c_scene:GetSceneType())
end

function SceneManager:GetObj(c_scene, objid)
	if nil == c_scene then
		return nil
	end

	return c_scene:GetObj(objid)
end

function SceneManager:GetCRole(c_scene, objid)
	if nil == c_scene then
		return nil
	end

	return c_scene:GetPrincipal(objid)
end

function SceneManager:GetSceneTimeOutStamp(c_scene)
	if nil == c_scene then
		return 0
	end

	return c_scene:GetSceneTimeOutStamp()
end

function SceneManager:ResetScene(c_scene, role)
	if nil == c_scene or nil == role then
		return false
	end

	return c_scene:ResetScene(role:GetCRole())
end

function SceneManager:GoTo(role, target_sceneid, target_scene_key, posx, posy)
	if nil == role then
		return false
	end

	return self:GetCSceneMgr():GoTo(role:GetCRole(), target_sceneid, target_scene_key, CFactory:CLuaPos(posx, posy))
end

function SceneManager:RoleTimeOut(c_scene, role)
	if nil == c_scene or nil == role then
		return
	end

	self:GetCSceneMgr():RoleTimeOut(c_scene, role:GetCRole(), "lualogout", 0)
end

function SceneManager:CreateMonster(monsterid, c_scene, posx, posy)
	local monster_obj = C_LuaToCFun:CreateMonster(monsterid, c_scene, CFactory:CLuaPos(posx, posy))
	
	if nil == monster_obj then
		return nil, -1
	end

	return monster_obj, monster_obj:GetId()
end

--@gather_time 采集时间(毫秒)
function SceneManager:CreateGather(gatherid, c_scene, posx, posy, gather_time)
	local gather_obj = C_LuaToCFun:CreateGather(gatherid, c_scene, CFactory:CLuaPos(posx, posy), gather_time)
	
	if nil == gather_obj then
		return nil, -1
	end

	return gather_obj, gather_obj:GetId()
end