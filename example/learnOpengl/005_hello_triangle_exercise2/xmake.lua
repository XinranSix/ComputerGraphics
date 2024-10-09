add_includedirs("include")

target("learnOpengl_005_hello_triangle_exercise2")
    add_files("./src/**.cpp")
    set_rundir(os.scriptdir())
    add_packages("glfw", "glad", "opengl", "glm", "imgui")

