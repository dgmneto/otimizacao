#ifndef DATASTRUCT_H
#define DATASTRUCT_H
class dataStruct
{
public:
  dataStruct(const int size);
  dataStruct(string *index, const int size);
  dataStruct(String *index, double value, const int size);
  double get(string *name);
  void set(string *name, double value)
private:
  const int size;
  string *index;
  dataStruct * *next;
  double value;
}
#endif
