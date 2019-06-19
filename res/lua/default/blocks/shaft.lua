zeus.register_block('default:shaft', {
    name = "Rotor Shaft",
    model = "default:shaft",
    textures = {"default_shaft"},
    culls = false,
    selection_box = {
        {0, 6/16, 6/16, 1, 10/16, 10/16}
    },
    on_construct = function(pos)
        zeus.delay(function()
            pos.x = pos.x + 1;
            zeus.set_block(pos, "default:shaft")
        end, .2);
    end
})