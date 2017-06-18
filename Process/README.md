# Design and Manufacture Process

* This project was designed and built with mainly open-source softare.
* The specialized tools involved were available through the [Tinkermill] in Longmont, CO.

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
 ![Final gears](https://raw.githubusercontent.com/rcalme/puzzle-box/master/Process/Images/39_Box_with_gears.jpg)
 
## Creating Magnet Puzzle

The magnet puzzle was always going to be a fixed-location puzzle, as the internal [reed switches] were wired in series.
The circuit is closed only when all switches were simultaneously closed.

The grid, and the symbols (in the [magzetition font]) determine the locations of the five magnets to complete the hidden circuit, with itself supplies power to the [Arduino].

The outer layer is 1/4" maple, routed on a [ShopBot PRS Alpha], available at my local maker space.
 ![Magnet router plan](https://raw.githubusercontent.com/rcalme/puzzle-box/master/Process/Images/09_Magnet_grid_plan.png)
 ![Magnet puzzle outer](https://raw.githubusercontent.com/rcalme/puzzle-box/master/Process/Images/10_Magnet_panel.jpg)

The inner layer is 1/8" Baltic birch, cut on an [80 watt Rabbit laser cutter], and wired up with these [mini reed switches].
 ![Magnet puzzle inner](https://raw.githubusercontent.com/rcalme/puzzle-box/master/Process/Images/11_Magnet_puzzle.jpg)
 
## Creating Peg Board Puzzle


 
   [Tinkermill]: <https://tinkermill.org>
   [Cipher gears]: <https://www.youtube.com/watch?v=hiOjqskDlS0>
   [Inverted drawer opening another]: <https://www.youtube.com/watch?v=foH1jnLvS_I>
   [Grille cipher]: <https://www.youtube.com/watch?v=tmgmOpaP_58>
   [Python code]: <https://github.com/rcalme/puzzle-box/blob/master/Cipher_Gears/Translator/cryptex.py>
   [stand-alone web interface]: <https://rawgit.com/rcalme/puzzle-box/master/Cipher_Gears/Translator/cipherGears.html>
   [reed switches]: <https://en.wikipedia.org/wiki/Reed_switch>
   [magzetition font]: <http://www.dafont.com/magzetician.font>
   [Arduino]: <https://www.sparkfun.com/products/11113>
   [ShopBot PRS Alpha]: <http://www.shopbottools.com/mProducts/prSalpha.htm>
   [80 watt Rabbit laser cutter]: <http://www.rabbitlaserusa.com/laser_RL1290.html>
   [mini reed switches]: <https://www.sparkfun.com/products/10601>
