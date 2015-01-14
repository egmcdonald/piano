# piano
An OpenGL application written in C++, simulates the internal movements of a piano upon key presses.

<h1>Parts</h1>
There are 8 parts to the model:
<ol>
  <li>Key (only visible part from the outside of a piano)</li>
  <li>Lever (arm attached to key for hammer/dampers to sit upon)</li>
  <li>Pivot (non-functional, purely to show creation of tetrahedron object and point of pivot)</li>
  <li>Hammer Arm (arm to connect lever to hammer)</li>
  <li>Hammer (simulates hammer contact with string)</li>
  <li>Damper Arm (arm to connect lever to damper)</li>
  <li>Damper (simulates damper release from string)</li>
  <li>String (named in project as “wire” due to “string” library already existing)</li>
</ol>

<h1>Functions</h1>
There are many functions available for the user to interact with the model and they are printed to the console upon running the program. The model mirrors a simulation of the inner workings of a piano:
<ul>
  <li>Upon pressing a piano key, the key and lever angle upwards, pushing the hammer/damper upwards towards/away from the string
  <li>Upon contact with the hammer, the string vibrates and the hammer/dampers are bounced back towards their initial starting position</li> 
  <li>The key and lever arm then angles itself back to a resting position
</ul>

The vibration intensity of the string is dependent on the speed of the hammer; the faster the hammer, the tighter the vibration. This is comparable to the real functionality of a piano. The keys can move independently of one another and the hammer speed can be adjusted.

In terms of the program view, the user can rotate the view/object on all three axes and zoom in and out on the model.

<h1>Extensions</h1>
The global constants within main.cpp singlehandedly control the specifics for a singular model and its positioning; e.g. only a singular value needs to be altered in order to edit the number of piano keys within the overall model (MODELS on line 3 of main.h).
