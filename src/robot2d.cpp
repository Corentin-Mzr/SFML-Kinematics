#include "robot2d.hpp"

/**************** ROBOT2D ****************/

// Constructor for a 2d planar robot
Robot2d::Robot2d(const std::vector<Joint2d> &joints, const std::vector<Link2d> links) : joints(joints), links(links)
{
    if (joints.size() != links.size())
        throw std::invalid_argument("Number of joints and links must match");

    // When creating the robot, compute all joint positions
    joint_positions.resize(joints.size() + 1, {0.0, 0.0});
    compute_joint_positions();
}

// Compute FK for 2D planar robot, gives every joint position
void Robot2d::compute_joint_positions()
{
    float x = 0.0, y = 0.0, sum_angle = 0.0;

    for (size_t i = 0; i < joints.size(); ++i)
    {
        sum_angle += joints[i].angle;

        x += links[i].length * cos(sum_angle);
        y += links[i].length * sin(sum_angle);

        // Position of joint i
        joint_positions[i + 1] = {x, y};
    }
}

// Set joint angle, in radians
void Robot2d::set_joint_angle(const size_t index, const float angle)
{
    if (index >= joints.size())
        throw std::out_of_range("Invalid joint index");
    joints[index].angle = angle;

    // Recompute joint positions
    compute_joint_positions();
}

// Set link length, in meters
void Robot2d::set_link_length(const size_t index, const float length)
{
    if (index >= links.size())
        throw std::out_of_range("Invalid link index");
    else if (length < 0)
        throw std::out_of_range("Link length must be positive");

    links[index].length = length;

    // Recompute joint positions
    compute_joint_positions();
}

// Print position and angle of each joint in the console, for debug purpose
void Robot2d::show() const
{
    for (size_t i = 0; i < joints.size(); ++i)
    {
        const auto [x, y] = joint_positions[i];
        std::cout << "Joint" << i << " | (x,y) = (" << x << ", " << y << ") | theta = " << joints[i].angle << " radians\n";
    }

    const size_t last = joints.size();
    const auto [x, y] = joint_positions[last];
    std::cout << "Joint" << last << " | (x,y) = (" << x << ", " << y << ")\n";
}

// Get number of joints
size_t Robot2d::get_joint_number() const
{
    return joints.size();
}

// Get number of links
size_t Robot2d::get_link_number() const
{
    return links.size();
}

/**************** SFML ROBOT2D ****************/

// Constructor of a drawable Robot2d
SFML_Robot2d::SFML_Robot2d(const Robot2d &robot) : Robot2d(robot)
{
    // Initial robot shape and color
    const float radius = 1.0;
    sf::CircleShape joint(radius);
    joint.setOrigin({radius, radius});
    joint.setFillColor(sf::Color::Green);

    const float width = 1.0;
    sf::RectangleShape link({10.0, width});
    link.setOrigin({0.0, width / 2.0f});
    link.setFillColor(sf::Color::White);

    sfml_joints.resize(get_joint_number() + 1, joint);
    sfml_links.resize(get_link_number(), link);

    update_drawables();
}

// Draw robot on a window
void SFML_Robot2d::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (size_t i = 0; i < sfml_links.size(); ++i)
        target.draw(sfml_links[i], states);

    for (size_t i = 0; i < sfml_joints.size(); ++i)
        target.draw(sfml_joints[i], states);
}

// Update drawable elements (circles and rectangles)
void SFML_Robot2d::update_drawables()
{
    float width = 1.0;
    for (size_t i = 0; i < joint_positions.size(); ++i)
    {
        // Joint as a circle
        sfml_joints[i].setPosition({joint_positions[i].first, -joint_positions[i].second});

        // Link as a line
        if (i != joint_positions.size() - 1)
        {
            const auto &start = joint_positions[i];
            const auto &end = joint_positions[i + 1];

            const float dx = end.first - start.first;
            const float dy = end.second - start.second;

            const float length = links[i].length;
            const sf::Angle angle = sf::radians(atan2(dy, dx));

            sfml_links[i].setSize({length, width});
            sfml_links[i].setPosition({start.first, -start.second});
            sfml_links[i].setRotation(-angle);
        }
    }
}

// Set joint angle
void SFML_Robot2d::set_joint_angle(const size_t index, const float angle)
{
    Robot2d::set_joint_angle(index, angle);
    update_drawables();
}

// Set link length, in meters
void SFML_Robot2d::set_link_length(const size_t index, const float length)
{
    Robot2d::set_link_length(index, length);
    update_drawables();
}

// Set joint color
void SFML_Robot2d::set_joint_color(const size_t index, const sf::Color &color)
{
    if (index >= sfml_joints.size())
        throw std::out_of_range("Invalid joint index");
    sfml_joints[index].setFillColor(color);
    update_drawables();
}

void SFML_Robot2d::set_joint_color(const size_t index, const float color[3])
{
    sf::Color c{static_cast<uint8_t>(255 * color[0]), static_cast<uint8_t>(255 * color[1]), static_cast<uint8_t>(255 * color[2])};
    set_joint_color(index, c);
}

// Set link color
void SFML_Robot2d::set_link_color(const size_t index, const sf::Color &color)
{
    if (index >= sfml_links.size())
        throw std::out_of_range("Invalid link index");
    sfml_links[index].setFillColor(color);
    update_drawables();
}

void SFML_Robot2d::set_link_color(const size_t index, const float color[3])
{
    sf::Color c{static_cast<uint8_t>(255 * color[0]), static_cast<uint8_t>(255 * color[1]), static_cast<uint8_t>(255 * color[2])};
    set_link_color(index, c);
}