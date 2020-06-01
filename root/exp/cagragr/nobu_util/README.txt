* How to compilte
In Makefile, please set GRUTLIBS and INCLUDE properly
Default:
GRUTLIBS  = -L$(HOME)/GRUTinizer/lib ...
INCLUDE   = -I$(HOME)/GRUTinizer/include

Then make the executable by

$ make

* How to use
In .bashrc, set PATH and LD_LIBRARY_PATH properly

export PATH=$HOME/nobukoba/kobamac/root/exp/cagragr/nobu_util:$PATH
export LD_LIBRARY_PATH=$HOME/GRUTinizer/lib:$LD_LIBRARY_PATH

Then, execute the programs

$ gadd merged.root bb.root cc.root
$ gh_to_th aa.root aa_nogrut.root

etc...
