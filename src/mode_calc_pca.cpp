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
    return "usage: mill calc pca {trajectory} [--top=(3 by default)] [--top-contribution=(95% by default)] [--output=(\"{base name of trajectory}_pca.dat\" by default)] [--scaled=(true|false)]\n"
           "         determines principal component from traj file and outputs trajectory along the axes.\n"
           "         --top=N specifies the number of component will be written out.\n"
           "         --top-contribution=% specifies the number of component via the accumulated contribution rate.\n"
           "         if you specify `--top=3 --top-contribution=95` and it would be found that top 5 components are\n"
           "         needed to achieve 95% contribution, 5 components are written.\n"
           "         if --scaled=true, the output trajectory along PC axes are scaled by the corresponding variance.\n"
           "     $ mill calc pca {input.toml}\n"
           "         determines principal component using customized input.\n"
           "         ```toml\n"
           "         input  = \"traj.dcd\"\n"
           "         output_basename = \"pca.dat\"\n"
           "         # (optional) how many components are reported.\n"
           "         # by default, 3.\n"
           "         top    = 10\n"
           "         # (optional) instead of `top`, output top N% contributing PCs.\n"
           "         top_contribution = 95 # %\n"
           "         # (optional) indices of particles to be used.\n"
           "         # by default, all the particles are used.\n"
           "         # {first, last} specifies the range. last is not included.\n"
           "         use    = [0, 2, 5, {first=10, last=20}]\n"
           "         ```";
}

