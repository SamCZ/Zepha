zepha.register_item("@auri:basic_tools:flint_pickaxe", {
    name = "Flint Pick",
    textures = {
        "@auri:basic_tools:flint_pickaxe"
    },
    tool_props = {
        interval = 0.4,
        damage_groups = {
            smash = 3,
            crack = 7
        }
    },
    on_hit = function(dim, pos) if dim:get_block(pos) == 'zeus:default:grass' then dim:set_block(pos, 'zeus:default:dirt') end end,
    on_hit_client = function(dim, pos) if dim:get_block(pos) == 'zeus:default:grass' then dim:set_block(pos, 'zeus:default:dirt') end end
})

crafting.register_recipe({
    output = "@auri:basic_tools:flint_pickaxe",
    recipe = {
        {"zeus:materials:flint_pickaxe_head", "zeus:materials:plant_twine"},
        {"zeus:materials:stick", ""}
    }
})

zepha.register_item("@auri:basic_tools:flint_hatchet", {
    name = "Flint Hatchet",
    textures = {
        "@auri:basic_tools:flint_hatchet"
    }
})

crafting.register_recipe({
    output = "@auri:basic_tools:flint_hatchet",
    recipe = {
        {"zeus:materials:flint_axe_head", "zeus:materials:plant_twine"},
        {"zeus:materials:stick", ""}
    }
})

zepha.register_item("@auri:basic_tools:flint_shovel", {
    name = "Flint Spade",
    textures = {
        "@auri:basic_tools:flint_shovel"
    },
    tool_props = {
         interval = 0.4,
         damage_groups = {
             scoop = 7
         }
    }
});

crafting.register_recipe({
    output = "@auri:basic_tools:flint_shovel",
    recipe = {
        {"zeus:materials:flint_shovel_head", "zeus:materials:plant_twine"},
        {"zeus:materials:stick", ""}
    }
})

zepha.register_item("@auri:basic_tools:wooden_pickaxe", {
    name = "Wooden Pick",
    textures = {
        "@auri:basic_tools:wooden_pickaxe"
    }
});

zepha.register_item("@auri:basic_tools:wooden_hatchet", {
    name = "Wooden Hatchet",
    textures = {
        "@auri:basic_tools:wooden_hatchet"
    }
});

zepha.register_item("@auri:basic_tools:wooden_shovel", {
    name = "Wooden Shovel",
    textures = {
        "@auri:basic_tools:wooden_shovel"
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
        inv:add_stack({"@auri:basic_tools:flint_pickaxe", 1})
--         inv:add_stack({"@auri:basic_tools:wooden_hatchet", 1})
--         inv:add_stack({"@auri:basic_tools:wooden_shovel", 1})
        inv:add_stack({"@auri:basic_tools:flint_shovel", 1})
    end)
end