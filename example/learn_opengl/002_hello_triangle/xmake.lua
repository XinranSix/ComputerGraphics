add_includedirs("include")
add_includedirs("../../../core/include")

target("learn_opengl_002_hello_triangle")
    add_files("./src/**.cpp")
    add_deps("core")
    set_rundir("$(projectdir)/example/learn_opengl/002_hello_triangle")
    add_packages("glfw", "imgui-file-dialog", "imgui-color-text-edit",
    "glad", "opengl", "glm", "stb", "rapidjson", "nlohmann_json", "yaml-cpp", "rapidobj", "tinyobjloader", "tinygltf", "openfbx", "boost", "boost_ut", "range-v3", "utfcpp", "entt", "box2d", "magic_enum", "nameof", "imgui", "imguizmo", "imnodes", "spdlog", "assimp")

