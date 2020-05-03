# kobamac
An extended TBrowser "TBrowserEx" and useful ROOT macros to handle the histograms

## How to install the macros from GitHub
On the terminal, clone the macros by the following command.
```
$ git clone https://github.com/nobukoba/kobamac
$ ls
$ ... kobamac ...
```
Then you can use the macros. You do not have to compile the macros at this moment.

## How to use the macros
On the terminal
```
$ ls
$ ... kobamac ...
$ root
root [0] gEnv->SetValue("KOBAMAC_DIR","./kobamac");
root [1] .x kobamac/root/base/ini_kobamac.C
root [2] TBrowserEx t
```
Then the extended TBrowser opens. Instead of "root[2] TBrowserEx t", "root[2] tbr()" also works.
```
root [2] tbr()

```
If you want to load and execute the macro automatically, you can add the following lines in $HOME/.rootlogon.C.
```
{
...
  /* For kobamac */
  gEnv->SetValue("KOBAMAC_DIR","<path_to_kobamac>");
  gROOT->ProcessLine(".x <path_to_kobamac>/root/base/ini_kobamac.C");
  gROOT->ProcessLine("TBrowserEx t");
}
```
Please note that the <path_to_kobamac> part should be matched to the cloned directory, e.g, <path_to_kobamac>=/home/kobayash/ana/kobamac or <path_to_kobamac>=$HOME/ana/kobamac. In .rootlogon.C, only absolute path should work as <path_to_kobamac>. <path_to_kobamac> can include "$HOME" and "~/". If you do not have .rootlogon.C in $HOME, you can copy $KOBAMAC_DIR/root/base/roootlogon_example.C to $HOME.
