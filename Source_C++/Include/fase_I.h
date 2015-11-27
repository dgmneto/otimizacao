#ifndef FASE_I_H
#define FASE_I_H

#include "gurobi_c++.h"

class fase_I
{
private:
  //sets definition
  int qtd_polo_abastecimento, qtd_polo_suprimento, qtd_terminal_candidato;
  string *nome_polo_abastecimento, *nome_polo_suprimento;
  int *ref_terminal_candidato;
  //param definition
  double *custo_implantacao_terminal_candidato;
  double *demanda_polo_abastecimento;
  double *distancia_terminal_candidato_polo_abastecimento;
  double *custo_transporte_terminal_candidato_polo_abastecimento;
  //decision variables
  GRBVar *esta_aberto_terminal_candidato;
  GRBVar *fracao_demanda_terminal_candidato_polo_abastecimento;
  //Gurobi stuff
  GRBEnv *enviromment;
  GRBModel *model;
public:
  fase_I(const int qtd_polo_abastecimento, const string *nome_polo_abastecimento,
    const int qtd_polo_suprimento, const string *nome_polo_suprimento);
  void setTerminalCandidato(const int qtd_terminal_candidato, const int ref_terminal_candidato);
  void loadParamaters(
    const double *custo_implantacao_terminal_candidato,
    const double *demanda_polo_abastecimento,
    const double *distancia_terminal_candidato_polo_abastecimento,
    const double *custo_transporte_terminal_candidato_polo_abastecimento
  )
  void updateConstraints();
  void optimize();
}

#endif
