#include <iostream>

#include "renderer/base_renderer.h"
#include "thread/thread_pool.h"
#include "util/profile.h"
#include "util/progress.h"

void BaseRenderer::render(size_t spp, const std::filesystem::path& filename) {
    PROFILE("Render " + std::to_string(spp) + "spp " + filename.string());
    size_t current_spp {}, increase { 1 };
    auto& film = camera.getFilm();
    film.clear();
    Progress progress { film.getWidth() * film.getHeight() * spp, 20 };
    while (current_spp < spp) {
        thread_pool.parallelFor(film.getWidth(), film.getHeight(), [&](size_t x, size_t y) {
            for (int i = 0; i < increase; i++) {
                film.addSample(x, y, renderPixel({ x, y }));
            }
            progress.update(increase);
        });
        thread_pool.wait();

        current_spp += increase;
        increase = std::min<size_t>(current_spp, 32);

        film.save(filename);
        std::cout << current_spp << "spp has been saved to " << filename << std::endl;
    }
}
