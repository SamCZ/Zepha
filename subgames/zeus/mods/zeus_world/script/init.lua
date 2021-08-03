runfile(_PATH .. 'keys')
runfile(_PATH .. 'register_biomes')

zepha.create_dimension('zeus:world:default', {
    biomes = { '#natural', '#default' }
})

-- zepha.create_dimension('zeus:world:endless_desert', {
--     biomes = { 'zeus:world:desert' }
-- })

zepha.set_default_dimension('zeus:world:default')