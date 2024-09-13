add_includedirs("include")

add_defines("GLM_FORCE_DEPTH_ZERO_TO_ONE", "GLM_FORCE_RADIANS", "GLM_FORCE_LEFT_HANDED")
add_defines("WITH_DEBUG_INFO")

target("cpu_path_tracing")
    add_files("./src/**.cpp")
    add_packages("glm", "stb", "rapidobj")
    set_rundir(os.scriptdir())
    