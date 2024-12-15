#pragma once
#include "CommonInclude.h"
#include "ysCollider.h"
#include "ysBVH.h"
#include "ysCircleCollider2D.h"
#include <unordered_map>

namespace ys
{
	using namespace enums;

	class CollideID
	{
	public:
		CollideID() = default;

		inline void UpdateLeftID(const UINT32& left) { id = static_cast<UINT64>(left) << 32; }
		inline void UpdateRightID(const UINT32& right) { id |= right; }
		inline UINT64 GetId() const { return id; }

		bool operator==(const CollideID& other) const { return id == other.id; }

	private:
		UINT64 id;
	};

	class CollisionManager
	{
	public:
		static void Init();
		static void Update();
		static void LateUpdate();
		static void Render(HDC hDC);

		static void CollisionLayerCheck(const LayerType& left
			, const LayerType& right
			, const bool& enable);

		static void LayerCollision(class Scene* scene
			, const LayerType& left
			, const LayerType& right);

		static void ColliderCollision(Collider* left, Collider* right);
		static bool Intersect(Collider* left, Collider* right);
		static void Clear();
	private:
		//상삼각만 사용
		static std::bitset<(UINT)LayerType::Max> collisionLayerMatrix[(UINT)LayerType::Max];
		static std::unordered_map<UINT64, bool> collisionMap;
	};
}

