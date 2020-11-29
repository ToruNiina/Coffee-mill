#ifndef COFFEE_MILL_PSF_WRITER_HPP
#define COFFEE_MILL_PSF_WRITER_HPP
#include <mill/common/Trajectory.hpp>
#include <mill/util/logger.hpp>
#include <unordered_set>
#include <iomanip>
#include <fstream>

namespace mill
{

class PSFWriter
{
  public:
    using base_type       = WriterBase;
    using trajectory_type = base_type::trajectory_type;
    using snapshot_type   = base_type::snapshot_type;
    using particle_type   = base_type::particle_type;

  public:

    explicit PSFWriter(const std::string_view fname)
        : current_(0), file_name_(fname), xyz_(file_name_)
    {
        if(!psf_.good())
        {
            log::fatal("PSFWriter: file open error: ", file_name_);
        }
    }
    ~PSFWriter() override = default;

    void write_header(const trajectory_type&) override
    {
        return;
    }
    void write_footer(const trajectory_type&) override
    {
        return;
    }

    void write(const trajectory_type& traj) override
    {
        this->write_frame(traj.at(0));
        return;
    }
    void write_frame(const snapshot_type& frame) override
    {
        using namespace std::literals::string_literals;
        current_ += 1;

        psf_ << frame.size() << " !NATOM\n";

        const std::size_t idx_w = std::to_string(frame.size()).size();
        std::size_t index=1;
        for(const auto& p : frame)
        {
            const auto name = p.try_string("name").value_or("X"s);
            const auto resn = p.try_string("res_name").value_or(name);
            const auto resi = p.try_integer("res_seq").value_or(1);
            const auto chni = p.try_integer("chain_id").value_or("A"s);
            const auto chrg = p.try_floating("charge").value_or(0.0);
            const auto mass = p.try_floating("mass").value_or(0.0);

            psf_ << std::setw(idx_w) << std::left << index << ' ';
            psf_ << std::setw(7)     << chni << ' ';
            psf_ << std::setw(10)    << resi << ' ';
            psf_ << std::setw(7)     << resn << ' ';
            psf_ << std::setw(7)     << name << ' ';
            psf_ << std::setw(7)     << name << ' ';
            psf_ << std::fixed       << chrg << ' ';
            psf_ << std::fixed       << mass << '\n';

            index += 1;
        }

        if( ! frame.topology().has_value())
        {
            return;
        }

        std::vector<std::pair<std::size_t, std::size_t>> bonds;
        const auto& topol = frame.topology().value();
        for(std::size_t i=0; i<frame.size(); ++i)
        {
            for(const auto connected : topol.list_connected(i, "bond"))
            {
                bonds.emplace_back(std::min(i, connected), std::max(i, connected));
            }
        }

        std::sort(bonds.begin(), bonds.end());
        bonds.erase(std::unique(bonds.begin(), bonds.end()), bonds.end());

        psf_ << std::setw(8) << std::right << bonds.size() << " !NBOND: bonds\n";
        for(std::size_t i=0; i<bonds.size(); i+=4)
        {
            psf_ << std::setw(8) << std::right << bonds.at(i+0).first;
            psf_ << std::setw(8) << std::right << bonds.at(i+0).second;
            if(i+1 == bonds.size()) {psf_ << '\n'; break;}

            psf_ << std::setw(8) << std::right << bonds.at(i+1).first;
            psf_ << std::setw(8) << std::right << bonds.at(i+1).second;
            if(i+2 == bonds.size()) {psf_ << '\n'; break;}

            psf_ << std::setw(8) << std::right << bonds.at(i+2).first;
            psf_ << std::setw(8) << std::right << bonds.at(i+2).second;
            if(i+3 == bonds.size()) {psf_ << '\n'; break;}

            psf_ << std::setw(8) << std::right << bonds.at(i+3).first;
            psf_ << std::setw(8) << std::right << bonds.at(i+3).second;
            psf_ << '\n';
        }
        return;
    }

    std::size_t      size()      const noexcept override {return current_;}
    std::string_view file_name() const noexcept override {return file_name_;}

  private:

    std::string_view truncate(std::string_view v, const std::size_t len) const
    {
        if(v.size() <= len)
        {
            return v;
        }
        else
        {
            return v.substr(0, len);
        }
    }

  private:

    std::size_t current_;
    std::string file_name_;
    std::ofstream psf_;
};

}// mill
#endif //COFFEE_MILL_DCD_READER
