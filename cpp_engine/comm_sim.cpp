#include <chrono>
#include <queue>
#include <random>
#include <utility>

namespace ashos {

struct Message {
    std::chrono::steady_clock::time_point delivery;
    std::vector<uint8_t> payload;
};

class CommSimulator {
  public:
    explicit CommSimulator(double drop_rate = 0.01) : drop_rate_(drop_rate), rng_(42) {}

    void send(std::vector<uint8_t> payload, std::chrono::milliseconds latency) {
        std::bernoulli_distribution drop(drop_rate_);
        if (drop(rng_)) return;
        Message msg{std::chrono::steady_clock::now() + latency, std::move(payload)};
        queue_.push(std::move(msg));
    }

    std::vector<std::vector<uint8_t>> receive_ready() {
        std::vector<std::vector<uint8_t>> out;
        auto now = std::chrono::steady_clock::now();
        while (!queue_.empty() && queue_.top().delivery <= now) {
            out.push_back(std::move(queue_.top().payload));
            queue_.pop();
        }
        return out;
    }

  private:
    struct Earlier {
        bool operator()(const Message &a, const Message &b) const { return a.delivery > b.delivery; }
    };

    double drop_rate_;
    std::mt19937 rng_;
    std::priority_queue<Message, std::vector<Message>, Earlier> queue_;
};

} // namespace ashos
