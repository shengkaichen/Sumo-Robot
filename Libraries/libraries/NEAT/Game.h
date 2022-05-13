#include <vector>
#include <cmath>

//class Point {
//  int _x, _y;
//  Point(int x, int y) {
//    _x = x;
//    _y = y;
//  };
//};
//
//class Pod {
//    //physics
//    Point _position;
//    std::pair<int, int> _speed;
//    float _angle; //
//    // race data
//    int _nextCheckPointId;
//    float _percentageOfTheRaceCompleted;
//    int _team;
//    int _id;
//    
//};
//
//class Circuit {
//public: 
//    Circuit() {
//      for (int i = 0; i<numberOfWaypoints; i++) {
//          _waypoints[i] = Point(rand()%maxW , rand()%maxH);          
//      }  
//    };
//    Point getCheckpoint(int i) {
//        return _waypoints[i];
//    }
//private:
//    std::map<int, Point> _waypoints; 
//    int maxW = 16000;
//    int maxH = 9000;
//    int numberOfWaypoints = 5;
//};
//
//class Race {
//    Circuit _circuit;
//    std::vector<Pod> _pods;
//    float _score;
//    
//    void addPod(Pod aPod) {
//        aPod._id = _pods.size();
//        _pods.push_back(aPod);
//    };
//    void startRace() {
//        // init conditions
//        
//        // run race
//        bool raceContinues = true;
//        while (raceContinues) {
//            for (Pod& p: _pods) {
//                p.get
//            }
//        }
//        
//        // compute how the current IA performed
//        computeScore(); 
//    };
//    void applyMove(Pod& iPod, float angle, int thrust) {
//        iPod._angle += angle;
////        std::pair<int, int> aNewPosition = std::make_pair()
//    };
//    void computeScore() {
//      _score = 0.;  
//    };
//};


//std::vector<int, int> get

class Game {
public:
    template <class T>
    static float evalSolution(const T& aSolution ) {
        
        
        
        // to get the result of the current predictive/decision model, call applyInput
        T aSolutionCopy = aSolution;
        
        ////////////////////////////////////////////
        // here goal is to approximate sin(x) * 0.8 
        
        float anAverageDeviation = 0.0;
        float angle;
        float aPrediction;
        float anExpectedResult;
        // inputs must be randomized in most cases
        
        ///////////////////////////////////////////////////////////////////////
        // input data needs to be normalized aka be in [-4, 4] with a mean of 0 \
        // for current activation function ( x/(1+|x|) ) 
        // 
        // here a is the angle in radian between [-pi, pi]
        
        for (int i = 0; i < 36; i++) {
            angle = float(i*10 - 180) * 3.14159265359/180.; 
            
            aPrediction = aSolutionCopy.execute({angle}).at(0);
            anExpectedResult = f(angle);
            
            // equals 1.9 at maximun => divide by 3.61 to have it normalise error
            anAverageDeviation += (aPrediction - anExpectedResult) * (aPrediction - anExpectedResult) / 3.24;
            
//            anAverageDeviation += abs(aPrediction - anExpectedResult)/1.9;
        }
        anAverageDeviation *= 1.0/36.0;
        return 1.0 - (anAverageDeviation);
    };
    
    static float f(float x) {
        return (cos(x)) * 0.9;
    }
};