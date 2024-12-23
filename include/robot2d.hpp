#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

struct Joint2d
{
    float angle;
};

struct Link2d
{
    float length;
};

class Robot2d
{
protected:
    std::vector<Joint2d> joints;
    std::vector<Link2d> links;

    std::vector<std::pair<float, float>> joint_positions;

    // Compute FK for 2D planar robot, gives every joint position
    void compute_joint_positions();

    // Get number of joints
    size_t get_joint_number() const;

    // Get number of links
    size_t get_link_number() const;

public:
    // Default constructor
    Robot2d() = default;

    // Constructor for a 2d planar robot
    Robot2d(const std::vector<Joint2d> &joints, const std::vector<Link2d> links);

    // Set joint angle, in radians
    void set_joint_angle(const size_t index, const float angle);

    // Print position and angle of each joint in the console, for debug purpose
    void show() const;
};

class SFML_Robot2d : public Robot2d, public sf::Drawable, public sf::Transformable
{
private:
    std::vector<sf::CircleShape> sfml_joints;
    std::vector<sf::RectangleShape> sfml_links;

    // Draw robot on a window
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    // Update drawable elements (circles and rectangles)
    void update_drawables();

public:
    // Default constructor
    SFML_Robot2d() = default;

    // Constructor of a drawable Robot2d
    SFML_Robot2d(const Robot2d &robot);

    // Set joint angle
    void set_joint_angle(const size_t index, const float angle);
};