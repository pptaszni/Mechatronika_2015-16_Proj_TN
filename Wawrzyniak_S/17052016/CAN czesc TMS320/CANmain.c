#include "DSP281x_Device.h"

void Gpio_select(void);
void InitSysCtrl(void);
void InitECan(void);
//void SpeedUpRevA(void); dla starszych wersji TMS320F2812
void InitSystem(void);
void InitPieCtrl(void);
void InitPieVectTable(void);
interrupt void przerwanie_CAN0(void);
interrupt void przerwanie_CAN1(void); // prototyp przerwania od odbioru wiadomosci

interrupt void cpu_timer0_isr(void);//przerwanie z procesora do za³aczania swiatel


void migaj3hz(void);
void migaj1p5hz(void);
void wylacz(void);
Uint32 MBXnr;

void main(void)
{
	//Uint16 j;
	/*Initialize System Control registers, PLL< watchdog, clocks to default state//this f-ction is found in the DSP281x_SysCtrl.c*/
	InitSysCtrl();

	/*Disable and clear all CPU interrupts*/
	DINT;
	IER = 0x0000;
	IFR = 0x0000;

	Gpio_select();	// Setup the GPIO Multiplex Registers (port GPIOB0 na wyjscie - do niego przekaznik swiatel, tymczasowo dioda LED)

	/*Initialize PIE Control Registers To Deafult State //this f-ction is found in the DSP281x_PieCrl.c*/
	InitPieCtrl();

	/*Initialize the PIE vector table to a known state*/
	InitPieVectTable();

	EALLOW;	// This is needed to write to EALLOW protected registers
	PieVectTable.TINT0=&cpu_timer0_isr;
	//PieVectTable.ECAN0INTA = &przerwanie_CAN0; //linia 0 na razie niewykorzystywana
	PieVectTable.ECAN1INTA = &przerwanie_CAN1; //obsluga przerwan sprawia problemy, ale bedzie wykorzystana
	EDIS;   // This is needed to disable write to EALLOW protected registers


	InitCpuTimers();

	ConfigCpuTimer(&CpuTimer0, 150, 50000); //CPU zegar 150MHz, czas po jakim pojawia sie przerwanie - 50ms

	PieCtrlRegs.PIEIER1.bit.INTx7 = 1; //uruchamia przerwanie dla CPU-Timer 0

	IER |= M_INT9; //Enable INT9 (0x0100)
	IER |= M_INT1; //Enable INT1 (0x0001)

	EINT;   // Enable Global interrupt INTM (by clearing INTM bit)
	ERTM;	// Enable Global realtime interrupt DBGM

	CpuTimer0Regs.TCR.bit.TSS = 0 ;  // Reload all counter register with period value

	/*Initialize ECAN module*/
	InitECan();


	//MessageReceiveCount = 0; //nie uzywane
	//ErrorCount = 0; // nie uzywane


	static struct ECAN_REGS ECanaRegsShadow; // rejestr "widmo" - nie mozna dokonywac zapisu do niektorych rejestrow procesora bezposrednio - metoda zalecana przez TI to shadow register

	/*poczatkowe ustalenie wartosci (wylaczone) skrzynek pocztowych 0,1,2  */

	ECanaRegsShadow.CANME.all = ECanaRegs.CANME.all;

	ECanaRegsShadow.CANME.bit.ME0=0;
	ECanaRegsShadow.CANME.bit.ME1=0;
	ECanaRegsShadow.CANME.bit.ME2=0;

	ECanaRegs.CANME.all = ECanaRegsShadow.CANME.all;

	struct ECAN_MBOXES ECanaMboxesShadow;

	/* ustawienie formatu dla skrzynek 0,1,2, identyfikator rozszerzony, maski wlaczone - okazuje sie ze nie trzeba tu korzystac z shadow registers  */

	ECanaMboxesShadow.MBOX0.MSGID.all = ECanaMboxes.MBOX0.MSGID.all;

	ECanaMboxesShadow.MBOX0.MSGID.bit.AME=1; //acceptance mask - wlaczone
	ECanaMboxesShadow.MBOX0.MSGID.bit.IDE=1; //wlaczony rozszerzony identyfikator
	ECanaMboxesShadow.MBOX0.MSGID.bit.STDMSGID=0x000; // 11 bitów STD identifier
	ECanaMboxesShadow.MBOX0.MSGID.bit.EXTMSGID_L=0x0000; //16 bitów EXT identifier
	ECanaMboxesShadow.MBOX0.MSGID.bit.EXTMSGID_H=0x0; //2 bity EXT identifier

	ECanaMboxes.MBOX0.MSGID.all = ECanaMboxesShadow.MBOX0.MSGID.all;

	ECanaMboxesShadow.MBOX0.MSGID.all = ECanaMboxes.MBOX0.MSGID.all;

	ECanaMboxesShadow.MBOX1.MSGID.bit.AME=1;//acceptance mask - wlaczone
	ECanaMboxesShadow.MBOX1.MSGID.bit.IDE=1; //wlaczony rozszerzony identyfikator
	ECanaMboxesShadow.MBOX1.MSGID.bit.STDMSGID=0x000; // 11 bitów STD identifier
	ECanaMboxesShadow.MBOX1.MSGID.bit.EXTMSGID_L=0x0001; //16 bitów EXT identifier
	ECanaMboxesShadow.MBOX1.MSGID.bit.EXTMSGID_H=0x0; //2 bity EXT identifier

	ECanaMboxes.MBOX1.MSGID.all = ECanaMboxesShadow.MBOX1.MSGID.all;

	ECanaMboxesShadow.MBOX2.MSGID.all = ECanaMboxes.MBOX2.MSGID.all;

	ECanaMboxesShadow.MBOX2.MSGID.bit.AME=1;//acceptance mask - wlaczone
	ECanaMboxesShadow.MBOX2.MSGID.bit.IDE=1; //wlaczony rozszerzony identyfikator
	ECanaMboxesShadow.MBOX2.MSGID.bit.STDMSGID=0x000; // 11 bitów STD identifier
	ECanaMboxesShadow.MBOX2.MSGID.bit.EXTMSGID_L=0x0002; //16 bitów EXT identifier
	ECanaMboxesShadow.MBOX2.MSGID.bit.EXTMSGID_H=0x0; //2 bity EXT identifier

	ECanaMboxes.MBOX2.MSGID.all = ECanaMboxesShadow.MBOX2.MSGID.all;

	/* Ustawienie lokalnych masek akceptacji identyfikatora dla poszczegolnych skrzynek */

	struct LAM_REGS ECanaLamShadow;

	ECanaLamShadow.LAM0.all = ECanaLAMRegs.LAM0.all;

	ECanaLamShadow.LAM0.bit.LAMI=0; // ustawianie LAMI zgodnie z rejestrem MSGID.IDE
	ECanaLamShadow.LAM0.bit.LAM_L=0xFFFE;  //Filtry w³¹czone, Ustawienie filtrów na wy³¹czone: 0xFFFF
	ECanaLamShadow.LAM0.bit.LAM_H=0x1FFF;

	ECanaLAMRegs.LAM0.all = ECanaLamShadow.LAM0.all;

	ECanaLamShadow.LAM1.all = ECanaLAMRegs.LAM1.all;

	ECanaLamShadow.LAM1.bit.LAMI=0;
	ECanaLamShadow.LAM1.bit.LAM_L=0xFFFE;  //Filtry w³¹czone, Ustawienie filtrów na wy³¹czone: 0xFFFF
	ECanaLamShadow.LAM1.bit.LAM_H=0x1FFF;

	ECanaLAMRegs.LAM1.all = ECanaLamShadow.LAM1.all;


	ECanaLamShadow.LAM2.all = ECanaLAMRegs.LAM2.all;

	ECanaLamShadow.LAM2.bit.LAMI=0;
	ECanaLamShadow.LAM2.bit.LAM_L=0xFFFC;  //Filtry w³¹czone, Ustawienie filtrów na wy³¹czone: 0xFFFF
	ECanaLamShadow.LAM2.bit.LAM_H=0x1FFF;

	ECanaLAMRegs.LAM2.all = ECanaLamShadow.LAM2.all;

	/* Ustawienie kierunku dzialania skrzynek - w tym wypadku na odbiorcze */

	ECanaRegsShadow.CANMD.all = ECanaRegs.CANMD.all;

	ECanaRegsShadow.CANMD.bit.MD0 = 1;
	ECanaRegsShadow.CANMD.bit.MD1 = 1;
	ECanaRegsShadow.CANMD.bit.MD2 = 1;

	ECanaRegs.CANMD.all = ECanaRegsShadow.CANMD.all;

	/*ochrona nadpisania wiadomosci - jesli wiadomosc nie jest jeszcze odczytana nie zostanie nadpisana, moze grozic "zgubieniem" informacji jesli nie bedzie wolnych skrzynek*/

	ECanaRegsShadow.CANOPC.all = ECanaRegs.CANOPC.all;

	ECanaRegsShadow.CANOPC.bit.OPC0 = 1;//ECanaRegs.CANOPC.bit.OPC0;
	ECanaRegsShadow.CANOPC.bit.OPC1 = 1;//ECanaRegs.CANOPC.bit.OPC1;
	ECanaRegsShadow.CANOPC.bit.OPC2 = 1;//ECanaRegs.CANOPC.bit.OPC2;

	ECanaRegs.CANOPC.all = ECanaRegsShadow.CANOPC.all;

	/* Ostateczne wlaczenie skrzynek 1,2,3 */

	ECanaRegsShadow.CANME.all = ECanaRegs.CANME.all;

	ECanaRegsShadow.CANME.bit.ME0=1;
	ECanaRegsShadow.CANME.bit.ME1=1;
	ECanaRegsShadow.CANME.bit.ME2=1;

	ECanaRegs.CANME.all |= ECanaRegsShadow.CANME.all;

	//konfiguracja przerwan //od skrzynek MBOX 1, 2, 3, na odbiór//

    /* ktora linia - 0 czy 1*/
	ECanaRegsShadow.CANMIL.all = ECanaRegs.CANMIL.all;

	ECanaRegsShadow.CANMIL.bit.MIL0 = 1; // mbox 0 przerwanie na linii 1
	ECanaRegsShadow.CANMIL.bit.MIL1 = 1; // mbox 1 przerwanie na linii 1
	ECanaRegsShadow.CANMIL.bit.MIL2 = 1; // mbox 2 przerwanie na linii 1

	ECanaRegs.CANMIL.all = ECanaRegsShadow.CANMIL.all;

	/* przerwanie od ktorej skrzynki jest aktywne */

	ECanaRegsShadow.CANMIM.all = ECanaRegs.CANMIM.all;

	ECanaRegsShadow.CANMIM.bit.MIM0 = 1; // mbox 0 przerwanie aktywne
	ECanaRegsShadow.CANMIM.bit.MIM1 = 0; // mbox 1 przerwanie aktywne-1 nieaktywne-0
	ECanaRegsShadow.CANMIM.bit.MIM2 = 0; // mbox 2 przerwanie aktywne-1 nieaktywne-0

	ECanaRegs.CANMIM.all = ECanaRegsShadow.CANMIM.all;

	/*zalaczenie zalecanych przez TI przerwan*/

	ECanaRegsShadow.CANGIM.all = ECanaRegs.CANGIM.all;

	ECanaRegsShadow.CANGIM.bit.AAIM=1; //przerwanie systemowe Abort Acknowledge Interrupt Mask.
	ECanaRegsShadow.CANGIM.bit.WDIM=1; //przerwanie systemowe Write denied interrupt mask
	ECanaRegsShadow.CANGIM.bit.WUIM=1; //przerwanie systemowe Wake-up interrupt mask
	ECanaRegsShadow.CANGIM.bit.BOIM=1; //przerwanie systemowe Bus-off interrupt mask
	ECanaRegsShadow.CANGIM.bit.EPIM=1; //przerwanie systemowe Error-passive interrupt mask
	ECanaRegsShadow.CANGIM.bit.WLIM=1; //przerwanie systemowe Warning level interrupt mask
	ECanaRegsShadow.CANGIM.bit.GIL=0; //przerwania systemowe na liniê 0
	ECanaRegsShadow.CANGIM.bit.I0EN=1; //globalne za³¹czenie linii 0 przerwañ - przerwania zgodnie z maskami
	ECanaRegsShadow.CANGIM.bit.I1EN=1; //globalne za³¹czenie linii 1 przerwañ - przerwania zgodnie z maskami

	ECanaRegs.CANGIM.all=ECanaRegsShadow.CANGIM.all;

	//koniec konfiguracji przerwan

	/*konfiguracje przeprowadzono zgodnie z dokumentem SPRU074F "TMS320x281x Enhanced Controller Area Network (eCAN)
Reference Guide" */



	for(;;)
	{
		asm ("NOP");

		if(ECanaRegs.CANRMP.bit.RMP0 == 1 ) { //jesli skrzynka 0 otrzyma wiadomosc (a otrzyma wiadomosc o najwyzszym priorytecie - najnizszej wartosi identyfikatora) resetuje bit ECanaRegs.CANRMP.bit.RMP0 i wykonuje sekwencje 3hz migania
			ECanaRegs.CANRMP.bit.RMP0=1;
				migaj3hz();
			}


		if(ECanaRegs.CANRMP.bit.RMP1 == 1 ) { // jesli przychodzi wiadomosc o wyzszym priorytecie w obecnym ksztalcie programu, nalezy zresetowac bit, aby powiadomic CPU o odczycie wiadomosci


						ECanaRegs.CANRMP.bit.RMP1=1;
						//wylacz(); //funkcja zawarta obecnie w migaj3hz
		}

		//if(ECanaRegs.CANRMP.bit.RMP2 == 1 ) { //obecnie wysylane sa tylko wiadomosci o 2 roznych id

		//				ECanaRegs.CANRMP.bit.RMP2=1;
						//wylacz();

		//}




		//migaj3hz();

		//GpioDataRegs.GPBDAT.bit.GPIOB0 = 0;


	}

}
/*interrupt void przerwanie_CAN0(void) //na razie przerwañ od bledow nie implementowano
{
	//obs³uga przerwan od systemu
//wyczysc powod przerwania! -> CANGIF0 = 1
}
*/


