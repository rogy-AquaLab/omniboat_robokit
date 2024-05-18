#ifndef OMNIBOAT_ROBOKIT_PACKET_OUTPUT_HPP
#define OMNIBOAT_ROBOKIT_PACKET_OUTPUT_HPP

#include <array>
#include <utility>

namespace packet {

struct OutputValues {
    /**
     * @brief サーボモーターの角度 (0~pi)
     * @note piの値はutils::PI
     */
    std::pair<float, float> servo = {0, 0};

    /**
     * @brief DCモーターの出力 (0~1)
     * @note first, secondはservoのfirst, secondと対応させること
     * @details 0で停止, 1で最大出力
     */
    std::pair<float, float> dc_motor = {0, 0};

    /**
     * @brief Construct a new Output Values object
     * @param servo サーボモーターの角度
     * @param dc_motor DCモーターの出力
     */
    OutputValues(const std::pair<float, float> &servo, const std::pair<float, float> &dc_motor);

    OutputValues() = default;
    ~OutputValues() = default;
    OutputValues(const OutputValues &) = default;
    auto operator=(const OutputValues &) -> OutputValues & = default;
    OutputValues(OutputValues &&) = default;
    auto operator=(OutputValues &&) -> OutputValues & = default;

    /**
     * @brief 値が適切かどうかを判定する
     * @return true 適切
     * @return false 不適切; servo, dc_motorのいずれかが指定された範囲にない
     */
    auto is_valid() const -> bool;
};

}  // namespace packet

#endif  // OMNIBOAT_ROBOKIT_PACKET_OUTPUT_HPP
