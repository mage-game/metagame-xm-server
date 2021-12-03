ShuijingAct = ShuijingAct or BaseClass(BaseActivity)

function ShuijingAct:__init()
end

function ShuijingAct:__delete()
	self.c_speciallogic = nil
end

function ShuijingAct:GetSceneId()
	local other_cfg = ShuijingActCfg.Instance:GetOtherCfg()
	return other_cfg.scene_id
end

function ShuijingAct:GetEnterPos()
	local pos_x, pos_y = ShuijingActCfg.Instance:GetRelivePoint()
	return pos_x, pos_y
end

function ShuijingAct:EnterMustInActOpening()
	return false
end