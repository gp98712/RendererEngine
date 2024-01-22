#pragma once
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>

const unsigned int MAX_COMPONENTS = 32;

// ��Ʈ�� ��ƼƼ�� � ������Ʈ�� ������ �ִ��� ǥ��
// ��Ʈ������ ���� Ȯ���Ѵ�.
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
protected:
	static int nextId;
};

template <typename T>
class Component : public IComponent {
	static int GetId() {
		static auto id = nextId++;
		return id;
	}
};

class Entity {
private:
	int id;

public:
	Entity(int id): id(id) {}
	Entity(const Entity& other) = default;
	int GetId() const;

	Entity& operator = (const Entity& other) = default;
	bool operator==(const Entity& other) const {
		return id == other.id;
	}
	bool operator!=(const Entity& other) const {
		return id != other.id;
	}
	bool operator<(const Entity & other) const {
		return id < other.id;
	}
	bool operator>(const Entity & other) const {
		return id > other.id;
	}
};

class System {
private:
	Signature componentSignature;
	std::vector<Entity> entities;

public:
	System() = default;
	~System() = default;
	
	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;

	template <typename T> void RequireComponent();
};

class IPool {
public:
	virtual ~IPool() {}

};


template <typename T>
class Pool: public IPool {
private:
	std::vector<T> data;

public:
	Pool(int size = 100) {
		data.resize(size);
	}

	virtual ~Pool() = default;

	bool isEmpty() const{
		return data.empty();
	}
	int GetSize() const {
		return data.size();
	}

	void Resize(int n) {
		data.resize(n);
	}

	void Clear() {
		data.clear();
	}

	void Add(T object) {
		data.push_back(object);
	}

	void Set(int index, T object) {
		data[index] = object;
	}

	T& Get(int index) const {
		return static_cast<T&>(data[index]);
	}

	T& operator[](int index) const {
		return static_cast<T&>(data[index]);
	}

};

/*
* Registry�� ������ ���� ������ �Ѵ�.
* 1. ��ƼƼ ������ �ı��� �����Ѵ�.
* 2. ��ƼƼ�� ������Ʈ, �ý����� �߰��ϴ°��� �����Ѵ�.
*/
class Registry { 
private:
	int numEntities = 0;

	/*
	 * componentPools �迭�� ������ ��Ÿ���� ǥ:
	 * �� entity ��: 2
	 * �� component ����: 3
	 * +-------------+-------------+-------------+-------------+--------+
	 * | component/poolIndex	|    [0]		|    [1]    |    [2]	|
	 * +-------------+-------------+-------------+-------------+--------+
	 * | entity 1				|     nullptr	|   {...}	|   nullptr	|
	 * | entity 2				|     nullptr	|   {...}   |   {...}	|
	 * +-------------+-------------+-------------+-------------+--------+
	 *
	 * ����:
	 * componentPools ���� �迭���� componentPool���� ������ �ֽ��ϴ�.
	 * componentPool�� �� ������Ʈ ������ ������ �ֽ��ϴ�. componentPool�� �ε����� ������Ʈ�� id�� �����ϴ�.
	 * componentPool�� ���ؼ� Ư�� ��ƼƼ�� ������ �ִ� ������Ʈ�� ������ �� �մ� ���̺����Դϴ�.
	 * 
	 * ��ƼƼ�� ������Ʈ�� ������ �ִ��� ��ȸ�ϴ°� entityComponentSignatures�� ���ؼ� �ϴ°��� �� ���մϴ�.
	 * 
	*/
	std::vector<IPool*> componentPools;
	std::vector<Signature> entityComponentSignatures;
	std::unordered_map<std::type_index, System&> systems;

	std::vector<Entity> entitiesToBeAdded;
	std::vector<Entity> entitiesToBeKilled;
public:
	Registry() = default;
	~Registry() = default;

	Entity CreateEntity();
	void Update();
	void AddEntityToSystem(Entity entity);

	template <typename T, typename ...TArgs> void AddComponent(Entity entitiy, TArgs&& ...args);
	

};

template<typename T>
void System::RequireComponent()
{
	const auto componentId = Component<T>::GetId();
	componentSignature.set(componentId);
}

template <typename T, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&&... args) {
	const auto componentId = Component<T>::GetId();
	const auto entityId = entity.GetId();

	// ������Ʈ Ǯ�� entity �ڸ��� ������Ʈ Ǯ�� �ø���.
    if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	// ������Ʈ Ǯ�� ������ �����Ѵ�.
    if (componentPools[componentId] == nullptr) {
		Pool<T>* newComponentPool = new Pool<T>();
        componentPools[componentId] = newComponentPool;
	}

	Pool<T>* componentPool = componentPools[componentId];

	if(entityId >= componentPool->GetSize()) {
		componentPool->Resize(entityId + 1);
	}

	T newComponent = T(std::forward<TArgs>(args));
	componentPool->Set(entityId, newComponent);
	entityComponentSignatures[entityId].set(componentId);
}
