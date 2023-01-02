// for MAX performance compile with:
// g++ -o main.exe main.cpp -O2 -s -DNDEBUG -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

//#define OLC_IMAGE_STB
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:

  // circuit parameters
  std::unique_ptr<olc::Sprite> sprCircuit = std::make_unique<olc::Sprite>("./circuit_1.png");
  olc::vi2d vCircuitSize = { 1000,1000 };


  // car parameters definition
  std::unique_ptr<olc::Sprite> sprCar = std::make_unique<olc::Sprite>("./car.png");
  olc::vi2d vCarSize = { 100,100 };
  olc::vi2d vCarPos = { 0,0 };
  float carSpeed = 0.0f;

  // text info
  std::string info = "none";

	Example()
	{
		// Name your application
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start

    // load circuit
    //DrawSprite(olc::vi2d(0, 0) * vCircuitSize, sprCircuit.get());

    // load car
    SetPixelMode(olc::Pixel::MASK); // preserve transparency
    //DrawSprite(olc::vi2d(1, 1) * vCarSize, sprCar.get());

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
    // load circuit
    DrawSprite(olc::vi2d(0, 0) * vCircuitSize, sprCircuit.get());

    // load car pos
    DrawSprite(olc::vi2d(vCarPos.x, vCarPos.y) * vCarSize, sprCar.get());

    // get mouse position
    if (GetMouse(0).bHeld)
    {
      vCarPos.x = float(GetMouseX())/100.0f;
      vCarPos.y = float(GetMouseY()/100.0f);
    }

    // increment car speed
    if (GetMouseWheel() > 0) carSpeed += 0.1f;
		if (GetMouseWheel() < 0) carSpeed -= 0.1f;
    vCarPos.x += carSpeed;
    vCarPos.y += carSpeed;
    
    // print out some numbers
		DrawString(5, ScreenHeight()-17, info, olc::BLACK, 2);
    
		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(1000, 1000, 1, 1))
		demo.Start();
	return 0;
}