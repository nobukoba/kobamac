# kobamac
An extended TBrowser "HistBrowser" and useful ROOT macros to hadle the histograms

## How to install the macros from GitHub
On the terminal, clone the macros by the following command.
```
$ git clone https://github.com/nobukoba/kobamac.git
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
