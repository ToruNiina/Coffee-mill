/*!
  @file PDBWriter.hpp
  @brief pdb writer.

  write pdb atoms or chains into output stream.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-10-18 10:00
  @copyright Toru Niina 2016 on MIT License
*/
#ifndef COFFEE_MILL_PDB_WRITER_HPP
#define COFFEE_MILL_PDB_WRITER_HPP
#include <mill/common/Trajectory.hpp>
#include <mill/common/WriterBase.hpp>
#include <mill/util/logger.hpp>
#include <iomanip>
#include <fstream>
#include <cstdio>

namespace mill
{

class PDBWriter final : public WriterBase
{
  public:
    using base_type                = WriterBase;
    using trajectory_type          = base_type::trajectory_type;
    using snapshot_type            = base_type::snapshot_type;
    using particle_type            = base_type::particle_type;
    using attribute_container_type = base_type::attribute_container_type;

    PDBWriter(const std::string_view fname)
        : current_(0), file_name_(fname), pdb_(file_name_)
    {
        if(!pdb_.good())
        {
            log::fatal("PDBWriter: file open error: ", fname);
        }
    }
    ~PDBWriter() override = default;

    void write_header(const attribute_container_type& header) override
    {
        if(header.count("boundary_width") != 0)
        {
            const auto w = header.at("boundary_width").as_vector();
            // 80 chars + line feed + null
            std::array<char, 82> buffer; buffer.fill('\0');
            std::snprintf(buffer.data(), buffer.size(),
                          "CRYST1%9.3f%9.3f%9.3f%7.2f%7.2f%7.2f%-10s%4d\n",
                          w[0], w[1], w[2], 90.0, 90.0, 90.0, "P 1", 1);
            pdb_ << buffer.data();
        }
        return; // xyz does not have any header info
    }
    void write(const trajectory_type& traj) override
    {
        for(const auto& frame : traj)
        {
            this->write_frame(frame);
        }
        return;
    }
    void write_frame(const snapshot_type& frame) override
    {
        using namespace std::literals::string_literals;
        this->current_ += 1;
        this->pdb_ << "MODEL     "
                   << std::setw(4) << std::right << this->current_ << '\n';

        char chain = '\0';
        std::int64_t serial = 0;
        for(const auto& p : frame)
        {
            serial += 1;
            const auto atom          = p.at("name"    ).try_string().value_or(" C  "s);
            const auto current_chain = p.at("chain_id").try_string().value_or("A"s).front();

            // 80 chars + line feed + null
            std::array<char, 82> buffer; buffer.fill('\0');
            std::snprintf(buffer.data(), buffer.size(),
                "%-6s%5lld %4s%c%3s %c%4lld%c   %8.3f%8.3f%8.3f%6.2f%6.2f          %2s%2s\n",
                p.at("record"  ).try_string() .value_or("ATOM  "s).c_str(),
                p.at("serial"  ).try_integer().value_or(serial),
                p.at("name"    ).try_string() .value_or(" C  "s).c_str(),
                p.at("alt_loc" ).try_string() .value_or(" "s).front(),
                p.at("res_name").try_string() .value_or("XXX"s).c_str(),
                p.at("chain_id").try_string() .value_or("A"s).front(),
                p.at("res_seq" ).try_integer().value_or(serial),
                p.at("i_code"  ).try_string() .value_or(" "s).front(),
                p.position()[0],
                p.position()[1],
                p.position()[2],
                p.at("occupancy"  ).try_floating().value_or(  0.0),
                p.at("temp_factor").try_floating().value_or(999.9),
                p.at("element"    ).try_string()  .value_or(atom.substr(0, 2)).c_str(),
                p.at("charge"     ).try_string()  .value_or("  "s).c_str());
            pdb_ << buffer.data();

            if(chain != '\0' && chain != current_chain)
            {
                pdb_ << "TER\n";
            }
            chain = current_chain;
        }
        pdb_ << "TER\n";
        pdb_ << "ENDMDL\n";
        return;
    }

    std::size_t      size()      const noexcept override {return current_;}
    std::string_view file_name() const noexcept override {return file_name_;}

  private:

    std::size_t current_;
    std::string file_name_;
    std::ofstream pdb_;

};

} // mill
#endif /* COFFEE_MILL_PDB_WRITER */
