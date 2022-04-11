#include <SFML/Graphics.hpp>

void transform(int& x, int& y);
float deg2rad(float deg);
float rad2deg(float rad);
double direction(const sf::Vector2f base, const sf::Vector2f point);
double vectorLength(const sf::Vector2f vec);
sf::Vector2f normalizeVector(const sf::Vector2f vec);
sf::Vector2f rotateVecLeft90(const sf::Vector2f vec);
sf::Vector2f rotateVecRight90(const sf::Vector2f vec);