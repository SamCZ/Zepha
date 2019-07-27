print("Hello from default, Zeus " .. (zeus.is_server() and "Server" or "Client") .. "!")

local path = zeus.get_path("default")

dofile(path .. "models.lua")
dofile(path .. "blocks.lua")