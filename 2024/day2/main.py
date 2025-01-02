from enum import IntEnum


class ProgState(IntEnum):
    PROG_NIL = 0
    PROG_INCR = 1
    PROG_DECR = -1


class Report:
    levels = []
    valid = False
    progression = ProgState.PROG_NIL

    def __init__(self, levels) -> None:
        self.levels = [int(level) for level in levels]

    def validate(self, use_dampner=True):
        dampner = None
        levels_len = len(self.levels)

        for i in range(levels_len):
            if i + 1 == levels_len:
                self.valid = True
                break
            if i == dampner:
                continue
            l1 = self.levels[i]
            l2 = self.levels[i + 1]

            self.validate_level(l1, l2)
            if self.valid:
                continue
            elif use_dampner is False or dampner is not None:
                break
            elif i == 0:
                dampner = i
                continue
            elif i + 2 == levels_len:  # out of bounds
                dampner = i + 1
                continue

            # default recovery
            l1 = self.levels[i - 1]
            l2 = self.levels[i + 1]
            self.validate_level(l1, l2)
            if self.valid:
                dampner = i
                continue
            l1 = self.levels[i]
            l2 = self.levels[i + 2]
            self.validate_level(l1, l2)
            if self.valid:
                dampner = i + 1
                continue
            # at this point we've tried 2 dampners that failed
            break
        return self.valid

    def validate_level(self, l1: int, l2: int):
        diff = abs(l1 - l2)
        if diff < 1 or diff > 3:
            self.valid = False
            return
        new_prog_state = ProgState.PROG_INCR if l1 > l2 else ProgState.PROG_DECR
        if self.progression == ProgState.PROG_NIL:
            self.progression = new_prog_state
            self.valid = True
        elif self.progression ^ new_prog_state:
            self.valid = False
        else:
            self.valid = True


safe_reports = 0
safe_reports_with_dampner = 0
with open("input.txt") as reports:
    for line in reports:
        levels = line.split()
        report = Report(levels=levels)
        if report.validate(use_dampner=False):
            safe_reports += 1
            continue
        if report.validate():
            safe_reports_with_dampner += 1
            continue
        report.levels.reverse()
        if report.validate():
            safe_reports_with_dampner += 1

print("Total safe reports:", safe_reports)
print("Total safe reports with dampner:", safe_reports + safe_reports_with_dampner)
