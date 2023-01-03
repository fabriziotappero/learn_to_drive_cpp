
# What is this? 
A C++ implementation of an **artificial intelligence** that can drive at unbelievable speed on *any* given circuit!

<p align="center">
  <img src="https://github.com/fabriziotappero/Learn_To_Drive_CPP/blob/main/assets/hero.png?raw=true" width = "100%" alt="" align=center/>
</p>

# How to compile
Install g++ (in Windows you can use MSYS2) and run:

    g++ -o main.exe main.cpp -O2 -s -DNDEBUG -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

 # What it does
You give it a race circuit (as an PNG image) and you let the car learn how to drive. After a cetain amount of iterations the car will be able to drive al maximum speed around the circuit!
 
 As you might have noticed from the code, it uses the great open-source **Pixel Game Engine** by olc. Have a look here for into and documentation:
 
 https://github.com/OneLoneCoder/olcPixelGameEngine
 
 # Why I made this
 Essentially I wanted to improve my C++ skills by implementing an *articial intelligence* that can process images in real time using a *graphic game engine*. 
 If you actually want to implement stuff it is a good idea to find data to process, a game engine has indeed a ton of video data!
 
 Regarding AI, on the web you can find a lot of implementations made in Python but not so many made in C++.
 
 # What I have learnt
 - How to draw stuff on a screen, meaning, how to use a simple graphical game engine. This is one step above GUI development where frame rate and code 
 efficiency are your constant worry.
 - How to structure a series of C++ classes and functions so that you can integrate a well established software tool (in this case the Game Engine tool) 
 in your own development. This is a particular important skill since a lot of coding that you end up doing is via the use of tools, API or software that 
 others have built.
 - How to implement from scratch an AI in C++. Learn the ins and outs of reinforced learning and the importance of input data.
 - How to deal with C++ Standard Library tools instead of writing up your own.
 
 
 # Reach out
 If you want to know more:
 
     fabrizio.tappero@gmail.com
