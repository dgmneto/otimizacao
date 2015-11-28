#include terminalCandidato.h

terminalCandidato::terminalCandidato(int index, polo *reference)
{
  this->index = index;
  this->reference = reference;
}

int getIndex()
{
  return index;
}

void setIndex(int index)
{
  this->index = index;
}

polo *terminalCandidato::getReference()
{
  return reference;
}
