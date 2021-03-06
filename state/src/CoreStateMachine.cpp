//
// File: CoreStateMachine.cpp
//
// Code generated for Simulink model 'CoreStateMachine'.
//
// Model version                  : 1.74
// Simulink Coder version         : 8.11 (R2016b) 25-Aug-2016
// C/C++ source code generated on : Wed Aug 01 15:34:08 2018
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "state/CoreStateMachine.h"
#include "state/CoreStateMachine_private.h"

// Named constants for Chart: '<S2>/AMCL_Interface'
#define CoreStateM_IN_SendToughLocation ((uint8_T)5U)
#define CoreStateMac_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define CoreStateMachin_IN_SendInitPose ((uint8_T)4U)
#define CoreStateMachine_IN_AMCLDisconn ((uint8_T)1U)
#define CoreStateMachine_IN_AMCLReady  ((uint8_T)2U)
#define CoreStateMachine_IN_Error      ((uint8_T)3U)
#define CoreStateMachine_IN_UpdatePose ((uint8_T)6U)

// Named constants for Chart: '<S2>/DetectState'
#define CoreStateMac_IN_DataPreparation ((uint8_T)1U)
#define CoreStateMachi_IN_DetectDisconn ((uint8_T)4U)
#define CoreStateMachine_IN_DetectCal  ((uint8_T)2U)
#define CoreStateMachine_IN_DetectCalOK ((uint8_T)3U)
#define CoreStateMachine_IN_DetectReady ((uint8_T)5U)

// Named constants for Chart: '<S1>/NavState'
#define CoreStateMachine_IN_NavCalib   ((uint8_T)1U)
#define CoreStateMachine_IN_NavDisconn ((uint8_T)2U)
#define CoreStateMachine_IN_NavOK      ((uint8_T)3U)
#define CoreStateMachine_IN_NavReady   ((uint8_T)4U)

// Named constants for Chart: '<S1>/StateMachine'
#define CoreStateMa_IN_GlobalLocationOK ((uint8_T)1U)
#define CoreStateMa_IN_GlobalLocationOn ((uint8_T)2U)
#define CoreStateMach_IN_Initialization ((uint8_T)3U)
#define CoreStateMachin_IN_SimuLocation ((uint8_T)6U)
#define CoreStateMachine_IN_PCReady    ((uint8_T)4U)
#define CoreStateMachine_IN_PLCReady   ((uint8_T)5U)

// Block states (auto storage)
DW_CoreStateMachine_T CoreStateMachine_DW;

// External inputs (root inport signals with auto storage)
ExtU_CoreStateMachine_T CoreStateMachine_U;

// External outputs (root outports fed by signals with auto storage)
ExtY_CoreStateMachine_T CoreStateMachine_Y;

// Real-time model
RT_MODEL_CoreStateMachine_T CoreStateMachine_M_;
RT_MODEL_CoreStateMachine_T *const CoreStateMachine_M = &CoreStateMachine_M_;

