zeus.register_block('default:sand', {
    name = "Sand",
    model = "default:block",
    textures = {"default_sand"},
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    }
})

zeus.register_block('default:sandstone', {
    name = "Sandstone",
    model = "default:block",
    textures = {"default_sandstone"},
    toughness = {
        hand = 14,
        pick = 3
    },
    on_construct = function(pos)
        zeus.delay(function()
            local max = 10
            local count = 0
            local p = {x = pos.x, y = pos.y, z = pos.z}
            while zeus.get_block(p) == "default:sand" and count < max do
                p.y = p.y + 1
                count = count + 1
            end
            zeus.set_block(p, "default:sand")
            return count < max;
        end, .2);
    end
})