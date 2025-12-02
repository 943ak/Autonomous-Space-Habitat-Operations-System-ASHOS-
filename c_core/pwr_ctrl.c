#include "proto.h"

#define SOC_MIN 2000
#define SOC_MAX 10000
#define BUS_VOLT_MIN 22000
#define BUS_VOLT_MAX 30000

static int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void pwr_ctrl_step(const power_sensor_t *sensors, const power_cmd_t *cmd, power_sensor_t *out) {
    power_sensor_t next = *sensors;

    int preferred = cmd->preferred_source;
    int solar_gain = (preferred == 1 || preferred == 2) ? cmd->charge_limit_w : 0;
    int discharge = (preferred == 0 || preferred == 2) ? cmd->discharge_limit_w : cmd->discharge_limit_w / 2;

    next.solar_input_w = clamp(next.solar_input_w + solar_gain / 10, 0, 20000);
    next.load_w = clamp(next.load_w - discharge / 20, 0, 20000);

    int net_power = next.solar_input_w - next.load_w;
    next.battery_soc = clamp(next.battery_soc + net_power / 100, SOC_MIN, SOC_MAX);

    next.bus_voltage_mv = clamp(next.bus_voltage_mv + net_power / 4, BUS_VOLT_MIN, BUS_VOLT_MAX);

    *out = next;
}
