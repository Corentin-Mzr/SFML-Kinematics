#include <SFML/Graphics.hpp>

#include "robot2d.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1280, 720u}), "SFML Kinematics");
    window.setVerticalSyncEnabled(true);
    sf::View view({0.0f, 0.0f}, {160.0f, 90.f});
    window.setView(view);

    if (!ImGui::SFML::Init(window))
        throw std::runtime_error("Could not initialize ImGui");

    std::vector<float> joint_angles(3, 0.0f);
    std::vector<float> link_lengths(3, 10.0f);
    std::vector<Joint2d> joints{{joint_angles[0]}, {joint_angles[1]}, {joint_angles[2]}};
    std::vector<Link2d> links{{link_lengths[0]}, {link_lengths[1]},{link_lengths[2]}};
    Robot2d robot(joints, links);
    SFML_Robot2d sfml_robot(robot);
    
    // End effector is a joint (but has no angles)
    std::vector<std::array<float, 3>> joint_colors(4, {0, 1, 0});
    std::vector<std::array<float, 3>> link_colors(3, {1, 1, 1});

    sf::Clock imgui_clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
        }

        ImGui::SFML::Update(window, imgui_clock.restart());

        // Robot arms angles
        ImGui::Begin("Robot Parameters");
        ImGui::Text("Robot Joint Angles");
        for (size_t i = 0; i < 3; ++i)
        {

            std::string label = "Joint" + std::to_string(i) + " Angle";
            ImGui::SliderFloat(label.c_str(), &joint_angles[i], -M_PI, M_PI);
        }
        ImGui::End();

        // Robot arms lengths
        ImGui::Begin("Robot Parameters");
        ImGui::Text("Robot Link Lengths");
        for (size_t i = 0; i < 3; ++i)
        {
            std::string label = "Link" + std::to_string(i) + " Length";
            ImGui::SliderFloat(label.c_str(), &link_lengths[i], 0, 30.0f);
        }
        ImGui::End();

        // Robot joints and links colors
        ImGui::Begin("Robot Parameters");
        ImGui::Text("Robot Joint Colors");
        for (size_t i = 0; i < 4; ++i)
        {
            std::string label = "Joint" + std::to_string(i) + " Color";
            ImGui::ColorEdit3(label.c_str(), joint_colors[i].data());
        }
        ImGui::Text("Robot Link Colors");
        for (size_t i = 0; i < 3; ++i)
        {
            std::string label = "Link" + std::to_string(i) + " Color";
            ImGui::ColorEdit3(label.c_str(), link_colors[i].data());
        }
        ImGui::End();

        // Update robot shape and color
        for (size_t i = 0; i < 3; ++i)
        {
            sfml_robot.set_joint_angle(i, joint_angles[i]);
            sfml_robot.set_link_length(i, link_lengths[i]);

            sfml_robot.set_joint_color(i, joint_colors[i].data());
            sfml_robot.set_link_color(i, link_colors[i].data());
        }
        // End effector
        sfml_robot.set_joint_color(3, joint_colors[3].data());

        window.clear();
        window.draw(sfml_robot);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown(window);
    return 0;
}
