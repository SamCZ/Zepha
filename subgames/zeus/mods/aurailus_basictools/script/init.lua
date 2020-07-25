zepha.register_item("@aurailus:basictools:flint_pickaxe", {
    name = "Flint Pick",
    textures = {
        "@aurailus:basictools:flint_pickaxe"
    }
})

crafting.register_recipe({
    output = "@aurailus:basictools:flint_pickaxe",
    recipe = {
        {"zeus:materials:flint_pickaxe_head", "zeus:materials:plant_twine"},
        {"zeus:materials:stick", ""}
    }
})

zepha.register_item("@aurailus:basictools:flint_hatchet", {
    name = "Flint Hatchet",
    textures = {
        "@aurailus:basictools:flint_hatchet"
    }
})

crafting.register_recipe({
    output = "@aurailus:basictools:flint_hatchet",
    recipe = {
        {"zeus:materials:flint_axe_head", "zeus:materials:plant_twine"},
        {"zeus:materials:stick", ""}
    }
})

zepha.register_item("@aurailus:basictools:flint_shovel", {
    name = "Flint Spade",
    textures = {
        "@aurailus:basictools:flint_shovel"
    },
    tool_props = {
         interval = 0.4,
         damage_groups = {
             scoop = 7
         }
    }
});

crafting.register_recipe({
    output = "@aurailus:basictools:flint_shovel",
    recipe = {
        {"zeus:materials:flint_shovel_head", "zeus:materials:plant_twine"},
        {"zeus:materials:stick", ""}
    }
})

zepha.register_item("@aurailus:basictools:wooden_pickaxe", {
    name = "Wooden Pick",
    textures = {
        "@aurailus:basictools:wooden_pickaxe"
    }
});

zepha.register_item("@aurailus:basictools:wooden_hatchet", {
    name = "Wooden Hatchet",
    textures = {
        "@aurailus:basictools:wooden_hatchet"
    }
});

zepha.register_item("@aurailus:basictools:wooden_shovel", {
    name = "Wooden Shovel",
    textures = {
        "@aurailus:basictools:wooden_shovel"
    },
    tool_props = {
        interval = 0.45,
        damage_groups = {
            scoop = 5
        }
    }
});

if zepha.server then
    zepha.bind("new_player", function(player)
        local inv = player:get_inventory():get_list("hot_wheel_1");
--         inv:add_stack({"@aurailus:basictools:wooden_pickaxe", 1})
--         inv:add_stack({"@aurailus:basictools:wooden_hatchet", 1})
        inv:add_stack({"@aurailus:basictools:wooden_shovel", 1})
        inv:add_stack({"@aurailus:basictools:flint_shovel", 1})
    end)
end