#include <dataRepository.h>

dataRepository::dataRepository()
{
  custoImplantacao = new dataStruct(1);
  demanda = new dataStruct(1);
  custoTransporte = new dataStruct(2);
  distancia = new dataStruct(2);
  foiAberto = new dataStruct(1);
  fracaoDemanda = new dataStruct(2);
}

double dataRepository::getCustoImplantacao(string poloAbastecimento)
{
  string *index = new string[1];
  string[0] = poloAbastecimento;
  return custoImplantacao->get(index);
}

void dataRepository::setCustoImplantacao(string poloAbastecimento, double value)
{
  string *index = new string[1];
  string[0] = poloAbastecimento;
  custoImplantacao->set(index, value);
}

double dataRepository::getDemanda(string poloAbastecimento)
{
  string *index = new string[1];
  string[0] = poloAbastecimento;
  return demanda->get(index);
}

void dataRepository::setDemanda(string poloAbastecimento, double value)
{
  string *index = new string[1];
  string[0] = poloAbastecimento;
  demanda->set(index, value);
}
double dataRepository::getCustoTransporte(string terminalCandidato, string poloAbastecimento)
{
  string *index = new string[2];
  string[0] = terminalCandidato;
  string[1] = poloAbastecimento;
  return custoTransporte->get(index);
}
void dataRepository::setCustoTransporte(string terminalCandidato, string poloAbastecimento, double value)
{
  string *index = new string[2];
  string[0] = terminalCandidato;
  string[1] = poloAbastecimento;
  custoTransporte->set(index, value);
}
double dataRepository::getDistancia(string terminalCandidato, string poloAbastecimento)
{
  string *index = new string[2];
  string[0] = terminalCandidato;
  string[1] = poloAbastecimento;
  return distancia->get(index);
}

void dataRepository::setDistancia(string terminalCandidato, string poloAbastecimento, double value)
{
  string *index = new string[2];
  string[0] = terminalCandidato;
  string[1] = poloAbastecimento;
  distancia->set(index, value);
}

double dataRepository::getFoiAberto(string terminalCandidato)
{
  string *index = new string[1];
  string[0] = terminalCandidato;
  return foiAberto->get(index);
}
void dataRepository::setFoiAberto(string terminalCandidato, double value)
{
  string *index = new string[1];
  string[0] = terminalCandidato;
  foiAberto->set(index, value);
}
double dataRepository::getFracaoDemanda(string terminalCandidato, string poloAbastecimento)
{
  string *index = new string[2];
  string[0] = terminalCandidato;
  string[1] = poloAbastecimento;
  return fracaoDemanda->get(index);
}

void dataRepository::setFracaoDemanda(string terminalCandidato, string poloAbastecimento, double value)
{
  string *index = new string[2];
  string[0] = terminalCandidato;
  string[1] = poloAbastecimento;
  fracaoDemanda->set(index, value);
}
