#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 30
#define LEVELS_ARRAY_SIZE 8

enum ProgState {
	PROG_DECR = -1,
	PROG_NULL,
	PROG_INCR
};

struct Report {
	int *levels;
	int levels_len;
	int valid;
	enum ProgState progression;
};

void reverse_levels(struct Report *r){
	for(int i=0; i < r->levels_len; i++){
		int tail = r->levels_len - i - 1;
		if (i > tail) break;
		int temp = r->levels[i];
		r->levels[i] = r->levels[tail];
		r->levels[tail] = temp;
	}
}

void validate_level(struct Report *r, int l1, int l2){
	int diff = abs(l1 - l2);
	if (diff < 1 || diff > 3){
		r->valid = 0; return;
	}

	enum ProgState new_prog_state = PROG_NULL;
	if (l1 > l2){
		new_prog_state = PROG_INCR;
	} else {
		new_prog_state = PROG_DECR;
	}

	if (r->progression == PROG_NULL){
		r->progression = new_prog_state;
		r->valid = 1; return;
	}
	if (r->progression ^ new_prog_state){
		r->valid = 0; return;
	}
	r->valid = 1;
}

void validate(struct Report *r, int use_dampner){
	int dampner = -1;

	for (int i = 0; i < r->levels_len; i++){
		if (i + 1 == r->levels_len){
			r->valid = 1; break;
		}
		if (i == dampner) continue;

		int l1 = r->levels[i];
		int l2 = r->levels[i+1];

		validate_level(r, l1, l2);
		if (r->valid) continue;
		if (!use_dampner || dampner >= 0) break;
		if (i == 0){
			dampner = i; continue;
			// the way we deal with the first level
			// is to revalidate the entire report in reverse.
		}
		if (i+2 == r->levels_len){ // out of bounds
			dampner = i+1; continue;
		}

		// default recovery
		l1 = r->levels[i-1];
		l2 = r->levels[i+1];
		validate_level(r, l1, l2);
		if (r->valid){
			dampner = i; continue;
		}
		l1 = r->levels[i];
		l2 = r->levels[i+2];
		validate_level(r, l1, l2);
		if (r->valid){
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

		struct Report r = {.levels=levels, .levels_len=levels_len, .progression=PROG_NULL};
		validate(&r, 0);
		if (r.valid){
			safe_reports += 1; continue;
		}

		validate(&r, 1);
		if (r.valid){
			safe_reports_with_dampner += 1; continue;
		}

		reverse_levels(&r);
		validate(&r, 1);
		if (r.valid) safe_reports_with_dampner += 1;
	}

	printf("Total safe reports: %d\n", safe_reports);
	printf("Total safe reports with dampner: %d\n", safe_reports + safe_reports_with_dampner);
}
