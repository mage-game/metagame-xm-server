FBManager = FBManager or BaseClass(BaseHotUpdate)
function FBManager:__init()
	if FBManager.Instance then
		ErrorLog("[FBManager] Attemp to create a singleton twice !")
	end
	
	FBManager.Instance = self

	self.handle_t = {}
	self:RegisterEnterHandle()
end

function FBManager:__delete()
	FBManager.Instance = nil
end

function FBManager:OnEnterFB(role_id, fb_type, param1, param2, param3)
	local handle_fun = self.handle_t[fb_type]
	if nil == handle_fun then
		return false
	end

	local role = RoleManager.Instance:GetRoleById(role_id)
	if nil == role then
		return false
	end

	handle_fun(role, param1, param2, param3)

	return true
end

function FBManager:RegisterEnterHandle()
	self.handle_t[FB_TYPE.TIANXIANGE] = BindTool.Bind1(self.EnterTianxiange, self)
	self.handle_t[FB_TYPE.CHALLENGE] = BindTool.Bind1(self.EnterChallenge, self)
end

function FBManager:EnterTianxiange(role, param1, param2, param3)
	local pass_level = role:GetCommonData():GetTianxiangeLevel()
	local level_cfg = TianxiangeFbCfg.Instance:GetLevelCfg(pass_level + 1)
	if nil == level_cfg then
		role:NoticeNumStr(LangTip.enter_fb_error)
		return
	end

	local ret = SceneManager.Instance:GoTo(role, level_cfg.scene_id, role:GetRoleId(), level_cfg.pos_x, level_cfg.pos_y)
	
	if not ret then
		role:NoticeNumStr(LangTip.enter_fb_error)
		return
	end

	local log = string.format("enter_tianxiange %d %d %d %d", pass_level + 1, param1, param2, param3)
	role:WriteLog(log) 
end

function FBManager:EnterChallenge(role, param1, param2, param3)
	local level = param1
	local is_autobuy = param2

	local chapter_cfg = ChallengeFbCfg.Instance:GetChappterCfg(level)
	if nil == chapter_cfg then
		return
	end

	local can_enter = role:GetChallengeFb():CanEnter(level, is_autobuy == 1)
	if not can_enter then
		return
	end

	local fight_layer = role:GetChallengeFb():GetFightLayer(level)
	local layer_cfg = ChallengeFbCfg.Instance:GetLayerCfg(level, fight_layer)
	if nil == layer_cfg then
		return
	end

	role:GetChallengeFb():OnEnterFB(level)

	local ret = SceneManager.Instance:GoTo(role, layer_cfg.scene_id, role:GetRoleId(), chapter_cfg.pos_x, chapter_cfg.pos_y)
	if not ret then
		role:NoticeNumStr(LangTip.enter_fb_error)
		return
	end


	local log = string.format("enter_challenge %d %d %d", level, fight_layer, is_autobuy)
	role:WriteLog(log) 
end