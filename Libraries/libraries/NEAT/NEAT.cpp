#include <ctime>
#include "NEAT.h"

NEAT::NEAT(int nbInput, int nbOutput) {
    srand(time(0)); // seed rand()
    _nbInput = nbInput; // bias is considered as an input and is manage at genome level
    _nbOutput= nbOutput;
    initialize();
}

void NEAT::initialize() {
    _species.clear();
    _globalPopulation.clear();
    
    // create wanted numbers of genomes with minimal topology within one species
    Species aSpecies;
    Genome aModel(_nbInput, _nbOutput);
//    for (int i=0; i<Settings::TARGET_SPECIES_SIZE; i++) {
    for (int i=0; i<Settings::POPULATION_SIZE; i++) {
        Genome aGenome = aModel;
        aGenome.getNewId();
        aGenome.randomizeWeight();
        aSpecies.addGenome(aGenome);
    }   
    _species.push_back(aSpecies);
}

void NEAT::assignToSpecies(Genome iGenome, int iDefaultSpeciesId) {
    if ((rand() / (float) (RAND_MAX)) < Settings::ADOPTION_RATE) {
//        cerr << "genome is poposed to adoption" << endl;
        std::random_shuffle(_species.begin(), _species.end());
        for (Species& aSpecies : _species) {
            if (aSpecies.testGenomeForSpecies(iGenome)) {
                aSpecies.addGenome(iGenome);
//                cerr <<"offspring adopted." << endl;
                return;
            }
        }
//        cerr <<"new species" << endl;
        Species aNewSpecies;
        aNewSpecies.addGenome(iGenome);
        _species.push_back(aNewSpecies);
    }
    else {
//        cerr <<"offspring nursed." << endl;
        getSpeciesById(iDefaultSpeciesId).addGenome(iGenome);
    }
};