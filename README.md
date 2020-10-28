# 3D Editor with BSP compiler
#### This is a 17 years old project.
#### Built on Visual Studio MFC version ???
#### How to at the bottom of tha page.

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=L9RVWU5NUZ4YG)   [donations are taken care by meeiot domain]
# GETIC 3D
#### see also the GETAE PROJECT WHICH IS RELATED  https://github.com/comarius/GETAE
https://youtu.be/l7wYiHln06Y
#### BSD Zero Clause License
#### Was last compiled with Visual Studio 2010 (Express + MFC). Windows OS only


* This is getic 3D on wine under ubuntu 12 (click to play demo)

[![Alt text for your video](http://marius.mine.nu/_res/getic/about11.jpg)](https://youtu.be/l7wYiHln06Y)


###  Design a 3D model.
##### Tips:  
   * Seal your design. 
   * The 3D map has to be water-sealed by a hallo reverse cube or cube shell.
   * Usually you create hallo shapes, join them side by side overlapping walls then create holes in the wals using cut all brushes.
   * Make sure there is no hole to outside. CHeck the bottom of this readme for details.



###  You can check my reverse ssh online service and online key value database at 

[reverse ssh as a service](http://www.mylinuz.com)

[key value database as a service](https://www.meeiot.org)

## How to.
As I dont have any Windows machines anymore and this was writen in old times using 
C++ MFC and Visual studio I am left with a prebuild version since. This short tutorial follows
howto for Linux. I started a port to QT thouhg few years ago, aka see: qgetic but I did not have 
time to finish it. In windows, I dont know if still runs, but under Linux using wine looks like is working.

  * Clone the git
 
![image](https://user-images.githubusercontent.com/69641625/91868488-c5dfda00-ec42-11ea-94b3-766b66500cb1.png)

  * New Box
  
![image](https://user-images.githubusercontent.com/69641625/91868698-07708500-ec43-11ea-8012-f46957f25781.png)

  * Check shell. We are making few rooms
  
  
![image](https://user-images.githubusercontent.com/69641625/91868816-25d68080-ec43-11ea-8e7a-724527c69d82.png)

 * After the hallo box is created to commit it to the scene press enter.
 * Allways keep an inside of a box scene to the center of the scene nomater what. 

![image](https://user-images.githubusercontent.com/69641625/91868919-44d51280-ec43-11ea-912b-aef40bedba60.png)

  * ENTER
  * The box is inserted to scene. No textures though.
      * Left click move -> look around
      * Right click move -> zoom
      * Right + Left move -> pan
      * 3D click on boxs -> select the box (or any other 3D element)

![image](https://user-images.githubusercontent.com/69641625/91869085-73eb8400-ec43-11ea-812c-9f58844f3cf1.png)

  * Add some texture to all box
  
![image](https://user-images.githubusercontent.com/69641625/91869226-9da4ab00-ec43-11ea-8459-f9fc100f863d.png)

   * Scroll for available textures and while the box is selected doubleclick to Texture.
   
![image](https://user-images.githubusercontent.com/69641625/91869543-02600580-ec44-11ea-92dd-ec420e36f21b.png)

   * Let's customise inside walls
   * Press 'f' or from toolbar click the 'Select face button'
   * Right click and move to go inside
   * Select a face by clicking on it in the 3D view
   
![image](https://user-images.githubusercontent.com/69641625/91869848-5a970780-ec44-11ea-9b0a-26f02aa3a657.png)

   * Apply different textures on each face
   * From Scale button make the room bigger by dragging the walls in the 2D view
   * Add also a light as well from the insert items source
   * Make the light to illuminate like this
   
   
![image](https://user-images.githubusercontent.com/69641625/91873530-1eb17180-ec47-11ea-85a9-83f1c05f17e6.png)

   
![image](https://user-images.githubusercontent.com/69641625/91873062-99c65800-ec46-11ea-93cb-31893f48739d.png)

   * Press Compile
   * Move out of the room
   * Press View compiled scene. There are also different view modes in the tool-bar as well you can click.
   
![image](https://user-images.githubusercontent.com/69641625/91873739-659f6700-ec47-11ea-9231-143a3742c8c2.png)

  * Let's add another room 
      * Select the box frmo outside
      * Ctrl + C  copy
      * Ctrl + V   paste
      * Click 'Move' tool-bar button
      * Onto 2D view move the second box aside someting like.... Keep it intersected with the old one
      * Press Enter to commit it to the scene
      
![image](https://user-images.githubusercontent.com/69641625/91874216-f1b18e80-ec47-11ea-8729-201cb679c9bb.png)
      
      
  * Now we have to make it hollow so the BSP wont take it out.
  * Move inside of the box and cut some doorsteps
      * Insert box, Select Cut
      * The box would cut existent objects.
      * Cut all flags wuld cut all objects event the one you add after the cut.
      
![image](https://user-images.githubusercontent.com/69641625/91874334-19a0f200-ec48-11ea-80a3-fa29cb6740be.png)

   * Resize the cut in the 2D views as to create a passage as shown in all 3 formated rooms
   * DO not cliick the 3D while adjusting the cut size.

![image](https://user-images.githubusercontent.com/69641625/91874503-4c4aea80-ec48-11ea-9df7-23ae5841f83d.png)

   * When all aligned press Enter to apply the cut.
   * Then move inside
   * You can readjust the size thogh.
   
![image](https://user-images.githubusercontent.com/69641625/91874971-dbf09900-ec48-11ea-8414-a9421e3bc854.png)


      ... tb continued...



