# jvstwrapper

This is a checkout of https://sourceforge.net/projects/jvstwrapper/, executed on 2019-07-28.

Purpose: recompile the dll to 64-bit.


## Caveats

Java does *NOT* support combining its GUI (AWT/Swing) with another GUI instance inside the same process. This means
that graphical Java VST plugins may either break VST compliance (by requiring to be run in a dedicated process and not in
the same process as the DAW itself) or either drop graphics support or circumvent the Java provided GUI (for example,
by using some JNI-glued opengl libs or similar). This renders this whole project somewhat a failure - unless we find
some alternative and easy GUI solution. Judging by the code, the Steinberg SDK supports cross-platform control widget:
do we need JNI glue for that to work?


## Windows

Here is a release of the 64-bit dll for Windows:

[jVSTwRapper.dll](jVSTwRapper.dll?raw=true)

It has been tested against Cubase LE AI Elements 10 on Windows 10/x86_64.

You might need to adjust the jvm.dll path inside the plugin config, or make
sure that you have a Java 8 *JRE* installed via some installer - because
only then the Java installation can be found via Windows registry keys.

The JRE 8 .msi installer (release 8.40.0.25) from

https://www.azul.com/downloads/zulu-community/?&version=java-8-lts&os=windows&architecture=x86-64-bit&package=jre

has been verified to work. A JDK won't get noticed by the plugin because 
it uses different registry keys than a JRE. However, manually pointing at
it will work.

See [jaydlay-win.zip](jaydlay-win.zip?raw=true) for examples.


## Linux

And here is the 64-bit library for linux (LADSPA, *not* VST):

[jvstwrapper.so](jvstwrapper.so?raw=true)

The linux lib is known to work with LMMS 1.2 (https://lmms.io/).
See [jaydlay-linux.zip](jaydlay-linux.zip?raw=true) for a working example.


## Mac

Contributions are welcome.

