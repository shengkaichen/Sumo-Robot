#ifndef SPECIES_H
#define SPECIES_H
#include "Genome.h"
#include "IDGenerator.h"
//#include "
#include <vector>

class Species
{
public:
    static float THRESHOLD;
    
    Species(int aId = -1);
    bool testGenomeForSpecies(const Genome& iGenome) const;
    void removeGenomeById(int id);
    void addGenome(Genome iGenome);
    void sortByFitness();
    void sortByAdjustedFitness();
    void computeAdjustedFitness();
    Genome& getWorstGenomeAfterAdjust();
    Genome& getGenomeById(int aId) ;
    std::vector<Genome> _members;
    int _id;
};

inline void Species::sortByFitness() {
    std::sort(_members.begin(), 
                _members.end(),
                [] (const Genome& g1, const Genome& g2) { return g1._fitness >  g2._fitness; });
};

inline void Species::sortByAdjustedFitness() {
    std::sort(_members.begin(), 
                _members.end(),
                [] (const Genome& g1, const Genome& g2) { return g1._adjustedFitness >  g2._adjustedFitness; });
};

inline void Species::addGenome(Genome iGenome) {
    _members.push_back(iGenome);
};

inline bool Species::testGenomeForSpecies(const Genome& iGenome) const {
    const Genome& aRefenceGenome = _members.at(rand()%_members.size()); 
    return Genome::getDistance(iGenome, aRefenceGenome) < Settings::THRESHOLD;
};

inline void Species::computeAdjustedFitness() {
    for (Genome& aGenome : _members) {
        // adjusted fitness
        float counter = 0.0;
        for (const Genome& otherGenome : _members) { //  it will be at least equal to 1 since the distance from itself is 0
            if (Genome::getDistance(aGenome, otherGenome) <= Settings::THRESHOLD) {  // could use some cache here / threshold can be 0
                counter +=1.0;
            }
        }
        aGenome.setAdjustedFitness(aGenome._fitness / counter);
    }
};

inline Genome& Species::getWorstGenomeAfterAdjust() {
    computeAdjustedFitness();
    sortByAdjustedFitness();
    return _members.back();
}

//inline void Species::computeFitness() {
//    for (Genome& aGenome : _members) {
//        // adjusted fitness
//        float counter = 0.0;
//        for (const Genome& otherGenome : _members) { //  it will be at least equal to 1 since the distance from itself is 0
//            if (Genome::getDistance(aGenome, otherGenome) <= Settings::THRESHOLD) {  // could use some cache here / threshold can be 0
//                counter +=1.0;
//            }
//        }
//        aGenome.setAdjustedFitness(aGenome._fitness / counter);
//    }
//};

inline Genome& Species::getGenomeById(int aId) {
    auto it = find_if(
            _members.begin(), 
            _members.end(), 
            [aId] (Genome& aGenome) { 
                    return aGenome._id == aId;});
    if (it != _members.end()) {
        return *it;
    }
    else {
        cerr << "!!!!!!!!! Genome " << aId << " not found! !!!!!!!!!!!!!!!!!!" << endl; 
    }
};

inline void Species::removeGenomeById(int aId) {
   _members.erase(
        std::remove_if(
            _members.begin(), 
            _members.end(), 
            [aId] (Genome& aGenome) { 
                    return aGenome._id == aId;}),
        _members.end());
    
};

inline Species::Species(int aId) {
    if (aId == -1) {
        _id = IDGenerator::instance()->getId();
    }
    else {
        _id = aId;
    }
};


#endif // SPECIES_H
