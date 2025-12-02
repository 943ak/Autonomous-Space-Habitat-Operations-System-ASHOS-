#ifndef ASHOS_PROTO_H
#define ASHOS_PROTO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int32_t o2_percent;      // fixed-point (percentage * 100)
    int32_t co2_ppm;         // parts per million
    int32_t pressure_pa;     // Pascals
    int32_t humidity_percent;// percentage * 100
    int32_t temperature_c;   // Celsius * 100
} life_support_sensor_t;

typedef struct {
    int32_t battery_soc;     // percentage * 100
    int32_t solar_input_w;   // Watts
    int32_t load_w;          // Watts
    int32_t bus_voltage_mv;  // millivolts
} power_sensor_t;

typedef struct {
    int32_t skin_temp_c;     // Celsius * 100
    int32_t coolant_temp_c;  // Celsius * 100
    int32_t radiator_flow;   // liters/hour * 100
} thermal_sensor_t;

typedef struct {
    int32_t set_o2_percent;
    int32_t set_pressure_pa;
    int32_t set_humidity_percent;
    int32_t set_temp_c;
} life_support_cmd_t;

typedef struct {
    int32_t charge_limit_w;
    int32_t discharge_limit_w;
    int32_t preferred_source; // 0 = battery, 1 = solar, 2 = mixed
} power_cmd_t;

typedef struct {
    int32_t radiator_valve; // 0-10000 = 0-100%
    int32_t heater_level;   // 0-10000 = 0-100%
    int32_t pump_speed;     // 0-10000 = 0-100%
} thermal_cmd_t;

typedef struct {
    life_support_cmd_t life_support;
    power_cmd_t power;
    thermal_cmd_t thermal;
} ashos_command_t;

typedef struct {
    life_support_sensor_t life_support;
    power_sensor_t power;
    thermal_sensor_t thermal;
    uint8_t watchdog_status; // 0 = nominal, 1 = degraded, 2 = safe mode
} ashos_telemetry_t;

void ls_ctrl_step(const life_support_sensor_t *sensors, const life_support_cmd_t *cmd, life_support_sensor_t *out);
void pwr_ctrl_step(const power_sensor_t *sensors, const power_cmd_t *cmd, power_sensor_t *out);
void thermal_ctrl_step(const thermal_sensor_t *sensors, const thermal_cmd_t *cmd, thermal_sensor_t *out);
void watchdog_step(const ashos_command_t *cmd, ashos_telemetry_t *telemetry);

#ifdef __cplusplus
}
#endif

#endif // ASHOS_PROTO_H
