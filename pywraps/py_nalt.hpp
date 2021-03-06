#ifndef __PY_IDA_NALT__
#define __PY_IDA_NALT__

//<code(py_nalt)>

//-------------------------------------------------------------------------
// callback for enumerating imports
// ea:   import address
// name: import name (NULL if imported by ordinal)
// ord:  import ordinal (0 for imports by name)
// param: user parameter passed to enum_import_names()
// return: 1-ok, 0-stop enumeration
static int idaapi py_import_enum_cb(
        ea_t ea,
        const char *name,
        uval_t ord,
        void *param)
{
  // If no name, try to get the name associated with the 'ea'. It may be coming from IDS
  qstring name_buf;
  if ( name == NULL && get_name(&name_buf, ea) > 0 )
    name = name_buf.begin();

  PYW_GIL_CHECK_LOCKED_SCOPE();
  ref_t py_name;
  if ( name == NULL )
    py_name = borref_t(Py_None);
  else
    py_name = newref_t(PyString_FromString(name));

  newref_t py_ord(Py_BuildValue(PY_BV_UVAL, bvuval_t(ord)));
  newref_t py_ea(Py_BuildValue(PY_BV_EA, bvea_t(ea)));
  newref_t py_result(
          PyObject_CallFunctionObjArgs(
                  (PyObject *)param,
                  py_ea.o,
                  py_name.o,
                  py_ord.o,
                  NULL));
  return py_result != NULL && PyObject_IsTrue(py_result.o) ? 1 : 0;
}
//</code(py_nalt)>

//<inline(py_nalt)>

//-------------------------------------------------------------------------
/*
#<pydoc>
def get_import_module_name(path, fname, callback):
    """
    Returns the name of an imported module given its index
    @return: None or the module name
    """
    pass
#</pydoc>
*/
static PyObject *py_get_import_module_name(int mod_index)
{
  PYW_GIL_CHECK_LOCKED_SCOPE();
  qstring qbuf;
  if ( !get_import_module_name(&qbuf, mod_index) )
    Py_RETURN_NONE;

  return PyString_FromStringAndSize(qbuf.begin(), qbuf.length());
}

//-------------------------------------------------------------------------
/*
#<pydoc>
def get_switch_info(ea):
    """
    Returns the a switch_info_t structure containing the information about the switch.
    Please refer to the SDK sample 'uiswitch'
    @return: None or switch_info_t instance
    """
    pass
#</pydoc>
*/
PyObject *py_get_switch_info(ea_t ea)
{
  switch_info_t *ex = new switch_info_t();
  ref_t py_obj;
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( ::get_switch_info(ex, ea) <= 0
    || (py_obj = create_linked_class_instance(S_IDA_NALT_MODNAME, S_PY_SWIEX_CLSNAME, ex)) == NULL )
  {
    delete ex;
    Py_RETURN_NONE;
  }
  py_obj.incref();
  return py_obj.o;
}

//-------------------------------------------------------------------------
/*
#<pydoc>
def set_switch_info(ea, switch_info):
    """
    Saves the switch information in the database
    Please refer to the SDK sample 'uiswitch'
    @return: Boolean
    """
    pass
#</pydoc>
*/
bool py_set_switch_info(ea_t ea, PyObject *py_swi)
{
  switch_info_t *swi = switch_info_t_get_clink(py_swi);
  if ( swi == NULL )
    return false;

  set_switch_info(ea, *swi);
  return true;
}

//-------------------------------------------------------------------------
/*
#<pydoc>
def del_switch_info(ea):
    """
    Deletes stored switch information
    """
    pass
#</pydoc>
*/
void py_del_switch_info(ea_t ea)
{
  del_switch_info(ea);
}

//-------------------------------------------------------------------------
/*
#<pydoc>
def enum_import_names(mod_index, callback):
    """
    Enumerate imports from a specific module.
    Please refer to ex_imports.py example.

    @param mod_index: The module index
    @param callback: A callable object that will be invoked with an ea, name (could be None) and ordinal.
    @return: 1-finished ok, -1 on error, otherwise callback return value (<=0)
    """
    pass
#</pydoc>
*/
static int py_enum_import_names(int mod_index, PyObject *py_cb)
{
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( !PyCallable_Check(py_cb) )
    return -1;
  return enum_import_names(mod_index, py_import_enum_cb, py_cb);
}

//-------------------------------------------------------------------------
static PyObject *switch_info_t_create()
{
  switch_info_t *inst = new switch_info_t();
  PYW_GIL_CHECK_LOCKED_SCOPE();
  return PyCObject_FromVoidPtr(inst, NULL);
}

