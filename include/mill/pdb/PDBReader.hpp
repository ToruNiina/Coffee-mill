#ifndef COFFEE_MILL_PDB_READER_HPP
#define COFFEE_MILL_PDB_READER_HPP
#include <mill/util/logger.hpp>
#include <mill/common/Trajectory.hpp>
#include <mill/common/DeferedReader.hpp>
#include <fstream>
#include <sstream>

namespace mill
{

class PDBReader final : public DeferedReaderBase
{
  public:
    using base_type = DeferedReaderBase;
    using trajectory_type          = base_type::trajectory_type         ;
    using snapshot_type            = base_type::snapshot_type           ;
    using particle_type            = base_type::particle_type           ;
    using attribute_container_type = base_type::attribute_container_type;

  public:

    explicit PDBReader(const std::string_view fname)
        : current_(0), file_name_(fname), pdb_(file_name_)
    {
        if(!pdb_.good())
        {
            log::fatal("PDBReader: file open error: ", fname);
        }
    }
    ~PDBReader() override = default;

    attribute_container_type read_header() override
    {
        log::debug("mill::PDBReader: reading header ...");
        //
        // TODO: read REMARKs?
        //

        // skip lines until "MODEL", "ATOM", "HETATM" is found.
        // When a line is found, rollback to the beginning of first line
        std::string line;
        auto checkpoint = this->pdb_.tellg();
        while(line.substr(0, 6) != "MODEL " &&
              line.substr(0, 6) != "ATOM  " &&
              line.substr(0, 6) != "HETATM")
        {
            checkpoint = this->pdb_.tellg(); // save the beginning of 1st line
            std::getline(this->pdb_, line);
            log::debug("skipping ", line);
        }
        this->pdb_.seekg(checkpoint); // go back to the beginning of 1st line
        log::debug("mill::PDBReader: the first line is ", line);
        log::debug("mill::PDBReader: done.");
        return attribute_container_type{};
    }
    trajectory_type read() override
    {
        log::debug("mill::PDBReader: reading the whole trajectory...");
        this->rewind();

        trajectory_type traj(this->read_header());
        while(!this->pdb_.eof())
        {
            if(auto frame = read_frame())
            {
                traj.snapshots().push_back(std::move(*frame));
                this->pdb_.peek();
            }
            else
            {
                log::debug("mill::PDBReader: frame read failed");
                break;
            }
        }
        log::debug("mill::PDBReader: done.");
        return traj;
    }

    std::optional<snapshot_type> read_frame() override
    {
        log::debug("mill::PDBReader: reading the next snapshot...");
        if(this->is_eof())
        {
            return std::nullopt;
        }
        snapshot_type frame;

        std::string line; // buffer for getline
        while(line.substr(0, 6) != "ENDMDL" && not this->is_eof())
        {
            std::getline(this->pdb_, line);

            const auto header = line.substr(0, 6);
            if(header == "ATOM  " || header == "HETATM")
            {
                Particle p;
                // according to the wwPDB field name
                p["record"  ]   = header;
                p["serial"  ]   = std::stoi(line.substr(6, 5));
                p["name"    ]   = line.substr(12, 4);
                p["alt_loc" ]   = line.substr(16, 1);
                p["res_name"]   = line.substr(17, 3);
                p["chain_id"]   = line.substr(21, 1);
                p["res_seq" ]   = std::stoi(line.substr(22, 4));
                p["i_code"  ]   = line.substr(26, 1);
                p.position()[0] = std::stod(line.substr(30, 8));
                p.position()[1] = std::stod(line.substr(38, 8));
                p.position()[2] = std::stod(line.substr(46, 8));

                if(60 <= line.size())
                {
                    p["occupancy"] = std::stod(line.substr(54, 6));
                }
                else
                {
                    p["occupancy"] = 0.0;
                }

                if(66 <= line.size())
                {
                    p["temp_factor"] = std::stod(line.substr(60, 6));
                }
                else
                {
                    p["temp_factor"] = 99.9;
                }

                if(78 <= line.size())
                {
                    p["element"] = line.substr(76, 2);
                }
                else
                {
                    p["element"] = "  ";
                }

                if(80 <= line.size())
                {
                    p["charge"] = line.substr(78, 2);
                }
                else
                {
                    p["charge"] = "  ";
                }
                frame.particles().push_back(std::move(p));
            }
            else if(header == "TER   ")
            {
                log::debug("PDBReader: `TER` found. Skipped.");
            }
            else if(header == "ANISOU")
            {
                log::debug("PDBReader: `ANISOU` is currently not supported.");
            }
        }

        if(frame.empty())
        {
            return std::nullopt;
        }

        pdb_.peek();
        log::debug("mill::PDBReader: done. there are ", frame.size(), " particles.");
        current_ += 1;

        return frame;
    }

    std::optional<snapshot_type> read_frame(const std::size_t idx) override
    {
        log::debug("mill::PDBReader: reading the ", idx, "-th snapshot");
        this->rewind();

        // skip n snapshots
        log::debug("mill::PDBReader: skipping ", idx, " snapshots...");
        std::string line;
        for(std::size_t i=0; i<idx; ++i)
        {
            if(const auto frame = this->read_frame(); not frame.has_value())
            {
                log::error("mill::PDBReader: ", idx, "-th snapshot does not exist");
                return std::nullopt;
            }
            pdb_.peek();
        }
        log::debug("mill::PDBReader: done.");
        return this->read_frame();
    }

    void rewind() override
    {
        log::debug("mill::PDBReader: rewinding the file");
        current_ = 0;
        pdb_.clear(); // clear failbit flags like the EOF flag
        pdb_.seekg(0, std::ios_base::beg);
        pdb_.peek();
        log::debug("mill::PDBReader: done.");
    }

    bool             is_eof()    const noexcept override {return pdb_.eof();}
    std::size_t      current()   const noexcept override {return current_;}
    std::string_view file_name() const noexcept override {return file_name_;}

  private:
    std::size_t   current_;
    std::string   file_name_;
    std::ifstream pdb_;
};

} // mill
#endif//COFFEE_MILL_PURE_PDB_READER
