#include <algorithm>  
#ifndef SETTINGS_H
#define SETTINGS_H
class Settings {
public:
    static float EXCESS_DIST_FACTOR;
    static float DISJOINT_DIST_FACTOR;
    static float WEIGHT_DIST_FACTOR;
    
    static float THRESHOLD;
    static float THRESHOLD_MAX;
    static float THRESHOLD_MIN;
    static float THRESHOLD_STEP;
    static int TARGET_SPECIES_SIZE;
//    static int MAX_SPECIES_SIZE;
    
    static float KILLING_RATE;
    static float MAX_KILLED_RATE_ON_1_SPECIES;
    static float PARENTING_RATE;
    static float ADOPTION_RATE;
    
    static int POPULATION_SIZE;
    static int MAX_NUMBER_GENERATION;
    static float STOP_EVOLUTION_AT_FITNESS;
    
    static bool  MUTATION_PROCEED_BY_COPY;
    static float MUTATION_RATE_NODE;
    static float MUTATION_RATE_WEIGHT;
    static float MUTATION_STEP_WEIGHT;
    static float MUTATION_RATE_DISABLE_LINK;
    static float MUTATION_RATE_CREATE_LINK;
    
//    static Settings* getManager();
    
    static void increaseSpeciesThreashold() {
        Settings::THRESHOLD = std::min(Settings::THRESHOLD_MAX, Settings::THRESHOLD + Settings::THRESHOLD_STEP);
    };
    static void decreaseSpeciesThreashold() {
        Settings::THRESHOLD = std::max(Settings::THRESHOLD_MIN, Settings::THRESHOLD - Settings::THRESHOLD_STEP);
    };
//    
//private:
//    static Settings* _address;
};
#endif
