health.health_values = {}

if zepha.server then
    zepha.bind('player_join', function(player)
        health.health_values[player.id] = { health = 10, buffer = 0, max = 10 }
        zepha.send_message("@auri:health:damage", health.health_values[player.id])

        zepha.after(function()
            health.player_heal(player, 0.5)
            return true
        end, 5)
    end)
else
    health.my_health = { health = 10, buffer = 0, max = 10 }
    health.internal.update(true)
    health.render_default(true)

    zepha.bind('message', function(channel, message)
        if channel ~= "@auri:health:damage" then return end
        health.my_health = message
        health.internal.update(true)
    end)
end