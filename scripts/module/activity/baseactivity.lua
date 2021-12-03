BaseActivity = BaseActivity or BaseClass(BaseHotUpdate)

function BaseActivity:__init(activity_type)
	self.dataset.c_activity = CFactory:CActivity(activity_type)
end

function BaseActivity:__delete()
end

function BaseActivity:Update(interval, now_second)
	self.dataset.c_activity:Update(interval, now_second)
end

function BaseActivity:GetActivityStatus()
	return self.dataset.c_activity:GetActivityStatus()
end

function BaseActivity:OnUserLogin(c_role)
	self.dataset.c_activity:OnUserLogin(c_role)
end

function BaseActivity:OnUserLogout(c_role)
	self.dataset.c_activity:OnUserLogout(c_role)
end

function BaseActivity:OnUserEnterScene(c_role)
	self.dataset.c_activity:OnUserEnterScene(c_role)
end

function BaseActivity:OnUserLeaveScene(c_role)
	self.dataset.c_activity:OnUserLeaveScene(c_role)
end

function BaseActivity:OnQueryRoomStatusReq(c_role)
	self.dataset.c_activity:OnQueryRoomStatusReq(c_role)
end

function BaseActivity:OnForceToNextState()
	self.dataset.c_activity:ForceToNextState()
end

function BaseActivity:OnActivityStatusChange(from_status, to_status)
	self.dataset.c_activity:OnActivityStatusChange(from_status, to_status)
end

function BaseActivity:GetSceneId()
	return 0
end

function BaseActivity:GetEnterPos()
	return 0, 0
end

function BaseActivity:EnterMustInActOpening()
	return true
end