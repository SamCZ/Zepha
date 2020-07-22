function zepha.get_tool_props(player)
    local props = {
        interval = 0,
        damage_groups = {}
    }

    if player:get_wield_list() then
        local wield_item = player:get_wield_stack()
        local wield_props = (zepha.registered_items[wield_item.name] or {}).tool_props or {}

        props.interval = wield_props.interval or -1
        for group, damage in pairs(wield_props.damage_groups or {}) do props.damage_groups[group] = damage end
    end

    if props.damage_groups._other == nil and player:get_hand_list() then
        local hand_item = player:get_hand_stack()
        local hand_props = (zepha.registered_items[hand_item.name] or {}).tool_props or {}

        if props.interval == -1 then props.interval = hand_props.interval or -1 end

        for group, damage in pairs(hand_props.damage_groups or {}) do
            if props.damage_groups[group] == nil then props.damage_groups[group] = damage end
        end
    end

    if props.interval == -1 then props.interval = 1 end

    return props
end

function zepha.get_hit_impact(player, block)
    local props = zepha.get_tool_props(player)
    local def = (zepha.registered_blocks[block] or {}).tool_props or {}

    local damage = 0
    for group, tool_damage in pairs(props.damage_groups) do
        local mul = def.multipliers[group]
        if mul == nil then mul = def.multipliers["_other"] end
        if mul == nil then mul = 1 end


        damage = math.max(damage, tool_damage * mul)
    end

    damage = math.max(0, damage - (def.defense or 0))
    return damage, props.interval
end