/****************************************************************************
** Meta object code from reading C++ file 'genericSettings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../src-nodes/genericNodes/customNodeFiles/genericSettings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'genericSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GenericCustomIOItemButtons_t {
    QByteArrayData data[1];
    char stringdata[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_GenericCustomIOItemButtons_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_GenericCustomIOItemButtons_t qt_meta_stringdata_GenericCustomIOItemButtons = {
    {
QT_MOC_LITERAL(0, 0, 26)
    },
    "GenericCustomIOItemButtons\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GenericCustomIOItemButtons[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void GenericCustomIOItemButtons::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject GenericCustomIOItemButtons::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GenericCustomIOItemButtons.data,
      qt_meta_data_GenericCustomIOItemButtons,  qt_static_metacall, 0, 0}
};


const QMetaObject *GenericCustomIOItemButtons::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GenericCustomIOItemButtons::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GenericCustomIOItemButtons.stringdata))
        return static_cast<void*>(const_cast< GenericCustomIOItemButtons*>(this));
    return QWidget::qt_metacast(_clname);
}

int GenericCustomIOItemButtons::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_GenericCustomIOItem_t {
    QByteArrayData data[9];
    char stringdata[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_GenericCustomIOItem_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_GenericCustomIOItem_t qt_meta_stringdata_GenericCustomIOItem = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 9),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 20),
QT_MOC_LITERAL(4, 52, 11),
QT_MOC_LITERAL(5, 64, 10),
QT_MOC_LITERAL(6, 75, 12),
QT_MOC_LITERAL(7, 88, 14),
QT_MOC_LITERAL(8, 103, 13)
    },
    "GenericCustomIOItem\0upClicked\0\0"
    "GenericCustomIOItem*\0downClicked\0"
    "delClicked\0needUpSignal\0needDownSignal\0"
    "needDelSignal\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GenericCustomIOItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06,
       4,    1,   47,    2, 0x06,
       5,    1,   50,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       6,    0,   53,    2, 0x08,
       7,    0,   54,    2, 0x08,
       8,    0,   55,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GenericCustomIOItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GenericCustomIOItem *_t = static_cast<GenericCustomIOItem *>(_o);
        switch (_id) {
        case 0: _t->upClicked((*reinterpret_cast< GenericCustomIOItem*(*)>(_a[1]))); break;
        case 1: _t->downClicked((*reinterpret_cast< GenericCustomIOItem*(*)>(_a[1]))); break;
        case 2: _t->delClicked((*reinterpret_cast< GenericCustomIOItem*(*)>(_a[1]))); break;
        case 3: _t->needUpSignal(); break;
        case 4: _t->needDownSignal(); break;
        case 5: _t->needDelSignal(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomIOItem* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomIOItem* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomIOItem* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GenericCustomIOItem::*_t)(GenericCustomIOItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GenericCustomIOItem::upClicked)) {
                *result = 0;
            }
        }
        {
            typedef void (GenericCustomIOItem::*_t)(GenericCustomIOItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GenericCustomIOItem::downClicked)) {
                *result = 1;
            }
        }
        {
            typedef void (GenericCustomIOItem::*_t)(GenericCustomIOItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GenericCustomIOItem::delClicked)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject GenericCustomIOItem::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GenericCustomIOItem.data,
      qt_meta_data_GenericCustomIOItem,  qt_static_metacall, 0, 0}
};


const QMetaObject *GenericCustomIOItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GenericCustomIOItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GenericCustomIOItem.stringdata))
        return static_cast<void*>(const_cast< GenericCustomIOItem*>(this));
    return QWidget::qt_metacast(_clname);
}

int GenericCustomIOItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void GenericCustomIOItem::upClicked(GenericCustomIOItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GenericCustomIOItem::downClicked(GenericCustomIOItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GenericCustomIOItem::delClicked(GenericCustomIOItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_GenericCustomParamItem_t {
    QByteArrayData data[10];
    char stringdata[136];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_GenericCustomParamItem_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_GenericCustomParamItem_t qt_meta_stringdata_GenericCustomParamItem = {
    {
QT_MOC_LITERAL(0, 0, 22),
QT_MOC_LITERAL(1, 23, 9),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 23),
QT_MOC_LITERAL(4, 58, 11),
QT_MOC_LITERAL(5, 70, 10),
QT_MOC_LITERAL(6, 81, 12),
QT_MOC_LITERAL(7, 94, 14),
QT_MOC_LITERAL(8, 109, 13),
QT_MOC_LITERAL(9, 123, 11)
    },
    "GenericCustomParamItem\0upClicked\0\0"
    "GenericCustomParamItem*\0downClicked\0"
    "delClicked\0needUpSignal\0needDownSignal\0"
    "needDelSignal\0typeChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GenericCustomParamItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06,
       4,    1,   52,    2, 0x06,
       5,    1,   55,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       6,    0,   58,    2, 0x08,
       7,    0,   59,    2, 0x08,
       8,    0,   60,    2, 0x08,
       9,    1,   61,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void GenericCustomParamItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GenericCustomParamItem *_t = static_cast<GenericCustomParamItem *>(_o);
        switch (_id) {
        case 0: _t->upClicked((*reinterpret_cast< GenericCustomParamItem*(*)>(_a[1]))); break;
        case 1: _t->downClicked((*reinterpret_cast< GenericCustomParamItem*(*)>(_a[1]))); break;
        case 2: _t->delClicked((*reinterpret_cast< GenericCustomParamItem*(*)>(_a[1]))); break;
        case 3: _t->needUpSignal(); break;
        case 4: _t->needDownSignal(); break;
        case 5: _t->needDelSignal(); break;
        case 6: _t->typeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomParamItem* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomParamItem* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomParamItem* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GenericCustomParamItem::*_t)(GenericCustomParamItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GenericCustomParamItem::upClicked)) {
                *result = 0;
            }
        }
        {
            typedef void (GenericCustomParamItem::*_t)(GenericCustomParamItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GenericCustomParamItem::downClicked)) {
                *result = 1;
            }
        }
        {
            typedef void (GenericCustomParamItem::*_t)(GenericCustomParamItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GenericCustomParamItem::delClicked)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject GenericCustomParamItem::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GenericCustomParamItem.data,
      qt_meta_data_GenericCustomParamItem,  qt_static_metacall, 0, 0}
};


const QMetaObject *GenericCustomParamItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GenericCustomParamItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GenericCustomParamItem.stringdata))
        return static_cast<void*>(const_cast< GenericCustomParamItem*>(this));
    return QWidget::qt_metacast(_clname);
}

int GenericCustomParamItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void GenericCustomParamItem::upClicked(GenericCustomParamItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GenericCustomParamItem::downClicked(GenericCustomParamItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GenericCustomParamItem::delClicked(GenericCustomParamItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_IOList_t {
    QByteArrayData data[7];
    char stringdata[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_IOList_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_IOList_t qt_meta_stringdata_IOList = {
    {
QT_MOC_LITERAL(0, 0, 6),
QT_MOC_LITERAL(1, 7, 6),
QT_MOC_LITERAL(2, 14, 0),
QT_MOC_LITERAL(3, 15, 20),
QT_MOC_LITERAL(4, 36, 8),
QT_MOC_LITERAL(5, 45, 7),
QT_MOC_LITERAL(6, 53, 10)
    },
    "IOList\0itemUp\0\0GenericCustomIOItem*\0"
    "itemDown\0itemDel\0addClicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IOList[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08,
       4,    1,   37,    2, 0x08,
       5,    1,   40,    2, 0x08,
       6,    0,   43,    2, 0x08,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,

       0        // eod
};

void IOList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IOList *_t = static_cast<IOList *>(_o);
        switch (_id) {
        case 0: _t->itemUp((*reinterpret_cast< GenericCustomIOItem*(*)>(_a[1]))); break;
        case 1: _t->itemDown((*reinterpret_cast< GenericCustomIOItem*(*)>(_a[1]))); break;
        case 2: _t->itemDel((*reinterpret_cast< GenericCustomIOItem*(*)>(_a[1]))); break;
        case 3: _t->addClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomIOItem* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomIOItem* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomIOItem* >(); break;
            }
            break;
        }
    }
}

const QMetaObject IOList::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_IOList.data,
      qt_meta_data_IOList,  qt_static_metacall, 0, 0}
};


const QMetaObject *IOList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IOList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IOList.stringdata))
        return static_cast<void*>(const_cast< IOList*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int IOList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
struct qt_meta_stringdata_ParamList_t {
    QByteArrayData data[7];
    char stringdata[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ParamList_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ParamList_t qt_meta_stringdata_ParamList = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 6),
QT_MOC_LITERAL(2, 17, 0),
QT_MOC_LITERAL(3, 18, 23),
QT_MOC_LITERAL(4, 42, 8),
QT_MOC_LITERAL(5, 51, 7),
QT_MOC_LITERAL(6, 59, 10)
    },
    "ParamList\0itemUp\0\0GenericCustomParamItem*\0"
    "itemDown\0itemDel\0addClicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ParamList[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08,
       4,    1,   37,    2, 0x08,
       5,    1,   40,    2, 0x08,
       6,    0,   43,    2, 0x08,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,

       0        // eod
};

void ParamList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ParamList *_t = static_cast<ParamList *>(_o);
        switch (_id) {
        case 0: _t->itemUp((*reinterpret_cast< GenericCustomParamItem*(*)>(_a[1]))); break;
        case 1: _t->itemDown((*reinterpret_cast< GenericCustomParamItem*(*)>(_a[1]))); break;
        case 2: _t->itemDel((*reinterpret_cast< GenericCustomParamItem*(*)>(_a[1]))); break;
        case 3: _t->addClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomParamItem* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomParamItem* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GenericCustomParamItem* >(); break;
            }
            break;
        }
    }
}

const QMetaObject ParamList::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_ParamList.data,
      qt_meta_data_ParamList,  qt_static_metacall, 0, 0}
};


const QMetaObject *ParamList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParamList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ParamList.stringdata))
        return static_cast<void*>(const_cast< ParamList*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int ParamList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
struct qt_meta_stringdata_PropertyList_t {
    QByteArrayData data[3];
    char stringdata[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_PropertyList_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_PropertyList_t qt_meta_stringdata_PropertyList = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 12),
QT_MOC_LITERAL(2, 26, 0)
    },
    "PropertyList\0colorClicked\0\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PropertyList[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void PropertyList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PropertyList *_t = static_cast<PropertyList *>(_o);
        switch (_id) {
        case 0: _t->colorClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject PropertyList::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_PropertyList.data,
      qt_meta_data_PropertyList,  qt_static_metacall, 0, 0}
};


const QMetaObject *PropertyList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PropertyList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PropertyList.stringdata))
        return static_cast<void*>(const_cast< PropertyList*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int PropertyList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_GenericCustomWidgetSetting_t {
    QByteArrayData data[4];
    char stringdata[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_GenericCustomWidgetSetting_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_GenericCustomWidgetSetting_t qt_meta_stringdata_GenericCustomWidgetSetting = {
    {
QT_MOC_LITERAL(0, 0, 26),
QT_MOC_LITERAL(1, 27, 14),
QT_MOC_LITERAL(2, 42, 0),
QT_MOC_LITERAL(3, 43, 13)
    },
    "GenericCustomWidgetSetting\0confirmClicked\0"
    "\0cancelClicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GenericCustomWidgetSetting[] = {

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
       1,    0,   24,    2, 0x08,
       3,    0,   25,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GenericCustomWidgetSetting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GenericCustomWidgetSetting *_t = static_cast<GenericCustomWidgetSetting *>(_o);
        switch (_id) {
        case 0: _t->confirmClicked(); break;
        case 1: _t->cancelClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject GenericCustomWidgetSetting::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_GenericCustomWidgetSetting.data,
      qt_meta_data_GenericCustomWidgetSetting,  qt_static_metacall, 0, 0}
};


const QMetaObject *GenericCustomWidgetSetting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GenericCustomWidgetSetting::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GenericCustomWidgetSetting.stringdata))
        return static_cast<void*>(const_cast< GenericCustomWidgetSetting*>(this));
    return QDialog::qt_metacast(_clname);
}

int GenericCustomWidgetSetting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
