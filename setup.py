"""
:Author: Steffen Breinbjerg
:Brief: This script setup the packages required to run the esp pic tool.
:date: 23 August 2021
"""

from setuptools import setup, find_packages

setup(
    name='espPic',
    version='0.1.5',
    packages=find_packages(),
    include_package_data=True,
    install_requires=[
        'Click',
        'Pillow',
    ],
    entry_points={
        'console_scripts': [
            'toolEspPicture = tools.tool_script:cli'
        ]
    }
)