// Model step function
void CoreStateMachine_step(void)
{
	int32_T tmp;

	// Chart: '<S2>/AMCL_Interface' incorporates:
	//   Inport: '<Root>/amcl_conn'
	//   Inport: '<Root>/cov_is_small'
	//   Memory: '<S1>/Memory2'

	// Gateway: CoreStateMachine/Global Location/AMCL_Interface
	// During: CoreStateMachine/Global Location/AMCL_Interface
	if (CoreStateMachine_DW.is_active_c4_CoreStateMachine == 0U) {
		// Entry: CoreStateMachine/Global Location/AMCL_Interface
		CoreStateMachine_DW.is_active_c4_CoreStateMachine = 1U;

		// Entry Internal: CoreStateMachine/Global Location/AMCL_Interface
		// Transition: '<S5>:5'
		CoreStateMachine_DW.is_c4_CoreStateMachine = CoreStateMachine_IN_AMCLDisconn;
	} else {
		switch (CoreStateMachine_DW.is_c4_CoreStateMachine) {
		case CoreStateMachine_IN_AMCLDisconn:
			// During 'AMCLDisconn': '<S5>:4'
			if (CoreStateMachine_U.amcl_conn) {
				// Transition: '<S5>:15'
				CoreStateMachine_DW.is_c4_CoreStateMachine =
					CoreStateMachine_IN_AMCLReady;
			} else {
				// Outport: '<Root>/amcl_status'
				CoreStateMachine_Y.amcl_status = 4U;
			}
			break;

		case CoreStateMachine_IN_AMCLReady:
			// During 'AMCLReady': '<S5>:1'
			if (!CoreStateMachine_U.amcl_conn) {
				// Transition: '<S5>:16'
				CoreStateMachine_DW.is_c4_CoreStateMachine =
					CoreStateMachine_IN_AMCLDisconn;
			} else if (CoreStateMachine_Y.sys_status == 2) {
				// Transition: '<S5>:21'
				CoreStateMachine_DW.is_c4_CoreStateMachine =
					CoreStateMachin_IN_SendInitPose;

				// Entry 'SendInitPose': '<S5>:2'
				CoreStateMachine_DW.send_count = 0U;
			} else {
				// Outport: '<Root>/amcl_status'
				CoreStateMachine_Y.amcl_status = 0U;
			}
			break;

		case CoreStateMachine_IN_Error:
			// During 'Error': '<S5>:28'
			// Transition: '<S5>:35'
			CoreStateMachine_DW.is_c4_CoreStateMachine = CoreStateMachine_IN_AMCLReady;
			break;

		case CoreStateMachin_IN_SendInitPose:
			// During 'SendInitPose': '<S5>:2'
			if (!CoreStateMachine_U.amcl_conn) {
				// Transition: '<S5>:19'
				CoreStateMachine_DW.is_c4_CoreStateMachine =
					CoreStateMachine_IN_AMCLDisconn;
			} else if (CoreStateMachine_Y.sys_status != 2) {
				// Transition: '<S5>:23'
				CoreStateMachine_DW.is_c4_CoreStateMachine =
					CoreStateMachine_IN_AMCLReady;
			} else if (CoreStateMachine_DW.send_count >= 3) {
				// Transition: '<S5>:25'
				CoreStateMachine_DW.is_c4_CoreStateMachine =
					CoreStateMachine_IN_UpdatePose;

				// Entry 'UpdatePose': '<S5>:3'
				CoreStateMachine_DW.update_count = 0U;
			} else {
				// Outport: '<Root>/amcl_status'
				CoreStateMachine_Y.amcl_status = 1U;
				tmp = CoreStateMachine_DW.send_count + 1;
				if (tmp > 65535) {
					tmp = 65535;
				}

				CoreStateMachine_DW.send_count = (uint16_T)tmp;
			}
			break;

		case CoreStateM_IN_SendToughLocation:
			// During 'SendToughLocation': '<S5>:6'
			if (!CoreStateMachine_U.amcl_conn) {
				// Transition: '<S5>:17'
				CoreStateMachine_DW.is_c4_CoreStateMachine =
					CoreStateMachine_IN_AMCLDisconn;
			} else if (CoreStateMachine_Y.sys_status != 2) {
				// Transition: '<S5>:32'
				CoreStateMachine_DW.is_c4_CoreStateMachine =
					CoreStateMachine_IN_AMCLReady;
			} else {
				// Outport: '<Root>/amcl_status'
				CoreStateMachine_Y.amcl_status = 3U;
			}
			break;

		default:
			// During 'UpdatePose': '<S5>:3'
			if (!CoreStateMachine_U.amcl_conn) {
				// Transition: '<S5>:18'
				CoreStateMachine_DW.is_c4_CoreStateMachine =
					CoreStateMachine_IN_AMCLDisconn;
			} else if (CoreStateMachine_DW.update_count > 200) {
				// Transition: '<S5>:29'
				CoreStateMachine_DW.is_c4_CoreStateMachine = CoreStateMachine_IN_Error;
			} else if (CoreStateMachine_U.cov_is_small) {
				// Transition: '<S5>:31'
				CoreStateMachine_DW.is_c4_CoreStateMachine =
					CoreStateM_IN_SendToughLocation;
			} else {
				// Outport: '<Root>/amcl_status'
				CoreStateMachine_Y.amcl_status = 2U;
				tmp = CoreStateMachine_DW.update_count + 1;
				if (tmp > 65535) {
					tmp = 65535;
				}

				CoreStateMachine_DW.update_count = (uint16_T)tmp;
			}
			break;
		}
	}

	// End of Chart: '<S2>/AMCL_Interface'

	// Chart: '<S2>/DetectState' incorporates:
	//   Inport: '<Root>/detect_conn'
	//   Inport: '<Root>/filter_count'
	//   Inport: '<Root>/is_calcd'
	//   Memory: '<S1>/Memory2'

	// Gateway: CoreStateMachine/Global Location/DetectState
	// During: CoreStateMachine/Global Location/DetectState
	if (CoreStateMachine_DW.is_active_c3_CoreStateMachine == 0U) {
		// Entry: CoreStateMachine/Global Location/DetectState
		CoreStateMachine_DW.is_active_c3_CoreStateMachine = 1U;

		// Entry Internal: CoreStateMachine/Global Location/DetectState
		// Transition: '<S6>:36'
		CoreStateMachine_DW.is_c3_CoreStateMachine = CoreStateMachi_IN_DetectDisconn;
	} else {
		switch (CoreStateMachine_DW.is_c3_CoreStateMachine) {
		case CoreStateMac_IN_DataPreparation:
			// During 'DataPreparation': '<S6>:4'
			if (!CoreStateMachine_U.detect_conn) {
				// Transition: '<S6>:41'
				CoreStateMachine_DW.is_c3_CoreStateMachine =
					CoreStateMachi_IN_DetectDisconn;
			} else if (CoreStateMachine_Y.sys_status != 2) {
				// Transition: '<S6>:10'
				CoreStateMachine_DW.is_c3_CoreStateMachine =
					CoreStateMachine_IN_DetectReady;
			} else if ((CoreStateMachine_U.filter_count >=
				    CoreStateMachine_DW.cal_count) &&
				   (CoreStateMachine_Y.amcl_status == 3)) {
				// Transition: '<S6>:30'
				CoreStateMachine_DW.is_c3_CoreStateMachine =
					CoreStateMachine_IN_DetectCal;

				// Outport: '<Root>/calc_request'
				// Entry 'DetectCal': '<S6>:27'
				CoreStateMachine_Y.calc_request = true;
			} else {
				// Outport: '<Root>/detect_status'
				CoreStateMachine_Y.detect_status = 2U;
			}
			break;

		case CoreStateMachine_IN_DetectCal:
			// During 'DetectCal': '<S6>:27'
			if (!CoreStateMachine_U.detect_conn) {
				// Outport: '<Root>/calc_request'
				// Transition: '<S6>:42'
				// Exit 'DetectCal': '<S6>:27'
				CoreStateMachine_Y.calc_request = false;
				CoreStateMachine_DW.is_c3_CoreStateMachine =
					CoreStateMachi_IN_DetectDisconn;
			} else if (CoreStateMachine_U.is_calcd) {
				// Outport: '<Root>/calc_request'
				// Transition: '<S6>:33'
				// Exit 'DetectCal': '<S6>:27'
				CoreStateMachine_Y.calc_request = false;
				CoreStateMachine_DW.is_c3_CoreStateMachine =
					CoreStateMachine_IN_DetectCalOK;
			} else {
				// Outport: '<Root>/detect_status'
				CoreStateMachine_Y.detect_status = 3U;
			}
			break;

		case CoreStateMachine_IN_DetectCalOK:
			// During 'DetectCalOK': '<S6>:5'
			if (!CoreStateMachine_U.detect_conn) {
				// Transition: '<S6>:40'
				CoreStateMachine_DW.is_c3_CoreStateMachine =
					CoreStateMachi_IN_DetectDisconn;
			} else if ((CoreStateMachine_Y.sys_status == 3) ||
				   (CoreStateMachine_Y.sys_status == 0) ||
				   (CoreStateMachine_Y.sys_status == 1)) {
				// Transition: '<S6>:14'
				CoreStateMachine_DW.is_c3_CoreStateMachine =
					CoreStateMachine_IN_DetectReady;
			} else {
				// Outport: '<Root>/detect_status'
				CoreStateMachine_Y.detect_status = 4U;
			}
			break;

		case CoreStateMachi_IN_DetectDisconn:
			// During 'DetectDisconn': '<S6>:35'
			if (CoreStateMachine_U.detect_conn) {
				// Transition: '<S6>:37'
				CoreStateMachine_DW.is_c3_CoreStateMachine =
					CoreStateMachine_IN_DetectReady;
			} else {
				// Outport: '<Root>/detect_status'
				CoreStateMachine_Y.detect_status = 0U;
			}
			break;

		default:
			// During 'DetectReady': '<S6>:1'
			if (!CoreStateMachine_U.detect_conn) {
				// Transition: '<S6>:38'
				CoreStateMachine_DW.is_c3_CoreStateMachine =
					CoreStateMachi_IN_DetectDisconn;
			} else if (CoreStateMachine_Y.sys_status == 2) {
				// Transition: '<S6>:9'
				CoreStateMachine_DW.is_c3_CoreStateMachine =
					CoreStateMac_IN_DataPreparation;

				// Entry 'DataPreparation': '<S6>:4'
				tmp = CoreStateMachine_U.filter_count + CoreStateMachine_P.counter_times;
				if (tmp > 65535) {
					tmp = 65535;
				}

				CoreStateMachine_DW.cal_count = (uint16_T)tmp;
			} else {
				// Outport: '<Root>/detect_status'
				CoreStateMachine_Y.detect_status = 1U;
			}
			break;
		}
	}

	// End of Chart: '<S2>/DetectState'

	// Chart: '<S1>/NavState' incorporates:
	//   Inport: '<Root>/nav_conn'
	//   Memory: '<S1>/Memory2'

	// Gateway: CoreStateMachine/NavState
	// During: CoreStateMachine/NavState
	if (CoreStateMachine_DW.is_active_c2_CoreStateMachine == 0U) {
		// Entry: CoreStateMachine/NavState
		CoreStateMachine_DW.is_active_c2_CoreStateMachine = 1U;

		// Entry Internal: CoreStateMachine/NavState
		// Transition: '<S3>:15'
		CoreStateMachine_DW.is_c2_CoreStateMachine = CoreStateMachine_IN_NavDisconn;

		// Outport: '<Root>/nav_status'
		// Entry 'NavDisconn': '<S3>:14'
		CoreStateMachine_Y.nav_status = 3U;
	} else {
		switch (CoreStateMachine_DW.is_c2_CoreStateMachine) {
		case CoreStateMachine_IN_NavCalib:
			// During 'NavCalib': '<S3>:3'
			if (!CoreStateMachine_U.nav_conn) {
				// Transition: '<S3>:20'
				CoreStateMachine_DW.is_c2_CoreStateMachine =
					CoreStateMachine_IN_NavDisconn;

				// Outport: '<Root>/nav_status'
				// Entry 'NavDisconn': '<S3>:14'
				CoreStateMachine_Y.nav_status = 3U;
			} else if (CoreStateMachine_Y.sys_status != 2) {
				// Transition: '<S3>:6'
				CoreStateMachine_DW.is_c2_CoreStateMachine =
					CoreStateMachine_IN_NavReady;
			} else if (CoreStateMachine_Y.detect_status == 4) {
				// Transition: '<S3>:7'
				CoreStateMachine_DW.is_c2_CoreStateMachine = CoreStateMachine_IN_NavOK;
			} else {
				// Outport: '<Root>/nav_status'
				CoreStateMachine_Y.nav_status = 1U;
			}
			break;

		case CoreStateMachine_IN_NavDisconn:
			// During 'NavDisconn': '<S3>:14'
			if (CoreStateMachine_U.nav_conn) {
				// Transition: '<S3>:16'
				CoreStateMachine_DW.is_c2_CoreStateMachine =
					CoreStateMachine_IN_NavReady;
			}
			break;

		case CoreStateMachine_IN_NavOK:
			// During 'NavOK': '<S3>:2'
			if (!CoreStateMachine_U.nav_conn) {
				// Transition: '<S3>:19'
				CoreStateMachine_DW.is_c2_CoreStateMachine =
					CoreStateMachine_IN_NavDisconn;

				// Outport: '<Root>/nav_status'
				// Entry 'NavDisconn': '<S3>:14'
				CoreStateMachine_Y.nav_status = 3U;
			} else if ((CoreStateMachine_Y.sys_status == 0) ||
				   (CoreStateMachine_Y.sys_status == 1)) {
				// Transition: '<S3>:8'
				CoreStateMachine_DW.is_c2_CoreStateMachine =
					CoreStateMachine_IN_NavReady;
			} else if ((CoreStateMachine_Y.sys_status == 2) &&
				   (CoreStateMachine_Y.detect_status != 4)) {
				// Transition: '<S3>:9'
				CoreStateMachine_DW.is_c2_CoreStateMachine =
					CoreStateMachine_IN_NavCalib;
			} else {
				// Outport: '<Root>/nav_status'
				CoreStateMachine_Y.nav_status = 2U;
			}
			break;

		default:
			// During 'NavReady': '<S3>:1'
			if (!CoreStateMachine_U.nav_conn) {
				// Transition: '<S3>:17'
				CoreStateMachine_DW.is_c2_CoreStateMachine =
					CoreStateMachine_IN_NavDisconn;

				// Outport: '<Root>/nav_status'
				// Entry 'NavDisconn': '<S3>:14'
				CoreStateMachine_Y.nav_status = 3U;
			} else if (CoreStateMachine_Y.sys_status == 2) {
				// Transition: '<S3>:5'
				CoreStateMachine_DW.is_c2_CoreStateMachine =
					CoreStateMachine_IN_NavCalib;
			} else {
				// Outport: '<Root>/nav_status'
				CoreStateMachine_Y.nav_status = 0U;
			}
			break;
		}
	}

	// End of Chart: '<S1>/NavState'

	// Chart: '<S1>/StateMachine' incorporates:
	//   Inport: '<Root>/lidar_conn'
	//   Inport: '<Root>/recv_msg'
	//   Inport: '<Root>/tcp_conn'

	// Gateway: CoreStateMachine/StateMachine
	// During: CoreStateMachine/StateMachine
	if (CoreStateMachine_DW.is_active_c1_CoreStateMachine == 0U) {
		// Entry: CoreStateMachine/StateMachine
		CoreStateMachine_DW.is_active_c1_CoreStateMachine = 1U;

		// Entry Internal: CoreStateMachine/StateMachine
		// Transition: '<S4>:9'
		CoreStateMachine_DW.is_c1_CoreStateMachine = CoreStateMach_IN_Initialization;

		// Outport: '<Root>/sys_status'
		// Entry 'Initialization': '<S4>:8'
		CoreStateMachine_Y.sys_status = 0U;
	} else {
		switch (CoreStateMachine_DW.is_c1_CoreStateMachine) {
		case CoreStateMa_IN_GlobalLocationOK:
			// During 'GlobalLocationOK': '<S4>:12'
			if ((!CoreStateMachine_U.lidar_conn) || (!CoreStateMachine_U.tcp_conn) ||
			    (CoreStateMachine_Y.detect_status == 0) ||
			    (CoreStateMachine_Y.nav_status == 3)) {
				// Transition: '<S4>:21'
				CoreStateMachine_DW.is_c1_CoreStateMachine =
					CoreStateMach_IN_Initialization;

				// Outport: '<Root>/sys_status'
				// Entry 'Initialization': '<S4>:8'
				CoreStateMachine_Y.sys_status = 0U;
			} else {
				if ((CoreStateMachine_Y.nav_status == 2) && (CoreStateMachine_U.recv_msg
									     == 2)) {
					// Transition: '<S4>:22'
					CoreStateMachine_DW.is_c1_CoreStateMachine =
						CoreStateMachin_IN_SimuLocation;

					// Outport: '<Root>/sys_status'
					// Entry 'SimuLocation': '<S4>:13'
					CoreStateMachine_Y.sys_status = 4U;
				}
			}
			break;

		case CoreStateMa_IN_GlobalLocationOn:
			// During 'GlobalLocationOn': '<S4>:11'
			if ((!CoreStateMachine_U.lidar_conn) || (!CoreStateMachine_U.tcp_conn) ||
			    (CoreStateMachine_Y.detect_status == 0) ||
			    (CoreStateMachine_Y.nav_status == 3)) {
				// Transition: '<S4>:18'
				CoreStateMachine_DW.is_c1_CoreStateMachine =
					CoreStateMach_IN_Initialization;

				// Outport: '<Root>/sys_status'
				// Entry 'Initialization': '<S4>:8'
				CoreStateMachine_Y.sys_status = 0U;
			} else {
				if (CoreStateMachine_Y.detect_status == 4) {
					// Transition: '<S4>:19'
					CoreStateMachine_DW.is_c1_CoreStateMachine =
						CoreStateMa_IN_GlobalLocationOK;

					// Outport: '<Root>/sys_status'
					// Entry 'GlobalLocationOK': '<S4>:12'
					CoreStateMachine_Y.sys_status = 3U;
				}
			}
			break;

		case CoreStateMach_IN_Initialization:
			// During 'Initialization': '<S4>:8'
			if (CoreStateMachine_U.lidar_conn && CoreStateMachine_U.tcp_conn &&
			    (CoreStateMachine_Y.detect_status == 1) &&
			    (CoreStateMachine_Y.nav_status == 0)) {
				// Transition: '<S4>:15'
				CoreStateMachine_DW.is_c1_CoreStateMachine = CoreStateMachine_IN_PCReady;

				// Outport: '<Root>/sys_status'
				// Entry 'PCReady': '<S4>:10'
				CoreStateMachine_Y.sys_status = 1U;
			}
			break;

		case CoreStateMachine_IN_PCReady:
			// During 'PCReady': '<S4>:10'
			if ((!CoreStateMachine_U.lidar_conn) || (!CoreStateMachine_U.tcp_conn) ||
			    (CoreStateMachine_Y.detect_status == 0) ||
			    (CoreStateMachine_Y.nav_status == 3)) {
				// Transition: '<S4>:16'
				CoreStateMachine_DW.is_c1_CoreStateMachine =
					CoreStateMach_IN_Initialization;

				// Outport: '<Root>/sys_status'
				// Entry 'Initialization': '<S4>:8'
				CoreStateMachine_Y.sys_status = 0U;
			} else {
				if (CoreStateMachine_U.recv_msg == 0) {
					// Transition: '<S4>:31'
					CoreStateMachine_DW.is_c1_CoreStateMachine =
						CoreStateMachine_IN_PLCReady;

					// Outport: '<Root>/sys_status'
					// Entry 'PLCReady': '<S4>:30'
					CoreStateMachine_Y.sys_status = 1U;
				}
			}
			break;

		case CoreStateMachine_IN_PLCReady:
			// During 'PLCReady': '<S4>:30'
			if ((!CoreStateMachine_U.lidar_conn) || (!CoreStateMachine_U.tcp_conn) ||
			    (CoreStateMachine_Y.detect_status == 0) ||
			    (CoreStateMachine_Y.nav_status == 3)) {
				// Transition: '<S4>:34'
				CoreStateMachine_DW.is_c1_CoreStateMachine =
					CoreStateMach_IN_Initialization;

				// Outport: '<Root>/sys_status'
				// Entry 'Initialization': '<S4>:8'
				CoreStateMachine_Y.sys_status = 0U;
			} else {
				if ((CoreStateMachine_Y.detect_status == 1) &&
				    (CoreStateMachine_Y.nav_status == 0) && (CoreStateMachine_U.recv_msg
									     == 1)) {
					// Transition: '<S4>:32'
					CoreStateMachine_DW.is_c1_CoreStateMachine =
						CoreStateMa_IN_GlobalLocationOn;

					// Outport: '<Root>/sys_status'
					// Entry 'GlobalLocationOn': '<S4>:11'
					CoreStateMachine_Y.sys_status = 2U;
				}
			}
			break;

		default:
			// During 'SimuLocation': '<S4>:13'
			if ((!CoreStateMachine_U.lidar_conn) || (!CoreStateMachine_U.tcp_conn) ||
			    (CoreStateMachine_Y.detect_status == 0) ||
			    (CoreStateMachine_Y.nav_status == 3)) {
				// Transition: '<S4>:23'
				CoreStateMachine_DW.is_c1_CoreStateMachine =
					CoreStateMach_IN_Initialization;

				// Outport: '<Root>/sys_status'
				// Entry 'Initialization': '<S4>:8'
				CoreStateMachine_Y.sys_status = 0U;
			} else {
				if (CoreStateMachine_U.recv_msg == 1) {
					// Transition: '<S4>:26'
					CoreStateMachine_DW.is_c1_CoreStateMachine =
						CoreStateMa_IN_GlobalLocationOn;

					// Outport: '<Root>/sys_status'
					// Entry 'GlobalLocationOn': '<S4>:11'
					CoreStateMachine_Y.sys_status = 2U;
				}
			}
			break;
		}
	}

	// End of Chart: '<S1>/StateMachine'
}

