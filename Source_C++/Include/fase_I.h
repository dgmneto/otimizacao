#ifndef FASE_I_H
#define FASE_I_H

#include "gurobi_c++.h"
#include <poloAbastecimento.h>
#include <poloSuprimento.h>
#include <terminalCandidato.h>
#include <dataRepository.h>
#include <list>

class fase_I
{
private:
  //sets definition
  int qtd_polo_abastecimento, qtd_polo_suprimento, qtd_terminal_candidato;
  list<polo *> poloAbastecimento;
  list<polo *> poloSuprimento;
  list<terminalCandidato *> terminalCandidato;

  //decision variables
  GRBVar *esta_aberto_terminal_candidato;
  GRBVar * *fracao_demanda_terminal_candidato_polo_abastecimento;

  //Gurobi stuff
  GRBEnv *environment;
  GRBModel *model;

  //data
  dataRepository* data;
public:
  fase_I(int qtd_polo_abastecimento, int qtd_polo_suprimento,
    int qtd_terminal_candidato, dataRepository* data);
  void addPoloAbastecimento(polo *poloAbastecimento);
  void addPoloSuprimento(polo *poloSuprimento);
  void loadData();
  void loadConstraints();
  void solve();
  void exportData();
}

#endif
