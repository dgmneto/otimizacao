#include <polo.h>

polo::polo(string instanceName, bool terminalCandidato)
{
  this->instanceName = instanceName;
  this->terminalCandidato = terminalCandidato;
}

string polo::getInstanceName()
{
  return instanceName;
}

int polo::getIndex()
{
  return index;
}

void polo::setIndex(int index)
{
  this->index = index;
}

bool polo::isTerminalCandidato()
{
  return terminalCandidato;
}

void polo::setTerminalCandidato(bool terminalCandidato)
{
  this->terminalCandidato = terminalCandidato;
}
