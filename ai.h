
// basic Ai Class
class Ai
{
private:
    
public: 
    Ai();
    ~Ai();
    void update(std::array<int,13> CurbDist, float fCarTravelledDist, bool bCarHasHitCurb, float fCarSpeedLin, float fCarDirection);
};

Ai::Ai()
{
}

Ai::~Ai()
{
}


void Ai::update(std::array<int,13> CurbDist, float fCarTravelledDist, bool bCarHasHitCurb, float fCarSpeedLin, float fCarDirection)
{
    if(1){ // DEBUG
        for(auto el: CurbDist)
            std::cout << el << " ";
        std::cout << "\t\tDirection:" << fCarDirection << "\tSpeed:" << fCarSpeedLin << "\tTravelled Distance:" << fCarTravelledDist << "\n";
    }
    

}