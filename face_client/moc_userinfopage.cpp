/****************************************************************************
** Meta object code from reading C++ file 'userinfopage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "userinfopage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userinfopage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UserInfoPage_t {
    QByteArrayData data[9];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UserInfoPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UserInfoPage_t qt_meta_stringdata_UserInfoPage = {
    {
QT_MOC_LITERAL(0, 0, 12), // "UserInfoPage"
QT_MOC_LITERAL(1, 13, 17), // "backToHomeClicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 17), // "userInfoCompleted"
QT_MOC_LITERAL(4, 50, 8), // "UserInfo"
QT_MOC_LITERAL(5, 59, 8), // "userInfo"
QT_MOC_LITERAL(6, 68, 13), // "onNextClicked"
QT_MOC_LITERAL(7, 82, 13), // "onBackClicked"
QT_MOC_LITERAL(8, 96, 14) // "onInputChanged"

    },
    "UserInfoPage\0backToHomeClicked\0\0"
    "userInfoCompleted\0UserInfo\0userInfo\0"
    "onNextClicked\0onBackClicked\0onInputChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserInfoPage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   43,    2, 0x08 /* Private */,
       7,    0,   44,    2, 0x08 /* Private */,
       8,    0,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UserInfoPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UserInfoPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->backToHomeClicked(); break;
        case 1: _t->userInfoCompleted((*reinterpret_cast< const UserInfo(*)>(_a[1]))); break;
        case 2: _t->onNextClicked(); break;
        case 3: _t->onBackClicked(); break;
        case 4: _t->onInputChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UserInfoPage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserInfoPage::backToHomeClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UserInfoPage::*)(const UserInfo & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserInfoPage::userInfoCompleted)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UserInfoPage::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_UserInfoPage.data,
    qt_meta_data_UserInfoPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UserInfoPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserInfoPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UserInfoPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int UserInfoPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void UserInfoPage::backToHomeClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void UserInfoPage::userInfoCompleted(const UserInfo & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
