zeus.register_block('kinetic:axle_0', {
    name = "Axle",
    model = "kinetic:axle_0",
    textures = {"kinetic_axle"},
    culls = false,
    selection_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    on_construct = function(pos)
        zeus.delay(function()
            zeus.set_block(pos, "kinetic:axle_1")
        end, 4)
    end
})

zeus.register_block('kinetic:axle_1', {
    name = "Axle",
    model = "kinetic:axle_1",
    textures = {"kinetic_axle"},
    culls = false,
    selection_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    on_construct = function(pos)
        zeus.delay(function()
            zeus.set_block(pos, "kinetic:axle_2")
        end, 4)
    end
})

zeus.register_block('kinetic:axle_2', {
    name = "Axle",
    model = "kinetic:axle_2",
    textures = {"kinetic_axle"},
    culls = false,
    selection_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    on_construct = function(pos)
        zeus.delay(function()
            zeus.set_block(pos, "kinetic:axle_3")
        end, 4)
    end
})

zeus.register_block('kinetic:axle_3', {
    name = "Axle",
    model = "kinetic:axle_3",
    textures = {"kinetic_axle"},
    culls = false,
    selection_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    on_construct = function(pos)
        zeus.delay(function()
            zeus.set_block(pos, "kinetic:axle_0")
        end, 4)
    end
})