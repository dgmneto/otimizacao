#ifndef TERMINALCANDIDATO_H
#define TERMINALCANDIDATO_H
#include <polo.h>
#include "gurobi_c++.h"
class terminalCandidato
{
public:
  terminalCandidato(polo *reference);
  int getIndex();
  void setIndex(int index);
  polo *getReference();
private:
  int index;
  polo *reference;
}
#endif
