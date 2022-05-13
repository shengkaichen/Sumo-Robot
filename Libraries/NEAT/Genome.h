#ifndef GENOME_H
#define GENOME_H
#include "ConnectionGene.h"
#include "IDGenerator.h"
#include "NodeGene.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "Settings.h"

class Genome {  // a genome is a network
public:
    Genome(int id = -1);
    Genome(int numberOfInputNode, int numberOfOuputNode, int id = -1);
    
    // UTILS
    void getNewId();
    void randomizeWeight();
    void display() const;
    void printCPP() const;
    
    // sorting
    void sortConnectionById();
    void sortAllNodes();
    
    // setters
    void setFitness(float iFitness);
    void setAdjustedFitness(float adjustmentFactor);
    
    map<int, vector<ConnectionGene> > getNodeIncomingConnectionMap() const;
    map<int, NodeGene> getNodeMap() const;
    
    // add elements to Genome
    void addNode(NodeGene iNode, int insertAfterNode=-1);
    void addConnection(ConnectionGene iConnection, int insertAfterConnection=-1);
    bool areNodesAlreadyConnected(int a, int b) const;
    
    ConnectionGene& getConnectionFromId(int nb);
    ConnectionGene getCopyConnectionFromId(int nb) const;
    static float getDistance(const Genome&  iGenome1, const Genome& iGenome2);    
    static void getTopologicalComparision(const Genome&  iGenome1, 
                                            const Genome& iGenome2, 
                                            vector<int>& commonGenes,
                                            vector<int>& disjointInGenome1,
                                            vector<int>& disjointInGenome2,
                                            vector<int>& excessInGenome1,
                                            vector<int>& excessInGenome2);
    
    // runs network!
    vector<float> execute(const vector<float>& input);
    
    // attributes
    int _id;
    float _fitness;
    float _adjustedFitness;
    
//private:
    int _maxId;
    
    // collections of Ids:
    vector<int> _biasLayer; 
    vector<int> _inputLayer; 
    vector<int> _outputLayer; 
    vector<int> _hiddenLayer; 
    vector<int> _allNodes;
    
    map<int, NodeGene> _nodes; 
    vector<ConnectionGene> _connections; 
    
    // check if a node has connection or exists
//    bool hasConnectionWithNodeId(int aNodeId) const;
    bool hasConnectionId(int aConId) const;
    bool isNodeRegistered(int aNodeId) const;
};

inline void Genome::addConnection(ConnectionGene iCon, int aConId) {
    auto it = find_if(
            _connections.begin(), 
            _connections.end(), 
            [aConId] (const ConnectionGene& aConnnectionGene) { 
                    return aConnnectionGene._id == aConId;});
    if (it!=_connections.end()) {it++;}
    _connections.insert(it, iCon); // insert at the end by default
};

inline ConnectionGene& Genome::getConnectionFromId(int aConId) {    
    auto it = find_if(
            _connections.begin(), 
            _connections.end(), 
            [aConId] (const ConnectionGene& aConnnectionGene) { 
                    return aConnnectionGene._id == aConId;});
    return *it;
};

inline ConnectionGene Genome::getCopyConnectionFromId(int aConId) const {    
    auto it = find_if(
            _connections.begin(), 
            _connections.end(), 
            [aConId] (const ConnectionGene& aConnnectionGene) { 
                    return aConnnectionGene._id == aConId;});
    ConnectionGene aCon = (*it);
    return aCon;
};

inline map<int, NodeGene> Genome::getNodeMap() const {
  return _nodes;  
};

inline bool Genome::isNodeRegistered(int iNodeId) const {
    return std::find(_allNodes.begin(), _allNodes.end(), iNodeId) != _allNodes.end();
};

inline void Genome::addNode(NodeGene iNode, int insertAfterNode) {
    _nodes.insert(std::pair<int, NodeGene>(iNode._id, iNode));
    
    auto it = find (_allNodes.begin(), _allNodes.end(), insertAfterNode);
    if (it!=_allNodes.end()) {it++;}
    
    _allNodes.insert(it, iNode._id); // insert at the end by default
    
    if (iNode.isInput())  {
        _inputLayer.push_back(iNode._id);
    } 
    else if (iNode.isOutput())  {
        _outputLayer.push_back(iNode._id);
    } 
    else if (iNode.isBias())  {
        _biasLayer.push_back(iNode._id);
    } 
    else {
        _hiddenLayer.push_back(iNode._id);
    }
};

