#include "pid.h"


/************************************************************************************ 
* @author�� ���L���� 
* @date : 2018/2/15 
* @fuction name��FUZZY_PID_CONTROL 
* @fuction description�� ģ������Ӧ�����㷨��Ϊ�˷������Ĭ��e��ec��[-3,3]���䣬 
* ����ı�e��ec��Χ����������������(Ke��Kec=N/emax)����������(Ku=umax/N)�����´���� 
*�����������������������Լ���Ȩƽ������ģ����PID����λ��ʽPID�㷨���㷨�����ο�����ӭ����

����ת��https://blog.csdn.net/a841771798/article/details/79323118

  ��лԭ���ߣ��ָ��ݱ���Ŀ����в����޸ģ�

  �����õ�����ʽ��pid������pid�����ԭ����ͬ  2018/6/10
*************************************************************************************/    


PID fuzzy(float e,float ec)  
{  
  
     float etemp, ectemp;  
     float eLefttemp, ecLefttemp;  
     float eRighttemp, ecRighttemp;  
  
     int eLeftIndex,ecLeftIndex;  
     int eRightIndex,ecRightIndex;  
     PID   fuzzy_PID;  
     etemp = e > 3.0 ? 0.0 : (e < - 3.0 ? 0.0 : (e >= 0.0 ? (e >= 2.0 ? 2.5: (e >= 1.0 ? 1.5 : 0.5)) : (e >= -1.0 ? -0.5 : (e >= -2.0 ? -1.5 : (e >= -3.0 ? -2.5 : 0.0) ))));  
  
     eLeftIndex = (int)e;  
     eRightIndex = eLeftIndex;  
     eLeftIndex = (int)((etemp-0.5) + 3);        //[-3,3] -> [0,6]  
     eRightIndex = (int)((etemp+0.5) + 3);  
  
     eLefttemp =etemp == 0.0 ? 0.0:((etemp+0.5)-e);  
     eRighttemp=etemp == 0.0 ? 0.0:( e-(etemp-0.5));  
  
     ectemp = ec > 3.0 ? 0.0 : (ec < - 3.0 ? 0.0 : (ec >= 0.0 ? (ec >= 2.0 ? 2.5: (ec >= 1.0 ? 1.5 : 0.5)) : (ec >= -1.0 ? -0.5 : (ec >= -2.0 ? -1.5 : (ec >= -3.0 ? -2.5 : 0.0) ))));  
  
     ecLeftIndex = (int)((ectemp-0.5) + 3);        //[-3,3] -> [0,6]  
     ecRightIndex = (int)((ectemp+0.5) + 3);  
  
     ecLefttemp =ectemp == 0.0 ? 0.0:((ectemp+0.5)-ec);  
     ecRighttemp=ectemp == 0.0 ? 0.0:( ec-(ectemp-0.5));  
  
/*************************************��ģ��*************************************/  

  
    fuzzy_PID.Kp = (eLefttemp * ecLefttemp *  fuzzyRuleKp[ecLeftIndex][eLeftIndex]                      
                    + eLefttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eLeftIndex]  
                    + eRighttemp * ecLefttemp * fuzzyRuleKp[ecLeftIndex][eRightIndex]  
                    + eRighttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eRightIndex]);  
  
    fuzzy_PID.Ki =   (eLefttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eLeftIndex]  
                    + eLefttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eLeftIndex]  
                    + eRighttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eRightIndex]  
                    + eRighttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eRightIndex]);  
  
    fuzzy_PID.Kd = (eLefttemp * ecLefttemp * fuzzyRuleKd[ecLeftIndex][eLeftIndex]  
                    + eLefttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eLeftIndex]  
                    + eRighttemp * ecLefttemp * fuzzyRuleKd[ecLeftIndex][eRightIndex]  
                    + eRighttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eRightIndex]);  
return fuzzy_PID;  
  
}  
  
/************ PID���� ************/ 
void PID_Calc(PID_Regulator_t *pid)
{
	static float SUM=0;
    float dError, Error;
	PID OUT = {0,0,0};
	
	Error = pid->ref - pid->fdb;  //
	SUM += Error;               //������
	
	if(SUM > 160) {SUM = 160;}  //�����޷�
	if(SUM < -160){SUM = -160;}
	
	dError = pid->err[0] - pid->err[1];//΢����
	pid->err[1] = pid->err[0];
	pid->err[0] = Error;
	
	OUT = fuzzy(Error/100, dError/100); //+OUT.Kp*10 +OUT.Ki +OUT.Kd//ģ��������õ�������
	
	pid->output = (pid->kp)*Error + (pid->ki) + (pid->kd)*dError; //����pid
	
	if(pid->output > 6000)    //���pwm�޷�
		pid->output = 6000;
	else if(pid->output < -6000)
		pid->output = -6000;
}

