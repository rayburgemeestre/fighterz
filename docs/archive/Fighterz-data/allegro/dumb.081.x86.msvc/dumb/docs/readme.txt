/*  _______         ____    __         ___    ___
 * \    _  \       \    /  \  /       \   \  /   /     '   '  '
 *  |  | \  \       |  |    ||         |   \/   |       .      .
 *  |  |  |  |      |  |    ||         ||\  /|  |
 *  |  |  |  |      |  |    ||         || \/ |  |       '  '  '
 *  |  |  |  |      |  |    ||         ||    |  |       .      .
 *  |  |_/  /        \  \__//          ||    |  |
 * /_______/edicated  \____/niversal  /__\  /____\usic /|  .  . astardisation
 *                                                    /  \
 *                                                   / .  \
 * readme.txt - General information on DUMB.        / / \  \
 *                                                 | <  /   \_
 *                                                 |  \/ /\   /
 *                                                  \_  /  > /
 *                                                    | \ / /
 *                                                    |  ' /
 *                                                     \__/
 */


********************
*** Introduction ***
********************


Thank you for downloading DUMB! You should have the following documentation:

   readme.txt     - This file
   licence.txt    - Conditions for the use of this software
   release.txt    - Release notes and changes for this and past releases
   docs/howto.txt - Step-by-step instructions on adding DUMB to your project
   docs/faq.txt   - Frequently asked questions and answers to them
   docs/dumb.txt  - DUMB API Reference
   docs/ptr.txt   - Quick introduction to pointers for those who need it
   docs/fnptr.txt - Explanation of function pointers for those who need it

This file will help you get DUMB set up. If you have not yet done so, please
read licence.txt and release.txt before proceeding. After you've got DUMB set
up, please refer to the files in the docs/ directory at your convenience. I
recommend you start with howto.txt.


****************
*** Features ***
****************


Here is the statutory feature list:

- Supports playback of IT, S3M and XM files

- Should be very faithful to the original trackers, especially IT; if it
  plays your module wrong, please tell me! (Bear in mind that a few features
  aren't implemented yet though)

- Very accurate timing and pitching; completely deterministic playback

- Facility to embed music files in other files

- Five resampling quality settings, from aliasing to cubic with low-pass

- Number of samples playing at once can be limited to reduce processor usage,
  but samples will come back in when other louder ones stop

- All notes will be present and correct even if you start a piece of music in
  the middle

- Highly portable and versatile

- Can be used with Allegro, can be used without (if you'd like to help make
  DUMB more approachable to people who aren't using Allegro, please contact
  me)


Since this is the Features section, it seems appropriate to feature a
conversation fragment from IRC EFnet #allegro. If this doesn't inspire you to
set up IRC, I don't know what will! I am 'enthehDUH'.

<enthehDUH> ok, I'm undecided about this
<enthehDUH> I need to choose a default value for dumb_resampling_quality
<enthehDUH> 0 - aliasing
<enthehDUH> 1 - linear interp
<enthehDUH> 2 - linear interp + linear average when downsampling
<enthehDUH> 3 - quadratic interp + linear average when downsampling
<enthehDUH> 4 - cubic interp + linear average when downsampling
<enthehDUH> what do you think? :)
<tero`> best quality
<enthehDUH> that'd be 4
<Inphernic> that quadradigdog thingie sounds good :)
<enthehDUH> but a 486 will have trouble doing 4 :)
<enthehDUH> an early Pentium probably would too
*** Signoff: tero` has left IRC [bye for now]
<enthehDUH> I'm gonna make it 4, I think
<Inphernic> well, make it detect the cpu and make the decision then
<enthehDUH> since the general trend is for computers to get better ;>
<enthehDUH> no, that'd add bloat :O
<Inphernic> bah
<enthehDUH> if I make it 4, ppl will listent to DUMB and say, "hey, this is
   pretty good :)"
<enthehDUH> :)
<enthehDUH> *listen
<Inphernic> what's a little bloat for the good of the mankind :)
<enthehDUH> if I make it 0, ppl will say, "hey, [that other module player]'s
   better :/"
<enthehDUH> :/
<enthehDUH> :P
<Inphernic> um, make the "detection" smaller, like if user_cpu <= PENTIUM
   d_r_q=3; else d_r_q=4;
<enthehDUH> no
<enthehDUH> let the user just set dumb_resampling_quality ;)
<enthehDUH> the examples will have that option built in by the time I've
   finished with them :)
