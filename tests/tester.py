import subprocess

tests_path = "/home/matiapa/TLA/tpe/tests"
compiler_path = "/home/matiapa/TLA/tpe/bin/compiler"

tests = [
    {"name": "Hello world", "file": "test1"},
    {"name": "Expressions", "file": "test2"},
    {"name": "Conditionals", "file": "test3"}
]

for test in tests:
    print(f'> {test["name"]}')
    
    comp_out = subprocess.run([compiler_path, f'{tests_path}/programs/{test["file"]}.stc'], stdout=subprocess.PIPE)
    if comp_out.returncode != 0:
        print(f'FAILED: {comp_out.stderr}')
        continue

    prog_out = subprocess.run(["./program"], stdout=subprocess.PIPE).stdout.decode()
    eout = open(f'{tests_path}/expected_outs/{test["file"]}.out', "r").read()

    if prog_out == eout:
        print("PASSED")
    else:
        print(f'FAILED: Expected {eout}')
        print(f'FAILED: Got {prog_out}')