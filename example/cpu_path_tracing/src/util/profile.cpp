#include <iostream>

#include "util/profile.h"

Profile::Profile(const std::string& name)
    : name { name }, start { std::chrono::high_resolution_clock::now() } {}

Profile::~Profile() {
    auto duration = std::chrono::high_resolution_clock::now() - start;
    auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << "Profile \"" << name << "\": " << ms << "ms" << std::endl;
}
