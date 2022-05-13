#include "Settings.h"

// speciation 
float Settings::EXCESS_DIST_FACTOR  = 2.0 ;
float Settings::DISJOINT_DIST_FACTOR  = 2.0 ;
float Settings::WEIGHT_DIST_FACTOR  = 1.0 ;

float Settings::THRESHOLD = 4.0;
float Settings::THRESHOLD_MAX = 8.0;
float Settings::THRESHOLD_MIN = 2.;
float Settings::THRESHOLD_STEP = 2.;

int Settings::TARGET_SPECIES_SIZE = 13;
//int Settings::MAX_SPECIES_SIZE = 25;

// genetics - reproduction
float Settings::KILLING_RATE = 0.3;
float Settings::PARENTING_RATE = 0.2;
float Settings::ADOPTION_RATE = 0.1;

// genetics - mutations
//   Preliminary experiments indicate that high weight mutation rates (i.e. 50% or more) 
//   are useful for control tasks, but lower rates (i.e. under 1%) are more appropriate for 
//   high input games like Othello. It may be that the number of inputs is the critical factor, 
//   and that low-input tasks respond better to high mutation weights. 
//   Although I do not have concrete statistics from which to draw strong conclusions, 
//   a good rule of thumb is to change the weight mutation rate 
//   if the systems seems to be performing below expectations. 
bool  Settings::MUTATION_PROCEED_BY_COPY = false;
float Settings::MUTATION_RATE_NODE = 0.05;
float Settings::MUTATION_RATE_WEIGHT = 0.6;
float Settings::MUTATION_STEP_WEIGHT = 0.05;
float Settings::MUTATION_RATE_DISABLE_LINK = 0.05;
float Settings::MUTATION_RATE_CREATE_LINK = 0.1;


// population
int Settings::POPULATION_SIZE = 100;
int Settings::MAX_NUMBER_GENERATION = 200;
float Settings::STOP_EVOLUTION_AT_FITNESS = 0.998;


//void Settings::increaseSpeciesThreashold() {
//    Settings::THRESHOLD += Settings::THRESHOLD_STEP;
//};

//Settings* Settings::_address = 0;
//Settings* Settings::getManager() {
//   if (!_address) {
//      _address = new Settings();
//   }
//   return _address;
//};