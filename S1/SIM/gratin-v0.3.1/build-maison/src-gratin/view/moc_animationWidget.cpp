/****************************************************************************
** Meta object code from reading C++ file 'animationWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src-gratin/view/animationWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'animationWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AnimationSettings_t {
    QByteArrayData data[4];
    char stringdata[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AnimationSettings_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AnimationSettings_t qt_meta_stringdata_AnimationSettings = {
    {
QT_MOC_LITERAL(0, 0, 17), // "AnimationSettings"
QT_MOC_LITERAL(1, 18, 14), // "confirmClicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 13) // "cancelClicked"

    },
    "AnimationSettings\0confirmClicked\0\0"
    "cancelClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AnimationSettings[] = {

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
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AnimationSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AnimationSettings *_t = static_cast<AnimationSettings *>(_o);
        switch (_id) {
        case 0: _t->confirmClicked(); break;
        case 1: _t->cancelClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject AnimationSettings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AnimationSettings.data,
      qt_meta_data_AnimationSettings,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AnimationSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnimationSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AnimationSettings.stringdata))
        return static_cast<void*>(const_cast< AnimationSettings*>(this));
    return QDialog::qt_metacast(_clname);
}

int AnimationSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_AnimationWidget_t {
    QByteArrayData data[8];
    char stringdata[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AnimationWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AnimationWidget_t qt_meta_stringdata_AnimationWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AnimationWidget"
QT_MOC_LITERAL(1, 16, 19), // "currentFrameChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 1), // "v"
QT_MOC_LITERAL(4, 39, 17), // "firstFrameClicked"
QT_MOC_LITERAL(5, 57, 16), // "lastFrameClicked"
QT_MOC_LITERAL(6, 74, 11), // "playClicked"
QT_MOC_LITERAL(7, 86, 12) // "frameChanged"

    },
    "AnimationWidget\0currentFrameChanged\0"
    "\0v\0firstFrameClicked\0lastFrameClicked\0"
    "playClicked\0frameChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AnimationWidget[] = {

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
       1,    1,   39,    2, 0x0a /* Public */,
       4,    0,   42,    2, 0x0a /* Public */,
       5,    0,   43,    2, 0x0a /* Public */,
       6,    0,   44,    2, 0x0a /* Public */,
       7,    0,   45,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AnimationWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AnimationWidget *_t = static_cast<AnimationWidget *>(_o);
        switch (_id) {
        case 0: _t->currentFrameChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->firstFrameClicked(); break;
        case 2: _t->lastFrameClicked(); break;
        case 3: _t->playClicked(); break;
        case 4: _t->frameChanged(); break;
        default: ;
        }
    }
}

const QMetaObject AnimationWidget::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_AnimationWidget.data,
      qt_meta_data_AnimationWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AnimationWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnimationWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AnimationWidget.stringdata))
        return static_cast<void*>(const_cast< AnimationWidget*>(this));
    if (!strcmp(_clname, "IOData"))
        return static_cast< IOData*>(const_cast< AnimationWidget*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int AnimationWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_ItemWidget_t {
    QByteArrayData data[10];
    char stringdata[129];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ItemWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ItemWidget_t qt_meta_stringdata_ItemWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ItemWidget"
QT_MOC_LITERAL(1, 11, 20), // "interpolationChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 19), // "leftBehaviorChanged"
QT_MOC_LITERAL(4, 53, 20), // "rightBehaviorChanged"
QT_MOC_LITERAL(5, 74, 11), // "hideClicked"
QT_MOC_LITERAL(6, 86, 12), // "cleanClicked"
QT_MOC_LITERAL(7, 99, 7), // "varName"
QT_MOC_LITERAL(8, 107, 10), // "setVarName"
QT_MOC_LITERAL(9, 118, 10) // "nodeNameId"

    },
    "ItemWidget\0interpolationChanged\0\0"
    "leftBehaviorChanged\0rightBehaviorChanged\0"
    "hideClicked\0cleanClicked\0varName\0"
    "setVarName\0nodeNameId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ItemWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       3,    1,   57,    2, 0x0a /* Public */,
       4,    1,   60,    2, 0x0a /* Public */,
       5,    0,   63,    2, 0x0a /* Public */,
       6,    0,   64,    2, 0x0a /* Public */,
       7,    0,   65,    2, 0x0a /* Public */,
       8,    1,   66,    2, 0x0a /* Public */,
       9,    0,   69,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,    7,
    QMetaType::QString,

       0        // eod
};

void ItemWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ItemWidget *_t = static_cast<ItemWidget *>(_o);
        switch (_id) {
        case 0: _t->interpolationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->leftBehaviorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->rightBehaviorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->hideClicked(); break;
        case 4: _t->cleanClicked(); break;
        case 5: _t->varName(); break;
        case 6: _t->setVarName((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 7: { QString _r = _t->nodeNameId();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject ItemWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ItemWidget.data,
      qt_meta_data_ItemWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ItemWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ItemWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ItemWidget.stringdata))
        return static_cast<void*>(const_cast< ItemWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ItemWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
