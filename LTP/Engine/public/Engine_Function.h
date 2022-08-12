#pragma once
#pragma once

namespace Engine
{
	class CTagFinder
	{
	public:
		CTagFinder(const _tchar* pTag) : m_pTag(pTag) {}
		~CTagFinder() = default;

	public:
		template <typename T>
		bool operator () (T& Pair)
		{
			if (0 == lstrcmp(Pair.first, m_pTag))
				return true;
			return false;
		}

	private:
		const _tchar*			m_pTag = nullptr;
	};

	class CTagStringFinder
	{
	public:
		CTagStringFinder(const _tchar * pTag) : m_pTag(wstring(pTag)) {}
		~CTagStringFinder() = default;

	public:
		template <typename T>
		bool operator () (T& Pair)
		{
			if (Pair.first == m_pTag)
				return true;
			return false;
		}

	private:
		const wstring			m_pTag = nullptr;
	};

	template <typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete[] pPointer;
			pPointer = nullptr;
		}
	}

	template <typename T>
	_ulong Safe_AddRef(T& pInstance)
	{
		_ulong		dwRefCnt = 0;

		if (nullptr != pInstance)
			dwRefCnt = pInstance->AddRef();

		return dwRefCnt;
	}

	template <typename T>
	_ulong Safe_Release(T& pInstance)
	{
		_ulong		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();
			pInstance = nullptr;
		}

		return dwRefCnt;
	}

	template <typename LIST>
	void Safe_Releasee_List(LIST& list)
	{

		for (auto& val : list)
		{
			Safe_Release(val);
		}
	}

	template <typename LIST>
	void Safe_Delete_List(LIST& list)
	{

		for (auto& val: list)
		{
			Safe_Delete(val);
		}
	}



	template <typename MAP>
	void Safe_Delete_Map(MAP& map)
	{

		for (auto& pair : map)
		{
			Safe_Delete(pair.second);
		}
	}





	class CHelperClass
	{
	public:
		// 파일입출력에서 쓰던거
		static vector<wstring> WStringSplit(wstring input, wchar_t de)
		{
			vector<wstring> answer;
			wstringstream ss(input);
			wstring temp;

			while (getline(ss, temp, de))
			{
				answer.push_back(temp);
			}
			return answer;
		}

		static vector<string> StringSplit(string input, char de)
		{
			vector<string> answer;
			stringstream ss(input);
			string temp;

			while (getline(ss, temp, de))
			{
				answer.push_back(temp);
			}
			return answer;
		}

		template <typename Container>
		static bool in_quote(const Container& cont, const std::string& s)
		{
			return std::search(cont.begin(), cont.end(), s.begin(), s.end()) !=
				cont.end();
		}


		static const string Convert_Wstr2str(wstring& wstr)
		{
			string str;
			str.assign(wstr.begin(), wstr.end());
			return str;
		}

		static const wstring Convert_str2wstr(string& str)
		{
			wstring wstr;
			wstr.assign(str.begin(), str.end());
			return wstr;
		}
	};

}