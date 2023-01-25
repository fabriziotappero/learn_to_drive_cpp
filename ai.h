
// basic Ai Class
class Ai
{
private:
    
public:
    Ai();
    ~Ai();
    void update(std::array<int,13> CurbDist, bool bCarOverArrival, bool bCarHasHitCurb, float fCarSpeedLin, float fCarDirection);
};

Ai::Ai()
{
}

Ai::~Ai()
{
}


void Ai::update(std::array<int,13> CurbDist, bool bCarOverArrival, bool bCarHasHitCurb, float fCarSpeedLin, float fCarDirection)
{
    if(0){ // DEBUG
        for(auto el: CurbDist)
            std::cout << el << " ";
        std::cout << "\n";
        std::cout << fCarDirection << ", " << fCarSpeedLin << "\n";
    }
    

}