#ifndef CONNECTIONGENE_H
#define CONNECTIONGENE_H
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "Settings.h"
#include "IDGenerator.h"

using namespace std;
class ConnectionGene {
public:
    ConnectionGene(int aStart, int aEnd, int aId=-1);
    bool operator< (const ConnectionGene& other) const;
    void setRandomWeight();
    void disable();
    void switchEnableDisable();
    void pointMutate();
    bool isEnabled() const;
    
    int _inputNodeId;
    int _outputNodeId;
    bool _enabled;
    float _weight;
    int _id;
};

inline void ConnectionGene::pointMutate() {
    if (rand()%2 == 0) {
        _weight += Settings::MUTATION_STEP_WEIGHT;
    }
    else {
        _weight -= Settings::MUTATION_STEP_WEIGHT;
    }
};
inline void ConnectionGene::setRandomWeight() {
    _weight = (float(rand()%400)/100. - 2.0);
};
inline void ConnectionGene::disable() {
    _enabled = false;  
};
inline void ConnectionGene::switchEnableDisable() {
    _enabled = !_enabled;  
};
inline bool ConnectionGene::operator< (const ConnectionGene& other) const {
    return _id<other._id;
};
inline ConnectionGene::ConnectionGene(int aStart, int aEnd, int aId)  : _inputNodeId(aStart), _outputNodeId(aEnd) {
    _enabled = true;
    _weight = float(rand()%5) - 2.0;
    if (aId==-1) {
        _id = IDGenerator::instance()->getId();
    }
    else {
        _id = aId;
    }
};
inline bool ConnectionGene::isEnabled() const {
    return _enabled;
};
#endif // CONNECTIONGENE_H
