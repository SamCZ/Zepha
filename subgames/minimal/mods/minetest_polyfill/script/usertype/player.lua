local MTObjectRef = runfile(_PATH .. 'object')

local MTPlayerRef = {
	-- TODO: Implement
	get_player_name = function(self)
		return " "
	end,

	is_player = function()
		return true
	end,

	get_player_velocity = function(self)
		return self.player.get_vel()
	end,
	set_player_velocity = function(self, vel)
		return self.player.set_vel(vel)
	end,
	add_player_velocity = function(self, vel)
		return self.player.set_vel(self.get_vel() + vel)
	end,

	-- TODO: Implement
	get_look_dir = function()
		return 0
	end,
	get_look_vertical = function(self)
		return self.player.get_look_pitch()
	end,
	get_look_horizontal = function(self)
		return self.player.get_look_yaw()
	end,
	set_look_vertical = function(self, pitch)
		return self.player.set_look_pitch(pitch)
	end,
	set_look_horizontal = function(self, yaw)
		return self.player.set_look_yaw(yaw)
	end,

	-- TODO: Implement
	get_breath = function()
		return 0
	end,
	-- TODO: Implement
	set_breath = function()
	
	end,

	-- TODO: Implement
	get_fov = function()
		return 0
	end,
	-- TODO: Implement
	set_fov = function()
	
	end,

	-- TODO: Implement
	get_meta = function()
		return nil
	end,

	get_inventory = function(self)
		return self.object.get_inventory()
	end,
	get_wield_list = function(self)
		return self.object:get_wield_list()
	end,
	get_wield_index = function(self)
		return self.object:get_wield_index()
	end,
	get_wielded_item = function(self)
		return minetest.__usertype.ItemStack(self.object:get_wield_stack())
	end,
	set_wielded_item = function(self, item)
		return self.object:get_wield_list():
			set_stack(self.object:get_wield_index(), item)
	end,

	-- TODO: Implement
	set_inventory_formspec = function()
		
	end,
	-- TODO: Implement
	set_formspec_prepent = function()
		
	end,
	-- TODO: Implement
	get_formspec_prepent = function()
		return ""	
	end,

	-- TODO: Implement
	get_player_control = function()
		return {}
	end,
	-- TODO: Implement
	get_player_control_bits = function()
		return 0
	end,

	-- TODO: Implement
	set_physics_override = function()
		
	end,
	-- TODO: Implement
	get_physics_override = function()
		return {}
	end,

	-- TODO: Implement
	hud_add = function()

	end,
	-- TODO: Implement
	hud_remove = function()

	end,
	-- TODO: Implement
	hud_change = function()

	end,
	-- TODO: Implement
	hud_get = function()

	end,
	-- TODO: Implement
	hud_set_flags = function()

	end,
	-- TODO: Implement
	hud_get_flags = function()

	end,
	-- TODO: Implement
	hud_set_hotbar_itemcount = function()

	end,
	-- TODO: Implement
	hud_set_hotbar_image = function()

	end,
	-- TODO: Implement
	hud_set_hotbar_selected_image = function()

	end,
	-- TODO: Implement
	set_minimap_modes = function()

	end,

	-- TODO: Implement
	set_sky = function()

	end,
	-- TODO: Implement
	get_sky = function()

	end,
	-- TODO: Implement
	get_sky_color = function()

	end,
	-- TODO: Implement
	set_sun = function()

	end,
	-- TODO: Implement
	get_sun = function()

	end,
	-- TODO: Implement
	set_moon = function()

	end,
	-- TODO: Implement
	get_moon = function()

	end,
	-- TODO: Implement
	set_stars = function()

	end,
	-- TODO: Implement
	get_stars = function()

	end,
	-- TODO: Implement
	get_clouds = function()

	end,
	-- TODO: Implement
	set_clouds = function()

	end,
	-- TODO: Implement
	override_day_night_ratio = function()

	end,
	-- TODO: Implement
	get_day_night_ratio = function()

	end,

	-- TODO: Implement
	set_local_animation = function()

	end,
	-- TODO: Implement
	get_local_animation = function()

	end,

	-- TODO: Implement
	set_eye_offset = function()

	end,
	-- TODO: Implement
	get_eye_offset = function()

	end,

	-- TODO: Implement
	send_mapblock = function()

	end
}

setmetatable(MTPlayerRef, { __index = MTObjectRef })

local mt = { __index = MTPlayerRef }

minetest.__usertype.PlayerRef = function(raw)
	local player = { player = raw, object = raw }
	setmetatable(player, mt)
	return player
end

return MTPlayerRef
