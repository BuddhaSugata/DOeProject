/*
 * File: pid_concept.h
 *
 * Code generated for Simulink model 'pid_concept'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 9.4 (R2020b) 29-Jul-2020
 * C/C++ source code generated on : Thu Mar 24 15:31:41 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Custom Processor->Custom Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_pid_concept_h_
#define RTW_HEADER_pid_concept_h_
#include <string.h>
#ifndef pid_concept_COMMON_INCLUDES_
#define pid_concept_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* pid_concept_COMMON_INCLUDES_ */

#include "pid_concept_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T Sum;                          /* '<S2>/Sum' */
} B_pid_concept_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Internal_CSTATE;              /* '<S1>/Internal' */
} X_pid_concept_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Internal_CSTATE;              /* '<S1>/Internal' */
} XDot_pid_concept_T;

/* State disabled  */
typedef struct {
  boolean_T Internal_CSTATE;           /* '<S1>/Internal' */
} XDis_pid_concept_T;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T y;                            /* '<Root>/y' */
} ExtU_pid_concept_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T u;                            /* '<Root>/u' */
} ExtY_pid_concept_T;

/* Real-time Model Data Structure */
struct tag_RTM_pid_concept_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_pid_concept_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[1];
  real_T odeF[3][1];
  ODE3_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block signals (default storage) */
extern B_pid_concept_T pid_concept_B;

/* Continuous states (default storage) */
extern X_pid_concept_T pid_concept_X;

/* External inputs (root inport signals with default storage) */
extern ExtU_pid_concept_T pid_concept_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_pid_concept_T pid_concept_Y;

/* Model entry point functions */
extern void pid_concept_initialize(void);
extern void pid_concept_step(void);
extern void pid_concept_terminate(void);

/* Real-time Model object */
extern RT_MODEL_pid_concept_T *const pid_concept_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'pid_concept'
 * '<S1>'   : 'pid_concept/LTI System'
 * '<S2>'   : 'pid_concept/Subsystem'
 * '<S3>'   : 'pid_concept/LTI System/IO Delay'
 * '<S4>'   : 'pid_concept/LTI System/Input Delay'
 * '<S5>'   : 'pid_concept/LTI System/Output Delay'
 */
#endif                                 /* RTW_HEADER_pid_concept_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
