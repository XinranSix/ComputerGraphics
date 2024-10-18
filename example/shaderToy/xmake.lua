set_runtimes("MD")

add_includedirs("include")

add_requires("glew")
add_requires("wxwidgets")

target("shaderToy")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("wxwidgets", "glew", "opengl")
    set_rundir(os.scriptdir())

