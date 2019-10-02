local entities = {}

local function collides(entity)
    return zepha.get_block({
        x = math.floor(entity.pos.x),
        y = math.floor(entity.pos.y - 0.5),
        z = math.floor(entity.pos.z)}) ~= "air"
end

zepha.register_block("default:sandstone", {
    name = "Sandstone",
    model = "default:block",
    textures = {"zeus:default:sandstone"},
    toughness = {
        hand = 14,
        pick = 3
    },
})