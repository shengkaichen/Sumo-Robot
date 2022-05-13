#include "IDGenerator.h"


IDGenerator* IDGenerator::_address = 0;
IDGenerator*  IDGenerator::instance () {
   if (!_address) {
      _address = new IDGenerator();
   }
   return _address;
};
