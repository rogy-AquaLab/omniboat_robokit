#include <algorithm>
#include <cmath>

#include "service/simple_service.hpp"

service::SimpleService::SimpleService() {}

auto service::SimpleService::init() -> void {}

auto service::SimpleService::call(const packet::InputValues& input) -> packet::OutputValues {
    const std::pair<float, float> polar = this->xy2polar(input.joy);
    const std::pair<float, float> rotate = this->vol2rotate(input.volume);
    const auto out = this->synthesis_polar_rotate(polar, rotate);
    return packet::OutputValues{out.second, out.first};
}

auto service::SimpleService::xy2polar(const std::pair<float, float>& joy)
    -> std::pair<float, float> {
    const double x = (joy.first - 0.5) * 2;
    const double y = (joy.second - 0.5) * 2;
    double mag = std::max(std::sqrt(x * x + y * y), 1.0);
    double angle = std::atan2(y, x);
    return {static_cast<double>(mag), static_cast<double>(angle)};
}

auto service::SimpleService::vol2rotate(const float& volume) -> std::pair<float, float> {
    // NOTE: ここにジャイロに基づいた回転を入れられるかも
    const double v = (volume - 0.5) * 2;
    // FIXME: 符号はテキトー
    return {v, -v};
}

auto service::SimpleService::synthesis_polar_rotate(
    const std::pair<float, float>& polar, const std::pair<float, float>& rotate)
    -> std::pair<std::pair<float, float>, std::pair<float, float>> {
    float bldc_power1 = polar.first + rotate.first;
    float bldc_power2 = polar.first + rotate.second;
    float servo1 = polar.second;
    float servo2 = polar.second;
    {
        // powerが -1~1 を超えないように
        using std::abs;
        using std::max;
        const float abs_power_max = max(abs(bldc_power1), abs(bldc_power2));
        if (abs_power_max > 1.0) {
            const float reduce_scale = 1.0 / abs_power_max;
            bldc_power1 *= reduce_scale;
            bldc_power2 *= reduce_scale;
        }
    }
    {
        // servo1 が 0~pi を超えないように
        // FIXME: -pi~0 の範囲にサーボを回せない かつ BLDCが逆回転できないがどうする?
        if (servo1 < 0) {
            bldc_power1 = 0;
            servo1 = 0;
        }
        if (servo2 < 0) {
            bldc_power2 = 0;
            servo2 = 0;
        }
    }
    return {{bldc_power1, bldc_power2}, {servo1, servo2}};
}
