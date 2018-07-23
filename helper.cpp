#include "helper.h"
extern int xPOS;
extern int yPOS;
extern int zPOS;

static PyObject* uarm_foo(PyObject* self, PyObject* args)
{
    printf("... in C++...: foo() method\n");
    return PyLong_FromLong(51);
}

static PyObject* uarm_show(PyObject* self, PyObject* args)
{
    PyObject *a;
    if(PyArg_UnpackTuple(args, "", 1, 1, &a))
    {
        printf("C++: show(%ld)\n", PyLong_AsLong(a));
    }

    return PyLong_FromLong(0);
}

static PyObject* uarm_add(PyObject* self,PyObject* args)
{
    PyObject *a,*b,*c,*d;
    char *s;
    float x,y,z;

    s=(char *)malloc(12);
    if(PyArg_ParseTuple(args, "sfff",&s,&x,&y,&z))
    {
        printf("C++: a %s add(%f,%f,%f)\n",s,x,y,z);
        //esfera=new Bad();
        //esfera->loader(s,GREEN,x,y,z);

    }

    return PyLong_FromLong(0);

}

static PyObject* uarm_gears(PyObject* self,PyObject* args)
{
    float x,y,z;

    if(PyArg_ParseTuple(args, "fff",&x,&y,&z))
    {
        printf("C++: uarm.gears(%f,%f,%f)\n",x,y,z);

        xPOS=x-1;
        yPOS=y-1;
        zPOS=z-1;
    }
    return PyLong_FromLong(0);
}

static struct PyMethodDef methods[] = {
    { "foo", uarm_foo, METH_VARARGS, "Returns the number"},
    { "show", uarm_show, METH_VARARGS, "Show a number" },
    { "add", uarm_add, METH_VARARGS, "Add an object" },
    { "gears", uarm_gears, METH_VARARGS, "Move the gears certain angle" },
    { NULL, NULL, 0, NULL }
};

static struct PyModuleDef modDef = {
    PyModuleDef_HEAD_INIT, "uarm", NULL, -1, methods,
    NULL, NULL, NULL, NULL
};

static PyObject* PyInit_uarm(void)
{
    return PyModule_Create(&modDef);
}
