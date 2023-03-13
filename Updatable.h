#pragma once

class Updatable
{
public:
	virtual void Update(double time)
	{
		prevTime = currentTime;
		currentTime = time;

		timeDelta = currentTime - prevTime;
	};
protected:
	double currentTime, prevTime, timeDelta;
};

