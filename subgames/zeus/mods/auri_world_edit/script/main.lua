local box = nil
local pos1 = nil
local pos2 = nil

zepha.register_entity('@auri:world_edit:box', {
	display = 'wireframe',
	display_stroke = '#0e7',

	update_pos = function(self)
		if pos2 == nil then
			self.object.pos = pos1
			self.object.scale = 1
		else
			local min = V(math.min(pos1.x, pos2.x), math.min(pos1.y, pos2.y), math.min(pos1.z, pos2.z))
			local max = V(math.max(pos1.x, pos2.x), math.max(pos1.y, pos2.y), math.max(pos1.z, pos2.z))
			local scale = max - min + 1

			self.object.pos = min
			self.object.scale = scale
		end
	end
})

zepha.register_item(':wand', {
	name = 'World Edit Wand',
	textures = { '@auri:world_edit:wand' },
	stack = 1,

	on_use_client = function(stack, target, player)
		if box == nil then
			box = zepha.player.dim:add_entity(V(), '@auri:world_edit:box')
		end

		if pos1 == nil or pos2 ~= nil then
			pos2 = nil
			pos1 = V(target.pos or player.pos):floor()
		else
			pos2 = V(target.pos or player.pos):floor()
		end

	    box:update_pos()
	end
})

zepha.bind('new_player', function(player)
	local inv = player:get_inventory():get_list('hot_wheel_1')
	inv:add_stack({ '@auri:world_edit:wand', 1 })
end)
