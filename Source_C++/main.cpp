#include <dataRepository.h>
#include <fase_I.h>
#include <fase_II.h>
#include <polo.h>
#include <iostream>
#include <fstream>
#include <stdio.h>

int main()
{
  try
  {
    dataRepository *data = new dataRepository();

    data->loadFase_I();

    polo * *tempPoloSuprimento;
    polo * *tempPoloAbastecimento;

    ifstream file;
    int a, s, t;

    file.open("poloAbastecimento.txt", ios::in);
    file >> a;
    tempPoloAbastecimento = new polo *[a];
    for(int j = 0; j < a; j++)
    {
      string temp;
      file >> temp;
      tempPoloAbastecimento[j] = new polo(temp, false);
    }
    file.close();

    file.open("poloSuprimento.txt", ios::in);
    file >> a;
    tempPoloSuprimento = new polo *[a];
    for(int j = 0; j < a; j++)
    {
      string temp;
      file >> temp;
      tempPoloSuprimento[j] = new polo(temp, false);
    }
    file.close();

    file.open("terminalCandidato.txt", ios::in);
    file >> t;
    for(int j = 0; j < t; j++)
    {
      string temp;
      file >> temp;
      if(temp[0] == 'A')
      {
        for(int k = 0; k < a; k++)
        {
          if(tempPoloAbastecimento[k]->getInstanceName() == temp)
          {
            tempPoloAbastecimento[k]->setTerminalCandidato(true);
          }
        }
      }
      else if(temp[0] == 'S')
      {
        for(int k = 0; k < a; k++)
        {
          if(tempPoloSuprimento[k]->getInstanceName() == temp)
          {
            tempPoloSuprimento[k]->setTerminalCandidato(true);
          }
        }
      }
    }
    file.close();

    fase_I *fase = new fase_I(a, s, t, data);

    for(int j = 0; j < a; j++)
    {
      fase->addPoloAbastecimento(tempPoloAbastecimento[j]);
    }

    for(int j = 0; j < s; j++)
    {
      fase->addPoloSuprimento(tempPoloSuprimento[j]);
    }

    fase->loadData();

    fase->loadConstraints();

    fase->solve();

    fase->tempPrint();

    fase->exportData();

    fase_II *fase2;

    data->loadFase_II();
    int at = 0, s2 = s, st = 0;
    polo *poloAbastecimentoTerminal;
    polo *poloSuprimentoTerminal;

    for(int i = 0; i < a; i++)
    {
      if(tempPoloAbastecimento[i]->isTerminalCandidato())
      {
        if(data->getFoiAbertoTerminal(tempPoloAbastecimento[i]->getInstanceName()) > 0)
        {
          at += 1;
        }
      }
    }
    for(int i = 0; i < s; i++)
    {
      if(tempPoloSuprimento[i]->isTerminalCandidato())
      {
        if(data->getFoiAbertoTerminal(tempPoloSuprimento[i]->getInstanceName()) > 0)
        {
          st += 1;
        }
      }
    }

    fase2 = new fase_II(at, s, st, data);

    for(int i = 0; i < a; i++)
    {
      if(tempPoloAbastecimento[i]->isTerminalCandidato())
      {
        if(data->getFoiAbertoTerminal(tempPoloAbastecimento[i]->getInstanceName()) > 0)
        {
          fase2->addPoloAbastecimentoTerminal(tempPoloAbastecimento[i]);
        }
      }
    }
    for(int i = 0; i < s; i++)
    {
      if(tempPoloSuprimento[i]->isTerminalCandidato())
      {
        if(data->getFoiAbertoTerminal(tempPoloSuprimento[i]->getInstanceName()) > 0)
        {
          fase2->addPoloSuprimentoTerminal(tempPoloSuprimento[i]);
        }
        else
        {
          fase2->addPoloSuprimento(tempPoloSuprimento[i]);
        }
      }
    }

    fase2->loadData();

    fase2->loadConstraints();

    fase2->solve();

    fase2->exportData();

    printf("oi\n");

    delete data;
    for(int i = 0; i < a; i++)
      delete tempPoloAbastecimento[i];
    delete[] tempPoloAbastecimento;
    for(int i = 0; i < s; i++)
      delete tempPoloSuprimento[i];
    delete[] tempPoloSuprimento;

  }
  catch(GRBException e)
  {
    return 1;
  }
  return 0;
}
