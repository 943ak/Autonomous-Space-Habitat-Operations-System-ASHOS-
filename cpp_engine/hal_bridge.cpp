#include <array>
#include <cstdint>
#include <mutex>
#include <optional>
#include "../c_core/proto.h"

namespace ashos {

struct HalState {
    ashos_command_t command{};
    ashos_telemetry_t telemetry{};
    std::mutex mtx;
};

static HalState g_state;

void write_command(const ashos_command_t &cmd) {
    std::lock_guard<std::mutex> lock(g_state.mtx);
    g_state.command = cmd;
}

ashos_command_t read_command() {
    std::lock_guard<std::mutex> lock(g_state.mtx);
    return g_state.command;
}

void publish_telemetry(const ashos_telemetry_t &tlm) {
    std::lock_guard<std::mutex> lock(g_state.mtx);
    g_state.telemetry = tlm;
}

ashos_telemetry_t latest_telemetry() {
    std::lock_guard<std::mutex> lock(g_state.mtx);
    return g_state.telemetry;
}

} // namespace ashos
