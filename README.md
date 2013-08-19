GtkTacToe
=========


Info
----

GtkTacToe is my entry into GUI programming in general, and the GTK+-3.0 library in specific. It is also a fully functional player-vs-player tictactoe game. Put them together and what have you got? *gtktactoe*.


Compilation
-----------

To build gtktactoe, you first need to install the dependencies.
```
[sudo] apt-get install libgtk-3-dev
```

Now, get the source code by cloning this git repository.
```
git clone git://github.com/mfabel/gtktactoe.git
```

Then, move into the new gtktactoe directory and run the make utility.
```
cd gtktactoe
make clean
make
```

A long list of warnings will spew forth, but do not fear: this is normal (for now). If you want, use `ls -l` to check for the `gtktactoe` executable. Now, install gtktactoe. You will need to be root for this step.
```
[sudo] make install
```

The binary will now be in /usr/bin and the game data in /usr/share/gtktactoe.


Use
---

To play gtktactoe, simply run `gtktactoe` from a terminal in X (a.k.a xterm or something comprable).

To gtktactoe also supports different command line options. The full list can be found by running `gtktactoe -?`.


Organization
------------

This project is pretty simple internally. The program consists of two files.

* `gtktactoe.c`
    The main loop and GTK interface. In model-view-controller speak, the view and controller live here.

* `engine.c`
    The tictactoe game engine. In model-view-controller speak, this is the model.


Authors
-------

Written by @mfabel in February, 2013.
Greatly expanded by @dejay in May, 2013.
