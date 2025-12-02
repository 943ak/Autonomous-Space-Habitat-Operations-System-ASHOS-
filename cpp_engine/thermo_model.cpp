#include <algorithm>
#include <cmath>

namespace ashos {

double radiator_rejection(double area_m2, double temp_k) {
    constexpr double sigma = 5.670374419e-8; // W/m^2 K^4
    return area_m2 * sigma * std::pow(temp_k, 4);
}

double heater_energy(double power_w, double duration_s) {
    return power_w * duration_s;
}

} // namespace ashos