//---------------------------------------------------------------------------
static bool switch_info_t_destroy(PyObject *py_obj)
{
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( !PyCObject_Check(py_obj) )
    return false;
  switch_info_t *inst = (switch_info_t *) PyCObject_AsVoidPtr(py_obj);
  delete inst;
  return true;
}

static bool switch_info_t_assign(PyObject *self, PyObject *other)
{
  switch_info_t *lhs = switch_info_t_get_clink(self);
  switch_info_t *rhs = switch_info_t_get_clink(other);
  if ( lhs == NULL || rhs == NULL )
    return false;

  *lhs = *rhs;
  return true;
}

//-------------------------------------------------------------------------
// Auto generated - begin
//

static PyObject *switch_info_t_get_regdtype(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue("b", (char)link->regdtype);
}
static void switch_info_t_set_regdtype(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  PYW_GIL_CHECK_LOCKED_SCOPE();
  link->regdtype = (char)PyInt_AsLong(value);
}

static PyObject *switch_info_t_get_flags(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue("i", link->flags);
}
static void switch_info_t_set_flags(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  PYW_GIL_CHECK_LOCKED_SCOPE();
  link->flags = (uint32)PyInt_AsLong(value);
}

static PyObject *switch_info_t_get_jcases(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue("i", link->jcases);
}
static void switch_info_t_set_jcases(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  PYW_GIL_CHECK_LOCKED_SCOPE();
  link->jcases = (int)PyInt_AsLong(value);
}

static PyObject *switch_info_t_get_regnum(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue("i", (int)link->regnum);
}
static void switch_info_t_set_regnum(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  PYW_GIL_CHECK_LOCKED_SCOPE();
  link->regnum = (int)PyInt_AsLong(value);
}

static PyObject *switch_info_t_get_ncases(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue("H", (uint16)link->ncases);
}
static void switch_info_t_set_ncases(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  PYW_GIL_CHECK_LOCKED_SCOPE();
  link->ncases = (ushort)PyInt_AsLong(value);
}

static PyObject *switch_info_t_get_defjump(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue(PY_BV_EA, (bvea_t)link->defjump);
}
static void switch_info_t_set_defjump(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  PYW_GIL_CHECK_LOCKED_SCOPE();
  uint64 v(0); PyW_GetNumber(value, &v);
  link->defjump = ea_t(v);
}

static PyObject *switch_info_t_get_jumps(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue(PY_BV_EA, (bvea_t)link->jumps);
}
static void switch_info_t_set_jumps(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  PYW_GIL_CHECK_LOCKED_SCOPE();
  uint64 v(0); PyW_GetNumber(value, &v);
  link->jumps = ea_t(v);
}

static PyObject *switch_info_t_get_elbase(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue(PY_BV_EA, (bvea_t)link->elbase);
}
static void switch_info_t_set_elbase(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  uint64 v(0);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  PyW_GetNumber(value, &v);
  link->elbase = ea_t(v);
}

static PyObject *switch_info_t_get_startea(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue(PY_BV_EA, (bvea_t)link->startea);
}
static void switch_info_t_set_startea(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  uint64 v(0);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  PyW_GetNumber(value, &v);
  link->startea = ea_t(v);
}

static PyObject *switch_info_t_get_custom(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue(PY_BV_UVAL, (bvuval_t)link->custom);
}
static void switch_info_t_set_custom(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  uint64 v(0);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  PyW_GetNumber(value, &v);
  link->custom = uval_t(v);
}

static PyObject *switch_info_t_get_ind_lowcase(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue(PY_BV_SVAL, (bvsval_t)link->ind_lowcase);
}
static void switch_info_t_set_ind_lowcase(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  uint64 v(0);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  PyW_GetNumber(value, &v);
  link->ind_lowcase = sval_t(v);
}

static PyObject *switch_info_t_get_values_lowcase(PyObject *self)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  if ( link == NULL )
    Py_RETURN_NONE;
  return Py_BuildValue(PY_BV_EA, (bvea_t)link->values);
}
static void switch_info_t_set_values_lowcase(PyObject *self, PyObject *value)
{
  switch_info_t *link = switch_info_t_get_clink(self);
  if ( link == NULL )
    return;
  uint64 v(0);
  PYW_GIL_CHECK_LOCKED_SCOPE();
  PyW_GetNumber(value, &v);
  link->values = ea_t(v);
}

//
// Auto generated - end
//
//-------------------------------------------------------------------------
//</inline(py_nalt)>

#endif
