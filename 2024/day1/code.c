#include <stdio.h>
#include <stdlib.h>

int compar(const void *elem0, const void *elem1){
    const int *x = elem0, *y = elem1;

    if (*x > *y) return 1;
    if (*x < *y) return -1;
    return 0;
}

int main(void){
	FILE *lists;
	int list1[1000], list2[1000];
	int num1, num2;
	int lines_count = 0;
	int total_distance = 0;
	int similiarity_score = 0;

	lists = fopen("input.txt", "r");
	while(fscanf(lists, "%d   %d", &num1, &num2) != EOF){
		list1[lines_count] = num1;
		list2[lines_count] = num2;
		lines_count++;
	}
	fclose(lists);

	qsort(list1, 1000, sizeof(int), compar);
	qsort(list2, 1000, sizeof(int), compar);

	for (int i = 0; i < 1000; i++){
		int occurrence = 0;
		for (int j = 0; j < 1000; j++){
			if (list1[i] == list2[j]) occurrence++;
		}
		total_distance += abs(list1[i] - list2[i]);
		similiarity_score += list1[i] * occurrence;
	}

	printf("Total distance: %d\n", total_distance);
	printf("Similiaraity score: %d\n", similiarity_score);
}
