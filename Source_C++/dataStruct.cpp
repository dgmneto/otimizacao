#include <dataStruct.h>

dataStruct::dataStruct(int size)
{
  this->size = size;
  index = NULL;
  value = 0;
  nextPointer = new dataStruct *[size];
  for(int i = 0; i < size; i++)
    nextPointer[i] = NULL;
}

dataStruct::dataStruct(string *index, int size)
{
  this->index = index;
  this->size = size;
  value = 0;
  nextPointer = new dataStruct *[size];
  for(int i = 0; i < size; i++)
    nextPointer[i] = NULL;
}

dataStruct::dataStruct(string *index, double value, int size)
{
  this->index = index;
  this->value = value;
  this->size = size;
  nextPointer = new dataStruct *[size];
  for(int i = 0; i < size; i++)
    nextPointer[i] = NULL;
}

double dataStruct::get(string *index)
{
  for(int i = 0; i < size; i++)
  {
    if(this->index[i] == index[i]) continue;
    if(nextPointer[i] == NULL) throw 0;
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
  if(index != NULL)
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
    this->index = index;
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
