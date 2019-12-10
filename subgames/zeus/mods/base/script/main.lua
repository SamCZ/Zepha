-- Register base models
runfile(_PATH .. "models/_index")

-- Load Libraries
runfile(_PATH .. "dump")
runfile(_PATH .. "math")
runfile(_PATH .. "vector")

-- Signal completion
print("Base definitions loaded.")