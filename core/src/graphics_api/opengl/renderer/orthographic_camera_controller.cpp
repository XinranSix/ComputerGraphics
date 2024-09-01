#include "graphics_api/opengl/renderer/orthographic_camera_controller.h"
#include "input/input.h"
#include "input/key_codes.h"

namespace CG::Utils {

    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : aspectRatio_ { aspectRatio },
          camera_ { -aspectRatio_ * zoomLevel_, aspectRatio_ * zoomLevel_, -zoomLevel_, zoomLevel_ },
          rotation_ { rotation } {}

    void OrthographicCameraController::OnUpdate(Timestep ts) {
        if (Input::IsKeyPressed(CG_KEY_A)) {
            cameraPosition_.x -= cos(glm::radians(cameraRotation_)) * cameraTranslationSpeed_ * ts;
            cameraPosition_.y -= sin(glm::radians(cameraRotation_)) * cameraTranslationSpeed_ * ts;
        } else if (Input::IsKeyPressed(CG_KEY_D)) {
            cameraPosition_.x += cos(glm::radians(cameraRotation_)) * cameraTranslationSpeed_ * ts;
            cameraPosition_.y += sin(glm::radians(cameraRotation_)) * cameraTranslationSpeed_ * ts;
        }

        if (Input::IsKeyPressed(CG_KEY_W)) {
            cameraPosition_.x += -sin(glm::radians(cameraRotation_)) * cameraTranslationSpeed_ * ts;
            cameraPosition_.y += cos(glm::radians(cameraRotation_)) * cameraTranslationSpeed_ * ts;
        } else if (Input::IsKeyPressed(CG_KEY_S)) {
            cameraPosition_.x -= -sin(glm::radians(cameraRotation_)) * cameraTranslationSpeed_ * ts;
            cameraPosition_.y -= cos(glm::radians(cameraRotation_)) * cameraTranslationSpeed_ * ts;
        }

        if (rotation_) {
            if (Input::IsKeyPressed(CG_KEY_Q)) {
                cameraRotation_ += cameraRotationSpeed_ * ts;
            }
            if (Input::IsKeyPressed(CG_KEY_E)) {
                cameraRotation_ -= cameraRotationSpeed_ * ts;
            }

            if (cameraRotation_ > 180.0f) {
                cameraRotation_ -= 360.0f;
            } else if (cameraRotation_ <= -180.0f) {
                cameraRotation_ += 360.0f;
            }

            camera_.SetRotation(cameraRotation_);
        }

        camera_.SetPosition(cameraPosition_);

        cameraTranslationSpeed_ = zoomLevel_;
    }

    void OrthographicCameraController::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(CG_CORE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(CG_CORE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
        zoomLevel_ -= e.GetYOffset() * 0.25f;
        zoomLevel_ = std::max(zoomLevel_, 0.25f);
        camera_.SetProjection(-aspectRatio_ * zoomLevel_, aspectRatio_ * zoomLevel_, -zoomLevel_, zoomLevel_);
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
        aspectRatio_ = (float)e.GetWidth() / (float)e.GetHeight();
        camera_.SetProjection(-aspectRatio_ * zoomLevel_, aspectRatio_ * zoomLevel_, -zoomLevel_, zoomLevel_);
        return false;
    }

} // namespace CG::Utils
