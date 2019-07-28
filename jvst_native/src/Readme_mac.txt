Writing VST Plugins in Java on the Mac -- Mini How-To
------------------------------------------------------

1. Write your plugin using the wrapper api.

2. Compile your plugin and include it in the Resources folder of the jvstwrapper.vst plugin bundle. 
   The Resources directory will be added to the classpath by default. You can add additional classpath entries 
   in the jVSTwRapper.ini file 
   (WARNING: you could leave this blank, but DON'T do it for now, put anything).

3. Specify your plugin main class in the jvstwrapper.jnilib.ini file 

4. Rename the bundle to YourPlugsName.vst. Also, rename libjvstwrapper.jnilib to YourPlugsName.jnilib. 
   But make sure to keep the .jnilib extension for the wrapper library. ALSO, edit the Info.plist file 
   in the bundle to reflect the new 'CFBundleExecutable' (e.g. YourPlugsName.jnilib) and 'CFBundleName' values.

5. Look for problems. If you left IsLoggingEnabled=1 in the ini file, the log files will appear in the 
   Resources directory and the MacOS directory inside the .vst bundle

--> A fully automated process that generates binaries of you plugin for all 3 supported 
	platforms (linux, windows, osx) just by executing one (maven-) command 
	(again, supported on every platform), can be found here: http://jvstwrapper.sourceforge.net/#multi



Creating an Audio Unit (AU) version of your jVSTwRapper VST plugin 
--------------------------------------------------------------------

1. Install (i.e. copy the bundle) your jVSTwRapper VST plugin 
   to /Library/Audio/Plug-Ins/VST/ or ~/Library/Audio/Plug-Ins/VST/

2. Start the "VSTAU Manager" application, find your plugin in the list and click "install". 
   This will create a wrapper AU plugin for the selected VST plug. 

3. You can find the created AU plugin -- named "YourOriginalVSTPluginName (VSTAU)" -- 
   in ~/Library/Audio/Plug-Ins/Components/ 

4. To distribute your plugin as AU, you need to *** SHIP THE VST VERSION OS WELL *** 
   -- the AU version is a wrapper that loads the VST version when started. 
   So, to distribute your plugin add both, the VST and the AU version 
   to a single archive to download. To install from this archive, simply copy both bundles to the 
   folders /VST or /Components in either /Library/Audio/Plug-Ins/ or ~/Library/Audio/Plug-Ins/ (I.e. 
   the .component bundle goes to either /Library/Audio/Plug-Ins/Components/ or 
   ~/Library/Audio/Plug-Ins/Components/ and the .vst bundle goes to /Library/Audio/Plug-Ins/VST/ or 
   ~/Library/Audio/Plug-Ins/VST/. You can freely choose between both folders.)
 

NOTE:
------
*** Java GUIs require at least Java 1.5.0_07 (Mac OSX 10.4.8 or higher) to be installed on your system ***
Use 'java -version' in a terminal window to check your version, and 'Software Update' to install 
if you have a lower version.

Please report to the forum any host application that causes problems with Java GUIs. I will include them
in my tests. 


*** PLEASE USE THE FORUM FOR QUESTIONS ***
http://sourceforge.net/forum/forum.php?forum_id=318265
