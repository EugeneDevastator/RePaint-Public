//
// Created by Eugene on 12/17/2023.
//

#ifndef SOURCE_REMATH_HPP
#define SOURCE_REMATH_HPP
inline float AtanXY(float x, float y) {
    double ang;
    signed int sg;
    sg = -1;
    if (y > 0)
        sg = 0;
    if (x == 0 && y == 0)
        ang = 0;
    else if (x == 0)
        ang = M_PI_2 + (sg + 1) / 2 * M_PI;
    else if (x > 0 && y < 0)
        ang = M_PI * 2 + atan(y / x);
    else if (x < 0)
        ang = M_PI + atan(y / x);
    else
        ang = atan(y / x);

    // ang=ang*180/M_PI;
    return ang - M_PI;
}

#endif //SOURCE_REMATH_HPP

