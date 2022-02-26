import os

for filename in os.listdir('Grammar'):
    filesplit = filename.split('.')
    if filesplit[-1] == 'gram':
        parser_path = '.'.join(filesplit[:-1]).capitalize()
        os.system(
            f"python -m pegen Grammar/{filename} -o {parser_path}/{parser_path.lower()}_paresr.py")
