#ifndef OMNIBOAT_ROBOKIT_SIMPLE_SERVICE_HPP
#define OMNIBOAT_ROBOKIT_SIMPLE_SERVICE_HPP

#include "packet/input.hpp"
#include "packet/output.hpp"

namespace service {

class SimpleService {
public:
    SimpleService();
    auto init() -> void;
    auto call(const packet::InputValues& input) -> packet::OutputValues;

public:
    // input joy -> (bldc powers, direction)
    auto xy2polar(const std::pair<float, float>& joy) -> std::pair<float, float>;

    // input volume -> (bldc power 1, bldc power 2)
    auto vol2rotate(const float& volume) -> std::pair<float, float>;

    // (bldc powers, direction), (bldc power1, bldc power2)
    // -> (bldc power 1, bldc power2), (servo 1, servo 2)
    auto synthesis_polar_rotate(
        const std::pair<float, float>& polar, const std::pair<float, float>& rotate)
        -> std::pair<std::pair<float, float>, std::pair<float, float>>;
};

}  // namespace service

#endif  // OMNIBOAT_ROBOKIT_SIMPLE_SERVICE_HPP
