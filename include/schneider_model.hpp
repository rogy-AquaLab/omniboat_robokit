#ifndef OMNIBOAT_R_INCLUDED
#define OMNIBOAT_R_INCLUDED

#include <array>

#include "mbed.h"

#include "device/input.hpp"
#include "device/output.hpp"
#include "packet/input.hpp"
#include "packet/output.hpp"

#ifdef USE_SIMPLE_SERVICE

#include "service/simple_service.hpp"

#else  // USE_SIMPLE_SERVICE

#include "service/service.hpp"

#endif  // USE_SIMPLE_SERVICE

namespace omniboat {

/**
 * @brief モータへの出力を計算するクラス
 */
class Schneider {
public:
    Schneider();
    Schneider(const Schneider&) = delete;
    Schneider(Schneider&&) = default;
    auto operator=(const Schneider&) -> Schneider& = delete;
    auto operator=(Schneider&&) -> Schneider& = default;
    ~Schneider();
    void debug();

    /**
     * @brief 1ステップ毎にモータへの入力値を計算する関数
     */
    void one_step();
    void init();

    // TODO:
    // これ実装されてないのでコメントアウト
    // commit `f50fb9b` 参照
    // /**
    //  * @brief 機体を停止させる関数
    //  */
    // void flip_shneider();

private:
    // flip_shneider と同様
    // /**
    //  * @brief ボタンが押されたときに機体を停止させる関数(割り込み処理)
    //  */
    // void ticker_flip();

    device::InputModules input_modules;
    device::OutputModules output_modules;

#ifdef USE_SIMPLE_SERVICE

    service::SimpleService service;

#else  // USE_SIMPLE_SERVICE

    service::Service service;

#endif  // USE_SIMPLE_SERVICE
};

}  // namespace omniboat

#endif
