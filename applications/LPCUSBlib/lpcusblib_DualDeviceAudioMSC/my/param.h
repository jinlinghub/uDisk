#include "Common.h"

	enum	JL_USBSelect {
		USBCommModePC,								//PCͨ��ģʽ;
		USBCommModePhone,							//Phoneͨ��ģʽ
	};
	
	enum	JL_PhoneMode {
		USBPhoneIOS,									//�����ֻ�ΪIOSϵͳ
		USBPhoneAndroid,							//�����ֻ�ΪAndroidϵͳ
	};
	
	typedef struct{
		uint8_t jl_USBSelect;
		uint8_t jl_PhoneMode;
	}_uDiskFlag;
	
	 extern _uDiskFlag uDiskFlag;