import re

list1 = []
list2 = []
length = 0
with open("input.txt") as lists:
    for line in lists:
        nums = re.split("\W+", line)
        list1.append(int(nums[0]))
        list2.append(int(nums[1]))
        length += 1

list1.sort()
list2.sort()

total_distance = 0
for i in range(length):
    total_distance += abs(list1[i] - list2[i])

print("Total distance: ", total_distance)

similiarity_score = 0
for num in list1:
    similiarity_score += num * list2.count(num)

print("Similiarity score: ", similiarity_score)
