add_includedirs("include")

target("shader_toy")
    set_kind("binary")
    add_files("src/**.cpp")
    add_packages("opengl", "glfw", "glad", "glm", "imgui", "imguizmo", "stb", "spdlog")
    set_rundir(os.scriptdir())
