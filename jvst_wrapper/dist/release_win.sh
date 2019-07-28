#!/usr/bin/env bash

echo 'TODO: change project version in 4 pom.xml files, add project version in 3 .ini files (jvst_native/conf/*.ini) and change version in main.cpp'

#sleep 5

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

# create a (temporary!) shell so that mvn deploy is able to create directories, ...
# prevents the "No such file or directory" error!
#ssh -t daniel309,jvstwrapper@shell.sf.net create
#mvn deploy

#mvn release:prepare
#mvn release:perform