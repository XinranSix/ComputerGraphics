#pragma once

#include "core/events/application_event.h"
#include "core/events/mouse_event.h"
#include "core/timestep.h"
#include "opengl/orthographic_camera.h"

namespace CG::Utils {

    class OrthographicCameraController {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        OrthographicCamera& GetCamera() { return camera_; }
        const OrthographicCamera& GetCamera() const { return camera_; }

        float GetZoomLevel() const { return zoomLevel_; }
        void SetZoomLevel(float level) { zoomLevel_ = level; }

    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

    private:
        float aspectRatio_;
        float zoomLevel_ = 1.0f;
        OrthographicCamera camera_;

        bool rotation_;

        glm::vec3 cameraPosition_ { 0.0f, 0.0f, 0.0f };
        float cameraRotation_ {}; // In degrees, in the anti-clockwise direction
        float cameraTranslationSpeed_ = 5.0f, cameraRotationSpeed_ = 180.0f;
    };

} // namespace CG::Utils
