#pragma once
class Scene
{
public:
	static Scene & GetIns() { static Scene ms; return ms; }
	~Scene();

	void Load();

private:
	Scene();
	void objLoader(const std::string & path);
	void mtlLoader(const std::string & path);
};

