//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "../SteeringAgent.h"
#include "../Obstacle.h"
#include "framework\EliteMath\EMatrix2x3.h"

//SEEK
//****
SteeringOutput Seek::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
    SteeringOutput steering = {};

    steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
    steering.LinearVelocity.Normalize();

    //std::cout << steering.LinearVelocity.y << std::endl;
    steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();

    if (pAgent->CanRenderBehavior())
        DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 });

    return steering;
}

//Flee

SteeringOutput Flee::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
    Elite::Vector2 fromTarget = pAgent->GetPosition() - m_Target.Position;
    float dis = fromTarget.Magnitude();

    SteeringOutput steering = {};

    if (dis > m_FleeRad)
    {
        steering.IsValid = false;
        return steering;
    }

    steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
    steering.LinearVelocity.Normalize();
    steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();
    steering.LinearVelocity = -steering.LinearVelocity;

    if (pAgent->CanRenderBehavior())
        DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 0,1,0 });


    return steering;
}

SteeringOutput Wander::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
    SteeringOutput steering = {};

    //Calculate the point on the given circle.
    float angle = Elite::randomFloat(m_MinAngleChange, m_MaxAngleChange);
    m_PreviousAngle += angle;
    //std::cout << "Calculated angle: " << Elite::ToDegrees(angle) << " Previous Angle: " << Elite::ToDegrees(m_PreviousAngle) << '\n';

    Elite::Vector2 pointOnCircle{ cos(m_PreviousAngle), sin(m_PreviousAngle) };
    pointOnCircle *= m_Radius;

    //Move circle in front of the agent with a set distance
    pointOnCircle += (pAgent->GetDirection() * m_OffsetDistance);

    //Normalize and set speed
    steering.LinearVelocity = pointOnCircle;
    steering.LinearVelocity.Normalize();
    steering.LinearVelocity *= pAgent->GetMaxLinearSpeed();
    Elite::Vector2 targetCircle{ pAgent->GetPosition() + (pAgent->GetDirection() * m_OffsetDistance)};

    if (pAgent->CanRenderBehavior())
    {
        DEBUGRENDERER2D->DrawCircle(targetCircle, m_Radius, { 0,0,1 }, 0);
        DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5, { 1,0,0 });
        DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), pAgent->GetDirection(), 5, {0,1,0});
        DEBUGRENDERER2D->DrawPoint(pAgent->GetPosition() + pointOnCircle, 5, {0.87f, 0.34f, 0.65f});
    }
    return steering;
}

SteeringOutput Evade::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
    return SteeringOutput();
}

SteeringOutput Arrive::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
    SteeringOutput arrive = {};
    Elite::Vector2 targetToAgent = m_Target.Position - pAgent->GetPosition();
    arrive.LinearVelocity = targetToAgent;
    arrive.LinearVelocity.Normalize();
    float distance = targetToAgent.Magnitude();

    if (distance < m_SlowRadius)
    {
        arrive.LinearVelocity *= pAgent->GetMaxLinearSpeed() * (distance / m_SlowRadius);
        if (distance < m_ArrivalRadius)
            arrive.LinearVelocity = { 0.f,0.f };
    }
    else
        arrive.LinearVelocity *= pAgent->GetMaxLinearSpeed();

    if (pAgent->CanRenderBehavior())
        DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), arrive.LinearVelocity, 5, { 0,1,0 });

    return arrive;
}
