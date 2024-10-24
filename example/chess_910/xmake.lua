add_includedirs("include")

target("chess_910")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("opengl", "glew", "glm", "assimp", "stb", "glfw")
    set_rundir(os.scriptdir())

