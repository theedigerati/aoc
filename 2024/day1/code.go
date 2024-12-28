package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"slices"
	"strconv"
	"strings"
)

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatalf("cannot open file: %s\n", err)
	}
	scanner := bufio.NewScanner(file)

	list1 := make([]int, 0, 1000)
	list2 := make([]int, 0, 1000)
	for scanner.Scan() {
		items := strings.Fields(scanner.Text())
		if i, err := strconv.Atoi(items[0]); err == nil {
			list1 = append(list1, i)
		}
		if i, err := strconv.Atoi(items[1]); err == nil {
			list2 = append(list2, i)
		}
		if err != nil {
			log.Fatalf("error converting item to int: %s\n", err)
		}
	}
	if err = scanner.Err(); err != nil {
		log.Fatalf("scanner encountered an error: %s", err)
	}

	slices.Sort(list1)
	slices.Sort(list2)

	distance, similarity := 0, 0
	for i, v1 := range list1 {
		occurrence := 0
		for _, v2 := range list2 {
			if v1 == v2 {
				occurrence++
			}
		}
		if d := v1 - list2[i]; d > 0 {
			distance += d
		} else {
			distance -= d
		}
		similarity += v1 * occurrence
	}

	fmt.Printf("Total distance: %d\n", distance)
	fmt.Printf("Similarity score: %d\n", similarity)

}
