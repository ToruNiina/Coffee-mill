# Coffee mill

A command line tool for Mjolnir/CafeMol users.

## Usage

`mill` has the following modes.

- `help` mode
- `calc` mode
- `traj` mode
- `dcd` mode
- `pdb` mode
- `ninfo` mode

And each mode has its own commands.

To see the explanation of each command, run the following command.

```sh
$ mill help [mode] [command]
or
$ mill [mode] [command] help
```

### `calc` mode

All the `.dcd`, `.pdb`, `.trr`, and `.xyz` formats are supported in `calc` mode.

- `rmsd`
  - calculates RMSD of each frame, taking reference structure.
- `dist`
  - calculates distances of pairs of particles, taking input file.
- `angle`
  - calculates angles of pairs of particles, taking input file.
- `wham`
  - performs WHAM from results of umbrella sampling.
- `aabb`
  - calculate axis-aligned bounding box from a given point cloud
- `obb`
  - calculate oriented bounding box from a given point cloud using covariance
- `autocorrelation`
  - calculate autocorrelation of data
- `help`
  - prints help messages.

### `traj` mode

All the `.dcd`, `.pdb`, `.trr`, and `.xyz` formats are supported in `traj` mode.

- `join`
  - concatenates several trajectory files into one file.
- `extract`
  - extract a portion of frames in a trajectory.
- `convert`
  - converts a trajectory file to a different format.
- `split`
  - splits a DCD file for every N frames.
- `impose`
  - imposes molecules onto the initial configuration by minimizing RMSD.
- `rotate`
  - rotate molecules around x, y, or z axis by a specified angle.
- `translate`
  - move molecules by a specified distance.
- `help`
  - prints help message.

### `dcd` mode

As the name suggests, `dcd` mode only supports `.dcd` format.

- `info`
  - dumps header information.
- `help`
  - prints help messages.

### `pdb` mode

As the name suggests, `pdb` mode only supports `.pdb` format.

- `seq`
  - prints sequence of the chains in a PDB file.
- `help`
  - prints help messages.

### `ninfo` mode

As the name suggests, `ninfo` mode only supports `.ninfo` format.

- `split`
  - splits an all-in-one ninfo file into one-by-one files.
- `help`
  - prints help messages.


### Build

```sh
$ git clone https://github.com/ToruNiina/Coffee-mill.git --recursive
$ cd build
$ cmake ..
$ make
```

## Reference

1. "CafeMol: A coarse-grained biomolecular simulator for simulating proteins at work. H. Kenzaki, N. Koga, N. Hori, R. Kanada, W. Li, K. Okazaki, XQ. Yao, and S. Takada Journal of Chemical Theory and Computation (2011) 7(6) pp1979-1989 DOI:10.1021/ct2001045"
