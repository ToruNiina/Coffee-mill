#include <mill/util/logger.hpp>
#include <mill/util/file_extension.hpp>
#include <mill/traj.hpp>
#include <mill/math/Constants.hpp>
#include "mode_traj_rotate.hpp"
#include <iostream>

namespace mill
{

const char* mode_traj_rotate_usage() noexcept
{
    return "usage: mill traj rotate [trajfile] [x|y|z] [angle(degree)]\n"
           "       rotate the snapshot around x, y, or z axis by a specified angle.\n";
           "     - mill traj rotate [trajfile] [vec x y z] [angle(degree)]\n"
           "       rotate the snapshot around the axis\n";
}

int mode_traj_rotate(std::deque<std::string_view> args)
{
    using namespace std::literals::string_view_literals;
    using namespace std::literals::string_literals;
    if(args.empty())
    {
        log::error("mill traj rotate: too few arguments");
        log::error(mode_traj_rotate_usage());
        return 1;
    }

    const auto input = args.front();
    args.pop_front();

    if(input == "help")
    {
        log::info(mode_traj_rotate_usage());
        return 0;
    }

    if(args.size() != 2)
    {
        log::error("mill traj rotate: too few arguments");
        log::error(mode_traj_rotate_usage());
        return 1;
    }

    const auto axis  = args.front();

    auto rot = Matrix<double, 3, 3>::zero();
    if(axis == "x"sv)
    {
        const auto theta = std::stod(std::string(args.back())) * constants::pi / 180.0;

        const auto cos_theta = std::cos(theta);
        const auto sin_theta = std::sin(theta);

        rot(0, 0) = 1.0;
        rot(1, 1) =  cos_theta;
        rot(1, 2) = -sin_theta;
        rot(2, 1) =  sin_theta;
        rot(2, 2) =  cos_theta;
    }
    else if(axis == "y"sv)
    {
        const auto theta = std::stod(std::string(args.back())) * constants::pi / 180.0;

        const auto cos_theta = std::cos(theta);
        const auto sin_theta = std::sin(theta);

        rot(0, 0) =  cos_theta;
        rot(0, 2) =  sin_theta;
        rot(1, 1) = 1.0;
        rot(2, 0) = -sin_theta;
        rot(2, 2) =  cos_theta;
    }
    else if(axis == "z"sv)
    {
        const auto theta = std::stod(std::string(args.back())) * constants::pi / 180.0;

        const auto cos_theta = std::cos(theta);
        const auto sin_theta = std::sin(theta);

        rot(0, 0) =  cos_theta;
        rot(0, 1) = -sin_theta;
        rot(1, 0) =  sin_theta;
        rot(1, 1) =  cos_theta;
        rot(2, 2) = 1.0;
    }
    else if(axis == "vec"sv)
    {
        Vector<double, 3> n;

        n[0] = std::stod(std::string(args.front())); args.pop_front();
        n[1] = std::stod(std::string(args.front())); args.pop_front();
        n[2] = std::stod(std::string(args.front())); args.pop_front();

        const auto theta = std::stod(std::string(args.back())) * constants::pi / 180.0;
        const auto sin_t = std::sin(theta);
        const auto cos_t = std::cos(theta);

        rot(0, 0) = n[0] * n[0] * (1.0 - cos_t) +        cos_t;
        rot(0, 1) = n[0] * n[1] * (1.0 - cos_t) - n[2] * sin_t;
        rot(0, 2) = n[0] * n[2] * (1.0 - cos_t) + n[1] * sin_t;

        rot(1, 0) = n[1] * n[0] * (1.0 - cos_t) + n[2] * sin_t;
        rot(1, 1) = n[1] * n[1] * (1.0 - cos_t) +        cos_t;
        rot(1, 2) = n[1] * n[2] * (1.0 - cos_t) - n[0] * sin_t;

        rot(2, 0) = n[2] * n[0] * (1.0 - cos_t) - n[1] * sin_t;
        rot(2, 1) = n[2] * n[1] * (1.0 - cos_t) + n[0] * sin_t;
        rot(2, 2) = n[2] * n[2] * (1.0 - cos_t) +        cos_t;
    }
    else
    {
        log::error("unknown axis: ", axis);
        log::error(mode_traj_rotate_usage());
        return 1;
    }

    const std::string output = std::string(base_name_of(input)) +
        "_rotated_"s + std::string(axis) + std::string(args.back()) +
        std::string(extension_of(input));

    auto w = writer(output);
    auto r = reader(input);

    w.write_header(r.read_header());

    for(auto frame : r)
    {
        Vector<double, 3> center(0.0, 0.0, 0.0);
        for(const auto& p : frame)
        {
            center += p.position();
        }
        center /= frame.size();

        for(auto& p : frame)
        {
            p.position() -= center;
            p.position()  = rot * p.position();
            p.position() += center;
        }
        w.write_frame(frame);
    }
    return 0;
}

} // mill
