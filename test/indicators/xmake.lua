for _, filepath in ipairs(os.files("./src/**.cpp")) do
target('indicators_' .. path.basename(filepath))
    add_files(filepath)
    add_packages("indicators")
end
