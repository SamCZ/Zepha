local MTObjectRef = {
	get_pos = function(self)
		return self.object.pos
	end,
	set_pos = function(self, pos)
		self.object.pos = pos
	end,
	set_velocity = function(self)
		return self.object.vel
	end,
	set_velocity = function(self, vel)
		self.object.vel = vel
	end,
	add_velocity = function(self, vel)
		self.object.vel = self.object.vel + vel
	end,
	move_to = function(self, pos)
		self.object.pos = pos
	end,

	-- TODO: Implement
	punch = function()

	end, 
	-- TODO: Implement
	right_click = function()

	end,

	-- TODO: Implement
	get_hp = function()
		return 0
	end, 

	-- TODO: Implement
	set_hp = function()

	end,

	-- TODO: Implement
	set_armor_groups = function()

	end,
	-- TODO: Implement
	get_armor_groups = function()
		return {}
	end,

	-- TODO: Implement
	set_animation = function()
		return {}
	end,
	-- TODO: Implement
	get_animation = function()

	end,
	-- TODO: Implement
	set_animation_frame_speed = function()

	end,
	-- TODO: Implement
	set_attach = function()

	end,
	-- TODO: Implement
	get_attach = function()
		return nil
	end,
	-- TODO: Implement
	get_children = function()
		return {}
	end,
	-- TODO: Implement
	set_detach = function()

	end,
	-- TODO: Implement
	set_bone_position = function()

	end,
	-- TODO: Implement
	get_bone_position = function()

	end,
	
	-- TODO: Implement
	set_properties = function()

	end,
	-- TODO: Implement
	get_properties = function()

	end,

	is_player = function()
		return false
	end,

	-- TODO: Implement
	get_nametag_attributes = function()
		return {}
	end,
	-- TODO: Implement
	set_nametag_attributes = function()

	end
}

local mt = { __index = MTObjectRef }

minetest.__usertype.ObjectRef = function(raw)
	local object = { object = raw }
	setmetatable(object, mt)
	return object
end

return MTObjectRef
