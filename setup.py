"""
:Author: Steffen Breinbjerg
:Brief: This script setup the packages required to run the esp pic tool.
:date: 23 August 2021
"""

from setuptools import setup, find_packages

setup(
    name='tools',
    version='0.1.0',
    packages=find_packages(),
    include_package_data=True,
    install_requires=[
        'Click',
    ],
    entry_points={
        'console_scripts': [
            'toolEspPicture = tools.toolEspPicture:cli'
        ]
    }
)
