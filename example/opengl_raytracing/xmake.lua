add_includedirs("include")

add_defines("STB_IMAGE_IMPLEMENTATION")

target("opengl_raytracing")
    set_kind("binary")
    add_files("./src/**.cpp")
    add_packages("glfw", "glm", "stb", "fmt", "box2d", "glad", "imgui")
    set_rundir("$(projectdir)/example/opengl_raytracing")
