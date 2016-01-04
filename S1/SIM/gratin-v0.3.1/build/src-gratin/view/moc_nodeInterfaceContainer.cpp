/****************************************************************************
** Meta object code from reading C++ file 'nodeInterfaceContainer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src-gratin/view/nodeInterfaceContainer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nodeInterfaceContainer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_InterfaceTitle_t {
    QByteArrayData data[9];
    char stringdata[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_InterfaceTitle_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_InterfaceTitle_t qt_meta_stringdata_InterfaceTitle = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 10),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 11),
QT_MOC_LITERAL(4, 39, 15),
QT_MOC_LITERAL(5, 55, 14),
QT_MOC_LITERAL(6, 70, 11),
QT_MOC_LITERAL(7, 82, 12),
QT_MOC_LITERAL(8, 95, 15)
    },
    "InterfaceTitle\0helpNeeded\0\0closeNeeded\0"
    "highlightNeeded\0externalNeeded\0"
    "helpClicked\0closeClicked\0externalClicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InterfaceTitle[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06,
       3,    1,   54,    2, 0x06,
       4,    1,   57,    2, 0x06,
       5,    1,   60,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       6,    0,   63,    2, 0x0a,
       7,    0,   64,    2, 0x0a,
       8,    0,   65,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QPoint,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void InterfaceTitle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        InterfaceTitle *_t = static_cast<InterfaceTitle *>(_o);
        switch (_id) {
        case 0: _t->helpNeeded((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 1: _t->closeNeeded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->highlightNeeded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->externalNeeded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->helpClicked(); break;
        case 5: _t->closeClicked(); break;
        case 6: _t->externalClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (InterfaceTitle::*_t)(int , const QPoint & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterfaceTitle::helpNeeded)) {
                *result = 0;
            }
        }
        {
            typedef void (InterfaceTitle::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterfaceTitle::closeNeeded)) {
                *result = 1;
            }
        }
        {
            typedef void (InterfaceTitle::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterfaceTitle::highlightNeeded)) {
                *result = 2;
            }
        }
        {
            typedef void (InterfaceTitle::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InterfaceTitle::externalNeeded)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject InterfaceTitle::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_InterfaceTitle.data,
      qt_meta_data_InterfaceTitle,  qt_static_metacall, 0, 0}
};


const QMetaObject *InterfaceTitle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InterfaceTitle::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InterfaceTitle.stringdata))
        return static_cast<void*>(const_cast< InterfaceTitle*>(this));
    return QWidget::qt_metacast(_clname);
}

int InterfaceTitle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void InterfaceTitle::helpNeeded(int _t1, const QPoint & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void InterfaceTitle::closeNeeded(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void InterfaceTitle::highlightNeeded(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void InterfaceTitle::externalNeeded(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
struct qt_meta_stringdata_NodeExternalContainer_t {
    QByteArrayData data[3];
    char stringdata[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_NodeExternalContainer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_NodeExternalContainer_t qt_meta_stringdata_NodeExternalContainer = {
    {
QT_MOC_LITERAL(0, 0, 21),
QT_MOC_LITERAL(1, 22, 15),
QT_MOC_LITERAL(2, 38, 0)
    },
    "NodeExternalContainer\0containerClosed\0"
    "\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NodeExternalContainer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void NodeExternalContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NodeExternalContainer *_t = static_cast<NodeExternalContainer *>(_o);
        switch (_id) {
        case 0: _t->containerClosed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NodeExternalContainer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NodeExternalContainer::containerClosed)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject NodeExternalContainer::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_NodeExternalContainer.data,
      qt_meta_data_NodeExternalContainer,  qt_static_metacall, 0, 0}
};


const QMetaObject *NodeExternalContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NodeExternalContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NodeExternalContainer.stringdata))
        return static_cast<void*>(const_cast< NodeExternalContainer*>(this));
    return QDialog::qt_metacast(_clname);
}

int NodeExternalContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void NodeExternalContainer::containerClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
struct qt_meta_stringdata_NodeInterfaceContainer_t {
    QByteArrayData data[7];
    char stringdata[102];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_NodeInterfaceContainer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_NodeInterfaceContainer_t qt_meta_stringdata_NodeInterfaceContainer = {
    {
QT_MOC_LITERAL(0, 0, 22),
QT_MOC_LITERAL(1, 23, 15),
QT_MOC_LITERAL(2, 39, 0),
QT_MOC_LITERAL(3, 40, 8),
QT_MOC_LITERAL(4, 49, 11),
QT_MOC_LITERAL(5, 61, 18),
QT_MOC_LITERAL(6, 80, 20)
    },
    "NodeInterfaceContainer\0setCurrentIndex\0"
    "\0showHelp\0closeWidget\0showExternalWidget\0"
    "externalWidgetClosed\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NodeInterfaceContainer[] = {

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
       1,    1,   39,    2, 0x0a,
       3,    2,   42,    2, 0x08,
       4,    1,   47,    2, 0x08,
       5,    1,   50,    2, 0x08,
       6,    0,   53,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QPoint,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void NodeInterfaceContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NodeInterfaceContainer *_t = static_cast<NodeInterfaceContainer *>(_o);
        switch (_id) {
        case 0: _t->setCurrentIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->showHelp((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 2: _t->closeWidget((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->showExternalWidget((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->externalWidgetClosed(); break;
        default: ;
        }
    }
}

const QMetaObject NodeInterfaceContainer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_NodeInterfaceContainer.data,
      qt_meta_data_NodeInterfaceContainer,  qt_static_metacall, 0, 0}
};


const QMetaObject *NodeInterfaceContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NodeInterfaceContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NodeInterfaceContainer.stringdata))
        return static_cast<void*>(const_cast< NodeInterfaceContainer*>(this));
    return QWidget::qt_metacast(_clname);
}

int NodeInterfaceContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
