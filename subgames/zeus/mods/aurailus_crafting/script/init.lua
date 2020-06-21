_G["crafting"] = {}

crafting.registered_recipes = {}

crafting.register_recipe = function(tbl)
    table.insert(crafting.registered_recipes, tbl)
end

crafting.bind = function(craft_input, craft_output)
    local width = craft_input.width
    local length = craft_input.length

    local crafting_changed = function()
        local items = {}

        local matched_any = false
        for _,r in crafting.registered_recipes do
            local matches = true
            for i = 1, length do
                local x = (i - 1) % width + 1
                local y = math.floor((i - 1) / width) + 1

                local recipe_item_name = ""

                if y <= #(r.recipe) and x <= #(r.recipe[y]) then
                    recipe_item_name = r.recipe[y][x]
                end

                if recipe_item_name ~= craft_input:get_stack(i).name then
                    matches = false
                    break
                end
            end

            if matches then
                craft_output:set_stack(1, {r.output, 1})
                matched_any = true
                break
            end
        end

        if not matched_any then
            craft_output:set_stack(1, {"invalid", 0})
        end
    end

    craft_input.on_put = crafting_changed
    craft_input.on_take = crafting_changed

    craft_output.allow_put = function()
        return 0
    end

    craft_output.on_take = function()
        for i = 1, length do
            craft_input:remove_stack(i, 1)
        end
        crafting_changed()
    end
end