/**
 * .cpp file for the grSim motion controller.
 *
 * In the current state it is a bang-bang controller.
 *
 * It assumed the robot max acceleration is constant.
 *
 * Uses constant acceleration kinematics equations to
 * calculate changes in speed.
 *
 * See https://en.wikipedia.org/wiki/Bang%E2%80%93bang_control for more info
 */

#include "grsim_communication/motion_controller/motion_controller.h"

#include <algorithm>
#include <chrono>

#include "shared/constants.h"


MotionController::Velocity MotionController::bangBangVelocityController(
    const Robot robot, const Point dest, const double desired_final_speed,
    const Angle desired_final_orientation, const double delta_time)
{
    MotionController::Velocity robot_velocities;

    // if the change is time is somehow negative or zero, just return the current robot
    // velocity
    // TODO: Implement exception handling for negative time case
    // See issue #123
    if (delta_time <= 0)
    {
        robot_velocities.linear_velocity  = robot.velocity();
        robot_velocities.angular_velocity = robot.angularVelocity();
    }
    else
    {
        robot_velocities.linear_velocity = MotionController::determineLinearVelocity(
            robot, dest, desired_final_speed, delta_time);
        robot_velocities.angular_velocity = MotionController::determineAngularVelocity(
            robot, desired_final_orientation, delta_time);
    }

    return robot_velocities;
}

AngularVelocity MotionController::determineAngularVelocity(
    const Robot robot, const Angle desired_final_orientation, const double delta_time)
{
    // Calculate the angular difference between us and a goal
    Angle angle_difference = (desired_final_orientation - robot.orientation()).angleMod();

    // Calculate our desired additional turn rate based on a sqrt-esque profile
    // This allows us to rapidly bring the velocity to zero when we're near the
    // target angle
    double new_robot_angular_velocity_magnitude =
        std::sqrt(std::max<double>(
            (angle_difference.abs() - Angle::ofDegrees(0.2)).toRadians(), 0)) *
        3;
    // Cap our angular velocity at the robot's physical limit
    new_robot_angular_velocity_magnitude = std::min<double>(
        new_robot_angular_velocity_magnitude, ROBOT_MAX_ANG_SPEED_RAD_PER_SECOND);
    // Figure out the maximum acceleration the robot is physically capable of
    double max_additional_angular_velocity =
        ROBOT_MAX_ANG_ACCELERATION_RAD_PER_SECOND_SQUARED * delta_time;

    // Compute the final velocity by taking the minimum of the physical max additional
    // turn rate and our desired additional turn rate
    double new_angular_velocity =
        robot.angularVelocity().toRadians() +
        std::copysign(max_additional_angular_velocity, angle_difference.toRadians());
    new_angular_velocity =
        std::clamp<double>(new_angular_velocity, -new_robot_angular_velocity_magnitude,
                           new_robot_angular_velocity_magnitude);

    return AngularVelocity::ofRadians(new_angular_velocity);
}

Vector MotionController::determineLinearVelocity(const Robot robot, const Point dest,
                                                 const double desired_final_speed,
                                                 const double delta_time)
{
    // Figure out the maximum (physically possible) velocity we can add to the robot
    double max_magnitude_of_velocity_to_apply =
        ROBOT_MAX_ACCELERATION_METERS_PER_SECOND_SQUARED * delta_time;

    // Calculate a vector from the robot to the destination
    Vector vector_to_dest = dest - robot.position();

    // Calculate the component of our current velocity pointing towards the destination
    double magnitude_of_robot_velocity_towards_dest =
        robot.velocity().len() *
        (vector_to_dest.orientation() - robot.velocity().orientation()).cos();
    Vector robot_velocity_towards_dest =
        vector_to_dest.norm(magnitude_of_robot_velocity_towards_dest);

    // Calculate the vector required to counteract our movement perpendicular to the
    // vector towards the dest. This "pushes" us onto a velocity vector straight towards
    // the destination
    Vector robot_velocity_perpendicular_to_dest =
        robot.velocity() - robot_velocity_towards_dest;

    // Use the "remaining" velocity (based on physical limits) to move us along the vector
    // towards the destination
    double magnitude_additional_velocity_towards_dest =
        std::max<double>(0, max_magnitude_of_velocity_to_apply -
                                robot_velocity_perpendicular_to_dest.len());
    Vector additional_velocity_towards_dest =
        vector_to_dest.norm(magnitude_additional_velocity_towards_dest);

    // The resulting additional velocity to apply to the robot is the velocity required to
    // counteract our movement perpendicular to the vector towards the destination, with
    // the "remaining" velocity pushing us along the vector to the destination
    Vector additional_velocity =
        -robot_velocity_perpendicular_to_dest + additional_velocity_towards_dest;

    // Clamp the new robot velocity to the minimum of the physical maximum velocity and
    // a square root-esque function of the distance to the destination. This allows us to
    // bring the robot velocity to zero as we approach the destination
    double new_robot_velocity_magnitude =
        std::sqrt(std::max<double>((dest - robot.position()).len() - 0.01, 0)) * 2 +
        desired_final_speed;
    new_robot_velocity_magnitude = std::clamp<double>(new_robot_velocity_magnitude, 0,
                                                      ROBOT_MAX_SPEED_METERS_PER_SECOND);
    Vector new_robot_velocity =
        (robot.velocity() + additional_velocity).norm(new_robot_velocity_magnitude);

    // Translate velocities into robot coordinates
    Vector new_robot_velocity_in_robot_coordinates =
        new_robot_velocity.rotate(-robot.orientation());

    return new_robot_velocity_in_robot_coordinates;
}
