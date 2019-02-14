import sys, os

def lt(x):
    tx = int(x[4:-4] if 'code' in x else x[3:-4])
    return tx

beginner = os.listdir('./testcase/1.beginner')
median = os.listdir('./testcase/2.median')
challenged = os.listdir('./testcase/3.challenged')
advanced = os.listdir('./testcase/4.advanced')

beginner.sort(key=lt)
median.sort(key=lt)
challenged.sort(key=lt)
advanced.sort(key=lt)

os.system('./do_make.sh')

print('1.beginner:')
for filename in beginner:
    if 'code' in filename:
        print('compile ', filename, ':')
        os.system('./build/pl0compiler ./testcase/1.beginner/' + filename)
        print('')

print('2.median:')
for filename in median:
    if 'code' in filename:
        print('compile ', filename, ':')
        os.system('./build/pl0compiler ./testcase/2.median/' + filename)
        print('')

print('3.challenged:')
for filename in challenged:
    if 'code' in filename:
        print('compile ', filename, ':')
        os.system('./build/pl0compiler ./testcase/3.challenged/' + filename)
        print('')

print('4.advanced:')
for filename in advanced:
    if 'code' in filename:
        print('compile ', filename, ':')
        os.system('./build/pl0compiler ./testcase/4.advanced/' + filename)
        print('')

