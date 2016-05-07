#pragma once
#include <iostream>
#include <unordered_set>
#include "ECSDefines.h"
#include "Component.h"
#include "EntityManager.h"
#include "ISystem.h"
#include "../Utils/Observer.h"
#include "../Utils/Refreshable.h"

namespace ska {
	
	template <class Storage, class ... ComponentType>
	class System : public Observer<EntityData>, virtual public ISystem, virtual protected Refreshable {
	public :
		System(EntityManager& entityManager) : 
			m_entityManager(entityManager) { 
			m_entityManager.addObserver(this);
			
			/* Bracket initializer trick */
			int _[] = { 0, (buildSystemMask<ComponentType>() , 0)... };
			(void)_;

		}

		void update() override {
			refresh();
			if (!m_toDelete.empty()) {
				for (EntityId entity : m_toDelete) {
					m_entityManager.removeEntity(entity);
				}
				m_toDelete.clear();
			}
		}

		void scheduleDeferredRemove(ska::EntityId e) {
			m_toDelete.emplace(e);
		}

		void update(Observable<EntityData>* obs, const EventArg& e, EntityData& t) override {
			
			/* An entity belongs to the system ONLY IF it has ALL the requiered components of the system */
			EntityComponentsMask resultMask = *t.first;
			EntityId entityId = t.second;
			resultMask &= m_systemComponentMask;

			switch (e.type()) {
			case EntityEventType::COMPONENT_ADD:
				if (resultMask == m_systemComponentMask && m_processed.count(entityId) == 0) {
					m_processed.insert(entityId);
				}
				break;
			
			case EntityEventType::COMPONENT_REMOVE:
				if (resultMask != m_systemComponentMask && m_processed.count(entityId) > 0) {
					m_processed.erase(entityId);
				}
				break;
			
			default:
				break;
			}
						
		}

		virtual ~System(){ m_entityManager.removeObserver(this); }

	private:
		std::unordered_set<ska::EntityId> m_toDelete;
		EntityComponentsMask m_systemComponentMask;
		

		template <class T>
		void buildSystemMask() {
			/* Retrieve all the components masks using the variadic template with ComponentType 
			   We "iterate" through each ComponentType with the bracket initializer trick and 
			   we add each component mask to the system component mask with a binary OR. */
			unsigned int mask = m_entityManager.template getMask<T>();
			if (mask >= m_systemComponentMask.size()) {
				throw IllegalStateException("Too many components are used in the game. Unable to continue.");
			}
			m_systemComponentMask[mask] = true;
		}
		

	protected:
		EntityManager& m_entityManager;	
		Storage m_processed;

	};
}
