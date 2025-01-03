#pragma once

#include <functional>
#include <string>

#include "core/base/base.h"

namespace CG {

    // Events are currently blocking, meaning when an event occurs it
    // immediately gets dispatched and must be dealt with right then an there.
    // For the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of the update stage.
    enum class EventType {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define EVENT_CLASS_TYPE(type)                                                                                         \
    static EventType GetStaticType() { return EventType::type; }                                                       \
    virtual EventType GetEventType() const override { return GetStaticType(); }                                        \
    virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)                                                                                 \
    virtual int GetCategoryFlags() const override { return category; }

    class Event {
    public:
        bool Handled = false;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }
    };

    class EventDispatcher {
    public:
        EventDispatcher(Event& event) : event_ { event } {}

        template <typename T, typename F>
        bool Dispatch(const F& func) {
            if (event_.GetEventType() == T::GetStaticType()) {
                event_.Handled = func(static_cast<T&>(event_));
                return true;
            }
            return false;
        }

    private:
        Event& event_;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.ToString(); }

} // namespace CG
