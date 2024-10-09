add_includedirs("include")

target("learnOpengl_001_hello_window_clear")
    add_files("./src/**.cpp")
    set_rundir(os.scriptdir())
    add_packages("glfw", "glad", "opengl", "glm", "imgui")

