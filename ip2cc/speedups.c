#include <Python.h>

void parse_ip(const char *raw_ip, int *parsed_ip) {
  size_t index = 0;
  while (*raw_ip) {
    if (isdigit((unsigned char)*raw_ip)) {
      parsed_ip[index] *= 10;
      parsed_ip[index] += *raw_ip - '0';
    } else {
      index++;
    }
    raw_ip++;
  }
}

static PyObject *ip2cc(PyObject *self, PyObject *args) {
  u_int32_t *data, start, value, offset = 0;
  const char *raw_ip;
  char cc[2];
  int i, ip[4] = {0};
  if (! PyArg_ParseTuple(args, "ls", &data, &raw_ip)) {
    return NULL;
  }
  parse_ip(raw_ip, ip);
  for (i = 0; i < 4; i++) {
    start = offset + ip[i];
    value = *(data + start);
    if (value == 0xffff0000) {
      PyErr_SetString(PyExc_KeyError, raw_ip);
      return NULL;
    } else if (value > 0xffff0000) {
      cc[0] = (value & 0xff00) >> 8;
      cc[1] = value & 0xff;
      return PyString_FromStringAndSize(cc, 2);
    } else {
      offset = value >> 2;
    }
  }
  PyErr_SetString(PyExc_RuntimeError, "ip2cc database is broken");
  return NULL;
}

static PyMethodDef speedupsMethods[] = {
    {"ip2cc", ip2cc, METH_VARARGS, "get country code by ip"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initspeedups(void) {
    (void) Py_InitModule("speedups", speedupsMethods);
}
