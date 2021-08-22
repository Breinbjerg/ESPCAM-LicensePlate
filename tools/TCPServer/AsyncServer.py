"""
:Synopsis:
:Author: Steffen Breinbjerg
:Latets update:
:Version: 0.1
"""
import asyncio
import logging
import datetime
import json


class TCPServerProtocol(asyncio.Protocol):

    def __init__(self):
        self.data = b""

    def connection_made(self, transport):
        self.peername = transport.get_extra_info('peername')
        self.logger = logging.getLogger(f'Client:{self.peername}')
        self.logger.debug('Connected')
        self.transport = transport

    def data_received(self, data):
        self.logger.debug(f'Recived the msg: {data}')
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
        # See first byte for type of msg

    def _timestamp(self) -> None:
        """
        The first message from the ESP32 after sampling
        Call this function to set timestamp for sampled data
        """
