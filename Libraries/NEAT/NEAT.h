#ifndef NEAT_H
#define NEAT_H

#include "Genome.h"
#include "Species.h"
#include "InnovationBank.h"
#include "GeneticOperators.h"
#include <vector>

class NEAT {
public:
    NEAT(int nbInput, int nbOutput);
    void setFitnessFunction(float (*foo) (const Genome&) ) {
        _fitnessFunction = foo;
        };
    void run();
    Genome getBest() const;
private:
    void stats() const;
    void regulateSpecies();
    void assignToSpecies(Genome iGenome, int iDefaultSpeciesId);
    void initialize();
    void evalFitness();
    void killWeakests();
    void reproduce();
    void mutate();
    Species& getSpeciesById(int id);
    void fillAndSortGlobalPopulationList();
    void removeEmptySpecies();
    
    std::vector<Species> _species;
    float (*_fitnessFunction) (const Genome&);
    int _nbInput;
    int _nbOutput;
    std::vector< std::tuple<int, int, float, float> > _globalPopulation;
};

inline void NEAT::evalFitness() {
    for (Species& aSpecies : _species) {
        for (Genome& aGenome : aSpecies._members) { 
            float aFitness = _fitnessFunction(aGenome);
            aGenome.setFitness(aFitness);
            
            if (aFitness < 0.) {
                throw "Error : negative fitness! fitness should be always positive!" ;
            }
            
            // adjusted fitness
            float counter = 0.0;
            for (const Genome& otherGenome : aSpecies._members) { //  it will be at least equal to 1 since the distance from itself is 0
                if (Genome::getDistance(aGenome, otherGenome) <= Settings::THRESHOLD) {  // could use some cache here / threshold can be 0
                    counter +=1.0;
                }
            }
            aGenome.setAdjustedFitness(aFitness / counter);
        }
    }
};

inline void NEAT::killWeakests() {
    evalFitness();
    
    /* 
     * we kill the weakest genomes by ranking them according to their adjusted fitness
     * the adjusted fitness is the fitness devided by the number of close looking other genomes 
     * within the species. 
     * 
     * we do not want a species performing well to be wiped out just because it has many members. 
     * Therefore we either need to limit the number of victims inside a species, 
     * or recompute the other adjusted fitnesses within the species 
     * 
     */
     
    // initialize vector of worst elements
    std::vector< std::tuple<int, int, float, float> > worstOfEachSpecies;
    int totalNumberOfElements = 0;
    for (Species& aSpecies: _species) {
        totalNumberOfElements += aSpecies._members.size();
        Genome& aWorst = aSpecies.getWorstGenomeAfterAdjust();
        worstOfEachSpecies.push_back(std::make_tuple(aWorst._id, aSpecies._id, aWorst._fitness, aWorst._adjustedFitness));
    }    
    
    int numberOfGenomesToFilter = abs(Settings::KILLING_RATE * Settings::POPULATION_SIZE) 
                                + totalNumberOfElements - Settings::POPULATION_SIZE;
                                
    for (int i =0 ; i < numberOfGenomesToFilter; i++) {
        std::sort(worstOfEachSpecies.begin(), worstOfEachSpecies.end(),
                [] (const std::tuple<int, int, float, float>& t1, const std::tuple<int, int, float, float>& t2) { return get<3>(t1) < get<3>(t2); });
        std::tuple<int, int, float, float> theWorst = worstOfEachSpecies.front();
//        cerr << "Deleted : "
//            << get<0>(theWorst) << " / " <<  get<1>(theWorst) << " / " 
//            << get<2>(theWorst) << " / " <<  get<3>(theWorst) << endl;
        
        Species& aSpecies = getSpeciesById(get<1>(theWorst));
        
        // remove worst from species
        aSpecies.removeGenomeById(get<0>(theWorst));
        // remove worst from vector
        worstOfEachSpecies.erase(worstOfEachSpecies.begin());
        
        if (aSpecies._members.empty()) {
            cerr << "species is extinct" << endl;
            removeEmptySpecies();
        }
        else {
            // add new worst for modified species
            Genome& aNewWorst = aSpecies.getWorstGenomeAfterAdjust();
            worstOfEachSpecies.push_back(std::make_tuple(aNewWorst._id, aSpecies._id, aNewWorst._fitness, aNewWorst._adjustedFitness));
        }
    }
};

inline void NEAT::removeEmptySpecies() {
    // remove empty species
   _species.erase(
        std::remove_if(
            _species.begin(), 
            _species.end(), 
            [] (Species& aSpecies) { 
                    return aSpecies._members.empty();}),
        _species.end());
};

