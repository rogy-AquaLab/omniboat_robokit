#ifndef OMNIBOAT_R_INCLUDED
#define OMNIBOAT_R_INCLUDED

#include <array>

#include "mbed.h"

#include "device/input.hpp"
#include "device/output.hpp"
#include "packet/input.hpp"
#include "packet/output.hpp"
#include "service/service.hpp"

namespace omniboat {

/**
 * @brief モータへの出力を計算するクラス
 */
class Machine {
public:
    Machine();
    Machine(const Machine&) = delete;
    Machine(Machine&&) = default;
    auto operator=(const Machine&) -> Machine& = delete;
    auto operator=(Machine&&) -> Machine& = default;
    ~Machine();
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
    service::Service service;
};

}  // namespace omniboat

#endif
