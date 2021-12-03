WordCommonData = WordCommonData or BaseClass(BaseHotUpdate)

function WordCommonData:__init()
	WordCommonData.Instance = self
end

function WordCommonData:__delete()
	WordCommonData.Instance = nil
end

function WordCommonData:Serialize(c_worlddb)

end

function WordCommonData:UnSerialize(c_worlddb)

end
