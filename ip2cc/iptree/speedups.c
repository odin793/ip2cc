#include <Python.h>


#define VALUE_FLAG 0b11000000
#define OFFSET_FLAG 0b00000000
#define UNDEFINED_FLAG 0b01000000


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


unsigned long long int get_offset(unsigned char *value, const unsigned char value_length) {
  unsigned long long int offset=0;
  unsigned char byte_position=0, i;
  for (i=0; i <= value_length-1; i++) {
    byte_position = (value_length - 1 - i) * 8;
    offset |= (unsigned long long int) value[i] << byte_position;
  };
  return offset;
}


static PyObject *ip2cc(PyObject *self, PyObject *args) {
  unsigned char *data, *value;
  const unsigned char value_length, node_size;
  unsigned long long int start=0, offset=0;
  const char *raw_ip;
  int i, ip[4] = {0};
  if (!PyArg_ParseTuple(args, "lsbb", &data, &raw_ip, &value_length, &node_size)) {
    return NULL;
  }

  parse_ip(raw_ip, ip);
  for (i = 0; i < 4; i++) {
    start = offset + (ip[i] * node_size);
    value = data + start;
    switch (*value) {

      case VALUE_FLAG:
        return PyString_FromStringAndSize((char *) value+1, value_length);
      break;

      case UNDEFINED_FLAG:
        PyErr_SetString(PyExc_KeyError, raw_ip);
        return NULL;
      break;

      // OFFSET_FLAG as first byte
      default:
        offset = get_offset(value, value_length);
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
