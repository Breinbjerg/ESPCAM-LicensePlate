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

    def __init__(self, json_config: dict):
        self.config = json_config
        self.message = b""

        self._dispatch = {
            1: self._send_config,
            2: self._adxl_data,
            3: self._mic_data,
            4: self._temp_data,
            5: self._rpm_data,
            6: self._timestamp
        }

    def connection_made(self, transport):
        self.peername = transport.get_extra_info('peername')
        self.logger = logging.getLogger(f'Client:{self.peername}')
        self.logger.debug('Connected')
        self.transport = transport

    def data_received(self, data):
        self.logger.debug(f'Recived the msg: {data}')
        self.message += data
        if self.message[-4:] == b"\x02\x03\x04\x7F":
            self.logger.debug(f'Found the end of transmission. Handling data now')
            self._handle_data()

    def eof_received(self):
        self.logger.debug('EOF received:Closing client')

    def connection_lost(self, exc) -> None:
        self.logger.debug('Connection was lost')

    def _handle_data(self):
        self.logger.debug('Handling data:')
        # See first byte for type of msg
        self._dispatch[self.message[0]]()

    def _send_config(self):
        self.logger.debug('SendConfig Called')
        # Create dict specific for the device
        for Modules in self.config["Modules"]:
            if Modules["Name"] == "ESP1":
                esp_dict = Modules
                self.transport.write(str.encode(json.dumps(esp_dict)))
            else:
                self.logger.error('Did not find any module with that name')

    def _adxl_data(self):
        self.logger.debug('ADXL data Called')
        if self.message[1] == 1:
            self.logger.debug('Receiving X-axis data')
        if self.message[1] == 2:
            self.logger.debug('Receiving Y-axis data')
        if self.message[1] == 3:
            self.logger.debug('Receiving Z-axis data')

    def _mic_data(self):
        self.logger.debug('micDataCalled')
        self.transport.write(b"OKAY")

    def _temp_data(self):
        self.logger.debug('TempData Called')
        self.transport.write(b"OKAY")

    def _rpm_data(self):
        self.logger.debug('RPMData Called')
        self.transport.write(b"OKAY")

    def _timestamp(self) -> None:
        """
        The first message from the ESP32 after sampling
        Call this function to set timestamp for sampled data
        """
