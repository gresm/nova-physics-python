#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include "nova_docs.h"
#include "novaphysics/novaphysics.h"



/**
 * Vector2 object interface
 */
typedef struct {
    PyObject_HEAD
    nv_Vector2 vec
} nv_Vector2Object;

/**
 * Create new Vector2 object
 */
nv_Vector2Object *nv_Vector2Object_new(double x, double y);

/**
 * Vector2 Python object to Vector2 struct
 */
#define PY_TO_VEC2(o) o->vec

/**
 * Body object interface
 */
typedef struct {
    PyObject_HEAD
    nv_Body *body;
    nv_BodyType type;
    int shape;
    nv_Vector2Object *position;
    double angle;
    double radius;
} nv_BodyObject;

/**
 * Space object interface
 */
typedef struct {
    PyObject_HEAD
    nv_Space *space;
    nv_Array *body_objects;
} nv_SpaceObject;



static void nv_Vector2Object_dealloc(nv_Vector2Object *self) {
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int nv_Vector2Object_init(
    nv_Vector2Object *self,
    PyObject *args,
    PyObject *kwds
) {
    double x;
    double y;

    if (!PyArg_ParseTuple(args, "dd", &x, &y))
        return -1;

    self->vec = NV_VEC2(x, y);

    return 0;
}

static PyObject *nv_Vector2Object___add__(
    nv_Vector2Object *self,
    nv_Vector2Object *vector
);

static PyObject *nv_Vector2Object___sub__(
    nv_Vector2Object *self,
    nv_Vector2Object *vector
);

static PyObject *nv_Vector2Object___mul__(
    nv_Vector2Object *self,
    PyObject *scalar
);

static PyObject *nv_Vector2Object___truediv__(
    nv_Vector2Object *self,
    PyObject *scalar
);


/**
 * Vector2 object method interface
 */
static PyMethodDef nv_Vector2Object_methods[] = {
    {
        "__add__",
        (PyCFunction)nv_Vector2Object___add__, METH_VARARGS,
        ""
    },

    {NULL} // Sentinel
};

static PyObject *nv_Vector2Object___add__(
    nv_Vector2Object *self,
    nv_Vector2Object *vector
) {
    nv_Vector2 result = nv_Vector2_add(PY_TO_VEC2(self), PY_TO_VEC2(vector));

    return nv_Vector2Object_new(result.x, result.y);
}

static PyObject *nv_Vector2Object___sub__(
    nv_Vector2Object *self,
    nv_Vector2Object *vector
) {
    nv_Vector2 result = nv_Vector2_sub(PY_TO_VEC2(self), PY_TO_VEC2(vector));

    return nv_Vector2Object_new(result.x, result.y);
}

static PyObject *nv_Vector2Object___mul__(
    nv_Vector2Object *self,
    PyObject *scalar
) {
    double s = PyFloat_AS_DOUBLE(scalar);

    nv_Vector2 result = nv_Vector2_muls(PY_TO_VEC2(self), s);

    return nv_Vector2Object_new(result.x, result.y);
}

static PyObject *nv_Vector2Object___truediv__(
    nv_Vector2Object *self,
    PyObject *scalar
) {
    double s = PyFloat_AS_DOUBLE(scalar);

    nv_Vector2 result = nv_Vector2_divs(PY_TO_VEC2(self), s);

    return nv_Vector2Object_new(result.x, result.y);
}

/**
 * Vector2 object operator overloadings
 */
static PyNumberMethods nv_Vector2Object_operators = {
    .nb_add =         (binaryfunc)nv_Vector2Object___add__,
    .nb_subtract =    (binaryfunc)nv_Vector2Object___sub__,
    .nb_multiply =    (binaryfunc)nv_Vector2Object___mul__,
    .nb_true_divide = (binaryfunc)nv_Vector2Object___truediv__
};

/**
 * Vector2 getters interface
 */
static PyObject *nv_Vector2Object_get_x(nv_Vector2Object *self, void *closure) {
    return PyFloat_FromDouble(self->vec.x);
}

static PyObject * nv_Vector2Object_set_x(nv_Vector2Object *self, PyObject *value, void *closure) {
    if (value == NULL) {
        PyErr_SetString(PyExc_AttributeError, "can't delete attribute");
        return -1;
    }

    self->vec.x = (nv_float)PyFloat_AsDouble(value);

    return 0;
}

static PyObject *nv_Vector2Object_get_y(nv_Vector2Object *self, void *closure) {
    return PyFloat_FromDouble(self->vec.y);
}

static PyObject * nv_Vector2Object_set_y(nv_Vector2Object *self, PyObject *value, void *closure) {
    if (value == NULL) {
        PyErr_SetString(PyExc_AttributeError, "can't delete attribute");
        return -1;
    }

    self->vec.y = (nv_float)PyFloat_AsDouble(value);

    return 0;
}


static PyGetSetDef nv_Vector2Object_getset[] = {
    {"x", (getter)nv_Vector2Object_get_x, (setter)nv_Vector2Object_set_x, VECTOR_OBJECT_X},
    {"y", (getter)nv_Vector2Object_get_y, (setter)nv_Vector2Object_set_y, VECTOR_OBJECT_Y},
    {NULL, 0, NULL, NULL, NULL} /* Sentinel */
};

/**
 * Vector2 object type internals
 */
PyTypeObject nv_Vector2ObjectType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "nova.Vector2",
    .tp_doc = "Vector2 object",
    .tp_basicsize = sizeof(nv_Vector2Object),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor)nv_Vector2Object_dealloc,
    .tp_init = (initproc)nv_Vector2Object_init,
    .tp_as_number = &nv_Vector2Object_operators,
    .tp_getset = nv_Vector2Object_getset,
};