<Inphernic> "Hello and welcome to the DUMB setup! Now you get to choose from
   a variety of options which even the author himself isn't sure of. Now
   please enter the value you wish to use for hypercarbonated atomic audio
   resampling fusion engine's exhaust port's manifold torque :"
<Inphernic> :)
<enthehDUH> :)
<enthehDUH> that's a classic
<enthehDUH> I'm putting it in the docs :)
<Inphernic> :D
<enthehDUH> want ur name in there? :)
<Inphernic> for what? it's not like i've written a single line of code for
   you project? :)
<enthehDUH> no, I mean do you want your name beside the quote :)
<Inphernic> do what you want :)
<enthehDUH> cool :)
<enthehDUH> now this is where ur supposed to tell me to link to ur website ;>
<Inphernic> eek :)
<Inphernic> http://edu.kauhajoki.fi/~juipe/Infini
<enthehDUH> muahahahahhaa
<Inphernic> you're planning something ^Bevil^B and ^Bdevious^B
<enthehDUH> aw crap, how the #%&$ am I supposed to quote bold in the docs :P
<Inphernic> my turn to say muahahahahhaa >D
<enthehDUH> ;>
<Inphernic> oh well, back to.. stuff
*** Inphernic is now known as Inph^Away


*********************
*** What you need ***
*********************


To use DUMB, you need a C compiler. If you have Allegro, DUMB can integrate
with its audio streams and datafiles, making your life easier. If you do not
wish to use Allegro, you will have to do some work to get music playing back.
The example programs require Allegro.

   Allegro - http://alleg.sf.net/

In order to use DUMB with Allegro, you must make sure you have built both the
optimised and debugging libraries of Allegro; see Allegro's documentation for
details.

If you are using Microsoft Visual C++, you will have to obtain a port of GCC
and GNU Make too. I recommend one of the following:

   DJGPP - http://www.delorie.com/djgpp/
   MinGW - http://www.mingw.org/

The first is a DOS compiler, so some Windows users will be unable to use it.
The second should work on all Windows systems. In particular, you need 'make'
and 'gcc'. MinGW comes as a single download which contains these tools, while
you have to download them separately for DJGPP. GNU Haters, do not concern
yourself; the library will consist only of MSVC-compiled code. Read on for
details of why GCC and GNU Make are required.

Windows executables have always had their command lines limited to 126
characters, but building DUMB involves command lines longer than this. It is
necessary to use GNU Make, which can pass long command lines to the programs
it uses if they are compatible. A special program called 'runner.c' (which
you can find in the src/misc/ directory) is used to take these long command
lines, write them to a temporary file, and pass the file to MSVC as a script.
GCC is used to compile 'runner.c' so it can take these long command lines
from GNU Make.


********************************************
*** How to set DUMB up on DOS or Windows ***
********************************************


