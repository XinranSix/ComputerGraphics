#pragma once

#include <cmath>
#include <memory>

#include "hittable.h"
#include "onb.h"
#include "vec3.h"

class sphere : public hittable {
public:
    // Stationary Sphere
    sphere(point3 _cenetr, double _radius, std::shared_ptr<material> _material)
        : center1 { _cenetr }, radius { _radius }, mat { _material },
          is_moving { false } {
        vec3 rvec { radius, radius, radius };
        bbox = { center1 - rvec, center1 + rvec };
    };

    // Moving Sphere
    sphere(point3 _center1, point3 _center2, double _radius,
           std::shared_ptr<material> _material)
        : center1 { _center1 }, radius { _radius }, mat { _material },
          is_moving { true } {
        vec3 rvec { radius, radius, radius };
        aabb box1 { _center1 - rvec, _center1 + rvec };
        aabb box2 { _center2 - rvec, _center2 + rvec };
        bbox = aabb(box1, box2);

        center_vec = _center2 - center1;
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        auto center { is_moving ? sphere_center(r.time()) : center1 };
        vec3 oc { r.origin() - center };
        auto a { r.direction().length_squared() };
        auto half_b = dot(oc, r.direction());
        auto c { oc.length_squared() - radius * radius };

        auto direction = half_b * half_b - a * c;
        if (direction < 0) {
            return false;
        }
        auto sqrtd { std::sqrt(direction) };

        // Find the nearest root that lies in the acceptabl range.
        auto root { (-half_b - sqrtd) / a };
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal { (rec.p - center) / radius };
        rec.set_face_normal(r, outward_normal);
        get_shpere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

    double pdf_value(const point3& o, const vec3& v) const override {
        // This method only works for stationary shperes.

        hit_record rec {};
        if (!this->hit({ o, v }, { 0.001, infinity }, rec)) {
            return 0;
        }

        auto cos_theta_max { std::sqrt(
            1 - radius * radius / (center1 - o).length_squared()) };
        auto solid_angle { 2 * pi * (1 - cos_theta_max) };

        return 1 / solid_angle;
    }

    vec3 random(const point3& o) const override {
        auto direction { center1 - o };
        auto distance_squared { direction.length_squared() };
        onb uvw;
        uvw.build_from_w(direction);
        return uvw.local(random_to_sphere(radius, distance_squared));
    }

private:
    point3 center1;
    double radius;
    std::shared_ptr<material> mat;
    bool is_moving;
    vec3 center_vec;
    aabb bbox;

    point3 sphere_center(double time) const {
        // Linearly interpolate from center1 to center2 according to time, where
        // t = 0 yields center1, and t = 1 yields centers2.
        return center1 + time * center_vec;
    }

    static void get_shpere_uv(const point3& p, double& u, double& v) {
        // p: given point on the sphere of radius one, centered at the origin.
        // u: returned value [0, 1] of angle around the Y axis from X = -1.
        // v: returned value [0, 1] of angle from Y = -1 to Y = +1.
        //  <1 0 0> yields <0.50 0.50>      <-1 0 0> yields <0.00 0.50>
        //  <0 1 0> yields <0.50 1,00>      <0 -1 0> yields <0.50 0.00>
        //  <0 0 1> yields <0.25 0.50>      <0 0 -1> yields <0.75 0.50>

        auto theta { std::acos(-p.y()) };
        auto phi { std::atan2(-p.z(), p.x()) + pi };

        u = phi / (2 * pi);
        v = theta / pi;
    }

    static vec3 random_to_sphere(double radius, double distance_squared) {
        auto r1 { random_double() };
        auto r2 { random_double() };
        auto z { 1 +
                 r2 * (std::sqrt(1 - radius * radius / distance_squared) - 1) };

        auto phi { 2 * pi * r1 };
        auto x { std::cos(phi) * std::sqrt(1 - z * z) };
        auto y { std::sin(phi) * std::sqrt(1 - z * z) };

        return { x, y, z };
    }
};
