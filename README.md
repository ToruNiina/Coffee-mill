# Coffee mill

A command line tool for Mjolnir/CafeMol users.

## Usage

The command line tool has 3 modes.

- `calc` mode
- `dcd` mode
- `pdb` mode
- `ninfo` mode
- `help` mode

And each mode has its own commands.

To see the explanation of each command, run the following command.

```sh
$ mill help [mode] [command]
or
$ mill [mode] [command] help
```

### `calc` mode

All the `dcd`, `xyz`, and `trr` formats are supported in `calc` mode.

- `rmsd`
  - calculates RMSD of each frame, taking reference structure.
- `dist`
  - calculates distances of pairs of particles, taking input file.
- `angle`
  - calculates angles of pairs of particles, taking input file.
- `wham`
  - performs WHAM from results of umbrella sampling.
- `help`
  - prints help messages.

### DCD

- `impose`
  - imposes all the frames onto a reference configuration by minimizing RMSD.
  - It is possible to ignore some of the particles/chains while superimpose.
- `info`
  - dumps header information.
- `join`
  - concatenates several DCD files into one file.
- `split`
  - splits a DCD file for every N frames.
- `extract`
  - extracts frames from a DCD file and creates another dcd file.
- `convert`
  - converts DCD file into PDB format.
  - It works without PDB or PSF files. But in that case, resulting PDB file will lose residue information.
- `help`
  - prints help messages.

### PDB

- `seq`
  - prints sequence of the chains in a PDB file.
- `help`
  - prints help messages.

### NINFO

- `split`
  - splits an all-in-one ninfo file into one-by-one files.
- `help`
  - prints help messages.

## Build

```sh
$ git clone https://github.com/ToruNiina/Coffee-mill.git --recursive
$ cd build
$ cmake ..
$ make
```

## Reference

1. "CafeMol: A coarse-grained biomolecular simulator for simulating proteins at work. H. Kenzaki, N. Koga, N. Hori, R. Kanada, W. Li, K. Okazaki, XQ. Yao, and S. Takada Journal of Chemical Theory and Computation (2011) 7(6) pp1979-1989 DOI:10.1021/ct2001045"
