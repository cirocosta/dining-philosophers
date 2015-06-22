#include "Input.hpp"

Input::Input()
  : foodCount(0), isUniform(false), philCount(0), sumWeights(0)
{ }

Input::~Input()
{
  delete[] weights;
}

void Input::parse_input(const int& argc, const char** argv)
{
  if (argc < 4) {
    print_usage();
    cerr << "Minimum number of arguments not provided." << endl;
    exit(1);
  }

  std::ifstream infile(argv[1]);
  if (!infile) {
    cerr << "Couldn't open file: " << argv[1] << endl;
    exit(1);
  }

  infile >> philCount;
  weights = new int[philCount];

  for (unsigned i = 0; i < philCount; i++) {
    infile >> weights[i];
    sumWeights += weights[i];
  }

  infile.close();

  foodCount = parse_int(argv[2]);
  isUniform = argv[3][0] == 'U' || argv[3][0] == 'u' ?
    true : false;
}

unsigned Input::parse_int(const char* c)
{
  unsigned i = -1;

  try {
    i = std::stoi(c);
  } catch (std::exception const &e) {
    cerr << "InputError: " << e.what() << endl;
    exit(1);
  }

  return i;
}

void Input::print_usage()
{
  cout << "Dinning Philosophers" << endl
       << "Usage:" << endl
       << "\t $ ./dp f r u|p" << endl
       << endl
       << "Params:" << endl
       << "f\tFilename containing input in the format:"  << endl
       << "\t\tN" << endl
       << "\t\tw_1 w_2 ... w_n" << endl
       << "\twhere w_i == weight." << endl
       << endl
       << "r\tAmount of food available" << endl
       << endl
       << "u|p\tExecution mode." << endl
       << endl;
}

