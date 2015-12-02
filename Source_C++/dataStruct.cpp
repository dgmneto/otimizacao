#include <dataStruct.h>

dataStruct::dataStruct(const int size)
{
  this->size = size;
  index = NULL;
  value = 0;
  nextPointer = new dataStruct *[size];
  for(int i = 0; i < size; i++)
    nextPointer[i] = NULL;
}

dataStruct::dataStruct(string *index, const int size)
{
  this->index = new string[size];
  for(int i = 0; i < size; i++)
  {
    this->index[i] = index[i];
  }
  this->size = size;
  value = 0;
  nextPointer = new dataStruct *[size];
  for(int i = 0; i < size; i++)
    nextPointer[i] = NULL;
}

dataStruct::dataStruct(string *index, double value, const int size)
{
  this->index = new string[size];
  for(int i = 0; i < size; i++)
  {
    this->index[i] = index[i];
  }
  this->value = value;
  this->size = size;
  nextPointer = new dataStruct *[size];
  for(int i = 0; i < size; i++)
    nextPointer[i] = NULL;
}

dataStruct::~dataStruct()
{
  delete[] index;
  for(int i = 0; i < size; i++)
    delete nextPointer[i];
  delete[] nextPointer;
}

double dataStruct::get(string *index)
{
  for(int i = 0; i < size; i++)
  {
    if(this->index[i] == index[i]) continue;
    return nextPointer[i]->get(index);
  }
  return value;
}

double dataStruct::get()
{
  return value;
}

void dataStruct::set(string *index, double value)
{
  if(this->index != NULL)
  {
    for(int i = 0; i < size; i++)
    {
      if(this->index[i] == index[i]) continue;
      if(nextPointer[i] == NULL) nextPointer[i] = new dataStruct(index, value, size);
      else nextPointer[i]->set(index, value);
      return;
    }
  }
  else
  {
    this->index = new string[size];
    for(int i = 0; i < size; i++)
    {
      this->index[i] = index[i];
    }
  }
  this->value = value;
}

dataStruct *dataStruct::next(int dir)
{
  return nextPointer[dir];
}

string dataStruct::getIndex(int i)
{
  return index[i];
}
