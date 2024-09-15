add_includedirs("include")

target("blackhole")
    add_files("./src/**.cpp")
    set_rundir(os.scriptdir())
    add_packages("glfw", "glad", "opengl", "glm", "stb", "imgui")
