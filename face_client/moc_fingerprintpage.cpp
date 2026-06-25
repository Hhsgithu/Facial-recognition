/****************************************************************************
** Meta object code from reading C++ file 'fingerprintpage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "fingerprintpage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fingerprintpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FingerprintPage_t {
    QByteArrayData data[23];
    char stringdata0[337];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FingerprintPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FingerprintPage_t qt_meta_stringdata_FingerprintPage = {
    {
QT_MOC_LITERAL(0, 0, 15), // "FingerprintPage"
QT_MOC_LITERAL(1, 16, 17), // "backToHomeClicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 13), // "verifySuccess"
QT_MOC_LITERAL(4, 49, 6), // "userId"
QT_MOC_LITERAL(5, 56, 8), // "userName"
QT_MOC_LITERAL(6, 65, 12), // "verifyFailed"
QT_MOC_LITERAL(7, 78, 7), // "message"
QT_MOC_LITERAL(8, 86, 19), // "startAuthentication"
QT_MOC_LITERAL(9, 106, 18), // "stopAuthentication"
QT_MOC_LITERAL(10, 125, 7), // "cleanup"
QT_MOC_LITERAL(11, 133, 19), // "onBackButtonClicked"
QT_MOC_LITERAL(12, 153, 23), // "onAuthenticationTimeout"
QT_MOC_LITERAL(13, 177, 21), // "onFingerprintCaptured"
QT_MOC_LITERAL(14, 199, 18), // "onFingerprintError"
QT_MOC_LITERAL(15, 218, 5), // "error"
QT_MOC_LITERAL(16, 224, 11), // "onConnected"
QT_MOC_LITERAL(17, 236, 14), // "onDisconnected"
QT_MOC_LITERAL(18, 251, 13), // "onNetworkData"
QT_MOC_LITERAL(19, 265, 13), // "onSocketError"
QT_MOC_LITERAL(20, 279, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(21, 308, 11), // "socketError"
QT_MOC_LITERAL(22, 320, 16) // "onReconnectTimer"

    },
    "FingerprintPage\0backToHomeClicked\0\0"
    "verifySuccess\0userId\0userName\0"
    "verifyFailed\0message\0startAuthentication\0"
    "stopAuthentication\0cleanup\0"
    "onBackButtonClicked\0onAuthenticationTimeout\0"
    "onFingerprintCaptured\0onFingerprintError\0"
    "error\0onConnected\0onDisconnected\0"
    "onNetworkData\0onSocketError\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "onReconnectTimer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FingerprintPage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x06 /* Public */,
       3,    2,   90,    2, 0x06 /* Public */,
       6,    1,   95,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   98,    2, 0x0a /* Public */,
       9,    0,   99,    2, 0x0a /* Public */,
      10,    0,  100,    2, 0x0a /* Public */,
      11,    0,  101,    2, 0x08 /* Private */,
      12,    0,  102,    2, 0x08 /* Private */,
      13,    1,  103,    2, 0x08 /* Private */,
      14,    1,  106,    2, 0x08 /* Private */,
      16,    0,  109,    2, 0x08 /* Private */,
      17,    0,  110,    2, 0x08 /* Private */,
      18,    0,  111,    2, 0x08 /* Private */,
      19,    1,  112,    2, 0x08 /* Private */,
      22,    0,  115,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    4,    5,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,

       0        // eod
};

void FingerprintPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FingerprintPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->backToHomeClicked(); break;
        case 1: _t->verifySuccess((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->verifyFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->startAuthentication(); break;
        case 4: _t->stopAuthentication(); break;
        case 5: _t->cleanup(); break;
        case 6: _t->onBackButtonClicked(); break;
        case 7: _t->onAuthenticationTimeout(); break;
        case 8: _t->onFingerprintCaptured((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->onFingerprintError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->onConnected(); break;
        case 11: _t->onDisconnected(); break;
        case 12: _t->onNetworkData(); break;
        case 13: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 14: _t->onReconnectTimer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FingerprintPage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FingerprintPage::backToHomeClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FingerprintPage::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FingerprintPage::verifySuccess)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FingerprintPage::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FingerprintPage::verifyFailed)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FingerprintPage::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_FingerprintPage.data,
    qt_meta_data_FingerprintPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FingerprintPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FingerprintPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FingerprintPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FingerprintPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void FingerprintPage::backToHomeClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FingerprintPage::verifySuccess(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FingerprintPage::verifyFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