interrupt void przerwanie_CAN1(void) //obsluga przerwan sprawila problemy, zdecydowano sie na debugowanie systemu polaczonego w siec w petlach programowych
{
	//obs³uga przerwan od skrzynek pocztowych
	struct ECAN_REGS ECanaRegsShadow;
	ECanaRegsShadow.CANGIF1.all = ECanaRegs.CANGIF1.all;
	if(ECanaRegsShadow.CANGIF1.bit.GMIF1==1){
		Uint32 Reg;


		Reg=ECanaRegsShadow.CANGIF1.all;
		Reg=(Reg<<27);
		MBXnr=(Reg>>27);

		if(MBXnr==0){
		ECanaRegsShadow.CANRMP.bit.RMP0 = 1;
		//GpioDataRegs.GPBDAT.bit.GPIOB0 = 0;
		migaj3hz();//funkcja_zapal_swiatla3Hz;
		}
		if(MBXnr==1){
		ECanaRegsShadow.CANRMP.bit.RMP0 = 1;
		//migaj1p5hz();//funkcja_zapal_swiatla1Hz;
		}
		if(MBXnr==2){
		ECanaRegsShadow.CANRMP.bit.RMP0 = 1;
		//wylacz();//funkcja_zgas_swiatla;
		}

		ECanaRegs.CANRMP.all=ECanaRegsShadow.CANRMP.all; //wyzerowanie flagi odpowiedzialnej za przerwanie od konkretnej skrzynki


		PieCtrlRegs.PIEACK.all = PIEACK_GROUP9; // Enables PIE to drive a pulse into the CPU

	}

}

