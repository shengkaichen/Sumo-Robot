#include "Genome.h"
#include "InnovationBank.h"
#include "IDGenerator.h"


// constructors
Genome::Genome(int id) {
    if (id == -1) {
        getNewId();
    }
    else {
        _id = id;
    }
}
Genome::Genome(int numberOfInputNode, int numberOfOuputNode, int id) {
    // add input nodes
    for(int i=0; i<numberOfInputNode; i++) {
        addNode(NodeGene("input"));
    }
    // add output nodes
    for(int i=0; i<numberOfOuputNode; i++) {
        addNode(NodeGene("output"));
    }
    addNode(NodeGene("bias")); // will be stored in inputLayer
    
    for (int const& aId1: _inputLayer) {
        for (int const& aId2: _outputLayer) {
            addConnection(ConnectionGene(aId1, aId2));       
        }     
    }    
    // set id
    if (id == -1) {
        getNewId();
    }
    else {
        _id = id;
    }
    _maxId = 0;
};


// MISC //////////////////////////////////////
// sorting
void Genome::sortAllNodes() {
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (const ConnectionGene& aCon : _connections) {
            auto i = std::find(_allNodes.begin(), _allNodes.end(), aCon._inputNodeId);
            auto o = std::find(_allNodes.begin(), _allNodes.end(), aCon._outputNodeId);
            // we need to verify that o is after i in _allNodes
            if (std::distance(i,o)<0) {
                // if not switch positions
                std::iter_swap(i, o);
                sorted = false;
            }
        }
    }
}

// testing
bool Genome::hasConnectionId(int aConId) const {
    auto it = find_if(
            _connections.begin(), 
            _connections.end(), 
            [aConId] (const ConnectionGene& aConnnectionGene) { 
                    return aConnnectionGene._id == aConId;});
    return (it!=_connections.end());
};

void Genome::randomizeWeight() {
    for (ConnectionGene& aCon: _connections) {
        aCon.setRandomWeight();
    }
};



// NETWORK RUN ///////////////////////////////
map<int, vector<ConnectionGene> > Genome::getNodeIncomingConnectionMap() const {
    // returns a map from node id to all incoming connections (for which the node id is registered as the output)
    std::map<int, vector<ConnectionGene> > aMap;
    for (int nodeId : _allNodes) {
        vector<ConnectionGene> incomingConnections;
        for (ConnectionGene aConnection: _connections) {
            if (aConnection._outputNodeId == nodeId) {
                incomingConnections.push_back(aConnection);
            }
        }
        aMap.insert(std::make_pair(nodeId, incomingConnections));
    }
        
    return aMap;
};

// runs network!
vector<float> Genome::execute(const vector<float>& input) {
    
    if (input.size() != _inputLayer.size()) { 
        cerr << "invalid input : not enough input nodes or too much: entries number = "
                << input.size() << " / input nodes number = " << _inputLayer.size() << endl;
        throw "Error : invalid input ";
    }
    else {
        // clean network
        for (int nodeId : _allNodes) {
            _nodes[nodeId].reset();
        }
        // fill inputs layer with values
        for (unsigned int i=0; i<_inputLayer.size(); i++) { 
            _nodes[_inputLayer.at(i)]._value = input.at(i);
        }
    }

    map<int, vector<ConnectionGene> > aNodeInputMap = getNodeIncomingConnectionMap();
    for (int nodeId : _allNodes) {
        // they are ordered so we can compute one node after an other
        if (!_nodes[nodeId].isInput() && !_nodes[nodeId].isBias()) {
            float aNodeInput = 0.0;
            // we multiply each input by the weight of the connection
            for (const ConnectionGene& aConnection : aNodeInputMap[nodeId]) {
                if (aConnection._enabled) {
                    aNodeInput += aConnection._weight * _nodes[aConnection._inputNodeId]._value;
                }
            }
            _nodes[nodeId]._value = aNodeInput / (1.0 + abs(aNodeInput));
        }
    }
    
    // return output
    vector<float> aOutput;
    for (int const& aOutputNodeId: _outputLayer) {
        aOutput.push_back(_nodes[aOutputNodeId]._value);
    }
    return aOutput;
};


// UTILS /////////////////////////////////////
void Genome::display() const {
//    sortConnectionById();
    cerr << endl << " Genome " << _id << " ---------------------------- " << endl 
        << "List of input nodes  : ";
    for (int n : _inputLayer) {
        cerr << n << " " ;
    }
    cerr << endl << "List of output nodes : ";
    for (int n : _outputLayer) {
        cerr << n << " " ;
    }
    cerr << endl << "List of hidden nodes : ";
    for (int n : _hiddenLayer) {
        cerr << n << " " ;
    }
    cerr << endl<< "List of active connections : "<<endl;
    
    for (const ConnectionGene& c: _connections) {
        if (c._enabled) {
            cerr << c._inputNodeId 
            << " ===(" << c._weight << " / N-" <<c._id <<")===> " << c._outputNodeId << endl;
        }
    }
    cerr << "List of inactive connections : "<<endl;
        for (const ConnectionGene& c: _connections) {
        if (!c._enabled) {
            cerr << c._inputNodeId 
            << " ===(" << c._weight << " / N-" <<c._id <<")===> " << c._outputNodeId << endl;
        }
    }
    cerr << endl;
};

void Genome::printCPP() const {
    cerr<< "----------------------------------------------------------"<<endl;
    map<int, vector<ConnectionGene> > aNodeInputMap = getNodeIncomingConnectionMap();
    
    map<int, NodeGene> nodesCopy = _nodes; 
    for (int nodeId : _allNodes) {
        // they are ordered so we can compute one node after an other
        if (nodesCopy[nodeId].isInput()) {
            cerr << "float n" << nodeId << " = input ;" << endl;
        }
        if (nodesCopy[nodeId].isHidden()) {
            cerr << "float n" << nodeId << " = 0.0 ;" << endl;        
        }
        if (nodesCopy[nodeId].isOutput()) {
            cerr << "float n" << nodeId << " = 0.0 ; // <= your output" << endl;   
        }
    }
     
    for (int nodeId : _allNodes) {
        if (nodesCopy[nodeId].isOutput() || nodesCopy[nodeId].isHidden()) {
            // we multiply each input by the weight of the connection
            for (const ConnectionGene& aConnection : aNodeInputMap[nodeId]) {
                if (aConnection._enabled) {
                    cerr << "n" << nodeId << " += float(" 
                            << aConnection._weight 
                            << ") * n" << aConnection._inputNodeId
                            << ";" << endl;
                }
            }
            
//            if (nodesCopy[nodeId].isHidden()) {
                cerr << "n" << nodeId 
                        << " = n" << nodeId 
                        << " / (1.0 + abs(n" << nodeId  <<"));" << endl;
//            }
        }
    }
    cerr<< "----------------------------------------------------------"<<endl;
};