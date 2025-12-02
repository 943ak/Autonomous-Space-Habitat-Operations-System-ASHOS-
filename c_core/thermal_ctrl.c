#include "proto.h"

#define COOLANT_MIN 500
#define COOLANT_MAX 4000
#define SKIN_TEMP_MIN -400
#define SKIN_TEMP_MAX 600

static int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void thermal_ctrl_step(const thermal_sensor_t *sensors, const thermal_cmd_t *cmd, thermal_sensor_t *out) {
    thermal_sensor_t next = *sensors;

    next.radiator_flow = clamp(next.radiator_flow + (cmd->radiator_valve - next.radiator_flow) / 8, COOLANT_MIN, COOLANT_MAX);
    next.coolant_temp_c = clamp(next.coolant_temp_c - (cmd->pump_speed / 30) + (cmd->heater_level / 60), -1000, 1500);
    next.skin_temp_c = clamp(next.skin_temp_c + (next.coolant_temp_c - next.skin_temp_c) / 12, SKIN_TEMP_MIN, SKIN_TEMP_MAX);

    *out = next;
}
