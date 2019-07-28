#!/usr/bin/env bash

mvn clean
rm ChangeLog.txt*
rm fb.jpg*
rm style.css*
rm READ_ME.html*

export CVSROOT=:pserver:anonymous@jvstwrapper.cvs.sourceforge.net:/cvsroot/jvstwrapper
mkdir target
cd target

cvs login
cvs co jvst_wrapper
cvs co jvst_system
cvs co jvst_examples
cvs co jvst_native

cp ../cvs2cl.pl .
./cvs2cl.pl -f../ChangeLog.txt

cd ..

wget -O READ_ME.html http://jvstwrapper.sourceforge.net/index.html
wget http://jvstwrapper.sourceforge.net/style.css
wget http://jvstwrapper.sourceforge.net/fb.jpg

mvn install javadoc:jar package assembly:assembly

#mvn deploy
#mvn release:prepare
#mvn release:perform