// Model initialize function
void CoreStateMachine_initialize(void)
{
	// Registration code

	// initialize error status
	rtmSetErrorStatus(CoreStateMachine_M, (NULL));

	// states (dwork)
	(void)memset((void *)&CoreStateMachine_DW, 0,
		     sizeof(DW_CoreStateMachine_T));

	// external inputs
	(void)memset((void *)&CoreStateMachine_U, 0, sizeof(ExtU_CoreStateMachine_T));

	// external outputs
	(void)memset((void *)&CoreStateMachine_Y, 0,
		     sizeof(ExtY_CoreStateMachine_T));

	// SystemInitialize for Chart: '<S2>/AMCL_Interface'
	CoreStateMachine_DW.is_active_c4_CoreStateMachine = 0U;
	CoreStateMachine_DW.is_c4_CoreStateMachine = CoreStateMac_IN_NO_ACTIVE_CHILD;
	CoreStateMachine_DW.send_count = 0U;
	CoreStateMachine_DW.update_count = 0U;

	// SystemInitialize for Outport: '<Root>/amcl_status' incorporates:
	//   SystemInitialize for Chart: '<S2>/AMCL_Interface'

	CoreStateMachine_Y.amcl_status = 0U;

	// SystemInitialize for Chart: '<S2>/DetectState'
	CoreStateMachine_DW.is_active_c3_CoreStateMachine = 0U;
	CoreStateMachine_DW.is_c3_CoreStateMachine = CoreStateMac_IN_NO_ACTIVE_CHILD;
	CoreStateMachine_DW.cal_count = 0U;

	// SystemInitialize for Outport: '<Root>/calc_request' incorporates:
	//   SystemInitialize for Chart: '<S2>/DetectState'

	CoreStateMachine_Y.calc_request = false;

	// SystemInitialize for Outport: '<Root>/detect_status' incorporates:
	//   SystemInitialize for Chart: '<S2>/DetectState'

	CoreStateMachine_Y.detect_status = 0U;

	// SystemInitialize for Chart: '<S1>/NavState'
	CoreStateMachine_DW.is_active_c2_CoreStateMachine = 0U;
	CoreStateMachine_DW.is_c2_CoreStateMachine = CoreStateMac_IN_NO_ACTIVE_CHILD;

	// SystemInitialize for Outport: '<Root>/nav_status' incorporates:
	//   SystemInitialize for Chart: '<S1>/NavState'

	CoreStateMachine_Y.nav_status = 0U;

	// SystemInitialize for Chart: '<S1>/StateMachine'
	CoreStateMachine_DW.is_active_c1_CoreStateMachine = 0U;
	CoreStateMachine_DW.is_c1_CoreStateMachine = CoreStateMac_IN_NO_ACTIVE_CHILD;

	// SystemInitialize for Outport: '<Root>/sys_status' incorporates:
	//   SystemInitialize for Chart: '<S1>/StateMachine'

	CoreStateMachine_Y.sys_status = 0U;
}

// Model terminate function
void CoreStateMachine_terminate(void)
{
	// (no terminate code required)
}

//
// File trailer for generated code.
//
// [EOF]
//
