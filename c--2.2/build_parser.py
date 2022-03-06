import os

for filename in os.listdir('Grammar'):
    root, ext = os.path.splitext(filename)
    if ext == '.gram':
        os.system(
            f'python -m pegen Grammar\\{filename} -o {root.capitalize()}\\{root.lower()}_parser.py')
