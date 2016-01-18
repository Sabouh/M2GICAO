/****************************************************************************
** Meta object code from reading C++ file 'widgetParams.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src-gratin/view/widgetParams.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widgetParams.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FloatAttribWidget_t {
    QByteArrayData data[4];
    char stringdata[42];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FloatAttribWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FloatAttribWidget_t qt_meta_stringdata_FloatAttribWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "FloatAttribWidget"
QT_MOC_LITERAL(1, 18, 11), // "editClicked"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 10) // "setClicked"

    },
    "FloatAttribWidget\0editClicked\0\0"
    "setClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FloatAttribWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FloatAttribWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FloatAttribWidget *_t = static_cast<FloatAttribWidget *>(_o);
        switch (_id) {
        case 0: _t->editClicked(); break;
        case 1: _t->setClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject FloatAttribWidget::staticMetaObject = {
    { &NodeWidget::staticMetaObject, qt_meta_stringdata_FloatAttribWidget.data,
      qt_meta_data_FloatAttribWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FloatAttribWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FloatAttribWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FloatAttribWidget.stringdata))
        return static_cast<void*>(const_cast< FloatAttribWidget*>(this));
    return NodeWidget::qt_metacast(_clname);
}

int FloatAttribWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NodeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_IntAttribWidget_t {
    QByteArrayData data[4];
    char stringdata[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IntAttribWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IntAttribWidget_t qt_meta_stringdata_IntAttribWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "IntAttribWidget"
QT_MOC_LITERAL(1, 16, 11), // "editClicked"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 10) // "setClicked"

    },
    "IntAttribWidget\0editClicked\0\0setClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IntAttribWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void IntAttribWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IntAttribWidget *_t = static_cast<IntAttribWidget *>(_o);
        switch (_id) {
        case 0: _t->editClicked(); break;
        case 1: _t->setClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject IntAttribWidget::staticMetaObject = {
    { &NodeWidget::staticMetaObject, qt_meta_stringdata_IntAttribWidget.data,
      qt_meta_data_IntAttribWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IntAttribWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IntAttribWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IntAttribWidget.stringdata))
        return static_cast<void*>(const_cast< IntAttribWidget*>(this));
    return NodeWidget::qt_metacast(_clname);
}

int IntAttribWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NodeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_Vector2fAttribWidget_t {
    QByteArrayData data[4];
    char stringdata[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector2fAttribWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector2fAttribWidget_t qt_meta_stringdata_Vector2fAttribWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "Vector2fAttribWidget"
QT_MOC_LITERAL(1, 21, 11), // "editClicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 10) // "setClicked"

    },
    "Vector2fAttribWidget\0editClicked\0\0"
    "setClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector2fAttribWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Vector2fAttribWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector2fAttribWidget *_t = static_cast<Vector2fAttribWidget *>(_o);
        switch (_id) {
        case 0: _t->editClicked(); break;
        case 1: _t->setClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Vector2fAttribWidget::staticMetaObject = {
    { &NodeWidget::staticMetaObject, qt_meta_stringdata_Vector2fAttribWidget.data,
      qt_meta_data_Vector2fAttribWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector2fAttribWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector2fAttribWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector2fAttribWidget.stringdata))
        return static_cast<void*>(const_cast< Vector2fAttribWidget*>(this));
    return NodeWidget::qt_metacast(_clname);
}

int Vector2fAttribWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NodeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_Vector2iAttribWidget_t {
    QByteArrayData data[4];
    char stringdata[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector2iAttribWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector2iAttribWidget_t qt_meta_stringdata_Vector2iAttribWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "Vector2iAttribWidget"
QT_MOC_LITERAL(1, 21, 11), // "editClicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 10) // "setClicked"

    },
    "Vector2iAttribWidget\0editClicked\0\0"
    "setClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector2iAttribWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Vector2iAttribWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector2iAttribWidget *_t = static_cast<Vector2iAttribWidget *>(_o);
        switch (_id) {
        case 0: _t->editClicked(); break;
        case 1: _t->setClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Vector2iAttribWidget::staticMetaObject = {
    { &NodeWidget::staticMetaObject, qt_meta_stringdata_Vector2iAttribWidget.data,
      qt_meta_data_Vector2iAttribWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector2iAttribWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector2iAttribWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector2iAttribWidget.stringdata))
        return static_cast<void*>(const_cast< Vector2iAttribWidget*>(this));
    return NodeWidget::qt_metacast(_clname);
}

int Vector2iAttribWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NodeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_Vector3fAttribWidget_t {
    QByteArrayData data[4];
    char stringdata[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector3fAttribWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector3fAttribWidget_t qt_meta_stringdata_Vector3fAttribWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "Vector3fAttribWidget"
QT_MOC_LITERAL(1, 21, 11), // "editClicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 10) // "setClicked"

    },
    "Vector3fAttribWidget\0editClicked\0\0"
    "setClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector3fAttribWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Vector3fAttribWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector3fAttribWidget *_t = static_cast<Vector3fAttribWidget *>(_o);
        switch (_id) {
        case 0: _t->editClicked(); break;
        case 1: _t->setClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Vector3fAttribWidget::staticMetaObject = {
    { &NodeWidget::staticMetaObject, qt_meta_stringdata_Vector3fAttribWidget.data,
      qt_meta_data_Vector3fAttribWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector3fAttribWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector3fAttribWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector3fAttribWidget.stringdata))
        return static_cast<void*>(const_cast< Vector3fAttribWidget*>(this));
    return NodeWidget::qt_metacast(_clname);
}

int Vector3fAttribWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NodeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_Vector3iAttribWidget_t {
    QByteArrayData data[4];
    char stringdata[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector3iAttribWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector3iAttribWidget_t qt_meta_stringdata_Vector3iAttribWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "Vector3iAttribWidget"
QT_MOC_LITERAL(1, 21, 11), // "editClicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 10) // "setClicked"

    },
    "Vector3iAttribWidget\0editClicked\0\0"
    "setClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector3iAttribWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Vector3iAttribWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector3iAttribWidget *_t = static_cast<Vector3iAttribWidget *>(_o);
        switch (_id) {
        case 0: _t->editClicked(); break;
        case 1: _t->setClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Vector3iAttribWidget::staticMetaObject = {
    { &NodeWidget::staticMetaObject, qt_meta_stringdata_Vector3iAttribWidget.data,
      qt_meta_data_Vector3iAttribWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector3iAttribWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector3iAttribWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector3iAttribWidget.stringdata))
        return static_cast<void*>(const_cast< Vector3iAttribWidget*>(this));
    return NodeWidget::qt_metacast(_clname);
}

int Vector3iAttribWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NodeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_Vector4fAttribWidget_t {
    QByteArrayData data[4];
    char stringdata[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector4fAttribWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector4fAttribWidget_t qt_meta_stringdata_Vector4fAttribWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "Vector4fAttribWidget"
QT_MOC_LITERAL(1, 21, 11), // "editClicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 10) // "setClicked"

    },
    "Vector4fAttribWidget\0editClicked\0\0"
    "setClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector4fAttribWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Vector4fAttribWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector4fAttribWidget *_t = static_cast<Vector4fAttribWidget *>(_o);
        switch (_id) {
        case 0: _t->editClicked(); break;
        case 1: _t->setClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Vector4fAttribWidget::staticMetaObject = {
    { &NodeWidget::staticMetaObject, qt_meta_stringdata_Vector4fAttribWidget.data,
      qt_meta_data_Vector4fAttribWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector4fAttribWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector4fAttribWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector4fAttribWidget.stringdata))
        return static_cast<void*>(const_cast< Vector4fAttribWidget*>(this));
    return NodeWidget::qt_metacast(_clname);
}

int Vector4fAttribWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NodeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_Vector4iAttribWidget_t {
    QByteArrayData data[4];
    char stringdata[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector4iAttribWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector4iAttribWidget_t qt_meta_stringdata_Vector4iAttribWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "Vector4iAttribWidget"
QT_MOC_LITERAL(1, 21, 11), // "editClicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 10) // "setClicked"

    },
    "Vector4iAttribWidget\0editClicked\0\0"
    "setClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector4iAttribWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Vector4iAttribWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector4iAttribWidget *_t = static_cast<Vector4iAttribWidget *>(_o);
        switch (_id) {
        case 0: _t->editClicked(); break;
        case 1: _t->setClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Vector4iAttribWidget::staticMetaObject = {
    { &NodeWidget::staticMetaObject, qt_meta_stringdata_Vector4iAttribWidget.data,
      qt_meta_data_Vector4iAttribWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector4iAttribWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector4iAttribWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector4iAttribWidget.stringdata))
        return static_cast<void*>(const_cast< Vector4iAttribWidget*>(this));
    return NodeWidget::qt_metacast(_clname);
}

int Vector4iAttribWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NodeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_FloatSliderWidget_t {
    QByteArrayData data[4];
    char stringdata[43];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FloatSliderWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FloatSliderWidget_t qt_meta_stringdata_FloatSliderWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "FloatSliderWidget"
QT_MOC_LITERAL(1, 18, 10), // "valChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 12) // "valueChanged"

    },
    "FloatSliderWidget\0valChanged\0\0"
    "valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FloatSliderWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void FloatSliderWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FloatSliderWidget *_t = static_cast<FloatSliderWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FloatSliderWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FloatSliderWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject FloatSliderWidget::staticMetaObject = {
    { &FloatAttribWidget::staticMetaObject, qt_meta_stringdata_FloatSliderWidget.data,
      qt_meta_data_FloatSliderWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FloatSliderWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FloatSliderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FloatSliderWidget.stringdata))
        return static_cast<void*>(const_cast< FloatSliderWidget*>(this));
    return FloatAttribWidget::qt_metacast(_clname);
}

int FloatSliderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = FloatAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void FloatSliderWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_FloatSpinWidget_t {
    QByteArrayData data[4];
    char stringdata[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FloatSpinWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FloatSpinWidget_t qt_meta_stringdata_FloatSpinWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "FloatSpinWidget"
QT_MOC_LITERAL(1, 16, 10), // "valChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 12) // "valueChanged"

    },
    "FloatSpinWidget\0valChanged\0\0valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FloatSpinWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,

       0        // eod
};

void FloatSpinWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FloatSpinWidget *_t = static_cast<FloatSpinWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FloatSpinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FloatSpinWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject FloatSpinWidget::staticMetaObject = {
    { &FloatAttribWidget::staticMetaObject, qt_meta_stringdata_FloatSpinWidget.data,
      qt_meta_data_FloatSpinWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FloatSpinWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FloatSpinWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FloatSpinWidget.stringdata))
        return static_cast<void*>(const_cast< FloatSpinWidget*>(this));
    return FloatAttribWidget::qt_metacast(_clname);
}

int FloatSpinWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = FloatAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void FloatSpinWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_IntSliderWidget_t {
    QByteArrayData data[4];
    char stringdata[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IntSliderWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IntSliderWidget_t qt_meta_stringdata_IntSliderWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "IntSliderWidget"
QT_MOC_LITERAL(1, 16, 10), // "valChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 12) // "valueChanged"

    },
    "IntSliderWidget\0valChanged\0\0valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IntSliderWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void IntSliderWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IntSliderWidget *_t = static_cast<IntSliderWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IntSliderWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IntSliderWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject IntSliderWidget::staticMetaObject = {
    { &IntAttribWidget::staticMetaObject, qt_meta_stringdata_IntSliderWidget.data,
      qt_meta_data_IntSliderWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IntSliderWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IntSliderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IntSliderWidget.stringdata))
        return static_cast<void*>(const_cast< IntSliderWidget*>(this));
    return IntAttribWidget::qt_metacast(_clname);
}

int IntSliderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IntAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void IntSliderWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_IntSpinWidget_t {
    QByteArrayData data[4];
    char stringdata[39];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IntSpinWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IntSpinWidget_t qt_meta_stringdata_IntSpinWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "IntSpinWidget"
QT_MOC_LITERAL(1, 14, 10), // "valChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 12) // "valueChanged"

    },
    "IntSpinWidget\0valChanged\0\0valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IntSpinWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void IntSpinWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IntSpinWidget *_t = static_cast<IntSpinWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IntSpinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IntSpinWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject IntSpinWidget::staticMetaObject = {
    { &IntAttribWidget::staticMetaObject, qt_meta_stringdata_IntSpinWidget.data,
      qt_meta_data_IntSpinWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IntSpinWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IntSpinWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IntSpinWidget.stringdata))
        return static_cast<void*>(const_cast< IntSpinWidget*>(this));
    return IntAttribWidget::qt_metacast(_clname);
}

int IntSpinWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IntAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void IntSpinWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_Vector2fSpinWidget_t {
    QByteArrayData data[4];
    char stringdata[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector2fSpinWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector2fSpinWidget_t qt_meta_stringdata_Vector2fSpinWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "Vector2fSpinWidget"
QT_MOC_LITERAL(1, 19, 10), // "valChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12) // "valueChanged"

    },
    "Vector2fSpinWidget\0valChanged\0\0"
    "valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector2fSpinWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,

       0        // eod
};

void Vector2fSpinWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector2fSpinWidget *_t = static_cast<Vector2fSpinWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Vector2fSpinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Vector2fSpinWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Vector2fSpinWidget::staticMetaObject = {
    { &Vector2fAttribWidget::staticMetaObject, qt_meta_stringdata_Vector2fSpinWidget.data,
      qt_meta_data_Vector2fSpinWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector2fSpinWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector2fSpinWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector2fSpinWidget.stringdata))
        return static_cast<void*>(const_cast< Vector2fSpinWidget*>(this));
    return Vector2fAttribWidget::qt_metacast(_clname);
}

int Vector2fSpinWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Vector2fAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Vector2fSpinWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_Vector2iSpinWidget_t {
    QByteArrayData data[4];
    char stringdata[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector2iSpinWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector2iSpinWidget_t qt_meta_stringdata_Vector2iSpinWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "Vector2iSpinWidget"
QT_MOC_LITERAL(1, 19, 10), // "valChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12) // "valueChanged"

    },
    "Vector2iSpinWidget\0valChanged\0\0"
    "valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector2iSpinWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void Vector2iSpinWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector2iSpinWidget *_t = static_cast<Vector2iSpinWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Vector2iSpinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Vector2iSpinWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Vector2iSpinWidget::staticMetaObject = {
    { &Vector2iAttribWidget::staticMetaObject, qt_meta_stringdata_Vector2iSpinWidget.data,
      qt_meta_data_Vector2iSpinWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector2iSpinWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector2iSpinWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector2iSpinWidget.stringdata))
        return static_cast<void*>(const_cast< Vector2iSpinWidget*>(this));
    return Vector2iAttribWidget::qt_metacast(_clname);
}

int Vector2iSpinWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Vector2iAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Vector2iSpinWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_Vector3fSpinWidget_t {
    QByteArrayData data[4];
    char stringdata[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector3fSpinWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector3fSpinWidget_t qt_meta_stringdata_Vector3fSpinWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "Vector3fSpinWidget"
QT_MOC_LITERAL(1, 19, 10), // "valChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12) // "valueChanged"

    },
    "Vector3fSpinWidget\0valChanged\0\0"
    "valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector3fSpinWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,

       0        // eod
};

void Vector3fSpinWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector3fSpinWidget *_t = static_cast<Vector3fSpinWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Vector3fSpinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Vector3fSpinWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Vector3fSpinWidget::staticMetaObject = {
    { &Vector3fAttribWidget::staticMetaObject, qt_meta_stringdata_Vector3fSpinWidget.data,
      qt_meta_data_Vector3fSpinWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector3fSpinWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector3fSpinWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector3fSpinWidget.stringdata))
        return static_cast<void*>(const_cast< Vector3fSpinWidget*>(this));
    return Vector3fAttribWidget::qt_metacast(_clname);
}

int Vector3fSpinWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Vector3fAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Vector3fSpinWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_Vector3fColorWidget_t {
    QByteArrayData data[6];
    char stringdata[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector3fColorWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector3fColorWidget_t qt_meta_stringdata_Vector3fColorWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "Vector3fColorWidget"
QT_MOC_LITERAL(1, 20, 10), // "valChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 13), // "buttonClicked"
QT_MOC_LITERAL(4, 46, 12), // "colorChanged"
QT_MOC_LITERAL(5, 59, 5) // "color"

    },
    "Vector3fColorWidget\0valChanged\0\0"
    "buttonClicked\0colorChanged\0color"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector3fColorWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   30,    2, 0x08 /* Private */,
       4,    1,   31,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,    5,

       0        // eod
};

