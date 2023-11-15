import sys
import os
from PyInstaller.__main__ import run

# Specify the main Python script that will be executed
main_script = 'deployer_main.py'

# Specify the additional Python files to be included
additional_files = [
    'deployer.py',
    'database_connections.py',
    'database_config.py',
    'democode.py'
]

# Specify the non-Python files to be included
additional_files += [
    'readme.md',
    'requirement.txt',
    'config.json'
]

# Specify the dependencies
dependencies = [
    'sqlalchemy',
    'numpy',
    'pandas'
]

# Create the PyInstaller command
command = [
    main_script,
    *additional_files,
    '-y',  # Overwrite output directory without asking for confirmation
    '-F',  # Package as a single executable file
    '-n', 'deployer',  # Specify the name of the output executable
    *[f'--hidden-import={dep}' for dep in dependencies]  # Include dependencies as hidden imports
]

# Run PyInstaller
run(command)

