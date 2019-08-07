print("Hello from default, Zeus " .. (zeus.is_server() and "Server" or "Client") .. "!")

dofile("zeus:default/models")
dofile("zeus:default/blocks")