#include "mode_calc_pca.hpp"

#include <mill/util/cmdarg.hpp>
#include <mill/math/Vector.hpp>
// #include <mill/math/EigenSolver.hpp>
#include <mill/traj.hpp>
#include <toml/toml.hpp>
#include <numeric>

#include <Eigen/Dense>

namespace mill
{

const char* mode_calc_pca_usage() noexcept
{
    return "usage: mill calc pca {trajectory} [--top=(3 by default)] [--output=(\"mill_pca.dat\" by default)]\n"
           "         determines principal component from traj file.\n"
           "       mill calc pca {input.toml}\n"
           "         determines principal component using customized input.\n"
           "         ```toml\n"
           "         input  = \"traj.dcd\"\n"
           "         output = \"pca.dat\"\n"
           "         # (optional) how many components are reported.\n"
           "         # by default, 3.\n"
           "         top    = 10\n"
           "         # (optional) indices of particles to be used.\n"
           "         # by default, all the particles are used.\n"
           "         # {first, last} specifies the range. last is not included.\n"
           "         use    = [0, 2, 5, {first=10, last=20}]\n"
           "         ```";
}

int mode_calc_pca(std::deque<std::string_view> args)
{
//     using Matrix = Matrix<double, DYNAMIC, DYNAMIC>;
//     using Vector = Vector<double, DYNAMIC>;
    using Matrix = Eigen::MatrixXd;
    using Vector = Eigen::VectorXd;

    auto top = pop_argument<std::size_t>(args, "top"   ).value_or(3);
    auto out = pop_argument<std::string>(args, "output").value_or("mill_pca.dat");
    std::vector<std::size_t> particles_to_be_used;

    if(args.empty())
    {
        log::error("mill calc pca: too few arguments.");
        log::error(mode_calc_pca_usage());
        return 1;
    }

    const std::string fname(args.front());
    if(fname == "help")
    {
        log::info(mode_calc_pca_usage());
        return 0;
    }

    std::string trajfile;
    if(extension_of(fname) == ".toml")
    {
        const auto data = toml::parse(fname);

        trajfile = toml::find<std::string>(data, "input");

        if(data.contains("top"))
        {
            top = toml::find<std::size_t>(data, "top");
        }
        if(data.contains("output"))
        {
            out = toml::find<std::string>(data, "output");
        }
        if(data.contains("use"))
        {
            for(auto&& elem : toml::find<toml::array>(data, "use"))
            {
                if(elem.is_integer())
                {
                    particles_to_be_used.push_back(elem.as_integer());
                }
                else
                {
                    for(std::size_t i=elem.at("first").as_integer(), e=elem.at("last").as_integer(); i<e; ++i)
                    {
                        particles_to_be_used.push_back(i);
                    }
                }
            }
        }
        else
        {
            particles_to_be_used.resize(reader(trajfile).read_frame()->size(), 0);
            std::iota(particles_to_be_used.begin(), particles_to_be_used.end(), 0);
        }
    }
    else
    {
        trajfile = std::string(fname);
        particles_to_be_used.resize(reader(trajfile).read_frame()->size(), 0);
        std::iota(particles_to_be_used.begin(), particles_to_be_used.end(), 0);
    }

    // we will use this `traj` to write the movement corresponds to the principal components.
    Trajectory traj = reader(trajfile).read();

    log::debug("input files are loaded");

    std::size_t num_frames = 0;
    std::vector<double> means(particles_to_be_used.size() * 3, 0.0);
    for(const auto frame : traj)
    {
        for(std::size_t i=0; i<particles_to_be_used.size(); ++i)
        {
            means[i*3+0] += frame.at(i).position()[0];
            means[i*3+1] += frame.at(i).position()[1];
            means[i*3+2] += frame.at(i).position()[2];
        }
        num_frames += 1;
    }
    for(auto& mean : means)
    {
        mean /= num_frames;
    }

    log::debug("mean positions calculated");

    // {x1, y1, z1, x2, y2, z2, ...}
//     Matrix mat = Matrix::zero(particles_to_be_used.size() * 3,
//                               particles_to_be_used.size() * 3);
    Matrix mat = Eigen::MatrixXd::Zero(particles_to_be_used.size() * 3,
                                       particles_to_be_used.size() * 3);
    for(const auto frame : traj)
    {
        for(std::size_t i=0; i<particles_to_be_used.size(); ++i)
        {
            const auto pix = frame.at(i).position()[0] - means[i*3+0];
            const auto piy = frame.at(i).position()[1] - means[i*3+1];
            const auto piz = frame.at(i).position()[2] - means[i*3+2];

            for(std::size_t j=i; j<particles_to_be_used.size(); ++j)
            {
                const auto pjx = frame.at(j).position()[0] - means[j*3+0];
                const auto pjy = frame.at(j).position()[1] - means[j*3+1];
                const auto pjz = frame.at(j).position()[2] - means[j*3+2];

                mat(i*3+0, j*3+0) += pix * pjx;
                mat(i*3+0, j*3+1) += pix * pjy;
                mat(i*3+0, j*3+2) += pix * pjz;

                mat(i*3+1, j*3+0) += piy * pjx;
                mat(i*3+1, j*3+1) += piy * pjy;
                mat(i*3+1, j*3+2) += piy * pjz;

                mat(i*3+2, j*3+0) += piz * pjx;
                mat(i*3+2, j*3+1) += piz * pjy;
                mat(i*3+2, j*3+2) += piz * pjz;
            }
        }
    }

    for(std::size_t i=0; i<particles_to_be_used.size() * 3; ++i)
    {
        mat(i, i) /= num_frames;
        for(std::size_t j=i+1; j<particles_to_be_used.size() * 3; ++j)
        {
            mat(i, j) /= num_frames;
            mat(j, i) = mat(i, j);
        }
    }

    log::debug("co-variance matrix constructed");

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(mat);
    std::vector<std::pair<double, Vector>> eigens(particles_to_be_used.size() * 3);
    for(std::size_t i=0; i<particles_to_be_used.size() * 3; ++i)
    {
        eigens[i].first  = solver.eigenvalues()[i];
        eigens[i].second = solver.eigenvectors().col(i);
        log::debug(i, "-th eigenvalue = ", eigens[i].first);
    }

//     JacobiEigenSolver solver;
//     auto eigens = solver.solve(mat);

    std::sort(eigens.begin(), eigens.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.first > rhs.first;
        });

