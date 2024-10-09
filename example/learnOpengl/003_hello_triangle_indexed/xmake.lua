add_includedirs("include")

target("learnOpengl_003_hello_triangle_indexed")
    add_files("./src/**.cpp")
    set_rundir(os.scriptdir())
    add_packages("glfw", "glad", "opengl", "glm", "imgui")

