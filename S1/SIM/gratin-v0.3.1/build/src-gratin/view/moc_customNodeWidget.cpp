/****************************************************************************
** Meta object code from reading C++ file 'customNodeWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src-gratin/view/customNodeWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customNodeWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CustomNodeWidget_t {
    QByteArrayData data[4];
    char stringdata[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CustomNodeWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CustomNodeWidget_t qt_meta_stringdata_CustomNodeWidget = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 14),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 13)
    },
    "CustomNodeWidget\0confirmClicked\0\0"
    "cancelClicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CustomNodeWidget[] = {

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
       1,    0,   24,    2, 0x0a,
       3,    0,   25,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CustomNodeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CustomNodeWidget *_t = static_cast<CustomNodeWidget *>(_o);
        switch (_id) {
        case 0: _t->confirmClicked(); break;
        case 1: _t->cancelClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CustomNodeWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CustomNodeWidget.data,
      qt_meta_data_CustomNodeWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *CustomNodeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CustomNodeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CustomNodeWidget.stringdata))
        return static_cast<void*>(const_cast< CustomNodeWidget*>(this));
    return QDialog::qt_metacast(_clname);
}

int CustomNodeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_ManagePathsWidget_t {
    QByteArrayData data[7];
    char stringdata[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ManagePathsWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ManagePathsWidget_t qt_meta_stringdata_ManagePathsWidget = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 14),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 13),
QT_MOC_LITERAL(4, 48, 10),
QT_MOC_LITERAL(5, 59, 11),
QT_MOC_LITERAL(6, 71, 10)
    },
    "ManagePathsWidget\0confirmClicked\0\0"
    "cancelClicked\0addClicked\0editClicked\0"
    "delClicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ManagePathsWidget[] = {

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
       1,    0,   39,    2, 0x0a,
       3,    0,   40,    2, 0x0a,
       4,    0,   41,    2, 0x08,
       5,    0,   42,    2, 0x08,
       6,    0,   43,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ManagePathsWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ManagePathsWidget *_t = static_cast<ManagePathsWidget *>(_o);
        switch (_id) {
        case 0: _t->confirmClicked(); break;
        case 1: _t->cancelClicked(); break;
        case 2: _t->addClicked(); break;
        case 3: _t->editClicked(); break;
        case 4: _t->delClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ManagePathsWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ManagePathsWidget.data,
      qt_meta_data_ManagePathsWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *ManagePathsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ManagePathsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ManagePathsWidget.stringdata))
        return static_cast<void*>(const_cast< ManagePathsWidget*>(this));
    return QDialog::qt_metacast(_clname);
}

int ManagePathsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
