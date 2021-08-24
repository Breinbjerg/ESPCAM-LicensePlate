"""
:Author: Steffen Breinbjerg
:Brief: Main script for a small ESPCAM package which create a TCP-Server to receive and store images from
the ESP32-Cam.

TODO: Add Labeling functionality for ML/tinyML.
TODO: If relevant add more picture formats support.
"""
import sys
import click
from socket import AF_INET
import asyncio
from tools.TCPServer.AsyncServer import TCPServerProtocol
import logging
from tools.espPic.espPicMethods import test_storage_path


async def main(configs: dict):
    loop = asyncio.get_running_loop()
    server = await loop.create_server(
        lambda: TCPServerProtocol(configs),
        host=configs["ip"], port=configs["port"], family=AF_INET)
    async with server:
        await server.serve_forever()


@click.command()
@click.argument('IPV4', type=str, )
@click.argument('PORT', type=str)
@click.argument('PATH', type=str)
@click.option('-d', '--debug', is_flag=True, help="Show debug messages from the server")
@click.option('--save', 'save_flag', flag_value=1, default=True,
              help="Saves received images to given path (Default on)")
@click.option('--load', 'save_flag', flag_value=0,
              help="Load specific image from given path to show.")
@click.option('--bmp', 'pic_format', flag_value="bmp", default=True,
              help="Set picture format to bmp (bitmap). Used with lower quality pictures.(Default on)")
@click.option('--jpeg', 'pic_format', flag_value="jpeg",
              help="Set picture format to jpeg.")
@click.option('-f', '--filename', type=str, default='Picture',
              help="Filename for picture. If more than one i taken this script will add 1-2-3 etc."
                   "Default name is Picture.")
@click.option('-s', '--show', is_flag=True, help="Show received pictures in GUI")
def cli(ipv4, port, debug, path, save_flag, pic_format, filename, show):
    """
    Small tool which creates a TCP-server to receive images from the ESPCAM board.
    This script is mainly for TCP and saving images, but can be used to load a single image.
    3 Arguments must be given to run the script, and there is optional arguments too.\n
    IPV4: Address for the TCP-Server (Use local iP)\n
    PORT: Port number for the TCP-server (Use a high number e.g 6000).\n
    PATH: Path to either load specific image or store images received from ESPCAM. See options.
    """
    if debug:
        logging.basicConfig(level=logging.DEBUG)
    if not test_storage_path(path):
        sys.exit("Fail: Storage path")
    if save_flag:
        logging.debug("Saving images received from ESPCAM to given path")
    if not save_flag:
        logging.debug("Loading image from the given path")

    logging.debug(f'Picture format {pic_format}')
    port = int(port)
    conf_args = {
        'ip': ipv4,
        'port': int(port),
        'path': path,
        'pic_format': pic_format,
        'filename': filename,
        'showpic': show
    }
    asyncio.run(main(conf_args))


if __name__ == '__main__':
    cli()
