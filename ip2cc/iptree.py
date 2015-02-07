# -*- coding: utf-8 -*-

import struct

from fetch import IPTree


class FullIPTree(IPTree):
    """IP tree capable of storing all IPv4 addresses"""

    def __init__(self, value_length):
        super(FullIPTree, self).__init__()
        self.value_length = value_length  # in bytes

        # Number of nodes in tree is 256**4 + 256**3 + 256**2 + 256.
        # It means we need 33 bits to address all nodes.
        # So address block size in bytes will be 5.
        self._address_block_size = 5  # in bytes

        # Each node consists of 2-bits service flag and value.
        # Value can be address offset or actual stored value (e.g city code).

        # If value_length is at most address_block_size - 1 (4 bytes), it's
        # enough to have nodes with the same size as address_block_size,
        # because we have 5 * 8 - 2 = 38 bits available (more than 4 bytes).
        # If value_length is more than address_block_size - 1  we have to
        # increase node size.

        if self.value_length <= self._address_block_size - 1:
            self._node_size = self._address_block_size
        else:
            self._node_size = self.value_length + 1

        self._tree = 256 * [None]
        self._value_flag = 0b11000000  # nodes stores actual code
        self._undefined_flag = 0b01000000  # node's content is undefined
        self._offset_flag = 0b00000000  # node stores address offset

    def write(self, fname):
        with open(fname, 'wb') as fp:
            fp.write(self.dump())

    def dump(self, offset=0, tree=None):
        if tree is None:
            tree = self._tree
        assert isinstance(tree, list)

        offsets = []
        subtrees = []
        end = offset + 256 * self._node_size
        for node in tree:
            if isinstance(node, str):  # actual value
                data = struct.pack('!B', self._value_flag) + node
                zeros = '\x00' * (self._node_size - len(data))
                assert len(data + zeros) == self._node_size
                offsets.append(data + zeros)
            elif node is None:  # undefined
                high_byte = struct.pack('!B', self._undefined_flag)
                zeros = '\x00' * (self._node_size - 1)
                assert len(high_byte + zeros) == self._node_size
                offsets.append(high_byte + zeros)
            else:  # address offset to reach next node
                high_byte = end >> 32
                low_bytes = end & 0xffffffff  # 4 bytes integer
                # merge address offset high byte with offset flag
                high_byte = self._offset_flag | high_byte
                address_block = struct.pack('!BI', high_byte, low_bytes)
                zeros = '\x00' * (self._node_size - self._address_block_size)
                assert len(address_block + zeros) == self._node_size

                offsets.append(address_block + zeros)
                subtrees.append(self.dump(end, node))
                end += len(subtrees[-1])

        assert len(''.join(offsets)) == 256 * self._node_size, \
            (len(''.join(offsets)), 256 * self._node_size)

        result = ''.join(offsets + subtrees)
        assert len(result) == end - offset
        return result

    def _get_sub_tree(self, tree, idx):
        if tree[idx] is None:
            tree[idx] = [tree[idx]]*256
        return tree[idx]
