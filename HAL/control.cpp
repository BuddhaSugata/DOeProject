#include "control/pid_concept.h"
#include "HAL/angle.h"
#include "HAL/motor.h"


/**
 * @brief MUST be executed every 10 mS
 */
void control_task()
{   
    // Sync data from angle with Simulink Code
    pid_concept_U.y = read_angle();

    // Calculate model
    pid_concept_step();

    // Extract results
    motor_write(pid_concept_Y.u);
}
