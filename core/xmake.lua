add_includedirs("include")
   
target("core")
    set_kind("static")
    add_files("./src/**.cpp")
    add_packages("glfw", "glad", "opengl", "glm", "stb", "rapidjson", "nlohmann_json", "yaml-cpp", "rapidobj", "tinyobjloader", "tinygltf", "openfbx", "boost", "boost_ut", "range-v3", "utfcpp", "entt", "box2d", "magic_enum", "nameof", "imgui", "imguizmo", "imnodes", "spdlog", "assimp"
    )

