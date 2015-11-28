#ifndef DATAREPOSITORY_H
#define DATAREPOSITORY_H

#include <dataStruct.h>

class dataRepository
{
public:
  dataRepository();
  double getCustoImplantacao(string poloAbastecimento);
  void setCustoImplantacao(string poloAbastecimento, double value);
  double getDemanda(string poloAbastecimento);
  void setDemanda(string poloAbastecimento, double value);
  double getCustoTransporte(string terminalCandidato, string poloAbastecimento);
  void setCustoTransporte(string terminalCandidato, string poloAbastecimento, double value);
  double getDistancia(string terminalCandidato, string poloAbastecimento);
  void setDistancia(string terminalCandidato, string poloAbastecimento, double value);
  double getFoiAberto(string terminalCandidato);
  void setFoiAberto(string terminalCandidato, double value);
  double getFracaoDemanda(string terminalCandidato, string poloAbastecimento);
  void setFracaoDemanda(string terminalCandidato, string poloAbastecimento, double value);
private:
  dataStruct *custoImplantacao;
  dataStruct *demanda;
  dataStruct *custoTransporte;
  dataStruct *distancia;
  dataStruct *foiAberto;
  dataStruct *fracaoDemanda;
}
#endif
