#include <SFML/Graphics.hpp>
#include <cmath>

float deg2rad(float deg){
    return deg / 360 * 2 * M_PI;
}

float rad2deg(float rad){
    return rad * 180 / M_PI;
}

double direction(const sf::Vector2f base, const sf::Vector2f point){
    double ans = atan2(point.y - base.y, point.x - base.x);
    if (ans < 0)
        return ans + 360;
    return ans;
}

double vectorLength(const sf::Vector2f vec){
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f normalizeVector(const sf::Vector2f vec){
    float len = vectorLength(vec);
    return {vec.x / len, vec.y / len};
}

sf::Vector2f rotateVecLeft90(const sf::Vector2f vec){
    // [0  -1]
    // [1  0]
    return {-vec.y, vec.x};
}

sf::Vector2f rotateVecRight90(const sf::Vector2f vec){
    // [0  1]
    // [-1  0]
    return {vec.y, -vec.x};
}