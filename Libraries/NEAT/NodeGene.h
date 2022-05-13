#ifndef NODEGENE_H
#define NODEGENE_H
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "IDGenerator.h"
using namespace std;
class NodeGene {
public:
    NodeGene(string iType = "hidden", int aId = -1); 
    void reset();
    bool isHidden() const;
    bool isInput() const;
    bool isOutput() const;
    bool isBias() const;
    int _id; // id
    float _value; 
protected:
    string _type;
};

inline bool NodeGene::isHidden() const {
    return _type=="hidden"; 
};
inline bool NodeGene::isInput() const {
    return _type=="input"; 
};
inline bool NodeGene::isOutput() const {
    return _type=="output"; 
};
inline bool NodeGene::isBias() const {
    return _type=="bias"; 
};
inline void NodeGene::reset() {
    if (!isBias()) {
        _value = 0.0;
    }
    else {
        _value = 1.0;
    }
};
inline NodeGene::NodeGene(std::string iType, int aId) {
    if (aId == -1) {
        _id = IDGenerator::instance()->getId();
    }
    else {
        _id = aId;
    }
    _type = iType;
};

#endif // NODEGENE_H
