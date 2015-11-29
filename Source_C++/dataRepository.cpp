#include <dataRepository.h>

dataRepository::dataRepository()
{
  custoImplantacao = new dataStruct(1);
  demanda = new dataStruct(1);
  custoTransporte = new dataStruct(2);
  distancia = new dataStruct(2);
  foiAbertoTerminal = new dataStruct(1);
  fracaoDemanda = new dataStruct(2);
}

double dataRepository::getCustoImplantacao(string poloAbastecimento)
{
  string *index = new string[1];
  index[0] = poloAbastecimento;
  return custoImplantacao->get(index);
}

double dataRepository::getDemanda(string poloAbastecimento)
{
  string *index = new string[1];
  index[0] = poloAbastecimento;
  return demanda->get(index);
}

double dataRepository::getCustoTransporte(string terminalCandidato, string poloAbastecimento)
{
  string *index = new string[2];
  index[0] = terminalCandidato;
  index[1] = poloAbastecimento;
  return custoTransporte->get(index);
}
double dataRepository::getDistancia(string terminalCandidato, string poloAbastecimento)
{
  string *index = new string[2];
  index[0] = terminalCandidato;
  index[1] = poloAbastecimento;
  return distancia->get(index);
}

double dataRepository::getFoiAbertoTerminal(string terminalCandidato)
{
  string *index = new string[1];
  index[0] = terminalCandidato;
  return foiAbertoTerminal->get(index);
}
void dataRepository::setFoiAbertoTerminal(string terminalCandidato, double value)
{
  string *index = new string[1];
  index[0] = terminalCandidato;
  foiAbertoTerminal->set(index, value);
}
double dataRepository::getFracaoDemanda(string terminalCandidato, string poloAbastecimento)
{
  string *index = new string[2];
  index[0] = terminalCandidato;
  index[1] = poloAbastecimento;
  return fracaoDemanda->get(index);
}

void dataRepository::setFracaoDemanda(string terminalCandidato, string poloAbastecimento, double value)
{
  string *index = new string[2];
  index[0] = terminalCandidato;
  index[1] = poloAbastecimento;
  fracaoDemanda->set(index, value);
}

void dataRepository::loadFase_II()
{
  for(dataStruct *data = fracaoDemanda; data != NULL; data = data->next(0))
  {
    string *index = new string[1];
    index[0] = data->getIndex(0);
    if(foiAbertoTerminal->get(index) == 1)
    {
      for(dataStruct *data2 = data; data2 != NULL; data2 = data2->next(1))
      {
        string *index = new string[1], *index2 = new string[1];
        index[0] = data2->getIndex(0);
        index2[0] = data2->getIndex(1);
        demanda->set(index, demanda->get(index) + demanda->get(index2));
      }
    }
  }
}

double dataRepository::getCustoAbertura(string from, string to)
{
  string *index = new string[2];
  index[0] = from;
  index[1] = to;
  return custoAbertura->get(index);
}

double dataRepository::getCustoFluxo(string from, string to)
{
  string *index = new string[2];
  index[0] = from;
  index[1] = to;
  return custoFluxo->get(index);
}

double dataRepository::getFluxoMaximo(string from, string to)
{
  string *index = new string[2];
  index[0] = from;
  index[1] = to;
  return fluxoMaximo->get(index);
}
