#include "graphics_api/opengl/renderer/opengl_debug.h"
#include "core/base/base.h"
#include "core/log/log.h"

namespace CG::GL::Utils {

    static DebugLogLevel sDebugLogLevel = DebugLogLevel::HighAssert;

    void SetGLDebugLogLevel(DebugLogLevel level) { sDebugLogLevel = level; }

    void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                          const void* userParam) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                if ((int)sDebugLogLevel > 0) {
                    CG_CORE_ERROR("[OpenGL Debug HIGH] {0}", message);
                    if (sDebugLogLevel == DebugLogLevel::HighAssert) {
                        CG_CORE_ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
                    }
                }
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                if ((int)sDebugLogLevel > 2) {
                    CG_CORE_WARN("[OpenGL Debug MEDIUM] {0}", message);
                }
                break;
            case GL_DEBUG_SEVERITY_LOW:
                if ((int)sDebugLogLevel > 3) {
                    CG_CORE_INFO("[OpenGL Debug LOW] {0}", message);
                }
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                if ((int)sDebugLogLevel > 4) {
                    CG_CORE_TRACE("[OpenGL Debug NOTIFICATION] {0}", message);
                }
                break;
        }
    }

    void EnableGLDebugging() {
        glDebugMessageCallback(OpenGLLogMessage, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

} // namespace CG::GL::Utils
