#include "Genome.h"
#include "Settings.h"
#include "InnovationBank.h"
#include <algorithm>

class GeneticOperators {
public:
    static void weightMutate(Genome& iGenome) { 
        // one chance out of 2 to increase or decrease the weight
        int i = 1.;
        if (rand()%2 == 0) { 
            i = -1.;
        }
        iGenome._connections
                .at(rand()%iGenome._connections.size())
                ._weight 
                += Settings::MUTATION_STEP_WEIGHT * i;
    };

    static void nodeMutate(Genome& iGenome) {
        cerr<< "nodeMutate" <<endl;
        // choose a link not connected to a bias and insert a node.
        
        // 1- choose a link where to insert a node
        vector<int> aListOfEligibleConnectionIds;
        int aBiasId = iGenome._biasLayer.at(0);
        for (const ConnectionGene& aConnection : iGenome._connections) {
            if (aConnection.isEnabled() && (aConnection._inputNodeId != aBiasId) ) { 
                aListOfEligibleConnectionIds.push_back(aConnection._id);
            }
        }
        if (aListOfEligibleConnectionIds.empty()) {
            return; // all links are mutated. it should never happen.
        }            
        ConnectionGene& aConnectionMutated = iGenome.getConnectionFromId(
                aListOfEligibleConnectionIds.at(rand()%aListOfEligibleConnectionIds.size()));


        // 2- disable mutated connection
        aConnectionMutated.switchEnableDisable();
        
        // 3- call the innovation bank to get innovation numbers and the new node id's 
        int aInputNodeId = aConnectionMutated._inputNodeId;
        int aOutputNodeId = aConnectionMutated._outputNodeId;
        std::tuple<int, int, int> aIdTuple = InnovationBank::instance()->getIdsAndNodeId(aInputNodeId, aOutputNodeId);
        
        // 4- add genes and connections
        // add new gene
        NodeGene aNodeGene("hidden", std::get<0>(aIdTuple)); // hidden by default
        iGenome.addNode(aNodeGene, aInputNodeId); // we need to insert it right after the inputNodeId
        
        // add new connections
        ConnectionGene aConnection1(aInputNodeId, aNodeGene._id, std::get<1>(aIdTuple));
        aConnection1._weight = 1.0;
        iGenome.addConnection(aConnection1, aConnectionMutated._id);

        ConnectionGene aConnection2(aNodeGene._id, aOutputNodeId, std::get<2>(aIdTuple));
        aConnection2._weight = aConnectionMutated._weight;
        iGenome.addConnection(aConnection2, aConnection1._id); 
    
        // 5- register last topological evolution in the current genome
        iGenome._maxId = max({iGenome._maxId, 
                    aConnection1._id,
                    aConnection2._id});
    };

    static void enableDisableMutate(Genome& iGenome) {
        iGenome._connections
                .at(rand()%iGenome._connections.size())
                .switchEnableDisable();
    };

