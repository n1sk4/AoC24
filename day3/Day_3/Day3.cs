using System.Text.RegularExpressions;

internal class Day3
{
  private static void Main(string[] args)
  {
    string path = @"D:\AoC24\day3\input.txt";

    if (!File.Exists(path))
    {
      Console.WriteLine("file does not exist!");
      return;
    }

    string fileContent = File.ReadAllText(path);

    string multiplicationPattern = @"mul\((\d{1,3}),(\d{1,3})\)";
    string doPattern = @"do\(\)";
    string dontPattern = @"don't\(\)";
    Regex multiplicationRegex = new Regex(multiplicationPattern);
    Regex doRegex = new Regex(doPattern);
    Regex dontRegex = new Regex(dontPattern);

    SortedDictionary<int, object> map = new SortedDictionary<int, object>();
    MatchCollection matches = multiplicationRegex.Matches(fileContent);

    foreach (Match mul in matches)
    {
      if(mul.Groups.Count == 3)
      {
        int x = int.Parse(mul.Groups[1].Value);
        int y = int.Parse(mul.Groups[2].Value);

        map.Add(mul.Index, x * y);
      }
    }

    matches = doRegex.Matches(fileContent);
    foreach (Match dos in matches)
    {
      map.Add(dos.Index, "do");
    }

    matches = dontRegex.Matches(fileContent);
    foreach (Match donts in matches)
    {
      map.Add(donts.Index, "don't");
    }

    int multiplicatorResult_1 = 0;
    int multiplicatorResult_2 = 0;
    bool enableMultiplicator = true;

    foreach (var element in map)
    {
      if(element.Value?.ToString() == "do")
        enableMultiplicator = true;
      if(element.Value?.ToString() == "don't")
        enableMultiplicator = false;

      if(element.Value is int value_1)
        multiplicatorResult_1 += value_1;

      if(enableMultiplicator)
      {
        if (element.Value is int value_2)
          multiplicatorResult_2 += value_2;
      }
    }

    // PART 1 solution
    Console.WriteLine($"Multiplicator 1 result: {multiplicatorResult_1}");

    // PART 2 solution
    Console.WriteLine($"Multiplicator 2 result: {multiplicatorResult_2}");
  }
}