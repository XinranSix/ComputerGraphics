add_includedirs("include")
add_includedirs("../../core/include")
   
target("opengl_sandbox")
    add_files("./src/**.cpp")
    add_deps("core")
    add_packages("glfw", "glad", "opengl", "glm", "stb", "rapidjson", "nlohmann_json", "yaml-cpp", "rapidobj", "tinyobjloader", "tinygltf", "openfbx", "boost", "boost_ut", "range-v3", "utfcpp", "entt", "box2d", "magic_enum", "nameof", "imgui", "imguizmo", "imnodes", "spdlog", "assimp")
    set_rundir(os.scriptdir())


