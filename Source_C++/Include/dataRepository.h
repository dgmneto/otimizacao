#ifndef DATAREPOSITORY_H
#define DATAREPOSITORY_H

#include <dataStruct.h>
#include <string>

class dataRepository
{
public:
  dataRepository();
  ~dataRepository();
  void loadFase_I();
  double getCustoImplantacao(string poloAbastecimento);
  double getDemanda(string poloAbastecimento);
  double getCustoTransporte(string terminalCandidato, string poloAbastecimento);
  double getDistancia(string terminalCandidato, string poloAbastecimento);
  double getFoiAbertoTerminal(string terminalCandidato);
  void setFoiAbertoTerminal(string terminalCandidato, double value);
  double getFracaoDemanda(string terminalCandidato, string poloAbastecimento);
  void setFracaoDemanda(string terminalCandidato, string poloAbastecimento, double value);
  void loadFase_II();
  double getCustoAbertura(string from, string to);
  double getCustoFluxo(string from, string to);
  double getFluxoMaximo(string from, string to);
  void setFoiAbertoDuto(string from, string to, double value);
  void setFluxoCanal(string from, string to, double value);

private:
  string *poloAbastecimento, *poloSuprimento, *terminalCandidato;
  int a, s, t;
  dataStruct *custoImplantacao;
  dataStruct *demanda;
  dataStruct *custoTransporte;
  dataStruct *distancia;
  dataStruct *foiAbertoTerminal;
  dataStruct *fracaoDemanda;

  dataStruct *custoAbertura;
  dataStruct *custoFluxo;
  dataStruct *fluxoMaximo;
  dataStruct *foiAbertoDuto;
  dataStruct *fluxoCanal;
};
#endif
