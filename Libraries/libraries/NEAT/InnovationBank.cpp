#include "InnovationBank.h"
#include <iostream>

InnovationBank* InnovationBank::_address = 0;
InnovationBank* InnovationBank::instance () {
   if (!_address) {
      _address = new InnovationBank();
   }
   return _address;
};

bool InnovationBank::isInnovationNew(int inputId, int outputId) {
    std::pair<int, int> aIdPair = std::make_pair(inputId, outputId);
    std::map<std::pair<int, int>, std::tuple<int, int, int> >::iterator it;
    it = _innovationMap.find(aIdPair);
    if (it != _innovationMap.end() )
    {
        return false;
    }
    else {
        return true;
    }
};