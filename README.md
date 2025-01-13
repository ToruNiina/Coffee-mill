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

To see the usage and explanation of each command, run the following command.

```sh
$ mill help [mode] [command]
or
$ mill [mode] [command] help
```

E.g.

```console
$ mill calc rmsd help
Info :   ___        __  __             __    __ _  _  _
Info :  / __| ___  / _|/ _| ___  ___   | \  / |(_)| || |
Info : | |   / _ \| |_| |_ / _ \/ _ \  |  \/  || || || |
Info : | |__| (_) |  _|  _|  __|  __/  | |\/| || || || |
Info :  \___|\___/|_| |_|  \___|\___|  |_|  |_||_||_||_|
Info :                    Copyright 2016-now Toru Niina)
Info : usage: mill calc rmsd [traj file] [ref file]
     : optinal args:
     :     --align=(true|false) [by default, true]
     :         If true (default), perform alignment by minimizing RMSD.
     :         otherwise, it calculates RMSD without minimization.
     :     --output=<filename>  [by default, "mill_rmsd.dat"]
     :         output data file.
     :     --only=<begin>:<end>
     :     --ref-only=<begin>:<end>
     :         use [begin, end) region only.
```

### `calc` mode

All the `.dcd`, `.pdb`, `.trr`, and `.xyz` formats are supported in `calc` mode.

- `rmsd`
  - calculates trajectory of RMSD between a snapshot and a reference structure.
- `dist`
  - calculates distances of pairs of particles specified via the input file.
- `angle`
  - calculates angles formed by a trio of particles or a pair of inter-particle vectors specified via the input file.
- `wham`
  - performs WHAM from results of umbrella sampling.
- `aabb`
  - calculates axis-aligned bounding box from a given particles.
  - *Note*: It does not condier the radius of particles.
- `obb`
  - calculates oriented bounding box from a given particles based on the covariance.
  - *Note*: It does not condier the radius of particles.
- `autocorrelation`
  - calculates autocorrelation of (single-column) data.
- `mean`
  - calculates mean structure from a trajectory
- `pca`
  - performs Principal Component Analysis and output the trajectory and conformational changes along the PCs.
  - *Note*: It is only available if you have already installed Eigen3.
  - *Note*: It is parallelized via OpenMP, so choose the number of threads you want to use via `export OMP_NUM_THREADS=N`.
- `neighbors`
  - lists neighboring particles within some cutoff.
- `help`
  - prints help messages.

### `traj` mode

All the `.dcd`, `.pdb`, `.trr`, and `.xyz` formats are supported in `traj` mode.
You can use any of it, and even mix them in a single command.

- `convert`
  - converts a trajectory file to a different format.
- `impose`
  - align molecules onto the initial configuration by minimizing the RMSD.
- `join`
  - concatenates several trajectory files into one file.
- `split`
  - splits a DCD file for every N frames.
- `extract`
  - extracts a portion of frames in a trajectory.
- `translate`
  - moves molecules by a specified distance.
- `rotate`
  - rotates molecules around x, y, or z axis by a specified angle.
- `running-average`
  - takes running average of particle positions with a specified window size.
- `projection`
  - takes a dot product between 3N-dimensional vector (an axis) and snapshot from a specified origin.
- `help`
  - prints help message.

### `dcd` mode

As the name suggests, `dcd` mode only supports `.dcd` format.

- `info`
  - dumps header information of a dcd file.
- `help`
  - prints help messages.

### `pdb` mode

As the name suggests, `pdb` mode only supports `.pdb` format.

- `seq`
  - prints sequence (one-letter amino acid code, or a DNA/RNA sequence) of the chains in a PDB file.
- `help`
  - prints help messages.

### `ninfo` mode

As the name suggests, `ninfo` mode only supports `.ninfo` format that is specific to CafeMol¹.

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

To find Eigen3, you can use `cmake .. -DEigen3_DIR=/path/to/eigen-3.y.z/`.

## Reference

1. "CafeMol: A coarse-grained biomolecular simulator for simulating proteins at work. H. Kenzaki, N. Koga, N. Hori, R. Kanada, W. Li, K. Okazaki, XQ. Yao, and S. Takada Journal of Chemical Theory and Computation (2011) 7(6) pp1979-1989 DOI:10.1021/ct2001045"
