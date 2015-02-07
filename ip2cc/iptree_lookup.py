# -*- coding: utf-8 -*-

import os
import re
import array

from speedups import ip2cc as ip2cc_speedup


is_IP = re.compile(
    '^%s$' % r'\.'.join([r'(?:(?:2[0-4]|1\d|[1-9])?\d|25[0-5])'] * 4)
).match


class FullIPTreeLookup(object):
    def __init__(self, config):
        self._value_length = config.value_length
        self._node_size = config.node_size

        fname = config.fname
        length = os.path.getsize(fname)
        self.ary = array.array(self._array_type)
        with open(fname, 'rb') as db:
            self.ary.fromfile(db, length)
            self.ary.byteswap()
        addr, _ = self.ary.buffer_info()
        self.addr = addr

    @property
    def _array_type(self):
        if array.array('B').itemsize == 1:
            return 'B'

    def __getitem__(self, ip):
        if not is_IP(ip):
            return
        return ip2cc_speedup(self.addr, ip,
                             self._value_length, self._node_size)
