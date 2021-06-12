#include <Taxi/Taxi.h>

/*
------------------
 -> Framebuffer <- 
------------------
*/

unsigned int framebuffer_id()
{
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    return fbo;
}

framebuffer_t framebuffer_new()
{
    framebuffer_t framebuffer;
    framebuffer.id = framebuffer_id();
    framebuffer.texture = texture_framebuffer(framebuffer.id);
    return framebuffer;
}

void framebuffer_bind(unsigned int id)
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}