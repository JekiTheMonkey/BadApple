#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Image.hpp>
#include <stdio.h>

#define HANDLE_ERROR(filePath) \
    do { fprintf(stderr, "%s not found\n", filePath); exit(1); } while(0)

float map(float valueToMap, float start1, float stop1, float start2, float stop2)
{
    return (valueToMap - start1) / (stop1 - start1) * (stop2 - start2) + start2;
}

char colorToChar(const sf::Color &color)
{
    static const char asciiGrandient[] = { ' ', ':', '+', '*', '?', '%', '$', 'S', '#', '@' };
    const float avgColor = (color.r + color.g + color.b) / 3.f;
    return asciiGrandient[static_cast<unsigned>(map(avgColor, 0, 255, 0, 10))];
}

int main(int argc, char **argv)
{
    char *filePath = (char*)malloc(sizeof(char) * 128);
    unsigned fileCounter = 0;
    const unsigned fileAmount = 5451;

    sf::Image image;

    sf::Clock clock;
    const sf::Time deltaTime = sf::seconds(1.f / 25);
    sf::Time timeSinceLastFrame = sf::Time::Zero;

    // It is possible to pass as argument a frame to start from
    for (unsigned fileCounter = (argc == 2 ? std::stoi(argv[1]) : 0); 
        fileCounter < fileAmount; ++fileCounter) 
    {
        // Make new frame be drawn only after deltaTime
        while (timeSinceLastFrame < deltaTime)
            timeSinceLastFrame += clock.restart();
        timeSinceLastFrame -= deltaTime;

        // Get file name
        sprintf(filePath, "../Assets/ba%04d.jpg", fileCounter);

        // Open image and handle possible errors
        if (!image.loadFromFile(filePath))
            HANDLE_ERROR(filePath);

        const sf::Vector2u imageSize = image.getSize();
        
        // Draw sprite
        for (unsigned row = 0; row < imageSize.y; ++row)
        {
            for (unsigned column = 0; column < imageSize.x; ++column)
            {
                putchar(colorToChar(image.getPixel(column, row)));
            }
            putchar(10);
        }
        printf("\033[12A");
    }

    free(filePath);
    return 0;
}