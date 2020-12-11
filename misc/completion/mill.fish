#
# Auto completion
#
# Copy this to ~/.config/fish/completions
#

set -l mill_commands help calc dcd pdb ninfo traj

complete -c mill -l quiet -d "suppresses all logs regardless of the level"
complete -c mill -l debug -d "output all logs regardless of the level"

complete -c mill -n "not __fish_seen_subcommand_from $mill_commands" -a help  -d "print help message"
complete -c mill -n "not __fish_seen_subcommand_from $mill_commands" -a calc  -d "calculate some property from traj"
complete -c mill -n "not __fish_seen_subcommand_from $mill_commands" -a traj  -d "modify a trajectory"
complete -c mill -n "not __fish_seen_subcommand_from $mill_commands" -a dcd   -d "manipulate dcd file"
complete -c mill -n "not __fish_seen_subcommand_from $mill_commands" -a pdb   -d "manipulate pdb file"
complete -c mill -n "not __fish_seen_subcommand_from $mill_commands" -a ninfo -d "manipulate ninfo file"

complete -c mill -n "__fish_seen_subcommand_from help" -a calc  -d "print help message of calc mode"
complete -c mill -n "__fish_seen_subcommand_from help" -a traj  -d "print help message of traj mode"
complete -c mill -n "__fish_seen_subcommand_from help" -a dcd   -d "print help message of pdb mode"
complete -c mill -n "__fish_seen_subcommand_from help" -a pdb   -d "print help message of dcd mode"
complete -c mill -n "__fish_seen_subcommand_from help" -a ninfo -d "print help message of ninfo mode"

complete -c mill -n "__fish_seen_subcommand_from calc" -a rmsd   -d "calc rmsd from specified structure"
complete -c mill -n "__fish_seen_subcommand_from calc" -a dist   -d "calc distance between specified particles"
complete -c mill -n "__fish_seen_subcommand_from calc" -a angle  -d "calc angle formed by specified particles"
complete -c mill -n "__fish_seen_subcommand_from calc" -a wham   -d "perform WHAM from umbrella sampling traj"
complete -c mill -n "__fish_seen_subcommand_from calc" -a obb    -d "calculate best fit oriented bounding box"
complete -c mill -n "__fish_seen_subcommand_from calc" -a aabb   -d "calculate axis aligned bounding box"
complete -c mill -n "__fish_seen_subcommand_from calc" -a center -d "calc geometric center of a molecule in trajectory"
complete -c mill -n "__fish_seen_subcommand_from calc" -a autocorrelation -d "calc autocorrelation of data"
complete -c mill -n "__fish_seen_subcommand_from calc" -a help   -d "print help message"

complete -c mill -n "__fish_seen_subcommand_from pdb" -a seq  -d "dump sequence of chains in pdb file"
complete -c mill -n "__fish_seen_subcommand_from pdb" -a help -d "print help message"

complete -c mill -n "__fish_seen_subcommand_from ninfo" -a split -d "split all-in-one ninfo into one-by-one"
complete -c mill -n "__fish_seen_subcommand_from ninfo" -a help  -d "print help message"

complete -c mill -n "__fish_seen_subcommand_from dcd" -a info    -d "dump header information of dcd file"
complete -c mill -n "__fish_seen_subcommand_from dcd" -a help    -d "print help message"

complete -c mill -n "__fish_seen_subcommand_from psf" -a gen     -d "generate psf from mjolnir input for visualization purpose"
complete -c mill -n "__fish_seen_subcommand_from psf" -a help    -d "print help message"

complete -c mill -n "__fish_seen_subcommand_from traj" -a join      -d "concatenate several trajectory files"
complete -c mill -n "__fish_seen_subcommand_from traj" -a split     -d "split a trajectory files into several files"
complete -c mill -n "__fish_seen_subcommand_from traj" -a extract   -d "extract some of the snapshots from trajectory file"
complete -c mill -n "__fish_seen_subcommand_from traj" -a convert   -d "convert a trajectory file into another format"
complete -c mill -n "__fish_seen_subcommand_from traj" -a impose    -d "superimpose all the snapshots by minimizing RMSD"
complete -c mill -n "__fish_seen_subcommand_from traj" -a rotate    -d "rotate molecule by a specified angle"
complete -c mill -n "__fish_seen_subcommand_from traj" -a translate -d "translate molecule by a specified distance"
complete -c mill -n "__fish_seen_subcommand_from traj" -a help      -d "print help message"
