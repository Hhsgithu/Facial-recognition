/****************************************************************************
** Meta object code from reading C++ file 'facerecognition.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "facerecognition.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'facerecognition.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FaceRecognition_t {
    QByteArrayData data[13];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FaceRecognition_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FaceRecognition_t qt_meta_stringdata_FaceRecognition = {
    {
QT_MOC_LITERAL(0, 0, 15), // "FaceRecognition"
QT_MOC_LITERAL(1, 16, 17), // "backToHomeClicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 18), // "recognitionSuccess"
QT_MOC_LITERAL(4, 54, 6), // "userId"
QT_MOC_LITERAL(5, 61, 8), // "userName"
QT_MOC_LITERAL(6, 70, 19), // "onBackButtonClicked"
QT_MOC_LITERAL(7, 90, 9), // "recv_data"
QT_MOC_LITERAL(8, 100, 10), // "resetState"
QT_MOC_LITERAL(9, 111, 9), // "onTimeout"
QT_MOC_LITERAL(10, 121, 10), // "tryConnect"
QT_MOC_LITERAL(11, 132, 11), // "onConnected"
QT_MOC_LITERAL(12, 144, 14) // "onDisconnected"

    },
    "FaceRecognition\0backToHomeClicked\0\0"
    "recognitionSuccess\0userId\0userName\0"
    "onBackButtonClicked\0recv_data\0resetState\0"
    "onTimeout\0tryConnect\0onConnected\0"
    "onDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FaceRecognition[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    2,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   65,    2, 0x08 /* Private */,
       7,    0,   66,    2, 0x08 /* Private */,
       8,    0,   67,    2, 0x08 /* Private */,
       9,    0,   68,    2, 0x08 /* Private */,
      10,    0,   69,    2, 0x08 /* Private */,
      11,    0,   70,    2, 0x08 /* Private */,
      12,    0,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FaceRecognition::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FaceRecognition *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->backToHomeClicked(); break;
        case 1: _t->recognitionSuccess((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->onBackButtonClicked(); break;
        case 3: _t->recv_data(); break;
        case 4: _t->resetState(); break;
        case 5: _t->onTimeout(); break;
        case 6: _t->tryConnect(); break;
        case 7: _t->onConnected(); break;
        case 8: _t->onDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FaceRecognition::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FaceRecognition::backToHomeClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FaceRecognition::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FaceRecognition::recognitionSuccess)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FaceRecognition::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_FaceRecognition.data,
    qt_meta_data_FaceRecognition,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FaceRecognition::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FaceRecognition::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FaceRecognition.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FaceRecognition::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void FaceRecognition::backToHomeClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FaceRecognition::recognitionSuccess(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
