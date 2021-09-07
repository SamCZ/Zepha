zepha.entities = {}

function zepha.__builtin.update_entities(delta)
    for k, v in pairs(zepha.entities) do
        if type(v.on_update) == 'function' then v:on_update(delta) end
    end
end