nv_Vector2Object *nv_Vector2Object_new(double x, double y) {
    PyObject *args = Py_BuildValue("dd", x, y);
    nv_Vector2Object *obj = (nv_Vector2Object *)PyObject_CallObject((PyObject *)&nv_Vector2ObjectType, args);
    Py_DECREF(args);
    Py_INCREF(obj);
    return obj;
}

nv_Vector2Object *nv_Vector2_to_Vector2Object(nv_Vector2 vec) {
    return nv_Vector2Object_new(vec.x, vec.y);
}



static void nv_BodyObject_dealloc(nv_BodyObject *self) {
    // Don't free nv_Body instance because space frees it
    Py_XDECREF(self->position);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int nv_BodyObject_init(
    nv_BodyObject *self,
    PyObject *args,
    PyObject *kwds
) {
    nv_BodyType type;
    int shape;
    double x;
    double y;
    double angle;
    double restitution;
    double friction;
    double radius;
    PyObject *vertices = NULL;

    if (!PyArg_ParseTuple(
        args, "iidddddd|O",
        &type, &shape, &x, &y, &angle, &restitution, &friction, &radius, &vertices
    ))
        return -1;

    self->type = type;
    self->shape = shape;
    self->position = (nv_Vector2Object *)nv_Vector2Object_new(x, y);
    self->angle = angle;
    self->radius = radius;

    nv_Array *new_vertices = NULL;

    // Validate polygon vertices
    if (vertices) {
        if (!PySequence_Check(vertices)) {
            PyErr_SetString(PyExc_TypeError, "Vertices must be a sequence of number pairs");
            return 0;
        }

        size_t vertices_len = PySequence_Length(vertices);

        if (vertices_len < 3) {
            PyErr_SetString(PyExc_ValueError, "Polygon vertices must be at least length of 3");
            return 0;
        }

        // Create nv_Array from vertices sequence
        new_vertices = nv_Array_new();
        PyObject *v;
        PyObject *vx;
        PyObject *vy;

        for (size_t i = 0; i < vertices_len; i++) {
            v = PySequence_GetItem(vertices, i);
            vx = PySequence_GetItem(v, 0);
            vy = PySequence_GetItem(v, 1);

            nv_Array_add(new_vertices, NV_VEC2_NEW(PyFloat_AS_DOUBLE(vx), PyFloat_AS_DOUBLE(vy)));

            Py_DECREF(v);
            Py_DECREF(vx);
            Py_DECREF(vy);
        }
    }

    if (shape == 0) {
        self->body = nv_Circle_new(
            type,
            NV_VEC2(x, y),
            angle,
            (nv_Material){1.0, restitution, friction},
            radius
        );
    }

    else if (shape == 1) {
        self->body = nv_Polygon_new(
            type,
            NV_VEC2(x, y),
            angle,
            (nv_Material){1.0, restitution, friction},
            new_vertices
        );
    }

    return 0;
}

/**
 * Body object member interface
 */
static PyMemberDef nv_BodyObject_members[] = {
    {
        "type",
        T_INT, offsetof(nv_BodyObject, type), 0,
        "Type of the body"
    },

    {
        "shape",
        T_INT, offsetof(nv_BodyObject, shape), 0,
        "Shape of the body"
    },

    {
        "position",
        T_OBJECT_EX, offsetof(nv_BodyObject, position), 0,
        "Position of the body"
    },

    {
        "angle",
        T_DOUBLE, offsetof(nv_BodyObject, angle), 0,
        "Angle"
    },

    {
        "radius",
        T_DOUBLE, offsetof(nv_BodyObject, radius), 0,
        "Radius"
    },

    {NULL} // Sentinel
};

static PyObject *nv_BodyObject_get_vertices(
    nv_BodyObject *self,
    PyObject *Py_UNUSED(ignored)
) {
    nv_Polygon_model_to_world(self->body);
    PyObject *return_tup = PyTuple_New(self->body->shape->trans_vertices->size);

    PyObject *vertex_tup;

    for (size_t i = 0; i < self->body->shape->trans_vertices->size; i++) {
        nv_Vector2 v = NV_TO_VEC2(self->body->shape->trans_vertices->data[i]);

        PyTuple_SET_ITEM(return_tup, i, (PyObject *)nv_Vector2Object_new(v.x, v.y));
    }

    return return_tup;
}

static PyObject *nv_BodyObject_apply_force(
    nv_BodyObject *self,
    PyObject *args
) {
    nv_Vector2Object *force;

    if (!PyArg_ParseTuple(args, "O!", &nv_Vector2ObjectType, &force))
        return NULL;

    nv_Body_apply_force(self->body, force->vec);

    Py_RETURN_NONE;
}

/**
 * Body object method interface
 */
static PyMethodDef nv_BodyObject_methods[] = {
    {
        "get_vertices",
        (PyCFunction)nv_BodyObject_get_vertices, METH_NOARGS,
        "Get the transformed vertices"
    },

    {
        "apply_force",
        (PyCFunction)nv_BodyObject_apply_force, METH_VARARGS,
        "Apply force at body center of mass"
    },

    {NULL} // Sentinel
};

PyTypeObject nv_BodyObjectType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "nova.Body",
    .tp_doc = "Body object",
    .tp_basicsize = sizeof(nv_BodyObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor)nv_BodyObject_dealloc,
    .tp_init = (initproc)nv_BodyObject_init,
    .tp_methods = nv_BodyObject_methods,
    .tp_members = nv_BodyObject_members
};


