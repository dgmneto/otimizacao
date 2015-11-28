#include <dataStruct.h>

dataStruct::dataStruct(const int size)
{
  this->size = size;
  index = NULL;
  value = 0;
  next = new dataStruct *[size];
  for(int i = 0; i < size; i++)
    next[i] = NULL;
}

dataStruct::dataStruct(string *index, const int size)
{
  this->index = index;
  this->size = size;
  value = 0;
  next = new dataStruct *[size];
  for(int i = 0; i < size; i++)
    next[i] = NULL;
}

dataStruct::dataStruct(string *index, double value, const int size)
{
  this->index = index;
  this->value = value;
  this->size = size;
  next = new dataStruct *[size];
  for(int i = 0; i < size; i++)
    next[i] = NULL;
}

double dataStruct::get(string *index)
{
  for(int i = 0; i < size; i++)
  {
    if(this->index[i] == index[i]) continue;
    if(next[i] == NULL) throw 0;
    return next[i]->get(index);
  }
  return value;
}

void dataStruct::set(string *index, value)
{
  if(index != NULL)
  {
    for(int i = 0; i < size; i++)
    {
      if(this->index[i] == index[i]) continue;
      if(next[i] == NULL) next[i] = new dataStruct(index, value, size);
      else next[i]->set(index, value);
      return;
    }
  }
  else
  {
    this->index = index;
  }
  this->value = value;
}
