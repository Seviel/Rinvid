#include <SFML/Window.hpp>

int main()
{
    sf::Window window(sf::VideoMode(800, 600), "Rinvid testing grounds");
    sf::Event event;

    while (window.isOpen())
    {
        if (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }
    }
}
