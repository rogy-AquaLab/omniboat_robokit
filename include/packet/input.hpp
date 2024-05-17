#ifndef OMNIBOAT_ROBOKIT_PACKET_INPUT_HPP
#define OMNIBOAT_ROBOKIT_PACKET_INPUT_HPP

#include <array>
#include <utility>

namespace packet {

struct InputValues {
    /**
     * @brief ジョイスティックの入力値 (0~1) (x, y)
     */
    std::pair<float, float> joy = {0, 0};

    /**
     * @brief ツマミの値 (0~1)
     */
    float volume = 0;

    /**
     * @brief ジャイロの値 [rad/s]
     */
    std::array<float, 3> gyro = {0, 0, 0};

    InputValues(
        const std::pair<float, float>& joy, const float& volume, const std::array<float, 3>& gyro);

    InputValues() = default;
    ~InputValues() = default;
    InputValues(const InputValues&) = default;
    auto operator=(const InputValues&) -> InputValues& = default;
    InputValues(InputValues&&) = default;
    auto operator=(InputValues&&) -> InputValues& = default;

    /**
     * @brief 値が適切かどうか判定する
     * @return true 適切
     * @return false 不適切; joy, volumeのいずれかが指定範囲にない
     */
    auto is_valid() const -> bool;
};

}  // namespace packet

#endif  // OMNIBOAT_ROBOKIT_PACKET_INPUT_HPP
