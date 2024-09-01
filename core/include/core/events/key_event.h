#pragma once

#include <sstream>

#include "core/events/event.h"

namespace CG {

    class KeyEvent : public Event {
    public:
        inline int GetKeyCode() const { return keyCode_; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(int keycode) : keyCode_ { keycode } {}

        int keyCode_;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), repeatCount_ { repeatCount } {}

        inline int GetRepeatCount() const { return repeatCount_; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << keyCode_ << " (" << repeatCount_ << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int repeatCount_;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(int keycode) : KeyEvent { keycode } {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << keyCode_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent {
    public:
        KeyTypedEvent(int keycode) : KeyEvent { keycode } {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << keyCode_;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
} // namespace CG
