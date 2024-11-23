add_includedirs("include")

target("particle_simulation")
    set_kind("binary")
    add_files("./src/**.cpp")
    add_packages("sfml")
    set_rundir(os.scriptdir())
