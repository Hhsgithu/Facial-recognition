/****************************************************************************
** Meta object code from reading C++ file 'passwordsetuppage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "passwordsetuppage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'passwordsetuppage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PasswordSetupPage_t {
    QByteArrayData data[10];
    char stringdata0[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PasswordSetupPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PasswordSetupPage_t qt_meta_stringdata_PasswordSetupPage = {
    {
QT_MOC_LITERAL(0, 0, 17), // "PasswordSetupPage"
QT_MOC_LITERAL(1, 18, 22), // "passwordSetupCompleted"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 8), // "password"
QT_MOC_LITERAL(4, 51, 11), // "backClicked"
QT_MOC_LITERAL(5, 63, 17), // "onConfirmPassword"
QT_MOC_LITERAL(6, 81, 13), // "onBackClicked"
QT_MOC_LITERAL(7, 95, 21), // "onNumberButtonClicked"
QT_MOC_LITERAL(8, 117, 6), // "number"
QT_MOC_LITERAL(9, 124, 21) // "onDeleteButtonClicked"

    },
    "PasswordSetupPage\0passwordSetupCompleted\0"
    "\0password\0backClicked\0onConfirmPassword\0"
    "onBackClicked\0onNumberButtonClicked\0"
    "number\0onDeleteButtonClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PasswordSetupPage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    0,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   48,    2, 0x08 /* Private */,
       6,    0,   49,    2, 0x08 /* Private */,
       7,    1,   50,    2, 0x08 /* Private */,
       9,    0,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

       0        // eod
};

void PasswordSetupPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PasswordSetupPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->passwordSetupCompleted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->backClicked(); break;
        case 2: _t->onConfirmPassword(); break;
        case 3: _t->onBackClicked(); break;
        case 4: _t->onNumberButtonClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->onDeleteButtonClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PasswordSetupPage::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PasswordSetupPage::passwordSetupCompleted)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PasswordSetupPage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PasswordSetupPage::backClicked)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PasswordSetupPage::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_PasswordSetupPage.data,
    qt_meta_data_PasswordSetupPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PasswordSetupPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PasswordSetupPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PasswordSetupPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PasswordSetupPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void PasswordSetupPage::passwordSetupCompleted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PasswordSetupPage::backClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
