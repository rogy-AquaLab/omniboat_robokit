#include "packet/output.hpp"

#include "utils.hpp"

using float_pair = std::pair<float, float>;

// NOLINTBEGIN(bugprone-easily-swappable-parameters): FIXME
packet::OutputValues::OutputValues(const float_pair &servo, const float_pair &dc_motor) :
    servo(servo), dc_motor(dc_motor) {}
// NOLINTEND(bugprone-easily-swappable-parameters)

constexpr auto pi_range(float value) -> bool {
    return 0 <= value && value <= utils::PI;
}

constexpr auto dc_range(float value) -> bool {
    constexpr float DC_MAX = 1.0F;
    return 0 <= value && value <= DC_MAX;
}

auto packet::OutputValues::is_valid() const -> bool {
    const bool servo_valid = pi_range(this->servo.first) && pi_range(this->servo.second);
    const bool dc_valid = dc_range(this->dc_motor.first) && dc_range(this->dc_motor.second);
    return servo_valid && dc_valid;
}
