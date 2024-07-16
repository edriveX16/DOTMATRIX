# DOT MATRIX 
Turn your Commander X16 screen into a 5x7 dot matrix display.

Two modes are supported:
20x10 characters, each 32x48 pixel (default mode)
or
40x20 characters, each 16x24 pixel.

There are 8 pages (0-7).
Use page up/down buttons to change pages.

Every page can have its own mode and color settings.

One of the unusual things about this program is that it displays menus in the same mode and color as the current page.

# Preconditions:

The program requires a Commander X16 with the default 65C02S CPU, which supports commands such as smbx, bbsx etc.. 
Or as an alternative the emulator 'x16emu'. Further information can be found at:

https://github.com/x16community/x16-emulator


# How to run the program:

Copy the binary file ("DOTMAT.PRG") to an SD card and insert it into the X16.
Navigate to the corresponding directory on the X16.

Load the program with ' LOAD "DOTMAT.PRG" '.
And start with 'run'.


Note:

If the file "DOTMAT.TXT" exists in the same working directory, the program will attempt to load its contents on page 0 when it is started.
Otherwise an attempt is made to create an empty "DOTMAT.TXT" (more details in the F2 file menu).


# Modes of operation:
* intro
* normal page view and edit mode
* menu mode
* sequence mode

# Intro:
The program greets you with an intro animation.

Keybindings while in intro mode:

F3: Restarts the intro animation.

F4: Exits the intro but sets all pages to 40x20 mode.

F1: Exits the intro and calls up the F1 'main/help' menu.

Any other key: Exits the intro (all pages in 20x10 mode).



# Normal page view and edit mode:

Keyboard input:

Each character entered within the character set should bring a character to the screen at the current cursor position. (Depending on the insert or overwrite mode, the following text is moved to the right).

Arrow keys: Moving the cursor.

Home ("CLR/HOME", "pos1"): Positions the cursor in the top left-hand corner.

End: Moves the cursor to the end of the current line.

Page up/down: Moves to the next/previous page.

Ins ("INST", "einfg"): Switches between insert and overwrite mode.

Del: Deletes the character at the current cursor position. (Depending on the insert or overwrite mode, the following text is moved to the left).


F1: Opens the 'main/help' menu

F2: Opens the 'file' menu

F3: Opens the 'edit' menu

F4: Switches the current page directly between 20x10 and 40x20 mode

F5: Directly starts (or stops) the sequencer

F6: Call up the 'sequencer options' menu

F7: Call up the 'sequencer step time' menu

F8: Call up the 'options' menu

F9: Call up the 'random' menu

F10: Call up the 'cursor options' menu

F11: Call up the 'color settings' menu




# Menu mode (all menus):

The menu mode is displayed with a title bar like:

'[=== Menu item ====]'

The menus are displayed in the same mode and with the same color settings as the current page.

All menu functions are the same in both modes (20x10 or 40x20), with the following exceptions:

- Some menus have more detailed text in 40x20 mode.
- The X16 logo is only available in 20x10 mode (applies to the F8 menu).
- Fewer color setting options are available in 40x20 mode (applies to the F11 menu).

Keybindings:

ESC: Exits the menu mode.

All other keybindings are mentioned within the menu.

If a menu item is marked with a right arrow '->', the action is executed immediately and the menu is exited.

If a menu item is followed by a '...', a next menu page is opened.




# Sequence mode:

A rudimentary sequencer runs through all pages.

The sequencer can be started with F5 from normal mode or from the sequence-related menus (F6, F7) or from the random menu (F9 -> F11).

A running sequence is stopped with F5, ESC or with F6, F7.

A running sequence can be paused/resumed with the space bar.



# Menus in detail:

## F1 Main/Help Menu
Press F1 repeatedly to scroll through 3 menu pages.

'menu 1 / 2' -> 'menu 2 / 2' -> 'character set' -> (exit menu)'

Menu pages 'menu 1 / 2 and 2 / 2' show the possible keybindings in normal (view/edit) mode.

'character set' shows the displayable characters that can be entered via the keyboard.

Keybindings:

The buttons specified in 'menu 1 / 2 and 2 / 2' function in the same way as in normal view/edit mode.
The corresponding menus are described below.


## F2 file Menu
Keybindings:

Q or q: Terminates the program and resets the X16.

F2: Saves the text (only) of the current page in "DOTMAT.TXT". An existing file will be overwritten without further inquiry.


F3: Reads the file "DOTMAT.TXT" into the current page.

Notes:

With "DOTMAT.TXT", 800 bytes are always written or read (without CBM style 2byte header and independent of the mode).
In case you want to create a "DOTMAT.TXT" before starting the program, be sure it is exacltely 800 byte. Otherwise the program may react incorrectly.
The filename is hardcoded (sofar).
No color or mode information is saved.



## F3 edit Menu
Displays the current overwrite or insert status and offers basic functions for deleting and copying.

Keybindings:

