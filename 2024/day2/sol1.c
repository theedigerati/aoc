#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 30
#define LEVELS_ARRAY_SIZE 8

void reverse_arr(int arr[], int len){
	for(int i=0; i < len; i++){
		int tail = len-i-1;
		if (i > tail) break;
		int temp = arr[i];
		arr[i] = arr[tail];
		arr[tail] = temp;
	}
}

void validate_level(int l1, int l2, int* diff_state, int* valid){
	int diff = abs(l1 - l2);
	if (diff < 1 || diff > 3){
		*valid = 0; return;
	}

	int new_diff_state;
	if (l1 > l2){
		new_diff_state = 1;
	} else {
		new_diff_state = -1;
	}

	if (*diff_state == 0){
		*diff_state = new_diff_state;
		*valid = 1; return;
	}
	if (*diff_state ^ new_diff_state){
		*valid = 0; return;
	}
	*valid = 1;
}

void validate(int levels[], int levels_len, int use_dampner, int* valid){
	int diff_state = 0;
	int dampner = -1;

	for (int i = 0; i<levels_len; i++){
		if (i + 1 == levels_len){
			*valid = 1; break;
		}
		if (i == dampner) continue;

		int l1 = levels[i];
		int l2 = levels[i+1];

		validate_level(l1, l2, &diff_state, valid);
		if (*valid) continue;
		if (!use_dampner) break;
		if (dampner >= 0 ) break;
		if (i == 0){
			dampner = i; continue;
			// the way we deal with the first level
			// is to revalidate the entire report in reverse.
		}
		if (i+2 == levels_len){ // out of bounds
			dampner = i+1; continue;
		}

		// default recovery
		l1 = levels[i-1];
		l2 = levels[i+1];
		validate_level(l1, l2, &diff_state, valid);
		if (*valid){
			dampner = i; continue;
		}
		l1 = levels[i];
		l2 = levels[i+2];
		validate_level(l1, l2, &diff_state, valid);
		if (*valid){
			dampner = i+1; continue;
		}
		break; // atp, we've tried 2 dampners that failed.

	}
}

int main(void){
	FILE *reports;
	char buffer[LINE_BUFFER_SIZE];
	int safe_reports = 0;
	int safe_reports_with_dampner = 0;

	reports = fopen("input.txt", "r");
	if (reports == NULL){
		perror("cannot open file");
		return 1;
	}

	while (fgets(buffer, sizeof(buffer), reports) != NULL) {
		buffer[strcspn(buffer, "\n")] = 0;
		int levels[LEVELS_ARRAY_SIZE];

		char *level = strtok(buffer, "  ");
		int levels_len = 0;
		while (level != NULL) {
			levels[levels_len] = strtol(level, NULL, 10);
			level = strtok(NULL, " ");
			levels_len++;
		}

		int valid = 0;
		validate(levels, levels_len, 0, &valid);
		if (valid){
			safe_reports += 1; continue;
		}

		validate(levels, levels_len, 1, &valid);
		if (valid){
			safe_reports_with_dampner += 1; continue;
		}

		reverse_arr(levels, levels_len);
		validate(levels, levels_len, 1, &valid);
		if (valid) safe_reports_with_dampner += 1;
	}

	printf("Total safe reports: %d\n", safe_reports);
	printf("Total safe reports with dampner: %d\n", safe_reports + safe_reports_with_dampner);
}
