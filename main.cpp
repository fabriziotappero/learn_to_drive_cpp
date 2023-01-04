// for MAX performance compile with:
// g++ -o main.exe main.cpp -O2 -s -DNDEBUG -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

//#define OLC_IMAGE_STB
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// given two (x,y) points returns vector of (x,y) points of the line that connects those points 
std::vector<olc::vi2d> GetLinePoints(const olc::vi2d pos1, const olc::vi2d pos2);

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
  olc::vf2d vCarPos = { 210,100 };               // 2D car position
  olc::vf2d vCarDir   = olc::vf2d(0.0f, 0.0f);  // 2D car direction
	olc::vf2d vCarSpeed = olc::vf2d(0.0f, 0.0f);  // 2D car speed
  float fCarSpeedLin = 0.0f;                    // Linear car speed
  float fCarDirection = 0.0f;                   // Direction of the car
  // car visibility lines
  std::vector<olc::vi2d> carLine0, carLine1l, carLine1r, carLine2l,
                                   carLine2r, carLine3l, carLine3r;

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
		sprCircuit = std::make_unique<olc::Sprite>("./assets/circuit_1.png");
    sprCar = std::make_unique<olc::Sprite>("./assets/car.png");
    decCar = std::make_unique<olc::Decal>(sprCar.get()); // car is actually a decal handled by GPU

    // draw race circuit as a sprite
    DrawSprite(olc::vi2d(0, 0) * vCircuitSize, sprCircuit.get());

    //Clear(olc::WHITE);
    //SetPixelMode(olc::Pixel::MASK); // Draw all pixels
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
    FillCircle(vCarPos.x, vCarPos.y, 2, olc::GREY); // car pivoting point

    // populate central car visibility lines
    carLine0 = GetLinePoints({int(vCarPos.x),int(vCarPos.y)},{int(vCarPos.x+(900*cos(fCarDirection))),int(vCarPos.y+(900*sin(fCarDirection)))});
    carLine1l= GetLinePoints({int(vCarPos.x),int(vCarPos.y)},{int(vCarPos.x+(900*cos(fCarDirection+0.3))),int(vCarPos.y+(900*sin(fCarDirection+0.3)))});
    carLine1r = GetLinePoints({int(vCarPos.x),int(vCarPos.y)},{int(vCarPos.x+(900*cos(fCarDirection-0.3))),int(vCarPos.y+(900*sin(fCarDirection-0.3)))});
    carLine2l = GetLinePoints({int(vCarPos.x),int(vCarPos.y)},{int(vCarPos.x+(900*cos(fCarDirection+0.6))),int(vCarPos.y+(900*sin(fCarDirection+0.6)))});
    carLine2r = GetLinePoints({int(vCarPos.x),int(vCarPos.y)},{int(vCarPos.x+(900*cos(fCarDirection-0.6))),int(vCarPos.y+(900*sin(fCarDirection-0.6)))});
    carLine3l = GetLinePoints({int(vCarPos.x),int(vCarPos.y)},{int(vCarPos.x+(900*cos(fCarDirection+0.9))),int(vCarPos.y+(900*sin(fCarDirection+0.9)))});
    carLine3r = GetLinePoints({int(vCarPos.x),int(vCarPos.y)},{int(vCarPos.x+(900*cos(fCarDirection-0.9))),int(vCarPos.y+(900*sin(fCarDirection-0.9)))});

    // plot car visibility lines
    if(1){
      for(auto i: carLine0)  {Draw(i, olc::GREY);}
      for(auto i: carLine1l) {Draw(i, olc::GREY);}
      for(auto i: carLine1r) {Draw(i, olc::GREY);}
      for(auto i: carLine2l) {Draw(i, olc::GREY);}
      for(auto i: carLine2r) {Draw(i, olc::GREY);}
      for(auto i: carLine3l) {Draw(i, olc::GREY);}
      for(auto i: carLine3r) {Draw(i, olc::GREY);}
    }

    // find closest car to curb distance along car visibility lines
    int min_v=1E6;
    for(auto i: carLine0)
    {
      pCircuitPixel = sprCircuit->GetPixel(i.x,i.y); // read Circuit Sprite pixel colour
      if (pCircuitPixel.r != 255 | pCircuitPixel.g != 255 | pCircuitPixel.b != 255){
        FillCircle(i.x, i.y, 4, olc::RED);

        // measure distance between i and vCarPos
        int dist = sqrt(pow(i.x-vCarPos.x,2)+pow(i.y-vCarPos.y,2));
        if (dist < min_v) 
        {
          min_v=dist;
          std::cout << min_v << "\n";
          }
      }
    }
    
    
    
    
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

// given two (x,y) points returns vector of (x,y) points of the line that connects those points 
std::vector<olc::vi2d> GetLinePoints(const olc::vi2d pos1, const olc::vi2d pos2)
	{
    uint32_t pattern = 0xFFFFFFFF;
    int x1=pos1.x, y1=pos1.y, x2=pos2.x, y2=pos2.y;
		int x, y, dy, dx1, dy1, px, py, xe, ye, i;
		int dx = x2 - x1; dy = y2 - y1;
    std::vector<olc::vi2d> v_out(0);

		auto rol = [&](void) { pattern = (pattern << 1) | (pattern >> 31); return pattern & 1; };

		olc::vi2d p1(x1, y1), p2(x2, y2);
		//if (!ClipLineToScreen(p1, p2))
		//	return;
		x1 = p1.x; y1 = p1.y;
		x2 = p2.x; y2 = p2.y;

		// straight lines idea by gurkanctn
		if (dx == 0) // Line is vertical
		{
			if (y2 < y1) std::swap(y1, y2);
			for (y = y1; y <= y2; y++) if (rol()) v_out.push_back({x1,y});
			return v_out;
		}

		if (dy == 0) // Line is horizontal
		{
			if (x2 < x1) std::swap(x1, x2);
			for (x = x1; x <= x2; x++) if (rol()) v_out.push_back({x,y1});
			return v_out;
		}

		// Line is Funk-aye
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = x1; y = y1; xe = x2;
			}
			else
			{
				x = x2; y = y2; xe = x1;
			}

			if (rol()) v_out.push_back({x,y});

			for (i = 0; x < xe; i++)
			{
				x = x + 1;
				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				if (rol()) v_out.push_back({x,y});
			}
		}
		else
		{
			if (dy >= 0)
			{
				x = x1; y = y1; ye = y2;
			}
			else
			{
				x = x2; y = y2; ye = y1;
			}

			if (rol()) v_out.push_back({x,y});

			for (i = 0; y < ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				if (rol()) v_out.push_back({x,y});
			}
		}
    return v_out;
	}
