import re
from datetime import datetime
from collections import defaultdict
import git
import matplotlib.pyplot as plt
import matplotlib.dates as mdates


class ProblemEntries(defaultdict):
    def __init__(self):
        super().__init__(dict)

    @classmethod
    def from_repo(cls, repo):
        probs = cls()

        for commit in repo.iter_commits():
            matched = re.match('^\[(\w+)\]', commit.message)
            if matched:
                code_name = matched.group(1)
                entry = probs[code_name]
                entry.setdefault('commits', [])
                entry['commits'].append((commit.authored_datetime, commit.message))

        return probs

    def probs_by_first_date(self):
        ans = defaultdict(list)
        for code_name, entry in self.items():
            first_commit_date, _ = min(entry['commits'])
            ans[first_commit_date.date()].append(code_name)

        return ans

    def commits_per_prob(self):
        return {
            code_name: len(entry['commits'])
            for code_name, entry in self.items()
        }

    def prob_starts_per_day(self):
        return {
            date: len(codes)
            for date, codes in self.probs_by_first_date().items()
        }

    def commits_by_day(self):
        ans = defaultdict(list)
        for code_name, entry in self.items():
            for date, commit in entry['commits']:
                ans[date.date()].append(commit)

        return ans

    def commits_per_day(self):
        return {
            date: len(commits)
            for date, commits in self.commits_by_day().items()
        }

    @classmethod
    def plot_date(self, dct):
        x = list(dct.keys())
        y = list(dct.values())

        fig, ax = plt.subplots()
        ax.bar(x, y)

        ax.xaxis.set_major_locator(mdates.DayLocator())
        ax.xaxis.set_major_formatter(mdates.DateFormatter('%m.%d'))
        for tk in ax.xaxis.get_major_ticks():
            tk.label.set_rotation('vertical')
            tk.label.set_fontsize(8)

        fig.set_size_inches(16, 4)

        return fig, ax
