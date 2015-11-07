#ifndef __SKYBOX_H__
#define __SKYBOX_H__


#include <string>
#include "Camera.h"
#include "CubeMapTexture.h"
#include "Mesh.h"
#include "IDrawable.h"
#include "GameObject.h"

using namespace std;

class Skybox : public IDrawable
{
public:
	Skybox(Camera* camera);
	~Skybox();

	bool init(const string& posXFilename, const string& negXFilename, const string& posYFilename, const string& negYFilename, const string& posZFilename, const string& negZFilename);
	
	virtual void render();
private:
	Camera* m_camera;
	CubeMapTexture* m_pCubemap;
	Shader m_shaderProgram;
	Mesh m_skyMesh;
	GameObject* m_skyObject;
};


#endif // !__SKYBOX_H__