PyObject *nv_create_circle(PyObject *self, PyObject *args) {
    nv_BodyType type;
    double x;
    double y;
    double angle;
    double restitution;
    double friction;
    double radius;

    if (!PyArg_ParseTuple(
        args, "idddddd",
        &type, &x, &y, &angle, &restitution, &friction, &radius
    ))
        return NULL;

    PyObject *inst_args = Py_BuildValue("iidddddd", type, 0, x, y, angle, restitution, friction, radius);
    nv_BodyObject *obj = (nv_BodyObject *)PyObject_CallObject((PyObject *)&nv_BodyObjectType, inst_args);
    Py_DECREF(inst_args);
    return (PyObject *)obj;
}

PyObject *nv_create_rect(PyObject *self, PyObject *args) {
    nv_BodyType type;
    double x;
    double y;
    double angle;
    double restitution;
    double friction;
    double width;
    double height;

    if (!PyArg_ParseTuple(
        args, "iddddddd",
        &type, &x, &y, &angle, &restitution, &friction, &width, &height
    ))
        return NULL;

    double w = width / 2.0;
    double h = height / 2.0;

    PyObject *inst_args = Py_BuildValue("iidddddd((dd)(dd)(dd)(dd))", type, 1, x, y, angle, restitution, friction, 0.0,
        -w, -h, w, -h, w, h, -w, h);

    nv_BodyObject *obj = (nv_BodyObject *)PyObject_CallObject((PyObject *)&nv_BodyObjectType, inst_args);
    Py_DECREF(inst_args);
    return (PyObject *)obj;
}



