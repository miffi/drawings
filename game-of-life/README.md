# game-of-life
Conway's "Game of Life" implemented in libtermbox

The program starts paused, so click on some spaces (or press r to get a random layout)
and then press p to unpause and watch the automata unfold.

## Controls
* c - clears the screen and unpauses
* r - randomises the screen
* p - toggles pausing
* q - quits
* left-mouse-press - toggles the cell underneath the mouse pointer

## Known bugs/limitations
### Resizing
The program doesn't handle resizing currently, it still seems to work correctly if resized smaller
but if it's resized bigger, it instantly segfaults, so please keep that in mind.
