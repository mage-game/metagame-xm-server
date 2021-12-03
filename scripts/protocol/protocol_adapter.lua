ProtocolAdapter = ProtocolAdapter or {}

function ProtocolAdapter:AdaptSCVisibleObjEnterRole(role_id, c_datastream)
	local role = RoleManager.Instance:GetRoleById(role_id)
	if nil == c_datastream or nil == role then
		return
	end

	c_datastream:WriteShort(role:GetCommonData():GetTianxiangeLevel())
	c_datastream:WriteShort(0)
end