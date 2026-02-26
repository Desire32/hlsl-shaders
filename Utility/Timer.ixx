export module timer;

import <chrono>;

export class Timer
{
public:
    Timer() = default;

    void Start()
    {
        m_start = clock::now();
        m_lap = m_start;
        m_running = true;
    }

    void Stop()
    {
        if (m_running)
        {
            m_stop = clock::now();
            m_running = false;
        }
    }

    void Reset()
    {
        m_start = clock::now();
        m_lap = m_start;
        if (!m_running)
            m_stop = m_start;
    }

    [[nodiscard]] float GetTime() const
    {
        const auto end = m_running ? clock::now() : m_stop;
        return std::chrono::duration<float>(end - m_start).count();
    }
    [[nodiscard]] float GetLapTime()
    {
        const auto now = m_running ? clock::now() : m_stop;
        const float lap = std::chrono::duration<float>(now - m_lap).count();
        m_lap = now;
        return lap;
    }

private:
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;

    bool       m_running = false;
    time_point m_start;
    time_point m_stop;
    time_point m_lap;
};