# -*- coding: utf-8 -*-

from setuptools import setup, Extension

setup(
    name='ip2cc',
    description='Lookup country country by IP address',
    version='0.6',
    author='Denis Otkidach, Viktor Kotseruba',
    author_email='denis.otkidach@gmail.com, barbuzaster@gmail.com',
    license='Python-style',
    keywords='web',
    url='https://github.com/barbuza/ip2cc',
    packages=["ip2cc"],
    zip_safe=False,
    ext_modules=[Extension('ip2cc/speedups', ['ip2cc/speedups.c'])],
    entry_points={
        "console_scripts": [
            "ip2cc_fetch = ip2cc.fetch:fetch",
            "ip2ru_fetch = ip2cc.fetch_ru:fetch"
        ]
    }
)
