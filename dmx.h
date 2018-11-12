#pragma once

#include <ola/DmxBuffer.h>
#include <ola/client/StreamingClient.h>
#include "framebuf.h"

class DMX
{
public: 
    DMX(Framebuf& fb);
    ~DMX();
    void render(void);
private:
    Framebuf& m_fb;
    ola::DmxBuffer m_buffer;
    ola::client::StreamingClient* m_ola_client;
};
