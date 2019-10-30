
-- Register base models
runfile(_PATH .. "models/_index")

runfile(_PATH .. "math")
runfile(_PATH .. "vector")

-- Signal completion
print("Base definitions loaded.")