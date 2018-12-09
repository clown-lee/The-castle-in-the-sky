#ifndef _PID_H_
#define _PID_H_


#define NL   -3
#define NM	 -2
#define NS	 -1
#define ZE	 0
#define PS	 1
#define PM	 2
#define PL	 3


typedef struct PID_Regulator_t
{
	float ref;
	float fdb;
	float err[2];//error_now  error_last error_inter
	float kp;
	float ki;
	float kd;
	float output;
	//void (*Calc)(struct PID_Regulator_t *pid);//������һ������ָ�룬û�з���ֵ����������Calc���βε�������
                                      //PID_Regulator_t�ṹ�������ָ�� ��������Ҫ�����ָ����и�ֵ����ʾ���ָ��ָ���Ǹ����� 
}PID_Regulator_t;

extern PID_Regulator_t Cir1_PID, Cir2_PID;
void PID_Calc(PID_Regulator_t *pid);

static const float fuzzyRuleKp[7][7]={  //Kpģ�������
	PL,	PL,	PM,	PM,	PS,	PS,	ZE,
	PL,	PL,	PM,	PM,	PS,	ZE,	ZE,
	PM,	PM,	PM,	PS,	ZE,	NS,	NM,
	PM,	PS,	PS,	ZE,	NS,	NM,	NM,
	PS,	PS,	ZE,	NS,	NS,	NM,	NM,
	ZE,	ZE,	NS,	NM,	NM,	NM,	NL,
	ZE,	NS,	NS,	NM,	NM,	NL,	NL
};

static const float fuzzyRuleKi[7][7]={  //Kiģ�������
	NL,	NL,	NL,	NM,	NM,	ZE,	ZE,
	NL,	NL,	NM,	NM,	NS,	ZE,	ZE,
	NM,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	ZE,	PS,	PS,	PM,
	NS,	NS,	ZE,	PS,	PS,	PM,	PM,
	ZE,	ZE,	PS,	PM,	PM,	PL,	PL,
	ZE,	ZE,	PS,	PM,	PL,	PL,	PL
};

static const float fuzzyRuleKd[7][7]={  //Kdģ�������
	PS,	PS,	ZE,	ZE,	ZE,	PL,	PL,
	NS,	NS,	NS,	NS,	ZE,	NS,	PM,
	NL,	NL,	NM,	NS,	ZE,	PS,	PM,
	NL,	NM,	NM,	NS,	ZE,	PS,	PM,
	NL,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	NS,	ZE,	PS,	PS,
	PS,	ZE,	ZE,	ZE,	ZE,	PL,	PL
};

typedef struct{
	float Kp;
	float Ki;
	float Kd;
}PID;



#endif
