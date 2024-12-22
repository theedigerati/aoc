def validate(levels, use_dampner=True):
    diff_state = 0
    dampner = None
    valid = False

    for i in range(len(levels)):
        if i + 1 == len(levels):
            valid = True
            break
        if i == dampner:
            continue
        a = int(levels[i])
        b = int(levels[i + 1])

        diff_state, valid = validate_level(diff_state, a, b)
        if valid:
            continue
        elif use_dampner is False:
            break
        elif dampner is not None:
            break
        elif i == 0:
            dampner = i
            continue
        elif i + 2 == len(levels):  # out of bounds
            dampner = i + 1
            continue

        # default recovery
        a = int(levels[i - 1])
        b = int(levels[i + 1])
        _, valid = validate_level(diff_state, a, b)
        if valid:
            dampner = i
            continue
        else:
            a = int(levels[i])
            b = int(levels[i + 2])
            _, valid = validate_level(diff_state, a, b)
            if valid:
                dampner = i + 1
                continue
            else:
                # at this point we've tried 2 dampners that failed
                break
    return valid


def validate_level(diff_state: int, a: int, b: int):
    diff = abs(a - b)
    if diff < 1 or diff > 3:
        return diff_state, False
    new_diff_state = 1 if a > b else -1
    if diff_state == 0:
        return new_diff_state, True
    elif diff_state ^ new_diff_state:
        return diff_state, False
    return diff_state, True


safe_reports = 0
safe_reports_with_dampner = 0
with open("input.txt") as reports:
    for report in reports:
        levels = report.split()

        if validate(levels, use_dampner=False):
            safe_reports += 1
        elif validate(levels):
            safe_reports_with_dampner += 1
        else:
            levels.reverse()
            if validate(levels):
                safe_reports_with_dampner += 1

print("Total safe reports: ", safe_reports)
print("Total safe reports with dampner: ", safe_reports + safe_reports_with_dampner)
