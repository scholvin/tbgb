#include <ola/Logging.h>
#include <ola/client/StreamingClient.h>
#include "dmx.h"

DMX::DMX(Framebuf& fb) : m_fb(fb)
{
    ola::InitLogging(ola::OLA_LOG_WARN, ola::OLA_LOG_STDERR); // stdout not available
    m_buffer.Blackout();
    m_ola_client = new ola::client::StreamingClient(ola::client::StreamingClient::Options());
    if (!m_ola_client->Setup())
        throw std::runtime_error("Could not setup ola client");

}

DMX::~DMX()
{
    delete m_ola_client;
}

void
DMX::render(void)
{
    // m_ola_client->SendDmx(universe, m_buffer);
}
