/****************************************************************************
** Meta object code from reading C++ file 'homepage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "homepage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'homepage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HomePage_t {
    QByteArrayData data[11];
    char stringdata0[221];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HomePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HomePage_t qt_meta_stringdata_HomePage = {
    {
QT_MOC_LITERAL(0, 0, 8), // "HomePage"
QT_MOC_LITERAL(1, 9, 22), // "faceRecognitionClicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 21), // "passwordUnlockClicked"
QT_MOC_LITERAL(4, 55, 24), // "fingerprintUnlockClicked"
QT_MOC_LITERAL(5, 80, 23), // "userRegistrationClicked"
QT_MOC_LITERAL(6, 104, 14), // "updateDateTime"
QT_MOC_LITERAL(7, 119, 24), // "onFaceRecognitionClicked"
QT_MOC_LITERAL(8, 144, 23), // "onPasswordUnlockClicked"
QT_MOC_LITERAL(9, 168, 26), // "onFingerprintUnlockClicked"
QT_MOC_LITERAL(10, 195, 25) // "onUserRegistrationClicked"

    },
    "HomePage\0faceRecognitionClicked\0\0"
    "passwordUnlockClicked\0fingerprintUnlockClicked\0"
    "userRegistrationClicked\0updateDateTime\0"
    "onFaceRecognitionClicked\0"
    "onPasswordUnlockClicked\0"
    "onFingerprintUnlockClicked\0"
    "onUserRegistrationClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HomePage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    0,   61,    2, 0x06 /* Public */,
       5,    0,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HomePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HomePage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->faceRecognitionClicked(); break;
        case 1: _t->passwordUnlockClicked(); break;
        case 2: _t->fingerprintUnlockClicked(); break;
        case 3: _t->userRegistrationClicked(); break;
        case 4: _t->updateDateTime(); break;
        case 5: _t->onFaceRecognitionClicked(); break;
        case 6: _t->onPasswordUnlockClicked(); break;
        case 7: _t->onFingerprintUnlockClicked(); break;
        case 8: _t->onUserRegistrationClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HomePage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomePage::faceRecognitionClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HomePage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomePage::passwordUnlockClicked)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HomePage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomePage::fingerprintUnlockClicked)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (HomePage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HomePage::userRegistrationClicked)) {
                *result = 3;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject HomePage::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_HomePage.data,
    qt_meta_data_HomePage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HomePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HomePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HomePage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HomePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void HomePage::faceRecognitionClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HomePage::passwordUnlockClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HomePage::fingerprintUnlockClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void HomePage::userRegistrationClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
