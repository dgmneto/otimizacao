#ifndef CANAL_H
#define CANAL_H

#include <polo.h>
#include <string.h>

class canal
{
public:
  canal(polo *from, polo *to);
  int getIndex();
  void setIndex(int index);
  polo *getFrom();
  void setFrom(polo *from);
  polo *getTo();
  void setTo(polo *to);
private:
  int index;
  polo *from, *to;
};
#endif
