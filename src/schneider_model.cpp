#include <thread>
#include <chrono>
#include "rtos.h"
#include "schneider_model.h"

namespace omniboat{
    
    Schneider::Schneider():adcIn1(A4),adcIn2(A5),volume(A6),mpu(D4,D5),\
                            servo_1(PB_4),servo_2(PA_11),fet_1(PA_9),fet_2(PA_10),\
                            led1(PA_1),led2(PA_3),led3(PA_4),button(D7),pc(USBTX,USBRX){
        led(1);
        led(2);
        led(3);
        printf("start up\n");
        flip.attach_us(callback(this,&Schneider::ticker_flip),10000);
        
        servo_1.period_ms(20);
        servo_2.period_ms(20);
    }

    Schneider::~Schneider(){
        led(1);
    }
    
    void Schneider::init(){
        
        phi = 0;
        memset(t_jacobianmatrix,0,sizeof(t_jacobianmatrix));
        for(int i=0;i<4;i++)q[i] = 0.01;
        for(int i=0;i<3;i++)x[i] = 0;
        cal_tjacob();
    }
    
    void Schneider::one_step(){
        led(3);
        mpu.getGyro(gyro);
        joy_read(adcIn1.read(),adcIn2.read(),0.0);
        
        volume_ = volume.read();
        
        q[0] = 0;
        q[1] = 0;
        
        if(abs(x_d[0])>0.4f||abs(x_d[1])>0.4f){
            cal_q();
            set_q();
        }
        else if(volume_<0.4f||0.7f<volume_){
            rotate();
            phi = 0;
        }
        else{
            fet_1 = 0;
            fet_2 = 0;
            phi = 0;
        }
        debug();
        led(3);
    }

      
    void Schneider::debug(){
        //printf("phi=%f\n",phi);
        //printf("ave=%f\n",ave_[0]/ave_[1]);
        //printf("motor=%f,%f\n",q[0],q[1]);
        //printf("servo=%f,%f\n",q[2],q[3]);
        //printf("volume=%f\n",volume.read());
        //printf("gyro[2]=%f\n",gyro[2]);
        //printf("\n");
    }
    

    void Schneider::joy_read(float joy_x,float joy_y,int rotate){
        x_d[0] = (joy_x-0.5f)*2;
        x_d[1] = (joy_y-0.5f)*2;
        //x_d[2] = rotate;
        x_d[2] = 0;
        
    }
    
    
    void Schneider::flip_shneider(){
        printf("flip_shneider\n");
        fet_1 = 0;
        fet_2 = 0;
	rtos::ThisThread::sleep_for(5s);
        NVIC_SystemReset();
    }
    

    inline void Schneider::cal_tjacob(){
        t_jacobianmatrix[0][0] = cos(q[2]+phi);
        t_jacobianmatrix[0][1] = sin(q[2]+phi);
        t_jacobianmatrix[0][2] = (a+sin(q[2]))/I;
        t_jacobianmatrix[1][0] = cos(q[3]+phi);
        t_jacobianmatrix[1][1] = sin(q[3]+phi);
        t_jacobianmatrix[1][2] = (-a-sin(q[3]))/I;
        t_jacobianmatrix[2][0] = -q[0]*sin(q[2]+phi);
        t_jacobianmatrix[2][1] = q[0]*cos(q[2]+phi);
        t_jacobianmatrix[2][2] = q[0]*cos(q[2])/I;
        t_jacobianmatrix[3][0] = -q[1]*sin(q[3]+phi);
        t_jacobianmatrix[3][1] = q[1]*cos(q[3]+phi);
        t_jacobianmatrix[3][2] = -q[1]*cos(q[3])/I;
    }

    void Schneider::cal_q(){
        //初期値
        if(x_d[0]>=0&&x_d[1]>=0)for(int i=0;i<2;i++)q[i+2]=schneider_PI/4-phi;
        else if(x_d[0]>=0&&x_d[1]<0)for(int i=0;i<2;i++)q[i+2]=-schneider_PI/4-phi;
        else if(x_d[0]<0&&x_d[1]>=0)for(int i=0;i<2;i++)q[i+2]=3*schneider_PI/4-phi;
        else for(int i=0;i<2;i++)q[i+2]=5*schneider_PI/4-phi;
        
        
        led(2);
        for(int i=0;i<trial_num;i++){
            state_equation();
            
            double diff = pow(x[0]-x_d[0],2)+pow(x[1]-x_d[1],2)+pow(x[2]-x_d[2],2);
            if(diff<1e-3)break;
            
            cal_tjacob();
            for(int j=0;j<4;j++){
                for(int k=0;k<3;k++){
                    q[j] -= e*t_jacobianmatrix[j][k]*(x[k]-x_d[k]);
                }
                if(j==0||j==1)q[j] -= pow((2*q[j]-1),7);
            }
        }
        led(2);
    }

    inline void Schneider::state_equation(){
        x[0] = q[0]*cos(q[2]+phi)+q[1]*cos(q[3]+phi);
        x[1] = q[0]*sin(q[2]+phi)+q[1]*sin(q[3]+phi);
        x[2] = (a*(q[0]-q[1])+q[0]*sin(q[2])-q[1]*sin(q[3]))/I;
    }
    
    void Schneider::set_q(){
        if(abs(q[0]<=0.4f))q[0]=0;
        if(abs(q[1]<=0.4f))q[1]=0;
        fet_1 = q[0];
        fet_2 = q[1];
    
        while(q[2]>=(float)schneider_PI)q[2]-=2*(float)schneider_PI;
        while(q[3]>=(float)schneider_PI)q[3]-=2*(float)schneider_PI;
        while(q[2]<-schneider_PI)q[2]+=2*schneider_PI;
        while(q[3]<-schneider_PI)q[3]+=2*schneider_PI;
        
        if(0<q[2]&&q[2]<(float)schneider_PI)servo_1.pulsewidth_us(500+1900/schneider_PI*q[2]-2200*gyro[2]);
        if(0<q[3]&&q[3]<(float)schneider_PI)servo_2.pulsewidth_us(500+1900/schneider_PI*q[3]+2200*gyro[2]);
    }
    void Schneider::rotate(){
        fet_1 = 0.5;
        fet_2 = 0.5;
        if(volume_<0.5f){
            servo_1.pulsewidth_us(550);
            servo_2.pulsewidth_us(2350);
        }
        else{
            servo_2.pulsewidth_us(550);
            servo_1.pulsewidth_us(2350);
        }
    }
    
    void Schneider::led(int num){
        switch(num){
            case 1: led1 = !led1;break;
            case 2: led2 = !led2;break;
            case 3: led3 = !led3;break;
            default:break;
        }
    }
    
    void Schneider::ticker_flip(){
        if(button==0)flip_shneider();
    }
    
    
}
