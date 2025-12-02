#include <chrono>
#include <iostream>
#include <thread>

#include "../c_core/proto.h"

extern "C" {
void ls_ctrl_step(const life_support_sensor_t *sensors, const life_support_cmd_t *cmd, life_support_sensor_t *out);
void pwr_ctrl_step(const power_sensor_t *sensors, const power_cmd_t *cmd, power_sensor_t *out);
void thermal_ctrl_step(const thermal_sensor_t *sensors, const thermal_cmd_t *cmd, thermal_sensor_t *out);
void watchdog_step(const ashos_command_t *cmd, ashos_telemetry_t *telemetry);
}

namespace ashos {

ashos_command_t default_command() {
    ashos_command_t cmd{};
    cmd.life_support.set_o2_percent = 2100;
    cmd.life_support.set_pressure_pa = 101325;
    cmd.life_support.set_humidity_percent = 4500;
    cmd.life_support.set_temp_c = 2200;
    cmd.power.charge_limit_w = 5000;
    cmd.power.discharge_limit_w = 4000;
    cmd.power.preferred_source = 2;
    cmd.thermal.heater_level = 1000;
    cmd.thermal.pump_speed = 5000;
    cmd.thermal.radiator_valve = 4000;
    return cmd;
}

void run_once(ashos_telemetry_t &tlm, const ashos_command_t &cmd) {
    life_support_sensor_t ls_out{};
    power_sensor_t pwr_out{};
    thermal_sensor_t th_out{};

    ls_ctrl_step(&tlm.life_support, &cmd.life_support, &ls_out);
    pwr_ctrl_step(&tlm.power, &cmd.power, &pwr_out);
    thermal_ctrl_step(&tlm.thermal, &cmd.thermal, &th_out);

    tlm.life_support = ls_out;
    tlm.power = pwr_out;
    tlm.thermal = th_out;
    watchdog_step(&cmd, &tlm);
}

} // namespace ashos

int main() {
    ashos::ashos_command_t cmd = ashos::default_command();
    ashos_telemetry_t telemetry{};
    telemetry.life_support.o2_percent = 2050;
    telemetry.life_support.pressure_pa = 101000;
    telemetry.life_support.humidity_percent = 4000;
    telemetry.life_support.temperature_c = 2000;
    telemetry.life_support.co2_ppm = 1000;
    telemetry.power.battery_soc = 8000;
    telemetry.power.solar_input_w = 3000;
    telemetry.power.load_w = 3500;
    telemetry.power.bus_voltage_mv = 24000;
    telemetry.thermal.coolant_temp_c = 2500;
    telemetry.thermal.skin_temp_c = 2000;
    telemetry.thermal.radiator_flow = 1200;

    for (int i = 0; i < 5; ++i) {
        ashos::run_once(telemetry, cmd);
        std::cout << "[tick " << i << "] O2=" << telemetry.life_support.o2_percent
                  << " temp=" << telemetry.life_support.temperature_c
                  << " SOC=" << telemetry.power.battery_soc
                  << " watchdog=" << static_cast<int>(telemetry.watchdog_status) << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}
