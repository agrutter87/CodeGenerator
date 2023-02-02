#ifndef MODULE_H
#define MODULE_H

/*------------------------------------------------------------------------------
 * INCLUDES
 *----------------------------------------------------------------------------*/
#include <QObject>
#include <QVector>
#include <QStringList>

/*------------------------------------------------------------------------------
 * CONSTANTS
 *----------------------------------------------------------------------------*/
#define VAR_SPACING         (-20) // Negative aligns to the left

/*------------------------------------------------------------------------------
 * GLOBAL TYPES
 *----------------------------------------------------------------------------*/
typedef enum
{
    CODE_TYPE_ONE_SHOT      = 1,
    CODE_TYPE_PERIODIC      = 2,
    CODE_TYPE_LOOP          = 3,
    CODE_TYPE_STATE_MACHINE = 4,
    CODE_TYPE_END_OF_LIST   = 5,
} code_type_t;

typedef enum
{
    VAR_TYPE_VOID           = 0,
    VAR_TYPE_BOOL           = 1,
    VAR_TYPE_UINT8          = 2,
    VAR_TYPE_DOUBLE         = 3,
    VAR_TYPE_DOUBLE_P       = 4,
    VAR_TYPE_DOUBLE_PP      = 5,
    VAR_TYPE_END_OF_LIST    = 6,
} var_type_t;

const QString VAR_TYPE_LOOKUP[VAR_TYPE_END_OF_LIST] =
{
    "void",
    "bool",
    "uint8_t",
    "double",
    "double *",
    "double **"
};

typedef struct st_code
{
    code_type_t                 type;
    QString                     text;
} code_t;

typedef struct st_variable
{
    QString                     name;
    QString                     type;
    QString                     initial_value;
    bool                        is_static;
} variable_t;

typedef struct st_function
{
    QString                     name;
    QString                     type;
    QString                     body; // DEPRECATED
    QVector<variable_t>         parameters;
    QVector<variable_t>         local_variables;
    QVector<code_t>             code_chunks;
    bool                        is_static;
} function_t;


/*------------------------------------------------------------------------------
 * CLASS : Module
 *----------------------------------------------------------------------------*/
class Module : public QObject
{
    Q_OBJECT

    /*--------------------------------------------------------------------------
     * CLASS TYPES
     *------------------------------------------------------------------------*/
    /*--------------------------------------------------------------------------
     * CLASS PROTOTYPES
     *------------------------------------------------------------------------*/
public:
    explicit                Module(QObject *parent = nullptr);
    void                    add_function(QString name,
                                         var_type_t return_type,
                                         QString body,
                                         QVector<variable_t> parameters,
                                         QVector<variable_t> local_variables,
                                         QVector<code_t> code_chunks,
                                         bool is_private);
    void                    add_variable(QString name,
                                         var_type_t type,
                                         bool is_static);
    void                    save(void);

signals:


private:
    QVector<function_t>     m_functions;
    QVector<variable_t>     m_variables;

    void                    fputs_heading(FILE * p_file, QString heading_text);
    void                    fputs_function_prototype(FILE * p_file, function_t * p_function);
    void                    fputs_function(FILE * p_file, function_t * p_function);
    void                    fputs_code_body(FILE * p_file, function_t * p_function);
};

/*------------------------------------------------------------------------------
 * GLOBAL PROTOTYPES
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * EXPORTED GLOBALS
 *----------------------------------------------------------------------------*/

#endif // MODULE_H
