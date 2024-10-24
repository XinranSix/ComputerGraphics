set_runtimes("MD")

add_includedirs("include")

target("shader_toy")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("wxwidgets", "glew", "opengl")
    set_rundir(os.scriptdir())

