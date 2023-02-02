/*------------------------------------------------------------------------------
 * INCLUDES
 *----------------------------------------------------------------------------*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"

#define TESTING (1)

/*------------------------------------------------------------------------------
 * GLOBALS
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * PRIVATE PROTOTYPES
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * FUNCTION : Constructor
 *----------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#if TESTING
    ui->addFunctionButton->setDisabled(true);
    ui->addVariableButton->setDisabled(true);
#endif
}

/*------------------------------------------------------------------------------
 * FUNCTION : Destructor
 *----------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*------------------------------------------------------------------------------
 * FUNCTION : on_saveButton_clicked
 *----------------------------------------------------------------------------*/
void MainWindow::on_saveButton_clicked()
{
#if TESTING
    QVector<variable_t> parameters;
    QVector<variable_t> local_variables;
    QVector<code_t> code_chunks;
    variable_t var;
    code_t code_chunk;

    parameters.clear();
    var.name = QString("radius");
    var.type = QString("double");
    var.is_static = false;
    var.initial_value = QString("");
    parameters.append(var);

    local_variables.clear();
    var.name = QString("radius");
    var.type = QString("double");
    var.is_static = true;
    var.initial_value = QString("0.0");
    local_variables.append(var);

    code_chunks.clear();
    code_chunk.type = CODE_TYPE_ONE_SHOT;
    code_chunk.text = "    Init_1();\n\n    Init_2();\n";
    code_chunks.append(code_chunk);

    m_module.add_function(QString("scad_circle"),
                          VAR_TYPE_VOID,
                          "    Testing \n    123\n",
                          parameters,
                          local_variables,
                          code_chunks,
                          false);

    parameters.clear();
    var.name = QString("size");
    var.type = QString("double");
    var.is_static = false;
    var.initial_value = QString("");
    parameters.append(var);

    var.name = QString("center");
    var.type = QString("bool");
    var.is_static = false;
    var.initial_value = QString("");
    parameters.append(var);

    local_variables.clear();
    var.name = QString("size");
    var.type = QString("double");
    var.is_static = false;
    var.initial_value = QString("0.0");
    local_variables.append(var);

    var.name = QString("center");
    var.type = QString("bool");
    var.is_static = true;
    var.initial_value = QString("false");
    local_variables.append(var);

    code_chunks.clear();
    code_chunk.type = CODE_TYPE_ONE_SHOT;
    code_chunk.text = "    Init_1();\n\n    Init_2();\n";
    code_chunks.append(code_chunk);

    m_module.add_function(QString("scad_square"),
                          VAR_TYPE_UINT8,
                          "    Testing \n    123\n",
                          parameters,
                          local_variables,
                          code_chunks,
                          false);

    parameters.clear();
    var.name = QString("pp_points");
    var.type = QString("double **");
    var.is_static = false;
    var.initial_value = QString("");
    parameters.append(var);

    var.name = QString("points_count");
    var.type = QString("uint32_t");
    var.is_static = false;
    var.initial_value = QString("");
    parameters.append(var);

    local_variables.clear();
    var.name = QString("pp_points");
    var.type = QString("double **");
    var.is_static = false;
    var.initial_value = QString("0");
    local_variables.append(var);

    var.name = QString("points_count");
    var.type = QString("uint32_t");
    var.is_static = false;
    var.initial_value = QString("0");
    local_variables.append(var);

    code_chunks.clear();
    code_chunk.type = CODE_TYPE_ONE_SHOT;
    code_chunk.text = "    Init_1();\n\n    Init_2();\n";
    code_chunks.append(code_chunk);

    m_module.add_function(QString("scad_polygon"),
                          VAR_TYPE_VOID,
                          "    Testing \n    123\n",
                          parameters,
                          local_variables,
                          code_chunks,
                          true);
#endif
    m_module.save();
}

/*------------------------------------------------------------------------------
 * FUNCTION : on_addFunctionButton_clicked
 *----------------------------------------------------------------------------*/
void MainWindow::on_addFunctionButton_clicked()
{
    static int function_count = 0;

    QVector<variable_t> parameters;
    QVector<variable_t> local_variables;
    QVector<code_t>     code_chunks;
    m_module.add_function(QString("Test%1").arg(function_count),
                          VAR_TYPE_VOID,
                          "    Testing \n    123\n",
                          parameters,
                          local_variables,
                          code_chunks,
                          false);
    function_count++;
}

/*------------------------------------------------------------------------------
 * FUNCTION : on_addVariableButton_clicked
 *----------------------------------------------------------------------------*/
void MainWindow::on_addVariableButton_clicked()
{
    static int variable_count = 0;
    m_module.add_variable(QString("Test%1").arg(variable_count), VAR_TYPE_VOID, true);
    variable_count++;
}

