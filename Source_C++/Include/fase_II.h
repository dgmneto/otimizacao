#ifndef FASE_II_H
#define FASE_II_H

#include "gurobi_c++.h"
#include <polo.h>
#include <canal.h>
#include <dataRepository.h>
#include <vector>

class fase_II
{
public:
  //sets
  int qtd_polo_abastecimento_terminal, qtd_polo_suprimento,
    qtd_polo_abastecimento_terminal;

  vector<polo *> poloAbastecimentoTerminal;
  vector<polo *> poloSuprimento;
  vector<polo *> poloSuprimentoTerminal;

  vector<polo *> polo;
  vector<polo *> terminal;
  vector<canal *> canal;

  //decision variables
  GRBVar *esta_aberto_canal;
  GRBVar *fluxo_canal;

  //gurobi stuff
  GRBEnv *environment;
  GRBEnv *model;

  //data
  dataRepository *data;
private:
  fase_I(int qtd_polo_abastecimento_terminal, int qtd_polo_suprimento,
    dataRepository* data);
  void addPoloAbastecimentoTerminal(polo *poloAbastecimentoTerminal);
  void addPoloSuprimento(polo *poloSuprimento);
  void addPoloSuprimentoTerminal(polo *poloSuprimentoTerminal);
  void loadData();
  void loadConstraints();
  void solve();
  void exportData();
}
#endif
