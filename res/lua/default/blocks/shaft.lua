zeus.register_block('default:shaft_0', {
    name = "Rotor Shaft",
    model = "default:shaft",
    textures = {"default_shaft"},
    culls = false,
    selection_box = {
        {0, 7/16, 7/16, 1, 9/16, 9/16}
    },
    on_construct = function(pos)
--        zeus.delay(function()
--            zeus.set_block(pos, "default:shaft_1")
--        end, .2);
    end
})

--zeus.register_block('default:shaft_1', {
--    name = "Rotor Shaft",
--    model = "default:shaft_1",
--    textures = {"default_planks"},
--    toughness = {
--        hand = 14,
--        pick = 3
--    },
--    on_construct = function(pos)
--        zeus.delay(function()
--            zeus.set_block(pos, "default:shaft_2")
--        end, .2);
--    end
--})
--
--zeus.register_block('default:shaft_2', {
--    name = "Rotor Shaft",
--    model = "default:shaft_2",
--    textures = {"default_planks"},
--    toughness = {
--        hand = 14,
--        pick = 3
--    },
--    on_construct = function(pos)
--        zeus.delay(function()
--            zeus.set_block(pos, "default:shaft_0")
--        end, .2);
--    end
--})