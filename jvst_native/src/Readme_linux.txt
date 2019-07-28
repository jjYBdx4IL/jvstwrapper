Installing Java VST plugins on LINUX mini HOW-TO
--------------------------------------------------

NOTE: these steps are only required once. If you got JayDlay (or another jVSTwRapper plugin) 
	to work, any other plugin will work too. 

1. Make sure you have a JAVA_HOME environment variable that is set to the correct value
   e.g. in your .bashrc add: 
   export JAVA_HOME=/usr/lib/jvm/java-6-sun/jre/
   
   If you have a JDK installed (like me), $JAVA_HOME should point to the location of your JRE. This is 
   usually done by appending /jre to the path of your jdk, like in the example above.
   In any case, please check if the path is valid. 
   
2. You need to tweak your LD_LIBRARY_PATH, e.g. add the following line to your .bashrc :
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$JAVA_HOME/lib/:$JAVA_HOME/lib/i386/:$JAVA_HOME/lib/i386/xawt/:$JAVA_HOME/lib/i386/client/
   
   Alternatively, it may work when you add all the paths from LD_LIBRARY_PATH above separately to /etc/ld.so.conf 
   and then execute "sudo ldconfig" afterwards.
      
3. *** CHECK YOUR CONFIGURATION *** 
   by executing "ldd jvstwrapper.so". Your configuration ONLY was successful when the output 
   of this call does NOT contain the text "=> not found" for ANY library that is listed in the output. 
   (i.e. "ldd jvstwrapper.so | grep not" returns zero lines)	
	
4. Fire up Jost, energyXT2, Renoise or any other VST host application and enjoy the JayDLay demo plugin!
   Note: some hosts require a VST_PATH environment variable set to the directory where you stored your plugins (e.g. Renoise), 
   others allow to configure this path in their Preferences dialog (e.g. energyXT2)
 
 
If you see the following error in jvstwrapper_log.txt:
** java.lang.UnsatisfiedLinkError: Can't load library: ### some .so file ###

Create a symbolic link to the directory $JAVA_HOME/lib/i386/xawt/ in the folder where the 
jvstwrapper.so file is. (e.g. execute "ln -s $JAVA_HOME/lib/i386/xawt/ xawt" in the folder where jvstwrapper.so is located)


LADSPA: jVSTwRapper now exposes your plugins as VST and LADSPA plugins at the same time. To use 
        your plug in a LADSPA host (e.g. ardour, rezound, muse, audacity, ...) simply symlink  
        jvstwrapper.so to the directory where your ladspa plugs are (e.g. /usr/lib/ladspa or /usr/local/lib/ladspa).
		e.g. "sudo ln -s absolute_path_to_jvstwrapper.so_file /usr/lib/ladspa/jvstwrapper.so"


	
**** The following steps are for plugin development ONLY ****
--------------------------------------------------------------

5. Write your java plugin using the wrapper API.

6. Specify your plugin main class in the jvstwrapper.ini file 

7. Make sure that the .ini has the same 
   name as the .so file and is stored in the same directory.
   
8. Look for problems. If you left IsLoggingEnabled=1 in the ini file, the log files will 
   appear in the same directory as the plugin .so file is.



**** Troubleshooting ****
--------------------------

Sometimes, adding $JAVA_HOME/lib/i386 to the PATH env variable is also required. So, to be save, add this one as well. 
(to be really save you can add all the dirs you added to LD_LIBRARY_PATH as described above also to PATH :-))

re-check using "ldd jvstwrapper.so" as described in step 3. above


*** PLEASE USE THE FORUM FOR QUESTIONS ***
http://sourceforge.net/forum/forum.php?forum_id=318265