void Vector3fColorWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector3fColorWidget *_t = static_cast<Vector3fColorWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->buttonClicked(); break;
        case 2: _t->colorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Vector3fColorWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Vector3fColorWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Vector3fColorWidget::staticMetaObject = {
    { &Vector3fAttribWidget::staticMetaObject, qt_meta_stringdata_Vector3fColorWidget.data,
      qt_meta_data_Vector3fColorWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector3fColorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector3fColorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector3fColorWidget.stringdata))
        return static_cast<void*>(const_cast< Vector3fColorWidget*>(this));
    return Vector3fAttribWidget::qt_metacast(_clname);
}

int Vector3fColorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Vector3fAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Vector3fColorWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_Vector3iSpinWidget_t {
    QByteArrayData data[4];
    char stringdata[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector3iSpinWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector3iSpinWidget_t qt_meta_stringdata_Vector3iSpinWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "Vector3iSpinWidget"
QT_MOC_LITERAL(1, 19, 10), // "valChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12) // "valueChanged"

    },
    "Vector3iSpinWidget\0valChanged\0\0"
    "valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector3iSpinWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void Vector3iSpinWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector3iSpinWidget *_t = static_cast<Vector3iSpinWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Vector3iSpinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Vector3iSpinWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Vector3iSpinWidget::staticMetaObject = {
    { &Vector3iAttribWidget::staticMetaObject, qt_meta_stringdata_Vector3iSpinWidget.data,
      qt_meta_data_Vector3iSpinWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector3iSpinWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector3iSpinWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector3iSpinWidget.stringdata))
        return static_cast<void*>(const_cast< Vector3iSpinWidget*>(this));
    return Vector3iAttribWidget::qt_metacast(_clname);
}