inline void NEAT::reproduce()  {
    regulateSpecies();
    
    // only the top x% of each species are allowed to reproduce
    std::vector<std::pair<Genome, int> > potentialParents; 
    int aCurrentPopulationSize = 0;
    for (Species& aSpecies : _species) {
        aCurrentPopulationSize += aSpecies._members.size();
        aSpecies.sortByFitness();
        int numberOfChildren = ceil(aSpecies._members.size() * Settings::PARENTING_RATE);
        for (int i = 0 ; i < numberOfChildren; i++)  {
            potentialParents.push_back(std::make_pair(aSpecies._members.at(i), aSpecies._id) );
        }
    }
    
//    int a = min(Settings::TARGET_SPECIES_SIZE, Settings::POPULATION_SIZE - aCurrentPopulationSize);
    for (int i = 0 ; i < (Settings::POPULATION_SIZE - aCurrentPopulationSize) ; i++) { 
        std::pair<Genome, int> aFirstParent = potentialParents.at(rand()%potentialParents.size());
        Species& aParent1Species = getSpeciesById(aFirstParent.second);
        const Genome& aParent1 = aFirstParent.first;
        const Genome& aParent2 = aParent1Species._members.at(rand()%aParent1Species._members.size());            
        Genome aNewGenome = GeneticOperators::crossOver(aParent1, aParent2);
        assignToSpecies(aNewGenome, aFirstParent.second);
    }
};

inline void NEAT::mutate()  {
    if (Settings::MUTATION_PROCEED_BY_COPY) {
        for (Species& s: _species) {
            std::vector<Genome> mutants;
            for (Genome g: s._members) {
                bool isMutant = false;
                if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_NODE )  {
                    GeneticOperators::nodeMutate(g);
                    isMutant = true;
                }
                if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_WEIGHT )  {
                    GeneticOperators::weightMutate(g);
                    isMutant = true;
                }
                if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_DISABLE_LINK )  {
                    GeneticOperators::enableDisableMutate(g);
                    isMutant = true;
                }
                if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_CREATE_LINK )  {
                    GeneticOperators::createConnection(g);
                    isMutant = true;
                }
                if (isMutant) {
                    g.getNewId();
                    mutants.push_back(g);
                }
            }
            for (Genome g: mutants) {
                s.addGenome(g);
            }
        }
    }
    else {
        for (Species& s: _species) {
            for (Genome& g: s._members) {
                if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_NODE )  {
                    GeneticOperators::nodeMutate(g);
                }
                if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_WEIGHT )  {
                    GeneticOperators::weightMutate(g);
                }
                if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_DISABLE_LINK )  {
                    GeneticOperators::enableDisableMutate(g);
                }
                if ((rand() / (float) (RAND_MAX)) < Settings::MUTATION_RATE_CREATE_LINK )  {
                    GeneticOperators::createConnection(g);
                }
            }
        }
    }
};

inline void NEAT::run() {
  for (int generation = 0 ; generation < Settings::MAX_NUMBER_GENERATION; generation++ ) {  //termination ?
    // clear innovationBank
    InnovationBank::instance()->resetBank();
    
    cerr << endl << " ********** " << generation << " ************ " << endl;     
    stats();
     
    cerr << "Kill weakests" << endl;
    killWeakests(); 
    
    cerr << "Reproduce" << endl;
    reproduce();
    
    cerr << "Mutate " << endl;
    mutate();
     
    Genome g = getBest();
    if (g._fitness > Settings::STOP_EVOLUTION_AT_FITNESS) {
        cerr << "Found a solution with expected fitness. Ending evolution..." << endl;
        return;
    }
    
    
//    if (g._fitness > 0.98 ) {
//        cerr << "changed!" << endl;
//        Settings::MUTATION_RATE_CREATE_LINK = 0.;
//        Settings::MUTATION_RATE_NODE = 0.;
//        Settings::MUTATION_STEP_WEIGHT = 0.05;
//    }
  }  
  
  evalFitness();  // before exiting
};

inline void NEAT::regulateSpecies() {
    int numberOfSpiecesTargetted = round(Settings::POPULATION_SIZE / Settings::TARGET_SPECIES_SIZE);

    if (numberOfSpiecesTargetted > _species.size()) {
        Settings::decreaseSpeciesThreashold();
    }
    else {
        Settings::increaseSpeciesThreashold();
    }
    cerr << "Threshold = " << Settings::THRESHOLD << " (nb of species wanted = " << numberOfSpiecesTargetted << ")" << endl;
};

inline Genome NEAT::getBest() const {
//    evalFitness();
    Genome theBest;
    theBest._fitness = 0;
    for (const Species& aSpecies: _species) {
        for (const Genome& aGenome: aSpecies._members) {
//            cerr << "current fitness : "<< aGenome._fitness << endl;
            if (aGenome._fitness > theBest._fitness) {
//                cerr << "found better : "<< aGenome._fitness;
                theBest = aGenome;
            }
        }
    }
    //theBest.display();
    cerr << "best fitness found = " << theBest._fitness << endl;
    cerr << "nb of hidden nodes = " << theBest._hiddenLayer.size() << endl;
    return theBest;
}

inline Species& NEAT::getSpeciesById(int aId) {
    auto it = find_if(
            _species.begin(), 
            _species.end(), 
            [aId] (Species& aSpecies) { 
                    return aSpecies._id == aId;});
    if (it != _species.end()) {
        return *it;
    }
    else {
        cerr << "!!!!!!!!! Species " << aId << " not found! !!!!!!!!!!!!!!!!!!" << endl; 
    }
};

inline void NEAT::stats() const {
    cerr << "  Number of Species : " << _species.size() << endl; 
    int n = 0;
    for (const Species& s : _species) {
        n+=s._members.size();
        cerr << "    - Species " << s._id << " : " << s._members.size() << endl; 
    }
    cerr << "    Total genomes : " << n << endl; 
}

#endif // NEAT_H
