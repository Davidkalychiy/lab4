
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

void increase_counters(void);

void _INIT ProgramInit(void)
{
	//������������� ���� SDC �������
	AxisZ_HW.EncIf1_Typ = ncSDC_ENC16;
	AxisZ_HW.DiDoIf_Typ = ncSDC_DIDO;
	AxisZ_HW.DrvIf_Typ = ncSDC_DRVSERVO16;
	
	//������������� ����� ����������
	strcpy(AxisZ_HW.EncIf1_Name, "AxisZ_EncIf");
	strcpy(AxisZ_HW.DrvIf_Name, "AxisZ_DrvIf");
	strcpy(AxisZ_HW.DiDoIf_Name, "AxisZ_DiDoIf");
		
	//������������� ����� ���������� � ���������� ������
	AxisZ_EncIf.iEncOK = 1;
	AxisZ_DrvIf.iDrvOK = 1;
	AxisZ_DrvIf.iStatusEnable = 1;
	AxisZ_DiDoIf.iDriveReady = 1;

	fb_controller.dt = 0.002;
	fb_controller.k_i = 0.16;
	fb_controller.k_p = 0.0064;
	fb_controller.max_abs_value = 24.0;
	
	pwm_period = 200;
	direction = 0;
}

void _CYCLIC ProgramCyclic(void)
{	
	coil_pwm_value = 32000;
	increase_counters();
	
	//������������ ��������� �������
	
	AxisZ_EncIf.iActTime = (INT)AsIOTimeCyclicStart();
	AxisZ_EncIf.iActPos = axis_Z.counter;
	AxisZ_DiDoIf.iPosHwEnd = axis_Z.endswitch_a_reached;
	AxisZ_DiDoIf.iNegHwEnd = axis_Z.endswitch_b_reached;
	fb_controller.in = AxisZ_DrvIf.oSetPos;
	
	if((fb_controller.in<min_task)&&(fb_controller.in>(-min_task))) fb_controller.in = 0; 
	fb_controller.in = fb_controller.in * 6500/32767 - axis_Z.speed;

	FB_Controller(&fb_controller);

	if(direction) axis_Z.pwm_value = coil_powered_Z ? -32767 : 0;
	else axis_Z.pwm_value = coil_powered_Z ? 32767 : 0;
	FB_Axis(&axis_Z);
}

void increase_counters(void)
{
	AxisZ_EncIf.iLifeCnt++;
	AxisZ_DiDoIf.iLifeCntDriveEnable++; 
	AxisZ_DiDoIf.iLifeCntDriveReady++; 
	AxisZ_DiDoIf.iLifeCntNegHwEnd++; 
	AxisZ_DiDoIf.iLifeCntPosHwEnd++; 
	AxisZ_DiDoIf.iLifeCntReference++;
	AxisZ_DrvIf.iLifeCnt++;
}

void _EXIT ProgramExit(void)
{
	// Insert code here 

}