int Vector3iSpinWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Vector3iAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Vector3iSpinWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_Vector4fSpinWidget_t {
    QByteArrayData data[4];
    char stringdata[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector4fSpinWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector4fSpinWidget_t qt_meta_stringdata_Vector4fSpinWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "Vector4fSpinWidget"
QT_MOC_LITERAL(1, 19, 10), // "valChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12) // "valueChanged"

    },
    "Vector4fSpinWidget\0valChanged\0\0"
    "valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector4fSpinWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,

       0        // eod
};

void Vector4fSpinWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector4fSpinWidget *_t = static_cast<Vector4fSpinWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Vector4fSpinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Vector4fSpinWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Vector4fSpinWidget::staticMetaObject = {
    { &Vector4fAttribWidget::staticMetaObject, qt_meta_stringdata_Vector4fSpinWidget.data,
      qt_meta_data_Vector4fSpinWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector4fSpinWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector4fSpinWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector4fSpinWidget.stringdata))
        return static_cast<void*>(const_cast< Vector4fSpinWidget*>(this));
    return Vector4fAttribWidget::qt_metacast(_clname);
}

int Vector4fSpinWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Vector4fAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Vector4fSpinWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_Vector4fColorWidget_t {
    QByteArrayData data[6];
    char stringdata[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector4fColorWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector4fColorWidget_t qt_meta_stringdata_Vector4fColorWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "Vector4fColorWidget"
QT_MOC_LITERAL(1, 20, 10), // "valChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 13), // "buttonClicked"
QT_MOC_LITERAL(4, 46, 12), // "colorChanged"
QT_MOC_LITERAL(5, 59, 5) // "color"

    },
    "Vector4fColorWidget\0valChanged\0\0"
    "buttonClicked\0colorChanged\0color"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector4fColorWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   30,    2, 0x08 /* Private */,
       4,    1,   31,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,    5,

       0        // eod
};

