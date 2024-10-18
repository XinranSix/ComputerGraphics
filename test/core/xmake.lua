add_includedirs("../../core/include")

for _, filepath in ipairs(os.files("./src/**.cpp")) do
target('core_' .. path.basename(filepath))
    add_files(filepath)
    add_deps("core")
    add_packages("indicators", "libsdl", "glad")
end
