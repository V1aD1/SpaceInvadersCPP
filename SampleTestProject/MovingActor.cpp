#include "MovingActor.h"

MovingActor::MovingActor(int id, float speed, std::pair<int, int> direction, std::pair<float, float> position, gfx::Texture* texture)
{
	Id = id;
	Speed = speed;
	Direction = direction;
	Position = position;
	ActorTexture = texture;
	Alive = true;
}

//this function can be overridden 
void MovingActor::Move(float delta)
{
	Position.first += delta * Speed * Direction.first;
	Position.second += delta * Speed * Direction.second;
}

bool MovingActor::IsCollidingWith(MovingActor * actor)
{
	std::list<std::pair<float, float>> myCorners{ std::pair<float,float>(Position.first, Position.second),
											  std::pair<float,float>(Position.first + ActorTexture->Width(), Position.second),
											  std::pair<float, float>(Position.first, Position.second + ActorTexture->Height()),
											  std::pair<float, float>(Position.first + ActorTexture->Width(), 
																 Position.second + ActorTexture->Height()) };
	
	std::pair<float, float> otherActorPos = actor->GetPosition();
	gfx::Texture* otherActorTex = actor->GetTexture();
	
	for each (auto myCorner in myCorners)
	{
		//x coords are within range
		if (myCorner.first >= otherActorPos.first && myCorner.first <= otherActorPos.first + otherActorTex->Width())
		{
			//y coords are within range
			if (myCorner.second >= otherActorPos.second && myCorner.second <= otherActorPos.second + otherActorTex->Height())
				return true;
		}
	}

	return false;
}

int MovingActor::GetId()
{
	return Id;
}

float MovingActor::GetSpeed()
{
	return Speed;
}

bool MovingActor::IsAlive()
{
	return Alive;
}

std::pair<float, float> MovingActor::GetPosition()
{
	return Position;
}

std::pair<int, int> MovingActor::GetDirection()
{
	return Direction;
}

gfx::Texture * MovingActor::GetTexture()
{
	return ActorTexture;
}

void MovingActor::SetDirection(std::pair<int, int> newDir)
{
	Direction = newDir;
}

void MovingActor::SetSpeed(float newSpeed)
{
	Speed = newSpeed;
}

MovingActor::~MovingActor()
{
}
