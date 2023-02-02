/*------------------------------------------------------------------------------
 * INCLUDES
 *----------------------------------------------------------------------------*/
#include "module.h"

/*------------------------------------------------------------------------------
 * GLOBALS
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * PRIVATE PROTOTYPES
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * FUNCTION : Constructor
 *----------------------------------------------------------------------------*/
Module::Module(QObject *parent)
    : QObject{parent}
{

}

/*------------------------------------------------------------------------------
 * FUNCTION : add_function
 *----------------------------------------------------------------------------*/
void Module::add_function(QString name,
                          var_type_t return_type,
                          QString body, // DEPRECATED
                          QVector<variable_t> parameters,
                          QVector<variable_t> local_variables,
                          QVector<code_t> code_chunks,
                          bool is_static)
{
    function_t function;
    function.name               = name;
    function.type               = VAR_TYPE_LOOKUP[return_type];
    function.body               = body; // DEPRECATED
    function.parameters         = parameters;
    function.local_variables    = local_variables;
    function.code_chunks        = code_chunks;
    function.is_static          = is_static;
    m_functions.append(function);
}

/*------------------------------------------------------------------------------
 * FUNCTION : add_variable
 *----------------------------------------------------------------------------*/
void Module::add_variable(QString name,
                          var_type_t type,
                          bool is_static)
{
    variable_t variable;
    variable.name       = name;
    variable.type       = VAR_TYPE_LOOKUP[type];
    variable.is_static  = is_static;
    m_variables.append(variable);
}

/*------------------------------------------------------------------------------
 * FUNCTION : save
 *----------------------------------------------------------------------------*/
void Module::save(void)
{
    FILE *p_file;

    char module_name[32] = "output";

    /* Create the source file */
    char filename_source[32] = "";
    snprintf(filename_source, 32, "%s.c", module_name);

    /* Open the 'c' file for writing */
    p_file = fopen("output.c", "w");
    if(NULL != p_file)
    {
        /* Start writing all the headings */
        fputs_heading(p_file, "INCLUDES");
        fputs("\n", p_file);

        fputs_heading(p_file, "GLOBALS");
        for(variable_t v : m_variables)
        {
            fputs(QString("%1 %2;\n").arg(v.type).arg(v.name).toLatin1(), p_file);
        }
        fputs("\n", p_file);

        fputs_heading(p_file, "PRIVATE PROTOTYPES");
        for(function_t f : m_functions)
        {
            if(f.is_static)
            {
                fputs_function_prototype(p_file, &f);
            }
        }
        fputs("\n", p_file);

        fputs_heading(p_file, "FUNCTIONS");
        for(function_t f : m_functions)
        {
            fputs_heading(p_file, QString("FUNCTION : %1").arg(f.name));
            fputs_function(p_file, &f);
            fputs("\n", p_file);
        }

        fclose(p_file);
    }

    /* Create the header file */
    char filename_header[32] = "";
    snprintf(filename_header, 32, "%s.h", module_name);

    p_file = fopen("output.h", "w");
    if(NULL != p_file)
    {
        fputs("#ifndef OUTPUT_H\n#define OUTPUT_H\n", p_file);

        fputs_heading(p_file, "INCLUDES");
        fputs("\n", p_file);

        fputs_heading(p_file, "CONSTANTS");
        fputs("\n", p_file);

        fputs_heading(p_file, "TYPES");
        fputs("\n", p_file);

        fputs_heading(p_file, "PROTOTYPES");
        for(function_t f : m_functions)
        {
            if(!f.is_static)
            {
                fputs_function_prototype(p_file, &f);
            }
        }
        fputs("\n", p_file);

        fputs_heading(p_file, "EXPORTED GLOBALS");
        fputs("\n", p_file);

        fputs("#endif // OUTPUT_H", p_file);

        fclose(p_file);
    }
}

/*------------------------------------------------------------------------------
 * FUNCTION : fputs_heading
 *----------------------------------------------------------------------------*/