    static void createConnection(Genome& iGenome) {
        cerr<< "createConnection" <<endl;
        /*
         * check previous implementation in the bottom
         * 
         * the current solution is much simpler but it  is still perfect
         * some links have more chances to be created than others. 
         * 
         * logic: 
         * we pick 2 random nodes 
         *  - check if there're different / not already linked / not 2 entries / not 2 outputs if there are not repick another
         *  - check one it depending on ther other, if so, it determines the order
         *  - 10 max tries before giving up.
         */
         
         int nbOfTries = 0;
         while (nbOfTries < 10) {
            nbOfTries++;
            
            // 0- init map of possible connections
            int aFirstNodeId = iGenome._allNodes.at(rand()%iGenome._allNodes.size());
            int aSecondNodeId = iGenome._allNodes.at(rand()%iGenome._allNodes.size());
            
            const NodeGene& aFirstNode = iGenome._nodes[aFirstNodeId];
            const NodeGene& aSecondNode = iGenome._nodes[aSecondNodeId];
            
            // 1- check eligibility 
            if (aFirstNodeId != aSecondNodeId 
                && !(aFirstNode.isInput() && aSecondNode.isInput())
                && !(aFirstNode.isOutput() && aSecondNode.isOutput())
                && !(aFirstNode.isBias() && !aSecondNode.isHidden())
                && !(!aFirstNode.isHidden() && aSecondNode.isBias())
                && !iGenome.areNodesAlreadyConnected(aFirstNodeId, aSecondNodeId)
                ) {
                
                map<int, vector<ConnectionGene> > aMapOfIncommingConnections = iGenome.getNodeIncomingConnectionMap();
                // we assume the node upstream is the 1st node. 
                // so we start from it and get all the upper nodes
                std::vector<int> aWorkset;
                bool invert = false;
                aWorkset.push_back(aFirstNodeId);
                while (!aWorkset.empty() && !invert) { 
                    std::vector<int> aNewWorkset;
                    for (int aNodeExplored: aWorkset) {
                        for (const ConnectionGene& aCon: aMapOfIncommingConnections[aNodeExplored]) {
                            aNewWorkset.push_back(aCon._inputNodeId);
                            if (aCon._inputNodeId == aSecondNodeId) {
                                invert = true; 
                                // in this case, the first node depends on the second node's value
                                // we need to invert the order
                            }
                        }
                    }
                    aWorkset = aNewWorkset;
                }
                
                if (invert) {
                    std::swap(aFirstNodeId, aSecondNodeId);
                }
                
                if (!iGenome._nodes[aSecondNodeId].isBias()) {
                    ConnectionGene aNewConnection(aFirstNodeId, aSecondNodeId);
                    iGenome.addConnection(aNewConnection);
                    return;
                }
            }
         }
         
         cerr << "\> failure" << endl;
    };
    
    
    static Genome crossOver(const Genome& fitest, const Genome& weakest) {
        cerr<< "cross";
        bool equal = (fitest._fitness == weakest._fitness);
        
//        fitest.display();
//        weakest.display();
        
        // the fitest will be the model
        Genome aReturnedGenome;
         // build connections
         for (ConnectionGene aCon : fitest._connections) {
             // common connections are inherited randomley from the parents 
             if (weakest.hasConnectionId(aCon._id)) {  
                 if (rand()%2 == 0) {  // get from weakest
                     aCon = weakest.getCopyConnectionFromId(aCon._id);
                 }
                aReturnedGenome.addConnection(aCon);
             }
             // nodes in excess or  disjoint:
             // - equal fitness = take randomly 50% chance
             // - or : take from fitest
             else  if (!equal || (rand()%2 == 0)) {
                aReturnedGenome.addConnection(aCon);
             }
         }
         if (equal) {
            for (ConnectionGene aCon : weakest._connections) {
                if (!fitest.hasConnectionId(aCon._id) && (rand()%2 == 0)) { 
                     aReturnedGenome.addConnection(aCon);
                }
            }
         }
         
         
         // reconstruct nodes
         // step 1 : merge the 2 maps from fitest ans weakest
         map<int, NodeGene> aMapForFitest = fitest.getNodeMap();
         map<int, NodeGene> aMapForWeakest = weakest.getNodeMap();
         aMapForFitest.insert(aMapForWeakest.begin(), aMapForWeakest.end());
         
         // step 2 : add bias nodes
         for (const int aInputNodeId : fitest._biasLayer) {
             aReturnedGenome.addNode(aMapForFitest[aInputNodeId]);
         }
         
         // step 3 : for each node found in connections, we add it
         // we still  need to have them ordered in _allNodes     
        for (const ConnectionGene& aCon : aReturnedGenome._connections) {
            if (!aReturnedGenome.isNodeRegistered(aCon._inputNodeId)) {
                aReturnedGenome.addNode(aMapForFitest[aCon._inputNodeId]);
            }
            if (!aReturnedGenome.isNodeRegistered(aCon._outputNodeId)) {
                aReturnedGenome.addNode(aMapForFitest[aCon._outputNodeId]);
            }
         }
         
         aReturnedGenome.sortAllNodes();
         
         // apply 25% chances to reactivate disabled genes
         for (ConnectionGene& aCon : aReturnedGenome._connections) {
             if (!aCon._enabled && rand()%4==0) {
                 aCon.switchEnableDisable();
             }
         }
        
        cerr<< "over" <<endl;
        return aReturnedGenome;
    };
    
    
    
    
    
};



//void Genome::createConnection() {
//
//    // output candidates are 
//    // 1- not upstream, 
//    // 2- not already linked
//    // 3- not itself
//    std::vector<int> anExclusionList;
//    anExclusionList.push_back(i);
//    // nodes upstream
//    map<int, vector<ConnectionGene> > aMapOfIncommingConnections = getNodeIncomingConnectionMap();
//    bool allNetworkScanned = false;
//    while (!allNetworkScanned) {
//        allNetworkScanned = true;
//        for (int aNodeIdExcluded: anExclusionList) {
//            for (const ConnectionGene& aCon: aMapOfIncommingConnections[aNodeIdExcluded]) {
//                if (std::find(anExclusionList.begin(),
//                                anExclusionList.end(),
//                                aCon._inputNodeId) == anExclusionList.end() ) {
//                    // if the connection is linked to an updstream node
//                    anExclusionList.push_back(aCon._inputNodeId);
//                    allNetworkScanned = false;
//                }
//            }
//                
//        }
//    } 
//    // nodes already linked
//    for (const ConnectionGene& aCon: _connections) {
//        if (aCon._inputNodeId == i) {
//            anExclusionList.push_back(aCon._outputNodeId);
//        }
//    }
//    // build list based on hidden and output layers, fitlering out the nodes on the exclusion list
//    std::vector<int> outputCandidateIds;
//    for (int aNodeId : _hiddenLayer) {
//        if (std::find(anExclusionList.begin(),
//                    anExclusionList.end(),
//                    aNodeId) == anExclusionList.end()) {
//            outputCandidateIds.push_back(aNodeId);
//        }
//    }
//    for (int aNodeId : _outputLayer) {
//        if (std::find(anExclusionList.begin(),
//                    anExclusionList.end(),
//                    aNodeId) == anExclusionList.end()) {
//            outputCandidateIds.push_back(aNodeId);
//        }
//    }
//    
//    if (!outputCandidateIds.empty()) { // TODO: register innovation
//        int o = outputCandidateIds.at(rand()%outputCandidateIds.size());
////        cerr << "we create a new connection from "<< i << " to " << o << endl;
//        ConnectionGene aNewConnection(i, o);
//        _connections.push_back(aNewConnection);
//    }
//    else {
////        cerr << "we cannot create a new connection" << endl;
//    }
//}