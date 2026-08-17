#include<SFML/Graphics.hpp>
#include <vector>
 const int Gx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
 const int Gy[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
int main()
{
	sf::Image image("Assets/Vansh0.jpeg");
	sf::Vector2u imageSize = image.getSize(); 
	int imageWidth = imageSize.x, imageHeight = imageSize.y;
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(960, 540)), "DrawFace");
    std::vector<std::vector<int>> greyBuffer(imageWidth, std::vector<int>(imageHeight)); // basically a matrix(just variable, non constant)
	for (int i = 0; i < imageWidth; i++)
	{
		for (int j = 0; j < imageHeight; j++)
		{
			sf::Color pixel = image.getPixel(sf::Vector2u(i, j));

            greyBuffer[i][j] = static_cast<int>(0.299f * pixel.r + 0.587f * pixel.g + 0.114f * pixel.b); // human eye perception bs
		}
	}
    sf::Image output;
    output.resize(imageSize, sf::Color::White);
    for (int x = 1; x < imageWidth - 1; x++) // ignores the one pixel(0th) for error free 3x3 grid(no out of bounds please)
    {
        for (int y = 1; y < imageHeight - 1; y++)
        {
            int sumX = 0;
            int sumY = 0;
            for (int kx = -1; kx <= 1; kx++) // -1 0 1 creates the 3x3 grid
            {
                for (int ky = -1; ky <= 1; ky++)
                {
                    int intensity = greyBuffer[x + kx][y + ky]; // value of intensity at grid coordinates

                    sumX += intensity * Gx[kx + 1][ky + 1]; // getting the sum after convolution 
                    sumY += intensity * Gy[kx + 1][ky + 1];
                }
            }
            int magnitude = static_cast<int>(std::sqrt(sumX * sumX + sumY * sumY)); // magnitude
            magnitude = std::clamp(magnitude, 0, 255);

            output.setPixel(sf::Vector2u(x, y), sf::Color(magnitude, magnitude, magnitude));
        }
    }
    sf::Texture texture(output);
    sf::Sprite faceSprite(texture);
    faceSprite.setOrigin(sf::Vector2f(texture.getSize().x / 2.f, texture.getSize().y / 2.f));
    faceSprite.setPosition(sf::Vector2f(480, 270));
    faceSprite.setScale(sf::Vector2f(0.5f, 0.5f));
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        window.clear();
        window.draw(faceSprite);
        window.display();
    }
	return 0;
}