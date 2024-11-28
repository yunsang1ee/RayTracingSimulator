#pragma once

namespace ys::enums
{
	enum class LayerType
	{
		None,
		BackGround,
		ChildObject,
		Object,
		PickedObject,
		Effect,
		Camera,
		Mouse,
		Max
	};

	enum class ColliderType
	{
		Box2D,
		Circle2D,
		End
	};

	enum class ResourceType
	{
		Texture,
		Mesh,
		Animation,
		AudioClip,
		Shader,
		Prefab,
		End
	};

	enum class AudioGroup
	{
		None,
		BackGorund,
		Player,
		Enemy,
		Effect,
		End
	};

	enum class ComponentType
	{
		Transform, 
		Collider,
		RigidBody,
		Script,
		SpriteRenderer,
		Animator,
		Camera,
		AudioListener,
		AudioSource,
		End
	};// Reference : Unity lifeCycle
}