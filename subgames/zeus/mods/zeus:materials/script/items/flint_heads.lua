zepha.register_item("zeus:materials:flint_axe_head", {
    name = "Flint Hatchet Head",
    stack = 1,
    textures = {
        "zeus:materials:flint_axe_head",
    }
})

crafting.register_recipe({
    output = "zeus:materials:flint_axe_head",
    recipe = {
        {"zeus:materials:flint", "zeus:materials:flint"},
        {"zeus:materials:flint", ""},
    }
})

zepha.register_item("zeus:materials:flint_blade_head", {
    name = "Flint Blade Head",
    stack = 1,
    textures = {
        "zeus:materials:flint_blade_head",
    }
})

crafting.register_recipe({
    output = "zeus:materials:flint_blade_head",
    recipe = {
        {"zeus:materials:flint"},
        {"zeus:materials:flint"},
    }
})

zepha.register_item("zeus:materials:flint_pick_head", {
    name = "Flint Pick Head",
    stack = 1,
    textures = {
        "zeus:materials:flint_pick_head",
    }
})

crafting.register_recipe({
    output = "zeus:materials:flint_pick_head",
    recipe = {
        {"zeus:materials:flint", "zeus:materials:flint"},
    }
})

zepha.register_item("zeus:materials:flint_shovel_head", {
    name = "Flint Spade Head",
    stack = 1,
    textures = {
        "zeus:materials:flint_shovel_head",
    }
})

crafting.register_recipe({
    output = "zeus:materials:flint_shovel_head",
    recipe = {
        {"zeus:materials:flint"},
    }
})
