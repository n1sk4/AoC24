def read_data(input):
  reports = list()
  for line in input:
    numbers = list()
    current_number = str()

    for character in line:
      if character == " " or character == "\n":
        numbers.append(int(current_number))
        current_number = str()
        continue

      current_number += character
    reports.append(numbers)
  return reports

def check_safety_score(report):
  score = int()
  ascending = None
  for i, level in enumerate(report):
    if i == 0:
      continue

    previous_level = report[i - 1]
    current_level = report[i]

    score = current_level - previous_level
    if score < -3 or score > 3 or score == 0:
      return False
    if ascending == True and score < 0:
      return False
    if ascending == False and score > 0:
      return False
        
    if score < 0:
      ascending = False
    else: ascending = True
        
  return True

reports = read_data(open("D:\\AoC24\\day2\\input.txt"))

safe_report_1 = 0
safe_report_2 = 0

for report in reports:
  if check_safety_score(report) == True:
    safe_report_1 += 1 
    safe_report_2 += 1 
  else:
    for i, level in enumerate(report):
      report_copy = report.copy()
      report_copy.pop(i)
      if check_safety_score(report_copy) == True:
        safe_report_2 += 1
        break

# PART 1 solution
print(f"Safe reports: {safe_report_1}")

# PART 2 solution
print(f"Safe reports with Problem Dampener: {safe_report_2}")