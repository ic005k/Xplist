/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSClickableLabelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSClickableLabelENDCLASS = QtMocHelpers::stringData(
    "ClickableLabel",
    "clicked",
    ""
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSClickableLabelENDCLASS_t {
    uint offsetsAndSizes[6];
    char stringdata0[15];
    char stringdata1[8];
    char stringdata2[1];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSClickableLabelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSClickableLabelENDCLASS_t qt_meta_stringdata_CLASSClickableLabelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "ClickableLabel"
        QT_MOC_LITERAL(15, 7),  // "clicked"
        QT_MOC_LITERAL(23, 0)   // ""
    },
    "ClickableLabel",
    "clicked",
    ""
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSClickableLabelENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ClickableLabel::staticMetaObject = { {
    QMetaObject::SuperData::link<QLabel::staticMetaObject>(),
    qt_meta_stringdata_CLASSClickableLabelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSClickableLabelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSClickableLabelENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ClickableLabel, std::true_type>,
        // method 'clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ClickableLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClickableLabel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ClickableLabel::*)();
            if (_t _q_method = &ClickableLabel::clicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

const QMetaObject *ClickableLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClickableLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSClickableLabelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QLabel::qt_metacast(_clname);
}

int ClickableLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ClickableLabel::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "on_Find",
    "",
    "on_actionMoveUp",
    "on_actionMoveDown",
    "actionAdd_activated",
    "actionRemove_activated",
    "eventFilter",
    "QEvent*",
    "changeEvent",
    "event",
    "on_NewWindow",
    "recentOpen",
    "filename",
    "on_copyBW",
    "on_pasteBW",
    "actionNew",
    "actionClose_activated",
    "actionClose_all_activated",
    "actionSave",
    "actionSaveAs",
    "actionExpand_all_activated",
    "onTabWidget_currentChanged",
    "index",
    "menu_aboutToShow",
    "onCleanChanged",
    "clean",
    "openRecentFile",
    "on_TabCloseRequest",
    "i",
    "on_copyAction",
    "on_cutAction",
    "on_pasteAction",
    "on_expandAction",
    "on_collapseAction",
    "on_actionNewSibling",
    "on_actionNewChild",
    "replyFinished",
    "QNetworkReply*",
    "reply",
    "CheckUpdate",
    "on_actionSort",
    "on_actionShowPlistText_triggered",
    "checked",
    "on_actionPaste_as_child_triggered",
    "on_editFind_returnPressed",
    "on_editFind_textChanged",
    "arg1",
    "on_ShowFindReplace",
    "on_btnFind_clicked",
    "on_btnHideFind_clicked",
    "on_btnNext_clicked",
    "on_btnPrevious_clicked",
    "on_btnReplace_clicked",
    "on_btnReplaceAll_clicked",
    "on_actionFind_triggered",
    "on_actionFindNext_triggered",
    "on_actionFindPrevious_triggered",
    "on_actionReplace_triggered",
    "on_actionReplaceAll_triggered",
    "on_btnShowReplace_clicked",
    "on_actionCut_triggered",
    "on_actionCopy_between_windows_triggered",
    "on_actionPaste_between_windows_triggered",
    "on_actionCheck_Update_triggered",
    "on_actionAbout_triggered",
    "on_actionAdd_triggered",
    "on_actionNew_Sibling_triggered",
    "on_actionOpen_triggered",
    "on_actionNew_triggered",
    "on_actionSave_triggered",
    "on_actionSave_as_triggered",
    "on_listFind_itemClicked",
    "QListWidgetItem*",
    "item",
    "on_actionBug_Report_triggered",
    "on_actionQuit_triggered",
    "on_listFind_itemSelectionChanged",
    "on_actionFont_triggered",
    "on_listFind_currentRowChanged",
    "currentRow",
    "on_btnNo_clicked",
    "on_btnYes_clicked",
    "on_actionDownload_Upgrade_Packages_triggered",
    "on_btnUpdateASCII_clicked",
    "on_btnUpdateBase64_clicked",
    "on_btnUpdateHex_clicked",
    "on_actProxy1_triggered",
    "on_actProxy2_triggered",
    "on_actProxy3_triggered",
    "on_actProxy4_triggered",
    "on_actProxy5_triggered",
    "on_actionMove_Up_triggered",
    "on_actionMove_Down_triggered",
    "on_actionSort_triggered",
    "on_actionExpand_All_triggered",
    "on_actionCollapse_All_triggered",
    "on_btnFind_Tool_clicked",
    "on_btnReplaceFind_clicked",
    "on_btnNew_clicked",
    "on_btnSave_clicked",
    "onTabBarClicked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[204];
    char stringdata0[11];
    char stringdata1[8];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[18];
    char stringdata5[20];
    char stringdata6[23];
    char stringdata7[12];
    char stringdata8[8];
    char stringdata9[12];
    char stringdata10[6];
    char stringdata11[13];
    char stringdata12[11];
    char stringdata13[9];
    char stringdata14[10];
    char stringdata15[11];
    char stringdata16[10];
    char stringdata17[22];
    char stringdata18[26];
    char stringdata19[11];
    char stringdata20[13];
    char stringdata21[27];
    char stringdata22[27];
    char stringdata23[6];
    char stringdata24[17];
    char stringdata25[15];
    char stringdata26[6];
    char stringdata27[15];
    char stringdata28[19];
    char stringdata29[2];
    char stringdata30[14];
    char stringdata31[13];
    char stringdata32[15];
    char stringdata33[16];
    char stringdata34[18];
    char stringdata35[20];
    char stringdata36[18];
    char stringdata37[14];
    char stringdata38[15];
    char stringdata39[6];
    char stringdata40[12];
    char stringdata41[14];
    char stringdata42[33];
    char stringdata43[8];
    char stringdata44[34];
    char stringdata45[26];
    char stringdata46[24];
    char stringdata47[5];
    char stringdata48[19];
    char stringdata49[19];
    char stringdata50[23];
    char stringdata51[19];
    char stringdata52[23];
    char stringdata53[22];
    char stringdata54[25];
    char stringdata55[24];
    char stringdata56[28];
    char stringdata57[32];
    char stringdata58[27];
    char stringdata59[30];
    char stringdata60[26];
    char stringdata61[23];
    char stringdata62[40];
    char stringdata63[41];
    char stringdata64[32];
    char stringdata65[25];
    char stringdata66[23];
    char stringdata67[31];
    char stringdata68[24];
    char stringdata69[23];
    char stringdata70[24];
    char stringdata71[27];
    char stringdata72[24];
    char stringdata73[17];
    char stringdata74[5];
    char stringdata75[30];
    char stringdata76[24];
    char stringdata77[33];
    char stringdata78[24];
    char stringdata79[30];
    char stringdata80[11];
    char stringdata81[17];
    char stringdata82[18];
    char stringdata83[45];
    char stringdata84[26];
    char stringdata85[27];
    char stringdata86[24];
    char stringdata87[23];
    char stringdata88[23];
    char stringdata89[23];
    char stringdata90[23];
    char stringdata91[23];
    char stringdata92[27];
    char stringdata93[29];
    char stringdata94[24];
    char stringdata95[30];
    char stringdata96[32];
    char stringdata97[24];
    char stringdata98[26];
    char stringdata99[18];
    char stringdata100[19];
    char stringdata101[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 7),  // "on_Find"
        QT_MOC_LITERAL(19, 0),  // ""
        QT_MOC_LITERAL(20, 15),  // "on_actionMoveUp"
        QT_MOC_LITERAL(36, 17),  // "on_actionMoveDown"
        QT_MOC_LITERAL(54, 19),  // "actionAdd_activated"
        QT_MOC_LITERAL(74, 22),  // "actionRemove_activated"
        QT_MOC_LITERAL(97, 11),  // "eventFilter"
        QT_MOC_LITERAL(109, 7),  // "QEvent*"
        QT_MOC_LITERAL(117, 11),  // "changeEvent"
        QT_MOC_LITERAL(129, 5),  // "event"
        QT_MOC_LITERAL(135, 12),  // "on_NewWindow"
        QT_MOC_LITERAL(148, 10),  // "recentOpen"
        QT_MOC_LITERAL(159, 8),  // "filename"
        QT_MOC_LITERAL(168, 9),  // "on_copyBW"
        QT_MOC_LITERAL(178, 10),  // "on_pasteBW"
        QT_MOC_LITERAL(189, 9),  // "actionNew"
        QT_MOC_LITERAL(199, 21),  // "actionClose_activated"
        QT_MOC_LITERAL(221, 25),  // "actionClose_all_activated"
        QT_MOC_LITERAL(247, 10),  // "actionSave"
        QT_MOC_LITERAL(258, 12),  // "actionSaveAs"
        QT_MOC_LITERAL(271, 26),  // "actionExpand_all_activated"
        QT_MOC_LITERAL(298, 26),  // "onTabWidget_currentChanged"
        QT_MOC_LITERAL(325, 5),  // "index"
        QT_MOC_LITERAL(331, 16),  // "menu_aboutToShow"
        QT_MOC_LITERAL(348, 14),  // "onCleanChanged"
        QT_MOC_LITERAL(363, 5),  // "clean"
        QT_MOC_LITERAL(369, 14),  // "openRecentFile"
        QT_MOC_LITERAL(384, 18),  // "on_TabCloseRequest"
        QT_MOC_LITERAL(403, 1),  // "i"
        QT_MOC_LITERAL(405, 13),  // "on_copyAction"
        QT_MOC_LITERAL(419, 12),  // "on_cutAction"
        QT_MOC_LITERAL(432, 14),  // "on_pasteAction"
        QT_MOC_LITERAL(447, 15),  // "on_expandAction"
        QT_MOC_LITERAL(463, 17),  // "on_collapseAction"
        QT_MOC_LITERAL(481, 19),  // "on_actionNewSibling"
        QT_MOC_LITERAL(501, 17),  // "on_actionNewChild"
        QT_MOC_LITERAL(519, 13),  // "replyFinished"
        QT_MOC_LITERAL(533, 14),  // "QNetworkReply*"
        QT_MOC_LITERAL(548, 5),  // "reply"
        QT_MOC_LITERAL(554, 11),  // "CheckUpdate"
        QT_MOC_LITERAL(566, 13),  // "on_actionSort"
        QT_MOC_LITERAL(580, 32),  // "on_actionShowPlistText_triggered"
        QT_MOC_LITERAL(613, 7),  // "checked"
        QT_MOC_LITERAL(621, 33),  // "on_actionPaste_as_child_trigg..."
        QT_MOC_LITERAL(655, 25),  // "on_editFind_returnPressed"
        QT_MOC_LITERAL(681, 23),  // "on_editFind_textChanged"
        QT_MOC_LITERAL(705, 4),  // "arg1"
        QT_MOC_LITERAL(710, 18),  // "on_ShowFindReplace"
        QT_MOC_LITERAL(729, 18),  // "on_btnFind_clicked"
        QT_MOC_LITERAL(748, 22),  // "on_btnHideFind_clicked"
        QT_MOC_LITERAL(771, 18),  // "on_btnNext_clicked"
        QT_MOC_LITERAL(790, 22),  // "on_btnPrevious_clicked"
        QT_MOC_LITERAL(813, 21),  // "on_btnReplace_clicked"
        QT_MOC_LITERAL(835, 24),  // "on_btnReplaceAll_clicked"
        QT_MOC_LITERAL(860, 23),  // "on_actionFind_triggered"
        QT_MOC_LITERAL(884, 27),  // "on_actionFindNext_triggered"
        QT_MOC_LITERAL(912, 31),  // "on_actionFindPrevious_triggered"
        QT_MOC_LITERAL(944, 26),  // "on_actionReplace_triggered"
        QT_MOC_LITERAL(971, 29),  // "on_actionReplaceAll_triggered"
        QT_MOC_LITERAL(1001, 25),  // "on_btnShowReplace_clicked"
        QT_MOC_LITERAL(1027, 22),  // "on_actionCut_triggered"
        QT_MOC_LITERAL(1050, 39),  // "on_actionCopy_between_windows..."
        QT_MOC_LITERAL(1090, 40),  // "on_actionPaste_between_window..."
        QT_MOC_LITERAL(1131, 31),  // "on_actionCheck_Update_triggered"
        QT_MOC_LITERAL(1163, 24),  // "on_actionAbout_triggered"
        QT_MOC_LITERAL(1188, 22),  // "on_actionAdd_triggered"
        QT_MOC_LITERAL(1211, 30),  // "on_actionNew_Sibling_triggered"
        QT_MOC_LITERAL(1242, 23),  // "on_actionOpen_triggered"
        QT_MOC_LITERAL(1266, 22),  // "on_actionNew_triggered"
        QT_MOC_LITERAL(1289, 23),  // "on_actionSave_triggered"
        QT_MOC_LITERAL(1313, 26),  // "on_actionSave_as_triggered"
        QT_MOC_LITERAL(1340, 23),  // "on_listFind_itemClicked"
        QT_MOC_LITERAL(1364, 16),  // "QListWidgetItem*"
        QT_MOC_LITERAL(1381, 4),  // "item"
        QT_MOC_LITERAL(1386, 29),  // "on_actionBug_Report_triggered"
        QT_MOC_LITERAL(1416, 23),  // "on_actionQuit_triggered"
        QT_MOC_LITERAL(1440, 32),  // "on_listFind_itemSelectionChanged"
        QT_MOC_LITERAL(1473, 23),  // "on_actionFont_triggered"
        QT_MOC_LITERAL(1497, 29),  // "on_listFind_currentRowChanged"
        QT_MOC_LITERAL(1527, 10),  // "currentRow"
        QT_MOC_LITERAL(1538, 16),  // "on_btnNo_clicked"
        QT_MOC_LITERAL(1555, 17),  // "on_btnYes_clicked"
        QT_MOC_LITERAL(1573, 44),  // "on_actionDownload_Upgrade_Pac..."
        QT_MOC_LITERAL(1618, 25),  // "on_btnUpdateASCII_clicked"
        QT_MOC_LITERAL(1644, 26),  // "on_btnUpdateBase64_clicked"
        QT_MOC_LITERAL(1671, 23),  // "on_btnUpdateHex_clicked"
        QT_MOC_LITERAL(1695, 22),  // "on_actProxy1_triggered"
        QT_MOC_LITERAL(1718, 22),  // "on_actProxy2_triggered"
        QT_MOC_LITERAL(1741, 22),  // "on_actProxy3_triggered"
        QT_MOC_LITERAL(1764, 22),  // "on_actProxy4_triggered"
        QT_MOC_LITERAL(1787, 22),  // "on_actProxy5_triggered"
        QT_MOC_LITERAL(1810, 26),  // "on_actionMove_Up_triggered"
        QT_MOC_LITERAL(1837, 28),  // "on_actionMove_Down_triggered"
        QT_MOC_LITERAL(1866, 23),  // "on_actionSort_triggered"
        QT_MOC_LITERAL(1890, 29),  // "on_actionExpand_All_triggered"
        QT_MOC_LITERAL(1920, 31),  // "on_actionCollapse_All_triggered"
        QT_MOC_LITERAL(1952, 23),  // "on_btnFind_Tool_clicked"
        QT_MOC_LITERAL(1976, 25),  // "on_btnReplaceFind_clicked"
        QT_MOC_LITERAL(2002, 17),  // "on_btnNew_clicked"
        QT_MOC_LITERAL(2020, 18),  // "on_btnSave_clicked"
        QT_MOC_LITERAL(2039, 15)   // "onTabBarClicked"
    },
    "MainWindow",
    "on_Find",
    "",
    "on_actionMoveUp",
    "on_actionMoveDown",
    "actionAdd_activated",
    "actionRemove_activated",
    "eventFilter",
    "QEvent*",
    "changeEvent",
    "event",
    "on_NewWindow",
    "recentOpen",
    "filename",
    "on_copyBW",
    "on_pasteBW",
    "actionNew",
    "actionClose_activated",
    "actionClose_all_activated",
    "actionSave",
    "actionSaveAs",
    "actionExpand_all_activated",
    "onTabWidget_currentChanged",
    "index",
    "menu_aboutToShow",
    "onCleanChanged",
    "clean",
    "openRecentFile",
    "on_TabCloseRequest",
    "i",
    "on_copyAction",
    "on_cutAction",
    "on_pasteAction",
    "on_expandAction",
    "on_collapseAction",
    "on_actionNewSibling",
    "on_actionNewChild",
    "replyFinished",
    "QNetworkReply*",
    "reply",
    "CheckUpdate",
    "on_actionSort",
    "on_actionShowPlistText_triggered",
    "checked",
    "on_actionPaste_as_child_triggered",
    "on_editFind_returnPressed",
    "on_editFind_textChanged",
    "arg1",
    "on_ShowFindReplace",
    "on_btnFind_clicked",
    "on_btnHideFind_clicked",
    "on_btnNext_clicked",
    "on_btnPrevious_clicked",
    "on_btnReplace_clicked",
    "on_btnReplaceAll_clicked",
    "on_actionFind_triggered",
    "on_actionFindNext_triggered",
    "on_actionFindPrevious_triggered",
    "on_actionReplace_triggered",
    "on_actionReplaceAll_triggered",
    "on_btnShowReplace_clicked",
    "on_actionCut_triggered",
    "on_actionCopy_between_windows_triggered",
    "on_actionPaste_between_windows_triggered",
    "on_actionCheck_Update_triggered",
    "on_actionAbout_triggered",
    "on_actionAdd_triggered",
    "on_actionNew_Sibling_triggered",
    "on_actionOpen_triggered",
    "on_actionNew_triggered",
    "on_actionSave_triggered",
    "on_actionSave_as_triggered",
    "on_listFind_itemClicked",
    "QListWidgetItem*",
    "item",
    "on_actionBug_Report_triggered",
    "on_actionQuit_triggered",
    "on_listFind_itemSelectionChanged",
    "on_actionFont_triggered",
    "on_listFind_currentRowChanged",
    "currentRow",
    "on_btnNo_clicked",
    "on_btnYes_clicked",
    "on_actionDownload_Upgrade_Packages_triggered",
    "on_btnUpdateASCII_clicked",
    "on_btnUpdateBase64_clicked",
    "on_btnUpdateHex_clicked",
    "on_actProxy1_triggered",
    "on_actProxy2_triggered",
    "on_actProxy3_triggered",
    "on_actProxy4_triggered",
    "on_actProxy5_triggered",
    "on_actionMove_Up_triggered",
    "on_actionMove_Down_triggered",
    "on_actionSort_triggered",
    "on_actionExpand_All_triggered",
    "on_actionCollapse_All_triggered",
    "on_btnFind_Tool_clicked",
    "on_btnReplaceFind_clicked",
    "on_btnNew_clicked",
    "on_btnSave_clicked",
    "onTabBarClicked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      88,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  542,    2, 0x0a,    1 /* Public */,
       3,    0,  543,    2, 0x0a,    2 /* Public */,
       4,    0,  544,    2, 0x0a,    3 /* Public */,
       5,    0,  545,    2, 0x0a,    4 /* Public */,
       6,    0,  546,    2, 0x0a,    5 /* Public */,
       7,    2,  547,    2, 0x0a,    6 /* Public */,
       9,    1,  552,    2, 0x0a,    9 /* Public */,
      11,    0,  555,    2, 0x0a,   11 /* Public */,
      12,    1,  556,    2, 0x08,   12 /* Private */,
      14,    0,  559,    2, 0x08,   14 /* Private */,
      15,    0,  560,    2, 0x08,   15 /* Private */,
      16,    0,  561,    2, 0x08,   16 /* Private */,
      17,    0,  562,    2, 0x08,   17 /* Private */,
      18,    0,  563,    2, 0x08,   18 /* Private */,
      19,    0,  564,    2, 0x08,   19 /* Private */,
      20,    0,  565,    2, 0x08,   20 /* Private */,
      21,    0,  566,    2, 0x08,   21 /* Private */,
      22,    1,  567,    2, 0x08,   22 /* Private */,
      24,    0,  570,    2, 0x08,   24 /* Private */,
      25,    1,  571,    2, 0x08,   25 /* Private */,
      27,    0,  574,    2, 0x08,   27 /* Private */,
      28,    1,  575,    2, 0x08,   28 /* Private */,
      28,    0,  578,    2, 0x28,   30 /* Private | MethodCloned */,
      30,    0,  579,    2, 0x08,   31 /* Private */,
      31,    0,  580,    2, 0x08,   32 /* Private */,
      32,    0,  581,    2, 0x08,   33 /* Private */,
      33,    0,  582,    2, 0x08,   34 /* Private */,
      34,    0,  583,    2, 0x08,   35 /* Private */,
      35,    0,  584,    2, 0x08,   36 /* Private */,
      36,    0,  585,    2, 0x08,   37 /* Private */,
      37,    1,  586,    2, 0x08,   38 /* Private */,
      40,    0,  589,    2, 0x08,   40 /* Private */,
      41,    0,  590,    2, 0x08,   41 /* Private */,
      42,    1,  591,    2, 0x08,   42 /* Private */,
      44,    0,  594,    2, 0x08,   44 /* Private */,
      45,    0,  595,    2, 0x08,   45 /* Private */,
      46,    1,  596,    2, 0x08,   46 /* Private */,
      48,    0,  599,    2, 0x08,   48 /* Private */,
      49,    0,  600,    2, 0x08,   49 /* Private */,
      50,    0,  601,    2, 0x08,   50 /* Private */,
      51,    0,  602,    2, 0x08,   51 /* Private */,
      52,    0,  603,    2, 0x08,   52 /* Private */,
      53,    0,  604,    2, 0x08,   53 /* Private */,
      54,    0,  605,    2, 0x08,   54 /* Private */,
      55,    0,  606,    2, 0x08,   55 /* Private */,
      56,    0,  607,    2, 0x08,   56 /* Private */,
      57,    0,  608,    2, 0x08,   57 /* Private */,
      58,    0,  609,    2, 0x08,   58 /* Private */,
      59,    0,  610,    2, 0x08,   59 /* Private */,
      60,    0,  611,    2, 0x08,   60 /* Private */,
      61,    0,  612,    2, 0x08,   61 /* Private */,
      62,    0,  613,    2, 0x08,   62 /* Private */,
      63,    0,  614,    2, 0x08,   63 /* Private */,
      64,    0,  615,    2, 0x08,   64 /* Private */,
      65,    0,  616,    2, 0x08,   65 /* Private */,
      66,    0,  617,    2, 0x08,   66 /* Private */,
      67,    0,  618,    2, 0x08,   67 /* Private */,
      68,    0,  619,    2, 0x08,   68 /* Private */,
      69,    0,  620,    2, 0x08,   69 /* Private */,
      70,    0,  621,    2, 0x08,   70 /* Private */,
      71,    0,  622,    2, 0x08,   71 /* Private */,
      72,    1,  623,    2, 0x08,   72 /* Private */,
      75,    0,  626,    2, 0x08,   74 /* Private */,
      76,    0,  627,    2, 0x08,   75 /* Private */,
      77,    0,  628,    2, 0x08,   76 /* Private */,
      78,    0,  629,    2, 0x08,   77 /* Private */,
      79,    1,  630,    2, 0x08,   78 /* Private */,
      81,    0,  633,    2, 0x08,   80 /* Private */,
      82,    0,  634,    2, 0x08,   81 /* Private */,
      83,    0,  635,    2, 0x08,   82 /* Private */,
      84,    0,  636,    2, 0x08,   83 /* Private */,
      85,    0,  637,    2, 0x08,   84 /* Private */,
      86,    0,  638,    2, 0x08,   85 /* Private */,
      87,    0,  639,    2, 0x08,   86 /* Private */,
      88,    0,  640,    2, 0x08,   87 /* Private */,
      89,    0,  641,    2, 0x08,   88 /* Private */,
      90,    0,  642,    2, 0x08,   89 /* Private */,
      91,    0,  643,    2, 0x08,   90 /* Private */,
      92,    0,  644,    2, 0x08,   91 /* Private */,
      93,    0,  645,    2, 0x08,   92 /* Private */,
      94,    0,  646,    2, 0x08,   93 /* Private */,
      95,    0,  647,    2, 0x08,   94 /* Private */,
      96,    0,  648,    2, 0x08,   95 /* Private */,
      97,    0,  649,    2, 0x08,   96 /* Private */,
      98,    0,  650,    2, 0x08,   97 /* Private */,
      99,    0,  651,    2, 0x08,   98 /* Private */,
     100,    0,  652,    2, 0x08,   99 /* Private */,
     101,    1,  653,    2, 0x08,  100 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QObjectStar, 0x80000000 | 8,    2,    2,
    QMetaType::Void, 0x80000000 | 8,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   29,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 38,   39,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   43,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   47,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 73,   74,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   80,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   23,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'on_Find'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionMoveUp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionMoveDown'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'actionAdd_activated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'actionRemove_activated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'eventFilter'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QObject *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QEvent *, std::false_type>,
        // method 'changeEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QEvent *, std::false_type>,
        // method 'on_NewWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'recentOpen'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'on_copyBW'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pasteBW'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'actionNew'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'actionClose_activated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'actionClose_all_activated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'actionSave'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'actionSaveAs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'actionExpand_all_activated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTabWidget_currentChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'menu_aboutToShow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCleanChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'openRecentFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_TabCloseRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_TabCloseRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_copyAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_cutAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pasteAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_expandAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_collapseAction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionNewSibling'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionNewChild'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'replyFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'CheckUpdate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionSort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionShowPlistText_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_actionPaste_as_child_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_editFind_returnPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_editFind_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_ShowFindReplace'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnFind_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnHideFind_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnNext_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnPrevious_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnReplace_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnReplaceAll_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionFind_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionFindNext_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionFindPrevious_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionReplace_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionReplaceAll_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnShowReplace_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionCut_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionCopy_between_windows_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionPaste_between_windows_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionCheck_Update_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionAbout_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionAdd_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionNew_Sibling_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionOpen_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionNew_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionSave_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionSave_as_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_listFind_itemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'on_actionBug_Report_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionQuit_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_listFind_itemSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionFont_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_listFind_currentRowChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_btnNo_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnYes_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionDownload_Upgrade_Packages_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnUpdateASCII_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnUpdateBase64_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnUpdateHex_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actProxy1_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actProxy2_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actProxy3_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actProxy4_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actProxy5_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionMove_Up_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionMove_Down_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionSort_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionExpand_All_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionCollapse_All_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnFind_Tool_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnReplaceFind_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnNew_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnSave_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTabBarClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_Find(); break;
        case 1: _t->on_actionMoveUp(); break;
        case 2: _t->on_actionMoveDown(); break;
        case 3: _t->actionAdd_activated(); break;
        case 4: _t->actionRemove_activated(); break;
        case 5: { bool _r = _t->eventFilter((*reinterpret_cast< std::add_pointer_t<QObject*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QEvent*>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->changeEvent((*reinterpret_cast< std::add_pointer_t<QEvent*>>(_a[1]))); break;
        case 7: _t->on_NewWindow(); break;
        case 8: _t->recentOpen((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->on_copyBW(); break;
        case 10: _t->on_pasteBW(); break;
        case 11: _t->actionNew(); break;
        case 12: _t->actionClose_activated(); break;
        case 13: _t->actionClose_all_activated(); break;
        case 14: _t->actionSave(); break;
        case 15: _t->actionSaveAs(); break;
        case 16: _t->actionExpand_all_activated(); break;
        case 17: _t->onTabWidget_currentChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->menu_aboutToShow(); break;
        case 19: _t->onCleanChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 20: _t->openRecentFile(); break;
        case 21: _t->on_TabCloseRequest((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->on_TabCloseRequest(); break;
        case 23: _t->on_copyAction(); break;
        case 24: _t->on_cutAction(); break;
        case 25: _t->on_pasteAction(); break;
        case 26: _t->on_expandAction(); break;
        case 27: _t->on_collapseAction(); break;
        case 28: _t->on_actionNewSibling(); break;
        case 29: _t->on_actionNewChild(); break;
        case 30: _t->replyFinished((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 31: _t->CheckUpdate(); break;
        case 32: _t->on_actionSort(); break;
        case 33: _t->on_actionShowPlistText_triggered((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 34: _t->on_actionPaste_as_child_triggered(); break;
        case 35: _t->on_editFind_returnPressed(); break;
        case 36: _t->on_editFind_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 37: _t->on_ShowFindReplace(); break;
        case 38: _t->on_btnFind_clicked(); break;
        case 39: _t->on_btnHideFind_clicked(); break;
        case 40: _t->on_btnNext_clicked(); break;
        case 41: _t->on_btnPrevious_clicked(); break;
        case 42: _t->on_btnReplace_clicked(); break;
        case 43: _t->on_btnReplaceAll_clicked(); break;
        case 44: _t->on_actionFind_triggered(); break;
        case 45: _t->on_actionFindNext_triggered(); break;
        case 46: _t->on_actionFindPrevious_triggered(); break;
        case 47: _t->on_actionReplace_triggered(); break;
        case 48: _t->on_actionReplaceAll_triggered(); break;
        case 49: _t->on_btnShowReplace_clicked(); break;
        case 50: _t->on_actionCut_triggered(); break;
        case 51: _t->on_actionCopy_between_windows_triggered(); break;
        case 52: _t->on_actionPaste_between_windows_triggered(); break;
        case 53: _t->on_actionCheck_Update_triggered(); break;
        case 54: _t->on_actionAbout_triggered(); break;
        case 55: _t->on_actionAdd_triggered(); break;
        case 56: _t->on_actionNew_Sibling_triggered(); break;
        case 57: _t->on_actionOpen_triggered(); break;
        case 58: _t->on_actionNew_triggered(); break;
        case 59: _t->on_actionSave_triggered(); break;
        case 60: _t->on_actionSave_as_triggered(); break;
        case 61: _t->on_listFind_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 62: _t->on_actionBug_Report_triggered(); break;
        case 63: _t->on_actionQuit_triggered(); break;
        case 64: _t->on_listFind_itemSelectionChanged(); break;
        case 65: _t->on_actionFont_triggered(); break;
        case 66: _t->on_listFind_currentRowChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 67: _t->on_btnNo_clicked(); break;
        case 68: _t->on_btnYes_clicked(); break;
        case 69: _t->on_actionDownload_Upgrade_Packages_triggered(); break;
        case 70: _t->on_btnUpdateASCII_clicked(); break;
        case 71: _t->on_btnUpdateBase64_clicked(); break;
        case 72: _t->on_btnUpdateHex_clicked(); break;
        case 73: _t->on_actProxy1_triggered(); break;
        case 74: _t->on_actProxy2_triggered(); break;
        case 75: _t->on_actProxy3_triggered(); break;
        case 76: _t->on_actProxy4_triggered(); break;
        case 77: _t->on_actProxy5_triggered(); break;
        case 78: _t->on_actionMove_Up_triggered(); break;
        case 79: _t->on_actionMove_Down_triggered(); break;
        case 80: _t->on_actionSort_triggered(); break;
        case 81: _t->on_actionExpand_All_triggered(); break;
        case 82: _t->on_actionCollapse_All_triggered(); break;
        case 83: _t->on_btnFind_Tool_clicked(); break;
        case 84: _t->on_btnReplaceFind_clicked(); break;
        case 85: _t->on_btnNew_clicked(); break;
        case 86: _t->on_btnSave_clicked(); break;
        case 87: _t->onTabBarClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 30:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 88)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 88;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 88)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 88;
    }
    return _id;
}
QT_WARNING_POP
