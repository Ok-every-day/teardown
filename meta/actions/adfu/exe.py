import logging
import argparse
import functools
from .cbw import usb_conf, ADECadfus


logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


def argparse_vendor_product(value):
    vendor, product = tuple(value.split(":"))

    return int(vendor, 16), int(product, 16)


def parse_args():
    args = argparse.ArgumentParser(description='upload and run some code')

    args.add_argument('--device', type=argparse_vendor_product, required=True, help="vendor:product of the device you want to interact with")
    args.add_argument('--binary', required=True)
    args.add_argument('--address', type=functools.partial(int, base=0), required=True)

    return args.parse_args()


if __name__ == '__main__':
    args = parse_args()

    dev, endpoint_read, endpoint_w = usb_conf(*args.device)

    logger.info(f"uploading binary '{args.binary}' at address {args.address:x} and executing it")
    # try to use the standard code from Actions
    ADECadfus(args.binary, endpoint_read, endpoint_write)
