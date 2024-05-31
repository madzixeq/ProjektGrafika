#include <glm/fwd.hpp>
#include <map>
struct Character {
    unsigned int textureID; 
    float height;
    float width;
    float bearingX;
    float bearingY;
    unsigned int advance;   
};

std::map<char, Character> Characters;