"""
:Synopsis:
:Author: Steffen Breinbjerg
:Latets update:
:Version: 0.1
"""
import asyncio
import logging
from tools.espPic.espPicMethods import EspPicTool


class TCPServerProtocol(asyncio.Protocol):

    def __init__(self, config: dict):
        self.data = b""
        self.imagehandle = EspPicTool(config)

    def connection_made(self, transport):
        self.peername = transport.get_extra_info('peername')
        self.logger = logging.getLogger(f'Client:{self.peername}')
        self.logger.debug('Connected')
        self.transport = transport

    def data_received(self, data):
        # self.logger.debug(f'Received data package')
        self.data += data
        if self.data[-4:] == b"\x02\x03\x04\x7F":
            self.logger.debug(f'Found the end of transmission. Handling data now')
            self._handle_data()

    def eof_received(self):
        self.logger.debug('EOF received:Closing client')

    def connection_lost(self, exc) -> None:
        self.logger.debug('Connection was lost')

    def _handle_data(self):
        self.logger.debug('Handling data:')
        self.imagehandle.load_image_from_bytes(self.data[:-4])
        self.imagehandle.save_picture()
        self.data = b""
