#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <cctype>

struct DiskSpace
{
  std::vector<long> used;
  long free = 0;
};

typedef std::unique_ptr<std::vector<DiskSpace>> DiskSpacePtr;

unsigned long long calculate_checksum(const std::vector<long> &diskSpace)
{
  unsigned long long checksum = 0;
  for (unsigned long i = 0; i < diskSpace.size(); i++)
  {
    checksum += diskSpace[i] * i;
  }
  return checksum;
}

void compress(DiskSpacePtr &diskSpace, std::vector<long> &compressed)
{
  if (diskSpace->empty())
    return;

  for (unsigned long i = 0; i < diskSpace->size() - 2; i++)
  {
    while (diskSpace->at(i).free > 0 && !diskSpace->empty())
    {
      auto &lastPartition = diskSpace->back();
      if (!lastPartition.used.empty())
      {
        diskSpace->at(i).used.push_back(lastPartition.used.front());
        lastPartition.used.erase(lastPartition.used.begin());
        diskSpace->at(i).free--;
      }
      if (lastPartition.used.empty())
      {
        diskSpace->pop_back();
        if (diskSpace->empty())
          break;
      }
    }
  }

  for (const auto &partition : *diskSpace)
  {
    for (const auto &file : partition.used)
    {
      compressed.push_back(file);
    }
  }
}

void format_input(const std::string &line, DiskSpacePtr &diskSpace)
{
  DiskSpace temp;
  for (unsigned long i = 0; i < line.size(); i += 2)
  {
    if (!std::isdigit(line[i]) || (i + 1 < line.size() && !std::isdigit(line[i + 1])))
    {
      std::cerr << "Invalid input format: " << line << "\n";
      return;
    }

    temp.used.clear();
    long usedFiles = line[i] - '0';
    for (long j = 0; j < usedFiles; j++)
    {
      temp.used.push_back(i / 2);
    }

    temp.free = (i + 1 < line.size()) ? (line[i + 1] - '0') : 0;
    diskSpace->push_back(temp);
  }
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Error: cannot open file: " << argv[1] << "\n";
    return 1;
  }

  std::string line;
  while (std::getline(input, line))
  {
    DiskSpacePtr diskSpace = std::make_unique<std::vector<DiskSpace>>();
    format_input(line, diskSpace);

    std::vector<long> compressedDiskSpace;
    compress(diskSpace, compressedDiskSpace);

    unsigned long long checksum = calculate_checksum(compressedDiskSpace);
    std::cout << "\nDisk checksum: " << checksum << "\n";
  }

  input.close();
  return 0;
}
