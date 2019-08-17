Coffee mill
====

A command line tool for CafeMol [1] users.
You can also use this as a header-only library to analize the CafeMol data.

## Usage

The command line tool has 3 modes.
- `dcd` mode
- `pdb` mode
- `ninfo` mode

And each mode has its own commands.

To see the explanation of each command, run

```sh
$ mill help [mode]
$ mill <mode> <command> help
```

### DCD

1. superimpose all the snapshots by minimizing RMSD between them
   - `mill dcd impose traj.dcd`
   - `mill dcd impose input.toml`
2. concatenate several dcd files into one
   - `mill dcd join traj1.dcd traj2.dcd traj3.dcd ...`
   - `mill dcd join input.toml`
3. split a .dcd files into pieces; for every N snapshots
   - `mill dcd split traj.dcd 100`
4. extract some of the snapshots from a dcd file
   - `mill dcd extract traj.dcd 100 200`
   - `mill dcd extract input.toml`
5. convert dcd file to another format
   - `mill dcd convert pdb traj.dcd`
   - `mill dcd convert pdb traj.dcd model.pdb`

### PDB

1. print the sequences of chains in pdb file.
   - `mill pdb seq model.pdb`

### NINFO

1. split ninfo file into one-by-one.
   - `mill ninfo split data.ninfo`

## Build

first, download this repo.

```sh
$ git clone https://github.com/ToruNiina/Coffee-mill.git --recursive
```

Coffee-mill uses `cmake` to build.

```sh
$ cd build
$ cmake ..
$ make
```

## For Developpers

See the `mill` directory. For example, when you want to read and/or write
a dcd file, you can use `DCDData`, `DCDReader` and `DCDWriter` in this way.

```cpp
#include <mill/dcd.hpp>
#include <mill/math/Vector.hpp>

int main()
{
    mill::DCDReader<mill::Vector<double, 3>> reader;
    std::string fname("sample.dcd");
    mill::DCDData<mill::Vector<double, 3>> data = reader.read(fname);

    std::vector<std::vector<mill::Vector<double, 3>>> trajectory = data.traj();
    return 0;
}
```

`mill::DCDData` is composed of `mill::DCDHeader` and `DCDData::trajectory_type`.
`DCDData::trajectory_type` is a typedef of `std::vector<DCDData::snapshot_type>`.
`DCDData::snapshot_type` is a typedef of `std::vector<DCDData::position_type>`.
And the `DCDData::position_type` is a template parameter of `mill::DCDData`.
In the above code, `DCDData::trajectory_type` become `std::vector<std::vector<mill::Vector<double, 3>>>`.

`mill::DCDHeader` has a information included in the header of DCDfile.
It has number of frames, total step of the trajectory, delta t, and so on.

`mill::Vector<scalarT, dimension>` is a mathematical vector class provided 
for portability. You can use other vector classes like `Eigen::Vector3d` instead
if you add the explicit instantiation of `mill::scalar_type_of`.

```cpp
#include <Eigen/Core>
#include <mill/dcd.hpp>

namespace mill
{
template<>
struct scalar_type_of<Eigen::Vector3d>
{
    typedef double type;
};
} // mill

int main()
{
    mill::DCDReader<Eigen::Vector3d> reader;
    std::string fname("sample.dcd");
    mill::DCDData<Eigen::Vector3d> data = reader.read(fname);

    std::vector<Eigen::Vector3d> trajectory = data.traj();
    return 0;
}
```

Some methods like `BestFit` do not support other Vector class because those
methods use not only vectors but also matrices.

`Coffee-mill` also provides reader/writer of PDB and Ninfo file.

```cpp
#include <mill/pdb.hpp>
#include <mill/math/Vector.hpp>

int main()
{
    mill::PDBReader<mill::Vector<double, 3>> reader;
    std::string fname("sample.pdb");
    std::vector<mill::PDBAtom<mill::Vector<double, 3>>>
        data = reader.read(fname);
    std::vector<mill::PDBChain<mill::Vector<double, 3>>>
        chains = reader.parse(data);
    std::cout << "chain " << chains.front().chain_id() "has "
              << chains.front().size() << " residues." << std::endl;
    return 0;
}
```

```cpp
#include <mill/ninfo.hpp>
#include <iostream>

int main()
{
    mill::NinfoReader<double> reader;
    std::string fname("sample.ninfo");
    mill::NinfoData<double> data = reader.read(fname);
    std::cout << "there are " << data[NinfoKind::Bond].size() << "bond defined."
              << std::endl;
    return 0;
}
```

For more information, see document.

## Document

Using `doxygen`, you can generate a document files of the Coffee-mill.

To generate a document, run following commands.

    $ cd build
    $ cmake .. # if you already built, this is not needed.
    $ make doc

After running it, you can see the detailed document in `doc/html`.
For instance, open `mill/doc/html/index.html` using your favorite web-browser.

## Reference

[1] "CafeMol: A coarse-grained biomolecular simulator for simulating proteins at work. H. Kenzaki, N. Koga, N. Hori, R. Kanada, W. Li, K. Okazaki, XQ. Yao, and S. Takada Journal of Chemical Theory and Computation (2011) 7(6) pp1979-1989 DOI:10.1021/ct2001045"
