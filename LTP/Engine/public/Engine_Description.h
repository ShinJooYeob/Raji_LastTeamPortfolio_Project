#pragma once

namespace Engine {


	typedef struct tagArg
	{
		void* pArg;
		CRITICAL_SECTION*	CriSec;
		_bool* IsClientQuit;

	}THREADARG;

}