void Module::fputs_heading(FILE * p_file, QString heading_text)
{
    fputs("/*------------------------------------------------------------------------------\n", p_file);
    fputs(QString(" * %1\n").arg(heading_text).toLatin1(), p_file);
    fputs(" *----------------------------------------------------------------------------*/\n", p_file);
}

/*------------------------------------------------------------------------------
 * FUNCTION : fputs_function_prototype
 *----------------------------------------------------------------------------*/
void Module::fputs_function_prototype(FILE * p_file, function_t * p_function)
{
    fputs(QString("%1 %2")
          .arg(QString("%1")
               .arg(QString((p_function->is_static)?("static %1"):("%1"))
                    .arg(p_function->type), VAR_SPACING))
          .arg(p_function->name, VAR_SPACING).toLatin1(),
          p_file);

    if(p_function->parameters.length() > 0)
    {
        fputs("(", p_file);
        for(int i = 0 ; i < p_function->parameters.length(); i++)
        {
            variable_t v = p_function->parameters.at(i);
            fputs(QString("%3 %4").arg(v.type).arg(v.name).toLatin1(), p_file);

            if(i < (p_function->parameters.length() - 1))
            {
                fputs(", ", p_file);
            }
            else
            {
                fputs(");\n", p_file);
            }
        }
    }
    else
    {
        fputs("(void);\n", p_file);
    }
}

/*------------------------------------------------------------------------------
 * FUNCTION : fputs_function
 *----------------------------------------------------------------------------*/
void Module::fputs_function(FILE * p_file, function_t * p_function)
{
    fputs(QString("%1 %2")
          .arg(QString("%1")
               .arg(QString((p_function->is_static)?("static %1"):("%1"))
                    .arg(p_function->type)))
          .arg(p_function->name).toLatin1(),
          p_file);

    if(p_function->parameters.length() > 0)
    {
        fputs("(", p_file);
        for(int i = 0 ; i < p_function->parameters.length(); i++)
        {
            variable_t v = p_function->parameters.at(i);
            fputs(QString("%3 %4").arg(v.type).arg(v.name).toLatin1(), p_file);

            if(i < (p_function->parameters.length() - 1))
            {
                fputs(", ", p_file);
            }
            else
            {
                fputs(")\n", p_file);
            }
        }
    }
    else
    {
        fputs("(void)\n", p_file);
    }

    fputs("{\n", p_file);

    /* Items needed at top of function */
    if(p_function->type != "void")
    {
        fputs(QString("    %1 %2 = %3;\n")
              .arg(QString("%1")
                   .arg(QString((p_function->is_static)?("static %1"):("%1"))
                        .arg(p_function->type), VAR_SPACING))
              .arg("ret", VAR_SPACING)
              .arg("0").toLatin1(),
              p_file);
    }

    for(variable_t v : p_function->local_variables)
    {
        fputs(QString("    %1 %2 = %3;\n")
              .arg(QString("%1")
                   .arg(QString((v.is_static)?("static %1"):("%1"))
                        .arg(v.type), VAR_SPACING))
              .arg(v.name, VAR_SPACING)
              .arg(v.initial_value).toLatin1(),
              p_file);
    }
    fputs("\n", p_file);

    fputs_code_body(p_file, p_function);
    fputs("\n", p_file);

    /* Items needed at bottom of function */
    if(p_function->type != "void")
    {
        fputs(QString("    return ret;\n").toLatin1(), p_file);
    }
    else
    {
        fputs(QString("    return;\n").toLatin1(), p_file);
    }
    fputs("}\n", p_file);
}

/*------------------------------------------------------------------------------
 * FUNCTION : fputs_code_body
 *----------------------------------------------------------------------------*/
void Module::fputs_code_body(FILE * p_file, function_t * p_function)
{
    for(code_t c : p_function->code_chunks)
    {
        switch(c.type)
        {
        case CODE_TYPE_ONE_SHOT:
        case CODE_TYPE_PERIODIC:
        case CODE_TYPE_LOOP:
        case CODE_TYPE_STATE_MACHINE:
        case CODE_TYPE_END_OF_LIST:
        default:
            /* Body goes in next */
            fputs(c.text.toLatin1(), p_file);
            break;
        }
    }
}
