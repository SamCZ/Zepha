print("----\nPrinting the Zeus Global table's contents:\n----")

for function_name, _ in pairs(zeus) do
    print(function_name);
end

print("----")

print(zeus.average(1, 2, 3, 4, 5));