Ins ("INST", "einfg"): Switches between insert and overwrite mode (does not close the menu).

F3: Deletes the text of the current page (colors and mode remain).

F4: Deletes the text of all pages (colors and mode remain).

Note:

During deletion using F3 or F4, a vertical erase line wipes across the screen from left to right.


F5: Copies the text of the current page to the next page and switches to this page (does round robin from page 7 to 0).

Hint:

To copy between non-consecutive pages, the save and load functions (F2 menu) can be used.


## F4
(F4 has no menu, F4 switches between 20x10 and 40x20 mode.)

## F5
(F5 has no menu, F5 starts or stops the sequencer.)


## F6 Sequencer options menu
Keybindings:

F3: Selects whether the page number indicator should be shown when a page change takes place in a running sequence. Appears in the top right-hand corner and fades out after a few seconds.

F4: Selects whether the indicator for a running sequencer is displayed. (A rotating "/" is displayed in the bottom left-hand corner. Rotation is stopped when paused.).

F5: Starts the sequencer.

F7: Go to the F7 menu to set the step times.

Note:

The keys mentioned below the dashed line are only intended as a reminder of the button assignment when the sequencer is running.



## F7 Sequencer step time menu
Sets the display duration for the pages when the sequencer is running.

The display duration can be set individually or the same for all pages in steps of 1 second.

The range is 1 to 120 seconds, default from program start is 10 seconds.

The page for which the setting is made is displayed at the top "step time page X" (X= page no.).

The display time for this page is shown below this.

When the menu is called up, it starts with the current page.


Keybindings:

Arrow up / down: Increase / decrease steptime of the displayed page number.

Page up / down: Switch to the next / previous page (without leaving the menu).

F5: Starts the sequencer (not mentioned when in 20x10 mode).

F6: Go to the F6 menu to set the sequencer options.

F10: Sets the step time of all pages to the same time currently displayed.

Notes:

Since the program always displays menus with the color settings and mode of the current page,
it is possible that the menu appearance changes when you change pages within the menu.

If you leave the F7 menu ("ESC"), you switch to the page whose time setting was last edited.



## F8 options menu
Some gerneral options

Keybindings:

F2: Inserts a page number in the form "page X" in the top left-hand corner of each page (while overwriting existing text).

F5: Show non-printables: Marks characters with values outside the character set with a dot instead of a space (applies to all pages), when on.

However, as only valid characters can be entered, this only applies if the "DOTMAT.TXT" file has been edited externally.

It is more or less just a possibly no longer complete test function from the early development period.

F8: Only in 20x10 mode: Switches the logo on or off (when in 40x20 mode the logo is not available).



## F9 radom menu
Functions to randomize page settings and/or to create pseudo random garbage text or pattern.
Keybindings:

F5: Generate random text on the current page.

F6: Generate random text on all pages.

F7: Generate random pattern on the current page.

F8: Generate random pattern on all pages.

Note: F5 - F7 don't change the mode or color settings.

F9: Randomize the used color preset and mode (20x10 or 40x20) of the current page.

F10: Randomize the used color preset and mode (20x10 or 40x20) of all pages.

Note: F9 and F10 don't change the text.

F11: Randomize everything and start the sequence. Fills each page with a random variation of random text or pattern. Selects a random color preset, a random mode and a random step time for each page. The sequencer is then started.

Note: The random step time is limited to a maximum of 10 seconds.



## F10 cursor menu
Some options for the cursor, changes can be viewed directly in the menu.

Keybindings:

F8: Switches the cursor between always on and automatic (disappears after a few seconds of inactivity).

F9: Switches between different cursor colors.

Note: One of the cursor colors is invisible, this can be used as an "always off" option.

F10: Switches between different erase line colors (The erase line is the vertical line that wipes across the screen when you use the erase functions of the F3 edit menu).

Note:

The color settings of the cursor are saved independently on each page for both modes (20x10 or 40x20). E.g. you can have a red cursor on page 1 in 20x10 mode and a green cursor on the same page 1 in 40x20 mode.



## F11 color menu
Keybindings:

Page up / down: Switch to the next / previous page (without leaving the menu).

F4: Switches through 8 predefined color presets (includes cursor colors).


F5: Changes the dot color when the dot is lit.

F6: Only in 20x10 mode: Changes the dot color when the dot is off.


F7: Changes the background color.

F8: Only in 20x10 mode: Changes the dot style (round, squared, thin, star).


F10: Applies the current color and mode settings to all pages.

Notes:

None of these color settings affect the text.

When in 20x10 mode and using the F5 - F7 functions it is likely to get a color combination which is hard to read.
Then try using F5 and F6 or F7 and F8 alternately. Or select another preset with F4 to restart from something readable.

A few combinations of dot color when on (F5) and when off (F6) are dependend on each other.
The same applies with F7 and F8.

When in 40x20 mode less dot colors and background colors are available with F5 and F8.

If you leave the F11 menu ("ESC"), you switch to the page whose color settings was last edited.

