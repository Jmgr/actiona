## 3.11.1 (13/04/25)
### Global
- Added support for "make install"
- Windows: fixed stdout and stderr not being printed in the command prompt
- Linux: added metainfo, fixed installation of file in standard places

### Interface
- Fixed issue in the code editor where validation would fail without reason - [@deoxify](https://github.com/deoxify)

### Code
- Fixed Console not being available when using actexec
- Fixed missing constructor for File
- Fixed inability to call static methods in Process

### Contributors
Thank you very much to all the beta testers: IceLab, MDLE46, The Poupouillou, Fredy123, J_Game, Pixfus and potdemiel2303.

Thanks to [@deoxify](https://github.com/deoxify) for contributing to this release!

## 3.11.0 (14/03/25)
### Global
- Improved loading times (migrated actions from plugins to libraries)
- Added a Random action
- Upgraded to Qt6
- Now hiding Actiona when a screenshot is taken
- Windows: improved screenshooter
- Added procedure jump checks
- Linux: fixed a crash when X11 is not available (display an error message and close instead)
- Fixed a crash in the FindImage action when the image could not be found and the "if not found" action was a goto
- Fixed a potential memory corruption in the FindImage action and Image code
- Fixed a bug when writing a script where a "version" property was missing, prevent the script from being open with older versions of Actiona
- Fixed a crash in the FindImage action when the input image was using indexed colors
- Fixed code action being blocking if no code was entered
- Linux: fixed pixel color
- Linux: fixed percentage position calculation
- Fixed a crash when reducing the number of recently opened files
- Fixed actions not starting if a short pre-pause is used
- Removed script format validation

### Interface
- Added support for dark and light themes
- Added "stop execution" to if actions
- Added a max entries to the console window
- Added current line in execution window
- Settings UI: fixed not being able to specify a key and a modifier
- Fixed a bug in the text evaluation where multiple variables would not be correctly detected
- Code editor: highlighting: updated used and reserved JS keywords
- Code editor: fixed line number display - [@deoxify](https://github.com/deoxify)
- Code editor: removed current line highlight
- Code editor: made the code editor use a monospace font
- Code editor: updated colors to be compatible with dark and light themes - [@deoxify](https://github.com/deoxify)
- Fixed console window glitch when running a small script
- Linux: fixed screenshot wizard
- Linux: fixed window selection

### Code
- Added an option to the Web class to send raw POST data
- Fixed an issue with some code classes that could not be created by an existing instance of that class
- Fixed an issue with code classes where parameters containing other code classes could not be used
- Fixed an issue when code classes triggered an error in their constructors
- Fixed a bug when searching through windows using Code, where all windows would be selected every time
- Linux: fixed screen capture in Image

### Contributors
Thank you very much to all the beta testers: IceLab, MDLE46, The Poupouillou, Fredy123, J_Game, Pixfus and potdemiel2303.

Thanks to [@deoxify](https://github.com/deoxify) for contributing to this release!

## 3.10.2 (28/10/23)
### Global
- Fixed a crash when running a script containing only two actions without pauses
- Fixed a bug where some actions weren't stopped when a script was interrupted
- Fixed a bug where the main window of Actiona could be hidden when an exception was triggered during the execution of the first action
- Windows: the minimum version is now Windows 7
- Windows: disabled support for 32-bit versions of Windows
- Windows: fixed a bug preventing the opening of scripts from the file explorer
- Fixed a bug where a progress window could appear during the editing of certain actions
- Increased the accuracy of many timers, including the duration of the Pause action
- Added pause/resume support for the Pause action
- Fixed a bug when calculating execution durations
- Fixed bugs in the heatmaps
- Many minor code corrections

### Interface
- The "Ok" button is now always the default selected when editing an action
- Removed input/output parameter fields in actions as they interfered with the OS's color theme
- Removed unused question mark buttons in dialogs
- Added Chinese translation, thanks to Asuka Minato
- The execution window can now be moved

## 3.10.1 (24/03/20)
### Global
- Unixes: fixed compilation using OpenCV 4
- Unixes: fixed compilation on unixes other than Linux
- Fixed compilation using recent Qt versions
- Windows: fixed compilation using MinGW

### Interface
- Unixes: fixed broken global shortcuts

## 3.10.0 (17/07/18)
### Global
- Added a keyboard key condition action that allows checking or waiting for a particular key combination to be pressed
- Added a text to speech action that uses the operating system's voice synthesis capabilities
- Click action: added an option to reset the cursor position to its former position after the action has finished executing
- DataInput action: the output variable now contains an empty string if the user cancels the data input dialog

### Interface
- Added filters allowing to find an action within the script or within the list of available actions
- Added an action heat map that allows viewing the execution count and time spent per action (available after the first script execution)
- Improved program loading times
- Fixed missing script line numbers (regression from previous version)
- Fixed a slowdown when copy and pasting multiple actions within a big script
- Fixed a possible bug on the console when running a script for longer than a day
- Fixed a few minor bugs

## 3.9.4 (11/05/18)
### Global
- Fixed a slowdown occurring when editing an action within a big script
- Fixed non-translated buttons
- Fixed various memory leaks
- Windows: now using Qt 5.6 to build official binaries => removed Webkit web browser, but reduced installer and SFX size

### Installer/Packages
- Windows installer: fixed missing .dll files used by SQL drivers (like MySQL)

## 3.9.3 (28/10/17)
### Global
- Windows: fixed compatibility with Windows 10
- SendMail action: fixed a bug where no exception was thrown when an error occured when sending the e-mail
- Click action: fixed not specifying a position would move the mouse cursor to the upper-left screen position
- Procedures: fixed a bug where actions were reset when a procedure ends
- TimeCondition action: fixed the "if after" and "if before" labels not being intuitive (replaced by "if in the future" and "if in the past")
- MultiDataInput action: fixed stretching occuring at the wrong place
- DataInput action: fixed default value being capped at 99
- Windows: fixed Actiona still writing an entry into the registry when starting with the --portable parameter

### Interface
- Linux: added a warning when starting Actiona on a non-X11 session

## 3.9.2 (28/05/16)
### Global
- Windows: fixed mouse and keyboard simulation on some applications (started with admin privileges)

### Code
- fixed loadUI and include functions not using the current script or code filename directory as working directory
- added the Execution.filename readonly property, containing the current script or code filename
- ActionPackSystem, Debian/Ubuntu/...: fixed a crash when using the System object
- ActionPackData: fixed crash when trying to read a non-existing key

### Installer/Packages
- Windows installer: fixed setup of PATH environment variable and removal when uninstalling

## 3.9.1 (04/11/15)
### Global
- Windows: fixed click simulation failures in some cases
- fixed a crash when canceling a script execution while a key was being pressed
- fixed SFX scripts not being run when a local installation of Actiona was required

### Interface
- Windows: fixed keyboard shortcuts not working
- Debian/Ubuntu/...: fixed a bug preventing the main window to be displayed on recent distribution versions
- fixed a crash when right-clicking a "go to a line" exception field
- fixed a bug preventing the selection of a pixel on the screen when the Actiona window was on top of it
- fixed a bug where the save script progress dialog would stay open when exporting an SFX script

### Code
- fixed not being able to display the value of an array variable in a text field
- fixed console output by way of code not being displayed until after a call to sleep() or pause()
- added "pretty printing" of Array and Object

## 3.9.0 (22/06/15)
### Global
- renamed Actionaz to Actiona (sounds better in French)
- now using Qt5 instead of Qt4 (minimal version is 5.2)
- ActionDataInput: added editor type (line, multiline or password)
- added a SendMail action
- Windows: added a "no unicode character" option to the WriteText action, should allow for a limited text writing in some applications like Remote Desktop
- fixed DataInput & MultiDataInput not opening URLS in their question fields
- fixed a crash when using the ChooseWindow button (fixes #31)
- Linux: fixed missing F1-F35 keys in the KeyMapper
- WriteIniFile: fixed a bug that prevented the creation of new ini files
- Windows: fixed keyboard keys not being simulated on some applications (Remote Desktop for example) Note that this doesn't include simulating unicode text input, for which there is, afaik, no solution

### Interface
- enabled secure connections for checking Internet connectivity, fetching update information and downloading new versions
- added a language change setting
- now checking if the SFX files are available to enable/disable the export SFX feature
- fixed a bug preventing saving the values of some settings (caused by the Qt5 migration): users will see those settings reset to their default values
- fixed a bug in localized tab names in action dialogs
- removed unused context help button on dialogs

### Code
- added a Mail object
- added ProcessHandle::parentId()
- added a MultilineText option to the InputDialog object
- added Console.clear()
- Image: added the pixels() and pixelData() methods to retreive the pixel values at once

### Internal
- replaced RudeConfig with boost::property_tree
- replaced Qt's foreach by C++11's range-based for

## 3.8.0 (09/03/14)
### Global
- added in-script resources
- added an Image parameter
- added a percent positioning to the position parameter
- set the positions as floating point
- added the evaluation of text-mode variables as non-text if they are the only entry of a parameter
- fixed some bugs in variable insertion in parameters
- replaced input masks by validators in position and color parameters
- fixed the default value in DateTime parameters
- VariableCondition: added a Contains operation allowing to test for the presence of a string in a string list
- VariableCondition: corrected the comparison of values, allowing for more intuitive comparisons
- CursorPath: added a mouse button that can be pressed during the action execution
### Interface
- added a screen capture wizard
- added the insertion of variables & resources in the code editor
- added a progress bar to the loading and saving of scripts
### Code
- added Script.callProcedure
- SQL: corrected a bug that ran a SQL request two times when using exec() with a non-prepared request

## 3.7.0 (28/06/13)
### Actions
- added a button to insert detected variables into parameter fields
- added a more explicit error message when a list field is empty
- fixed a bug when opening a script that had been created using a previous version of ### Actiona: new action parameters did not have default values
- corrected a bug that prevented using the Code Error exception to skip the current action or go to a line of the script
- Pixel Color: added an offset to the position to be checked
- Read INI File: added reading the whole file at once
- Read INI File: corrected a bug that froze the execution when trying to read a file with an empty filename
- Find Image: added a field allowing to retreive the confidence value
- Find Image: added a field to choose which matching method to use
- Find Image: added a delay between two searches when waiting
- Find Image: added two conditions actions "if found" and "if not found"
- Find Image: removed the "Image not found" exception, previous scripts will be automatically upgraded to use the "if found" and "if not found" parameters
- Find Image: search can now be done in multiple windows if they satisfy the search criterion
- Find Image: renamed "Down pyramids" to "Downsampling"
- Find Image: fixed a bug that prevented searching in some screens when using a multiple screen configuration
- Find Image: fixed a bug that prevented using an absolute position when searching within a window
Code:
- Script: added the line attribute
- Image: added the Image.takeScreenshotUsingScreenIndex function that allows taking screenshots using a screen index
- Image: added a parameter to the findSubImage functions to choose which matching method to use
- Image: fixed a bug that prevented searching in some screens when using a multiple screen configuration
- Image: fixed a bug in the Image.takeScreenshot function that took only a screenshot of the main screen when using a multiple screen configuration

## 3.6.2 (04/05/13)
- Actions: fixed a regression that caused a memory leak and a crash after a repeated use of the Find Image action
- Code: fixed a bug that caused a memory leak and a crash after a repeated use of Image.findSubImage and similar

## 3.6.1 (04/03/13)
- Actions: fixed a bug in the in-text variable insertions that prevented using backslashes

## 3.6.0 (03/03/13)
- Actions: added a position offset in the Click, Move cursor and Cursor path
- Actions: added an action to read environment variables
- Gui: fixed a bug where pause before and after each action were not reset after creating a new script
- Code: added the File.exists() function
- Actions: upgraded the in-text variable insertions to allow inserting table elements
- ActExec & Gui: fixed a bug that prevented the translation of the parameters usage
- Actions: fixed a bug in the Command action that prevented getting the standard output and standard error texts
- Actions: fixed a bug that prevented using Date objects in the Time Condition action
- Actions: fixed a bug in the Find image action & code that could crash the program in certain cases

## 3.5.1 (28/12/12)
- ActExec: fixed a bug that prevented the use of the pauseBefore and pauseAfter script parameters
- Actions: fixed a bug that involved setVariable to create variables of type Object and not of the values type
- Actions: fixed a bug preventing comparison of variables other than strings in the VariableCondition action

## 3.5.0 (08/12/12)
- Actions: added a parameter to choose the key press amount in the Key action
- Actions: added a parameter to trigger a pause between key presses to the Text action
- Actions: setting the default value for "down pyramid count" to 1 in the Find Image action
- Code: added a delay to the writeText function

## 3.4.2 (27/05/12)
- Actions: corrected a bug in the Time Condition action
- Code: corrected a bug preventing Mouse functions to work with other buttons than the left one
- Code: corrected bugs in the waitForX Process functions
- Code: corrected bugs in code classes using enums as function parameters

## 3.4.1 (10/04/12)
- Executer: fixed a bug increasing the pause before and after an action
- Gui: removed spaces at begin and end of imported scripts to prevent an XML import error
- ActionTools: corrected a bug where a newer version of the script file schema could be seen as a corrupted script

## 3.4.0 (08/04/12)
- Actions: added three actions allowing the use of procedures
- Actions: added a new "If action" action allowing to call a procedure
- Actions: deprecated the x-coodinate, y-coordinate, width and height parameters in the Window condition action
- Actions: added position and size parameters in the Window condition action
- Packaging: fixed Debian bug #667093 (http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=667093)
- Gui, Console: clicking on a log console item now selects the corresponding action
- Gui: fixed a bug where the execute selection button was disabled after an execution even if an action was still selected
- Gui: fixed a bug where clicking on a console item could select/open another action than the one concerned by the item
- ActExec, Windows: added a program parameter to show a console
- fixed some warnings due to useless & dangerous calls to deleteLater (close() auto-deletes QWidgets by default)

## 3.3.0 (29/03/12)
- Actions: added a window title to the DataInput action
- Actions: added new action MultiDataInput
- Actions: added a text mode to the Messagebox action
- fixed a bug crashing the application when canceling the execution when the debugger is open and a MessageBox action is shown

## 3.2.2 (29/01/12)
- Code: fixed getter functions having the same name as properties
- Packaging: removed compiler options to allow compilation on non-x86 platforms
- removed 7z binaries from repository

## 3.2.1.1 (16/01/12)
- Packaging: dependency on OpenCV is now done using Pkgconfig

## 3.2.1 (14/01/12)
- Actions: added a tolerance value for the Pixel color action
- Actions: fixed a bug preventing to enter no data in a number parameter
- Actions: fixed a bug preventing to enter integer or decimal values below 0 or over 100
- Windows: fixed a bug during installation preventing the use of image formats other than PNG
- fixed a bug corrupting the Actiona window when using the Exit Actiona action
- fixed some memory leaks

## 3.2.0 (06/01/12)
- Actions: added the FindImage action
- Actions: added an option to use window borders to the Window action
- Actions: added Point (position) and Color variable types to the Variable action
- Actions: fixed a bug in the Window Condition action, blocking if "goto a line" was selected in "if false"
- Gui: updated the settings dialog to show the last opened tab
- Gui: fixed check for updates parameter not being hidden when updates are disabled
- Gui: now hiding exceptions when no parameters are shown
- Executer: fixed a bug crashing Actiona when using the Stop action

## 3.1.1 (30/12/11)
- Code editor: added Ctrl+Return shortcut to close the code/text editor
- Code editor: added Shift+Tab to unindent the selection in the code/text editor
- Code editor: improved the Code syntax highlighter
- Code: fixed a crash when using the Web object
- Code: fixed Execution.sleep() and Execution.pause() not working under Linux
- Code: fixed possible crash when using Execution.sleep() and Execution.pause()
- Windows: fixed a bug in the installer preventing the use of SFX scripts

## 3.1.0 (21/12/11)
- added edition of actions that are not compatible with the current OS
- added modifier keys in the Key action

## 3.0.1 (11/12/11)
- Gui: replaced GroupBoxes by invisible widgets (increased readability)
- Code: fixed a bug that could cause crashes when using events
- Code: added Image.findSubImage()
- Code: added Stdio.println(), Stdio.printlnWarning() and Stdio.printlnError()
- Gui, Linux: set window opacity only if the compositing manager is running
- Code, Windows: improved the File object
- Actions: fixed a bug in WriteIniFile where the INI file was overwritten on each edit

## 3.0.0 (13/11/11)
- official stable release

## 0.1.3 (06/11/11)
- ActExec: added proxy parameters
- ActExec: using system proxy by default
- fixed a bug with action parameter evaluation during execution
- Code: added Window.foreground()

## 0.1.2 (08/10/11)
- fixed a bug concerning the last opened script
- fixed a bug when jumping to another line of the script
- added a WebDownload action
- added a Web code object
- fixed a bug in the cursor path actions
- added a field to the MessageBox and DataInput actions to specify the title icon to use
- Sfx: added a "requires Actiona" option
- Sfx: added a runner executable to warn about 64 bit compatibility issues

## 0.1.1 (24/09/11)
- corrected a bug involving the "goto line" action when the tested pixel was not equal to the wanted value
- added a new contributor to the about dialog
- Windows: added a missing mediaservice dll used to play sound & videos
- fixed a bug when validating a script file with non-ascii characters
- ActionMessageBox: added a custom icon choice
- Gui: added an option to use the system proxy
