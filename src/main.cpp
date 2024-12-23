#include <SFML/Graphics.hpp>

#include "robot2d.hpp"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({480u, 480u}), "SFML Kinematics");
    sf::View view({0.0f, 0.0f}, {100.0f, 100.f});
    window.setView(view);

    Joint2d origin{M_PI / 4.0};
    Joint2d j1{M_PI / 4.0};
    Joint2d j2{M_PI / 4.0};

    Link2d l1{10.0};
    Link2d l2{10.0};
    Link2d l3{10.0};

    Robot2d robot({origin, j1, j2}, {l1, l2, l3});
    SFML_Robot2d sfml_robot(robot);

    sf::Clock clock;
    float elapsed_time = 0.0f;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        elapsed_time = clock.getElapsedTime().asSeconds();
        for (size_t i = 0; i < 3; ++i)
            sfml_robot.set_joint_angle(i, M_PI / 9.0 * elapsed_time);

        window.clear();
        window.draw(sfml_robot);
        window.display();
    }
}
