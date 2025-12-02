#include "proto.h"

#define SAFE_O2 2100
#define SAFE_PRESSURE 101325
#define SAFE_HUMIDITY 4500
#define SAFE_TEMP 2150
#define SAFE_BUS_VOLT 25000

static void apply_safe_profiles(ashos_telemetry_t *telemetry) {
    telemetry->life_support.o2_percent = SAFE_O2;
    telemetry->life_support.pressure_pa = SAFE_PRESSURE;
    telemetry->life_support.humidity_percent = SAFE_HUMIDITY;
    telemetry->life_support.temperature_c = SAFE_TEMP;
    telemetry->life_support.co2_ppm = 800;

    telemetry->power.bus_voltage_mv = SAFE_BUS_VOLT;
    telemetry->power.battery_soc = 8000;
    telemetry->power.solar_input_w = 5000;
    telemetry->power.load_w = 3000;
}

void watchdog_step(const ashos_command_t *cmd, ashos_telemetry_t *telemetry) {
    // Simple validation: ensure commands are within expected bounds.
    int invalid = 0;
    if (cmd->life_support.set_o2_percent < 1800 || cmd->life_support.set_o2_percent > 2400) invalid = 1;
    if (cmd->power.charge_limit_w < 0 || cmd->power.charge_limit_w > 20000) invalid = 1;
    if (cmd->thermal.radiator_valve < 0 || cmd->thermal.radiator_valve > 10000) invalid = 1;

    if (invalid) {
        telemetry->watchdog_status = 2; // safe mode
        apply_safe_profiles(telemetry);
    } else if (telemetry->watchdog_status != 0) {
        telemetry->watchdog_status = 1; // degraded but recovering
    } else {
        telemetry->watchdog_status = 0;
    }
}
