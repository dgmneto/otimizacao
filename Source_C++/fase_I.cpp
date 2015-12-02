#include <fase_I.h>
#include <sstream>

fase_I::fase_I(int qtd_polo_abastecimento, int qtd_polo_suprimento,
  int qtd_terminal_candidato, dataRepository* data)
{
  try{
    environment = new GRBEnv();
    model = new GRBModel(*environment);

    this->qtd_polo_abastecimento = qtd_polo_abastecimento;
    this->qtd_polo_suprimento = qtd_polo_suprimento;
    this->qtd_terminal_candidato = qtd_terminal_candidato;

    this->data = data;

    esta_aberto_terminal_candidato = model->addVars(qtd_terminal_candidato, GRB_BINARY);
    model->update();
    fracao_demanda_terminal_candidato_polo_abastecimento = new GRBVar *[qtd_terminal_candidato];
    for(int i = 0; i < qtd_terminal_candidato; i++)
    {
      fracao_demanda_terminal_candidato_polo_abastecimento[i] = model->addVars(
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        qtd_polo_abastecimento
      );
      model->update();
      for(int j = 0; j < qtd_polo_abastecimento; j++)
      {
        fracao_demanda_terminal_candidato_polo_abastecimento[i][j].set(GRB_DoubleAttr_UB, 1);
      }
    }
  }
  catch(GRBException e)
  {
    cout << e.getErrorCode();
    cout << e.getMessage();
    cout << "contructor";
    throw e;
  }
}

fase_I::~fase_I()
{
  delete model;
  delete environment;
  delete[] esta_aberto_terminal_candidato;
  delete[] fracao_demanda_terminal_candidato_polo_abastecimento;
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
  try
  {
    for(list<terminalCandidato *>::iterator it = terminaisCandidatos.begin(); it != terminaisCandidatos.end(); ++it)
    {
      esta_aberto_terminal_candidato[(*it)->getReference()->getIndex()].set(GRB_DoubleAttr_Obj, data->getCustoImplantacao((*it)->getReference()->getInstanceName()));
      for(list<polo *>::iterator it2 = poloAbastecimento.begin(); it2 != poloAbastecimento.end(); ++it2)
      {
        double temp = data->getDemanda((*it2)->getInstanceName())*data->getCustoTransporte((*it)->getReference()->getInstanceName(), (*it2)->getInstanceName())*data->getDistancia((*it)->getReference()->getInstanceName(), (*it2)->getInstanceName());
        fracao_demanda_terminal_candidato_polo_abastecimento[(*it)->getIndex()][(*it2)->getIndex()].set(GRB_DoubleAttr_Obj, temp);
      }
    }
  }
  catch(GRBException e)
  {
    cout << e.getErrorCode();
    cout << e.getMessage();
    cout << "loadData";
    throw e;
  }
}

void fase_I::loadConstraints()
{
  try
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
  catch(GRBException e)
  {
    cout << e.getErrorCode();
    cout << e.getMessage();
    cout << "loadConstraints";
    throw e;
  }
}

void fase_I::solve()
{
  try
  {
    model->set(GRB_IntAttr_ModelSense, 1);
    model->update();
    model->getEnv().set(GRB_IntParam_Method, GRB_METHOD_BARRIER);
    model->optimize();
  }
  catch(GRBException e)
  {
    cout << e.getErrorCode();
    cout << e.getMessage();
    cout << "solve";
    throw e;
  }
}

void fase_I::exportData()
{
  try
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
  catch(GRBException e)
  {
    cout << e.getErrorCode();
    cout << e.getMessage();
    cout << "exportData";
    throw e;
  }
}

void fase_I::tempPrint()
{
  try
  {
    for(int i = 0; i < qtd_terminal_candidato; i++)
    {
      printf("\n%lf | ", esta_aberto_terminal_candidato[i].get(GRB_DoubleAttr_X));
      for(int j = 0; j < qtd_polo_abastecimento; j++)
      {
        printf("%lf ", fracao_demanda_terminal_candidato_polo_abastecimento[i][j].get(GRB_DoubleAttr_X));
      }
    }
    printf("\n%lf\n", model->get(GRB_DoubleAttr_ObjVal));
  }
  catch(GRBException e)
  {
    cout << e.getErrorCode();
    cout << e.getMessage();
    cout << "tempPrint";
    throw e;
  }
}
