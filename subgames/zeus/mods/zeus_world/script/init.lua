runfile(_PATH.."register_biomes")

local dim = zepha.create_dimension("zeus:world:default", { biomes = { "#natural" } })
-- zepha.set_default_dimension(dim.identifier)

zepha.create_dimension("zeus:world:endless_desert", { biomes = { "zeus:world:desert" } })
zepha.set_default_dimension("zeus:world:endless_desert")