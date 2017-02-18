#ifndef PLOT_H
#define PLOT_H

#include <iostream>
#include <vector>

#include <python2.7/Python.h>
#include <numpy/arrayobject.h>


namespace plt
{
	PyObject *python_function_show;
	PyObject *python_function_plot;
	PyObject *python_function_imshow;
	PyObject *python_function_figure;
	PyObject *python_function_imsave;
	PyObject *python_empty_tuple;

	void init()
	{
		char name[] = "plotting";
		Py_SetProgramName(name);
		Py_Initialize();

	 	PyObject* pymod =  PyImport_ImportModule("matplotlib.pyplot");

		python_function_show   = PyObject_GetAttrString(pymod, "show");
		python_function_plot   = PyObject_GetAttrString(pymod, "plot");
		python_function_imshow = PyObject_GetAttrString(pymod, "imshow");
		python_function_figure = PyObject_GetAttrString(pymod, "figure");
		python_function_imsave = PyObject_GetAttrString(pymod, "imsave");

		import_array();

	}

	void figure(int a, int b)
	{
		PyObject* kwargs = PyDict_New();
		PyObject* figsize = PyTuple_New(2);
		PyTuple_SetItem(figsize, 0, PyLong_FromLong(a));
		PyTuple_SetItem(figsize, 1, PyLong_FromLong(b));
		//PyDict_SetItemString(kwargs, "num", PyFloat_FromDouble(0.0));
		PyDict_SetItemString(kwargs, "figsize", figsize);
		PyObject* res = PyObject_CallObject(python_function_figure, kwargs);

		Py_DECREF(res);
  	}

	void show()
	{
		PyObject* res = PyObject_CallObject(python_function_show, python_empty_tuple);
		Py_DECREF(res);
	}

	void imsave(PyObject *a, const std::string& filename)
	{
		PyObject* pyfilename = PyString_FromString(filename.c_str());

		PyObject* args = PyTuple_New(2);
		PyTuple_SetItem(args, 0, pyfilename);
		PyTuple_SetItem(args, 1, a);

		PyObject* res = PyObject_CallObject(python_function_imsave, args);//, kwargs);

		//Py_DECREF(kwargs);
		//Py_DECREF(pyfilename);
		//Py_DECREF(args);
		//Py_DECREF(res);
	}

	bool plot(PyObject *a, PyObject *b, const std::string& s = "")
	{
		PyObject* pystring = PyString_FromString(s.c_str());
		PyObject* plot_args = PyTuple_New(3);
		PyTuple_SetItem(plot_args, 0, a);
		PyTuple_SetItem(plot_args, 1, b);
		PyTuple_SetItem(plot_args, 2, pystring);

		PyObject* res = PyObject_CallObject(python_function_plot, plot_args);

		Py_DECREF(plot_args);
		if(res) Py_DECREF(res);

		return res;
	}

	bool imshow(PyObject *a)
	{
		PyObject* plot_args = PyTuple_New(1);
		PyTuple_SetItem(plot_args, 0, a);

		PyObject* res = PyObject_CallObject(python_function_imshow, plot_args);

		Py_DECREF(plot_args);
		if(res) Py_DECREF(res);

		return res;
	}
}

#endif