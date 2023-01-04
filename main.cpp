// for MAX performance compile with:
// g++ -o main.exe main.cpp -O2 -s -DNDEBUG -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

//#define OLC_IMAGE_STB
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Race : public olc::PixelGameEngine
{
public:

  // circuit and car sprites
  std::unique_ptr<olc::Sprite> sprCircuit;
	std::unique_ptr<olc::Sprite> sprCar;
	std::unique_ptr<olc::Decal> decCar;

  // car and circuit parameters
  olc::Pixel pCircuitPixel;
  olc::vi2d vCircuitSize = { 1000,1000 };
  olc::vi2d vCarSize = { 1,1 };
  olc::vf2d vCarPos = { 190,85 };               // 2D car position
  olc::vf2d vCarDir   = olc::vf2d(0.0f, 0.0f);  // 2D car direction
	olc::vf2d vCarSpeed = olc::vf2d(0.0f, 0.0f);  // 2D car speed
  float fCarSpeedLin = 0.0f;                    // Linear car speed
  float fCarDirection = 0.0f;                   // Direction of the car

  // text info
  std::string info = "none";

	Race()
	{
		// Name your application
		sAppName = "AI Learn to Drive";
	}

public:
	bool OnUserCreate() override
	{

    // Load circuit and car sprites
		sprCircuit = std::make_unique<olc::Sprite>("./assets/circuit_4.png");
    sprCar = std::make_unique<olc::Sprite>("./assets/car.png");
    decCar = std::make_unique<olc::Decal>(sprCar.get()); // car is actually a decal handled by GPU

    // draw race circuit as a sprite
    DrawSprite(olc::vi2d(0, 0) * vCircuitSize, sprCircuit.get());

    //SetPixelMode(olc::Pixel::MASK); // Dont draw pixels which have any transparency
    //SetPixelMode(olc::Pixel::NORMAL); // Draw all pixels
    //Clear(olc::WHITE);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

    // get some keyboard inputs
    if (1)
    {
      // change car direction and speed linearly
      if (GetKey(olc::Key::LEFT).bHeld) fCarDirection  +=  1.1f * fElapsedTime;
      if (GetKey(olc::Key::RIGHT).bHeld) fCarDirection -=  1.1f * fElapsedTime;

      if (GetKey(olc::Key::UP).bHeld) fCarSpeedLin    +=  30.0f * fElapsedTime;
      if (GetKey(olc::Key::DOWN).bHeld) fCarSpeedLin  -=  30.0f * fElapsedTime;

      // override car position using mouse pointer
      if (GetMouse(0).bHeld)
      {
        vCarPos.x = float(GetMouseX())/1.0f;
        vCarPos.y = float(GetMouseY())/1.0f;
      }
    }

    // Update car physics
    vCarSpeed = { fCarSpeedLin * cos(fCarDirection), fCarSpeedLin * sin(fCarDirection) }; // Convert linear speed to 2D speed
    vCarPos += vCarSpeed * fElapsedTime;

    // Draw car
    DrawRotatedDecal(vCarPos * vCarSize, decCar.get() , fCarDirection, { 80, 54/2 }, { 1, 1 }, olc::WHITE);

    // Limit car motion
    if (vCarPos.x + 20 > ScreenWidth())  vCarPos.x = ScreenWidth()  - 20;
    if (vCarPos.y + 40 > ScreenHeight()) vCarPos.y = ScreenHeight() - 40;
    if (vCarPos.x - 20 < 0)  vCarPos.x = 0  + 20;
    if (vCarPos.y - 20 < 0)  vCarPos.y = 0  + 20;
    
    //  Calculate Collision WIP
    FillCircle(vCarPos.x, vCarPos.y, 4, olc::GREY); // car pivoting point
    // lines that define car visibility (a kind of lidar)
    DrawLine(vCarPos, {int(vCarPos.x+(500*cos(fCarDirection))),int(vCarPos.y+(500*sin(fCarDirection)))}, olc::GREY);
    DrawLine(vCarPos, {int(vCarPos.x+(500*cos(fCarDirection+0.2))),int(vCarPos.y+(500*sin(fCarDirection+0.2)))}, olc::GREY);
    DrawLine(vCarPos, {int(vCarPos.x+(500*cos(fCarDirection-0.2))),int(vCarPos.y+(500*sin(fCarDirection-0.2)))}, olc::GREY);
    DrawLine(vCarPos, {int(vCarPos.x+(500*cos(fCarDirection+0.5))),int(vCarPos.y+(500*sin(fCarDirection+0.5)))}, olc::GREY);
    DrawLine(vCarPos, {int(vCarPos.x+(500*cos(fCarDirection-0.5))),int(vCarPos.y+(500*sin(fCarDirection-0.5)))}, olc::GREY);
    
    // read Circuit Sprite pixel colour
    pCircuitPixel = sprCircuit->GetPixel(GetMouseX(),GetMouseY());
    std::cout << "Pixel: " << std::to_string(pCircuitPixel.r) << " " << std::to_string(pCircuitPixel.g) << " " << std::to_string(pCircuitPixel.b) << "\n";

    // Measure car-border distances
    // TODO

    // AI driving
    // TODO


    // print some data
    info = "POS:(" + std::to_string(int(vCarPos.x)) + "," + std::to_string(int(vCarPos.y)) +
                                                  "),DIR:" + std::to_string(fCarDirection*57.2958).substr(0,6) +
                                             ",SPEED:" + std::to_string(int(fCarSpeedLin)) + 
                                           ",FPS:" + std::to_string(int(1/fElapsedTime));

    FillRect(0, ScreenHeight()-21, ScreenWidth(), 21, olc::BLACK);
		DrawString(5, ScreenHeight()-17, info, olc::WHITE, 2);

		return true;
	}
};

int main()
{
	Race demo;
	if (demo.Construct(1000, 1000, 1, 1))
		demo.Start();
	return 0;
}
