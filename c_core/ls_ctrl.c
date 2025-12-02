#include "proto.h"

#define O2_MIN 1900  // 19.00%
#define O2_MAX 2300  // 23.00%
#define CO2_MAX 5000 // ppm
#define HUMIDITY_MIN 3000
#define HUMIDITY_MAX 6000
#define PRESSURE_MIN 95000
#define PRESSURE_MAX 105000
#define TEMP_MIN -1000  // -10.00 C
#define TEMP_MAX 3500   // 35.00 C

static int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void ls_ctrl_step(const life_support_sensor_t *sensors, const life_support_cmd_t *cmd, life_support_sensor_t *out) {
    // Deterministic, fixed-time control using integer math.
    life_support_sensor_t next = *sensors;

    int delta_o2 = cmd->set_o2_percent - sensors->o2_percent;
    next.o2_percent += delta_o2 / 4; // small step to avoid oscillation

    int delta_pressure = cmd->set_pressure_pa - sensors->pressure_pa;
    next.pressure_pa += delta_pressure / 8;

    int delta_humidity = cmd->set_humidity_percent - sensors->humidity_percent;
    next.humidity_percent += delta_humidity / 6;

    int delta_temp = cmd->set_temp_c - sensors->temperature_c;
    next.temperature_c += delta_temp / 5;

    // CO2 scrubbing modeled as decay
    if (next.co2_ppm > 0) {
        next.co2_ppm -= (next.co2_ppm / 16) + 1;
    }

    next.o2_percent = clamp(next.o2_percent, O2_MIN, O2_MAX);
    next.pressure_pa = clamp(next.pressure_pa, PRESSURE_MIN, PRESSURE_MAX);
    next.humidity_percent = clamp(next.humidity_percent, HUMIDITY_MIN, HUMIDITY_MAX);
    next.temperature_c = clamp(next.temperature_c, TEMP_MIN, TEMP_MAX);
    next.co2_ppm = clamp(next.co2_ppm, 300, CO2_MAX);

    *out = next;
}
