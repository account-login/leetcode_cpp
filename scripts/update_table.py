#!/usr/bin/env python3


import re
from pprint import pprint


def is_fullwidth(char):
    ranges = [
        # https://en.wikipedia.org/wiki/Template:CJK_ideographs_in_Unicode
        (0x4E00, 0x9FFF),
        (0x3400, 0x4DBF),
        (0x20000, 0x2A6DF),
        (0x2A700, 0x2CEAF),
        (0x2E80, 0x2FDF),
        (0x2FF0, 0x303F),
        (0x31C0, 0x31EF),
        (0x3200, 0x33FF),
        (0xF900, 0xFAFF),
        (0xFE30, 0xFE4F),
        (0x1F200, 0x1F2FF),
        (0x2F800, 0x2FA1F),
        # https://en.wikipedia.org/wiki/Halfwidth_and_fullwidth_forms
        (0xFF01, 0xFF5E),
        (0xFFE0, 0xFFE6),
    ]
    for l, r in ranges:
        if l <= ord(char) <= r:
            return True
    else:
        return False


def display_width(s):
    '''全角为 2 ，半角为 1 。

    >>> display_width('。')
    2
    >>> display_width('啊')
    2
    >>> all(display_width(x) == 2 for x in '，。！＠＃￥％＆（）＋｛｝【】＼｜？、；～')
    True
    >>> all(display_width(chr(x)) == 1 for x in range(ord('a'), ord('z') + 1))
    True
    '''
    return sum( 2 if is_fullwidth(char) else 1 for char in s )


def ljust(s, width):
    '''补齐宽度到 width ，左对齐。

    >>> ljust('asdf', 3)
    'asdf'
    >>> ljust('asdf', 5)
    'asdf '
    >>> ljust('啊sdf', 5)
    '啊sdf'
    >>> ljust('啊sdf', 6)
    '啊sdf '
    '''
    return s + ' ' * (width - display_width(s))


def read_leetcodes():
    '''parse leetcodes from Makefile'''
    leetcodes = []
    with open('Makefile', 'rt') as f:
        started = False
        while True:
            if started:
                if line.endswith('\\'):
                    line = line[:-1].strip()
                    has_next = True
                else:
                    has_next = False

                words = [ w.strip() for w in line.split() if w.strip() ]
                leetcodes += words

                if has_next:
                    line = next(f).strip()
                else:
                    break
            else:
                line = next(f).strip()
                if line.startswith('LEETCODES'):
                    started = True
                    line = line.partition('=')[2].strip()

    return leetcodes


def make_table(leetcodes):
    '''generate table from source'''
    table = [
        # header
        [ 'Problem', 'Answer', 'Time', 'Beats' ],
    ]
    for code in leetcodes:
        src = 'src/{}.cpp'.format(code)
        url = None
        title = None

        with open(src, 'rt') as f:
            for line in f:
                # first url
                if not url:
                    res = re.match('.*(https://leetcode\.com/problems/\S+).*', line)
                    if res:
                        url = res.group(1)
                # last TEST_CASE
                res = re.match('\s*TEST_CASE\(\s*"([^"]+)"\s*\).*', line)
                if res:
                    title = res.group(1)
        assert(url)
        assert(title)
        #     [ 'Problem', 'Answer', 'Time', 'Beats' ]
        row = [ '[{title}](<{url}>)'.format_map(vars()), '[{code}](<{src}>)'.format_map(vars()), '', '' ]
        table.append(row)

    return table


def read_table_lines():
    table_lines = []
    with open('README.md', 'rt') as f:
        status = None
        start_no = None
        end_no = None
        for line_no, line in enumerate(f):
            line = line.strip()
            if status is None:
                if line.startswith('## Indexes'):
                    status = 'index'
                    continue
            if status == 'index':
                if line.startswith('|'):
                    start_no = line_no
                    status = 'table'
            if status == 'table':
                if line.startswith('|'):
                    table_lines.append(line)
                else:
                    end_no = line_no
                    break
        assert(start_no)
        if end_no is None:
            end_no = line_no + 1

    return table_lines, start_no, end_no


def parse_table(lines):
    table = []
    for line_no, line in enumerate(lines):
        # skip header seperator
        if line_no == 1:
            continue

        line = line.strip()
        assert(line[0] == '|' and line[-1] == '|')
        row = [ x.strip() for x in line.split('|') ][1:-1]
        table.append(row)

    return table


def update_table(new, orig):
    assert(len(new) >= len(orig))
    for idx in range(len(orig)):
        assert(new[idx][1] == orig[idx][1])     # answer source link
        new[idx] = new[idx][:2] + orig[idx][2:]
        #          ^ prob & ans   ^ time & beats
    return new


def format_table(table):
    assert(len(table) > 0)

    def max_col_width(i):
        return max(display_width(row[i]) for row in table)

    col_width = [ max_col_width(i) for i in range(len(table[0])) ]

    lines = []
    for row_no, row in enumerate(table):
        cells = [ ljust(row[i], col_width[i]) for i in range(len(row)) ]
        line = '| {} |'.format(' | '.join(cells))
        lines.append(line)

        if row_no == 0:
            head_seperator = '-|-'.join( '-' * w for w in col_width )
            head_seperator = '|-{}-|'.format(head_seperator)
            lines.append(head_seperator)

    return lines


def update_table_lines(lines, start_no, end_no):
    new_lines = []
    with open('README.md', 'rt') as f:
        table_inserted = False
        for line_no, line in enumerate(f):
            line = line.rstrip()
            if start_no <= line_no < end_no:
                if not table_inserted:
                    new_lines += lines
                    table_inserted = True
            else:
                new_lines.append(line)
    with open('README.md', 'wt', encoding='utf8') as f:
        f.write('\n'.join(new_lines) + '\n')


def main():
    leetcodes = read_leetcodes()
    table = make_table(leetcodes)

    origin_table_lines, start_no, end_no = read_table_lines()
    origin_table = parse_table(origin_table_lines)

    table = update_table(table, origin_table)
    lines = format_table(table)
    update_table_lines(lines, start_no, end_no)


if __name__ == '__main__':
    main()
