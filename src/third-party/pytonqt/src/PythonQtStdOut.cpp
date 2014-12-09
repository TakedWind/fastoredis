#include "PythonQtStdOut.h"

static PyObject *PythonQtStdOutRedirect_new(PyTypeObject *type, PyObject * /*args*/, PyObject * /*kwds*/)
{
  PythonQtStdOutRedirect *self;
  self = (PythonQtStdOutRedirect *)type->tp_alloc(type, 0);

  self->softspace = 0;
  self->_cb = NULL;

  return (PyObject *)self;
}

static PyObject *PythonQtStdOutRedirect_write(PyObject *self, PyObject *args)
{
  PythonQtStdOutRedirect*  s = (PythonQtStdOutRedirect*)self;
  if (s->_cb) {
    QString output;
    if (PyTuple_GET_SIZE(args)>=1) {
      PyObject* obj = PyTuple_GET_ITEM(args,0);
      if (PyUnicode_Check(obj)) {
#ifdef PY3K
        output = QString::fromUtf8(PyUnicode_AsUTF8(obj));
#else
        PyObject *tmp = PyUnicode_AsUTF8String(obj);
        if(tmp) {
          output = QString::fromUtf8(PyString_AS_STRING(tmp));
          Py_DECREF(tmp);
        } else {
          return NULL;
        }
#endif
      } else {
        char *string;
        if (!PyArg_ParseTuple(args, "s", &string)) {
          return NULL;
        }
        output = QString::fromLatin1(string);
      }
    }

    void* data = (*s).data;

    if (s->softspace > 0) {
      (*s->_cb)(QString(""), data);
      s->softspace = 0;
    }

    (*s->_cb)(output, data);
  }
  return Py_BuildValue("");
}

static PyObject *PythonQtStdOutRedirect_flush(PyObject * /*self*/, PyObject * /*args*/)
{
  return Py_BuildValue("");
}

static PyObject *PythonQtStdOutRedirect_isatty(PyObject * /*self*/, PyObject * /*args*/)
{
  Py_INCREF(Py_False);
  return Py_False;
}

static PyMethodDef PythonQtStdOutRedirect_methods[] = {
  {"write", (PyCFunction)PythonQtStdOutRedirect_write, METH_VARARGS,
  "redirect the writing to a callback"},
  {"flush", (PyCFunction)PythonQtStdOutRedirect_flush, METH_VARARGS,
  "flush the output, currently not implemented but needed for logging framework"
  },
  {"isatty", (PyCFunction)PythonQtStdOutRedirect_isatty,   METH_NOARGS,
  "return False since this object is not a tty-like device. Needed for logging framework"
  },
  {NULL,    NULL, 0 , NULL} /* sentinel */
};

static PyMemberDef PythonQtStdOutRedirect_members[] = {
  {const_cast<char*>("softspace"), T_INT, offsetof(PythonQtStdOutRedirect, softspace), 0,
    const_cast<char*>("soft space flag")
  },
  {NULL}  /* Sentinel */
};

PyTypeObject PythonQtStdOutRedirectType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PythonQtStdOutRedirect",             /*tp_name*/
    sizeof(PythonQtStdOutRedirect),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,           /*tp_compare*/
    0,              /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "PythonQtStdOutRedirect",           /* tp_doc */
    0,                   /* tp_traverse */
    0,                   /* tp_clear */
    0,                   /* tp_richcompare */
    0,                   /* tp_weaklistoffset */
    0,                   /* tp_iter */
    0,                   /* tp_iternext */
    PythonQtStdOutRedirect_methods,                   /* tp_methods */
    PythonQtStdOutRedirect_members,                   /* tp_members */
    0,                   /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    PythonQtStdOutRedirect_new,                 /* tp_new */
};