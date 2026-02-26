module timer;

void Timer::Start()
{
    if (m_running) return;

    const auto now = clock::now();
    const auto pauseDuration = now - m_stop;
    m_start += pauseDuration;
    m_lap += pauseDuration;
    m_running = true;
}

void Timer::Stop()
{
    if (!m_running) return;
    m_stop = clock::now();
    m_running = false;
}

void Timer::Reset()
{
    m_start = clock::now();
    m_lap = m_start;
    m_stop = m_start;
}

float Timer::GetTime() const
{
    const auto end = m_running ? clock::now() : m_stop;
    return std::chrono::duration<float>(end - m_start).count();
}

float Timer::GetLapTime()
{
    const auto now = m_running ? clock::now() : m_stop;
    const float lapTime = std::chrono::duration<float>(now - m_lap).count();
    m_lap = now;
    return lapTime;
}