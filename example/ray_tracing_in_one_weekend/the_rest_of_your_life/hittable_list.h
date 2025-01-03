#pragma once

#include <memory>
#include <vector>

#include "hittable.h"

class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() = default;
    hittable_list(std::shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<hittable> object) {
        objects.push_back(object);
        bbox = { bbox, object->bounding_box() };
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
            if (object->hit(r, interval { ray_t.min, closest_so_far },
                            temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    double pdf_value(const point3& o, const vec3& v) const override {
        auto weight { 1.0 / objects.size() };
        double sum {};

        for (const auto& object : objects) {
            sum += weight * object->pdf_value(o, v);
        }

        return sum;
    }

    vec3 random(const vec3& o) const override {
        auto int_size { static_cast<int>(objects.size()) };
        return objects[random_int(0, int_size - 1)]->random(o);
    }

    aabb bounding_box() const override { return bbox; }

private:
    aabb bbox;
};
