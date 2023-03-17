#ifndef OMNIBOAT_R_INCLUDED
#define OMNIBOAT_R_INCLUDED

#include<stdio.h>
#include<math.h>
#include "mbed.h"
#include "MPU6050.h"

#define schneider_PI 3.141592653589

namespace omniboat{
    
    const static float I = 1;
    const static float e = 0.01;
    const static float a = 0.1;
    const static int trial_num = 1000;//試行回数

    class Schneider{
    public:
        Schneider();
        ~Schneider();
        void debug();
        void one_step();
        void init();
        void flip_shneider();
        void led(int num);//点灯
        
    private:
        float phi;// 機体の姿勢
        float gyro[3];//ジャイロ入力
        float gyro_buf[3];
        float acc[3];
        void ticker_flip();
        
        float t_jacobianmatrix[4][3];// ヤコビ行列
        float x_d[3];// ジョイスティックからの入力
        float q[4];// 更新される入力
        float x[3];// qに対しての出力
        float volume_;
        
        void cal_tjacob();//転置したヤコビ行列の計算
        void state_equation();//機体の状態の計算
        void cal_q();//入力の推定と更新
        void set_q();//モーターへの入力
        void joy_read(float joy_x, float joy_y, int rotate);
        void rotate();
        
    private:
        AnalogIn adcIn1;//ジョイスティック
        AnalogIn adcIn2;//ジョイスティック
        AnalogIn volume;
        MPU6050 mpu;
        
        PwmOut servo_1;
        PwmOut servo_2;
        PwmOut fet_1;
        PwmOut fet_2;
        
        DigitalOut led1;
        DigitalOut led2;
        DigitalOut led3;
        
        Ticker flip;
        
        DigitalIn button;
        BufferedSerial pc;
    };
}


#endif
