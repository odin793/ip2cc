# ip2cc: Lookup country country by IP address

(c) 2002-2013 Denis S. Otkidach

## WHAT IS IT

If you want to gather web statistics by countries (not by top-level
domains) or implement targeting, here is solution: ip2cc.  This module
allows to resolve country from IP address.

## USAGE

```
pip install ip2cc
ip2cc_fetch
```

```python
import ip2cc
db = ip2cc.CountryByIP("ip2cc.db")
db["89.178.189.23"] == "RU"
```

## LICENSE

Python-style

## ACKNOWLEDGEMENTS

* Jason R. Mastaler
* Fredrik Lundh
