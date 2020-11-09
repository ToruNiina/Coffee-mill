#include "mode_calc_obb.hpp"

#include <mill/math.hpp>
#include <mill/traj.hpp>
#include <toml/toml.hpp>
#include <numeric>

namespace mill
{

const char* mode_calc_obb_usage() noexcept
{
    return "usage: mill calc obb <trajfile>\n"
           "       calculates oriented bounding box(OBB) from a set of points, based on their covariance.";
}

int mode_calc_obb(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill calc obb: too few arguments.");
        log::error(mode_calc_obb_usage());
        return 1;
    }

    const auto fname = args.front();
    if(fname == "help")
    {
        log::info(mode_calc_obb_usage());
        return 0;
    }

    for(const auto& frame : reader(fname))
    {
        const auto center = std::accumulate(frame.begin(), frame.end(),
                Vector<double, 3>(0, 0, 0), [](const auto c, const auto p) {
                    return c + p.position();
                }) / static_cast<double>(frame.size());

        Matrix<double, 3, 3> cov(0,0,0,
                                 0,0,0,
                                 0,0,0);
        for(const auto& p : frame)
        {
            const auto dr = p.position() - center;
            cov(0, 0) += dr[0] * dr[0];
            cov(0, 1) += dr[0] * dr[1];
            cov(0, 2) += dr[0] * dr[2];
            cov(1, 0) += dr[1] * dr[0];
            cov(1, 1) += dr[1] * dr[1];
            cov(1, 2) += dr[1] * dr[2];
            cov(2, 0) += dr[2] * dr[0];
            cov(2, 1) += dr[2] * dr[1];
            cov(2, 2) += dr[2] * dr[2];
        }
        cov /= static_cast<double>(frame.size());
        log::debug("calculating eigenvalue ...");

        JacobiEigenSolver solver;
        std::array<std::pair<double, Vector<double, 3>>, 3> es = solver.solve(cov);
        std::sort(es.begin(), es.end(), [](const auto& lhs, const auto& rhs) {
                    return lhs.first > rhs.first;
                });

        log::debug("ev1 = ", es[0].first, ", ", es[0].second);
        log::debug("ev2 = ", es[1].first, ", ", es[1].second);
        log::debug("ev3 = ", es[2].first, ", ", es[2].second);

        const auto axis1 = regularize(es[0].second);
        const auto axis2 = regularize(es[1].second - dot_product(es[1].second, axis1) * axis1);
        const auto axis3 = cross_product(axis1, axis2);

        const auto dmax = std::numeric_limits<double>::max();
        Vector<double, 3> lower( dmax,  dmax,  dmax);
        Vector<double, 3> upper(-dmax, -dmax, -dmax);

        for(const auto& p : frame)
        {
            const auto dx = dot_product(p.position(), axis1);
            const auto dy = dot_product(p.position(), axis2);
            const auto dz = dot_product(p.position(), axis3);

            lower[0] = std::min(lower[0], dx);
            lower[1] = std::min(lower[1], dy);
            lower[2] = std::min(lower[2], dz);
            upper[0] = std::max(upper[0], dx);
            upper[1] = std::max(upper[1], dy);
            upper[2] = std::max(upper[2], dz);
        }

        const auto width = upper - lower;
        std::cout << "center = " << center << '\n';
        std::cout << "axis1  = " << axis1  << " length = " << width[0] << '\n';
        std::cout << "axis2  = " << axis2  << " length = " << width[1] << '\n';
        std::cout << "axis3  = " << axis3  << " length = " << width[2] << '\n';
        std::cout << "-------------------------------------------------------\n";
    }
    return 0;
}

} // mill