void migaj3hz(void){
	CpuTimer0.InterruptCount=0;
	while(ECanaRegs.CANRMP.bit.RMP1!=1){ //warunek - jesli przyjdzie wiadomosc o id "wylacz" - przestan migac
	    	GpioDataRegs.GPBDAT.bit.GPIOB0 = 1;
	    	 while(CpuTimer0.InterruptCount < 2); //czekaj 100 ms
	    		 CpuTimer0.InterruptCount=0;

	    	GpioDataRegs.GPBDAT.bit.GPIOB0 = 0;
	    	while(CpuTimer0.InterruptCount < 2); //czekaj 100ms
	    		    		 CpuTimer0.InterruptCount=0;
	}

}
void migaj1p5hz(void){
	CpuTimer0.InterruptCount=0;
	while(MBXnr==1 && CpuTimer0.InterruptCount!=0x12C){
	GpioDataRegs.GPBDAT.bit.GPIOB0 = 1;
	while(CpuTimer0.InterruptCount < 12); //czekaj 600 ms
	 CpuTimer0.InterruptCount=0;

	GpioDataRegs.GPBDAT.bit.GPIOB0 = 0;
	while(CpuTimer0.InterruptCount < 12); //czekaj 600ms
	    		 CpuTimer0.InterruptCount=0;
	}
}
void wylacz(void){
	//CpuTimer0.InterruptCount=0;
	//while(MBXnr==2 && CpuTimer0.InterruptCount!=0x12C){
	GpioDataRegs.GPBDAT.bit.GPIOB0 = 0;
	//}
}

