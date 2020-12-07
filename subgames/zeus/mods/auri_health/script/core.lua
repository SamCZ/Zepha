health.health_values = {}

if zepha.server then
    zepha.bind('player_join', function(player)
        health.health_values[player.id] = { health = 20, buffer = 0 }
    end)
else
    zepha.bind('message', function(channel, message)
        dump(message)
    end)
end