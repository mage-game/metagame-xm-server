WorldManager = WorldManager or BaseClass(BaseHotUpdate)
function WorldManager:__init()
	if WorldManager.Instance then
		ErrorLog("[WorldManager] Attemp to create a singleton twice !")
	end
	
	WorldManager.Instance = self
end

function WorldManager:__delete()
	WorldManager.Instance = nil
end

function WorldManager:GetCWorld()
	return C_World:GetInstWorld()
end

function WorldManager:MailToUser(role_id, c_mailcontentparam)
	C_MailRoute:MailToUser(C_LuaToCFun:IntToUid_Inline(role_id), 0, c_mailcontentparam)
end

function WorldManager:BroadcastMsg(msg)
	C_LuaToCFun:BroadcastMsg(msg)
end

function WorldManager:WriteLog(log)
	C_LuaToCFun:WriteLog(log)
end

-- 服务器启动完成
function WorldManager:OnServerStart()
	IS_SERVER_STARTING = false
end

function WorldManager:OnWorldUpdate(interval, now_second)
	MemManager.Instance:Update(interval, now_second)
end