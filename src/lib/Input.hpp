#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>
#include <fstream>

using std::endl;
using std::cout;
using std::cerr;

class Input
{
public:
  int* weights;
  unsigned foodCount;
  bool isUniform;
  unsigned philCount;
  unsigned sumWeights;
public:
  Input();
  ~Input();
  void parse_input(const int& argc, const char** argv);

  friend std::ostream& operator<<(std::ostream& os, const Input& in)
  {
    os << "weights:";

    for (unsigned i = 0; i < in.philCount; i++)
      os << std::to_string(in.weights[i]) << ",";

    os << endl
       << "sumWeights" << in.sumWeights << endl
       << "foodCount: " << in.foodCount << endl
       << "isUniform: " << in.isUniform << endl
       << "philCount:" << in.philCount;

    return os;
  }
private:
  static void print_usage();
  static unsigned parse_int(const char* c);
};

#endif
