"""
:Synopsis: Uses TCP-Server to receive and save the received picture on given filesystem.
:Author: Steffen Breinbjerg
:Latets update:
:Version: 0.1
"""
import logging
import os
from PIL import Image
import io
import sys


class EspPicTool:
    """
    EspPicTool Class.
    Basic methods to save and load pictures.
    """

    def __init__(self, path: str, pictype: str):

        self.path = path
        self.format = pictype
        self.count = 0
        self.image_bytes = b""
        self.logger = logging.getLogger("EspPicTool")

    def load_image_from_bytes(self, image_bytes):
        """
        Take a bytestream and load into a Pillow image class.
        Bytestream can be from TCP-Server or file.
        """
        self.im = Image.open(io.BytesIO(image_bytes))

    def show_image(self):
        """
        Show the loaded Pillow image class in gui.
        """
        self.im.show()

    def load_pic_from_folder(self):
        """
        Load picture from path given.
        Loaded into bytearray. Use func load_image_from_bytes to convert into Pillow image class.
        """
        try:
            with open(self.path, 'rb') as file:
                self.image_bytes = bytearray(file.read())
                self.logger.debug("Successfully loaded the image as bytes.")
        except IOError:
            sys.exit("Error: espPicMethod -> load_pic_from_folder: IOError: Could not open given path/file")

    def save_picture(self):
        """
        Saves the picture to given path.
        Load image from bytes is required to run first, else this will fail.
        """
        filename = f'{self.path}/PictureTest{self.count}'
        self.logger.debug(f'Trying to save picture {filename}')
        try:
            self.im.save(filename, self.format)
            self.count += 1
            self.logger.debug("Successfully saved the image.")
        except IOError:
            sys.exit("Error: func() save_picture espPicMethods.py -> IOError: File could not be written")
        except ValueError:
            sys.exit(
                "Error: func() save_picture espPicMethods.py ->"
                " ValueError: Output picture format could not be determined.")


def test_storage_path(path) -> bool:
    if os.path.exists(path):
        return True
    else:
        return False
