add_includedirs("include")

target("learnOpengl_004_hello_triangle_exercise1")
    add_files("./src/**.cpp")
    set_rundir(os.scriptdir())
    add_packages("glfw", "glad", "opengl", "glm", "imgui")

