# Design and Manufacture Process

* This project was designed and built with mainly open-source softare.
* The specialized tools involved were available through the [TinkerMill] in Longmont, CO.

## Puzzle Concepts

I did a lot of research on puzzles and puzzle box mechanisms while deciding what elements to include in this box.
Below are links to some content that inspired me:
* [Cipher gears]
* [Inverted drawer opening another]
* [Grille cipher]

## Initial prototype out of foam board:
 ![Foam prototype](https://raw.githubusercontent.com/rcalme/puzzle-box/master/Process/Images/00_Foam_prototype.jpg)

## Creating Cipher Gears

I wrote [Python code] to generate the order of the randomized letters on the five cipher gears, as well as to encrypt/decrypt messages.

The same was created as a [stand-alone web interface] by porting the code to Javascript to make it operable in a browser.

The gears themselves were first 3D printed, and later laser-cut in 1/4" red-stained maple, with the letters on the gear teeth engraved.
 ![3D printed gears](https://raw.githubusercontent.com/rcalme/puzzle-box/master/Process/Images/02_3D_printed_gears.jpg)
 ![Cipher set](https://raw.githubusercontent.com/rcalme/puzzle-box/master/Process/Images/04_Cipher_set.jpg)
 ![Final gears](https://raw.githubusercontent.com/rcalme/puzzle-box/master/Process/Images/39_Box_gears.jpg)
 
## Creating Magnet Puzzle

The magnet puzzle was always going to be a fixed-location puzzle, as the internal [reed switches] were wired in series.
The circuit is closed only when all switches were simultaneously closed.

The grid, and the symbols (in the [Magzetition font]) determine the locations of the five magnets to complete the hidden circuit, with itself supplies power to the [Arduino].

The outer layer is 1/4" maple, routed on a [ShopBot PRS Alpha], available at my local maker space.
 ![Magnet router plan](https://raw.githubusercontent.com/rcalme/puzzle-box/master/Process/Images/09_Magnet_grid_plan.png)
 ![Magnet puzzle outer](https://raw.githubusercontent.com/rcalme/puzzle-box/master/Process/Images/10_Magnet_panel.jpg)

The inner layer is 1/8" Baltic birch, cut on an [80 watt Rabbit laser cutter], and wired up with these [mini reed switches].
 ![Magnet puzzle inner](https://raw.githubusercontent.com/rcalme/puzzle-box/master/Process/Images/11_Magnet_puzzle.jpg)
 
## Creating Peg Board Puzzle

The peg board uses a jumper cable with banana connector ends, to detect the connection of any two-digit number pairs.
I set the code up to accept any combinations, and installed six LEDs to show progress when solving the puzzle's six ordered pairs of digits.

The [C++ code that probes this peg board] to detect any connection(s) using only 10 pins on an Arduino took some time to get right.

 ![Peg board improvements](https://github.com/rcalme/puzzle-box/blob/master/Process/Images/13_Peg_board_improvements.jpg)

## Creating the Back Panel Storage

The back panel is just an elaborate storage container for the [grille] mask plate.
The two faces are routed on a ShopBot, and sealed via security hex bit screws to make access difficult until the correct tool is located.

The grille itself is laser-cut out of two layers of 1/8" alder, sandwiching neodymium magnets.
The magnets ensure that the grille snaps into a single, specific position on the front of the box. When flipped to its 'B' side, the magnets are reversed, and again, the grille only snaps into a single (but different)location.

 ![mask plate in place](https://github.com/rcalme/puzzle-box/blob/master/Process/Images/19_Letter_panel_with_mask.jpg)
 
 ## Creating the Front Letter Panel
 
 The front letter panel is again routed on a ShopBot, with a field of random letters (in [Instruction font]).
 Only the letters highlighted with the grille are consequential (though encrypted).
 ![letter panel](https://github.com/rcalme/puzzle-box/blob/master/Process/Images/18_Letter_panel.jpg)
 
 I wrote code to generate this letter field randomly, but consistently, as well as to calculate where to put the holes in the grille, such that when flipped from the 'A' side, to the 'B' side, there was no interference between the two patterns.

 ![grille code output](https://github.com/rcalme/puzzle-box/blob/master/Letter_Panel/Grid/grille_cipher.png)
 
 ## Creating the Drawers
 
 There are three operable drawers in the base of the box. 
 
 ![drawer assembly](https://github.com/rcalme/puzzle-box/blob/master/Process/Images/27_Finished_drawers.jpg)
 
 ### Left Drawer
 
 The left drawer is spring-loaded, and held in place by one of the legs of the box, which threads up through the base, into a hole in the drawer.
  When the leg is unscrewed, the drawer slides out.
  
 The drawer holds the magnets needed to complete the magnet puzzle on the left face.
 
 The threaded leg itself becomes a useful tool in solving the puzzle, as its tip is a star security bit, enabling access to the grille plate on the back of the box.
 
 ![screw driver leg](https://github.com/rcalme/puzzle-box/blob/master/Process/Images/23_Finished_screw_driver_leg.jpg)
 
 ### Front Drawer
 
 The front drawer holds two more cipher gears, needed to encrypt instructions for later in the puzzle.
 The drawer is released by turning the left drawer upside down, and pushing it back into its alcove.
 
 The latch mechanism was custom designed, and 3D-printed for this purpose.
  ![latch mechanism](https://github.com/rcalme/puzzle-box/blob/master/Process/Images/05_Latch_prototype.jpg)
  
 ### Right Drawer
 
 The right drawer holds the banana jumper cable, and is released via a servo latch.
 The drawer is released as soon as the Arduino receives power (when the magnet puzzle is solved).
 
 ### Key Slider
 
 The key to unlock the box is finally accessible when it slides out the left side of the drawers via a servo.
 The sliding structure is made of two layers of laser-cut acrylic, and the brackets for the servo, and the slider are also custom designed and 3D-printed.
 
  ![key slider](https://github.com/rcalme/puzzle-box/blob/master/Process/Images/24_Key_servo_mechanism.jpg)
  
# Future improvements

I investigated the possibility of filling all the CNC-routed lettering with colored epoxy resin, and sanding it back down to flush with the face of the box. The results looked nice, but took quite a bit of time to get right.

 ![epoxy resin](https://github.com/rcalme/puzzle-box/blob/master/Process/Images/15_Epoxy_inlay_process.jpg)
 ![epoxy finished](https://github.com/rcalme/puzzle-box/blob/master/Process/Images/16_Epoxy_inlay_finished.jpg)
 
# Tools

## Hardware Used
* 80 Watt Laser Cutter ([Rabbit RL-80-1290])
* CNC Router ([Shopbot PRS Alpha])
* 3D Printer ([LulzBot Mini])
* Table Saw ([SawStop])
* Arduino ([Sparkfun Arduino Pro Mini 328 - 5V/16MHz])

## Software Used
* 3D Modelng ([SketchUp])
* 3D Slicing ([Cura])
* 2D Vector Design ([Inkscape])

   [Tinkermill]: <https://tinkermill.org>
   [Cipher gears]: <https://www.youtube.com/watch?v=hiOjqskDlS0>
   [Inverted drawer opening another]: <https://www.youtube.com/watch?v=foH1jnLvS_I>
   [Grille cipher]: <https://www.youtube.com/watch?v=tmgmOpaP_58>
   [Python code]: <https://github.com/rcalme/puzzle-box/blob/master/Cipher_Gears/Translator/cryptex.py>
   [stand-alone web interface]: <https://rawgit.com/rcalme/puzzle-box/master/Cipher_Gears/Translator/cipherGears.html>
   [reed switches]: <https://en.wikipedia.org/wiki/Reed_switch>
   [Magzetition font]: <http://www.dafont.com/magzetician.font>
   [Instruction font]: <http://www.dafont.com/instruction.font>
   [Arduino]: <https://www.sparkfun.com/products/11113>
   [Sparkfun Arduino Pro Mini 328 - 5V/16MHz]: <https://www.sparkfun.com/products/11113>
   [ShopBot PRS Alpha]: <http://www.shopbottools.com/mProducts/prSalpha.htm>
   [80 watt Rabbit laser cutter]: <http://www.rabbitlaserusa.com/laser_RL1290.html>
   [Rabbit RL-80-1290]: <http://www.rabbitlaserusa.com/laser_RL1290.html>
   [mini reed switches]: <https://www.sparkfun.com/products/10601>
   [C++ code that probes this peg board]: <https://github.com/rcalme/puzzle-box/tree/master/Arduino/Puzzle_Box>
   [grille]: <https://en.wikipedia.org/wiki/Grille_(cryptography)>
   [LulzBot Mini]: <https://www.lulzbot.com/store/printers/lulzbot-mini>
   [SawStop]: <http://www.sawstop.com>
   [SketchUp]: <http://www.sketchup.com>
   [Cura]: <https://www.lulzbot.com/cura>
   [Inkscape]: <https://inkscape.org/en>
