GmManager = GmManager or BaseClass(BaseHotUpdate)
function GmManager:__init()
	if GmManager.Instance then
		ErrorLog("[GmManager] Attemp to create a singleton twice !")
	end
	
	GmManager.Instance = self
end

function GmManager:__delete()
	GmManager.Instance = nil
end

-- 使用方法如  /gm lua:lingyu_upstar#0#0#0 
function GmManager:OnGm(c_role, cmd)
	if nil == c_role or nil == cmd then
		return
	end

	local role = RoleManager.Instance:GetRoleByCRole(c_role)
	if nil == role then
		return
	end

	local param_t = Split(cmd, "#")
	
	local cmd_fun = param_t[1]
	local param1 = tonumber(param_t[2]) or -1
	local param2 = tonumber(param_t[3]) or -1
	local param3 = tonumber(param_t[4]) or -1
	local param4 = tonumber(param_t[5]) or -1
	local param5 = tonumber(param_t[6]) or -1

	Log(string.format("OnGm:%s, %d, %d, %d ,%d, %d", cmd_fun, param1, param2, param3, param4, param5))

	if "lingyu_upstar" == cmd_fun then
		role:GetLingyu():OnUpStar(param1, param2)
	end

	if "roledb" == cmd_fun then
		DBManager.Instance:GetRoleDB():PrintDBInfo()
	end

	if "worlddb" == cmd_fun then
		DBManager.Instance:GetWorldDB():PrintDBInfo()
	end

	if "enterfb" == cmd_fun then
		FBManager.Instance:OnEnterFB(role:GetRoleId(), param1, param2, param3, param4)
	end

	if "fbreqnext" == cmd_fun then
		SpecialLogicManager.Instance:SpecialReqNextLevel(role:GetRoleId())
	end

	if "challenge_reset" == cmd_fun then
		role:GetChallengeFb():ResetLevel(param1)
	end

	if "challenge_auto" == cmd_fun then
		role:GetChallengeFb():AutoFBReq(param1, param2)
	end

	if "challenge_buy" == cmd_fun then
		role:GetChallengeFb():BuyJoinTimes()
	end

	if "collect" == cmd_fun then
		MemManager.Instance:Collect()
	end

	if "open_mem" == cmd_fun then
		MemManager.Instance:OpenMonitor()
	end

	if "close_mem" == cmd_fun then
		MemManager.Instance:CloseMonitor()
	end
end