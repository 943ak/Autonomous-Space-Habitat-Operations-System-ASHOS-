#include <array>
#include <cmath>
#include <numeric>
#include <vector>

namespace ashos {

struct StateVector {
    double position[3];
    double velocity[3];
};

static constexpr double kMuEarth = 3.986004418e14; // m^3/s^2

StateVector propagate_orbit(const StateVector &state, double dt_seconds) {
    StateVector next = state;
    double r = std::sqrt(state.position[0] * state.position[0] +
                         state.position[1] * state.position[1] +
                         state.position[2] * state.position[2]);
    double accel = -kMuEarth / (r * r * r);

    for (int i = 0; i < 3; ++i) {
        next.velocity[i] += accel * state.position[i] * dt_seconds;
        next.position[i] += next.velocity[i] * dt_seconds;
    }
    return next;
}

} // namespace ashos
