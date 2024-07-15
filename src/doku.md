# DOT MATRIX 
turns your screen into a 5x7 dot matrix display.
And just lets you put some letters on it.

Two modes are supported:
20x10 characters, each 32x48 pixel (default mode)
or
40x20 characters, each 16x24 pixel.

There are 8 pages (0-7).
Use page up/down buttons to change pages.

Every page can have its own mode and color settings.

One of the unusual things about this program is that it displays menus in the same mode and color as the current page.



# Modes of operation:
* intro
* normal page view and edit mode
* menu mode
* sequence mode

# Intro:
The program greets you with an intro animation.

Keybindings while in intro mode:

F3: Restarts the intro animation

F4: Exits the intro but sets all pages to 40x20 mode

F1: Exits the intro and calls up the F1 'main/help' menu

Any other key: Exits the intro (all pages in 20x10 mode)



# Normal page view and edit mode:

Keyboard input:

Each character entered within the character set should bring a character to the screen at the current cursor position. (Depending on the insert or overwrite mode, the following text is moved to the right).

Arrow keys: Moving the cursor

Home ("CLR/HOME", "pos1"): Positions the cursor in the top left-hand corner

End: Moves the cursor to the end of the current line

Page up/down: Moves to the next/previous page

Insert ("INST", "einfg"): Switches between insert and overwrite mode

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

keybingings:

ESC: Exits the menu mode.

All other keybindings are mentioned within the menu.

If a menu item is marked with a right arrow '->', the action is executed immediately.

If a menu item is followed by a '...', the next menu page is opened.




# Sequence mode:

A rudimentary sequencer runs through all pages.

The sequencer can be started with F5 from normal mode or from the sequence-related menus (F6, F7) or from the random menu (F9).

A running sequence is stopped with F5, ESC or with F6, F7.

A running sequence can be paused/resumed with the space bar.

