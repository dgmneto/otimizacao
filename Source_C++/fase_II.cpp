#include <fase_II.h>

fase_II::fase_II(int qtd_polo_abastecimento_terminal, int qtd_polo_suprimento,
  int qtd_polo_suprimento_terminal, dataRepository* data)
{
  environment = new GRBEnv();
  model new GRBModel(environment);

  this->qtd_polo_abastecimento_terminal = qtd_polo_abastecimento_terminal;
  this->qtd_polo_suprimento = qtd_polo_suprimento;
  this->qtd_polo_suprimento_terminal = qtd_polo_suprimento_terminal;

  this->data = data;

  poloAbastecimentoTerminal = vector<polo *>();
  poloSuprimento = vector<polo *>();
  poloSuprimentoTerminal = vector<polo *>();

  polo = vector<polo *>();
  terminal = vector<polo *>();
  canais = vector<polo *>();

  esta_aberto_canal = model->addVars((qtd_polo_abastecimento_terminal + qtd_polo_suprimento_terminal - 1)*(qtd_polo_abastecimento_terminal + qtd_polo_suprimento_terminal + qtd_polo_suprimento), GRB_BINARY);
  model->udpadte();
  fluxo_canal = model->addVars(
    0,
    NULL,
    NULL,
    GRB_CONTINUOUS,
    NULL,
    (qtd_polo_abastecimento_terminal + qtd_polo_suprimento_terminal - 1)*(qtd_polo_abastecimento_terminal + qtd_polo_suprimento_terminal + qtd_polo_suprimento)
  );
  model->update();
}

void fase_II::addPoloAbastecimentoTerminal(polo *poloAbastecimentoTerminal)
{
  for(vector<polo *>::iterator it = poloAbastecimentoTerminal.begin(); it != poloAbastecimentoTerminal.end(); ++it)
  {
    Canal *canal_1 = new Canal(poloAbastecimentoTerminal, &(*it));
    canal_1->setIndex(canal.size());
    canal.push_back(canal_1);

    Canal *canal_2 = new Canal(&(*it), poloAbastecimentoTerminal);
    canal_2->setIndex(canal.size());
    canal.push_back(canal_2);
  }
  for(vector<polo *>::iterator it = poloSuprimentoTerminal.begin(); it != poloSuprimentoTerminal.end(); ++it)
  {
    Canal *canal_1 = new Canal(poloAbastecimentoTerminal, &(*it));
    canal_1->setIndex(canal.size());
    canal.push_back(canal_1);

    Canal *canal_2 = new Canal(&(*it), poloAbastecimentoTerminal);
    canal_2->setIndex(canal.size());
    canal.push_back(canal_2);
  }
  poloAbastecimentoTerminal->setIndex(this->poloAbastecimentoTerminal.size());
  this->poloAbastecimento.push_back(poloAbastecimentoTerminal);
  this->polo.push_back(poloAbastecimentoTerminal);
  this->terminal.push_back(poloAbastecimentoTerminal);
}

void fase_II::addPoloSuprimento(polo *poloSuprimento)
{
  for(vector<polo *>::iterator it = poloAbastecimentoTerminal.begin(); it != poloAbastecimentoTerminal.end(); ++it)
  {
    Canal *canal_1 = new Canal(poloSuprimento, &(*it));
    canal_1->setIndex(canal.size());
    canal.push_back(canal_1);
  }
  for(vector<polo *>::iterator it = poloSuprimentoTerminal.begin(); it != poloSuprimentoTerminal.end(); ++it)
  {
    Canal *canal_1 = new Canal(poloSuprimento, &(*it));
    canal_1->setIndex(canal.size());
    canal.push_back(canal_1);
  }
  poloSuprimento->setIndex(this->poloSuprimento.size());
  this->poloSuprimento.push_back(poloSuprimento);
  this->polo.push_back(poloSuprimento);
}

void fase_II::addPoloSuprimentoTerminal(polo *poloSuprimentoTerminal)
{
  for(vector<polo *>::iterator it = poloAbastecimentoTerminal.begin(); it != poloAbastecimentoTerminal.end(); ++it)
  {
    Canal *canal_1 = new Canal(poloSuprimentoTerminal, *it);
    canal_1->setIndex(canal.size());
    canal.push_back(canal_1);

    Canal *canal_2 = new Canal(*it, poloSuprimentoTerminal);
    canal_2->setIndex(canal.size());
    canal.push_back(canal_2);
  }
  for(vector<polo *>::iterator it = poloSuprimentoTerminal.begin(); it != poloSuprimentoTerminal.end(); ++it)
  {
    Canal *canal_1 = new Canal(poloSuprimentoTerminal, &(*it));
    canal_1->setIndex(canal.size());
    canal.push_back(canal_1);

    Canal *canal_2 = new Canal(&(*it), poloSuprimentoTerminal);
    canal_2->setIndex(canal.size());
    canal.push_back(canal_2);
  }
  poloSuprimentoTerminal->setIndex(this->poloSuprimentoTerminal.size());
  this->poloSuprimentoTerminal.push_back(poloSuprimentoTerminal);
  this->polo.push_back(poloSuprimentoTerminal);
  this->terminal.push_back(poloSuprimentoTerminal);
}

void fase_II::loadData()
{
  for(vector<canal *>::iterator it = canal.begin(); it != canal.end(); ++it)
  {
    canal *temp = *it;
    esta_aberto_canal[temp->getIndex()].set(GRB_DoubleAttr_Obj, data->getCustoAbertura(temp->getFrom()->getInstanceName(), temp->getTo()->getInstanceName()));
    fluxo_canal[temp->getIndex()].set(GRB_DoubleAttr_Obj, data->getCustoFluxo(temp->getFrom()->getInstanceName(), temp->getTo()->getInstanceName()));
  }
}

void fase_II::loadConstraints()
{
  model->update();
  for(vector<polo *>::iterator it = polo.begin(); it != polo.end(); ++it)
  {
    GRBLinExpr constraint = 0
    for(vector<canal *>::iterator it2 = canal.begin(); it2 != canal.end(); ++it2)
    {
      if((*it2)->getFrom()->getInstanceName() == (*it)->getInstanceName())
        constraint -= fluxo_canal[(*it2)->getIndex()];
      else if((*it2)->getFrom()->getInstanceName() == (*it)->getInstanceName())
        constraint += fluxo_canal[(*it2)->getIndex()];
    }
    model->addConstr(constraint, GRB_GREATER_EQUAL, data->getDemanda((*it)->getInstanceName()));
  }
  for(vector<canal *>::iterator it = canal.begin(); it != canal.end(); ++it)
  {
    model->addConstr(fluxo_canal[it->getIndex()], GRB_LESS_EQUAL, data->getFluxoMaximo((*it)->getFrom()->getInstanceName(), (*it)->getTo()->getInstanceName()));
  }
}

void fase_II::solve()
{
  model->set(GRB_IntAttr_ModelSense, 1);
  model->update();
  model->getEnv().set(GRB_IntParam_Method, GRB_METHOD_BARRIER);
  model->optimize();
}

void exportData()
{
  for(vector<canal *>::iterator it = canal.begin(); it != canal.end(); ++it)
  {
    data.setFoiAbertoDuto((*it)->getFrom()->getInstanceName(), (*it)->getTo()->getInstanceName(), esta_aberto_canal[(*it)->getIndex()]);
    data.setFluxoCanal((*it)->getFrom()->getInstanceName(), (*it)->getTo()->getInstanceName(), fluxo_canal[(*it)->getIndex()]);
  }
}
