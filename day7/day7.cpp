#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

struct Equation 
{
  unsigned long target = 0;
  std::vector<int> variables;
};
struct State
{
  int value;
  std::string expression;
};

typedef std::unique_ptr<std::vector<Equation>> EquationType;

void get_input(std::fstream& input, EquationType& equations);
unsigned long calculate_calibration_result(EquationType& equations);

int main([[maybe_unused]] int argc, char** argv)
{
  std::fstream input{argv[1]};
  if(!input)
  {
    std::cerr << "Error: cannot open file: " << argv[1] << "\n";
    return 1;
  }

  EquationType equations = std::make_unique<std::vector<Equation>>();

  get_input(input, equations);

  unsigned long cal_result = calculate_calibration_result(equations);

  // PART 1 solution
  std::cout << "Total calibration result: " << cal_result << "\n";

  #ifdef _WIN32
  system("pause");
  #endif 
  #ifdef linux
  std::getchar();
  #endif

  input.close();
  return 0;
}

unsigned long calculate_calibration_result(EquationType& equations)
{
  unsigned long result = 0;
  for(const Equation& eq : *equations)
  {
    std::vector<std::unordered_map<int, State>> dp(eq.variables.size());

    dp[0][eq.variables[0]] = {eq.variables[0], std::to_string(eq.variables[0])};

    for(int i = 1; i < eq.variables.size(); ++i)
    {
      for(const auto& [val, state] : dp[i - 1])
      {
        int addResult = val + eq.variables[i];
        dp[i][addResult] = {addResult, state.expression + " + " + std::to_string(eq.variables[i])};

        int mulResult = val * eq.variables[i];
        dp[i][mulResult] = {mulResult, state.expression + " * " + std::to_string(eq.variables[i])};
      }
    }

    if (dp[eq.variables.size() - 1].find(eq.target) != dp[eq.variables.size() - 1].end())
    {
      result += eq.target;
    }
  }

  return result;
}

void get_input(std::fstream& input, EquationType& equations)
{
  std::string line;
  while(std::getline(input, line))
  {
    std::string result;
    Equation eq;

    std::istringstream iss(line);
    std::getline(iss, result, ':');
    try
    {
      eq.target = std::stoul(result);
    }
    catch(const std::exception& e)
    {
      std::cerr << "Invalid number in line: " << line << " E: " << e.what() << '\n';
      continue;
    }

    std::string variables;
    std::getline(iss, variables);
    std::istringstream iis2(variables);

    int var;
    while(iis2 >> var)
    {
      eq.variables.push_back(var);
    }

    equations->push_back(eq);
  }
}