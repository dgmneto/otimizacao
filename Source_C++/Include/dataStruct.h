#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <string>

using namespace std;

class dataStruct
{
public:
  dataStruct(int size);
  dataStruct(string *index, int size);
  dataStruct(string *index, double value,  int size);
  double get(string *name);
  double get();
  void set(string *name, double value);
  dataStruct *next(int dir);
  string getIndex(int i);
private:
  int size;
  string *index;
  dataStruct * *nextPointer;
  double value;
};
#endif
