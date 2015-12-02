#include <dataRepository.h>
#include <iostream>
#include <fstream>

dataRepository::dataRepository()
{
  custoImplantacao = new dataStruct(1);
  demanda = new dataStruct(1);
  custoTransporte = new dataStruct(2);
  distancia = new dataStruct(2);
  foiAbertoTerminal = new dataStruct(1);
  fracaoDemanda = new dataStruct(2);

  custoAbertura = new dataStruct(2);
  custoFluxo = new dataStruct(2);
  fluxoMaximo = new dataStruct(2);
  foiAbertoDuto = new dataStruct(2);
  fluxoCanal = new dataStruct(2);
}

dataRepository::~dataRepository()
{
  delete custoImplantacao;
  delete demanda;
  delete custoTransporte;
  delete distancia;
  delete foiAbertoTerminal;
  delete fracaoDemanda;
}

void dataRepository::loadFase_I()
{
  ifstream file;

  file.open("poloAbastecimento.txt", ios::in);
  file >> a;
  poloAbastecimento = new string[a];
  for(int j = 0; j < a; j++)
  {
    string temp;
    file >> temp;
    poloAbastecimento[j] = temp;
  }
  file.close();

  file.open("poloSuprimento.txt", ios::in);
  file >> s;
  poloSuprimento = new string[s];
  for(int j = 0; j < s; j++)
  {
    string temp;
    file >> temp;
    poloSuprimento[j] = temp;
  }
  file.close();

  file.open("terminalCandidato.txt", ios::in);
  file >> t;
  terminalCandidato = new string[t];
  for(int j = 0; j < t; j++)
  {
    string temp;
    file >> temp;
    terminalCandidato[j] = temp;
  }
  file.close();

  file.open("custoImplantacao.txt", ios::in);
  for(int i = 0; i < t; i++)
  {
    double temp;
    file >> temp;
    string *index = new string[1];
    index[0] = terminalCandidato[i];
    custoImplantacao->set(index, temp);
  }
  file.close();

  file.open("demanda.txt", ios::in);
  for(int i = 0; i < a; i++)
  {
    double temp;
    file >> temp;
    string *index = new string[1];
    index[0] = poloAbastecimento[i];
    demanda->set(index, temp);
  }
  file.close();

  file.open("distancia.txt", ios::in);
  for(int i = 0; i < t; i++)
  {
    string *index = new string[2];
    string name = terminalCandidato[i];
    index[0] = name;
    for(int j = 0; j < a; j++)
    {
      string nome_2 = poloAbastecimento[j];
      index[1] = nome_2;
      double temp;
      file >> temp;
      distancia->set(index, temp);
    }
  }
  file.close();

  file.open("custoTransporte.txt", ios::in);
  for(int i = 0; i < t; i++)
  {
    string *index = new string[2];
    string name = terminalCandidato[i];
    index[0] = name;
    for(int j = 0; j < a; j++)
    {
      string nome_2 = poloAbastecimento[j];
      index[1] = nome_2;
      double temp;
      file >> temp;
      custoTransporte->set(index, temp);
    }
  }
  file.close();
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
  ifstream file;

  for(dataStruct *data = fracaoDemanda; data != NULL; data = data->next(0))
  {
    string *index = new string[1];
    index[0] = data->getIndex(0);
    if(foiAbertoTerminal->get(index) > 0)
    {
      for(dataStruct *data2 = data; data2 != NULL; data2 = data2->next(1))
      {
        string *index = new string[1], *index2 = new string[1];
        index[0] = data2->getIndex(0);
        index2[0] = data2->getIndex(1);
        demanda->set(index, (index[0].front() == 'A' ? demanda->get(index) : 0) + demanda->get(index2));
      }
    }
  }

  file.open("fluxoMaximo.txt", ios::in);
  for(int i = 0; i < t; i++)
  {
    if(terminalCandidato[i].front() == 'A')
    {
      string *index = new string[2];
      string name = terminalCandidato[i];
      index[0] = name;
      for(int j = 0; j < t; j++)
      {
        index[1] = terminalCandidato[j];
        double temp;
        file >> temp;
        fluxoMaximo->set(index, temp);
      }
    }
  }
  for(int i = 0; i < s; i++)
  {
    string *index = new string[2];
    index[0] = poloSuprimento[i];
    for(int j = 0; j < t; j++)
    {
      index[1] = terminalCandidato[j];
      double temp;
      file >> temp;
      fluxoMaximo->set(index, temp);
    }
  }
  file.close();

  file.open("custoFluxo.txt", ios::in);
  for(int i = 0; i < t; i++)
  {
    if(terminalCandidato[i].front() == 'A')
    {
      string *index = new string[2];
      string name = terminalCandidato[i];
      index[0] = name;
      for(int j = 0; j < t; j++)
      {
        index[1] = terminalCandidato[j];
        double temp;
        file >> temp;
        custoFluxo->set(index, temp);
      }
    }
  }
  for(int i = 0; i < s; i++)
  {
    string *index = new string[2];
    index[0] = poloSuprimento[i];
    for(int j = 0; j < t; j++)
    {
      index[1] = terminalCandidato[j];
      double temp;
      file >> temp;
      custoFluxo->set(index, temp);
    }
  }
  file.close();

  file.open("custoAbertura.txt", ios::in);
  for(int i = 0; i < t; i++)
  {
    if(terminalCandidato[i].front() == 'A')
    {
      string *index = new string[2];
      string name = terminalCandidato[i];
      index[0] = name;
      for(int j = 0; j < t; j++)
      {
        index[1] = terminalCandidato[j];
        double temp;
        file >> temp;
        custoAbertura->set(index, temp);
      }
    }
  }
  for(int i = 0; i < s; i++)
  {
    string *index = new string[2];
    index[0] = poloSuprimento[i];
    for(int j = 0; j < t; j++)
    {
      index[1] = terminalCandidato[j];
      double temp;
      file >> temp;
      custoAbertura->set(index, temp);
    }
  }
  file.close();
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

void dataRepository::setFoiAbertoDuto(string from, string to, double value)
{
  string *index = new string[2];
  index[0] = from;
  index[1] = to;
  foiAbertoDuto->set(index, value);
}
void dataRepository::setFluxoCanal(string from, string to, double value)
{
  string *index = new string[2];
  index[0] = from;
  index[1] = to;
  fluxoCanal->set(index, value);
}
