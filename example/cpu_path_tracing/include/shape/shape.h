#pragma once

#include <optional>

#include "accelerate/bounds.h"
#include "camera/ray.h"

struct Shape {
    virtual std::optional<HitInfo> intersect(const Ray& ray, float t_min, float t_max) const = 0;
    virtual Bounds getBounds() const { return {}; }
};
