"""



"""

import click
from socket import AF_INET
import asyncio
from tools.TCPServer.AsyncServer import TCPServerProtocol


async def main(configs: dict):
    loop = asyncio.get_running_loop()
    server = await loop.create_server(
        lambda: TCPServerProtocol(),
        host=configs["ip"], port=configs["port"], family=AF_INET)
    async with server:
        await server.serve_forever()


@click.command()
@click.argument('IPV4', type=str)
@click.argument('PORT', type=str)
def handle_args(ipv4, port):
    """
     ESP32-CAM: Receive and show picture. Enable option to save the picture in given path.\n
    IPV4: Address for the TCP-Server (Use Own IP)\n
    PORT: Port number for the TCP-server.
    """
    port = int(port)
    conf_args = {
        'ip': ipv4,
        'port': int(port)
    }
    print(f'IPV4={conf_args["ip"]} - Port number= {conf_args["port"]}')
    asyncio.run(main(conf_args))


if __name__ == '__main__':
    handle_args()

