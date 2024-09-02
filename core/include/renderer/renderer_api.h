#pragma once

#include <string>

namespace CG {
    using RendererID = uint32_t;

    enum class RendererAPIType { None, OpenGL };

    // TODO: move into separate header
    enum class PrimitiveType { None = 0, Triangles, Lines };

    struct RenderAPICapabilities {
        std::string vendor;
        std::string renderer;
        std::string version;

        int maxSamples = 0;
        float maxAnisotropy = 0.0f;
        int maxTextureUnits = 0;
    };

    class RendererAPI {
    private:
    public:
        static void Init();
        static void Shutdown();

        static void Clear(float r, float g, float b, float a);
        static void SetClearColor(float r, float g, float b, float a);

        static void DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest = true, bool faceCulling = true);
        static void SetLineThickness(float thickness);

        static RenderAPICapabilities& GetCapabilities() {
            static RenderAPICapabilities _;
            return _;
        }

        static RendererAPIType Current() { return sCurrentRendererAPI_; }

    private:
        static void LoadRequiredAssets();

    private:
        static RendererAPIType sCurrentRendererAPI_;
    };
} // namespace CG
