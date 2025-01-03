#pragma once

#include <cmath>
#include <memory>

#include "color.h"
#include "hittable.h"
#include "onb.h"
#include "pdf.h"
#include "ray.h"
#include "rtweekend.h"
#include "texture.h"
#include "vec3.h"

class scatter_record {
public:
    color attenuation;
    std::shared_ptr<pdf> pdf_ptr;
    bool skip_pdf;
    ray skip_pdf_ray;
};

class material {
public:
    virtual ~material() = default;

    virtual color emitted(const ray& r_in, const hit_record& rec, double u,
                          double v, const point3& p) const {
        return { 0, 0, 0 };
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         scatter_record& srec) const {
        return false;
    }

    virtual double scattering_pdf(const ray& r_in, const hit_record& rec,
                                  const ray& scattered) const {
        return 0;
    }
};

class lambertian : public material {
public:
    lambertian(const color& a) : albedo { std::make_shared<solid_color>(a) } {}

    lambertian(std::shared_ptr<texture> a) : albedo { a } {}

    bool scatter(const ray& r_in, const hit_record& rec,
                 scatter_record& srec) const override {
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = std::make_shared<cosine_pdf>(rec.normal);
        srec.skip_pdf = false;
        return true;
    }

    double scattering_pdf(const ray& r_in, const hit_record& rec,
                          const ray& scattered) const override {
        auto cosine { dot(rec.normal, unit_vector(scattered.direction())) };
        return cosine < 0 ? 0 : cosine / pi;
    }

private:
    std::shared_ptr<texture> albedo;
};

class metal : public material {
public:
    metal(const color& a, double f) : albedo { a }, fuzz { f < 1 ? f : 1 } {}

    bool scatter(const ray& r_in, const hit_record& rec,
                 scatter_record& srec) const override {
        srec.attenuation = albedo;
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
        auto reflected { reflect(unit_vector(r_in.direction()), rec.normal) };
        srec.skip_pdf_ray = { rec.p, reflected + fuzz * random_in_unit_sphere(),
                              r_in.time() };
        return true;
    }

private:
    color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir { index_of_refraction } {}

    bool scatter(const ray& r_in, const hit_record& rec,
                 scatter_record& srec) const override {
        srec.attenuation = color { 1.0, 1.0, 1.0 };
        srec.pdf_ptr = nullptr;
        srec.skip_pdf = true;
        double refraction_ratio { rec.front_face ? (1.0 / ir) : ir };

        vec3 unit_direction { unit_vector(r_in.direction()) };
        double cos_theta { std::min(dot(-unit_direction, rec.normal), 1.0) };
        double sin_theta { std::sqrt(1.0 - cos_theta * cos_theta) };

        bool cannot_refract { refraction_ratio * sin_theta > 1.0 };
        vec3 direction;

        if (cannot_refract ||
            reflectance(cos_theta, refraction_ratio) > random_double()) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        srec.skip_pdf_ray = { rec.p, direction, r_in.time() };
        return true;
    }

private:
    double ir; // Index of Refraction

    // 计算菲涅尔项
    static double reflectance(double consine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 { (1 - ref_idx) / (1 + ref_idx) };
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow(1 - consine, 5);
    }
};

class diffuse_light : public material {
public:
    diffuse_light(std::shared_ptr<texture> a) : emit { a } {}
    diffuse_light(color c) : emit { std::make_shared<solid_color>(c) } {}

    bool scatter(const ray& r_in, const hit_record& rec,
                 scatter_record& srec) const override {
        return false;
    }

    color emitted(const ray& r_in, const hit_record& rec, double u, double v,
                  const point3& p) const override {
        if (!rec.front_face) {
            return { 0, 0, 0 };
        }
        return emit->value(u, v, p);
    }

private:
    std::shared_ptr<texture> emit;
};

class isotropic : public material {
public:
    isotropic(color c) : albedo { std::make_shared<solid_color>(c) } {}
    isotropic(std::shared_ptr<texture> a) : albedo { a } {}

    bool scatter(const ray& r_in, const hit_record& rec,
                 scatter_record& srec) const override {
        srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = std::make_shared<sphere_pdf>();
        srec.skip_pdf = false;
        return true;
    }

    double scattering_pdf(const ray& r_in, const hit_record& rec,
                          const ray& scattered) const override {
        return 1 / (4 * pi);
    }

private:
    std::shared_ptr<texture> albedo;
};
