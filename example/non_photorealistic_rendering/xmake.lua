add_includedirs("include")

target("non_photorealistic_rendering")
    add_files("./src/**.cpp")
    set_rundir(os.scriptdir())
    add_packages("glfw", "glad", "opengl", "glm", "stb", "imgui")
