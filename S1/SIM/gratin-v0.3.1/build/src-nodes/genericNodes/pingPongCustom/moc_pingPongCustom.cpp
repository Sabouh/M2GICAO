/****************************************************************************
** Meta object code from reading C++ file 'pingPongCustom.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src-nodes/genericNodes/pingPongCustom/pingPongCustom.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pingPongCustom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PingPongCustomWidget_t {
    QByteArrayData data[4];
    char stringdata[56];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_PingPongCustomWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_PingPongCustomWidget_t qt_meta_stringdata_PingPongCustomWidget = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 11),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 20)
    },
    "PingPongCustomWidget\0initClicked\0\0"
    "applyPingPongClicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PingPongCustomWidget[] = {

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
       1,    0,   24,    2, 0x09,
       3,    0,   25,    2, 0x09,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PingPongCustomWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PingPongCustomWidget *_t = static_cast<PingPongCustomWidget *>(_o);
        switch (_id) {
        case 0: _t->initClicked(); break;
        case 1: _t->applyPingPongClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject PingPongCustomWidget::staticMetaObject = {
    { &GenericCustomWidget::staticMetaObject, qt_meta_stringdata_PingPongCustomWidget.data,
      qt_meta_data_PingPongCustomWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *PingPongCustomWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PingPongCustomWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PingPongCustomWidget.stringdata))
        return static_cast<void*>(const_cast< PingPongCustomWidget*>(this));
    return GenericCustomWidget::qt_metacast(_clname);
}

int PingPongCustomWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GenericCustomWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_PingPongCustomHandle_t {
    QByteArrayData data[1];
    char stringdata[22];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_PingPongCustomHandle_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_PingPongCustomHandle_t qt_meta_stringdata_PingPongCustomHandle = {
    {
QT_MOC_LITERAL(0, 0, 20)
    },
    "PingPongCustomHandle\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PingPongCustomHandle[] = {

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

void PingPongCustomHandle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject PingPongCustomHandle::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PingPongCustomHandle.data,
      qt_meta_data_PingPongCustomHandle,  qt_static_metacall, 0, 0}
};


const QMetaObject *PingPongCustomHandle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PingPongCustomHandle::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PingPongCustomHandle.stringdata))
        return static_cast<void*>(const_cast< PingPongCustomHandle*>(this));
    if (!strcmp(_clname, "NodeHandleInterface"))
        return static_cast< NodeHandleInterface*>(const_cast< PingPongCustomHandle*>(this));
    if (!strcmp(_clname, "Gratin.NodeHandle/1.0"))
        return static_cast< NodeHandleInterface*>(const_cast< PingPongCustomHandle*>(this));
    return QObject::qt_metacast(_clname);
}

int PingPongCustomHandle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}

QT_PLUGIN_METADATA_SECTION const uint qt_section_alignment_dummy = 42;

#ifdef QT_NO_DEBUG

QT_PLUGIN_METADATA_SECTION
static const unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', ' ',
    0x71, 0x62, 0x6a, 0x73, 0x01, 0x00, 0x00, 0x00,
    0xa4, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x90, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 0x49, 0x49, 0x44, 0x00, 0x00, 0x00,
    0x15, 0x00, 0x47, 0x72, 0x61, 0x74, 0x69, 0x6e,
    0x2e, 0x50, 0x69, 0x6e, 0x67, 0x50, 0x6f, 0x6e,
    0x67, 0x43, 0x75, 0x73, 0x74, 0x6f, 0x6d, 0x00,
    0x1b, 0x08, 0x00, 0x00, 0x09, 0x00, 0x63, 0x6c,
    0x61, 0x73, 0x73, 0x4e, 0x61, 0x6d, 0x65, 0x00,
    0x14, 0x00, 0x50, 0x69, 0x6e, 0x67, 0x50, 0x6f,
    0x6e, 0x67, 0x43, 0x75, 0x73, 0x74, 0x6f, 0x6d,
    0x48, 0x61, 0x6e, 0x64, 0x6c, 0x65, 0x00, 0x00,
    0x3a, 0x40, 0xa0, 0x00, 0x07, 0x00, 0x76, 0x65,
    0x72, 0x73, 0x69, 0x6f, 0x6e, 0x00, 0x00, 0x00,
    0x11, 0x00, 0x00, 0x00, 0x05, 0x00, 0x64, 0x65,
    0x62, 0x75, 0x67, 0x00, 0x95, 0x10, 0x00, 0x00,
    0x08, 0x00, 0x4d, 0x65, 0x74, 0x61, 0x44, 0x61,
    0x74, 0x61, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00,
    0x30, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00,
    0x58, 0x00, 0x00, 0x00
};

#else // QT_NO_DEBUG

QT_PLUGIN_METADATA_SECTION
static const unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', ' ',
    0x71, 0x62, 0x6a, 0x73, 0x01, 0x00, 0x00, 0x00,
    0xa4, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x90, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 0x49, 0x49, 0x44, 0x00, 0x00, 0x00,
    0x15, 0x00, 0x47, 0x72, 0x61, 0x74, 0x69, 0x6e,
    0x2e, 0x50, 0x69, 0x6e, 0x67, 0x50, 0x6f, 0x6e,
    0x67, 0x43, 0x75, 0x73, 0x74, 0x6f, 0x6d, 0x00,
    0x15, 0x08, 0x00, 0x00, 0x08, 0x00, 0x4d, 0x65,
    0x74, 0x61, 0x44, 0x61, 0x74, 0x61, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x9b, 0x0b, 0x00, 0x00,
    0x09, 0x00, 0x63, 0x6c, 0x61, 0x73, 0x73, 0x4e,
    0x61, 0x6d, 0x65, 0x00, 0x14, 0x00, 0x50, 0x69,
    0x6e, 0x67, 0x50, 0x6f, 0x6e, 0x67, 0x43, 0x75,
    0x73, 0x74, 0x6f, 0x6d, 0x48, 0x61, 0x6e, 0x64,
    0x6c, 0x65, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x64, 0x65, 0x62, 0x75, 0x67, 0x00,
    0x3a, 0x40, 0xa0, 0x00, 0x07, 0x00, 0x76, 0x65,
    0x72, 0x73, 0x69, 0x6f, 0x6e, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
    0x4c, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00
};
#endif // QT_NO_DEBUG

QT_MOC_EXPORT_PLUGIN(PingPongCustomHandle, PingPongCustomHandle)

QT_END_MOC_NAMESPACE
