#pragma once
class Render
{
public:
	Render& operator=(Render&) = delete;
	Render(const Render&) = delete;

	static Render& Instance();
	void Initialize();

private:
	Render();
};