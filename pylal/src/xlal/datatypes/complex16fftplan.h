/*
 * Copyright (C) 2009  Kipp Cannon
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


/*
 * ============================================================================
 *
 *                                  Preamble
 *
 * ============================================================================
 */


#include <Python.h>
#include <lal/ComplexFFT.h>


#define PYLAL_COMPLEX16FFTPLAN_MODULE_NAME "pylal.xlal.datatypes.complex16fftplan"


/*
 * ============================================================================
 *
 *                                    Type
 *
 * ============================================================================
 */


static PyTypeObject *_pylal_COMPLEX16FFTPlan_Type = NULL;
#define pylal_COMPLEX16FFTPlan_Type (*_pylal_COMPLEX16FFTPlan_Type)


typedef struct {
	PyObject_HEAD
	PyObject *owner;
	COMPLEX16FFTPlan *plan;
} pylal_COMPLEX16FFTPlan;


static PyObject *pylal_complex16fftplan_import(void)
{
	PyObject *name = PyString_FromString(PYLAL_COMPLEX16FFTPLAN_MODULE_NAME);
	PyObject *module = PyImport_Import(name);
	Py_DECREF(name);

	name = PyString_FromString("COMPLEX16FFTPlan");
	_pylal_COMPLEX16FFTPlan_Type = (PyTypeObject *) PyDict_GetItem(PyModule_GetDict(module), name);
	Py_INCREF(&pylal_COMPLEX16FFTPlan_Type);
	Py_DECREF(name);

	return module;
}


static PyObject *pylal_COMPLEX16FFTPlan_new(COMPLEX16FFTPlan *plan, PyObject *owner)
{
	PyObject *empty_tuple = PyTuple_New(0);
	pylal_COMPLEX16FFTPlan *obj = (pylal_COMPLEX16FFTPlan *) PyType_GenericNew(&pylal_COMPLEX16FFTPlan_Type, empty_tuple, NULL);
	Py_DECREF(empty_tuple);
	if(!obj) {
		if(!owner)
			XLALDestroyCOMPLEX16FFTPlan(plan);
		return NULL;
	}
	if(owner)
		Py_INCREF(owner);
	obj->owner = owner;
	XLALDestroyCOMPLEX16FFTPlan(obj->plan);
	obj->plan = plan;
	return (PyObject *) obj;
}
