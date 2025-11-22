import subprocess
import re
from random import randint
path_to_exe = './nfa'
errs = []
passed = 0
def gen_nfa(reFilename, outName):
    subprocess.run(f"{path_to_exe} -f {reFilename} -re {outName}", shell=True, capture_output=True, text=True)

def test_regex(regex, tests):
    global passed
    global errs
    with open("Tests/re.test", 'w') as reFile:
        reFile.write(regex)
    gen_nfa("Tests/re.test", "Tests/re_nfa.test")
    for (testStr, testRes) in tests:
        process = subprocess.run(f'{path_to_exe} -f Tests/re_nfa.test -ds "{testStr}"', shell=True, capture_output=True, text=True)
        err = process.stderr
        out = '' if process.stdout == '' else process.stdout.split()[-1]
        if err != '': errs.append((testStr, err))
        if out == testRes: passed += 1


testRe = "(0|1*)2*0" # change to your regex
tests = ["0222220", '1111110', '011112000', '00012', '02', '', '12', '222', '0222222222200', '1220222', '0122222', '10220222'] #write your test cases

answers = ['false' if re.fullmatch(testRe, s) is None else 'true' for s in tests]

test_regex(testRe, zip(tests, answers))
print(f"tests passed: {passed} / {len(tests)}")