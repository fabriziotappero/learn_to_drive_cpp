// for MAX performance compile with:
// g++ -o main.exe main.cpp -O2 -s -DNDEBUG -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

//#define OLC_IMAGE_STB
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "extensions/olcPGEX_Graphics2D.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:

  // circuit parameters
  std::unique_ptr<olc::Sprite> sprCircuit = std::make_unique<olc::Sprite>("./assets/circuit_4.png");
  olc::vi2d vCircuitSize = { 1000,1000 };

  // car parameters definition
  std::unique_ptr<olc::Sprite> sprCar = std::make_unique<olc::Sprite>("./assets/car.png");
  olc::vi2d vCarSize = { 1,1 };
  olc::vi2d vCarPos = { 0,0 };
  float carSpeed = 0.0f;
  float carDirection = 0.0f;

  // text info
  std::string info = "none";

	Example()
	{
		// Name your application
		sAppName = "AI Learn to Drive";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start
    SetPixelMode(olc::Pixel::MASK); // preserve transparency
  
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
    // load circuit
    DrawSprite(olc::vi2d(0, 0) * vCircuitSize, sprCircuit.get());

    // load car
    DrawSprite(olc::vi2d(vCarPos.x, vCarPos.y) * vCarSize, sprCar.get());
    // you need to rotate the car accordingly. See:
    https://community.onelonecoder.com/2020/01/18/new-pixelgameengine-extension-for-animating-2d-sprites/

    // set car to mouse pointer position
    if (GetMouse(0).bHeld)
    {
      vCarPos.x = float(GetMouseX())/1.0f;
      vCarPos.y = float(GetMouseY()/1.0f);
    }

    //increment car speed
    if (GetMouseWheel() > 0) carSpeed += 0.5f;
		if (GetMouseWheel() < 0) carSpeed -= 0.5f;
    vCarPos.x += carSpeed;
    //vCarPos.y += carSpeed;

    // change car direction
    if (GetKey(olc::Key::LEFT).bHeld) carDirection += 0.5f;
    if (GetKey(olc::Key::RIGHT).bHeld) carDirection -= 0.5f;
    
    // print some data
    info = "POS:(" + std::to_string(vCarPos.x) + "," + std::to_string(vCarPos.y) + "), DIR:" + std::to_string(carDirection);
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
