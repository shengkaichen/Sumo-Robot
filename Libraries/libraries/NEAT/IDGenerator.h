#ifndef IDGENERATOR_H
#define IDGENERATOR_H
#include <iostream>

class IDGenerator {
   public:
      static IDGenerator* instance ();
      int getId () { 
          return _id++; };
   private:
      IDGenerator () : _id(0) {};

      static IDGenerator* _address;
      int _id;
};

#endif // IDGENERATOR_H
