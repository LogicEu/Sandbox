#include <Universe/Universe.h>

wxDirectory wxDirectoryCreate()
{
    wxDirectory d;
    d.size = 1;
    d.used = 0;
    d.selected = 0;
    d.groups = (wxGroup*)malloc(sizeof(wxGroup));
    return d;
}

void wxDirectoryPushGroup(wxDirectory* dir, wxGroup* group)
{
    if (dir->used >= dir->size) {
        dir->size *= 2;
        dir->groups = (wxGroup*)realloc(dir->groups, sizeof(wxGroup) * dir->size);
    }
    dir->selected = dir->used;
    memcpy(&dir->groups[dir->used++], group, sizeof(wxGroup));
}

void wxDirectoryPushWidget(wxDirectory* dir, void* widget, wxEnum type)
{
    wxGroupPush(&dir->groups[dir->selected], widget, type);
}

void wxDirectoryFree(wxDirectory* dir)
{
    for (unsigned int i = 0; i < dir->used; i++) {
        wxGroupFree(&dir->groups[i]);
    }
    free(dir->groups);
}

void wxDirectoryDraw(wxDirectory* dir)
{
    wxGroupDraw(&dir->groups[dir->selected]);
}

void wxDirectoryUpdate(wxDirectory* dir, vec2 mouse, bool pressed, bool down)
{
    wxGroupUpdate(&dir->groups[dir->selected], mouse, pressed, down);
}

void wxDirectorySelect(wxDirectory* dir, unsigned int index)
{
    if (index >= dir->used) {
        printf("wxGroup does not existin this wxDirectory.\n");
        return;
    }
    dir->selected = index;
}

wxGroup* wxDirectoryGroup(wxDirectory* dir)
{
    return &dir->groups[dir->selected];
}