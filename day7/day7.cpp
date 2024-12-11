#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>

struct Equation
{
  unsigned long result = 0;
  std::vector<int> variables;
};

typedef std::unique_ptr<std::vector<Equation>> EquationType;

unsigned long concatenate(unsigned long a, unsigned long b);
bool evaluate(const std::vector<int> &variables, unsigned long target, size_t idx, unsigned long current,
              std::string expression, std::string &solution, bool isPartTwo);
void get_input(std::fstream &input, EquationType &equations);

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  std::fstream input{argv[1]};
  if (!input)
  {
    std::cerr << "Error: cannot open file: " << argv[1] << "\n";
    return 1;
  }

  EquationType equations = std::make_unique<std::vector<Equation>>();

  get_input(input, equations);

  unsigned long calibration_result_1 = 0;
  unsigned long calibration_result_2 = 0;

  for (const Equation &eq : *equations)
  {
    std::string solution;
    if (evaluate(eq.variables, eq.result, 1, eq.variables[0], std::to_string(eq.variables[0]), solution, false))
    {
      calibration_result_1 += eq.result;
    }
    if (evaluate(eq.variables, eq.result, 1, eq.variables[0], std::to_string(eq.variables[0]), solution, true))
    {
      calibration_result_2 += eq.result;
    }
  }

  // PART 1 solution
  std::cout << "Calibration result:" << calibration_result_1 << "\n";
  
  // PART 2 solution
  std::cout << "New calibration result:" << calibration_result_2 << "\n";

  #ifdef _WIN32
  system("pause");
  #endif 
  #ifdef linux
  std::getchar();
  #endif

  input.close();
  return 0;
}

unsigned long concatenate(unsigned long a, unsigned long b)
{
  std::string concated = std::to_string(a) + std::to_string(b);
  return std::stoul(concated);
}

bool evaluate(const std::vector<int> &variables, unsigned long target, size_t idx, unsigned long current,
              std::string expression, std::string &solution, bool isPartTwo)
{
  if (idx == variables.size())
  {
    if (current == target)
    {
      solution = expression;
      return true;
    }
    return false;
  }

  unsigned long next = variables[idx];

  // Try addition
  if (evaluate(variables, target, idx + 1, current + next, expression + " + " + std::to_string(next), solution, isPartTwo))
    return true;

  // Try multiplication
  if (evaluate(variables, target, idx + 1, current * next, expression + " * " + std::to_string(next), solution, isPartTwo))
    return true;

  if(isPartTwo)
  {
    // Try concatenation
    if (evaluate(variables, target, idx + 1, concatenate(current, next), expression + " || " + std::to_string(next), solution, isPartTwo))
      return true;
  }

  return false;
}

void get_input(std::fstream &input, EquationType &equations)
{
  std::string line;
  while (std::getline(input, line))
  {
    std::string result;
    Equation eq;

    std::istringstream iss(line);
    std::getline(iss, result, ':');
    try
    {
      eq.result = std::stoul(result);
    }
    catch (const std::exception& e)
    {
      std::cerr << "Invalid number in line: " << line << " E: " << e.what() << '\n';
      continue;
    }

    std::string variables;
    std::getline(iss, variables);
    std::istringstream iis2(variables);

    int var;
    while (iis2 >> var)
    {
      eq.variables.push_back(var);
    }

    equations->push_back(eq);
  }
}