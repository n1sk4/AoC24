import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;


public class day5 {
  public static void main(String[] args) throws IOException
  {
    final File parent = new File("D:\\Aoc24\\day5");
    final File file = new File(parent, "input.txt");

    final Set<String> rules = new LinkedHashSet<>();
    final List<String> updates = new ArrayList<>();

    final List<String> lines = Files.readAllLines(Path.of(file.getPath()));

    for (final String line : lines)
    {
      if (line.contains("|"))
      {
        rules.add(line.trim());
      }
      else if (line.matches("\\d+(,\\d+)*")) 
      {
        updates.add(line.trim());
      }
    }

    final int part1 = day5.solvePart1(rules, updates);
    System.out.println("part1 = " + part1);

    final int part2 = day5.solvePart2(rules, updates);
    System.out.println("part2 = " + part2);
  }

  private static int solvePart2(final Set<String> rules, final List<String> updates) {
    int sumOfMiddlePages = 0;

    for (final String update : updates)
    {
      final List<String> pages = Arrays.asList(update.split(","));

      if (!isValidOrder(pages, rules))
      {
        final List<String> orderedPages = pages.stream().sorted((o1, o2) -> 
        {
          if (rules.contains(o1 + "|" + o2))
          {
            return -1;
          }
          else if (rules.contains(o2 + "|" + o1))
          {
            return 1;
          }
          else 
          {
            return 0;
          }
        }).toList();

        final int middlePosition = orderedPages.size() / 2;
        sumOfMiddlePages += Integer.parseInt( orderedPages.get(middlePosition) );
      }
    }

    return sumOfMiddlePages;
  }


  private static int solvePart1(final Set<String> rules, final List<String> updates)
  {
    int sumOfMiddlePages = 0;

    for (final String update : updates) {
      final List<String> pages = Arrays.asList(update.split(","));

      if (isValidOrder(pages, rules))
      {
        final int middlePosition = pages.size() / 2;
        final int middlePage = Integer.parseInt(pages.get(middlePosition));
        sumOfMiddlePages += middlePage;
      }
    }

    return sumOfMiddlePages;
  }

  private static boolean isValidOrder(final List<String> pages, final Set<String> rules)
  {
    if (pages.size() <= 1)
    {
      return true;
    }

    for (int i = 0; i < pages.size(); i++)
    {
      final String page = pages.get(i);
      final List<String> beforePages = pages.subList(0, i);
      final List<String> afterPages = pages.subList(i + 1, pages.size());

      if ( !isBeforeAndPageValid(page, beforePages, rules) || !isAfterAndPageValid(page, afterPages, rules))
      {
        return false;
      }
    }
    return true;
  }

  private static boolean isBeforeAndPageValid(final String page, final List<String> beforePages, final Set<String> rules)
  {
    for (final String beforePage : beforePages)
    {
      final String requiredRule = beforePage + "|" + page;
      if (!rules.contains(requiredRule))
      {
        return false;
      }
    }
    return true;
  }

  private static boolean isAfterAndPageValid(final String page, final List<String> afterPages, final Set<String> rules)
  {
    for (final String afterPage : afterPages) 
    {
      final String requiredRule = page + "|" + afterPage;
      if (!rules.contains(requiredRule)) 
      {
        return false;
      }
    }
    return true;
  }
}
