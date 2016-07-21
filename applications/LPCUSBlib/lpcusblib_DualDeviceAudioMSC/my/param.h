#include "Common.h"

	enum	JL_USBSelect {
		USBCommModePC,								//PC通信模式;
		USBCommModePhone,							//Phone通信模式
	};
	
	enum	JL_PhoneMode {
		USBPhoneIOS,									//插入手机为IOS系统
		USBPhoneAndroid,							//插入手机为Android系统
	};
	
	typedef struct{
		uint8_t jl_USBSelect;
		uint8_t jl_PhoneMode;
	}_uDiskFlag;
	
	 extern _uDiskFlag uDiskFlag;