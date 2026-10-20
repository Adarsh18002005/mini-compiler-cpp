#!/usr/bin/env python3
import pathlib
import subprocess
import sys
import tempfile

root = pathlib.Path(__file__).resolve().parents[1]
binary = pathlib.Path(sys.argv[1]).resolve() if len(sys.argv) > 1 else root / 'build' / 'mini_compiler'

cases = [
    ('let x = 2 + 3 * 4; print x;', 0, '14\n'),
    ('print (2 + 3) * 4;', 0, '20\n'),
    ('let a = 9; let b = a - 4; print b;', 0, '5\n'),
    ('print -8 / 2;', 0, '-4\n'),
    ('print missing;', 1, 'undefined variable'),
    ('print 1 / 0;', 1, 'division by zero'),
    ('let x = 2 print x;', 1, "expected ';'"),
    ('print @;', 1, 'Lexical error'),
]
with tempfile.TemporaryDirectory() as tmp:
    path = pathlib.Path(tmp) / 'test.mini'
    for index, (source, expected_code, expected_text) in enumerate(cases, 1):
        path.write_text(source)
        result = subprocess.run([str(binary), str(path)], capture_output=True, text=True)
        output = result.stdout if expected_code == 0 else result.stderr
        assert (result.returncode == 0) == (expected_code == 0), f'Test {index} exit status: {result}'
        assert expected_text in output, f'Test {index} output: {result}'
        print(f'PASS test {index}')
print('All 8 tests passed')
