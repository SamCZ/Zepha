
-- zepha.block_interact_or_place(player: Player , target: Target [, stack: ItemStack]): ItemStack, Vector | nil
-- Calls zepha.block_interact if the targeted block can be interacted with, returns the passed in stack or wield stack and nil.
-- Otherwise, calls zepha.block_place and returned the function's returned values.

function zepha.block_interact_or_place(player, target, stack)
    local stack = stack or player:get_wield_stack()
    local target_block = target.dim:get_block(target.pos)
    local target_def = zepha.registered_blocks[target_block]
    local stack_def = zepha.registered_blocks[stack and stack.name or nil]

    if target_def and (target_def.on_interact or target_def.on_interact_client) then
        -- Trigger an interaction if the targeted block has one.
        zepha.block_interact(player, target)
        return stack, nil
    end

    -- Return if stack isn't a block.
    if stack == nil or stack_def == nil then return stack, nil end

    -- Place a block using the passed in stack.
    return zepha.block_place(player, target, stack)
end


-- zepha.block_interact(player: Player, target: Target): nil
-- Interacts with the targeted block. Returns true if the block has an on_interact or
-- on_interact_client callback, returns false otherwise.

function zepha.block_interact(player, target)
    local block = target.dim:get_block(target.pos)
    local def = zepha.registered_blocks[block]

    local args = { target.dim, target.pos, player }
    local cb = zepha.server and 'on_interact' or 'on_interact_client'
    if type(def[cb]) == 'function' then def[cb](table.unpack(args)) end
    zepha.trigger(cb, table.unpack(args))

    return def.on_interact or def.on_interact_client
end


-- zepha.block_place(player: Player, target: Target [, stack: ItemStack]): ItemStack, Vector | nil
-- Attempts to place `stack` or the wield stack in the world, on success returning the
-- wield stack with one count removed, and the placed position, or on failure returning the
-- original stack and nil.

function zepha.block_place(player, target, stack)
    local stack = stack or player:get_wield_stack()
    local stack_def = zepha.registered_blocks[stack and stack.name or nil]
    if stack == nil or stack_def == nil then return stack, nil end

    target.dim:set_block(target.pos_above, stack.name)

    stack.count = stack.count - 1
    return stack, target.pos_above
end


-- zepha.block_hit(player: Player, target: Target): float, float
-- Hits the block at `target` using the player's wielded item.
-- Returns the damage done followed by the timeout before the next hit can begin.

function zepha.block_hit(player, target)
    local block = player.dim:get_block(target.pos)
    local def = zepha.registered_blocks[block]

    -- Don't do anything, return a small timeout to avoid spamming the function.
    if not def then return 0, 0.1 end

    local damage, timeout = zepha.get_hit_impact(player, block)
    local total_damage = target.dim:add_block_damage(target.pos, damage)

    local health = (def.tool_props or {}).health == nil and -1 or (def.tool_props or {}).health
    local broken = health ~= -1 and total_damage > health

    if broken then zepha.block_break(player, target) end

    return damage, timeout
end


-- zepha.block_break(player: Player, target: Target):
-- Breaks the block at `target`.

function zepha.block_break(player, target)
    local block = player.dim:get_block(target.pos)
    local def = zepha.registered_blocks[block] or {}

    local args = { target.dim, target.pos, player }

    local cb = zepha.server and 'on_break' or 'on_break_client'
    if type(def[cb]) == 'function' then def[cb](table.unpack(args)) end
    zepha.trigger(cb, table.unpack(args))

    target.dim:set_block(target.pos, 'air')

    local cb = zepha.server and 'after_break' or 'after_break_client'
    if type(def[cb]) == 'function' then def[cb](table.unpack(args)) end
    zepha.trigger(cb, table.unpack(args))
end


-- zepha.item_use(player: Player, target: Target [, stack: ItemStack]): ItemStack
-- Attempts to use `stack` or the wield stack in,
-- returning a modified wieldstack or nil if the stack is unchanged.

function zepha.item_use(player, target, stack)
    local stack = stack or player:get_wield_stack()
    local def = zepha.registered_items[stack and stack.name or nil]

    if stack == nil or def == nil then return stack end

    local args = { stack, target, player }
    local cb = zepha.server and 'on_use' or 'on_use_client'

    if type(def[cb]) == 'function' then return def[cb](table.unpack(args)) end
    return nil
end