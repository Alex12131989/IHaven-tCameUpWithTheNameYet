#pragma once
#include <cmath>
#include <array>

#include "CONSTANTS.h"

template <typename T>
int Int(T num, unsigned int round_option)
{
    switch (round_option)
    {
    case CAST:
	    return static_cast<int>(num);
    case ROUND:
	    return static_cast<int>(std::round(num));
    case FLOOR:
	    return static_cast<int>(std::floor(num));
    case CEIL:
	    return static_cast<int>(std::ceil(num));
    default:
	    throw std::invalid_argument("Wrong rounding option");
    }
}
template <typename T>
int Int(T num)
{
    return static_cast<int>(num);
}


Color static EditColor(Color ogColor, float multiplicantR, float multiplicantG, float multiplicantB, float multiplicantA)
{
    return
    { 
        static_cast<unsigned char>(ogColor.r*multiplicantR), 
        static_cast<unsigned char>(ogColor.g*multiplicantG), 
        static_cast<unsigned char>(ogColor.b*multiplicantB), 
        static_cast<unsigned char>(ogColor.a*multiplicantA)
    };
}
Color static EditColor(Color ogColor, float multiplicant)
{
    return
    {
         static_cast<unsigned char>(ogColor.r*multiplicant),
         static_cast<unsigned char>(ogColor.g*multiplicant),
         static_cast<unsigned char>(ogColor.b*multiplicant),
         ogColor.a
    };
}
Color static ChangeColorOpacity(Color ogColor, float opacity)
{
    if ((opacity <= 1) && (opacity >= 0))
        return { ogColor.r, ogColor.g, ogColor.b, static_cast<unsigned char>(ogColor.a*opacity) };
    else
        throw new std::invalid_argument("Invalid opacity value");
}

Image static LoadSprite(std::string address, Vector2 size)
{
    if ((size.x == -1) && (size.y == -1))
        return LoadImage(address.data());
    else
    {
        Image sprite = LoadImage(address.data());
        if (size.x == -1)
            ImageResize(&sprite, sprite.height, Int(size.y, FLOOR));
        else if (size.y == -1)
            ImageResize(&sprite, Int(size.x, FLOOR), sprite.width);
        else
            ImageResize(&sprite, Int(size.x, FLOOR), Int(size.y, FLOOR));
        return sprite;
    }
}

void static DecomposeSprite(Image sprite, Vector2 singleImageSize, std::array<Image, TILEMAP_SIZE>& frames)
{
    size_t index = 0;
    for (float i = 0; i < sprite.height; i += singleImageSize.y)
        for (float j = 0; j < sprite.width; j += singleImageSize.x)
            if (!((i == 7*singleImageSize.y) && (j > 1*singleImageSize.x )))
            {
                Image frame = ImageFromImage(sprite, { static_cast<float>(j), static_cast<float>(i), singleImageSize.x, singleImageSize.y });
                frames[index++] = frame;
            }
}

void static GetTextures(std::string address, Vector2 size, Vector2 induvidualTextureSize, std::array<Texture2D, TILEMAP_SIZE>& textures)
{
    Image sprite = LoadSprite(address, size);
    std::array<Image, TILEMAP_SIZE> frames;
    DecomposeSprite(sprite, induvidualTextureSize, frames);
    for (size_t i = 0; i < textures.size(); ++i)
        textures[i] = LoadTextureFromImage(frames[i]);
}

bool static operator==(const Color c1, const Color c2)
{
    if ((c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c1.b) && (c1.a == c2.a))
        return true;
    return false;
}

bool static operator!=(const Color c1, const Color c2)
{
    if ((c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c1.b) && (c1.a == c2.a))
        return true;
    return false;
}
