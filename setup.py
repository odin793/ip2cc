# -*- coding: utf-8 -*-

from setuptools import setup, Extension

setup(
    name='ip2cc',
    description='Lookup country/city codes or other arbitrary data by IP address',
    version='0.1',
    author='Denis Otkidach, Viktor Kotseruba, Stas Kotseruba',
    author_email='denis.otkidach@gmail.com, barbuzaster@gmail.com, kotserubas@gmail.com',
    license='Python-style',
    keywords='web',
    url='https://github.com/odin793/ip2cc',
    packages=["ip2cc", "ip2cc/iptree"],
    zip_safe=False,
    ext_modules=[
        Extension('ip2cc/speedups', ['ip2cc/speedups.c']),
        Extension('ip2cc/iptree/speedups', ['ip2cc/iptree/speedups.c'])
    ],
    entry_points={
        "console_scripts": [
            "ip2cc_fetch = ip2cc.fetch:fetch",
            "ip2ru_fetch = ip2cc.fetch_ru:fetch"
        ]
    }
)
