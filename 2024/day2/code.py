# this approach did not work!
def validate_report_part(part, state={}, reverse=False):
    if reverse:
        part.reverse()
    diff_state = state.get("diff_state")
    diff_state = 1 - diff_state if reverse else diff_state
    dampner = state.get("dampner")
    valid = state.get("valid") or False

    for i in range(len(part)):
        if i == len(part) - 1:
            valid = True
            break
        a = int(part[i])
        b = int(part[i + 1])

        diff_state, valid = validate_level(diff_state, a, b)
        if not valid and i == 0:
            dampner = i
            continue
        if not valid and dampner is None:
            dampner = i
            a = int(part[i - 1])
            _, valid = validate_level(diff_state, a, b)
            if not valid and (i < len(part) - 2):
                dampner = i + 1
                a = int(part[i])
                b = int(part[i + 2])
                _, valid = validate_level(diff_state, a, b)
            if not valid and (i == len(part) - 2):
                continue
        if not valid:
            break
        valid = True
    state.update({"diff_state": diff_state, "dampner": dampner, "valid": valid})
    return state


def validate_report_traverse(levels):
    diff_state = None
    valid = False
    dampner = None

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
        elif dampner is not None:
            break
        elif i == 0:
            dampner = i
            continue

            # a = int(levels[i + 1])
            # b = int(levels[i + 2])
            # diff_state, valid = validate_level(None, a, b)
            # if valid:
            #     dampner = i
            #     continue
            # else:
            #     a = int(levels[i])
            #     diff_state, valid = validate_level(None, a, b)
            #     if valid:
            #         dampner = i + 1
            #         continue
            #     else:
            #         break
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


def validate_report(levels, dampner=None):
    diff_state = None
    valid = False

    for i in range(len(levels)):
        if i == len(levels) - 1:
            valid = True
            break
        if dampner == i:
            continue
        j = i + 1
        if dampner == j and j == len(levels) - 1:
            continue
        elif dampner == j:
            j += 1
        a = int(levels[i])
        b = int(levels[j])

        diff_state, valid = validate_level(diff_state, a, b)
        if not valid:
            break
    return valid


def validate_level(diff_state: int | None, a: int, b: int):
    diff = abs(a - b)
    if diff < 1 or diff > 3:
        return diff_state, False
    new_diff_state = -1 if a == b else (1 if a > b else 0)
    if diff_state is None:
        return new_diff_state, True
    elif diff_state == -1:
        return diff_state, False
    elif diff_state ^ new_diff_state:
        return diff_state, False
    return diff_state, True


safe_reports = 0
with open("input.txt") as reports:
    for report in reports:
        levels = report.split()

        # if validate_report_traverse(levels):
        #     safe_reports += 1

        if validate_report(levels):
            safe_reports += 1
        else:
            for dampner in range(len(levels)):
                if validate_report(levels, dampner):
                    safe_reports += 1
                    break


print("Total safe reports: ", safe_reports)
