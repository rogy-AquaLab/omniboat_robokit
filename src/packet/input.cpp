#include "packet/input.hpp"

// NOLINTBEGIN(bugprone-easily-swappable-parameters): FIXME
packet::InputValues::InputValues(
    const std::pair<float, float> &joy, const float &volume, const std::array<float, 3> &gyro) :
    joy(joy), volume(volume), gyro(gyro) {}
// NOLINTEND(bugprone-easily-swappable-parameters)

constexpr auto in_range(const float &value) -> bool {
    return 0 <= value && value <= 1;
}

auto packet::InputValues::is_valid() const -> bool {
    const bool joy_valid = in_range(this->joy.first) && in_range(this->joy.second);
    const bool volume_valid = in_range(this->volume);
    return joy_valid && volume_valid;
}
