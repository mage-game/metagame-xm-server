ActivityManager = ActivityManager or BaseClass(BaseHotUpdate)
function ActivityManager:__init()
	if ActivityManager.Instance then
		ErrorLog("[ActivityManager] Attemp to create a singleton twice !")
	end
	
	ActivityManager.Instance = self
end

function ActivityManager:__delete()
	ActivityManager.Instance = nil
end

function ActivityManager:CreateObjects()
	self:AddObject("activity_" .. ACTIVITY_TYPE.SHUIJING, ShuijingAct.New(ACTIVITY_TYPE.SHUIJING))  -- 水晶活动
end

function ActivityManager:GetActivity(activity_type)
	return self:GetObject("activity_" .. activity_type)
end

function ActivityManager:IsLuaActivityType(activity_type)
	return nil ~= self:GetActivity(activity_type)
end

function ActivityManager:Update(interval, now_second)
	for i, v in ipairs(self.object_queue) do
		v:Update(interval, now_second)
	end
end

function ActivityManager:IsActivityOpen(activity_type)
	local activity = self:GetActivity(activity_type)
	if nil ~= activity then
		return ACTIVITY_STATUS.OPEN == activity:GetActivityStatus()
	end

	return false
end

function ActivityManager:IsActivityClose(activity_type)
	local activity = self:GetActivity(activity_type)
	if nil ~= activity then
		return ACTIVITY_STATUS.CLOSE == activity:GetActivityStatus()
	end

	return true
end

function ActivityManager:OnUserLogin(c_role)
	for i, v in ipairs(self.object_queue) do
		v:OnUserLogin(c_role)
	end
end

function ActivityManager:OnUserLogout(c_role)
	for i, v in ipairs(self.object_queue) do
		v:OnUserLogout(c_role)
	end
end

function ActivityManager:OnUserEnterScene(activity_type, c_role)
	for i, v in ipairs(self.object_queue) do
		v:OnUserEnterScene(c_role)
	end
end

function ActivityManager:OnUserLeaveScene(activity_type, c_role)
	for i, v in ipairs(self.object_queue) do
		v:OnUserLeaveScene(c_role)
	end
end

function ActivityManager:OnQueryRoomStatusReq(activity_type, c_role)
	local activity = self:GetActivity(activity_type)
	if nil ~= activity then
		activity:OnQueryRoomStatusReq(c_role)
	end
end

function ActivityManager:OnForceToNextState(activity_type)
	local activity = self:GetActivity(activity_type)
	if nil ~= activity then
		activity:OnForceToNextState()
	end
end

function ActivityManager:OnActivityStatusChange(activity_type, from_status, to_status)
	local activity = self:GetActivity(activity_type)
	if nil ~= activity then
		activity:OnActivityStatusChange(from_status, to_status)
	end
end

function ActivityManager:OnActivityEnterReq(activity_type, room_index, role_id)
	local activity = self:GetActivity(activity_type)
	local role = RoleManager.Instance:GetRoleById(role_id)
	
	if nil == activity or nil == role then
		return
	end

	local limit_level = JoinLimitManager.Instance:GetActivityLimitLevel(activity_type)
	if role:GetLevel() < limit_level then
		role:NoticeNumStr(LangTip.role_level_limit)
		return
	end

	if activity:EnterMustInActOpening() and not self:IsActivityOpen(activity_type) then
		role:NoticeNumStr(LangTip.activity_not_open)
		return
	end

	local scene_id = activity:GetSceneId()
	local pos_x, pos_y = activity:GetEnterPos()
	local scene_key = self:GetKeyByRoomIndex(room_index)

	Log(string.format("enter activity scene %d %d %d %d", scene_id, scene_key, pos_x, pos_y))
	local ret = SceneManager.Instance:GoTo(role, scene_id, scene_key, pos_x, pos_y)
	if not ret then
		role:NoticeNumStr(LangTip.enter_act_error)
		return
	end

	local log = string.format("enter_act_scene %d %d %d", role_id, activity_type, room_index)
	WorldManager.Instance:WriteLog(log)
end

function ActivityManager:GetKeyByRoomIndex(room_index)
	return room_index + 1
end