int mode_calc_pca(std::deque<std::string_view> args)
{
    using namespace std::literals::string_literals;

    auto top_n_opt       = pop_argument<std::size_t>(args, "top");
    auto top_contrib_opt = pop_argument<double     >(args, "top-contribution");
    auto output_opt      = pop_argument<std::string>(args, "output");
    auto scaled_opt      = pop_argument<bool       >(args, "scaled");
    std::vector<std::size_t> particles_to_be_used;

    if(args.empty())
    {
        log::error("mill calc pca: too few arguments.");
        log::error(mode_calc_pca_usage());
        return 1;
    }

    for(const auto& arg : args)
    {
        if(arg.substr(0, 2) == "--")
        {
            log::error("unknown argument ", arg, " found. It will be ignored. please check it.");
        }
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
            top_n_opt = toml::find<std::size_t>(data, "top");
        }

        if(data.contains("top_contribution"))
        {
            top_contrib_opt = toml::find<double>(data, "top_contribution");
        }
        if(data.contains("top-contribution"))
        {
            top_contrib_opt = toml::find<double>(data, "top-contribution");
        }

        if(data.contains("scaled"))
        {
            scaled_opt = toml::find<bool>(data, "scaled");
        }

        if(data.contains("output_basename"))
        {
            output_opt = toml::find<std::string>(data, "output_basename");
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

    if(not output_opt.has_value())
    {
        output_opt = std::string(base_name_of(trajfile));
    }

    const std::string output_basename = output_opt.value();

    log::info("It will use ", particles_to_be_used.size(), " particles in total.");
    if(top_n_opt.has_value())
    {
        log::info("It will output ", top_n_opt.value(), " components");
    }
    if(top_contrib_opt.has_value())
    {
        log::info("It will output top ", top_contrib_opt.value(), "% contributing components.");
    }
    log::info("The results will be written in ", output_basename);

    // we will use this `traj` to write the movement corresponds to the principal components.
    Trajectory traj = reader(trajfile).read();

    log::debug("input files are loaded.");
    log::info("trajectory has ", traj.size(), " snapshots.");

    log::debug("scaled = ", scaled_opt);
    if(scaled_opt.has_value() && scaled_opt.value())
    {
        log::info("The output trajectory will be scaled by the corresponding variance.");
    }
    else
    {
        log::info("The output trajectory will not be scaled and the eigenvector is regularized.");
    }

    // -----------------------------------------------------------------------
    // constructing covariance matrix

    std::size_t num_frames = 0;
    std::vector<double> means(particles_to_be_used.size() * 3, 0.0);
    for(const auto& frame : traj)
    {
#pragma omp parallel for
        for(std::size_t i=0; i<particles_to_be_used.size(); ++i)
        {
            means[i*3+0] += frame.at(particles_to_be_used[i]).position()[0];
            means[i*3+1] += frame.at(particles_to_be_used[i]).position()[1];
            means[i*3+2] += frame.at(particles_to_be_used[i]).position()[2];
        }
        num_frames += 1;
    }

    const double normalize = 1.0 / num_frames;
#pragma omp parallel for
    for(std::size_t i=0; i<means.size(); ++i)
    {
        auto& mean = means[i];
        mean *= normalize;
    }

    log::info("mean positions are calculated");

    // {x1, y1, z1, x2, y2, z2, ...}
    Eigen::MatrixXd mat = Eigen::MatrixXd::Zero(particles_to_be_used.size() * 3,
                                                particles_to_be_used.size() * 3);

    for(const auto& frame : traj)
    {
        // the matrix often will be large enough for this
#pragma omp parallel for
        for(std::size_t i=0; i<particles_to_be_used.size(); ++i)
        {
            const auto pix = frame.at(particles_to_be_used[i]).position()[0] - means[i*3+0];
            const auto piy = frame.at(particles_to_be_used[i]).position()[1] - means[i*3+1];
            const auto piz = frame.at(particles_to_be_used[i]).position()[2] - means[i*3+2];

            for(std::size_t j=i; j<particles_to_be_used.size(); ++j)
            {
                const auto pjx = frame.at(particles_to_be_used[j]).position()[0] - means[j*3+0];
                const auto pjy = frame.at(particles_to_be_used[j]).position()[1] - means[j*3+1];
                const auto pjz = frame.at(particles_to_be_used[j]).position()[2] - means[j*3+2];

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

#pragma omp parallel for
    for(std::size_t i=0; i<particles_to_be_used.size() * 3; ++i)
    {
        mat(i, i) *= normalize;
        for(std::size_t j=i+1; j<particles_to_be_used.size() * 3; ++j)
        {
            mat(i, j) *= normalize;
            mat(j, i) = mat(i, j);
        }
    }

    log::info("co-variance matrix is constructed");

    // -----------------------------------------------------------------------
    // calculating eigenvalues of covariance matrix

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(mat);
    std::vector<std::pair<double, Eigen::VectorXd>> eigens(particles_to_be_used.size() * 3);

#pragma omp parallel for
    for(std::size_t i=0; i<particles_to_be_used.size() * 3; ++i)
    {
        eigens[i].first  = solver.eigenvalues()[i];
        eigens[i].second = solver.eigenvectors().col(i);
    }

    std::sort(eigens.begin(), eigens.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.first > rhs.first;
        });

    log::info("eigenvalues are calculated");

    // -----------------------------------------------------------------------
    // determining contribution rate

    const auto sum_eigenvalues = std::accumulate(eigens.begin(), eigens.end(),
        0.0, [](const auto& acc, const auto& elem) {
            return acc + elem.first;
        }) * 0.01; // scale to the per-cent
    std::vector<double> contribution_rate(eigens.size());
    std::transform(eigens.begin(), eigens.end(), contribution_rate.begin(),
            [sum_eigenvalues](const auto& eigenpair) noexcept -> double {
                return eigenpair.first / sum_eigenvalues;
            });

    std::vector<double> accumulated_contribution_rate(eigens.size());
    const double contribution_threshold = top_contrib_opt.value_or(95.0);
    std::size_t top_contributions = 0;
    double accum = 0.0;
    std::transform(contribution_rate.begin(), contribution_rate.end(),
            accumulated_contribution_rate.begin(),
            [&accum, &top_contributions, contribution_threshold](const auto& rate) {
                if(accum <= contribution_threshold)
                {
                    top_contributions += 1;
                }
                accum += rate;
                return accum;
            });

    std::size_t num_components = std::max<std::size_t>(3, top_contributions);
    if(top_n_opt.has_value())
    {
        num_components = top_n_opt.value();
    }
    if(top_contrib_opt.has_value())
    {
        num_components = top_contributions;
    }
    if(top_n_opt.has_value() && top_contrib_opt.has_value())
    {
        num_components = std::max(top_contributions, top_n_opt.value());
    }

    if(num_components < eigens.size())
    {
        eigens.resize(num_components);
    }

    log::info("top ", num_components, " components (",
            accumulated_contribution_rate.at(num_components), "% contribution) will be written");

    // ------------------------------------------------------------------------
    // determine direction (sign) of eigenvector to make the trajectory (-)->(+)

    for(auto& [eigval, eigvec] : eigens)
    {
        Eigen::VectorXd first_frame = Eigen::VectorXd::Zero(particles_to_be_used.size() * 3);
        for(std::size_t i=0; i<particles_to_be_used.size(); ++i)
        {
            first_frame[i*3+0] = traj.front().at(particles_to_be_used[i]).position()[0] - means[i*3+0];
            first_frame[i*3+1] = traj.front().at(particles_to_be_used[i]).position()[1] - means[i*3+1];
            first_frame[i*3+2] = traj.front().at(particles_to_be_used[i]).position()[2] - means[i*3+2];
        }
        Eigen::VectorXd  last_frame = Eigen::VectorXd::Zero(particles_to_be_used.size() * 3);
        for(std::size_t i=0; i<particles_to_be_used.size(); ++i)
        {
            last_frame[i*3+0] = traj.back().at(particles_to_be_used[i]).position()[0] - means[i*3+0];
            last_frame[i*3+1] = traj.back().at(particles_to_be_used[i]).position()[1] - means[i*3+1];
            last_frame[i*3+2] = traj.back().at(particles_to_be_used[i]).position()[2] - means[i*3+2];
        }

        const auto first = eigvec.dot(first_frame);
        const auto  last = eigvec.dot( last_frame);
        if(last < first)
        {
            eigvec *= -1.0;
        }
    }

    // -----------------------------------------------------------------------
    // writing trajectory along the eigenvector

    std::vector<std::pair<double, double>> component_range(eigens.size(),
            std::make_pair(std::numeric_limits<double>::max(),
                          -std::numeric_limits<double>::max()));

    std::ofstream ofs(output_basename + "_PCA.dat"s);
    if(not ofs.good())
    {
        log::fatal("file open error: ", output_basename + "_PCA.dat"s);
    }

    ofs << '#';
    for(std::size_t i=0; i<eigens.size(); ++i)
    {
        ofs << " PC" << i;
    }
    ofs << '\n';

    ofs << '#';
    for(std::size_t i=0; i<eigens.size(); ++i)
    {
        ofs << " " << std::fixed << std::setprecision(5) << contribution_rate.at(i) << "%";
    }
    ofs << '\n';

    for(const auto& frame : traj)
    {
        Eigen::VectorXd snapshot = Eigen::VectorXd::Zero(particles_to_be_used.size() * 3);
        for(std::size_t i=0; i<particles_to_be_used.size(); ++i)
        {
            snapshot[i*3+0] = frame.at(particles_to_be_used[i]).position()[0] - means[i*3+0];
            snapshot[i*3+1] = frame.at(particles_to_be_used[i]).position()[1] - means[i*3+1];
            snapshot[i*3+2] = frame.at(particles_to_be_used[i]).position()[2] - means[i*3+2];
        }

        std::size_t idx=0;
        for(const auto& eigen : eigens)
        {
            auto component = snapshot.dot(eigen.second);
            if(scaled_opt.has_value() && scaled_opt.value())
            {
                component /= eigen.first;
            }
            ofs << component << ' ';

            component_range.at(idx).first  = std::min(component, component_range.at(idx).first);
            component_range.at(idx).second = std::max(component, component_range.at(idx).second);

            ++idx;
        }
        ofs << '\n';
    }

    log::info("trajectory along PCs are written in ", output_basename + "_PCA.dat");

    // -----------------------------------------------------------------------
    // output principal motion

    // freeze the trajectory at the mean position to see the movement of selected particles
    auto& init = traj.at(0);
    for(std::size_t i=1; i<traj.size(); ++i)
    {
        const auto frame = traj.at(i);
        for(std::size_t p_idx=0; p_idx<frame.size(); ++p_idx)
        {
            init.at(p_idx).position() += frame.at(p_idx).position();
        }
    }
    for(std::size_t p_idx=0; p_idx<init.size(); ++p_idx)
    {
        init.at(p_idx).position() *= normalize;
    }

    const std::size_t PC_movement_len = std::min<std::size_t>(1000, traj.size());
    traj.snapshots().resize(PC_movement_len);

    for(std::size_t i=1; i<traj.size(); ++i)
    {
        traj.at(i) = init;
    }

    // construct movement along PCs

    std::size_t idx=0;
    for(const auto& eigen : eigens)
    {
        const auto [lower, upper] = component_range.at(idx);

        const auto dx = (upper - lower) / PC_movement_len;
        for(std::size_t t=0; t<PC_movement_len; ++t)
        {
            auto& frame = traj.at(t);

            const auto x = lower + dx * t;
            for(std::size_t i=0; i<particles_to_be_used.size(); ++i)
            {
                frame.at(particles_to_be_used[i]).position()[0] = means.at(i*3+0) + eigen.second[i*3+0] * x;
                frame.at(particles_to_be_used[i]).position()[1] = means.at(i*3+1) + eigen.second[i*3+1] * x;
                frame.at(particles_to_be_used[i]).position()[2] = means.at(i*3+2) + eigen.second[i*3+2] * x;
            }
        }
        const auto outtrajname = output_basename + "_PC"s +
            std::to_string(idx+1) + std::string(extension_of(trajfile));
        auto w = writer(outtrajname);
        w.write(traj);

        log::info("structure change along PC", idx+1, " (", contribution_rate.at(idx) , "% contribution) is written in ", outtrajname);

        ++idx;
    }

    // -----------------------------------------------------------------------
    // output principal vectors as a trajectory

    // normally, number of Eigen vectors to be written is smaller than 1000...
    traj.snapshots().resize(eigens.size());
    for(std::size_t i=0; i<eigens.size(); ++i)
    {
        const auto& [val, vec] = eigens.at(i);

        auto& frame = traj.at(i);
        for(auto& p : frame)
        {
            p.position() = Vector<double, 3>(0, 0, 0); // clear frame
        }
        // write eigen vector
        for(std::size_t i=0; i<particles_to_be_used.size(); ++i)
        {
            frame.at(particles_to_be_used[i]).position()[0] = vec[i*3+0];
            frame.at(particles_to_be_used[i]).position()[1] = vec[i*3+1];
            frame.at(particles_to_be_used[i]).position()[2] = vec[i*3+2];
        }
    }
    const auto eigen_vec_name = output_basename + "_EigenVectors" +
                                std::string(extension_of(trajfile));
    auto w = writer(eigen_vec_name);
    w.write(traj);

    return 0;
}

} // mill
