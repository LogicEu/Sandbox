#include <Universe/Universe.h>

static void wxGroupWidgetDraw(wxPtr w)
{
    switch (w.type) {
        case WIDGET_RECT: {
            wxRectDraw(*(wxRect*)w.widget);
            break;
        }
        case WIDGET_TITLE: {
            wxTitleDraw((wxTitle*)w.widget);
            break;
        }
        case WIDGET_ICON: {
            iconDraw((Icon*)w.widget);
            break;
        }
        case WIDGET_BUTTON: {
            wxButtonDraw((wxButton*)w.widget);
            break;
        }
        case WIDGET_SWITCH: {
            wxSwitchDraw((wxSwitch*)w.widget);
            break;
        }
        case WIDGET_SLIDER: {
            wxSliderDraw((wxSlider*)w.widget);
            break;
        }
        case WIDGET_FIELD: {
            wxFieldDraw((wxField*)w.widget);
            break;
        }
        default: {break;}
    }
}

static void wxGroupWidgetUpdate(wxPtr w, vec2 mouse, bool pressed, bool down)
{
    switch (w.type) {
        case WIDGET_RECT: {
            return;
        }
        case WIDGET_TITLE: {
            return;
        }
        case WIDGET_ICON: {
            return;
        }
        case WIDGET_BUTTON: {
            wxButtonUpdate((wxButton*)w.widget, mouse, down);
            break;
        }
        case WIDGET_SWITCH: {
            wxSwitchUpdate((wxSwitch*)w.widget, mouse, pressed);
            break;
        }
        case WIDGET_SLIDER: {
            wxSliderUpdate((wxSlider*)w.widget, mouse, down);
            break;
        }
        case WIDGET_FIELD: {
            wxFieldUpdate((wxField*)w.widget, mouse, pressed);
            break;
        }
        default: { break; }
    }
}

size_t wxWidgetSize(wxEnum e)
{
    switch (e) {
        case WIDGET_RECT: {
            return sizeof(wxRect);
        }
        case WIDGET_TITLE: {
            return sizeof(wxTitle);
        }
        case WIDGET_ICON: {
            return sizeof(Icon);
        }
        case WIDGET_BUTTON: {
            return sizeof(wxButton);
        }
        case WIDGET_SWITCH: {
            return sizeof(wxSwitch);
        }
        case WIDGET_SLIDER: {
            return sizeof(wxSlider);
        }
        case WIDGET_FIELD: {
            return sizeof(wxField);
        }
        default: {return 0;}
        break;
    }
    return 0;
}

wxGroup wxGroupCreate()
{
    wxGroup g;
    g.size = 1;
    g.used = 0;
    g.widgets = (wxPtr*)malloc(sizeof(wxPtr));
    return g;
}

void wxGroupDraw(wxGroup* group)
{
    for (unsigned int i = 0; i < group->used; i++) {
        wxGroupWidgetDraw(group->widgets[i]);
    }
}

void wxGroupUpdate(wxGroup* group, vec2 mouse, bool pressed, bool down)
{
    for (unsigned int i = 0; i < group->used; i++) {
        wxGroupWidgetUpdate(group->widgets[i], mouse, pressed, down);
    }
}

void wxGroupPush(wxGroup* group, void* widget, wxEnum type)
{
    if (group->used >= group->size) {
        group->size *= 2;
        group->widgets = (wxPtr*)realloc(group->widgets, sizeof(wxPtr) * group->size);
    }

    unsigned int size = wxWidgetSize(type);
    group->widgets[group->used].type = type;
    group->widgets[group->used].widget = malloc(size);
    memcpy(group->widgets[group->used++].widget, widget, size);
}

void wxGroupFree(wxGroup* group)
{
    for (unsigned int i = 0; i < group->used; i++) {
        free(group->widgets[i].widget);
    }
    free(group->widgets);
}