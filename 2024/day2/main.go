package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
	"slices"
	"strconv"
	"strings"
)

type ProgState int

const (
	ProgDecr ProgState = iota - 1
	ProgNil
	ProgIncr
)

type Report struct {
	levels      []int
	valid       bool
	progression ProgState
}

func (r *Report) validateLevel(l1, l2 int) {
	diff := math.Abs(float64(l1 - l2))
	if diff < 1 || diff > 3 {
		r.valid = false
		return
	}

	newProgState := ProgNil
	if l1 > l2 {
		newProgState = ProgIncr
	} else {
		newProgState = ProgDecr
	}

	if r.progression == ProgNil {
		r.progression = newProgState
		r.valid = true
		return
	}

	if (r.progression ^ newProgState) != 0 {
		r.valid = false
		return
	}
	r.valid = true
}

func (r *Report) validate(useDampner bool) {
	dampner := -1
	for i, _ := range r.levels {
		if i+1 == len(r.levels) {
			r.valid = true
			break
		}
		if i == dampner {
			continue
		}
		l1 := r.levels[i]
		l2 := r.levels[i+1]

		if r.validateLevel(l1, l2); r.valid {
			continue
		}
		if !useDampner || dampner >= 0 {
			break
		}
		if i == 0 {
			dampner = i
			continue
		}
		if i+2 == len(r.levels) {
			dampner = i + 1
			continue
		}

		// default recovery
		l1 = r.levels[i-1]
		l2 = r.levels[i+1]
		if r.validateLevel(l1, l2); r.valid {
			dampner = i
			continue
		}
		l1 = r.levels[i]
		l2 = r.levels[i+2]
		if r.validateLevel(l1, l2); r.valid {
			dampner = i + 1
			continue
		}
		break
	}
}

func main() {

	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatalf("cannot open file: %s\n", err)
	}
	scanner := bufio.NewScanner(file)

	safeReports := 0
	safeReportsWithDampner := 0
	for scanner.Scan() {
		levels_str := strings.Fields(scanner.Text())
		levels_int := make([]int, len(levels_str))
		for i, s := range levels_str {
			s_int, err := strconv.Atoi(s)
			if err != nil {
				log.Fatalf("error converting str to int: %s\n", err)
			}
			levels_int[i] = s_int
		}

		report := Report{levels: levels_int, progression: ProgNil}
		if report.validate(false); report.valid {
			safeReports += 1
			continue
		}
		if report.validate(true); report.valid {
			safeReportsWithDampner += 1
			continue
		}
		slices.Reverse(report.levels)
		if report.validate(true); report.valid {
			safeReportsWithDampner += 1
		}
	}

	fmt.Printf("Total safe reports: %d\n", safeReports)
	fmt.Printf("Total safe reports with dampner: %d\n", safeReports+safeReportsWithDampner)
}
