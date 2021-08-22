"""
:Synopsis: Uses TCP-Server to receive and save the received picture on given filesystem.
:Author: Steffen Breinbjerg
:Latets update:
:Version: 0.1
"""

import asyncio
from socket import AF_INET
import os





def test_storage_path(path: str) -> bool:
    if os.path.exists(path):
        return True
    else:
        return False


