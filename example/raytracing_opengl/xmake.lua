add_includedirs("include")

add_defines("STB_IMAGE_IMPLEMENTATION")

target("raytracing_opengl")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("glfw", "glad", "glm", "stb", "fmt", "box2d")
    set_rundir(os.scriptdir())
    