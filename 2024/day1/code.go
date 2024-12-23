package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

func main() {
	file, err := os.Open("input.txt")
	if err != nil {
		log.Fatalf("cannot open file: %s\n", err)
	}
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		fmt.Printf("Contents: %q \n", scanner.Text())
	}
	if err = scanner.Err(); err != nil {
		log.Fatalf("scanner encountered an error: %s", err)
	}
}