inline void Genome::setFitness(float iFitness) {
  _fitness = iFitness;
};

inline void Genome::setAdjustedFitness(float iAdjustedfitness) {
  _adjustedFitness =  iAdjustedfitness;
};

inline float Genome::getDistance(const Genome&  iGenome1, const Genome& iGenome2) {
    float aReturnedDistance;
    // dist = (excess * c1 + disjoint * c2 ) / N + c3 * AverageWeightDifference
    // c1, c2, c3 arbitary, N=1 or normalization (= total number of genes for the biggest)
    vector<int> commonGenes;
    vector<int> disjointInGenome1;
    vector<int> disjointInGenome2;
    vector<int> excessInGenome1;
    vector<int> excessInGenome2;
    getTopologicalComparision(iGenome1, 
                            iGenome2, 
                            commonGenes,
                            disjointInGenome1,
                            disjointInGenome2,
                            excessInGenome1,
                            excessInGenome2);
    
    float N = float(max(iGenome1._connections.size(), iGenome2._connections.size()));
    float W = 0.0;
    for (int innovNber : commonGenes) {
        W += iGenome1.getCopyConnectionFromId(innovNber)._weight;
        W -= iGenome2.getCopyConnectionFromId(innovNber)._weight;
    }
    W = abs( W / float(commonGenes.size()) );
    
    aReturnedDistance = (Settings::EXCESS_DIST_FACTOR * float(excessInGenome1.size() + excessInGenome2.size())) / N
                      + (Settings::DISJOINT_DIST_FACTOR * float(disjointInGenome1.size() + disjointInGenome2.size())) / N  
                      +  Settings::WEIGHT_DIST_FACTOR * W;
    
    if (false) {
        cerr << "average weight difference = " << W << endl;
        cerr << "nb of genes in excess = " << float(excessInGenome1.size() + excessInGenome2.size()) << endl;
        cerr << "nb of genes disjoint  = " << float(disjointInGenome1.size() + disjointInGenome2.size()) << endl;
        cerr << "distance = " << aReturnedDistance << endl;
    }
    return aReturnedDistance;
};

inline void Genome::sortConnectionById() {
    std::sort(_connections.begin(),
        _connections.end());
};

inline void Genome::getTopologicalComparision(const Genome&  iGenome1, 
                                        const Genome& iGenome2, 
                                        vector<int>& commonGenes,
                                        vector<int>& disjointInGenome1,
                                        vector<int>& disjointInGenome2,
                                        vector<int>& excessInGenome1,
                                        vector<int>& excessInGenome2) {
    int minMaxInnovNumber = min(iGenome1._maxId, iGenome2._maxId);
    
    for (const ConnectionGene& aCon: iGenome1._connections) {  
        // check if it's common
        if (iGenome2.hasConnectionId(aCon._id)) {
            commonGenes.push_back(aCon._id);
        }
        else if (aCon._id < minMaxInnovNumber) {
            disjointInGenome1.push_back(aCon._id);
        }
        else {
            excessInGenome1.push_back(aCon._id);
        }
    }
    
    for (const ConnectionGene& aCon: iGenome2._connections) {  
        if (std::find(commonGenes.begin(), commonGenes.end(), aCon._id) == commonGenes.end()) {
            if (aCon._id < minMaxInnovNumber) {
                disjointInGenome2.push_back(aCon._id);
            }
            else {
                excessInGenome2.push_back(aCon._id);
            }
        }
    }
};

inline bool Genome::areNodesAlreadyConnected(int a, int b) const {
    auto it = find_if(
        _connections.begin(), 
        _connections.end(), 
        [a, b] (const ConnectionGene& aConnnectionGene) { 
                return (aConnnectionGene._inputNodeId == a && aConnnectionGene._outputNodeId == b) 
                || (aConnnectionGene._inputNodeId == b && aConnnectionGene._outputNodeId == a);
                });
    return it != _connections.end();
};

inline void Genome::getNewId() {
    _id = IDGenerator::instance()->getId();
};
#endif // GENOME_H
