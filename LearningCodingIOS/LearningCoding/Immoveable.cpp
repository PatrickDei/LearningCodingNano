//
//  Immoveable.cpp
//  LearningCoding
//
//  Created by Patrick Deisinger on 04/10/2020.
//

#include "Immoveable.hpp"
#define pi 3.141592654

void Immoveable::bounce(MyObject* wall, MyObject* ball){
    float fi = atan(abs(wall->getPoint1().y - wall->getPoint2().y) / abs(wall->getPoint1().x - wall->getPoint2().x));
    if((wall->getPoint2().y - wall->getPoint1().y < 0 && wall->getPoint1().x - wall->getPoint2().x < 0) || (wall->getPoint2().y - wall->getPoint1().y > 0 && wall->getPoint1().x - wall->getPoint2().x > 0))
        fi = -fi;
    float alpha = atan(ball->getVelocityY() / ball->getVelocityX());
    if(ball->getVelocityX() < 0)
        alpha += pi;
    else if(ball->getVelocityY() < 0)
        alpha += 2 * pi;
    //float psi = pi / 2 - alpha - fi;
    
    //float exitAngle = fi + pi / 2 - psi;
    float exitAngle = 2 * fi - alpha;
    
    float sizeOfYVelocity = tan(exitAngle);
    
    float hipothenuse = sqrt(pow(ball->getVelocityX(), 2) + pow(ball->getVelocityY(), 2));
    
    float x = sqrt(abs(pow(hipothenuse, 2) - pow(sizeOfYVelocity * ball->getVelocityX(), 2)));
    float y = sqrt(abs(pow(hipothenuse, 2) - pow(x, 2)));
    printf("\nfi: %f alpha: %f psi: %f, size: %f, hipothenuse: %f\nx. %f y: %f\n%f", fi * 180 / pi, alpha * 180 / pi, exitAngle * 180 / pi, sizeOfYVelocity, hipothenuse, x, y, ball->getVelocityX());
    
    if(exitAngle > pi || exitAngle < 0)
        y = -y;
    if(exitAngle > pi / 2 || exitAngle < -pi / 2)
        x = -x;

    //ball->setVelocityY(y);
    //ball->setVelocityX(x);
    if(fi == 0)
        ball->setVelocityY(-ball->getVelocityY());
    else
        ball->setVelocityX(-ball->getVelocityX());

}
