add_includedirs("include")
add_includedirs("../../../core/include")

target("learn_opengl_001.hello_window_clear")
    add_files("./src/**.cpp")
    add_deps("core")
    set_rundir("$(projectdir)/example/learn_opengl/001.hello_window_clear")
    add_packages("glfw", "imgui-file-dialog", "imgui-color-text-edit",
    "glad", "opengl", "glm", "stb", "rapidjson", "nlohmann_json", "yaml-cpp", "rapidobj", "tinyobjloader", "tinygltf", "openfbx", "boost", "boost_ut", "range-v3", "utfcpp", "entt", "box2d", "magic_enum", "nameof", "imgui", "imguizmo", "imnodes", "spdlog", "assimp")

