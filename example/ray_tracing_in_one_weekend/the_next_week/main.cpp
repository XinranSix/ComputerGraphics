#include <memory>

#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "constant_medium.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"

void random_spheres() {
    hittable_list world {};

    auto checker { std::make_shared<checker_texture>(
        0.32, color { 0.2, 0.3, 0.1 }, color { 0.9, 0.9, 0.9 }) };
    world.add(std::make_shared<sphere>(point3 { 0, -1000, 0 }, 1000,
                                       std::make_shared<lambertian>(checker)));

    for (int a { -11 }; a < 11; ++a) {
        for (int b { -11 }; b < 11; ++b) {
            auto choose_mat { random_double() };
            point3 center { a + 0.9 * random_double(), 0.2,
                            b + 0.9 * random_double() };

            if ((center - point3 { 4, 0.2, 0 }).length() > 0.9) {
                std::shared_ptr<material> sphere_material;
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo { color::random() * color::random() };
                    sphere_material = std::make_shared<lambertian>(albedo);
                    auto center2 { center +
                                   vec3 { 0, random_double(0, 0.5), 0 } };
                    world.add(std::make_shared<sphere>(center, center2, 0.2,
                                                       sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo { color::random(0.5, 1) };
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(
                        std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(
                        std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 { std::make_shared<dielectric>(1.5) };
    world.add(std::make_shared<sphere>(point3 { 0, 1, 0 }, 1.0, material1));

    auto material2 { std::make_shared<lambertian>(color { 0.4, 0.2, 0.1 }) };
    world.add(std::make_shared<sphere>(point3 { -4, 1, 0 }, 1.0, material2));

    auto material3 { std::make_shared<metal>(color { 0.7, 0.6, 0.5 }, 0.0) };
    world.add(std::make_shared<sphere>(point3 { 4, 1, 0 }, 1.0, material3));

    world = { std::make_shared<bvh_node>(world) };

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = { 13, 2, 3 };
    cam.lookat = { 0, 1, 0 };
    cam.vup = { 0, 1, 0 };

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);
}

void two_spheres() {
    hittable_list world;

    auto checker { std::make_shared<checker_texture>(
        0.32, color { 0.2, 0.3, 0.1 }, color { 0.9, 0.9, 0.9 }) };

    world.add(std::make_shared<sphere>(point3 { 0, -10, 0 }, 10,
                                       std::make_shared<lambertian>(checker)));
    world.add(std::make_shared<sphere>(point3 { 0, 10, 0 }, 10,
                                       std::make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 20;

    cam.lookfrom = { 13, 2, 3 };
    cam.lookat = { 0, 0, 0 };
    cam.vup = { 0, 1, 0 };

    cam.defocus_angle = 0;

    cam.render(world);
}

void earth() {
    auto earth_texture { std::make_shared<image_texture>(
        "./asset/texture/earthmap.jpg") };
    auto earth_surface { std::make_shared<lambertian>(earth_texture) };
    auto globe { std::make_shared<sphere>(point3 { 0, 0, 0 }, 2,
                                          earth_surface) };

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = { 0, 0, 12 };
    cam.lookat = { 0, 0, 0 };
    cam.vup = { 0, 1, 0 };

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}

void two_perlin_spheres() {
    hittable_list world;

    auto pertext { std::make_shared<noise_texture>(4) };
    world.add(std::make_shared<sphere>(point3 { 0, -1000, 0 }, 1000,
                                       std::make_shared<lambertian>(pertext)));
    world.add(std::make_shared<sphere>(point3 { 0, 2, 0 }, 2,
                                       std::make_shared<lambertian>(pertext)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = { 13, 2, 3 };
    cam.lookat = { 0, 0, 0 };
    cam.vup = { 0, 1, 0 };

    cam.defocus_angle = 0;

    cam.render(world);
}

void quads() {

    hittable_list world {};

    // Materials
    auto left_red { std::make_shared<lambertian>(color { 1.0, 0.2, 0.2 }) };
    auto back_green { std::make_shared<lambertian>(color { 0.2, 1.0, 0.2 }) };
    auto right_blue { std::make_shared<lambertian>(color { 0.2, 0.2, 1.0 }) };
    auto upper_orange { std::make_shared<lambertian>(color { 1.0, 0.5, 0.0 }) };
    auto lower_teal { std::make_shared<lambertian>(color { 0.2, 0.8, 0.8 }) };

    // Quads
    world.add(std::make_shared<quad>(point3 { -3, -2, 5 }, vec3 { 0, 0, -4 },
                                     vec3 { 0, 4, 0 }, left_red));
    world.add(std::make_shared<quad>(point3 { -2, -2, 0 }, vec3 { 4, 0, 0 },
                                     vec3 { 0, 4, 0 }, back_green));
    world.add(std::make_shared<quad>(point3 { 3, -2, 1 }, vec3 { 0, 0, 4 },
                                     vec3 { 0, 4, 0 }, right_blue));
    world.add(std::make_shared<quad>(point3 { -2, 3, 1 }, vec3 { 4, 0, 0 },
                                     vec3 { 0, 0, 4 }, upper_orange));
    world.add(std::make_shared<quad>(point3 { -2, -3, 5 }, vec3 { 4, 0, 0 },
                                     vec3 { 0, 0, -4 }, lower_teal));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 80;
    cam.lookfrom = { 0, 0, 0 };
    cam.lookat = { 0, 0, 0 };
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void simple_light() {
    hittable_list world {};

    auto pertext { std::make_shared<noise_texture>(4) };
    world.add(std::make_shared<sphere>(point3 { 0, -1000, 0 }, 1000,
                                       std::make_shared<lambertian>(pertext)));
    world.add(std::make_shared<sphere>(point3 { 0, 2, 0 }, 2,
                                       std::make_shared<lambertian>(pertext)));

    auto difflight { std::make_shared<diffuse_light>(color { 4, 4, 4 }) };
    world.add(std::make_shared<sphere>(point3 { 0, 7, 0 }, 2, difflight));
    world.add(std::make_shared<quad>(point3 { 3, 1, -2 }, vec3 { 2, 0, 0 },
                                     vec3 { 0, 2, 0 }, difflight));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = { 0, 0, 0 };

    cam.vfov = 20;
    cam.lookfrom = { 26, 3, 6 };
    cam.lookat = { 0, 2, 0 };
    cam.vup = { 0, 1, 0 };

    cam.defocus_angle = 0;

    cam.render(world);
}

void cornell_box() {
    hittable_list world;

    auto red { std::make_shared<lambertian>(color { 0.65, 0.05, 0.05 }) };
    auto white { std::make_shared<lambertian>(color { 0.73, 0.73, 0.73 }) };
    auto green { std::make_shared<lambertian>(color { 0.12, 0.45, 0.15 }) };
    auto light { std::make_shared<diffuse_light>(color { 15, 15, 15 }) };

    world.add(std::make_shared<quad>(point3 { 555, 0, 0 }, vec3 { 0, 555, 0 },
                                     vec3 { 0, 0, 555 }, green));
    world.add(std::make_shared<quad>(point3 { 0, 0, 0 }, vec3 { 0, 555, 0 },
                                     vec3 { 0, 0, 555 }, red));
    world.add(std::make_shared<quad>(point3 { 343, 554, 332 },
                                     vec3 { -130, 0, 0 }, vec3 { 0, 0, -105 },
                                     light));
    world.add(std::make_shared<quad>(point3 { 0, 0, 0 }, vec3 { 555, 0, 0 },
                                     vec3 { 0, 0, 555 }, white));
    world.add(std::make_shared<quad>(point3 { 555, 555, 555 },
                                     vec3 { -555, 0, 0 }, vec3 { 0, 0, -555 },
                                     white));
    world.add(std::make_shared<quad>(point3 { 0, 0, 555 }, vec3 { 555, 0, 0 },
                                     vec3 { 0, 555, 0 }, white));

    std::shared_ptr<hittable> box1 { box({ 0, 0, 0 }, { 165, 330, 165 },
                                         white) };
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, point3 { 265, 0, 295 });
    world.add(box1);

    std::shared_ptr<hittable> box2 { box({ 0, 0, 0 }, { 165, 165, 165 },
                                         white) };
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>(box2, point3 { 130, 0, 65 });
    world.add(box2);

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 200;
    cam.max_depth = 50;
    cam.background = { 0, 0, 0 };

    cam.vfov = 40;
    cam.lookfrom = { 278, 278, -800 };
    cam.lookat = { 278, 278, 0 };
    cam.vup = { 0, 1, 0 };

    cam.defocus_angle = 0;
    cam.render(world);
}

void cornell_smoke() {

    hittable_list world {};

    auto red { std::make_shared<lambertian>(color { 0.65, 0.05, 0.05 }) };
    auto white { std::make_shared<lambertian>(color { 0.73, 0.73, 0.73 }) };
    auto green { std::make_shared<lambertian>(color { 0.12, 0.45, 0.15 }) };
    auto light { std::make_shared<diffuse_light>(color { 7, 7, 7 }) };

    world.add(std::make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0),
                                     vec3(0, 0, 555), green));
    world.add(std::make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0),
                                     vec3(0, 0, 555), red));
    world.add(std::make_shared<quad>(point3(113, 554, 127), vec3(330, 0, 0),
                                     vec3(0, 0, 305), light));
    world.add(std::make_shared<quad>(point3(0, 555, 0), vec3(555, 0, 0),
                                     vec3(0, 0, 555), white));
    world.add(std::make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0),
                                     vec3(0, 0, 555), white));
    world.add(std::make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0),
                                     vec3(0, 555, 0), white));

    std::shared_ptr<hittable> box1 { box({ 0, 0, 0 }, { 165, 330, 165 },
                                         white) };
    box1 = std::make_shared<rotate_y>(box1, 15);
    box1 = std::make_shared<translate>(box1, vec3(265, 0, 295));

    std::shared_ptr<hittable> box2 = box({ 0, 0, 0 }, { 165, 165, 165 }, white);
    box2 = std::make_shared<rotate_y>(box2, -18);
    box2 = std::make_shared<translate>(box2, vec3(130, 0, 65));

    world.add(std::make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
    world.add(std::make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth = 50;
    cam.background = color(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat = point3(278, 278, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {

    hittable_list boxes1;
    auto ground { std::make_shared<lambertian>(color { 0.48, 0.83, 0.53 }) };

    int boxes_per_side { 20 };
    for (int i {}; i < boxes_per_side; ++i) {
        for (int j {}; j < boxes_per_side; ++j) {
            auto w { 100.0 };
            auto x0 { -1000.0 + i * w };
            auto z0 { -1000.0 + j * w };
            auto y0 { 0.0 };
            auto x1 { x0 + w };
            auto y1 { random_double(1, 101) };
            auto z1 = z0 + w;

            boxes1.add(box({ x0, y0, z0 }, { x1, y1, z1 }, ground));
        }
    }

    hittable_list world {};

    world.add(std::make_shared<bvh_node>(boxes1));

    auto light { std::make_shared<diffuse_light>(color { 7, 7, 7 }) };
    world.add(std::make_shared<quad>(point3 { 123, 554, 147 },
                                     vec3 { 300, 0, 0 }, vec3 { 0, 0, 265 },
                                     light));

    point3 center1 { 400, 400, 200 };
    auto center2 { center1 + vec3 { 30, 0, 0 } };
    auto sphere_material { std::make_shared<lambertian>(
        color { 0.7, 0.3, 0.1 }) };
    world.add(std::make_shared<sphere>(center1, center2, 50, sphere_material));

    world.add(std::make_shared<sphere>(point3 { 260, 150, 45 }, 50,
                                       std::make_shared<dielectric>(1.5)));
    world.add(std::make_shared<sphere>(
        point3 { 0, 150, 145 }, 50,
        std::make_shared<metal>(color { 0.8, 0.8, 0.9 }, 1.0)));

    auto boundary { std::make_shared<sphere>(
        point3 { 360, 150, 145 }, 70, std::make_shared<dielectric>(1.5)) };
    world.add(boundary);
    world.add(std::make_shared<constant_medium>(boundary, 0.2,
                                                color { 0.2, 0.4, 0.9 }));
    boundary = std::make_shared<sphere>(point3 { 0, 0, 0 }, 5000,
                                        std::make_shared<dielectric>(1.5));
    world.add(
        std::make_shared<constant_medium>(boundary, 0.0001, color { 1, 1, 1 }));

    auto emat { std::make_shared<lambertian>(
        std::make_shared<image_texture>("./asset/texture/earthmap.jpg")) };
    world.add(std::make_shared<sphere>(point3 { 400, 200, 400 }, 100, emat));
    auto pertext { std::make_shared<noise_texture>(0.1) };
    world.add(std::make_shared<sphere>(point3 { 220, 280, 300 }, 80,
                                       std::make_shared<lambertian>(pertext)));

    hittable_list boxes2 {};
    auto white { std::make_shared<lambertian>(color { 0.73, 0.73, 0.73 }) };
    int ns { 1000 };
    for (int j {}; j < ns; ++j) {
        boxes2.add(std::make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    world.add(std::make_shared<translate>(
        std::make_shared<rotate_y>(std::make_shared<bvh_node>(boxes2), 15),
        vec3 { -100, 270, 395 }));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = image_width;
    cam.samples_per_pixel = samples_per_pixel;
    cam.max_depth = max_depth;
    cam.background = { 0, 0, 0 };

    cam.vfov = 40;
    cam.lookfrom = { 478, 278, -600 };
    cam.lookat = { 278, 278, 0 };
    cam.vup = { 0, 1, 0 };

    cam.defocus_angle = 0;

    cam.render(world);
}

int main() {
    switch (9) {
        case 1: random_spheres(); break;
        case 2: two_spheres(); break;
        case 3: earth(); break;
        case 4: two_perlin_spheres(); break;
        case 5: quads(); break;
        case 6: simple_light(); break;
        case 7: cornell_box(); break;
        case 8: cornell_smoke(); break;
        case 9: final_scene(800, 10000, 40); break;
        default: final_scene(400, 250, 4); break;
    }

    return 0;
}
