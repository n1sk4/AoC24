package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"slices"
)

type position struct {
	row, col int
}

func main() {
	grid, start := loadGrid("D:\\AoC24\\day6\\input.txt")
	visited, solution1 := calculatePartOne(grid, start)
	solution2 := calculatePartTwo(grid, start, visited)

	// PART 1 solution
	fmt.Println("Part 1:", solution1)

	// PART 2 solution
	fmt.Println("Part 2:", solution2)
}

func loadGrid(filePath string) ([][]rune, position) {
	file, err := os.Open(filePath)
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	var grid [][]rune
	var start position

	for scanner.Scan() {
		row := []rune(scanner.Text())
		grid = append(grid, row)
		if col := slices.Index(row, '^'); col != -1 {
			start = position{row: len(grid) - 1, col: col}
			grid[start.row][start.col] = '.'
		}
	}

	return grid, start
}

func calculatePartOne(grid [][]rune, start position) ([]position, int) {
	directions := []position{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}
	rows, cols := len(grid), len(grid[0])

	visited := map[position]bool{}
	current := start
	directionIdx := 0

	for withinBounds(current, rows, cols) {
		if grid[current.row][current.col] == '#' {
			current.row -= directions[directionIdx].row
			current.col -= directions[directionIdx].col
			directionIdx = (directionIdx + 1) % len(directions)
			continue
		}

		visited[current] = true
		grid[current.row][current.col] = 'X'
		current.row += directions[directionIdx].row
		current.col += directions[directionIdx].col
	}

	visitedPositions := make([]position, 0, len(visited))
	for pos := range visited {
		visitedPositions = append(visitedPositions, pos)
	}

	return visitedPositions, len(visitedPositions)
}

func calculatePartTwo(grid [][]rune, start position, visitedPositions []position) int {
	cycles := 0

	for _, pos := range visitedPositions {
		if pos == start || grid[pos.row][pos.col] == '#' {
			continue
		}

		grid[pos.row][pos.col] = '#'
		if detectsCycle(grid, start) {
			cycles++
		}
		grid[pos.row][pos.col] = '.'
	}

	return cycles
}

func detectsCycle(grid [][]rune, start position) bool {
	directions := []position{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}
	rows, cols := len(grid), len(grid[0])

	visited := map[position]position{}
	current := start
	directionIdx := 0

	for withinBounds(current, rows, cols) {
		if visited[current] == directions[directionIdx] {
			return true
		}

		visited[current] = directions[directionIdx]

		if grid[current.row][current.col] == '#' {
			current.row -= directions[directionIdx].row
			current.col -= directions[directionIdx].col
			directionIdx = (directionIdx + 1) % len(directions)
			continue
		}

		grid[current.row][current.col] = 'X'
		current.row += directions[directionIdx].row
		current.col += directions[directionIdx].col
	}

	return false
}

func withinBounds(pos position, rows, cols int) bool {
	return pos.row >= 0 && pos.col >= 0 && pos.row < rows && pos.col < cols
}
