#include <iomanip>
#include <iostream>
#include <math.h>

#include "rtweekend.h"

double f(double r1, double r2) {
    auto z { 1 - r2 };
    auto cos_theta { z };
    return cos_theta * cos_theta * cos_theta;
}

double pdf(double r1, double r2) { return 1.0 / (2.0 * pi); }

int main(int argc, char* argv[]) {
    int N = 1000000;

    auto sum = 0.0;
    for (int i = 0; i < N; i++) {
        auto r1 = random_double();
        auto r2 = random_double();
        sum += f(r1, r2) / pdf(r1, r2);
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "PI/2 = " << pi / 2.0 << '\n';
    std::cout << "Estimate = " << sum / N << '\n';

    return 0;
}
