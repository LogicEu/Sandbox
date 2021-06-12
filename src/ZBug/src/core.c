#include <ZBug/ZBug.h>

static FILE* dump;
unsigned int self = 0;

void zbug_init(const char* fp, unsigned int self_check)
{
    if (fp) dump = fopen(fp, "w");
    else dump = stdout;
    self = self_check;
}

void zbug_log(const char* format, ...)
{
    va_list arglist;
    va_start(arglist, format);
    vfprintf(dump, format, arglist);
    va_end(arglist);
}

void zbug_log_fmt(const char* _file_, unsigned int _line_, const char* _func_, const char* format, ...)
{
    fprintf(dump, "ZBugLog: | '%s' / %u / %s | ", _file_, _line_, _func_);
    va_list arglist;
    va_start(arglist, format);
    vfprintf(dump, format, arglist);
    va_end(arglist);
}

void zbug_mark(const char* _file_, unsigned int _line_, const char* _func_)
{
    fprintf(dump, "ZBugMark! | '%s' / %u / %s |\n", _file_, _line_, _func_);
}

void zbug_exit()
{
    fprintf(dump, "ZBug closed succesfully!\n");
    fclose(dump);
}