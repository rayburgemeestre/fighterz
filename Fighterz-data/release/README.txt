***************************************************************************

      _______  __         __     __      T A C H Y O N
     |    ___||__|.-----.|  |--.|  |_ .-----..----..-----.
     |    ___||  ||  _  ||     ||   _||  -__||   _||-- __|
     |___|    |__||___  ||__|__||____||_____||__|  |_____|
                  |_____|                                 



********************
* INSTALLATION 
***************************************************************************

---------------------
Compile in WINDOWS:
---------------------

using msvc++: 

 *  Install Allegro version >= 4.0.2 for MSVC
    (link: http://208.247.248.26/~matthew/files/4.0.2/allegro-4.0.2-msvc.zip
     link2: www.google.com)

 *  Open tmpdev.dsp

using djgpp: 

  -- INSERT --

---------------------
Compile in LINUX:
---------------------

Not yet available


---------------------
You can use grabber.exe to maintain the .dat file and it's header (data.h)


********************
* CONTROLS
***************************************************************************

---------------------
Ship Controls
---------------------

Turn the ship:       Arrow keys [LEFT] / [RIGHT]
Move up/down:        Arrow keys [UP] / [DOWN]
Fire:                [Left Control]

---------------------
Function Keys
---------------------

Framerate on/off:    [F6]
Player list on/off:  [F2]
Smooth GFX on/off:   [F3]
Fullscreen on/off:   [F4]
Exit game:           [F12]

---------------------
Other
---------------------

Speech (ON):         [T] (then type the message and press enter to submit)
Type command:        [.] (then type the command)
Cancel Speech or..
  command:           [ESC]

Grid on/off:         [G]
Radar on/off:        [R]

********************
* COMMANDS
***************************************************************************

---------------------
STANDARD
---------------------

.nick <nick>        Change nickname

---------------------
ADMIN
---------------------

********************
* TODO (server&client)
***************************************************************************

---------------------
Higher priority:
---------------------

  - Fix the fucking bots

  - Scores should be kept (vars already reserved for this in code)

  - Multiplay server list (mainserver(will be made in the future) <-> (sub)servers)

  - Options (not only to change fighterz.cfg)
 
  - TEAMS <- important

  - Powerups

  - Different (bouncing)walls etc.

  - Bouncing bullets

  - Sounds (already code available)

---------------------
Lower priority:
---------------------

  - Powerups

  - Server sends map before entering the game

  - Map maker (and put some options in the .lvl file)


