/*
 * File: pid_concept.c
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

#include "pid_concept.h"
#include "pid_concept_private.h"

/* Block signals (default storage) */
B_pid_concept_T pid_concept_B;

/* Continuous states */
X_pid_concept_T pid_concept_X;

/* External inputs (root inport signals with default storage) */
ExtU_pid_concept_T pid_concept_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_pid_concept_T pid_concept_Y;

/* Real-time model */
static RT_MODEL_pid_concept_T pid_concept_M_;
RT_MODEL_pid_concept_T *const pid_concept_M = &pid_concept_M_;

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 1;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  pid_concept_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  pid_concept_step();
  pid_concept_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  pid_concept_step();
  pid_concept_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void pid_concept_step(void)
{
  if (rtmIsMajorTimeStep(pid_concept_M)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&pid_concept_M->solverInfo,
                          ((pid_concept_M->Timing.clockTick0+1)*
      pid_concept_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(pid_concept_M)) {
    pid_concept_M->Timing.t[0] = rtsiGetT(&pid_concept_M->solverInfo);
  }

  /* Sum: '<S2>/Sum' incorporates:
   *  Constant: '<S2>/Constant'
   *  Inport: '<Root>/y'
   */
  pid_concept_B.Sum = 0.0 - pid_concept_U.y;

  /* Outport: '<Root>/u' incorporates:
   *  StateSpace: '<S1>/Internal'
   */
  pid_concept_Y.u = 3.0 * pid_concept_X.Internal_CSTATE + 12.0 *
    pid_concept_B.Sum;
  if (rtmIsMajorTimeStep(pid_concept_M)) {
    rt_ertODEUpdateContinuousStates(&pid_concept_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++pid_concept_M->Timing.clockTick0;
    pid_concept_M->Timing.t[0] = rtsiGetSolverStopTime
      (&pid_concept_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      pid_concept_M->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void pid_concept_derivatives(void)
{
  XDot_pid_concept_T *_rtXdot;
  _rtXdot = ((XDot_pid_concept_T *) pid_concept_M->derivs);

  /* Derivatives for StateSpace: '<S1>/Internal' */
  _rtXdot->Internal_CSTATE = 0.0;
  _rtXdot->Internal_CSTATE += 4.0 * pid_concept_B.Sum;
}

/* Model initialize function */
void pid_concept_initialize(void)
{
  /* Registration code */
  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&pid_concept_M->solverInfo,
                          &pid_concept_M->Timing.simTimeStep);
    rtsiSetTPtr(&pid_concept_M->solverInfo, &rtmGetTPtr(pid_concept_M));
    rtsiSetStepSizePtr(&pid_concept_M->solverInfo,
                       &pid_concept_M->Timing.stepSize0);
    rtsiSetdXPtr(&pid_concept_M->solverInfo, &pid_concept_M->derivs);
    rtsiSetContStatesPtr(&pid_concept_M->solverInfo, (real_T **)
                         &pid_concept_M->contStates);
    rtsiSetNumContStatesPtr(&pid_concept_M->solverInfo,
      &pid_concept_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&pid_concept_M->solverInfo,
      &pid_concept_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&pid_concept_M->solverInfo,
      &pid_concept_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&pid_concept_M->solverInfo,
      &pid_concept_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&pid_concept_M->solverInfo, (&rtmGetErrorStatus
      (pid_concept_M)));
    rtsiSetRTModelPtr(&pid_concept_M->solverInfo, pid_concept_M);
  }

  rtsiSetSimTimeStep(&pid_concept_M->solverInfo, MAJOR_TIME_STEP);
  pid_concept_M->intgData.y = pid_concept_M->odeY;
  pid_concept_M->intgData.f[0] = pid_concept_M->odeF[0];
  pid_concept_M->intgData.f[1] = pid_concept_M->odeF[1];
  pid_concept_M->intgData.f[2] = pid_concept_M->odeF[2];
  pid_concept_M->contStates = ((X_pid_concept_T *) &pid_concept_X);
  rtsiSetSolverData(&pid_concept_M->solverInfo, (void *)&pid_concept_M->intgData);
  rtsiSetSolverName(&pid_concept_M->solverInfo,"ode3");
  rtmSetTPtr(pid_concept_M, &pid_concept_M->Timing.tArray[0]);
  pid_concept_M->Timing.stepSize0 = 0.01;

  /* InitializeConditions for StateSpace: '<S1>/Internal' */
  pid_concept_X.Internal_CSTATE = 0.0;
}

/* Model terminate function */
void pid_concept_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
