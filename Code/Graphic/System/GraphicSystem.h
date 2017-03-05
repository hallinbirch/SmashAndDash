#pragma once
#include <unordered_set>
#include "../../ska/Graphic/GraphicComponent.h"
#include "../../ska/Physic/PositionComponent.h"
#include "../../ska/ECS/System.h"
#include "AbstractGraphicSystem.h"

class GraphicSystem : public AbstractGraphicSystem, public ska::System<std::unordered_set<ska::EntityId>, ska::GraphicComponent, ska::PositionComponent>
{
public:
	GraphicSystem(ska::CameraSystem* camera, ska::EntityManager& entityManager);
	GraphicSystem& operator=(const GraphicSystem&) = delete;
	int getTopLayerPriority() const;
	virtual void update() override;
	virtual ~GraphicSystem();
protected:
	virtual void refresh() override;
private:
	int m_topLayerPriority;
};


