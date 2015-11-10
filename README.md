Coffee-mill
====

(plobably) useful tools for using CafeMol[1]

##Build

if you have cmake, it is very easy.

    $ cmake .
    $ make

##Usage

###nsplit

splitter splits all in one style ninfo file to one by one style ninfo files.

`./nsplit <filename>.ninfo [-sim N(1 by default)]`

`outputs <filename>_iunit1_iunit2.ninfo and native_info_simN.inp`

this inp file contains only native-info-simN block for one by one style.

###seqextr

`$./seqextr [option: -cg] <filename>.pdb`

seqextr outputs DNA and amino acid sequence in pdb file(both regular PDB file and cg style PDB file).

###mutator

`$./mutator <filename>.pdb`

mutator can change the sequence of DNA and Protein in CG style PDB file interactively.

this outputs mutated pdb file named "outX.pdb". X is chain ID(alphabet).

mutator regards '=' as "no change in this residue".

So if you fill the input sequence with '=', mutator outputs the same file as input pdb file.

when you input "pass" instead of modified sequence, mutator outputs nothing and goes next chain.

if you want to quit the mutation, type "quit", "end", "exit", or "bye" instead of sequence.

###rmsdcalc

`$./rmsdcalc <filename> <chainIDs(alphabet)>`

rmsdcalc outputs time series of RMSD value of chain <chainIDs> between structure in each time-point and initial structure(__not reference structure__).

This reads CG style pdb file and dcd file that has same filename as pdb file.

CafeMol can output these two files when you specify dcd in the OUTPUT.

rmsdcalc automatically adds the extension(.pdb and .dcd), so you should input filename without extension.

and you can set chain IDs that you want to calculate RMSD value. 

###superimpose

`$./superimpose <filename>.dcd`

superimposer outputs dcd file including superimposed structure.

##included library

- boost(boost regular expression)
- Eigen(for calculate eigenvalue)

to use my tool, install Eigen on /usr/include (Ubuntu) or re-write #include path

##Reference

[1] "CafeMol: A coarse-grained biomolecular simulator for simulating proteins at work. H. Kenzaki, N. Koga, N. Hori, R. Kanada, W. Li, K. Okazaki, XQ. Yao, and S. Takada Journal of Chemical Theory and Computation (2011) 7(6) pp1979-1989 DOI:10.1021/ct2001045"
