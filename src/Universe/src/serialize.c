#include <Universe/Universe.h>

void wxDirectorySave(const char* path, wxDirectory* dir)
{
    FILE* file = fopen(path, "wb");
    if (!file) {
        printf("Could not open file '%s\n", path);
    }

    fwrite(dir, sizeof(wxDirectory), 1, file);
    for (unsigned int i = 0; i < dir->used; i++) {
        wxGroup* g = (wxGroup*)&dir->groups[i];
        fwrite(g, sizeof(wxGroup), 1, file);
        for (unsigned int j = 0; j < g->used; j++) {
            wxPtr* p = &g->widgets[j];
            fwrite(p, sizeof(wxPtr), 1, file);
            fwrite(p->widget, wxWidgetSize(p->type), 1, file);
        }
    }
    fclose(file);
    printf("Succesfully writed UI settings at '%s'\n", path);
}

wxDirectory wxDirectoryLoad(const char* path)
{
    wxDirectory dir;
    memset(&dir, 0, sizeof(wxDirectory));

    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Could not open file '%s\n", path);
        return dir;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    char buffer[size];
    fseek(file, 0, SEEK_SET);
    fread(buffer, size, 1, file);
    fclose(file);

    char* mark = &buffer[0];
    memcpy(&dir, mark, sizeof(wxDirectory));
    mark += sizeof(wxDirectory);

    dir.groups = (wxGroup*)malloc(sizeof(wxGroup) * dir.size);
    for (unsigned int i = 0; i < dir.used; i++) {
        wxGroup* g = &dir.groups[i];
        memcpy(g, mark, sizeof(wxGroup));
        mark += sizeof(wxGroup);

        g->widgets = (wxPtr*)malloc(sizeof(wxPtr) * g->size);
        for (unsigned int j = 0; j < g->used; j++) {
            wxPtr* p = &g->widgets[j];
            memcpy(p, mark, sizeof(wxPtr));
            mark += sizeof(wxPtr);

            size_t size = wxWidgetSize(p->type);
            p->widget = malloc(size);
            memcpy(p->widget, mark, size);
            mark += size;
        }
    }

    printf("Succesfully read UI settings at '%s'\n", path);
    return dir;
}