void Vector4fColorWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector4fColorWidget *_t = static_cast<Vector4fColorWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->buttonClicked(); break;
        case 2: _t->colorChanged((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Vector4fColorWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Vector4fColorWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Vector4fColorWidget::staticMetaObject = {
    { &Vector4fAttribWidget::staticMetaObject, qt_meta_stringdata_Vector4fColorWidget.data,
      qt_meta_data_Vector4fColorWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector4fColorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector4fColorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector4fColorWidget.stringdata))
        return static_cast<void*>(const_cast< Vector4fColorWidget*>(this));
    return Vector4fAttribWidget::qt_metacast(_clname);
}

int Vector4fColorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Vector4fAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Vector4fColorWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_Vector4iSpinWidget_t {
    QByteArrayData data[4];
    char stringdata[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Vector4iSpinWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Vector4iSpinWidget_t qt_meta_stringdata_Vector4iSpinWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "Vector4iSpinWidget"
QT_MOC_LITERAL(1, 19, 10), // "valChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12) // "valueChanged"

    },
    "Vector4iSpinWidget\0valChanged\0\0"
    "valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Vector4iSpinWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void Vector4iSpinWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Vector4iSpinWidget *_t = static_cast<Vector4iSpinWidget *>(_o);
        switch (_id) {
        case 0: _t->valChanged(); break;
        case 1: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Vector4iSpinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Vector4iSpinWidget::valChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Vector4iSpinWidget::staticMetaObject = {
    { &Vector4iAttribWidget::staticMetaObject, qt_meta_stringdata_Vector4iSpinWidget.data,
      qt_meta_data_Vector4iSpinWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Vector4iSpinWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Vector4iSpinWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Vector4iSpinWidget.stringdata))
        return static_cast<void*>(const_cast< Vector4iSpinWidget*>(this));
    return Vector4iAttribWidget::qt_metacast(_clname);
}

int Vector4iSpinWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Vector4iAttribWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Vector4iSpinWidget::valChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_TrackballCameraWidget_t {
    QByteArrayData data[7];
    char stringdata[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TrackballCameraWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TrackballCameraWidget_t qt_meta_stringdata_TrackballCameraWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "TrackballCameraWidget"
QT_MOC_LITERAL(1, 22, 11), // "editClicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 10), // "setClicked"
QT_MOC_LITERAL(4, 46, 17), // "projectionChanged"
QT_MOC_LITERAL(5, 64, 15), // "rotationChanged"
QT_MOC_LITERAL(6, 80, 16) // "checkChangeCurve"

    },
    "TrackballCameraWidget\0editClicked\0\0"
    "setClicked\0projectionChanged\0"
    "rotationChanged\0checkChangeCurve"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TrackballCameraWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    1,   41,    2, 0x08 /* Private */,
       5,    1,   44,    2, 0x08 /* Private */,
       6,    0,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void TrackballCameraWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TrackballCameraWidget *_t = static_cast<TrackballCameraWidget *>(_o);
        switch (_id) {
        case 0: _t->editClicked(); break;
        case 1: _t->setClicked(); break;
        case 2: _t->projectionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->rotationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->checkChangeCurve(); break;
        default: ;
        }
    }
}

const QMetaObject TrackballCameraWidget::staticMetaObject = {
    { &NodeWidget::staticMetaObject, qt_meta_stringdata_TrackballCameraWidget.data,
      qt_meta_data_TrackballCameraWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TrackballCameraWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrackballCameraWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TrackballCameraWidget.stringdata))
        return static_cast<void*>(const_cast< TrackballCameraWidget*>(this));
    return NodeWidget::qt_metacast(_clname);
}

int TrackballCameraWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NodeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
