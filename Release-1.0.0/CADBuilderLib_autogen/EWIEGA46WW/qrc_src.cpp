/****************************************************************************
** Resource object code
**
** Created by: The Resource Compiler for Qt version 5.12.9
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

static const unsigned char qt_resource_data[] = {
  // D:/WORK/CADBuilder/config.json
  0x0,0x0,0x0,0x80,
  0x7b,
  0xd,0xa,0x20,0x20,0x20,0x20,0x22,0x77,0x69,0x64,0x74,0x68,0x22,0x3a,0x20,0x34,
  0x30,0x2e,0x30,0x2c,0xd,0xa,0x20,0x20,0x20,0x20,0x22,0x68,0x65,0x69,0x67,0x68,
  0x74,0x22,0x3a,0x20,0x32,0x30,0x2e,0x30,0x2c,0xd,0xa,0x20,0x20,0x20,0x20,0x22,
  0x74,0x68,0x65,0x74,0x61,0x22,0x3a,0x20,0x32,0x30,0x2e,0x30,0x2c,0xd,0xa,0x20,
  0x20,0x20,0x20,0x22,0x65,0x78,0x74,0x72,0x75,0x64,0x65,0x5f,0x64,0x69,0x73,0x74,
  0x61,0x6e,0x63,0x65,0x22,0x3a,0x20,0x31,0x30,0x2e,0x30,0x2c,0xd,0xa,0x20,0x20,
  0x20,0x20,0x22,0x73,0x61,0x76,0x65,0x5f,0x70,0x61,0x74,0x68,0x22,0x3a,0x20,0x22,
  0x6f,0x75,0x74,0x70,0x75,0x74,0x2e,0x62,0x72,0x65,0x70,0x22,0xd,0xa,0x7d,
  
};

static const unsigned char qt_resource_name[] = {
  // config.json
  0x0,0xb,
  0xf,0x71,0xd3,0xde,
  0x0,0x63,
  0x0,0x6f,0x0,0x6e,0x0,0x66,0x0,0x69,0x0,0x67,0x0,0x2e,0x0,0x6a,0x0,0x73,0x0,0x6f,0x0,0x6e,
  
};

static const unsigned char qt_resource_struct[] = {
  // :
  0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  // :/config.json
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,
0x0,0x0,0x1,0x95,0xea,0xf7,0xbd,0xa6,

};

#ifdef QT_NAMESPACE
#  define QT_RCC_PREPEND_NAMESPACE(name) ::QT_NAMESPACE::name
#  define QT_RCC_MANGLE_NAMESPACE0(x) x
#  define QT_RCC_MANGLE_NAMESPACE1(a, b) a##_##b
#  define QT_RCC_MANGLE_NAMESPACE2(a, b) QT_RCC_MANGLE_NAMESPACE1(a,b)
#  define QT_RCC_MANGLE_NAMESPACE(name) QT_RCC_MANGLE_NAMESPACE2( \
        QT_RCC_MANGLE_NAMESPACE0(name), QT_RCC_MANGLE_NAMESPACE0(QT_NAMESPACE))
#else
#   define QT_RCC_PREPEND_NAMESPACE(name) name
#   define QT_RCC_MANGLE_NAMESPACE(name) name
#endif

#ifdef QT_NAMESPACE
namespace QT_NAMESPACE {
#endif

bool qRegisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);

bool qUnregisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);

#ifdef QT_NAMESPACE
}
#endif

int QT_RCC_MANGLE_NAMESPACE(qInitResources_src)();
int QT_RCC_MANGLE_NAMESPACE(qInitResources_src)()
{
    QT_RCC_PREPEND_NAMESPACE(qRegisterResourceData)
        (0x2, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

int QT_RCC_MANGLE_NAMESPACE(qCleanupResources_src)();
int QT_RCC_MANGLE_NAMESPACE(qCleanupResources_src)()
{
    QT_RCC_PREPEND_NAMESPACE(qUnregisterResourceData)
       (0x2, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

namespace {
   struct initializer {
       initializer() { QT_RCC_MANGLE_NAMESPACE(qInitResources_src)(); }
       ~initializer() { QT_RCC_MANGLE_NAMESPACE(qCleanupResources_src)(); }
   } dummy;
}
