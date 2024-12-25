#include <stdio.h>
#include <stdlib.h>

#define LIST_LEN 1000

int compar(const void *elem0, const void *elem1){
    const int *x = elem0, *y = elem1;

    if (*x > *y) return 1;
    if (*x < *y) return -1;
    return 0;
}

int main(void){
	FILE *lists;
	int list1[LIST_LEN], list2[LIST_LEN];
	int num1, num2;
	int lines_count = 0;
	int total_distance = 0;
	int similiarity_score = 0;

	lists = fopen("input.txt", "r");
	if(lists == NULL){
		printf("cannot open file");
		return EXIT_FAILURE;
	}
	while(fscanf(lists, "%d   %d", &num1, &num2) != EOF){
		list1[lines_count] = num1;
		list2[lines_count] = num2;
		lines_count++;
	}
	fclose(lists);

	qsort(list1, LIST_LEN, sizeof(int), compar);
	qsort(list2, LIST_LEN, sizeof(int), compar);

	for (int i = 0; i < LIST_LEN; i++){
		int occurrence = 0;
		for (int j = 0; j < LIST_LEN; j++){
			if (list1[i] == list2[j]) occurrence++;
		}
		total_distance += abs(list1[i] - list2[i]);
		similiarity_score += list1[i] * occurrence;
	}

	printf("Total distance: %d\n", total_distance);
	printf("Similiaraity score: %d\n", similiarity_score);
}
