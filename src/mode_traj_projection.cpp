#include <mill/util/logger.hpp>
#include <mill/util/file_extension.hpp>
#include <mill/traj.hpp>
#include "mode_traj_projection.hpp"
#include <iostream>

namespace mill
{

const char* mode_traj_projection_usage() noexcept
{
    return "usage: mill traj projection [trajfile] [axes] [origin]\n"
           "       project trajectory onto a 3N-dimensional vector (axes).\n"
           "       To pass a 3N dimensional vector, trajectory file can be used.\n"
           "       The file should have the same number of elements as the traj file.\n";
}

int mode_traj_projection(std::deque<std::string_view> args)
{
    using namespace std::literals::string_view_literals;
    using namespace std::literals::string_literals;
    if(args.empty())
    {
        log::error("mill traj projection: too few arguments");
        log::error(mode_traj_projection_usage());
        return 1;
    }

    const auto traj = args.front(); args.pop_front();

    if(traj == "help")
    {
        log::info(mode_traj_projection_usage());
        return 0;
    }
    else if(args.size() < 2)
    {
        log::error("mill traj projection: too few arguments");
        log::info(mode_traj_projection_usage());
        return 1;
    }

    const auto axes_file = args.front(); args.pop_front();
    const auto orig_file = args.front(); args.pop_front();

    const auto origin_traj = reader(orig_file).read();
    const auto axes        = reader(axes_file).read();

    if(1 < origin_traj.size())
    {
        log::warn("mill traj projection: origin file (", orig_file, ") has ",
                  "multiple (", origin_traj.size(), ") number of structures. Only "
                  "the first frame is used as the origin.");
        return 1;
    }
    const auto origin = origin_traj.at(0);

    const auto output = std::string(base_name_of(traj)) + "_projected_"s +
        std::string(extension_of(traj));

    std::ofstream ofs(output);
    if(!ofs.good())
    {
        log::error("mill traj projection: file open error -> ", output);
        return 1;
    }

    ofs << "# [frame index], [coordinate along 1st axis], [coordinate along 2nd axis], ...\n";
    std::size_t t = 0;
    for(auto frame : reader(traj))
    {
        ofs << std::setw(8) << std::right << t ;
        for(const auto& axis : axes)
        {
            if(frame.size() != axis.size())
            {
                log::fatal("mill traj projection: number of particles in a frame"
                           " differs from that of the axis");
            }
            if(frame.size() != origin.size())
            {
                log::fatal("mill traj projection: number of particles in a frame"
                           " differs from that of the origin");
            }

            double coord = 0;
            for(std::size_t i=0; i<frame.size(); ++i)
            {
                coord += mill::dot_product(frame.at(i).position() - origin.at(i).position(), axis.at(i).position());
            }
            ofs << ' ' << std::setprecision(10) << coord;
        }
        ofs << '\n';
    }
    return 0;
}

} // mill