    if(top < eigens.size())
    {
        eigens.resize(top);
    }

    std::vector<double> ev;
    for(const auto& eigen : eigens)
    {
        ev.push_back(eigen.first);
    }
    log::debug("top-", top, " eigenvalues = ", ev);


    log::debug("eigenvalues are calculated");

    std::vector<std::pair<double, double>> component_range(eigens.size(),
            std::make_pair(std::numeric_limits<double>::max(),
                          -std::numeric_limits<double>::max()));

    std::ofstream ofs(out);
    if(not ofs.good())
    {
        log::fatal("file open error: ", out);
    }

    ofs << '#';
    for(std::size_t i=0; i<eigens.size(); ++i)
    {
        ofs << " PC" << i;
    }
    ofs << '\n';


    for(const auto frame : traj)
    {
        Eigen::VectorXd snapshot = Eigen::VectorXd::Zero(particles_to_be_used.size() * 3);
        for(std::size_t i=0; i<particles_to_be_used.size(); ++i)
        {
            snapshot[i*3+0] = frame.at(i).position()[0] - means[i*3+0];
            snapshot[i*3+1] = frame.at(i).position()[1] - means[i*3+1];
            snapshot[i*3+2] = frame.at(i).position()[2] - means[i*3+2];
        }

        std::size_t idx=0;
        for(const auto& eigen : eigens)
        {
            const auto component = snapshot.dot(eigen.second);
            ofs << component << ' ';

            component_range.at(idx).first  = std::min(component, component_range.at(idx).first);
            component_range.at(idx).second = std::max(component, component_range.at(idx).second);

            ++idx;
        }
        ofs << '\n';
    }

    // output principal motion
    {
        const std::size_t PC_movement_len = std::min<std::size_t>(1000, traj.size());
        traj.snapshots().resize(PC_movement_len);

        std::size_t idx=0;
        for(const auto& eigen : eigens)
        {
            using namespace std::literals::string_literals;
            const auto [lower, upper] = component_range.at(idx);

            const auto dx = (upper - lower) / PC_movement_len;
            for(std::size_t t=0; t<PC_movement_len; ++t)
            {
                auto& frame = traj.at(t);

                const auto x = lower + dx * t;
                for(std::size_t i=0; i<frame.size(); ++i)
                {
                    frame.at(i).position()[0] = means.at(i*3+0) + eigen.second[i*3+0] * x;
                    frame.at(i).position()[1] = means.at(i*3+1) + eigen.second[i*3+1] * x;
                    frame.at(i).position()[2] = means.at(i*3+2) + eigen.second[i*3+2] * x;
                }
            }
            auto w = writer("mill_PC"s + std::to_string(idx+1) + std::string(extension_of(trajfile)));
            w.write(traj);
            ++idx;
        }
    }

    return 0;
}

} // mill
