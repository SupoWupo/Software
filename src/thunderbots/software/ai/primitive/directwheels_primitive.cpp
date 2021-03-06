#include "ai/primitive/directwheels_primitive.h"

#include "ai/primitive/visitor/primitive_visitor.h"

const std::string DirectWheelsPrimitive::PRIMITIVE_NAME = "DirectWheels Primitive";

DirectWheelsPrimitive::DirectWheelsPrimitive(unsigned int robot_id, int wheel0_power,
                                             int wheel1_power, int wheel2_power,
                                             int wheel3_power, double dribbler_rpm)
    : robot_id(robot_id),
      wheel0_power(wheel0_power),
      wheel1_power(wheel1_power),
      wheel2_power(wheel2_power),
      wheel3_power(wheel3_power),
      dribbler_rpm(dribbler_rpm)
{
}

DirectWheelsPrimitive::DirectWheelsPrimitive(
    const thunderbots_msgs::Primitive &primitive_msg)
{
    validatePrimitiveMessage(primitive_msg, getPrimitiveName());

    robot_id     = primitive_msg.robot_id;
    wheel0_power = int(primitive_msg.parameters.at(0));
    wheel1_power = int(primitive_msg.parameters.at(1));
    wheel2_power = int(primitive_msg.parameters.at(2));
    wheel3_power = int(primitive_msg.parameters.at(3));
    dribbler_rpm = primitive_msg.parameters.at(4);
}

std::string DirectWheelsPrimitive::getPrimitiveName() const
{
    return PRIMITIVE_NAME;
}

unsigned int DirectWheelsPrimitive::getRobotId() const
{
    return robot_id;
}

int DirectWheelsPrimitive::getWheel0Power() const
{
    return wheel0_power;
}

int DirectWheelsPrimitive::getWheel1Power() const
{
    return wheel1_power;
}

int DirectWheelsPrimitive::getWheel2Power() const
{
    return wheel2_power;
}

int DirectWheelsPrimitive::getWheel3Power() const
{
    return wheel3_power;
}

double DirectWheelsPrimitive::getDribblerRPM() const
{
    return dribbler_rpm;
}

std::vector<double> DirectWheelsPrimitive::getParameters() const
{
    std::vector<double> parameters = {double(wheel0_power), double(wheel1_power),
                                      double(wheel2_power), double(wheel3_power),
                                      dribbler_rpm};

    return parameters;
}

std::vector<bool> DirectWheelsPrimitive::getExtraBits() const
{
    return std::vector<bool>();
}

void DirectWheelsPrimitive::accept(PrimitiveVisitor &visitor) const
{
    visitor.visit(*this);
}
