/****************************************************************************
** Meta object code from reading C++ file 'hardwarecontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hardwarecontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hardwarecontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HardwareController_t {
    QByteArrayData data[18];
    char stringdata0[301];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HardwareController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HardwareController_t qt_meta_stringdata_HardwareController = {
    {
QT_MOC_LITERAL(0, 0, 18), // "HardwareController"
QT_MOC_LITERAL(1, 19, 19), // "fingerprintCaptured"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 6), // "userId"
QT_MOC_LITERAL(4, 47, 16), // "fingerprintError"
QT_MOC_LITERAL(5, 64, 5), // "error"
QT_MOC_LITERAL(6, 70, 4), // "data"
QT_MOC_LITERAL(7, 75, 20), // "servoPositionChanged"
QT_MOC_LITERAL(8, 96, 5), // "angle"
QT_MOC_LITERAL(9, 102, 21), // "doorOperationComplete"
QT_MOC_LITERAL(10, 124, 19), // "onFingerprintResult"
QT_MOC_LITERAL(11, 144, 24), // "onFingerprintDeviceError"
QT_MOC_LITERAL(12, 169, 26), // "onFingerprintCaptureResult"
QT_MOC_LITERAL(13, 196, 25), // "onFingerprintCaptureError"
QT_MOC_LITERAL(14, 222, 19), // "onServoAngleChanged"
QT_MOC_LITERAL(15, 242, 24), // "onServoOperationComplete"
QT_MOC_LITERAL(16, 267, 18), // "onServoDeviceError"
QT_MOC_LITERAL(17, 286, 14) // "onServoTimeout"

    },
    "HardwareController\0fingerprintCaptured\0"
    "\0userId\0fingerprintError\0error\0data\0"
    "servoPositionChanged\0angle\0"
    "doorOperationComplete\0onFingerprintResult\0"
    "onFingerprintDeviceError\0"
    "onFingerprintCaptureResult\0"
    "onFingerprintCaptureError\0onServoAngleChanged\0"
    "onServoOperationComplete\0onServoDeviceError\0"
    "onServoTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HardwareController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       1,    1,   85,    2, 0x06 /* Public */,
       7,    1,   88,    2, 0x06 /* Public */,
       9,    0,   91,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   92,    2, 0x08 /* Private */,
      11,    1,   95,    2, 0x08 /* Private */,
      12,    1,   98,    2, 0x08 /* Private */,
      13,    1,  101,    2, 0x08 /* Private */,
      14,    1,  104,    2, 0x08 /* Private */,
      15,    0,  107,    2, 0x08 /* Private */,
      16,    1,  108,    2, 0x08 /* Private */,
      17,    0,  111,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,

       0        // eod
};

void HardwareController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HardwareController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fingerprintCaptured((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->fingerprintError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->fingerprintCaptured((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->servoPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->doorOperationComplete(); break;
        case 5: _t->onFingerprintResult((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onFingerprintDeviceError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->onFingerprintCaptureResult((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 8: _t->onFingerprintCaptureError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->onServoAngleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->onServoOperationComplete(); break;
        case 11: _t->onServoDeviceError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->onServoTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HardwareController::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HardwareController::fingerprintCaptured)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HardwareController::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HardwareController::fingerprintError)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HardwareController::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HardwareController::fingerprintCaptured)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (HardwareController::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HardwareController::servoPositionChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (HardwareController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HardwareController::doorOperationComplete)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HardwareController::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_HardwareController.data,
    qt_meta_data_HardwareController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HardwareController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HardwareController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HardwareController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HardwareController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void HardwareController::fingerprintCaptured(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HardwareController::fingerprintError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HardwareController::fingerprintCaptured(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HardwareController::servoPositionChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void HardwareController::doorOperationComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
