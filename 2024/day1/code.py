list1 = []
list2 = []
with open("input.txt") as lists:
    for line in lists:
        nums = line.split()
        list1.append(int(nums[0]))
        list2.append(int(nums[1]))

list1.sort()
list2.sort()

distance = 0
similarity = 0
for i in range(len(list1)):
    distance += abs(list1[i] - list2[i])
    similarity += list1[i] * list2.count(list1[i])

print("Total distance: ", distance)
print("Similiarity score: ", similarity)
