local MTItemStack = {
	is_empty = function(self)
		return self.stack.count == 0
	end,

	get_name = function(self)
		return minetest.__util.identifier.encode(self.stack.name)
	end,
	set_name = function(self, name)
		self.stack.name = minetest.__util.identifier.decode(name)
	end,

	get_count = function(self)
		return self.stack.count
	end,
	set_count = function(self, count)
		self.stack.count = count
	end,

	-- TODO: Implement
	get_wear = function(self)
		return 0
	end,
	-- TODO: Implement
	set_wear = function(self)

	end,
	-- TODO: Implement
	add_wear = function(self)

	end,

	-- TODO: Implement
	get_meta = function(self)
		return {}
	end,

	get_description = function(self)
		return (zepha.registered_blocks(self.stack.name) or 
			zepha.registered_items(self.stack.name) or {}).name
	end,
	get_short_description = function(self)
		return (zepha.registered_blocks(self.stack.name) or 
			zepha.registered_items(self.stack.name) or {}).name
	end,

	clear = function(self)
		self.stack.count = 0
	end,
	replace = function(self, name)
		self:set_name(name)
	end,

	to_string = function(self)
		return self.stack.name .. ' ' .. tostring(self.stack.count)
	end,
	to_table = function(self)
		return { self.stack.name, self.stack.count }
	end,

	get_stack_max = function(self)
		return (zepha.registered_blocks(self.stack.name) or 
			zepha.registered_items(self.stack.name) or {}).stack or 64
	end,
	get_free_space = function(self)
		return self.get_max_stack() - self.get_count()
	end,

	is_known = function(self)
		return (zepha.registered_blocks(self.stack.name) or 
			zepha.registered_items(self.stack.name)) ~= nil
	end,
	get_definition = function(self)
		return zepha.registered_blocks(self.stack.name) or 
			zepha.registered_items(self.stack.name)
	end,

	-- TODO: Implement
	get_tool_capabilities = function()
		return {}
	end,

	-- TODO: Implement
	add_item = function()

	end,
	-- TODO: Implement
	item_fits = function()

	end,
	-- TODO: Implement
	take_item = function()

	end,
	-- TODO: Implement
	peek_item = function()

	end
}

local mt = { __index = MTItemStack }

minetest.__usertype.ItemStack = function(raw)
	local stack = { stack = raw }
	setmetatable(stack, mt)
	return stack
end

return MTItemStack
