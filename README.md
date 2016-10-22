Coffee-mill
====

command line tool for CafeMol [1] users.

You can use some files as header-only library to analize the CafeMol data.

## Build

__Coffee-mill__ depends on the library __TOMLParser__ written by ToruNiina.
Please clone the library and add the include-path to __TOMLParser__ first.

```sh
$ cd build
$ cmake ..
$ make
```

## Usage

You can use this like this way.

```sh
$ ls
sample.dcd sample.pdb dcd2movie.toml
$ mill dcd make-movie sample.dcd
$ ls
sample.dcd sample.pdb dcd2movie.toml sample.dcd.movie
$ cat dcd2movie.toml
output   = "fromdcd.movie"
dcdfile = "sample.dcd"
pdbfile = "sample.pdb"
pymol = true
$ mill dcd make-movie dcd2movie.toml
sample.dcd sample.pdb dcd2movie.toml sample.dcd.movie fromdcd.movie
```

The supported commands are listed below.

| command                        | function                                             |
|:-------------------------------|:-----------------------------------------------------|
| mill pdb seq [filename]        | output sequences of the chains included in the file  |
| mill dcd make-movie [filename] | make movie file from dcd file                        |
| mill dcd impose [filename]     | impose each snapshots in the trajectory on the initial snapshot|
| mill ninfo split [filename]    | split all-in-one ninfo file to one-by-one ninfo file |

There are some commands that is not stable yet. Those are not listed now.

## For Developpers

see `includes` directory. For example, when you want to read and/or write a dcd
file, you can use `DCDData`, `DCDReader` and `DCDWriter` in this way.

```cpp
#include "mill/includes/DCDData.hpp"
#include "mill/includes/DCDReader.hpp"
#include "mill/includes/DCDWriter.hpp"
#include "mill/includes/Vector.hpp"

int main()
{
    mill::DCDReader<mill::Vector<double, 3>> reader;
    std::string fname("sample.dcd");
    mill::DCDData<mill::Vector<double, 3>> data = reader.read(fname);

    std::vector<mill::Vector<double, 3>> trajectory = data.traj();
    return 0;
}
```

`mill::Vector<scalarT, dimension>` is a mathematical vector class provided 
for portability. You can use other vector classes like `Eigen::Vector3d` instead
if you add the explicit instantiation of `mill::scalar_type_extractor`.

The code is a bit complicated
because the template instantiation is sensitive to the order of declaration.

```cpp
#include <Eigen/Core>
#include "mill/includes/detail/scalar_type_extractor.hpp"

namespace mill
{
template<>
struct scalar_type_extractor<Eigen::Vector3d>
{
    typedef double type;
};
} // mill

// do not include those files before you add declaration of
// scalar_type_extractor<Eigen::Vector3d>.
#include "mill/includes/DCDData.hpp"
#include "mill/includes/DCDReader.hpp"
#include "mill/includes/DCDWriter.hpp"

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

## Document

Using doxygen, you can generate the document files of the Coffee-mill.

To generate document, run following commands.

    $ cd build
    $ cmake ..
    $ make doc

## Reference

[1] "CafeMol: A coarse-grained biomolecular simulator for simulating proteins at work. H. Kenzaki, N. Koga, N. Hori, R. Kanada, W. Li, K. Okazaki, XQ. Yao, and S. Takada Journal of Chemical Theory and Computation (2011) 7(6) pp1979-1989 DOI:10.1021/ct2001045"
