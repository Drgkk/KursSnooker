#pragma once
#include "SceneBuilder.h"
#include "Scene.h"
#include "ObjectAssimpParser.h"

class RegularSceneBuilder : public SceneBuilder
{
public:
	RegularSceneBuilder(SceneConfig cfg);
	void BuildSprite(SceneSpriteBuilderConfig cfg) const override;
	void BuildLightSource(SceneLightSourceBuilderConfig cfg) const override;
	void Reset();
	std::unique_ptr<Scene> Build();
private:
	std::unique_ptr<Scene> scene;
	SceneConfig cfg;
};

