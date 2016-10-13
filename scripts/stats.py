import re
import functools
from collections import defaultdict
import git
import matplotlib.pyplot as plt
import matplotlib.dates as mdates


def count_lines(file):
    try:
        with open(file, 'rt', encoding='utf-8') as f:
            return len(f.readlines())
    except Exception:
        return 0


class ProblemEntriesMeta(type):
    def __new__(mcs, name, bases, namespace):
        def add_plot_attr(obj):
            class NewClass(obj.__class__):
                plot = None
            return NewClass(obj)

        def make_plotable(f, plot_type_):
            @functools.wraps(f)
            def g(self):
                res = f(self)

                res = add_plot_attr(res)
                plot_func = getattr(self, 'plot_' + plot_type_)
                res.plot = lambda: plot_func(res)

                return res

            return g

        for k, v in namespace.items():
            plot_type_ = getattr(v, '_plot_type', None)
            if plot_type_:
                v = make_plotable(v, plot_type_)
                namespace[k] = v

                namespace['plots'][k] = None

        klass = super().__new__(mcs, name, bases, namespace)
        return klass


def plot_type(type_):
    def decorator(f):
        f._plot_type = type_
        return f

    return decorator


class ProblemEntries(defaultdict, metaclass=ProblemEntriesMeta):
    def __init__(self):
        super().__init__(dict)

        for plot_name in self.plots.keys():
            self.plots[plot_name] = lambda: getattr(self, plot_name)().plot()

    @classmethod
    def from_repo(cls, repo):
        probs = cls()

        for commit in repo.iter_commits():
            matched = re.match(r'^\[(\w+)\]', commit.message)
            if matched:
                code_name = matched.group(1)
                entry = probs[code_name]
                entry.setdefault('commits', [])
                entry['commits'].append((commit.authored_datetime, commit.message))

        for code_name, entry in probs.items():
            file = 'src/{}.cpp'.format(code_name)
            entry['count_lines'] = count_lines(file)

        return probs

    plots = dict()

    @plot_type('prob')
    def lines_per_prob(self):
        pairs = [ (code_name, entry['count_lines']) for code_name, entry in self.items() ]
        pairs.sort(key=lambda pair: pair[1], reverse=True)
        return pairs

    def probs_by_first_date(self):
        ans = defaultdict(list)
        for code_name, entry in self.items():
            first_commit_date, _ = min(entry['commits'])
            ans[first_commit_date.date()].append(code_name)

        return ans

    @plot_type('prob')
    def commits_per_prob(self):
        pairs = [ (code_name, len(entry['commits'])) for code_name, entry in self.items() ]
        pairs.sort(key=lambda pair: pair[1], reverse=True)
        return pairs

    @plot_type('date')
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

    @plot_type('date')
    def commits_per_day(self):
        return {
            date: len(commits)
            for date, commits in self.commits_by_day().items()
        }

    @classmethod
    def plot_date(cls, dct):
        x = list(dct.keys())
        y = list(dct.values())

        fig, ax = plt.subplots()
        ax.barh(x, y, align='center')

        ax.yaxis.set_major_locator(mdates.DayLocator())
        ax.yaxis.set_major_formatter(mdates.DateFormatter('%m.%d'))
        for tk in ax.yaxis.get_major_ticks():
            tk.label.set_fontsize(8)

        ax.grid(axis='x', which='both')

        fig.set_size_inches(8, 16)

        return fig, ax

    @classmethod
    def plot_prob(cls, pairs):
        x = list(range(len(pairs)))
        y = [ lines for code_name, lines in pairs ]
        labels = [ code_name for code_name, lines in pairs ]

        fig, ax = plt.subplots()
        ax.barh(x, y, align='center')

        ax.set_ylim((0, len(pairs)))
        ax.set_yticks(x)
        ax.set_yticklabels(labels)

        xmax = int(ax.get_xbound()[1])
        if xmax > 100:
            xticks = list(range(0, xmax + 50, 50))
            xticks_minor = list(range(0, xmax + 50, 10))
            ax.set_xticks(xticks)
            ax.set_xticks(xticks_minor, minor=True)

        ax.grid(axis='x', which='both')

        fig.set_size_inches(8, 16)

        return fig, ax
