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
    using base_type       = WriterBase;
    using trajectory_type = base_type::trajectory_type;
    using snapshot_type   = base_type::snapshot_type;
    using particle_type   = base_type::particle_type;

    PDBWriter(const std::string_view fname)
        : current_(0), file_name_(fname), pdb_(file_name_)
    {
        if(!pdb_.good())
        {
            log::fatal("PDBWriter: file open error: ", fname);
        }
    }
    ~PDBWriter() override = default;

    void write_header(const trajectory_type& traj) override
    {
        const auto& header = traj.attributes();
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
    void write_footer(const trajectory_type& traj) override
    {
        using namespace std::literals::string_literals;
        if(traj.empty())
        {
            return ;
        }
        const auto& topol = traj.at(0).topology();
        if(topol.empty())
        {
            return;
        }

        for(std::size_t i=0; i<topol.size(); ++i)
        {
            const auto& bound = topol.list_connected(i, "bond"s);
            for(std::size_t idx=0; idx<bound.size(); idx+=4)
            {
                pdb_ << "CONECT" << std::setw(5) << std::right << i;
                pdb_ << std::setw(5) << std::right << bound.at(idx+0);
                if(bound.size() <= idx+1) {break;}
                pdb_ << std::setw(5) << std::right << bound.at(idx+1);
                if(bound.size() <= idx+2) {break;}
                pdb_ << std::setw(5) << std::right << bound.at(idx+2);
                if(bound.size() <= idx+3) {break;}
                pdb_ << std::setw(5) << std::right << bound.at(idx+3);
            }
        }
        return ;
    }
    void write(const trajectory_type& traj) override
    {
        this->write_header(traj);
        for(const auto& frame : traj)
        {
            this->write_frame(frame);
        }
        this->write_footer(traj);
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
            const auto atom          = p.try_string("name"    ).value_or(" C  "s);
            const auto current_chain = p.try_string("chain_id").value_or("A"s).front();

            // 80 chars + line feed + null
            std::array<char, 82> buffer; buffer.fill('\0');

            // suppress "-Wformat-truncation"
            // since float may have very large integer part (2^1023 ~ 10^300),
            // %8.3f might have nearly 300 digits in the integer part.
            // Of course it does not normally happen and does not cause any
            // problem (if the actual value has such a large value, possibly
            // the structure file or simulation result is invalid). But
            // compiler complains about that, because they don't know the
            // meaning of the code.
            // Since we use std::snprintf with buffer size, we don't need to be
            // afraid of buffer overflow.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
            std::snprintf(buffer.data(), buffer.size(),
                "%-6s%5lld %4s%c%3s %c%4lld%c   %8.3f%8.3f%8.3f%6.2f%6.2f          %2s%2s\n",
                p.try_string("record"  ).value_or("ATOM  "s).c_str(),
                static_cast<long long>(p.try_integer("serial"  ).value_or(serial)),
                p.try_string("name"    ).value_or(" C  "s).c_str(),
                p.try_string("alt_loc" ).value_or(" "s).front(),
                p.try_string("res_name").value_or("XXX"s).c_str(),
                p.try_string("chain_id").value_or("A"s).front(),
                static_cast<long long>(p.try_integer("res_seq" ).value_or(serial)),
                p.try_string("i_code"  ).value_or(" "s).front(),
                (std::min)(p.position()[0], 999.999),
                (std::min)(p.position()[1], 999.999),
                (std::min)(p.position()[2], 999.999),
                p.try_floating("occupancy"  ).value_or(  0.0),
                p.try_floating("temp_factor").value_or(999.9),
                p.try_string  ("element"    ).value_or(atom.substr(0, 2)).c_str(),
                p.try_string  ("charge"     ).value_or("  "s).c_str());
#pragma GCC diagnostic pop
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
