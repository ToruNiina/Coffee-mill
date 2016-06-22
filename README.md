Coffee-mill
====

command line tool for using CafeMol [1].

## Build

    $ cd build
    $ cmake ..
    $ make

## Usage

Coffee-mill has some modes and jobs.
To run coffee-mill, you should specify the pair of mode and job.

for example, to see the sequence of chains included in pdb file, run this.

    $ mill pdb --seq foo.pdb

foo.pdb can be normal pdb file and also CG-style pdb file.

To run some jobs, you should write toml-like format input file.
Some example input files are in example directory.

Now, these commands are available.

| mode | job | input | description |
|:-----|:----|:------|:------------|
| pdb  | --seq  |foo.pdb |extract sequence of chains included in pdb file|
| dna  | --seq  |foo.pdb |same as pdb --seq|
| dna  | --complemental |ATCG... |output complemental sequence|
| dcd  | --join |foo.inp |join dcdfiles to one dcd file|
| dcd  | --make-movie |foo.inp |make movie file from dcd trajectory and pdb structure file|
| ninfo| --split |foo.inp |split all-in-one ninfo file to one-by-one ninfo files|

## Document

Using doxygen, you can generate the document files of the Coffee-mill.

To generate document, run following commands.

    $ cd build
    $ cmake ..
    $ make doc

## Reference

[1] "CafeMol: A coarse-grained biomolecular simulator for simulating proteins at work. H. Kenzaki, N. Koga, N. Hori, R. Kanada, W. Li, K. Okazaki, XQ. Yao, and S. Takada Journal of Chemical Theory and Computation (2011) 7(6) pp1979-1989 DOI:10.1021/ct2001045"
