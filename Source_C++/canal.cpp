#include <canal.h>
#include <sstream>
canal::canal(polo *from, polo *to)
{
  this->from = from;
  this->to = to;
}

int canal::getIndex()
{
  return index;
}

void canal::setIndex(int index)
{
  this->index = index;
}

polo *canal::getFrom()
{
  return from;
}

void canal::setFrom(polo *from)
{
  this->from = from;

polo *canal::getTo()
{
  return to;
}

void canal::setTo(polo *to)
{
  this->to = to;
}
