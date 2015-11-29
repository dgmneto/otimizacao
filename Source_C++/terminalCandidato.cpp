#include <terminalCandidato.h>

terminalCandidato::terminalCandidato(polo *reference)
{
  this->reference = reference;
}

int terminalCandidato::getIndex()
{
  return index;
}

void terminalCandidato::setIndex(int index)
{
  this->index = index;
}

polo *terminalCandidato::getReference()
{
  return reference;
}
