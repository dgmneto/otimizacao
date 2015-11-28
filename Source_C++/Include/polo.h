#ifndef POLO_H
#define POLO_H
class polo
{
public:
  polo(string instanceName, bool terminalCandidato);
  string getInstanceName();
  int getIndex();
  void setIndex(int index);
  bool isTerminalCandidato();
private:
  int index;
  string instanceName;
  bool terminalCandidato;
}
#endif