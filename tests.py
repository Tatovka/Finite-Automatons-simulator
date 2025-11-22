import subprocess
from random import randint
path_to_exe = './nfa'

def generate_tests_odd01(testsCount: int):
    tests: list[str] = []
    results: list[str] = []
    for i in range(testsCount):
        counts: list[int] = [0, 0]
        curstr = ''
        for _ in range(i):
            curSym = randint(0, 1)
            counts[curSym] ^= 1
            curstr += f'{curSym} '
        tests.append(('sample_nfa/odd01.txt', curstr))
        results.append('true' if (counts[0] == 1) and (counts[1] == 1) else 'false')
    return (tests, results)

def generate_tests_mult5(testsCount: int):
    tests: list[str] = []
    results: list[str] = []
    tests.append(('sample_nfa/mult5.txt', ''))
    results.append('false')
    for i in range(testsCount):
        tests.append( ('sample_nfa/mult5.txt', ' '.join(bin(i)[2:])) )
        results.append('true' if i % 5 == 0 else 'false')
    return (tests, results)

def run_nfa(filename, string):
    process = subprocess.run(f'{path_to_exe} -f {filename} -s "{string}"', shell=True, capture_output=True, text=True)
    err = process.stderr
    out = process.stdout
    if (out != ''): 
        out = out.split()[0]
    return (err, out)

def run_nfa_test(test, result):
    err, out = run_nfa(test[0], test[1])
    if (out == result and err == ''): 
        return True
    print(f"input: {test}")
    print(f"correct answer: {result}")
    if (err != ''): 
        print(f'error: {err}\nfailed')   
    print(f"output: {out}")
    return False

def random_str(alphabetSize: int):
    maxLen = 20
    length = randint(0, maxLen)
    curstr = ' '.join([str(randint(0, alphabetSize - 1)) for _ in range(length)])
    return curstr

def is_determinized(nfa: list[str]):
    startStates = nfa[2].split()
    if len(startStates) > 1: return False
    delta = dict()
    for line in nfa[4:]:
        transition = tuple(map(int, line.split()))
        if tuple(transition[:2]) in delta.keys() and delta[tuple(transition[:2])] != transition[2]: 
            return False
        delta[tuple(transition[:2])] = transition[2]
    return True

def det_test(filename: str, testsCount):
    alphabetSize: int
    with open(filename) as nfaFile:
        alphabetSize = int(nfaFile.readlines()[1])
    outputName = 'detTest'
    process = subprocess.run(f"{path_to_exe} -f {filename} -o {outputName}", shell=True, capture_output=True, text=True)
    err = process.stderr
    if (err != ''): 
        print(f'determinization algortithm finished with an error: {err}')
        return False
    else:
        with open("sample_nfa/detTest") as dfaFile:
            lines = dfaFile.readlines()
            if (int(lines[1]) != alphabetSize): 
                print("failed: alphabet sizes are differrent")
                return False
            if not is_determinized(lines):
                print("failed: result is not a dfa")
                return False
        testStrs = [random_str(alphabetSize) for _ in range(testsCount)]
        if all([run_nfa(filename, s) == run_nfa(outputName, s) for s in testStrs]): 
            return True
        else: 
            print("dfa doesn't behave as nfa")
            return False

def init_tests():
    tests, results = generate_tests_odd01(100)
    t = generate_tests_mult5(100)
    tests += t[0]
    results += t[1]
    return tests, results

tests, results = init_tests()
passedCount = 0
print("tests on running nfa")
for i in range(len(tests)):
    passedCount += run_nfa_test(tests[i], results[i])
print(f"tests passed: {passedCount}/{len(tests)}")

print("tests on determinization")
passedCount = 0
passedCount += det_test('sample_nfa/simple_nfa.txt', 50)
passedCount += det_test('sample_nfa/big_nfa.txt', 100)
print(f"tests passed: {passedCount}/2")