static void nv_SpaceObject_dealloc(nv_SpaceObject *self) {
    nv_Space_free(self->space);

    // Decrease reference of each body object in array
    for (size_t i = 0; i < self->body_objects->size; i++) {
        PyObject *body = (PyObject *)self->body_objects->data[i];
        Py_DECREF(body);
    }

    nv_Array_free(self->body_objects);

    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int nv_SpaceObject_init(
    nv_SpaceObject *self,
    PyObject *args,
    PyObject *kwds
) {
    self->space = nv_Space_new();
    self->body_objects = nv_Array_new();

    return 0;
}


static PyObject *nv_SpaceObject_step(
    nv_SpaceObject *self,
    PyObject *args
) {
    double dt;
    int velocity_iters;
    int position_iters;
    int constraint_iters;
    int substeps;

    if (!PyArg_ParseTuple(
            args, "diiii",
            &dt,
            &velocity_iters,
            &position_iters,
            &constraint_iters,
            &substeps
    )) return NULL;

    nv_Space_step(
        self->space,
        dt,
        velocity_iters,
        position_iters,
        constraint_iters,
        substeps
    );

    // Update every body object
    for (size_t i = 0; i < self->body_objects->size; i++) {
        nv_Body *body = (nv_Body *)self->space->bodies->data[i];
        nv_BodyObject *body_object = (nv_BodyObject *)self->body_objects->data[i];
        Py_INCREF(body_object);
        
        body_object->position->vec.x = body->position.x;
        body_object->position->vec.y = body->position.y; 
        body_object->angle = body->angle; 
        body_object->radius = body->shape->radius;
    }

    Py_RETURN_NONE;
}

static PyObject *nv_SpaceObject_get_bodies(
    nv_SpaceObject *self,
    PyObject *Py_UNUSED(ignored)
) {
    PyObject *return_tup = PyTuple_New(self->body_objects->size);

    for (size_t i = 0; i < self->body_objects->size; i++) {
        nv_BodyObject *body = (nv_BodyObject *)self->body_objects->data[i];
        
        Py_INCREF(body);
        PyTuple_SET_ITEM(return_tup, i, body);
    }

    return return_tup;
}

static PyObject *nv_SpaceObject_add(
    nv_SpaceObject *self,
    PyObject *args
) {
    nv_BodyObject *body;

    if (!PyArg_ParseTuple(args, "O!", &nv_BodyObjectType, &body))
        return NULL;

    nv_Space_add(self->space, body->body);
    Py_INCREF(body);
    nv_Array_add(self->body_objects, body);

    Py_RETURN_NONE;
}

/**
 * Space object method interface
 */
static PyMethodDef nv_SpaceObject_methods[] = {
    {
        "step",
        (PyCFunction)nv_SpaceObject_step, METH_VARARGS,
        "Advance the simulation"
    },

    {
        "get_bodies",
        (PyCFunction)nv_SpaceObject_get_bodies, METH_NOARGS,
        "Get bodies"
    },

    {
        "add",
        (PyCFunction)nv_SpaceObject_add, METH_VARARGS,
        "Add body to space"
    },

    {NULL} // Sentinel
};

PyTypeObject nv_SpaceType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "nova.Space",
    .tp_doc = "Space object",
    .tp_basicsize = sizeof(nv_SpaceObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor)nv_SpaceObject_dealloc,
    .tp_init = (initproc)nv_SpaceObject_init,
    .tp_methods = nv_SpaceObject_methods,
    .tp_members = NULL
};



/**
 * Nova Physics method interface
*/
static PyMethodDef nova_methods[] = {
    {
        "create_circle",
        nv_create_circle, METH_VARARGS,
        "Create a body with circle shape."
    },

    {
        "create_rect",
        nv_create_rect, METH_VARARGS,
        "Create a body with rect (polygon) shape."
    },

    {NULL, NULL, 0, NULL} /* Sentinel */
};

/**
 * Nova Physics module interface
 */
static PyModuleDef nova_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "nova",
    .m_doc = "Nova Physics Engine",
    .m_size = -1,
    .m_methods = nova_methods
};

/**
 * Nova Physics module initializer
 */
PyMODINIT_FUNC PyInit__nova() {
    PyObject *m;

    if (PyType_Ready(&nv_SpaceType) < 0)
        return NULL;

    if (PyType_Ready(&nv_BodyObjectType) < 0)
        return NULL;

    if (PyType_Ready(&nv_Vector2ObjectType) < 0)
        return NULL;


    m = PyModule_Create(&nova_module);
    if (m == NULL)
        return NULL;

    /**
     * Add nova.Space
     */
    Py_INCREF(&nv_SpaceType);
    if (PyModule_AddObject(m, "Space", (PyObject *) &nv_SpaceType) < 0) {
        Py_DECREF(&nv_SpaceType);
        Py_DECREF(m);
        return NULL;
    }

    /**
     * Add nova.Body
     */
    Py_INCREF(&nv_BodyObjectType);
    if (PyModule_AddObject(m, "Body", (PyObject *) &nv_BodyObjectType) < 0) {
        Py_DECREF(&nv_BodyObjectType);
        Py_DECREF(m);
        return NULL;
    }

    /**
     * Add nova.Vector2
     */
    Py_INCREF(&nv_Vector2ObjectType);
    if (PyModule_AddObject(m, "Vector2", (PyObject *) &nv_Vector2ObjectType) < 0) {
        Py_DECREF(&nv_Vector2ObjectType);
        Py_DECREF(m);
        return NULL;
    }

    /* Add constants */

    PyModule_AddIntConstant(m, "STATIC",  nv_BodyType_STATIC);
    PyModule_AddIntConstant(m, "DYNAMIC", nv_BodyType_DYNAMIC);

    return m;
}
