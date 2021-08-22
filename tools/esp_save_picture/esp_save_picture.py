"""
:Synopsis: Uses TCP-Server to receive and save the received picture on given filesystem.
:Author: Steffen Breinbjerg
:Latets update:
:Version: 0.1
"""

import asyncio
from socket import AF_INET
import os
import click


async def main(configs: dict):
    loop = asyncio.get_running_loop()
    server = await loop.create_server(
        lambda: TCPServerProtocol(json_config=configs),
        host=configs["ipaddress"], port=configs["port"], family=AF_INET)
    async with server:
        await server.serve_forever()


def test_storage_path(path: str) -> bool:
    if os.path.exists(path):
        return True
    else:
        return False


@click.command()
@click.argument('IPV4')
@click.argument('PORT')
def handle_args(ipv4, port):
    """
     ESP32-CAM: Receive and show picture. Enable option to save the picture in given path.\n
    IPV4: Address for the TCP-Server (Use Own IP)\n
    PORT: Port number for the TCP-server.
    """
    click.echo(f'IPV4={ipv4} - Port number={port}')


if __name__ == '__main__':
    handle_args()
