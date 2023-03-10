#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*------------------------------------------------------------------------------
 * INCLUDES
 *----------------------------------------------------------------------------*/
#include <QMainWindow>
#include "stdarg.h"
#include "qfile.h"
#include "module.h"

/*------------------------------------------------------------------------------
 * CONSTANTS
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * GLOBAL TYPES
 *----------------------------------------------------------------------------*/
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    /*------------------------------------------------------------------------------
     * CLASS TYPES
     *----------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------
     * CLASS PROTOTYPES
     *----------------------------------------------------------------------------*/
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void            on_saveButton_clicked();
    void            on_addFunctionButton_clicked();
    void            on_addVariableButton_clicked();

private:
    Ui::MainWindow  *ui;
    QFile           *mp_current_file;
    Module          m_module;
};

/*------------------------------------------------------------------------------
 * GLOBAL PROTOTYPES
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * EXPORTED GLOBALS
 *----------------------------------------------------------------------------*/

#endif // MAINWINDOW_H
