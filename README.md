# jvstwrapper

This is a checkout of https://sourceforge.net/projects/jvstwrapper/, executed on 2019-07-28.

Purpose: recompile the dll to 64-bit.

## Windows

Here is an untested release of the 64-bit dll for Windows:

[jVSTwRapper.dll](jVSTwRapper.dll?raw=true)

You might need to adjust the jvm.dll path inside the plugin config.
See [jaydlay-win.zip](jaydlay-win.zip?raw=true) for an untested example.

## Linux

And here is the 64-bit library for linux (LADSPA, *not* VST):

[jvstwrapper.so](jvstwrapper.so?raw=true)

The linux lib is known to work with LMMS 1.2 (https://lmms.io/).
See [jaydlay-linux.zip](jaydlay-linux.zip?raw=true) for a working example.

## Mac

Contributions are welcome.

