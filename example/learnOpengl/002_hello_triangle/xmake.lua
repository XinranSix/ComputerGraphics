add_includedirs("include")

target("learnOpengl_002_hello_triangle")
    add_files("./src/**.cpp")
    set_rundir(os.scriptdir())
    add_packages("glfw", "glad", "opengl", "glm", "imgui")

