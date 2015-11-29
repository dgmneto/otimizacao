#include <fase_I.h>
#include <sstream>

fase_I::fase_I(int qtd_polo_abastecimento, int qtd_polo_suprimento,
  int qtd_terminal_candidato, dataRepository* data)
{
  environment = new GRBEnv();
  model = new GRBModel(*environment);

  this->qtd_polo_abastecimento = qtd_polo_abastecimento;
  this->qtd_polo_suprimento = qtd_polo_suprimento;
  this->qtd_terminal_candidato = qtd_terminal_candidato;

  this->data = data;

  esta_aberto_terminal_candidato = model->addVars(qtd_terminal_candidato, GRB_BINARY);
  model->update();
  fracao_demanda_terminal_candidato_polo_abastecimento = new GRBVar *[qtd_terminal_candidato];
  const char temp = GRB_CONTINUOUS;
  for(int i = 0; i < qtd_terminal_candidato; i++)
  {
    fracao_demanda_terminal_candidato_polo_abastecimento[i] = model->addVars(
      NULL,
      NULL,
      NULL,
      &temp,
      NULL,
      qtd_polo_abastecimento
    );
    for(int j = 0; j < qtd_polo_abastecimento; j++)
    {
      fracao_demanda_terminal_candidato_polo_abastecimento[i][j].set(GRB_DoubleAttr_UB, 1);
    }
    model->update();
  }
}

void fase_I::addPoloAbastecimento(polo *poloAbastecimento)
{
  poloAbastecimento->setIndex(this->poloAbastecimento.size());
  this->poloAbastecimento.push_back(poloAbastecimento);
  if(poloAbastecimento->isTerminalCandidato())
  {
    terminalCandidato *temp = new terminalCandidato(poloAbastecimento);
    temp->setIndex(terminaisCandidatos.size());
    terminaisCandidatos.push_back(temp);
  }
}

void fase_I::addPoloSuprimento(polo *poloSuprimento)
{
  poloSuprimento->setIndex(this->poloSuprimento.size());
  this->poloSuprimento.push_back(poloSuprimento);
  if(poloSuprimento->isTerminalCandidato())
  {
    terminalCandidato *temp = new terminalCandidato(poloSuprimento);
    temp->setIndex(terminaisCandidatos.size());
    terminaisCandidatos.push_back(temp);
  }
}

void fase_I::loadData()
{
  for(list<terminalCandidato *>::iterator it = terminaisCandidatos.begin(); it != terminaisCandidatos.end(); ++it)
  {
    esta_aberto_terminal_candidato[(*it)->getReference()->getIndex()].set(GRB_DoubleAttr_Obj, data->getCustoImplantacao((*it)->getReference()->getInstanceName()));
    for(list<polo *>::iterator it2 = poloAbastecimento.begin(); it2 != poloAbastecimento.end(); ++it2)
    {
      fracao_demanda_terminal_candidato_polo_abastecimento[(*it)->getIndex()][(*it2)->getIndex()].set(GRB_DoubleAttr_Obj, data->getDemanda((*it)->getReference()->getInstanceName())*data->getCustoTransporte((*it)->getReference()->getInstanceName(), (*it2)->getInstanceName())*data->getDistancia((*it)->getReference()->getInstanceName(), (*it2)->getInstanceName()));
    }
  }
}

void fase_I::loadConstraints()
{
  model->update();
  for(list<polo *>::iterator it = poloAbastecimento.begin(); it != poloAbastecimento.end(); ++it)
  {
    GRBLinExpr constraint = 0;
    for(list<terminalCandidato *>::iterator it2 = terminaisCandidatos.begin(); it2 != terminaisCandidatos.end(); ++it2)
    {
      constraint += fracao_demanda_terminal_candidato_polo_abastecimento[(*it2)->getIndex()][(*it)->getIndex()];
      GRBLinExpr constraint2 = fracao_demanda_terminal_candidato_polo_abastecimento[(*it2)->getIndex()][(*it)->getIndex()];
      model->addConstr(constraint2, GRB_LESS_EQUAL, esta_aberto_terminal_candidato[(*it2)->getIndex()]);
    }
    model->addConstr(constraint, GRB_EQUAL, 1);
  }
}

void fase_I::solve()
{
  model->set(GRB_IntAttr_ModelSense, 1);
  model->update();
  model->getEnv().set(GRB_IntParam_Method, GRB_METHOD_BARRIER);
  model->optimize();
}

void fase_I::exportData()
{
  for(list<terminalCandidato *>::iterator it = terminaisCandidatos.begin(); it != terminaisCandidatos.end(); ++it)
  {
    data->setFoiAbertoTerminal((*it)->getReference()->getInstanceName(), esta_aberto_terminal_candidato[(*it)->getIndex()].get(GRB_DoubleAttr_X));
    for(list<polo *>::iterator it2 = poloAbastecimento.begin(); it2 != poloAbastecimento.end(); ++it2)
    {
      data->setFracaoDemanda((*it)->getReference()->getInstanceName(), (*it2)->getInstanceName(), fracao_demanda_terminal_candidato_polo_abastecimento[(*it)->getIndex()][(*it2)->getIndex()].get(GRB_DoubleAttr_X));
    }
  }
}
