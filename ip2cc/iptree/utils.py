# -*- coding: utf-8 -*-

from collections import namedtuple


IPTreeConfig = namedtuple('IPTreeConfig', [
    'address_block_size',
    'value_length',
    'node_size',
    'fname',
])


def configure(value_length, fname):
    """Creates config for FullIPTree and FullIPTreeLookup classes"""

    # Number of nodes in IPv4 tree is 256**4 + 256**3 + 256**2 + 256.
    # It means we need 33 bits to address all nodes.
    # So we need 5 bytes for address_block_size.
    address_block_size = 5  # in bytes

    # Each node consists of 2-bits service flag and value.
    # Value can be address offset or actual stored value (e.g city code).

    # If value_length is at most address_block_size - 1 (4 bytes), it's
    # enough to have nodes with the same size as address_block_size,
    # because we have 5 * 8 - 2 = 38 bits available (more than 4 bytes).
    # If value_length is more than address_block_size - 1  we have to
    # increase node size.
    if value_length <= address_block_size - 1:
        node_size = address_block_size
    else:
        node_size = value_length + 1

    return IPTreeConfig(
        address_block_size=address_block_size,
        value_length=value_length,
        node_size=node_size,
        fname=fname
    )
