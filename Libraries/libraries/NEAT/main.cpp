#include <iostream>
#include "NEAT.h"
#include "Game.h"

#include "cmath"
#include "Genome.h"
using namespace std;


int main(int argc, char **argv)
{
    NEAT aNeat(1,1);
    aNeat.setFitnessFunction(&Game::evalSolution);
    aNeat.run();
    Genome g = aNeat.getBest();

    float angle;
    float aPrediction;
    float anExpectedResult;
    for (int i = 0; i < 36; i++) {
        angle = float(i*10 - 180) * 3.14159265359/180.; 
        aPrediction = g.execute({angle}).at(0);
        anExpectedResult = Game::f(angle);
        
        cerr << aPrediction << " / " << abs(aPrediction - anExpectedResult) << endl;
    }
	return 0;
};

    
//    cerr << " ************ " << endl;
//    
//    for (int i = 0; i < 360; i++) {
//        angle = float(i - 180) * 3.14159265359/180.; 
//        anExpectedResult = Game::f(angle);
//        cerr << anExpectedResult << endl;
//    }
//    
    
    // a genome is a pair of Nodes / links
//    std::pair<std::vector<int>, std::vector<int> >

// or just a vector of links?    
//    std::vector<std::vector<int> > aGenome;
//    aGenome = {
//        {1, 1, 2, 30, 0, 1}, // innovation number, inputNodeId, outputNodeId, weight, activation, bias, link type (input->n , other)
//        {2, 1, 3, 100, 0, 1},
//        {3, 1, 4, 100,  0, 1},
//        {4, 2, 4, 1, 0, 0},
//        {5, 3, 4, -200, 0, 1}
//        };
//        
//    std::map<int, float> aMap;
//    aMap[1] = 10.;
//    for (std::vector<int>& aLink : aGenome) {
//        aMap[aLink.at(2)] += float(aLink.at(3)) /100. * float(aLink.at(4)) * aMap[aLink.at(1)] / (1 + abs(aMap[aLink.at(1)]));
//    }
//    // apply sigmoid to ouptut
//    cerr << aMap[4] << endl;

    // MUTATIONS:
    // point mutate : easy
    
    // nodeMutate silence a connection and adds a connection between the 2 nodes adding 1 new intermediary node :
    // just insert the 2 new connections right after the link mutated.
    
    // enable or disable connection : easy
    
    // ConnectionCreate creates a connection between 2 nodes: takes a node is the node list find an other & push back + swap algo
    
    // delete links: easy
    
    
    // crossOver:
    // more complex: needs to align the 2 genomes by innovation number (so needs a copy and sort)
    