You should have got dumb.zip. If for some reason you got dumb.tar.gz instead,
you may have to use utod on some or all of the text files (certainly to load
them in Notepad, although compilation probably won't be affected). If this
confuses you, just go and download dumb.zip instead.

Make sure you preserved the directory structure when you extracted DUMB from
the archive. Most unzipping programs will do this by default, but pkunzip
requires you to pass -d. If not, please delete DUMB and extract it again
properly.

If you are on Windows, open an MS-DOS Prompt or a Windows Command Line.
Change to the directory into which you unzipped DUMB.

Type ONE of the following lines, depending on which compiler you want to use:

   fix djgpp
   fix mingw32
   fix msvc

If you have Allegro, type the following:

   makeall

If you do not have Allegro, type the following:

   makecore

That's right, no spaces! These are batch files that will build and install
both the debugging and optimised libraries for you. Trust me - unless you are
really experienced with DUMB, you need the debugging library.

Please let me know if you have any trouble. Also note that the procedure may
change in future releases.

Once this step is complete, please refer to howto.txt to start using DUMB! If
you use DUMB in a game, let me know - I might decide to place a link to your
game on DUMB's website :)


**********************************
*** How to set DUMB up on Unix ***
**********************************


You should have got dumb.tar.gz. If for some reason you got dumb.zip instead,
you may have to use dtou on some or all of the text files. You may also have
to use chmod on some of them. If this confuses you, just go and download
dumb.tar.gz instead.

To set DUMB up for Unix, first type this in the directory containing DUMB:

   ./fix.sh unix

Then, if you have Allegro, type the following:

   ./makeall.sh

If you do not have Allegro, type the following:

   ./makecore.sh

These scripts will build and install both the debugging and optimised
libraries for you. Trust me - unless you are really experienced with DUMB,
you need the debugging library.

The scripts will install into the /usr/local/lib and /usr/local/include
directories by default. You can use the --prefix option to override that, for
example:

   ./makeall.sh --prefix=${HOME}

This would install the library into your home directory instead.

Please let me know if you have any trouble. Also note that the procedure may
change in future releases.

Once this step is complete, please refer to howto.txt to start using DUMB! If
you use DUMB in a game, let me know - I might decide to place a link to your
game on DUMB's website :)


**************************
*** Using the examples ***
**************************


The examples will only have been compiled if you have Allegro. There is a
separate example player for each supported file format. You can invoke an
example by passing a filename at the command line, or by dragging a music
module on top of it in Windows. You can control the playback quality and
troubleshoot problems by editing dumb.ini. Further instructions are embedded
in dumb.ini. That's all there is to know!


*********************************************
*** Downloading music or writing your own ***
*********************************************


If you would like to compose your own music modules, then first I must offer
a word of warning: not everyone is capable of composing music. Do not assume
you will be able to learn the art. By all means have a go; if you can learn
to play tunes on the computer keyboard, you're well on the way to being a
composer!

The best programs for the job are the trackers that pioneered the file
formats:

   Impulse Tracker - IT files - http://www.noisemusic.org/it/
   Fast Tracker II - XM files - http://www.gwinternet.com/music/ft2/
   Scream Tracker 3 - S3M files -
          http://www.united-trackers.org/resources/software/screamtracker.htm

Note that Fast Tracker II is Shareware. It offers the best interface, but the
IT file format is more powerful and better defined. The other editors are
Freeware. DUMB is likely to be at its best with IT files.

These are DOS programs. Users of DOS-incapable operating systems may like to
try ModPlug Tracker, but should be aware that it does not support all
combinations of effects correctly, and some files will sound wrong. If you
use a different operating system, or if you know of any better module editors
for Windows, please give me some links so I can put them here!

   ModPlug Tracker - http://www.modplug.com/

BEWARE OF WINAMP! Although it's excellent for MP3s, it is notorious for being
one of the worst module players in existence. There are plug-ins available to
improve its module support, for example WSP.

   Winamp - http://www.winamp.com/
   WSP - http://www.spytech.cz/index.php?sec=demo

If you would like to download module files composed by other people, check
the following sites:

   http://www.modplug.com/
   http://www.traxinspace.com/

Once again, if you know of more sites where module files are available for
download, please let me know. Please respect the composers; if you wish to
use someone's music in your game, then you must ask. In most cases the author
will be thrilled, so don't hesitate!

WARNING: ModPlug Tracker cannot convert between different module formats to
         save its life (at the time of writing). Impulse Tracker can convert
         S3M to IT quite successfully (XM less so), but otherwise I should
         put more faith in DUMB's S3M or XM support if I were you!


***********************
*** Contact details ***
***********************


If you have trouble with DUMB, or want to contact me for any other reason, my
e-mail address is given below. However, I may be able to help more if you
come on to IRC EFnet #dumb. If you would like to set up IRC, please visit one
of the following links:

   http://www.xchat.org/
   http://www.mirc.com/
   http://www.hansprestige.com/virc/beta.php

Getting on to IRC can be a steep cliff, but it is not insurmountable, and
it's well worth it. Once you have set up the client software, here is a list
of European servers you can try. Type "/server" (without quotes), then a
space, then the name of a server.

   irc.homelien.no
   irc.webgiro.se
   efnet.vuurwerk.nl
   efnet.demon.co.uk
   irc.isdnet.fr
   irc.prison.net

If these servers do not work, please e-mail me and tell me where you are; I
shall try to find some servers in your area for you to try. If you are
already familiar with IRC and know of some good servers that aren't in this
list, please let me know.

Once you're on IRC, type the following:

   /join #dumb

A window will appear, and you can ask your question. It should be clear
what's going on from this point onwards. I am 'entheh'. Note that I am not
always there; if you see 'enthehOUT' in the list, for example, that means
I've gone out and left the computer on.


******************
*** Conclusion ***
******************


This is the conclusion.


Ben Davis
entheh@users.sf.net
IRC EFnet #dumb
