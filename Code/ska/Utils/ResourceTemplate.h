#pragma once

#include <memory>
#include <unordered_map>

namespace ska {
	template <class V, class K>
	class ResourceTemplate
	{

	public:
		virtual void load(K key) {
			m_key = key;
			if (m_container.find(m_key) == m_container.end() || m_container[m_key].lock() == nullptr) {
				m_value = std::make_shared<V>(m_key);
				m_container[m_key] = m_value;
			} else {
				m_value = m_container[m_key].lock();
			}
		}

		virtual void free() { m_value = nullptr; }

		virtual ~ResourceTemplate() { free(); }


	protected:
		ResourceTemplate() {
			ResourceTemplate<V, K>::free(); 
		}

		explicit ResourceTemplate(K key) {
			ResourceTemplate<V, K>::free();
			ResourceTemplate<V, K>::load(key); 
		}

		K m_key;
		std::shared_ptr<V> m_value;
	
		static std::unordered_map<K, std::weak_ptr<V>> m_container;

	};

	template<class V, class K>
	std::unordered_map<K, std::weak_ptr<V>> ResourceTemplate<V, K>::m_container;
}

