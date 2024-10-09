add_includedirs("include")

target("learnOpengl_006_hello_triangle_exercise3")
    add_files("./src/**.cpp")
    set_rundir(os.scriptdir())
    add_packages("glfw", "glad", "opengl", "glm", "imgui")

