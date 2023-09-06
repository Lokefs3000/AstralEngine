#pragma once

#include <memory>
#include <chrono>

struct StopWatchHigh_NonPtr
{
private:
	std::chrono::high_resolution_clock::time_point m_StartTimePoint;

	float m_Duration = 0.0f;
public:
	void Start() {
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	void End() {
		m_Duration = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(std::chrono::high_resolution_clock::now() - m_StartTimePoint).count();
	}

	float GetDuration() { return m_Duration; }
};

typedef std::shared_ptr<StopWatchHigh_NonPtr> StopWatchHigh;
#define newStopWatchHigh() std::make_shared<StopWatchHigh_NonPtr>();