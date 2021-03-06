#pragma once

#include "ai/primitive/catch_primitive.h"
#include "ai/primitive/chip_primitive.h"
#include "ai/primitive/direct_velocity_primitive.h"
#include "ai/primitive/directwheels_primitive.h"
#include "ai/primitive/kick_primitive.h"
#include "ai/primitive/move_primitive.h"
#include "ai/primitive/movespin_primitive.h"
#include "ai/primitive/pivot_primitive.h"

/**
 * This class provides an interface for all Primitive Visitors. The Visitor design pattern
 * allows us to perform operations on Primitive objects without needing to check which
 * concrete type it is with an if/else statement, and we don't need to pollute the
 * Primitive classes with information or functions that are specific to the task we
 * want to perform.
 */
class PrimitiveVisitor
{
   public:
    virtual ~PrimitiveVisitor() = default;

    /**
     * Visits a CatchPrimitive to perform an operation.
     *
     * @param catch_primitive The CatchPrimitive to visit
     */
    virtual void visit(const CatchPrimitive &catch_primtiive) = 0;

    /**
     * Visits a ChipPrimitive to perform an operation.
     *
     * @param chip_primitive The ChipPrimitive to visit
     */
    virtual void visit(const ChipPrimitive &chip_primtiive) = 0;

    /**
     * Visits a DirectVelocityPrimitive to perform an operation.
     *
     * @param direct_velocity_primitive The DirectVelocityPrimitive to visit
     */
    virtual void visit(const DirectVelocityPrimitive &direct_velocity_primtiive) = 0;

    /**
     * Visits a DirectWheelsPrimitive to perform an operation.
     *
     * @param direct_wheels_primitive The DirectWheelsPrimitive to visit
     */
    virtual void visit(const DirectWheelsPrimitive &direct_wheels_primtiive) = 0;

    /**
     * Visits a KickPrimitive to perform an operation.
     *
     * @param kick_primitive The KickPrimitive to visit
     */
    virtual void visit(const KickPrimitive &kick_primtiive) = 0;

    /**
     * Visits a MovePrimitive to perform an operation.
     *
     * @param move_primitive The MovePrimitive to visit
     */
    virtual void visit(const MovePrimitive &move_primitive) = 0;

    /**
     * Visits a MoveSpinPrimitive to perform an operation.
     *
     * @param move_spin_primitive The MoveSpinPrimitive to visit
     */
    virtual void visit(const MoveSpinPrimitive &move_spin_primitive) = 0;

    /**
     * Visits a PivotPrimitive to perform an operation.
     *
     * @param pivot_primitive The PivotPrimitive to visit
     */
    virtual void visit(const PivotPrimitive &pivot_primtiive) = 0;
};
