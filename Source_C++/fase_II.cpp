#include <fase_II.h>

fase_II::fase_II(int qtd_polo_abastecimento_terminal, int qtd_polo_suprimento,
  int qtd_polo_suprimento_terminal, dataRepository* data)
{
  environment = new GRBEnv();
  model = new GRBModel(*environment);

  this->qtd_polo_abastecimento_terminal = qtd_polo_abastecimento_terminal;
  this->qtd_polo_suprimento = qtd_polo_suprimento;
  this->qtd_polo_suprimento_terminal = qtd_polo_suprimento_terminal;

  this->data = data;

  esta_aberto_canal = model->addVars((qtd_polo_abastecimento_terminal + qtd_polo_suprimento_terminal - 1)*(qtd_polo_abastecimento_terminal + qtd_polo_suprimento_terminal + qtd_polo_suprimento), GRB_BINARY);
  model->update();
  fluxo_canal = model->addVars(
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    (qtd_polo_abastecimento_terminal + qtd_polo_suprimento_terminal - 1)*(qtd_polo_abastecimento_terminal + qtd_polo_suprimento_terminal + qtd_polo_suprimento)
  );
  model->update();
}

void fase_II::addPoloAbastecimentoTerminal(polo *poloAbastecimentoTerminal)
{
  for(vector<polo *>::iterator it = this->poloAbastecimentoTerminal.begin(); it != this->poloAbastecimentoTerminal.end(); ++it)
  {
    canal *canal_1 = new canal(poloAbastecimentoTerminal, *it);
    canal_1->setIndex(canais.size());
    canais.push_back(canal_1);

    canal *canal_2 = new canal(*it, poloAbastecimentoTerminal);
    canal_2->setIndex(canais.size());
    canais.push_back(canal_2);
  }
  for(vector<polo *>::iterator it = poloSuprimentoTerminal.begin(); it != poloSuprimentoTerminal.end(); ++it)
  {
    canal *canal_1 = new canal(poloAbastecimentoTerminal, *it);
    canal_1->setIndex(canais.size());
    canais.push_back(canal_1);

    canal *canal_2 = new canal(*it, poloAbastecimentoTerminal);
    canal_2->setIndex(canais.size());
    canais.push_back(canal_2);
  }
  poloAbastecimentoTerminal->setIndex(this->poloAbastecimentoTerminal.size());
  this->poloAbastecimentoTerminal.push_back(poloAbastecimentoTerminal);
  this->polos.push_back(poloAbastecimentoTerminal);
  this->terminal.push_back(poloAbastecimentoTerminal);
}

void fase_II::addPoloSuprimento(polo *poloSuprimento)
{
  for(vector<polo *>::iterator it = this->poloAbastecimentoTerminal.begin(); it != this->poloAbastecimentoTerminal.end(); ++it)
  {
    canal *canal_1 = new canal(poloSuprimento, *it);
    canal_1->setIndex(canais.size());
    canais.push_back(canal_1);
  }
  for(vector<polo *>::iterator it = this->poloSuprimentoTerminal.begin(); it != this->poloSuprimentoTerminal.end(); ++it)
  {
    canal *canal_1 = new canal(poloSuprimento, *it);
    canal_1->setIndex(canais.size());
    canais.push_back(canal_1);
  }
  poloSuprimento->setIndex(this->poloSuprimento.size());
  this->poloSuprimento.push_back(poloSuprimento);
  this->polos.push_back(poloSuprimento);
}

void fase_II::addPoloSuprimentoTerminal(polo *poloSuprimentoTerminal)
{
  for(vector<polo *>::iterator it = this->poloAbastecimentoTerminal.begin(); it != this->poloAbastecimentoTerminal.end(); ++it)
  {
    canal *canal_1 = new canal(poloSuprimentoTerminal, *it);
    canal_1->setIndex(canais.size());
    canais.push_back(canal_1);

    canal *canal_2 = new canal(*it, poloSuprimentoTerminal);
    canal_2->setIndex(canais.size());
    canais.push_back(canal_2);
  }
  for(vector<polo *>::iterator it = this->poloSuprimentoTerminal.begin(); it != this->poloSuprimentoTerminal.end(); ++it)
  {
    canal *canal_1 = new canal(poloSuprimentoTerminal, *it);
    canal_1->setIndex(canais.size());
    canais.push_back(canal_1);

    canal *canal_2 = new canal(*it, poloSuprimentoTerminal);
    canal_2->setIndex(canais.size());
    canais.push_back(canal_2);
  }
  poloSuprimentoTerminal->setIndex(this->poloSuprimentoTerminal.size());
  this->poloSuprimentoTerminal.push_back(poloSuprimentoTerminal);
  this->polos.push_back(poloSuprimentoTerminal);
  this->terminal.push_back(poloSuprimentoTerminal);
}

void fase_II::loadData()
{
  for(vector<canal *>::iterator it = canais.begin(); it != canais.end(); ++it)
  {
    canal *temp = *it;
    esta_aberto_canal[temp->getIndex()].set(GRB_DoubleAttr_Obj, data->getCustoAbertura(temp->getFrom()->getInstanceName(), temp->getTo()->getInstanceName()));
    fluxo_canal[temp->getIndex()].set(GRB_DoubleAttr_Obj, data->getCustoFluxo(temp->getFrom()->getInstanceName(), temp->getTo()->getInstanceName()));
  }
}

void fase_II::loadConstraints()
{
  model->update();
  for(vector<polo *>::iterator it = polos.begin(); it != polos.end(); ++it)
  {
    GRBLinExpr constraint = 0;
    for(vector<canal *>::iterator it2 = canais.begin(); it2 != canais.end(); ++it2)
    {
      if((*it2)->getFrom()->getInstanceName() == (*it)->getInstanceName())
        constraint -= fluxo_canal[(*it2)->getIndex()];
      else if((*it2)->getTo()->getInstanceName() == (*it)->getInstanceName())
        constraint += fluxo_canal[(*it2)->getIndex()];
    }
    model->addConstr(constraint, GRB_GREATER_EQUAL, data->getDemanda((*it)->getInstanceName()));
  }
  for(vector<canal *>::iterator it = canais.begin(); it != canais.end(); ++it)
  {
    GRBLinExpr constraint = esta_aberto_canal[(*it)->getIndex()]*data->getFluxoMaximo((*it)->getFrom()->getInstanceName(), (*it)->getTo()->getInstanceName());
    model->addConstr(fluxo_canal[(*it)->getIndex()], GRB_LESS_EQUAL, constraint);
  }
}

void fase_II::solve()
{
  model->set(GRB_IntAttr_ModelSense, 1);
  model->getEnv().set(GRB_IntParam_Method, GRB_METHOD_BARRIER);
  model->update();
  model->optimize();
}

void fase_II::exportData()
{
  for(vector<canal *>::iterator it = canais.begin(); it != canais.end(); ++it)
  {
    data->setFoiAbertoDuto((*it)->getFrom()->getInstanceName(), (*it)->getTo()->getInstanceName(), esta_aberto_canal[(*it)->getIndex()].get(GRB_DoubleAttr_X));
    data->setFluxoCanal((*it)->getFrom()->getInstanceName(), (*it)->getTo()->getInstanceName(), fluxo_canal[(*it)->getIndex()].get(GRB_DoubleAttr_X));
  }
}
