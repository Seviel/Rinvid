#include <SFML/Window.hpp>

#include "core/include/rinvid_gfx.h"

void handle_events(sf::Window& window, sf::Event& event);

int main()
{
    sf::Window window(sf::VideoMode(800, 600), "Rinvid testing grounds");
    sf::Event  event;

    rinvid::RinvidGfx::set_viewport(0, 0, 800, 600);

    while (window.isOpen())
    {
        handle_events(window, event);
    }

    return 0;
}

void handle_events(sf::Window& window, sf::Event& event)
{
    while (window.pollEvent(event))
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