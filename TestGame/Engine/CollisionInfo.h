#pragma once

namespace MustacheEngine
{
	class ColliderComponent;

	struct CollisionInfo
	{
		ColliderComponent* First;
		ColliderComponent* Second;
		bool IsOngoing;

		bool IfMatch(ColliderComponent* f, ColliderComponent* s) { return (First == f && Second == s) || (First == s && Second == f); }
	};
}