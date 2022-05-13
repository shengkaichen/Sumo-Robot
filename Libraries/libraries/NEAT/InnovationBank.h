#ifndef INNOVATIONBANK_H
#define INNOVATIONBANK_H
#include <iostream>
#include <stdio.h>
#include <map>
#include <utility>
#include <tuple>
#include "IDGenerator.h"

class InnovationBank {
public:
    static InnovationBank* instance ();
//    int getId ();
    std::tuple<int, int, int> getIdsAndNodeId(int inputId, int outputId);
    bool isInnovationNew(int inputId, int outputId);
    void registerInnovation(int inputId, int outputId, int nodeId, int inputConnectionId, int outputConnectionId);
    void resetBank();
private:
    InnovationBank () : _id(0) {};
    static InnovationBank* _address;
    int _id;
    std::map<std::pair<int, int>, std::tuple<int, int, int> > _innovationMap;
};

inline void InnovationBank::resetBank() {
    _innovationMap.clear();
}


inline void InnovationBank::registerInnovation(int inputId, int outputId, int newNodeId, int inputConnectionId, int outputConnectionId) {
    std::pair<int, int> aIdPair = std::make_pair(inputId, outputId);
    std::tuple<int, int, int> aTuple = std::make_tuple(newNodeId, inputConnectionId, outputConnectionId);
    _innovationMap.insert(std::make_pair(aIdPair, aTuple));
};

inline std::tuple<int, int, int> InnovationBank::getIdsAndNodeId(int inputId, int outputId) {
    std::pair<int, int> aIdPair = std::make_pair(inputId, outputId);
    if (!isInnovationNew(inputId,outputId)) {
        return _innovationMap[aIdPair];
    }
    else {
        IDGenerator* aIdGenerator = IDGenerator::instance();
        int aNewNodeId = aIdGenerator->getId();
        int aNewConnectionId_1 = aIdGenerator->getId();
        int aNewConnectionId_2 = aIdGenerator->getId();
        registerInnovation(inputId, outputId, aNewNodeId, aNewConnectionId_1, aNewConnectionId_2);
        return std::make_tuple(aNewNodeId,aNewConnectionId_1,aNewConnectionId_2);
    }
};

//inline int InnovationBank::getId() {
//    return _id++; 
//};



#endif // INNOVATIONBANK_H
