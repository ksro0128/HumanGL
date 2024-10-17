#ifndef __HUMAN_H__
#define __HUMAN_H__

#include "common.h"
#include "mesh.h"

CLASS_PTR(Human)
class Human {
public:
    static HumanUPtr Create();
private:
    Human() {}
};


#endif // __HUMAN_H__