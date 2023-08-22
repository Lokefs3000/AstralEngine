#pragma once

#include <memory>
#include <chrono>

struct StopWatch_NonPtr
{
private:
	std::chrono::system_clock::time_point m_StartTimePoint;

	float m_Duration = 0.0f;
public:
	void Start() {
		m_StartTimePoint = std::chrono::system_clock::now();
	}

	void End() {
		m_Duration = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(std::chrono::system_clock::now() - m_StartTimePoint).count();
	}

	float GetDuration() { return m_Duration; }
};

typedef std::shared_ptr<StopWatch_NonPtr> StopWatch;
#define newStopWatch() std::make_shared<StopWatch_NonPtr>();