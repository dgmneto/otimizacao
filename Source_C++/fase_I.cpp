#include <fase_I.h>
#include <sstream.h>

fase_I::fase_I(int qtd_polo_abastecimento, int qtd_polo_suprimento,
  int qtd_terminal_candidato, dataRepository* data)
{
  evironment = new GRBEnv();
  model = new GRBModel(environment);

  this->qtd_polo_abastecimento = qtd_polo_abastecimento;
  this->qtd_polo_suprimento = qtd_polo_suprimento;
  this->qtd_terminal_candidato = qtd_terminal_candidato;

  poloAbastecimento = list<poloAbastecimento *>();
  poloSuprimento = list<poloSuprimento *>();
  terminalCandidato = list<terminalCandidato *>();

  this->data = data;

  esta_aberto_terminal_candidato = model->addVars(qtd_terminal_candidato, GRB_BINARY);
  model->update();
  fracao_demanda_terminal_candidato_polo_abastecimento = new GRBVal *[qtd_terminal_candidato];
  for(int i = 0; i < qtd_terminal_candidato; i++)
    fracao_demanda_terminal_candidato_polo_abastecimento[i] = model->addVars(
      0,
      1,
      NULL,
      GRB_CONTINUOUS,
      NULL,
      qtd_polo_abastecimento
    );
    model->update();
  }
}

void fase_I::addPoloAbastecimento(polo *poloAbastecimento)
{
  poloAbastecimento->setIndex(this->poloAbastecimento.size());
  this->poloAbastecimento.push_back(poloAbastecimento);
  if(poloAbastecimento->isTerminalCandidato())
  {
    terminalCandidato *terminalCandidato = new terminalCandidato(poloAbastecimento);
    terminalCandidato->setIndex(this->terminalCandidato.size());
    this->terminalCandidato.push_back(terminalCandidato);
  }
}

void fase_I::addPoloSuprimento(polo *poloSuprimento)
{
  poloSuprimento->setIndex(this->poloSuprimento.size());
  this->poloSuprimento.push_back(poloSuprimento);
  if(poloSuprimento->isTerminalCandidato())
  {
    terminalCandidato *terminalCandidato = new terminalCandidato(poloSuprimento);
    terminalCandidato->setIndex(this->terminalCandidato.size());
    this->terminalCandidato.push_back(terminalCandidato);
  }
}

void fase_I::loadData()
{
  for(list<poloAbastecimento>::iterator it = poloAbastecimento.begin(); it != poloAbastecimento.end(); ++it)
  {
    esta_aberto_terminal_candidato[it->getIndex()].set(GRB_DoubleAttr_Obj, data->getCustoImplantacao(it->getInstanceName()));
    for(list<terminalCandidato>::iterator it2 = terminalCandidato.begn(); it2 != terminalCandidato.end(); ++it2)
    {
      fracao_demanda_terminal_candidato_polo_abastecimento[it2->getIndex()][it->getIndex()].set(GRB_DoubleAttr_Obj, data->getDemanda(it->getInstanceName())*data->getCustoTransporte(it2->getInstanceName(), it->getInstanceName())*data->getDistancia(it2->getInstanceName(), it->getInstanceName()));
    }
  }
}

void fase_I::loadConstraints()
{
  model->update();
  for(list<poloAbastecimento>::iterator it = poloAbastecimento.begin(); it != poloAbastecimento.end(); ++it)
  {
    GRBLinExpr constraint = 0;
    for(list<terminalCandidato>::iterator it2 = terminalCandidato.begin(); it2 != terminalCandidato.end(); ++it2)
    {
      constraint += fracao_demanda_terminal_candidato_polo_abastecimento[it2->getIndex()][it->getIndex()];
      GRBLinExpr constraint2 = fracao_demanda_terminal_candidato_polo_abastecimento[it2->getIndex()][it->getIndex];
      model->addConstr(constraint2, GRB_LESS_EQUAL, esta_aberto_terminal_candidato[it2->getIndex()])
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
  for(list<terminalCandidato>::iterator it = terminalCandidato.begin(); it != terminalCandidato.end(); ++it)
  {
    data->setFoiAberto(it->getInstanceName(), esta_aberto_terminal_candidato[it->getIndex()].get(GRB_DoubleAttr_X));
    for(list<poloAbastecimento>::iterator it2 = poloAbastecimento.begin(); it2 != poloAbastecimento.end(); ++it2)
    {
      data->setFracaoDemanda(it->getInstanceName(), it2->getInstanceName(), fracao_demanda_terminal_candidato_polo_abastecimento[it->getIndex()][it2->getIndex()].get(GRB_DoubleAttr_X));
    }
  }
}
