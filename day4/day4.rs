use std::fs;

fn get_horizontal_count(lines: &Vec<String>, searched_word: &str) -> usize
{
  let mut word_count = 0;
  let searched_word_reversed: String = searched_word.chars().rev().collect();
  
  for line in lines
  {
    word_count += line.matches(searched_word).count() + line.matches(&searched_word_reversed).count();
  }
  
  word_count
}

fn get_vertical_count(lines: &Vec<String>, searched_word: &str) -> usize 
{
  let mut transposed_lines = vec![String::new(); lines[0].len()];

  for row in lines {
    for (col_index, c) in row.chars().enumerate() {
      transposed_lines[col_index].push(c);
    }
  }

  get_horizontal_count(&transposed_lines, searched_word)
}

fn get_diagonal_count(lines: &Vec<String>, searched_word: &str) -> usize
{
  let mut diagonals = Vec::new();
  let rows = lines.len();
  let cols = lines[0].len();

  for d in 0..(rows + cols - 1)
  {
    let mut diagonal = String::new();
    for row in 0..rows
    {
      let col = d as isize - row as isize;
      if col >= 0 && col < cols as isize
      {
        diagonal.push(lines[row].chars().nth(col as usize).unwrap());
      }
    }

    if !diagonal.is_empty()
    {
      diagonals.push(diagonal);
    }
  }

  for d in 0..(rows + cols - 1) 
  {
    let mut diagonal = String::new();
    for row in 0..rows 
    {
      let col = row as isize - d as isize + (cols as isize - 1);
      if col >= 0 && col < cols as isize 
      {
        diagonal.push(lines[row].chars().nth(col as usize).unwrap());
      }
    }
    if !diagonal.is_empty() 
    {
      diagonals.push(diagonal);
    }
  }

  let searched_word_reversed: String = searched_word.chars().rev().collect();
  let mut word_count = 0;
  for diagonal in diagonals.iter()
  {
    word_count += diagonal.matches(searched_word).count() + diagonal.matches(&searched_word_reversed).count();
  }

  word_count
}

fn get_pattern_count(lines: &Vec<String>, pattern: &Vec<&str>) -> usize 
{
  let rows = lines.len();
  let cols = lines[0].len();
  let pattern_height = pattern.len();
  let pattern_width = pattern[0].len();

  let mut count = 0;

  for row in 0..=(rows - pattern_height) 
  {
    for col in 0..=(cols - pattern_width) 
    {
      let mut matches_forward = true;
      for (p_row, p_line) in pattern.iter().enumerate() 
      {
        for (p_col, p_char) in p_line.chars().enumerate() 
        {
          if p_char != ' ' && lines[row + p_row].chars().nth(col + p_col).unwrap() != p_char 
          {
            matches_forward = false;
            break;
          }
        }
        if !matches_forward 
        {
            break;
        }
      }

      let mut matches_backward = true;
      for (p_row, p_line) in pattern.iter().enumerate() 
      {
        for (p_col, p_char) in p_line.chars().enumerate() 
        {
          if p_char != ' ' && lines[row + p_row].chars().nth(col + pattern_width - 1 - p_col).unwrap() != p_char 
          {
            matches_backward = false;
            break;
          }
        }
        if !matches_backward
        {
          break;
        }
      }

    if matches_forward || matches_backward 
    {
      count += 1;
    }
  }
}
  count
}

fn main()
{
  let file_path = "input.txt";
  let file_contents = fs::read_to_string(file_path);

  let lines: Vec<String> = file_contents
        .expect("Input not found!")
        .lines()
        .map(|line| line.to_string())
        .collect();

  let searched_word = "XMAS";
  let mut word_count = 0;    

  word_count += get_horizontal_count(&lines, &searched_word);
  word_count += get_vertical_count(&lines, &searched_word);
  word_count += get_diagonal_count(&lines, &searched_word);

  let search_pattern = vec![
    vec!["M S", " A ", "M S"],
    vec!["S S", " A ", "M M"],
    vec!["M M", " A ", "S S"]
  ];

  let pattern_count: usize = search_pattern.iter()
      .map(|pattern| get_pattern_count(&lines, pattern))
      .sum();

  // PART 1 solution
  println!("{} appears: {} times!", searched_word, word_count);

  
  // PART 2 solution
  println!("Pattern appears: {} times!", pattern_count);
}