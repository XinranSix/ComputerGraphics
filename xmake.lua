set_project("ComputerGraphics")
set_version("1.0.0")

add_rules("mode.debug", "mode.release")

set_languages("c++23")

if is_plat("windows") then
    add_cxflags("/utf-8")
end

-- add_defines("IMGUI_DEFINE_MATH_OPERATORS")

-- includes("core")
includes("test")
includes("example")

-- 图形学库
add_requires("glfw") 
add_requires("glad") 
add_requires("opengl") 
add_requires("glm")
add_requires("libsdl")

-- image loader
add_requires("stb")
add_requires("libjpeg-turbo")
add_requires("libwebp")

-- 字体库
add_requires("freetype")

-- 文本文件解析
add_requires("rapidjson") 
add_requires("simdjson") 
add_requires("nlohmann_json") 
add_requires("yaml-cpp") 

-- 音频与视频
add_requires("ffmpeg")
add_requires("raylib")

--  mesh loader
add_requires("assimp") 
add_requires("rapidobj")
add_requires("tinyobjloader") 
add_requires("tinygltf") 
add_requires("fastgltf") 
add_requires("fx-gltf") 
add_requires("openfbx") 
add_requires("ufbx") 
add_requires("rply") 
add_requires("happly") 
add_requires("tinyply") 

add_requires("boost", {configs = {all = true}}) 
add_requires("boost_ut")
add_requires("range-v3") 
add_requires("stduuid") 
add_requires("xsimd") 
add_requires("xxhash") 
add_requires("crossguid") 
add_requires("utfcpp") 
add_requires("entt") 
add_requires("spdlog", {configs = {header_only = false}}) 
add_requires("fmt") 

add_requires("box2d") 
add_requires("bullet3") 

add_requires("indicators")  -- 进度条

add_requires("magic_enum")
add_requires("nameof")
add_requires("watcher")

add_requires("imgui docking", {configs = {glfw = true, opengl3 = true, freetype = true, wchar32 = true}}) 
add_requires("imguizmo")
add_requires("imnodes")
add_requires("imgui-file-dialog")
add_requires("imgui-color-text-edit")
add_requires("im3d")

-- xmake project -k compile_commands
-- xmake project -k makefile
-- xmake project -k cmakelists
-- xmake project -k vsxmake -m "debug,release"

