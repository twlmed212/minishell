
#include "minishell.h"

int builtin_cd(char **args)
{
    char *path;
   
    // الحالة 1: cd بدون وسيط -> نذهب إلى HOME
    if (!args[1])
    {
        path = getenv("HOME");
        if (!path)
        {
            printf("cd: HOME not set\n");
            return 1;
        }
    }
    // الحالة 2: أكثر من وسيط -> خطأ
    else if (args[2])
    {
        printf("cd: too many arguments\n");
        return 1;
    }
    // الحالة 3: cd path -> ننتقل إلى المسار المحدد
    else
        path = args[1];

    // محاولة الانتقال إلى المجلد الجديد
    if (chdir(path) != 0)
    {
        printf("error in cd");
        return 1;
    }
    return 0;
}


// ----------------- pwd -----------------
int builtin_pwd(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("error in pwd");
        return 1;
    }
    printf("%s\n", cwd);
    return 0;
}