interrupt void cpu_timer0_isr(void)
{
	CpuTimer0.InterruptCount++;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

void Gpio_select(void)
{
	EALLOW;
	GpioMuxRegs.GPAMUX.all = 0x0;	// all GPIO port Pin's to I/O oprócz GPIOFMUX - obs³uga CAN
    GpioMuxRegs.GPBMUX.all = 0x0;
    GpioMuxRegs.GPDMUX.all = 0x0;
    //GpioMuxRegs.GPFMUX.all = 0x0;
    GpioMuxRegs.GPEMUX.all = 0x0;
    GpioMuxRegs.GPGMUX.all = 0x0;

    GpioMuxRegs.GPADIR.all = 0x0;	// GPIO PORT  as input
    GpioMuxRegs.GPBDIR.all = 0x00FF;	// GPIO Port B15-B8 input , B7-B0 output - pod³¹czono przekaŸnik kierunkowskazów pod port B0
    GpioMuxRegs.GPDDIR.all = 0x0;	// GPIO PORT  as input
    GpioMuxRegs.GPEDIR.all = 0x0;	// GPIO PORT  as input
    //GpioMuxRegs.GPFDIR.all = 0x0;	// GPIO PORT  as input
    GpioMuxRegs.GPGDIR.all = 0x0;	// GPIO PORT  as input

    GpioMuxRegs.GPAQUAL.all = 0x0;	// Set GPIO input qualifier values to zero
    GpioMuxRegs.GPBQUAL.all = 0x0;
    GpioMuxRegs.GPDQUAL.all = 0x0;
    GpioMuxRegs.GPEQUAL.all = 0x0;
    EDIS;
}



