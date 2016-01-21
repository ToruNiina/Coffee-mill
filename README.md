Coffee-mill
====

tools for using CafeMol [1].

##Build

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

##Usage

###dcdtomovie

dcdtomovie recieves dcd file and pdb file and outputs movie file.

`./dcdtomovie <filename>.pdb <filename>.dcd`

this is still being adjusted but it works.

###nsplit

splitter splits all in one style ninfo file to one by one style ninfo files.

`./nsplit <filename>.ninfo [-sim N(1 by default)]`

this outputs some ninfo files and cafemol input file that contains only native-info-simN block for one-by-one style.

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

rmsdcalc outputs time series of RMSD value of certain chains between snapshot at each point of time and initial structure(__not reference structure__).

This reads CG style pdb file and dcd file that has same filename as pdb file.

CafeMol outputs these two files when you specify dcd in the OUTPUT in filename block.

rmsdcalc automatically adds the extension(.pdb and .dcd), so you should input filename without extension.

and you can set chain IDs that you want to calculate RMSD value. 

the value is same as cafemol .ts file. you can check this if you specify cafemol reference structure that is same as the initial structure.

###superimpose

`$./superimpose <filename>.dcd [Optional: chainIDs(default: all)]`

superimposer outputs dcd file that contains snapshots superimposed on the previous snapshot.

you can set chain IDs to use in the same way as rmsdcalc.

and when you set the chainIDs, superimpose requires the CG style PDB file having same name as dcd file.

(This program is a by-product of rmsdcalc. rmsdcalc calls this as subroutine when calculate rmsd of each snapshot)

##included library

- boost(boost regular expression)

##Reference

[1] "CafeMol: A coarse-grained biomolecular simulator for simulating proteins at work. H. Kenzaki, N. Koga, N. Hori, R. Kanada, W. Li, K. Okazaki, XQ. Yao, and S. Takada Journal of Chemical Theory and Computation (2011) 7(6) pp1979-1989 DOI:10.1021/ct2001045"
