//
//  Immoveable.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 04/10/2020.
//

#include "Immoveable.hpp"
#define pi 3.141592654

void Immoveable::bounce(MyObject* wall, MyObject* ball){
    
    //angle of wall
    float fi = atan(abs(wall->variables.getPoint1().y - wall->variables.getPoint2().y) / abs(wall->variables.getPoint1().x - wall->variables.getPoint2().x));
    //angle of ball
    float alpha = atan(ball->variables.getVelocityY() / ball->variables.getVelocityX());
    
    
    float angleBetweenWallAndBall = angleBetweenLineAndDot(wall, ball, fi);

    //float beta;
    float exitAngle;
    //wall through second and third quadrant
    //if(fi >= 0 && fi <= pi / 2){
        //"above" the wall
        if(pi / 2 - fi <= 0.1){
            ball->variables.setVelocityX(-ball->variables.getVelocityX());
            return;
        }
        else if(angleBetweenWallAndBall > 0){
            //beta = fi - alpha;
            printf("1");
            exitAngle = 2 * fi - alpha;
        }
        //below the wall
        else{
            if(alpha > 0){
                printf("2");
                exitAngle = 2 * fi - alpha;
            }
            else{
                printf("3");
                alpha += 2 * pi;
                exitAngle = 2 * fi - alpha;
            }
        }
    //}
    
    float x, y, hipothenuse;
    hipothenuse = sqrt(pow(ball->variables.getVelocityX(), 2) + pow(ball->variables.getVelocityY(), 2));
    y = atan(exitAngle) * ball->variables.getVelocityX();
    x = y / atan(exitAngle);
    
    ball->variables.setVelocityX(x);
    ball->variables.setVelocityY(y);
}



float Immoveable::angleBetweenLineAndDot(MyObject* wall, MyObject* ball, float fi){
    float angle;
    float x, y;
    x = wall->variables.getPoint2().x - wall->variables.getPoint1().x + wall->variables.getPoint2().x - ball->variables.getPositionX();
    y = tan(fi) * x;
    
    if(y < ball->variables.getPositionY())
        angle = pi / 2 + fi;
    else
        angle = fi - pi / 2;
    
    return angle;
}
