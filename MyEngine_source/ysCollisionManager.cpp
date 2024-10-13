#include "ysCollisionManager.h"
#include "ysSceneManager.h"
#include <ysTransform.h>
#include <ysInputManager.h>
namespace ys
{
	std::bitset<(UINT)LayerType::Max> CollisionManager::collisionLayerMatrix[(UINT)LayerType::Max]{};
	std::unordered_map<UINT64, bool> CollisionManager::collisionMap{};

	void CollisionManager::Init()
	{
	}
	void CollisionManager::Update()
	{
		if (InputManager::getKeyDown(';'))
		{
			Collider::SetRender(!Collider::isRender());
		}

		auto scene = SceneManager::GetActiveScene();

		for (UINT row = 0; row < (UINT)LayerType::Max; ++row)
			for (UINT col = row; col < (UINT)LayerType::Max; ++col)
				if (collisionLayerMatrix[row][col] == true)
					LayerCollision(scene, static_cast<LayerType>(row), static_cast<LayerType>(col));
	}
	void CollisionManager::LateUpdate()
	{
	}
	void CollisionManager::Render(HDC hDC)
	{
	}
	void CollisionManager::CollisionLayerCheck(const LayerType& left/*col*/, const LayerType& right/*row*/, const bool& enable)
	{
		int row = 0; int col = 0;
		if (left <= right)
		{
			row = (UINT)left;
			col = (UINT)right;
		}
		else
		{
			row = (UINT)right;
			col = (UINT)left;
		}

		collisionLayerMatrix[row][col] = enable;
	}
	void CollisionManager::LayerCollision(Scene* scene, const LayerType& left, const LayerType& right)
	{
		const auto& lefts = SceneManager::GetGameObejcts(left);
		const auto& rights = SceneManager::GetGameObejcts(right);

		for(auto left : lefts)
		{
			if (left->IsActive() == false) continue;
			auto leftCD = left->GetComponent<Collider>();
			if (leftCD == nullptr) continue;

			for (auto right : rights)
			{
				if (right->IsActive() == false) continue;
				auto rightCD = right->GetComponent<Collider>();
				if (rightCD == nullptr) continue;
				if (left == right) continue;

				ColliderCollision(leftCD, rightCD);
			}
		}
	}
	void CollisionManager::ColliderCollision(Collider* left, Collider* right)
	{
		CollideID id{};
		id.UpdateLeftID(left->GetID());
		id.UpdateRightID(right->GetID());

		auto iter = collisionMap.find(id.GetId());
		if (iter == collisionMap.end())
		{
			collisionMap.insert(std::make_pair(id.GetId(), false));
			iter = collisionMap.find(id.GetId());
		}

		if (Intersect(left, right))
		{
			if (iter->second == false)
			{
				left->OnCollisionEnter(right);
				right->OnCollisionEnter(left);
				iter->second = true;
				//ÄÝ¸®
			}
			else
			{
				left->OnCollisionStay(right);
				right->OnCollisionStay(left);
			}
		}
		else if(iter->second == true)
		{
			left->OnCollisionExit(right);
			right->OnCollisionExit(left);
			iter->second = false;
		}
	}

	bool CollisionManager::Intersect(Collider* left, Collider* right)
	{
		auto LTr = left->GetOwner()->GetComponent<Transform>();
		auto RTr = right->GetOwner()->GetComponent<Transform>();

		auto LPosition = LTr->GetPosition() + left->GetOffset();
		auto RPosition = RTr->GetPosition() + right->GetOffset();

		auto LSize = left->GetSize() * 100;
		auto RSize = right->GetSize() * 100;

		auto LType = left->GetColliderType();
		auto RType = right->GetColliderType();
		
		if(LType == enums::ColliderType::Box2D && RType == enums::ColliderType::Box2D)
		{
			auto LBoxCenter = LPosition + (LSize / 2.0f);
			auto RBoxCenter = RPosition + (RSize / 2.0f);
			if (fabs(LBoxCenter.x - RBoxCenter.x) < fabs(LSize.x / 2.0f + RSize.x / 2.0f)
				&& fabs(LBoxCenter.y - RBoxCenter.y) < fabs(LSize.y / 2.0f + RSize.y / 2.0f))
			{
				return true;
			}
		}

		if (LType == enums::ColliderType::Circle2D && RType == enums::ColliderType::Circle2D)
		{
			auto LCicleCenter = LPosition + (LSize / 2.0f);
			auto RCicleCenter = RPosition + (RSize / 2.0f);
			float distance = (LCicleCenter - RCicleCenter).scalar();
			if (distance < ((LSize.x / 2.0f) + (RSize.x / 2.0f)))
			{
				return true;
			}
		}

		if (LType == enums::ColliderType::Box2D && RType == enums::ColliderType::Circle2D)
		{
			auto CicleCenter = RPosition + (RSize / 2.0f);
			auto Lrb = LPosition + LSize + (RSize / 2.0f);
			auto Llt = LPosition - (RSize / 2.0f);
			if (CicleCenter.x > Llt.x && CicleCenter.x < Lrb.x
				&& CicleCenter.y > Llt.y && CicleCenter.y < Lrb.y)
			{
				return true;
			}
		}
		if(LType == enums::ColliderType::Circle2D && RType == enums::ColliderType::Box2D)
		{
			auto CicleCenter = LPosition + (LSize / 2.0f);
			auto Rrb = RPosition + RSize + (LSize / 2.0f);
			auto Rlt = RPosition - (LSize / 2.0f);
			if (CicleCenter.x > Rlt.x && CicleCenter.x < Rrb.x
				&& CicleCenter.y > Rlt.y && CicleCenter.y < Rrb.y)
			{
				return true;
			}
		}

		return false;
	}
	void CollisionManager::Clear()
	{
		collisionMap.clear();
		collisionLayerMatrix->reset();
	}
}